#include "CartDumper.h"

void CartDumper::setup() {
	#ifndef ARDUINO
	wiringPiSetup();
	pinMode(BUFDIR, OUTPUT);
	digitalWrite(BUFDIR, LOW);
	#endif

	pinMode(D0, INPUT);
	pinMode(D1, INPUT);
	pinMode(D2, INPUT);
	pinMode(D3, INPUT);
	pinMode(D4, INPUT);
	pinMode(D5, INPUT);
	pinMode(D6, INPUT);
	pinMode(D7, INPUT);

	pinMode(A0, OUTPUT);
	pinMode(A1, OUTPUT);
	pinMode(A2, OUTPUT);
	pinMode(A3, OUTPUT);
	pinMode(A4, OUTPUT);
	pinMode(A5, OUTPUT);
	pinMode(A6, OUTPUT);
	pinMode(A7, OUTPUT);
	pinMode(A8, OUTPUT);
	pinMode(A9, OUTPUT);
	pinMode(A10, OUTPUT);
	pinMode(A11, OUTPUT);

	pinMode(CS, OUTPUT);
	digitalWrite(CS, HIGH);
}

CartDumper::CartDumper() {
	setup();
}

Cart* CartDumper::findCart() {
	return new CartE7();
}

void CartDumper::setAddress(uint16_t addr) {
	digitalWrite(A11, 0x1 & (addr >> 11));
	digitalWrite(A10, 0x1 & (addr >> 10));
	digitalWrite(A9, 0x1 & (addr >> 9));
	digitalWrite(A8, 0x1 & (addr >> 8));
	digitalWrite(A7, 0x1 & (addr >> 7));
	digitalWrite(A6, 0x1 & (addr >> 6));
	digitalWrite(A5, 0x1 & (addr >> 5));
	digitalWrite(A4, 0x1 & (addr >> 4));
	digitalWrite(A3, 0x1 & (addr >> 3));
	digitalWrite(A2, 0x1 & (addr >> 2));
	digitalWrite(A1, 0x1 & (addr >> 1));
	digitalWrite(A0, 0x1 & addr);
	delayMicroseconds(READ_DELAY);
}

bool inArray(uint16_t needle, uint16_t arr[], uint16_t len) {
	bool found = false;
	for (int i = 0; i < len; i++) {
		if (arr[i] == needle) {
			found = true;
			break;
		}
	}
	return found;
}

uint8_t CartDumper::readByte(Cart& cart, uint16_t addr) {
	// don't return data for hotspots to prevent unwanted bankswitches
	if (inArray(addr, cart.hotspotList, cart.hotspotCount)) {
		return 0;
	} 
	setAddress(addr);

	uint8_t d0 = digitalRead(D0);
	uint8_t d1 = digitalRead(D1);
	uint8_t d2 = digitalRead(D2);
	uint8_t d3 = digitalRead(D3);
	uint8_t d4 = digitalRead(D4);
	uint8_t d5 = digitalRead(D5);
	uint8_t d6 = digitalRead(D6);
	uint8_t d7 = digitalRead(D7);

	uint8_t val = (d7 << 7) + (d6 << 6) + (d5 << 5) + (d4 << 4) + (d3 << 3) + (d2 << 2) + (d1 << 1) + d0;

	return val;
}

uint16_t CartDumper::readNBytes(Cart& cart, uint8_t* buf, uint16_t addr, uint16_t nbytes) {
	for(int i = 0; i < nbytes; i++) {
		buf[i] = readByte(cart, addr + i); 
	}

	return nbytes;
}

void CartDumper::accessHotspot(uint16_t addr) {
	digitalWrite(CS, LOW);
	delayMicroseconds(READ_DELAY);
	setAddress(addr);
	digitalWrite(CS, HIGH);
	delayMicroseconds(READ_DELAY);
}

uint16_t CartDumper::dump(Cart& cart, uint8_t* romBuffer, uint16_t offset, uint16_t nbytes) {
	uint16_t bytesRead = 0;
	uint16_t startBank = cart.getBank(offset);
	uint16_t endBank = min(cart.bankCount - 1, (int)((offset + nbytes) / cart.bankSize)); 

	bool firstRead = true;
	for(uint8_t bankNo = startBank; bankNo <= endBank; bankNo++) {
		uint16_t bytesRemaining = nbytes - bytesRead;

		if (bytesRemaining == 0) continue;

		if (cart.selectedBank != bankNo) {
			cart.selectBank(bankNo);
		}

		uint16_t startAddress = cart.bankAddress + (firstRead ? offset % cart.bankSize : 0);
		
		bytesRead += readNBytes(cart, romBuffer + bytesRead, startAddress, min(bytesRemaining, cart.bankSize));

		firstRead = false;
	}

	return bytesRead;
}

