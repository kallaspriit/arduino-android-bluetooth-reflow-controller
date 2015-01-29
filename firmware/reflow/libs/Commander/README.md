Arduino Commander
=================
**Configurable C++ command parser that works well for Arduino and the likes.**

# What it does
Parses commands like ```<sum:3:5>``` from the Serial stream into a command with name "sum" with two parameters "3" and "5".

# How to use it
See the ```examples/Commander/Commander.ino``` file for a complete example but basically just create an instance of the Commander providing pointer to the Serial you're using and in your loop, call gotCommand() which returns true if a new command was received. You can then directly access the latest command info from the commander.

# Example
```c++
#include "Commander.h"

Commander commander(&Serial);

void setup() {}

void loop() {
  while (commander.gotCommand()) {
    handleCommand(commander.command, commander.parameters, commander.parameterCount);
  }
}

void handleCommand(String command, String parameters[], int parameterCount) { ... }
```

# Configuring
By default, the Commander library uses ```<``` and ```>``` as message start and end tokens and ```:``` for parameters delimiter but you can configure these in the constructor of Commander class initiation.

# Licence
The code snipper is licenced under MIT so basically do whatever you want with it, hope it helps someone :)
