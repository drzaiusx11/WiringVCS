# WiringVCS
Arduino and Raspberry Pi compatible library for manipulating Atari 2600 cartridges

##### Example 1:
Print out some details about the attached cartridge
```c
#include <WiringVCS.h>

void setup() {
  VCS.setup();
  
  Cart* cart = VCS.findCart();

  Serial.print("Cart Mapper Type: ");
  Serial.println(VCS.getMapperName(cart->mapper));
  
  Serial.print("Cart ROM Size: ");
  Serial.println(cart->size);
  
  Serial.print("Cart Bank Size: ");
  Serial.println(cart->bankSize);
  
  Serial.print("Number of banks: ");
  Serial.println(cart->bankCount);
  
  if (cart->ramSize > 0) {
    Serial.print("Cart RAM Size: ");
    Serial.println(cart->ramSize);
  }
}
```

##### Example 2:
Dump the entire contents of the attached cartridge's ROM to the serial port, 32 bytes at a time:
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
