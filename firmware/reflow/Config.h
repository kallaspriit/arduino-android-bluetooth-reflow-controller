// board version selection
//#define REFLOW_BOARD_V1
#define REFLOW_BOARD_V2

#if defined(REFLOW_BOARD_V1)
  // thermocouple
  #define THERMO_DO 12   // data
  #define THERMO_CS 10   // chip-select
  #define THRERMO_CLK 13 // clock
  
  // PCD8544 Nokia screen
  #define SCREEN_SCLK 13 // clock
  #define SCREEN_MOSI 11 // data out
  #define SCREEN_DC 16   // data/command select
  #define SCREEN_SCE 14  // chip select
  #define SCREEN_RST 15  // reset
  
  // buttons
  #define BTN_PIN_MODE INPUT_PULLUP
  #define BTN_PRESSED_LEVEL LOW
  #define BTN_UP 17
  #define BTN_SELECT 18
  #define BTN_DOWN 19
  
  // eeprom
  #define EEPROM_USABLE_SIZE 2048
#elif defined(REFLOW_BOARD_V2)
  // thermocouple
  #define THERMO_DO MISO   // data
  #define THERMO_CS SS   // chip-select
  #define THRERMO_CLK SCL // clock
  
  // PCD8544 Nokia screen
  #define SCREEN_SCLK SCK // clock
  #define SCREEN_MOSI MOSI // data out
  #define SCREEN_DC 8   // data/command select
  #define SCREEN_SCE 4  // chip select
  #define SCREEN_RST 7  // reset
  
  // buttons
  #define BTN_PIN_MODE INPUT_PULLUP
  #define BTN_PRESSED_LEVEL LOW
  #define BTN_UP A2
  #define BTN_SELECT A1
  #define BTN_DOWN A0
  
  // eeprom
  #define EEPROM_USABLE_SIZE 1024
#endif

// relay
#define RELAY_PIN 20
#define RELAY_CONNECTED_LEVEL HIGH
#define RELAY_MIN_INTERVAL 3.0f // minimum pause between relay switching states

// configuration
#define SIMUALATION_MODE false // if this is set true, a dummy simulated owen is used instead of the real thermocouple
#define TIME_MULTIPLIER 1.0f // increase this to for example to 20.0f to make the time pass faster for simulation
#define BTN_DEBOUNCE_DURATION 10
#define BTN_REPEAT_INTERVAL 300
#define PROFILE_MAX_DURATION 3600 // 60 minutes storage for graph, make sure your profile is not longer
#define MEMORY_VERSION 3 // update this every time the memory footprint changes
#define PREHEAT_TEMPERATURE 80

// PID
#define PID_DEFAULT_P 1.0f
#define PID_DEFAULT_I 0.0005f
#define PID_DEFAULT_D 20.0f

// simulation parameters
#define SIMULATION_HEATING_ACCELERATION 0.05f
#define SIMULATION_COOLING_ACCELERATION -0.05f
#define SIMULATION_MAX_HEATING_SPEED 2.0f
#define SIMULATION_MAX_COOLING_SPEED -1.5f

// choose which serial to use - "Serial" for debugging, "Serial1" for bluetooth
#define COMM Serial
//#define COMM Serial1

// intents
#define INTENT_NONE 0
#define INTENT_MAIN_MENU 1
#define INTENT_START_REFLOW 2
#define INTENT_CONFIGURE_PID 3
