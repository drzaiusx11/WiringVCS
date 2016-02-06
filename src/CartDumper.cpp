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

const char* CartDumper::getMapperName(Mapper mapper) {
	switch(mapper) {
	case M_E0:
		return "E0";
	break;
	case M_E7:
		return "E7";
	break;
	case M_F4:
		return "F4";
	break;
	case M_F6:
		return "F6";
	break;
	case M_F8:
		return "F8";
	break;
	case M_FE:
		return "FE";
	break;
	default: // 4K
		return "4K";
	}
}

Mapper CartDumper::detectMapper() {
	if (isE0()) {
		return M_E0;
	}
	if (isE7()) {
		return M_E7;
	}
	if (isF4()) {
		return M_F4;
	}
	if (isF6()) {
		return M_F6;
	}
	if (isF8()) {
		return M_F8;
	}
	if (isFE()) {
		return M_FE;
	}
	if (is2K()) {
		return M_2K;
	}
	return M_4K; // default to 4k for unknowns
}

Cart* CartDumper::findCart() {
	Mapper mapper = detectMapper();
	Cart* cart;

	switch(mapper) {
	case M_E0:
		cart = new CartE0();
	break;
	case M_E7:
		cart = new CartE7();
	break;
	case M_F4:
		cart = new CartF4();
	break;
	case M_F6:
		cart = new CartF6();
	break;
	case M_F8:
		cart = new CartF8();
	break;
	case M_FE:
		cart = new CartFE();
	break;
	default: // 4K
		cart = new Cart(0x1000, 1, 0, M_4K);
	}

	return cart;
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

uint8_t CartDumper::readByte(Cart* cart, uint16_t addr) {
	// don't return data for hotspots to prevent unwanted bankswitches
	if (cart != NULL && inArray(addr, cart->hotspotList, cart->hotspotCount)) {
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

uint16_t CartDumper::readNBytes(Cart* cart, uint8_t* buf, uint16_t addr, uint16_t nbytes) {
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
		
		bytesRead += readNBytes(&cart, romBuffer + bytesRead, startAddress, min(bytesRemaining, cart.bankSize));

		firstRead = false;
	}

	return bytesRead;
}

// 8 1K banks selectable into 4 slots, 
// but last bank is always at the last slot (0xC00 - 0xFFF)
// +------------------+
// | slot 0 (000-3FF) | banks selectable via: FE0 - FE7
// +------------------+
// | slot 1 (400-7FF) | ": FE8 - FEF
// +------------------+
// | slot 2 (800-BFF) | ": FF0 - FF7 
// +------------------+
// | slot 3 (C00-FFF) | (always contains last 1k rom bank of the 8k)
// +------------------+
bool CartDumper::isE0() {  
	uint8_t bank0[NCOMPARES];
	uint8_t bank6[NCOMPARES];
	uint8_t bank7[NCOMPARES];
	uint8_t bank7Verify[NCOMPARES];

	// put bank0 into slot 1
	accessHotspot(0xFE8);

	// read from slot 1
	readNBytes(NULL, bank0, 0x400, NCOMPARES);

	// in E0, the last 0xC00 - 0xFFF never changes
	readNBytes(NULL, bank7, 0xC00, NCOMPARES);

	// put bank 6 into slot 1
	accessHotspot(0xFEF);

	// read from slot 1
	readNBytes(NULL, bank6, 0x400, NCOMPARES);

	// last slot should never change (always stuck at bank 7)
	readNBytes(NULL, bank7Verify, 0xC00, NCOMPARES);

	// different banks on the same slot should differ, last should stay the same, other slots should stay the same
	bool isE0 = strncmp((char*)bank0, (char*)bank6, NCOMPARES) != 0 && !strncmp((char*)bank7, (char*)bank7Verify, NCOMPARES);
  
	return isE0;
}

//////////////////////////////////////////////////
// Cart E7
//
// 8 2K banks with seletable RAM
//
// 0xFE0 through 0xFE6 selects bank 0 - 6 into 0-0x7FF
// 0xFE7 enables 1K of the 2K RAM into 0-0x7FF
// 0x800 through 0x9FF has additional RAM (512K - 256K Write/256K Read)
// 0xFE8 through 0xFEB selects which 256K RAM bank appears
// 0xA00 through 0xFFF is fixed to the last 1.5K of ROM
//////////////////////////////////////////////////
bool CartDumper::isE7() {
	uint8_t bank0[NCOMPARES];
	uint8_t bank1[NCOMPARES];
	uint8_t bank7[NCOMPARES];
	uint8_t bank7Verify[NCOMPARES];

	// put bank 0 into address 0 - 7FF
	accessHotspot(0xFE0);

	// read bank 0
	readNBytes(NULL, bank0, 0, NCOMPARES);

	// last 2k bank starts at 0x800, but that points to RAM
	// so start at first non-ram address
	readNBytes(NULL, bank7, 0xA00, NCOMPARES);

	// put bank 1 into address 0 - 7FF
	accessHotspot(0xFE1);

	// read bank 1
	readNBytes(NULL, bank1, 0, NCOMPARES);

	// in E0, the last 0xA00 - 0xFFF never changes
	readNBytes(NULL, bank7Verify, 0xA00, NCOMPARES);
  
	bool isE7 = strncmp((char*)bank0, (char*)bank1, NCOMPARES) != 0 && !strncmp((char*)bank7, (char*)bank7Verify, NCOMPARES);  
  
	return isE7;
}

// untested, are there even any F4 commercial carts?
bool CartDumper::isF4() {
	uint8_t bank0[NCOMPARES];
	uint8_t bank1[NCOMPARES];
	accessHotspot(0xFF4); // select bank 0
	readNBytes(NULL, bank0, 0, NCOMPARES);
	accessHotspot(0xFF5); // select bank 1
	readNBytes(NULL, bank1, 0, NCOMPARES);
	bool isF4 = strncmp((char*)bank0, (char*)bank1, NCOMPARES) != 0;
	return isF4;
}

// should be called after isF4, due to overlap
bool CartDumper::isF6() {
	uint8_t bank0[NCOMPARES];
	uint8_t bank1[NCOMPARES];
	accessHotspot(0xFF6); // select bank 0
	readNBytes(NULL, bank0, 0, NCOMPARES);
	accessHotspot(0xFF7); // select bank 1
	readNBytes(NULL, bank1, 0, NCOMPARES);
	bool isF6 = strncmp((char*)bank0, (char*)bank1, NCOMPARES) != 0;
	return isF6;
}

// should be called after isF6, due to overlap
bool CartDumper::isF8() {
	uint8_t bank0[NCOMPARES];
	uint8_t bank1[NCOMPARES];
	accessHotspot(0xFF8); // select bank 0
	readNBytes(NULL, bank0, 0, NCOMPARES);
	accessHotspot(0xFF9); // select bank 1
	readNBytes(NULL, bank1, 0, NCOMPARES);
	bool isF8 = strncmp((char*)bank0, (char*)bank1, NCOMPARES) != 0;
	return isF8;
}

bool CartDumper::isFE() {
	uint8_t bank0[NCOMPARES];
	uint8_t bank1[NCOMPARES];

	// grab bank 0 and 1
	for (int i = 0; i <= 1; i++) {
		// de-select cart
		digitalWrite(CS, LOW);

		#ifndef ARDUINO
		// reverse buffer to WRITE mode
		digitalWrite(BUFDIR, HIGH);
		#endif

		pinMode(D5, OUTPUT);
		digitalWrite(D5, i);

		// set hotspot
		setAddress(0x1FE);
		delayMicroseconds(READ_DELAY);

		// restore D5 as input, select chip
		digitalWrite(CS, HIGH);
		pinMode(D5, INPUT);

		#ifndef ARDUINO
		// restore buffer as READ mode
		digitalWrite(BUFDIR, LOW);
		#endif

		readNBytes(NULL, i == 0 ? bank1 : bank0, 0, NCOMPARES);
	}

	bool isFE = strncmp((char*)bank0, (char*)bank1, NCOMPARES) != 0;
	return isFE;
}

bool CartDumper::is2K() {
	uint8_t front[NCOMPARES];
	uint8_t mirror[NCOMPARES];
	readNBytes(NULL, front, 0, NCOMPARES);
	readNBytes(NULL, mirror, 0x800, NCOMPARES);
	bool is2K = strncmp((char*)front, (char*)mirror, NCOMPARES) != 0;
	return is2K;
}

