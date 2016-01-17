#include "Cart.h"
#include "CartDumper.h"

void Cart::selectBank(uint8_t bank) {
	if (bankCount > 1 && bank < bankCount) {
		CartDumper::accessHotspot(hotspots[bank]);
	}
}

