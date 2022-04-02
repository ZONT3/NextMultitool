#pragma once

#include "Arduino.h"

#include <iostream>
#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>

namespace web {
    extern WebServer server;
    extern bool ledState;

    void setup();
    void loop();

    void index();
    void styles();
    void notFound();

    void toggle();
    void sendState();
    void setLedState(bool state);
}
