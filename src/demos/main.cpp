#ifndef ARDUINO

#include <stdlib.h>
#include <stdio.h>
#include "WiringVCS.h"

int main(void) {
	uint8_t rom[4096];

	FILE* file = fopen("rom.bin", "wb");

	VCS.setup();

	Cart cart = VCS.findCart();

	VCS.dump(cart, rom, 0, cart.size);

	fwrite(rom, 1, cart.size, file);

	fclose(file);

	return 0;
}

#endif
