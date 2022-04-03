//
// Created by ZONT_ on 31.03.2022.
//

#include "web.hpp"

WebServer web::server(80);
bool web::ledState = false;

void web::setup() {
    using namespace std;
    setLedState(true);

    WiFi.mode(WIFI_STA);
    WiFi.begin("aNat", "9037140615a");

    cout << "Connecting to WiFi..";
    while (WiFi.status() != WL_CONNECTED) {
        delay(300);
        setLedState(!ledState);
        cout << ".";
    }
    cout << endl;
    cout << "Connected. IP address: ";
    cout << WiFi.localIP().toString().c_str() << endl;

    if (MDNS.begin("zont-esp32"))
        cout << "MDNS responder started" << endl;

    server.onNotFound(notFound);
    server.on("/", index);
    server.on("/index", index);
    server.on("/styles.css", styles);

    server.on("/getState", sendState);
    server.on("/toggle", toggle);

    server.begin();
    MDNS.addService("http", "tcp", 80);

    pinMode(LED_BUILTIN, OUTPUT);
    setLedState(false);
}

void web::setLedState(bool state) {
    digitalWrite(LED_BUILTIN, state ? LED_OUTPUT_ON : LED_OUTPUT_OFF);
    ledState = state;
}

void web::sendState() {
    server.send(200, "text/plain", ledState ? "1" : "0");
}

void web::toggle() {
    setLedState(!ledState);
    sendState();
}

void web::loop() {
    server.handleClient();
}

void web::notFound() {
    String message = "404 Not Found\n\n";
    message += "URI: ";
    message += server.uri();
    message += "\nMethod: ";
    message += (server.method() == HTTP_GET) ? "GET" : "POST";
    message += "\nArguments: ";
    message += server.args();
    message += "\n";
    for (int i = 0; i < server.args(); i++)
        message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
    server.send(404, "text/plain", message);
}

void web::index() {
    std::string response = R"(<!DOCTYPE HTML>
<html lang="ru">
<head>
    <meta charset="UTF-8">
    <title>ZONT ESP</title>
    <link rel="stylesheet" href="styles.css">
</head>
<body>
<noscript>
    <div style="text-align: center">
        <h1>Соре, но ваш браузер - додик.<br/>Сожалеем об этом.</h1>
    </div>
</noscript>
<div style="text-align: center;"><h1>Верхний текст</h1></div>
<br/>
<script>
    async function updAllButtons(toggle) {
        const xmlHttp = new XMLHttpRequest();
        xmlHttp.open( "GET", toggle ? '/toggle' : '/getState', false );
        xmlHttp.send( null );
        let responseText = xmlHttp.responseText;
        console.log(`Response: '${responseText}'`)
        const on = responseText === '1';

        for (let but of document.getElementsByClassName('button_steamlike')) {
            if (on) but.classList.add('active')
            else but.classList.remove('active')
        }
    }
</script>
<div style="text-align: center;">
    <p style="font-size:48pt">Текст заголовка</p>
    <p style="font-size:16pt">Текст подзаголовка</p>
    <p style="font-size:11pt">Дополнительный текст</p>
    <div class="button_steamlike" style="text-align: center; width: 120px; margin: auto">
        <a onclick="updAllButtons(true);"><span>Кнопка</span></a>
    </div>
</div>
<script>
    updAllButtons(false)
</script>
</body>
</html>)";
    server.send(200, "text/html", response.c_str());
}

void web::styles() {
    std::string response = R"(
body {
    background: #1b2838;
    text-align: left;
    color: #8F98A0;
    font-size: 14px;
    margin: 0;
    padding: 0;
}

.btn_connect {
    display: inline-block;
    position: relative;
    height: 21px;
    line-height: 24px;
    margin-right: 3px;
    background-color: #5c7e10;
}
.btn_connect_content {
    display: inline-block;
    padding-left: 35px;
    padding-right: 9px;
    background-position: 10px 5px;
    background-image: url('http://rgsv.xyz/pic/launch-white-18dp.svg');
    background-repeat:	no-repeat;
    text-decoration: none;
    color: #e5e4dc;
    background-color: #5c7e10;
    font-weight: normal;
}
.btn_connect_content:hover {
    text-decoration: none;
    color: #ffffff;
}
.button_steamlike {
    --main-text-color: #dbe2e6;
    --main-light-blue-background: #93b3c8;
    --main-text-on-light-blue: rgb(209, 209, 209);
    --main-top-image-bg: #1f2126;
    --main-editor-bg-color: #363a43;
    --main-editor-text-color: #e6e7e8;
    --main-editor-input-bg-color: #30333b;
    --main-editor-section-title-color: #a3a3a3;
    font-size: 14px;
    color: #969696;
    font-family: "Motiva Sans", Arial, Helvetica, sans-serif;
    text-align: left;
}
.button_steamlike a {
    cursor: pointer;
    text-decoration: none !important;
    color: #ffffff !important;
    background: linear-gradient( to bottom, #9500ff 5%, #7800cd 95%);
    position: relative;

    display: block;
    border-radius: 3px;
    padding: 1px 15px;
    font-size: 15px;
    line-height: 30px;
}
.button_steamlike a:hover {
    color: #000000 !important;
    background: linear-gradient( to bottom, #c57fff 5%, #a056d6 95%);
}

.button_steamlike.active a {
    color: #ffffff !important;
    background: linear-gradient( to bottom, #059005 5%, #007000 95%);
}
.button_steamlike.active a:hover {
    color: #000000 !important;
    background: linear-gradient( to bottom, #05f005 5%, #00d000 95%);
}
)";
    server.send(200, "text/css", response.c_str());
}