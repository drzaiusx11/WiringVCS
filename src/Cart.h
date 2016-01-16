#ifndef Cart_H
#define Cart_H

#include <inttypes.h>

enum Mapper {
	M_2K,
	M_4K,
	M_E0
};

class Cart {
	public:
	uint8_t banks;
	uint16_t bankSize;
	uint16_t size;
	virtual void selectBank(uint8_t bank);
	Mapper mapper;
	Cart(uint8_t b, uint16_t bs, Mapper m) :
		banks(b),
		bankSize(bs),
		size(bs*b),
		mapper(m) {}
};

class CartE0 : public Cart {
	public:
	CartE0() : Cart(8, 1024, Mapper::M_E0) {
		hotspots[0] = 0xFE0;
		hotspots[1] = 0xFE1;
		hotspots[2] = 0xFE2;
		hotspots[3] = 0xFE3;
		hotspots[4] = 0xFE4;
		hotspots[5] = 0xFE5;
		hotspots[6] = 0xFE6;
		hotspots[7] = 0xFE7;
	}
	uint16_t hotspots[8];
	void selectBank(uint8_t bank);
};

#endif //Cart_H
