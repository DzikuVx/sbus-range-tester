#include "oled_display.h"
#include "Arduino.h"

OledDisplay::OledDisplay(SSD1306 *display) {
    _display = display;
}

void OledDisplay::init() {
    _display->init();
    _display->flipScreenVertically();
    _display->setFont(ArialMT_Plain_10);
}

void OledDisplay::loop() {
    page();
}

void OledDisplay::setPage(uint8_t page) {
    _page = page;
}

void OledDisplay::page() {

    static uint32_t lastUpdate = 0;

    //Do not allow for OLED to be updated too often
    if (lastUpdate > 0 && millis() - lastUpdate < 200 && _forceDisplay == false) {
        return;
    }

    _forceDisplay = false;

    switch (_page) {
        
        case OLED_PAGE_DATA:
            renderPageData();
            break;
    }

    lastUpdate = millis();
}

void OledDisplay::renderPageData() {
    _display->clear();

    _display->setFont(ArialMT_Plain_10);

    _display->drawString(0, 0, "Sat: " + String(gps.satellites.value()));
    _display->drawString(64, 0, "Dst: " + String(gpsState.dist, 1));
    _display->drawString(0, 54, "FS Counter: " + String(failsafeEventCounter));


    _display->setFont(ArialMT_Plain_16);
    int rssi = map(constrain(rcChannels[13], 1000, 2000), 1000, 2000, 0, 100);

    _display->drawString(0, 18, "RSSI: " + String(rssi));
    
    if (sbusInput.isFailsafe()) {
        _display->drawString(0, 36, "!!Failsafe!!");
    } 

    yield();

    _display->display();
}