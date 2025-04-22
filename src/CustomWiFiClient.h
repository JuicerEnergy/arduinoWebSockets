/**
 * Juicer Energy Inc. changed
 */
// Wifi Client custom to fix issue with connected() function
#pragma once

#include <WiFiClient.h>

class CustomWiFiClient: public WiFiClient{
    public :
    CustomWiFiClient(int fd):WiFiClient(fd){}
    uint8_t connected();    //JUCR Overridden
};