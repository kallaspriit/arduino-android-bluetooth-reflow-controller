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

// relay
#define RELAY_PIN 20
#define RELAY_CONNECTED_LEVEL LOW
#define RELAY_MIN_INTERVAL 3.0f // minimum pause between relay switching states

// configuration
#define BTN_DEBOUNCE_DURATION 10
#define BTN_REPEAT_INTERVAL 300
#define PROFILE_MAX_DURATION 1200 // 20 minutes storage for graph

// simulation parameters
#define SIMUALATION_MODE true
#define SIMULATION_HEATING_SPEED 2.0f
#define SIMULATION_COOLING_SPEED 1.0f

// choose which serial to use - "Serial" for debugging, "Serial1" for bluetooth
#define SERIAL Serial
//#define SERIAL Serial1

// intents
#define INTENT_NONE 0
#define INTENT_MAIN_MENU 1
#define INTENT_START_REFLOW 2
