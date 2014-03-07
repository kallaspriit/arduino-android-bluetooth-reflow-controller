// indicators
#define LED_PIN 3

// thermocouple
#define THERMO_DO 12
#define THERMO_CS 10
#define THRERMO_CLK 13

// PCD8544 Nokia screen
#define SCREEN_SCLK 13 // clock
#define SCREEN_MOSI 11 // data out
#define SCREEN_DC 16 // data/command select
#define SCREEN_SCE 14 // chip select
#define SCREEN_RST 15 // reset

// buttons
#define BTN_UP 17
#define BTN_SELECT 18
#define BTN_DOWN 19

// configuration
#define btnDebounceDuration 10
#define btnRepeatInterval 300

// choose which serial to use - "Serial" for debugging, "Serial1" for bluetooth
#define SERIAL Serial
//#define SERIAL Serial1

// intents
#define INTENT_NONE 0
#define INTENT_MAIN_MENU 1
#define INTENT_START_REFLOW 2
