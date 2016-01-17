#include <cstdio>
#include "WiringVCS.h"

#define NBYTES 32

int main(void) {

	VCS.setup();

	Cart* cart = VCS.findCart();

	// array to save a rom segment in
	uint8_t romPartial[NBYTES];

	FILE* file = fopen("rom.bin", "wb");

	for (int offset = 0; offset < cart->size; offset += NBYTES) {
		VCS.dump(*cart, romPartial, offset, NBYTES);
		fwrite(romPartial, 1, NBYTES, file);
	}

	fclose(file);

	return 0;
}

