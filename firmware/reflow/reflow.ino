#include "Button.h"
#include "Adafruit_MAX31855.h"
#include "Adafruit_GFX.h"
#include "Adafruit_PCD8544.h"

// indicators
const int LED_PIN = 3;

// thermocouple
const int THERMO_DO = 12;
const int THERMO_CS = 10;
const int THRERMO_CLK = 13;

// PCD8544 Nokia screen
const int SCREEN_SCLK = 13; // clock
const int SCREEN_MOSI = 11; // data out
const int SCREEN_DC = 16; // data/command select
const int SCREEN_SCE = 14; // chip select
const int SCREEN_RST = 15; // reset

// buttons
const int BTN_UP = 17;
const int BTN_SELECT = 18;
const int BTN_DOWN = 19;

// configuration
const int btnDebounceDuration = 10;
const int btnRepeatInterval = 300;

// temperature profile timing
int preheatTime = 1.5f * 60.0f; // TODO use memory
int soakingTime = 1.5f * 60.0f;
int reflowTime = 1.0f * 60.0f;
int peakTime = 20;
int coolingTime = 2.0f * 60.0f;
int totalTime = preheatTime + soakingTime + reflowTime + peakTime + coolingTime;

// temperature profile temperatures
int startTemp = 25; // todo internal temp
int preheatTemp = 150;
int soakingTemp = 170;
int reflowTemp = 250;
int coolingTemp = startTemp;

// states
const int STATE_MAIN_MENU = 0;
const int STATE_REFLOWING = 1;

// runtime information
int state = STATE_MAIN_MENU;
int lastState = -1;
int ledValue = 0;
unsigned long lastUIRenderTime = 0;
unsigned long startTime = 0;
int counter = 1;

// main menu state
String mainMenuItems[] = {"Start reflow", "Pick profile", "Show profile", "Learn PID", "About"};
int mainMenuItemCount = 5;
int activeMainMenuIndex = 0;

String command = "";
char commandStart = '<';
char commandEnd = '>';

// choose which serial to use - "Serial" for debugging, "Serial1" for bluetooth
#define SERIAL Serial
//#define SERIAL Serial1

// display renderer
Adafruit_PCD8544 display = Adafruit_PCD8544(SCREEN_SCLK, SCREEN_MOSI, SCREEN_DC, SCREEN_SCE, SCREEN_RST);

// thermocouple
Adafruit_MAX31855 thermocouple(THRERMO_CLK, THERMO_CS, THERMO_DO);

// buttons
Button btnUp = Button(BTN_UP, btnDebounceDuration);
Button btnSelect = Button(BTN_SELECT, btnDebounceDuration);
Button btnDown = Button(BTN_DOWN, btnDebounceDuration);
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
  display.setContrast(30);
  display.clearDisplay();
  display.display();
  
  // TODO this should be set from the UI
  startTime = millis();
}

void loop() {
  // update buttons
  for (int i = 0; i < buttonCount; i++) {
    int duration = buttons[i].duration();
    boolean repeated = false;
    
    if (buttons[i].read() == LOW && duration > btnRepeatInterval) {
       buttons[i].rebounce(btnRepeatInterval);
       repeated = true;
    }
    
    if (buttons[i].update()) {
      if (buttons[i].fallingEdge()) {
        onKeyPress(buttons[i].pin, duration, repeated);
      } else {
        onKeyRelease(buttons[i].pin, duration);
      }
    }
    
    if (buttons[i].read() == LOW) {
      onKeyDown(buttons[i].pin);
    } else {
      onKeyUp(buttons[i].pin);
    }
  }
  
  // render current state
  switch (state) {
    case STATE_MAIN_MENU:
      displayMainMenuState(false);
    break;
    
    case STATE_REFLOWING:
      displayReflowingState(false);
    break;
  }
  
  // handle input
  while (SERIAL.available() > 0) {
    int input = SERIAL.read();

    if (input == commandStart) {
      command = "";
    } else if (input == commandEnd) {
      handle(command);

      command = "";
    } else {
      command += (char)input;
    }
  }
}

void onKeyPress(int btn, unsigned long duration, boolean repeated) {
  //SERIAL.print("Pressed: ");
  //SERIAL.print(btn);
  //SERIAL.println(repeated ? "repeated" : "not repeated");
  
  if (state == STATE_MAIN_MENU) {
    if (btn == BTN_UP) {
      if (activeMainMenuIndex > 0) {
        activeMainMenuIndex--;
       
        displayMainMenuState(true);
      }
    } else if (btn == BTN_DOWN) {
      if (activeMainMenuIndex < mainMenuItemCount - 1) {
        activeMainMenuIndex++;
       
        displayMainMenuState(true);
      }
    } else if (btn == BTN_SELECT) {
      onMainMenuSelect(activeMainMenuIndex);
    }
  } else if (state == STATE_REFLOWING) {
    if (btn == BTN_SELECT) {
      setLastState();
    }
  }
}

void onKeyRelease(int btn, unsigned long duration) {
  //SERIAL.print("Released: ");
  //SERIAL.print(btn);
  //SERIAL.print(" after ");
  //SERIAL.print(duration);
  //SERIAL.println("ms");
  
  if (state == STATE_MAIN_MENU) {
    
  }
}

void onKeyDown(int btn) {
  //SERIAL.print("DOWN: ");
  //SERIAL.println(btn);
}

void onKeyUp(int btn) {
  //SERIAL.print("UP: ");
  //SERIAL.println(btn);
}

void setState(int newState) {
  lastState = state;
  state = newState; 
}

void setLastState() {
  if (lastState == -1) {
    return; 
  }
  
  setState(lastState);
  lastState = -1;
}

void displayMainMenuState(boolean force) {
  unsigned long currentTime = millis();
  
  // update UI at certain interval
  if (!force && currentTime - lastUIRenderTime < 500) {
    return; 
  }
  
  renderMenu(mainMenuItems, activeMainMenuIndex);
}

void onMainMenuSelect(int index) {
   setState(STATE_REFLOWING);
}

void renderMenu(String items[], int activeIndex) {
  display.clearDisplay();
  display.setTextSize(1);
  
  int scrollOffset = 0;
  
  if (activeMainMenuIndex <= 1) {
    scrollOffset = 0;
  } else if (activeMainMenuIndex == mainMenuItemCount - 1) {
    scrollOffset = activeMainMenuIndex - 2;
  } else {
    scrollOffset = activeMainMenuIndex - 1;
  }
  
  for (int i = 0; i < mainMenuItemCount; i++) {
    renderMenuItem(mainMenuItems[i], i, scrollOffset, i == activeMainMenuIndex);
  }
  
  display.display();
}

void renderMenuItem(String text, int index, int scrollOffset, boolean active) {
  int lineHeight = 13;
  int textHeight = 7;
  int padding = (lineHeight - textHeight) / 2;
  int rowX = 0;
  int rowY = lineHeight * index - scrollOffset * lineHeight;
  
  if (active) {
    display.fillRect(0, rowY, display.width() - 1, lineHeight, BLACK);
    display.setTextColor(WHITE);
  } else {
    display.setTextColor(BLACK);
  }
  
  display.setCursor(rowX + 3, rowY + padding);
  display.println(text);
}

void displayReflowingState(boolean force) {
  unsigned long currentTime = millis();
  
  // update UI at certain interval
  if (!force && currentTime - lastUIRenderTime < 500) {
    return; 
  }
  
  //ledValue = ledValue == 0 ? 1 : 0;
  //digitalWrite(LED_PIN, ledValue);
  
  int internalTemp = thermocouple.readInternal();
  int sensorTemp = thermocouple.readCelsius();
  int duration = (currentTime - startTime) / 1000;
  int percentage = duration * 100 / totalTime; // todo
  int targetTemp = getProfileTempAt(duration);
  
  sensorTemp = targetTemp; // test

  // send information over serial
  /*SERIAL.print(counter);
  SERIAL.print(". internal: ");
  SERIAL.print(internalTemp);
  SERIAL.print(", sensor: ");
  SERIAL.println(sensorTemp);*/
  
  lastUIRenderTime = currentTime;
  
  if (btnUp.fallingEdge()) {
    sensorTemp *= 2; 
  }
  
  renderProgress(sensorTemp, targetTemp, duration, percentage);
  
  counter++;
}

void renderProgress(int sensorTemp, int targetTemp, int duration, int percentage) {
  // reset
  display.clearDisplay();
  
  // draw temperatures
  int textX = 0;
  
  if (sensorTemp < 10) {
    textX += 12;
  } else if (sensorTemp < 100) {
    textX += 6;
  }
  
  if (targetTemp < 10) {
    textX += 12;
  } else if (targetTemp < 100) {
    textX += 6;
  }
  
  display.setTextSize(2);
  display.setTextColor(BLACK);
  display.setCursor(textX, 0);
  display.print(sensorTemp);
  display.print("/");
  display.println(targetTemp);
  
  renderProfile();
  
  // draw current temp indicator
  /*int x = (float)percentage / 100.0f * display.width();
  int y = display.height() - 1 - sensorTemp / 4.0f;
  //display.fillCircle(x, y, 2, BLACK);
  //display.fillTriangle(x, y, x - 1, y - 1, x + 1, y - 1, BLACK);
  display.drawFastHLine(x - 2, y, 5, BLACK);
  display.drawFastVLine(x, y - 2, 5, BLACK);*/
  
  // draw percentage
  textX = 20;
  int textY = display.height() - 7;
  int charWidth = 6;
  int charHeight = 7;
  int chars = 7;
  int paddingX = 3;
  int paddingY = 2;
  
  if (percentage < 10) {
    textX += 3;
    chars -= 1;
  } else if (percentage > 99) {
    textX -= 3;
    chars += 1;
  }
  
  if (duration < 10) {
    textX += 3;
    chars -= 1;
  } else if (duration > 99) {
    textX -= 3;
    chars += 1;
  }
  
  display.setTextSize(1);
  display.setTextColor(BLACK);
  display.fillRect(textX - paddingX, textY - paddingY, chars * charWidth + paddingX * 2, charHeight + paddingY, WHITE);
  display.setCursor(textX, textY);
  display.print(percentage);
  display.print("% ");
  display.print(duration);
  display.print("s");
  
  display.display(); 
}

void renderProfile() {
  int posX = 0;
  int posY = 17;
  int windowWidth = display.width();
  int windowHeight = display.height() - posY;
  
  float pixelsPerSecond = (float)windowWidth / (float)totalTime;
  float pixelsPerDegree = (float)windowHeight / (float)reflowTemp;
  
  int startY = posY + windowHeight - 1;
  
  // draw graph
  /*int x = posX;
  int startY = posY + windowHeight - 1;
  int dx, y1, y2;
  
  // calculate preheat line
  dx = (float)preheatTime * pixelsPerSecond;
  y1 = startY - (float)startTemp * pixelsPerDegree;
  y2 = startY - (float)preheatTemp * pixelsPerDegree;
  display.drawLine(x, y1, x + dx, y2, BLACK);
  x = x + dx;
  
  // soaking
  dx = (float)soakingTime * pixelsPerSecond;
  y1 = y2;
  y2 = startY - (float)soakingTemp * pixelsPerDegree;
  display.drawLine(x, y1, x + dx, y2, BLACK);
  x = x + dx;
  
  // reflow
  dx = (float)reflowTime * pixelsPerSecond;
  y1 = y2;
  y2 = startY - (float)reflowTemp * pixelsPerDegree;
  display.drawLine(x, y1, x + dx, y2, BLACK);
  x = x + dx;
  
  // peak
  dx = (float)peakTime * pixelsPerSecond;
  y1 = y2;
  y2 = startY - (float)reflowTemp * pixelsPerDegree;
  display.drawLine(x, y1, x + dx, y2, BLACK);
  x = x + dx;
  
  // cooling
  dx = (float)coolingTime * pixelsPerSecond;
  y1 = y2;
  y2 = startY - (float)coolingTemp * pixelsPerDegree;
  display.drawLine(x, y1, x + dx, y2, BLACK);
  x = x + dx;*/
  
  unsigned long currentTime = millis();
  unsigned long duration = (currentTime - startTime) / 1000;
  
  for (int i = 0; i < totalTime; i++) {
    int profileTemp = getProfileTempAt(i);
    
    int x = posX + (float)i * pixelsPerSecond;
    int dy = (float)profileTemp * pixelsPerDegree;
    
    display.drawPixel(x, startY - dy, BLACK); 
    
    if (i < duration) {
      int realTemp = profileTemp; // TODO get real temp at given time
      dy = (float)realTemp * pixelsPerDegree;
      display.drawLine(x, startY, x, startY - dy, BLACK);
    }
  }
}

int getProfileTempAt(int seconds) {
  int t1, t2;
  float progress;
  
  if (seconds < preheatTime) {
    t1 = startTemp;
    t2 = preheatTemp;
    progress = (float)seconds / (float)preheatTime;
  } else if (seconds < preheatTime + soakingTime) {
    t1 = preheatTemp;
    t2 = soakingTemp;
    progress = (float)(seconds - preheatTime) / (float)soakingTime;
  } else if (seconds < preheatTime + soakingTime + reflowTime) {
    t1 = soakingTemp;
    t2 = reflowTemp;
    progress = (float)(seconds - preheatTime - soakingTime) / (float)reflowTime;
  } else if (seconds < preheatTime + soakingTime + reflowTime + peakTime) {
    t1 = reflowTemp;
    t2 = reflowTemp;
    progress = (float)(seconds - preheatTime - soakingTime - reflowTime) / (float)peakTime;
  } else {
    t1 = reflowTemp;
    t2 = coolingTemp;
    progress = (float)(seconds - preheatTime - soakingTime - reflowTime - peakTime) / (float)coolingTime;
  }

  if (progress > 1.0f) {
    return -1;
  }
  
  return t1 + (float)(t2 - t1) * progress; 
}

void handle(String command) {
  if (command == "on") {
    setLed(1);
  } else if (command == "off") {
    setLed(0);
  }
  
  Serial1.println("Received: '" + command + "'");
}

void setLed(int value) {
  ledValue = value;
  digitalWrite(LED_PIN, ledValue);
}
