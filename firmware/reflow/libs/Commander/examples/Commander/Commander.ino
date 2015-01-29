#include "Commander.h"

Serial_ *serial = &Serial;
//HardwareSerial *serial = &Serial1;

Commander commander(serial);

void setup() {
  serial->begin(9600);
}

void loop() {
  while (commander.gotCommand()) {
    handleCommand(commander.command, commander.parameters, commander.parameterCount);
  }
}

void handleCommand(String command, String parameters[], int parameterCount) {
  if (command == "sum" && parameterCount == 2) {
     int a = parameters[0].toInt();
     int b = parameters[1].toInt();
     
     serial->print("Sum ");
     serial->print(a);
     serial->print("+");
     serial->print(b);
     serial->print("=");
     serial->println(a+b);
  } else {
    serial->print("Got command '");
    serial->print(command);
    serial->print("' with ");
    serial->print(parameterCount);
    serial->println(" parameters: ");
    
    for (int i = 0; i < parameterCount; i++) {
      serial->print("  > ");
      serial->print(i);
      serial->print(": ");
      serial->println(parameters[i]);
    }
  }
}