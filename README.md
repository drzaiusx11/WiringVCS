# WiringVCS
Arduino and Raspberry Pi compatible library for manipulating Atari 2600 cartridges

##### Examples
Dump the entire contents of a cartridge's ROM to the serial port on an arduino, 32 bytes at a time:
```c
#include <WiringVCS.h>

#define SIZE 32

void setup() {
  VCS.setup();
  
  Cart* cart = VCS.findCart();

  uint8_t romSegment[SIZE];

  for (int offset = 0; offset < cart->size; offset += SIZE) {
    VCS.dump(*cart, romSegment, offset, SIZE);
    Serial.write(romSegment, SIZE);
  }
}
```
