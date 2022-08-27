#pragma once

#include "Arduino.h"

#include <iostream>
#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>

// On MH-ET ESP32 DevKIT LOW == Led ON, HIGH == Led OFF
#define LED_OUTPUT_ON LOW
#define LED_OUTPUT_OFF HIGH


namespace web {
    extern WebServer server;
    extern bool ledState;

    void setup();
    void loop();

    void index();
    void styles();
    void notFound();

    void ctxToggle();
    void ctxEmulatePush();

    void sendState();
    void setLedState(bool state);
}
