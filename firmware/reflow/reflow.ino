#include "Button.h"
#include "Menu.h"
#include "Owen.h"
#include "ReflowProfile.h"
#include "Config.h"

#include "MainMenuState.h"
#include "ReflowState.h"
#include "ConfigurePidState.h"

#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>
#include <Adafruit_MAX31855.h>
#include <EEPROMex.h>
#include <aJSON.h>

// runtime information
State* state = NULL;
unsigned long lastStepTime = 0;
String command = "";
char commandStart = '<';
char commandEnd = '>';

// create serial stream for JSON
aJsonStream serialStream(&SERIAL);

// display renderer
Adafruit_PCD8544 display = Adafruit_PCD8544(SCREEN_SCLK, SCREEN_MOSI, SCREEN_DC, SCREEN_SCE, SCREEN_RST);

// reflow profile
ReflowProfile profile = ReflowProfile();

// owen interface
Owen owen = Owen();

// states
MainMenuState mainMenuState = MainMenuState(&display);
ReflowState reflowState = ReflowState(&display, &owen, &profile);
ConfigurePidState configurePidState = ConfigurePidState(&display, owen.getPID());

// buttons
Button btnUp = Button(BTN_UP, BTN_DEBOUNCE_DURATION);
Button btnSelect = Button(BTN_SELECT, BTN_DEBOUNCE_DURATION);
Button btnDown = Button(BTN_DOWN, BTN_DEBOUNCE_DURATION);
Button buttons[] = {btnUp, btnSelect, btnDown};
int buttonCount = 3;

void setup() {
  // setup serial
  Serial1.begin(9600);
  
  // buttons
  pinMode(BTN_UP, INPUT_PULLUP);
  pinMode(BTN_SELECT, INPUT_PULLUP);
  pinMode(BTN_DOWN, INPUT_PULLUP);
  
  // setup display
  display.begin();
  display.setContrast(55);
  display.clearDisplay();
  display.display();
  
  // initialize eeprom
  EEPROM.setMemPool(0, EEPROMSizeUno);
  
  // initialize the reflow profile
  profile.init();
  
  // initialized owen
  owen.init();
  
  // set starting state
  setState(mainMenuState);
}

void loop() {
  unsigned long currentTime = millis();
  float dt = (float)(currentTime - lastStepTime) / 1000.0f;

  // time can be made to move faster for simulation etc
  dt *= TIME_MULTIPLIER;
  
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
  } else if (intent == INTENT_CONFIGURE_PID) {
    setState(configurePidState);
  }
}

void processCommand(String command) {
  if (command.substring(0, 1) == '{') {
    processJsonCommand(command);
  } else {
    SERIAL.println("Unknown command: '" + command + "'"); 
  }
}

void processJsonCommand(String json) {
  const int bufSize = 256;
  char buf[bufSize];
  json.toCharArray(buf, bufSize);
  
  aJsonObject* msg = aJson.parse(buf);
  aJsonObject* typeObj = aJson.getObjectItem(msg, "type");
  
  if (typeObj->type != aJson_String) {
    SERIAL.println("Json command is missing type");
    
    return; 
  }
  
  String type = typeObj->valuestring;
  aJsonObject* data = aJson.getObjectItem(msg, "data");
  
  processRequest(type, data);
  
  aJson.deleteItem(msg);
}

void processRequest(String type, aJsonObject* data) {
  SERIAL.println("Process request: " + type); 
  
  if (type == "get-pid") {
    // <{"type":"get-pid"}>
    sendPid();
  } else if (type == "set-pid") {
    // <{"type":"set-pid","data":{"p":1,"i":2,"d":3}}>
    float p = aJson.getObjectItem(data, "p")->valueint > 0 ? aJson.getObjectItem(data, "p")->valueint : aJson.getObjectItem(data, "p")->valuefloat; 
    float i = aJson.getObjectItem(data, "i")->valueint > 0 ? aJson.getObjectItem(data, "i")->valueint : aJson.getObjectItem(data, "i")->valuefloat;
    float d = aJson.getObjectItem(data, "d")->valueint > 0 ? aJson.getObjectItem(data, "d")->valueint : aJson.getObjectItem(data, "d")->valuefloat;
    
    setPid(p, i, d);
  }if (type == "get-state") {
    // <{"type":"get-state"}>
    sendStateChanged();
  } else if (type == "intent") {
    // <{"type":"intent","data":{"intent":3}}>
    int intent = aJson.getObjectItem(data, "intent")->valueint;
    
    processIntent(intent);
  }
}

void sendPid() {
  aJsonObject* msg = createPidMessage();
  aJson.print(msg, &serialStream);
  SERIAL.println();
  aJson.deleteItem(msg);
}

void sendStateChanged() {
  aJsonObject* msg = createStateChangedMessage();
  aJson.print(msg, &serialStream);
  SERIAL.println();
  aJson.deleteItem(msg);
}

void setPid(float p, float i, float d) {
  owen.getPID()->profile.p = p;
  owen.getPID()->profile.i = i;
  owen.getPID()->profile.d = d;
  
  owen.getPID()->save();
  
  sendPid();
}

aJsonObject* createPidMessage() {
  aJsonObject* msg = aJson.createObject();
  aJsonObject* data = aJson.createObject();
  
  aJson.addNumberToObject(data, "p", owen.getPID()->profile.p);
  aJson.addNumberToObject(data, "i", owen.getPID()->profile.i);
  aJson.addNumberToObject(data, "d", owen.getPID()->profile.d);

  aJson.addStringToObject(msg, "type", "pid");
  aJson.addItemToObject(msg, "data", data);

  return msg;
}

aJsonObject* createStateChangedMessage() {
  char* stateName = "none";
  
  if (state != NULL) {
    stateName = state->getName(); 
  }
  
  aJsonObject* msg = aJson.createObject();
  aJsonObject* data = aJson.createObject();
  
  aJson.addStringToObject(data, "name", stateName);

  aJson.addStringToObject(msg, "type", "state-changed");
  aJson.addItemToObject(msg, "data", data);

  return msg;
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
  
  state = &newState;
  
  sendStateChanged();
}
