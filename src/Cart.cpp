#include "Cart.h"
#include "CartDumper.h"

void Cart::selectBank(uint8_t bank) {}

void CartE0::selectBank(uint8_t bank) {
	CartDumper::accessHotspot(hotspots[bank]);
}

