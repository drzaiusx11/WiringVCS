#ifndef ARDUINO

#include <stdlib.h>
#include <stdio.h>
#include "WiringVCS.h"

#define NBYTES 8

int main(void) {

	VCS.setup();

	Cart cart = VCS.findCart();

	uint8_t romPartial[NBYTES];
	FILE* file = fopen("rom.bin", "wb");

	for (int offset = 0; offset < cart.size; offset += NBYTES) {
		VCS.dump(cart, romPartial, offset, NBYTES);
		fwrite(romPartial, 1, NBYTES, file);
	}

	fclose(file);

	return 0;
}

#endif
