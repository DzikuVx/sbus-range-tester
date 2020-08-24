#ifndef OLED_DISPLAY
#define OLED_DISPLAY

#include "SSD1306.h"
#include <TinyGPS++.h>
#include "sbus.h"
#include "types.h"

enum txOledPages {
    OLED_PAGE_NONE,
    OLED_PAGE_DATA
};

#define OLED_COL_COUNT 64
#define OLED_DISPLAY_PAGE_COUNT 2

extern TinyGPSPlus gps;
extern int16_t rcChannels[16];
extern SbusInput sbusInput;
extern GpsDataState_t gpsState;
extern uint32_t failsafeEventCounter;

class OledDisplay {
    public:
        OledDisplay(SSD1306 *display);
        void init();
        void loop();
        
        void setPage(uint8_t page);
    private:
        SSD1306 *_display;
        void renderPageData();
        void page();
        uint8_t _page = OLED_PAGE_NONE;
        bool _forceDisplay = false;
};


#endif