#ifndef ARDUINO
#include <cstdio>
#include "WiringVCS.h"

#define NBYTES 32

int main(void) {

	VCS.setup();

	Cart* cart = VCS.findCart();

	printf("Found cart of type %s. Dumping to rom.bin\n", VCS.getMapperName(cart->mapper));

	// array to save a rom segment in
	uint8_t romSegment[NBYTES];

	FILE* file = fopen("rom.bin", "wb");

	for (int offset = 0; offset < cart->size; offset += NBYTES) {
		VCS.dump(*cart, romSegment, offset, NBYTES);
		fwrite(romSegment, 1, NBYTES, file);
	}

	fclose(file);

	delete cart;

	return 0;
}

#endif
