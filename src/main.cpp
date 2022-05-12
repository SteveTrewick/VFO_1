
#include <Arduino.h>
#include <U8x8lib.h>
#include <Wire.h>
#include "si5351.h"
#include "rotary.h"
#include "frequency.h"
#include "button.h"

// config for the rotary encoder.

#define CLK 4
#define DT  3
#define SW  2


// what we're here for
Si5351 si5351;

// so the AdaFruit library will not play nice with the Si5351 I have,
// and/or the cheap OLED screen I used, so I used the UG82 lib
U8X8_SSD1306_128X64_NONAME_HW_I2C u8x8(/* reset=*/ U8X8_PIN_NONE);  

// Deals with radix and conversion to printable values
// see frequency.h
Frequency freq;

// basic bitch functionality for the rotary encoder and its switch
RotaryEncoder encoder(CLK, DT);       // rotary.h
Button        encoder_switch(SW);     // button.h




void updateDisplay() {
  

  // display a 9 digit decimal seperated frequency.
  // really I only need 7 for the reciever but I also wanted to test
  // with up conversion via a mixer, so ...

  Frequency::DisplayParts parts = freq.displayParts();
  
  char display[12];
  sprintf(display, "%03i.%03i.%03i", parts.mhz, parts.khz, parts.hz);
  
  u8x8.setFont(u8x8_font_7x14B_1x2_n);    
  u8x8.drawString(0, 2, display);

  // ugly solutiojn to keep track of the radix, just draw a -
  // under whatever we're changing

  int shift = freq.radixIndex;
  if (shift > 2) { shift++; }
  if (shift > 6) { shift++; }
  u8x8.drawString(0, 4, "              ");
  u8x8.drawString(10 - shift, 4, "-");

}


void setup() {
  

  // so, the encoder I have is +V pulse and pulls SW to ground
  // thus ...
  pinMode(CLK,INPUT);
	pinMode(DT,INPUT);
	pinMode(SW, INPUT_PULLUP);

  // init the 5351 or give up
  bool i2c_found = si5351.init(SI5351_CRYSTAL_LOAD_8PF, 0, 0);
  if(!i2c_found)
  {
    Serial.println("Si5351 not found on I2C bus!");
    while(1);
  }

  // set initial freq
  si5351.set_freq(freq.siValue, SI5351_CLK0);

  // say hi
  u8x8.begin();
  u8x8.setFont(u8x8_font_amstrad_cpc_extended_f);    
  u8x8.clear();

  u8x8.print("M7STZ Si5351 VFO");

  updateDisplay();

}






void updateFrequency() {
  si5351.set_freq(freq.siValue, SI5351_CLK0);
}


// and well. there we go.

void loop() {
  
  switch (encoder.poll()) {
    case RotaryEncoder::Direction::none : 
      break;
    
    case RotaryEncoder::Direction::left:
      
      freq.decrement();
      updateFrequency();
      updateDisplay();
      
      break;

    case RotaryEncoder::Direction::right:
      
      freq.increment();
      updateFrequency();
      updateDisplay();
      
      break;
  }

  if (encoder_switch.poll() ) {
    freq.changeRadix();
    updateDisplay();
  }

}