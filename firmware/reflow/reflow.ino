#include "Button.h"
#include "Menu.h"
#include "Owen.h"
#include "ReflowProfile.h"
#include "Config.h"

#include "MainMenuState.h"
#include "ReflowState.h"

#include "Adafruit_GFX.h"
#include "Adafruit_PCD8544.h"
#include "Adafruit_MAX31855.h"

// runtime information
State* state = NULL;
State* lastState = NULL; // TODO Use stack of last states
unsigned long lastStepTime = 0;
String command = "";
char commandStart = '<';
char commandEnd = '>';

int ledValue = 0; // TODO Remove
unsigned long lastUIRenderTime = 0; // TODO Move to state
unsigned long startTime = 0; // TODO Move to state
int counter = 1; // TODO Move to state

// display renderer
Adafruit_PCD8544 display = Adafruit_PCD8544(SCREEN_SCLK, SCREEN_MOSI, SCREEN_DC, SCREEN_SCE, SCREEN_RST);

// reflow profile
ReflowProfile profile = ReflowProfile();

// owen interface
Owen owen = Owen();

// states
MainMenuState mainMenuState = MainMenuState(&display);
ReflowState reflowState = ReflowState(&display, &owen, &profile);

// buttons
Button btnUp = Button(BTN_UP, BTN_DEBOUNCE_DURATION);
Button btnSelect = Button(BTN_SELECT, BTN_DEBOUNCE_DURATION);
Button btnDown = Button(BTN_DOWN, BTN_DEBOUNCE_DURATION);
Button buttons[] = {btnUp, btnSelect, btnDown};
int buttonCount = 3;

void setup() {
  // setup serial
  Serial1.begin(9600);
  
  // setup pinmodes
  pinMode(LED_PIN, OUTPUT);
  
  // buttons
  pinMode(BTN_UP, INPUT_PULLUP);
  pinMode(BTN_SELECT, INPUT_PULLUP);
  pinMode(BTN_DOWN, INPUT_PULLUP);
  
  // setup display
  display.begin();
  display.setContrast(45);
  display.clearDisplay();
  display.display();
  
  // set starting state
  setState(mainMenuState);
  
  // TODO this should be set from the UI
  startTime = millis();
}

void loop() {
  unsigned long currentTime = millis();
  float dt = (float)(currentTime - lastStepTime) / 1000.0f;
  
  // make time move faster for testing
  dt *= 20.0f;
  
  // update buttons
  for (int i = 0; i < buttonCount; i++) {
    int duration = buttons[i].duration();
    boolean repeated = false;
    
    // repeat button presses if held down
    if (buttons[i].read() == LOW && duration > BTN_REPEAT_INTERVAL) {
       buttons[i].rebounce(BTN_REPEAT_INTERVAL);
       repeated = true;
    }
    
    // detect button press/release
    if (buttons[i].update()) {
      if (buttons[i].fallingEdge()) {
        onKeyPress(buttons[i].pin, duration, repeated);
      } else {
        onKeyRelease(buttons[i].pin, duration);
      }
    }
    
    // detect whether button is held down
    if (buttons[i].read() == LOW) {
      onKeyDown(buttons[i].pin);
    } else {
      onKeyUp(buttons[i].pin);
    }
  }
  
  // step active state
  if (state != NULL) {
    int intent = state->step(dt); 
    
    if (intent != INTENT_NONE) {
      processIntent(intent);
    }
  }
  
  // handle serial input
  while (SERIAL.available() > 0) {
    int input = SERIAL.read();

    if (input == commandStart) {
      command = "";
    } else if (input == commandEnd) {
     processCommand(command);

      command = "";
    } else {
      command += (char)input;
    }
  }
  
  lastStepTime = currentTime;
}

void processIntent(int intent) {
  if (intent == INTENT_MAIN_MENU) {
    setState(mainMenuState);
  } else if (intent == INTENT_START_REFLOW) {
    setState(reflowState);
  }
}

void processCommand(String command) {
  if (command == "on") {
    setLed(1);
  } else if (command == "off") {
    setLed(0);
  }
  
  Serial1.println("Received: '" + command + "'");
}

void onKeyPress(int btn, unsigned long duration, boolean repeated) {
  /*SERIAL.print("Pressed: ");
  SERIAL.print(btn);
  SERIAL.println(repeated ? " repeated" : " not repeated");*/
  
  if (state != NULL) {
    state->onKeyPress(btn, duration, repeated); 
  }
}

void onKeyRelease(int btn, unsigned long duration) {
  //SERIAL.print("Released: ");
  //SERIAL.print(btn);
  //SERIAL.print(" after ");
  //SERIAL.print(duration);
  //SERIAL.println("ms");
  
  if (state != NULL) {
    state->onKeyRelease(btn, duration); 
  }
}

void onKeyDown(int btn) {
  //SERIAL.print("DOWN: ");
  //SERIAL.println(btn);
  
  if (state != NULL) {
    state->onKeyDown(btn); 
  }
}

void onKeyUp(int btn) {
  //SERIAL.print("UP: ");
  //SERIAL.println(btn);
  
  if (state != NULL) {
    state->onKeyUp(btn); 
  }
}

void setState(State& newState) {
  if (state != NULL) {
    state->onExit();
  }
  
  newState.onEnter();
  
  lastState = state;
  state = &newState;
}

/*void setLastState() {
  if (lastState == NULL) {
    return; 
  }
  
  setState(*lastState);
  lastState = NULL;
}*/

void setLed(int value) {
  ledValue = value;
  digitalWrite(LED_PIN, ledValue);
}
