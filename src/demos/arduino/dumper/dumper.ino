#include <WiringVCS.h>

#define NBYTES 32

void setup() {
  VCS.setup();
  
  Cart cart = VCS.findCart();

  uint8_t partialRom[NBYTES];

  for (int offset = 0; offset < cart.size; offset += NBYTES) {
    VCS.dump(cart, partialRom, offset, NBYTES);
    Serial.write(partialRom, NBYTES);
  }
}

void loop() {
  // put your main code here, to run repeatedly:
}
