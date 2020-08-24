#include <TinyGPS++.h>
#include <HardwareSerial.h>
#include "SSD1306.h"
#include <QmuTactile.h>
#include <SPI.h>
#include "oled_display.h"
#include "sbus.h"
#include "soc/rtc_wdt.h"
#include "EEPROM.h"
#include "types.h"
#include "esp_task_wdt.h"

#define PIN_BUTTON_L 4
#define PIN_BUTTON_R 0

TinyGPSPlus gps;
HardwareSerial SerialGPS(1);
HardwareSerial SerialSBUS(2);
SSD1306  display(0x3c, 21, 22);
OledDisplay oledDisplay(&display);

SbusInput sbusInput(SerialSBUS);

QmuTactile buttonL(PIN_BUTTON_L);
QmuTactile buttonR(PIN_BUTTON_R);

#define EEPROM_SIZE 128

#define TASK_SERIAL_RATE 1000
uint32_t nextSerialTaskTs;

int16_t rcChannels[16] = {0};

GpsDataState_t gpsState = {};

void setRcChannel_wrapper(uint8_t channel, int value, int offset) {
    rcChannels[channel] = value + offset;
}

void setup()
{
    Serial.begin(115200);
	SerialGPS.begin(9600, SERIAL_8N1, 13, 15);

    sbusInput.setRcChannelCallback = setRcChannel_wrapper;

    sbusInput.start();

    buttonL.start();
    buttonR.start();

    oledDisplay.init();
    oledDisplay.setPage(OLED_PAGE_DATA);

    while (!EEPROM.begin(EEPROM_SIZE)) {
        true;
    }

    long readValue; 
    EEPROM_readAnything(0, readValue);
    gpsState.originLat = (double)readValue / 10000000;

    EEPROM_readAnything(4, readValue);
    gpsState.originLon = (double)readValue / 10000000;

    EEPROM_readAnything(8, readValue);
    gpsState.originAlt = (double)readValue / 10000000;

}

template <class T> int EEPROM_writeAnything(int ee, const T& value)
{
   const byte* p = (const byte*)(const void*)&value;
   int i;
   for (i = 0; i < sizeof(value); i++)
       EEPROM.write(ee++, *p++);
   return i;
}

template <class T> int EEPROM_readAnything(int ee, T& value)
{
   byte* p = (byte*)(void*)&value;
   int i;
   for (i = 0; i < sizeof(value); i++)
       *p++ = EEPROM.read(ee++);
   return i;
}

uint32_t failsafeEventCounter = 0;

void loop()
{
    buttonL.loop();
    buttonR.loop();

    if (buttonL.getState() == TACTILE_STATE_LONG_PRESS) {
        failsafeEventCounter = 0;
    }

    static bool prevIsFailsafe = false;

    if (prevIsFailsafe == false && sbusInput.isFailsafe() == true) {
        failsafeEventCounter++;
    }

    prevIsFailsafe = sbusInput.isFailsafe();

    if (buttonR.getState() == TACTILE_STATE_LONG_PRESS) {
        gpsState.originLat = gps.location.lat();
        gpsState.originLon = gps.location.lng();
        gpsState.originAlt = gps.altitude.meters();

        long writeValue;
        writeValue = gpsState.originLat * 10000000;
        EEPROM_writeAnything(0, writeValue);
        writeValue = gpsState.originLon * 10000000;
        EEPROM_writeAnything(4, writeValue);
        writeValue = gpsState.originAlt * 10000000;
        EEPROM_writeAnything(8, writeValue);
        EEPROM.commit();

        gpsState.distMax = 0;
        gpsState.altMax = -999999;
        gpsState.spdMax = 0;
        gpsState.altMin = 999999;
    }

    if (SerialGPS.available()) {
        while (SerialGPS.available() > 0) {
            gps.encode(SerialGPS.read());
            yield();
        }
    }

    if (gps.satellites.value() > 4) {
        gpsState.dist = TinyGPSPlus::distanceBetween(gps.location.lat(), gps.location.lng(), gpsState.originLat, gpsState.originLon);
    }

    sbusInput.loop();
    
    if (nextSerialTaskTs < millis()) 
    {
        Serial.println(xPortGetCoreID());
        nextSerialTaskTs = millis() + TASK_SERIAL_RATE;
    }

     oledDisplay.loop();
}
