#ifndef Cart_H
#define Cart_H

#include <cinttypes>
#include <cstdio>

enum Mapper {
	M_2K,
	M_4K,
	M_E0,
	M_E7,
	M_F8
};

class Cart {
	public:
	// overall size of cart
	uint16_t size;

	// bank size in the cart
	uint16_t bankSize;

	uint16_t* hotspotList;

	uint8_t hotspotCount;

	// number of banks in the cart
	uint8_t bankCount;

	uint8_t selectedBank;

	uint16_t bankAddress;

	// sets the selected bank (0 - bankCount-1)
	// retuns starting address of bank
	virtual uint16_t selectBank(uint8_t bank);

	// get the bank for a given address
	virtual uint8_t getBank(uint16_t address);

	// mapper type, eg M_F8,etc
	Mapper mapper;

	Cart(uint16_t bnksz, uint8_t bnkcnt, uint8_t hscnt, Mapper m) :
		size(bnksz * bnkcnt),
		bankSize(bnksz),
		bankCount(bnkcnt),
		hotspotCount(hscnt),
		hotspotList(new uint16_t[hscnt]),
		selectedBank(-1), // init to 'invalid' bank
		bankAddress(0),
		mapper(m) {}
	~Cart() {}
};

//////////////////////////////////////////////////
// Cart E0
//
// 8 1K banks visable via 4 "slices"
// slice 0 - 2 can access any 1K block via hotspots:
// 0xFE0 - 0xFE7 selects bank# in "slice" 0
// 0xFE8 - 0xFEF selects bank# in "slice" 1
// 0xFF0 - 0xFF7 selects bank# in "slice" 2
// slice 3 is static and always points to bank 7
//////////////////////////////////////////////////
class CartE0 : public Cart {
	public:
	CartE0()
	: Cart(
		1024,	// banksize
		8,	// # of banks
		24,	// # of hotspots
		Mapper::M_E0
	) {
		for(int i = 0; i < hotspotCount; i++) {
			hotspotList[i] = 0xFE0 + i;
		}
	}
};

//////////////////////////////////////////////////
// Cart E7
//
// 7 2K banks
// 0xFE0 through 0xFE6 selects bank 0 - 6 into 0-0x7FF
// 0xFE7 enables 1K of the 2K RAM into 0-0x7FF
// 0x800 through 0x9FF has additional RAM (512K - 256K Write/256K Read)
// 0xFE8 through 0xFEB selects which 256K RAM bank appears
// 0xA00 through 0xFFF is fixed to the last 1.5K of ROM
//////////////////////////////////////////////////
class CartE7 : public Cart {
	public:
	CartE7()
	: Cart(
		2048,	// banksize
		8,	// # of banks
		8,	// # of hotspots
		Mapper::M_E7
	) {
		for(int i = 0; i < hotspotCount; i++) {
			hotspotList[i] = 0xFE0 + i;
		}
	}
	virtual uint16_t selectBank(uint8_t bank);
};

class CartF8 : public Cart {
	public:
	CartF8()
	: Cart(
		4096,	// banksize
		2,	// # of banks
		2,	// # of hotspots
		Mapper::M_F8
	) {
		hotspotList[0] = 0xFF8;
		hotspotList[1] = 0xFF9;
	}
};

#endif //Cart_H
