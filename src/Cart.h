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
	virtual void selectBank(uint8_t bank);
	Mapper mapper;
	Cart(uint8_t b, uint16_t bs, Mapper m) :
		banks(b),
		bankSize(bs),
		mapper(m) {}

};

class CartE0 : public Cart {
	public:
	CartE0() : Cart(8, 1024, Mapper::M_E0) {}
	uint16_t hotspots[8] = { 0xFE0, 0xFE1, 0xFE2, 0xFE3, 0xFE4, 0xFE5, 0xFE6, 0xFE7 };
	void selectBank(uint8_t bank);
};

#endif //Cart_H
