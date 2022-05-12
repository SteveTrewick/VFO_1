#ifndef frequency_h
#define frequency_h

#include <Arduino.h>

// A dump for frequency state
class Frequency {


  // radix values for VFO adjustment
  uint64_t radixValue[7] = {1, 10, 100, 1000, 10000, 100000, 1000000};

  public :

    // we'll need to come down from uint64_t for display formatting
    struct DisplayParts {
      int mhz;
      int khz;
      int hz;
    };


    // for reference : this == 120 MHz
    // the etherkit library will allow adjustments to 0.01 Hz!
    // so like MMMKKKHHHhh (IYKWIM?)
    // this is the value we will pass to the si5351

    uint64_t siValue = 12000000000;


    int radixIndex = 4; // Mhz, coarse adjustment



    void changeRadix() {
      radixIndex = (radixIndex + 1) % 7;
    }

    // NB get some min max going on here please, avoid rollovers
    // note the multiply by 100 to left shift up to the Hz values
    void increment() {
      siValue += ((uint64_t)radixValue[radixIndex] * 100);
    }

    void decrement() {
      siValue -= ((uint64_t)radixValue[radixIndex] * 100);
    }


    DisplayParts displayParts() {
      // ok, this is fairly nasty, first we're going to divide down to Hz so
      // I don't lose track of the number of zeros, then the mod math cometh

      uint64_t hz_value = siValue / 100;
      uint64_t mhz      = hz_value / 1000000;
      uint64_t khz      = (hz_value / 1000) % 1000;
      uint64_t hz       = hz_value % 1000;
      
      return DisplayParts {.mhz = (int)mhz, .khz = (int)khz, .hz = (int)hz };
    }
};

#endif
