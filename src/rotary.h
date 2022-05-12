#ifndef rotary_h
#define rotary_h

#include <Arduino.h>


// basic bitch rotaey encoder handling, I'm aware that this should be split
// but life is short and I want to get stuff done,

class RotaryEncoder {

  uint8_t clk_pin;
  uint8_t dat_pin;

  int last_clk;
  int last_dat;

  public :

    enum Direction {
      left,
      right,
      none
    };


    RotaryEncoder(uint8_t clk, uint8_t dt) {
      clk_pin  = clk;
      dat_pin  = dt;
      last_clk = digitalRead(clk_pin);
      last_dat = digitalRead(dat_pin);
    }


    Direction poll() {
      
      Direction direction = none;

      // ok, this works, but needs a bit of debouncing

      int clk = digitalRead(clk_pin);
      int dat = digitalRead(dat_pin);

      if (clk != last_clk) {
        if (clk == dat) {  direction = left;  }
        else            {  direction = right; }
      }
 
      last_clk = clk;
      last_dat = dat;
      
      return direction;
    }

};

#endif