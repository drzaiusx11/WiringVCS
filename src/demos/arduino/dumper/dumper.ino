#include <WiringVCS.h>

#define NBYTES 32

void setup() {
  VCS.setup();
  
  Cart* cart = VCS.findCart();

  uint8_t romSegment[NBYTES];

  for (int offset = 0; offset < cart->size; offset += NBYTES) {
    VCS.dump(*cart, romSegment, offset, NBYTES);
    Serial.write(romSegment, NBYTES);
  }
}

void loop() {
  // nothing to see here folks...
}

