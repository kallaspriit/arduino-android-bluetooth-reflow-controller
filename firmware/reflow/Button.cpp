
// Please read Button.h for information about the liscence and authors

#include "WProgram.h"
#include "Button.h"


Button::Button(uint8_t pin,unsigned long interval_millis)
{
	interval(interval_millis);
	previous_millis = millis();
	state = digitalRead(pin);
    this->pin = pin;
}


void Button::write(int new_state)
       {
       	this->state = new_state;
       	digitalWrite(pin,state);
       }


void Button::interval(unsigned long interval_millis)
{
  this->interval_millis = interval_millis;
  this->rebounce_millis = 0;
}

void Button::rebounce(unsigned long interval)
{
	 this->rebounce_millis = interval;
}



int Button::update()
{
	if ( debounce() ) {
        rebounce(0);
        return stateChanged = 1;
    }

     // We need to rebounce, so simulate a state change
     
	if ( rebounce_millis && (millis() - previous_millis >= rebounce_millis) ) {
        previous_millis = millis();
		 rebounce(0);
		 return stateChanged = 1;
	}

	return stateChanged = 0;
}


unsigned long Button::duration()
{
  return millis() - previous_millis;
}


int Button::read()
{
	return (int)state;
}


// Protected: debounces the pin
int Button::debounce() {
	
	uint8_t newState = digitalRead(pin);
	if (state != newState ) {
  		if (millis() - previous_millis >= interval_millis) {
  			previous_millis = millis();
  			state = newState;
  			return 1;
	}
  }
  
  return 0;
	
}

// The risingEdge method is true for one scan after the de-bounced input goes from off-to-on.
bool  Button::risingEdge() { return stateChanged && state; }
// The fallingEdge  method it true for one scan after the de-bounced input goes from on-to-off. 
bool  Button::fallingEdge() { return stateChanged && !state; }

