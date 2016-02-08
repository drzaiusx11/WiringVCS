# WiringVCS
Arduino and Raspberry Pi compatible library for manipulating Atari 2600 cartridges

##### Setup
###### Making a harness to attach cartridges to your arduino
Pinout of a VCS cartridge, the bottom facing you with the label facing up:
```
+--------------------LABEL SIDE-------------------+
| D03 D04 D05 D06 D07 CSE A10 A11 A09 A08 VCC GND |
| GND D02 D01 D00 A00 A01 A02 A03 A04 A05 A06 A07 |
+-------------------------------------------------+
```
Using an old floppy disk cable or 2x24 0.1" edge connector (search ebay), 
attach it to the cartridge and run a wire to each lead.  Attach each wire to an IO
pin on your arduino noting which IO number corresponds to which pinout from the VCS.

Edit WiringVCS/CartDumper.h to have each of the following match the IO #s on your arduino:
A0-A11, CS, D0-D7. Additionally attach GNDs from the cart to GND on your arduino. VCC on the 
cart should be connected to 5V on your arduino.

Then load up an example and give it a go!
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
