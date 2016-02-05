#include "Cart.h"
#include "CartDumper.h"

uint8_t Cart::getBank(uint16_t address) {
	return (uint8_t)(address / bankSize);
}

uint16_t Cart::selectBank(uint8_t bankNo) {
	if (bankCount > 1 && bankNo < bankCount && bankNo >= 0) {
		CartDumper::accessHotspot(hotspotList[bankNo]);
		selectedBank = bankNo;
		bankAddress = 0;
	}
	else {
		return -1;
	}

	return bankAddress;
}

uint16_t CartE7::selectBank(uint8_t bankNo) {
	// only allow valid bankNo values
	if (bankNo < 0 || bankNo > 7) return -1;

	// banks 0 - 6 use hotspots
	if (bankNo < 7) {
		CartDumper::accessHotspot(hotspotList[bankNo]);
		bankAddress = 0;
	}
	// bank 7 is static and always lives at 0x800
	else if (bankNo == 7) {
		bankAddress = 0x800;
	}

	selectedBank = bankNo;

	return bankAddress;
}

