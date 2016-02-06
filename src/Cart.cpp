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

uint16_t CartFE::selectBank(uint8_t bankNo) {
	// only allow valid bankNo values
	if (bankNo < 0 || bankNo > 1) return -1;

	uint8_t d5 = 0;
	if (bankNo == 0) {
		d5 = 1;
	}

	selectedBank = bankNo;

	// de-select cart
	digitalWrite(CS, LOW);
	delayMicroseconds(READ_DELAY);

	#ifndef ARDUINO
	// reverse buffer (make D0 - D7 writable)
	digitalWrite(BUFDIR, HIGH);
	delayMicroseconds(READ_DELAY);
	#endif

	pinMode(D5, OUTPUT);
	digitalWrite(D5, d5);
	delayMicroseconds(READ_DELAY);

	// set hotspot
	CartDumper::setAddress(0x1FE);
	delayMicroseconds(READ_DELAY);

	// select cart
	digitalWrite(CS, HIGH);
	delayMicroseconds(READ_DELAY);
	pinMode(D5, INPUT);

	#ifndef ARDUINO
	// restore buffer as read-only
	digitalWrite(BUFDIR, LOW);
	delayMicroseconds(READ_DELAY);
	#endif
}

