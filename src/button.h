#ifndef button_h
#define button_h

#include <Arduino.h>



class Button {
  
  private :  
    
    
    uint32_t previous = 0;
    uint8_t  pin;
  
  public:
  
    Button(uint8_t pin_number) {
      pin = pin_number;
    }

    bool poll() {
      // poll button state
      uint8_t read = digitalRead(pin);
      // did it change ?
      if (read == LOW) {
        // did it change very quickly?
        if ( millis() - previous > 300 ) { 
          previous = millis();
          return true;
        }
      }
      return false;
    }
    // ok, that's good enough now. Now we can use any old pin and no interrupts.
    // friends don't let friends use ISRs to handle switches.
};

#endif
