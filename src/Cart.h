#ifndef Cart_H
#define Cart_H

#include <inttypes.h>

enum Mapper {
	M_2K,
	M_4K,
	M_E0,
	M_F8
};

class Cart {
	public:
	// overall size of cart
	uint16_t size;

	// size of a single bank
	uint16_t bankSize;

	// number of banks in the cart
	uint8_t bankCount;

	// stores array of hotspots 
	uint16_t* hotspots;
	
	// number of hotspots
	uint8_t hotspotCount;

	// sets the selected bank (0 - bankCount-1)
	virtual void selectBank(uint8_t bank);

	// mapper type, eg M_F8,etc
	Mapper mapper;

	Cart(uint8_t bnkcnt, uint16_t bnksz, uint8_t hscnt, Mapper m) :
		bankCount(bnkcnt),
		bankSize(bnksz),
		size(bnksz*bnkcnt),
		hotspotCount(hscnt),
		mapper(m),
		hotspots(new uint16_t[hscnt]) {}
};

class CartE0 : public Cart {
	public:
	CartE0()
	: Cart(
		8, // # of banks
		1024, // bank size
		8, // # of hotspots
		Mapper::M_E0
	) {
		hotspots[0] = 0xFE0;
		hotspots[1] = 0xFE1;
		hotspots[2] = 0xFE2;
		hotspots[3] = 0xFE3;
		hotspots[4] = 0xFE4;
		hotspots[5] = 0xFE5;
		hotspots[6] = 0xFE6;
		hotspots[7] = 0xFE7;
	}
};

class CartF8 : public Cart {
	public:
	CartF8()
	: Cart(
		2, // # of banks
		4096, // bank size
		2, // # of hotspots
		Mapper::M_F8
	) {
		hotspots[0] = 0xFF8;
		hotspots[1] = 0xFF9;
	}
};

#endif //Cart_H
