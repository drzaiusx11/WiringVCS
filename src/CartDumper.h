#ifndef CartDumper_H
#define CartDumper_H

#include "Cart.h"

#define READ_DELAY 25
#define noop 

// number of bytes for bank compares
#define NCOMPARES 128

#if defined(ARDUINO)
	#include <Arduino.h>

	#define	D0 1
	#define	D1 2
	#define	D2 3
	#define	D3 4
	#define	D4 5
	#define	D5 6
	#define	D6 7
	#define	D7 8

	// chip select (aka A12)
	#define CS 9

	// address bus
	#define	A0 10
	#define	A1 11
	#define	A2 12
	#define	A3 13
	#define	A4 14
	#define	A5 15
	#define	A6 16
	#define	A7 17
	#define	A8 18
	#define	A9 19
	#define	A10 20
	#define	A11 21
#else // Raspberry Pi

	#include <wiringPi.h>

	#define min(a,b) (((a)<(b))?(a):(b))
	#define max(a,b) (((a)>(b))?(a):(b))

	#define	GPIO_17 0
	#define	GPIO_18 1
	#define	GPIO_27 2
	#define	GPIO_22 3
	#define	GPIO_23 4
	#define	GPIO_24 5
	#define	GPIO_25 6
	#define	GPIO_4 7
	#define	GPIO_SDA 8
	#define	GPIO_SCL 9
	#define	GPIO_CE0 10
	#define	GPIO_CE1 11
	#define	GPIO_MOSI 12
	#define	GPIO_MISO 13
	#define	GPIO_SCLK 14
	#define	GPIO_TX 15
	#define	GPIO_RX 16
	#define	GPIO_5 21
	#define	GPIO_6 22
	#define	GPIO_13 23
	#define	GPIO_19 24
	#define	GPIO_26 25
	#define	GPIO_12 26
	#define	GPIO_16 27
	#define	GPIO_20 28
	#define	GPIO_21 29

	// data bus
	#define	D0 GPIO_TX
	#define	D1 GPIO_RX
	#define	D2 GPIO_18
	#define	D3 GPIO_22
	#define	D4 GPIO_24
	#define	D5 GPIO_25
	#define	D6 GPIO_CE0
	#define	D7 GPIO_CE1

	// chip select (aka A12)
	#define CS GPIO_23

	// 74LVC245's buffer dir pin
	#define	BUFDIR GPIO_27

	// address bus
	#define	A0 GPIO_26
	#define	A1 GPIO_19
	#define	A2 GPIO_13
	#define	A3 GPIO_6
	#define	A4 GPIO_5
	#define	A5 GPIO_SCLK
	#define	A6 GPIO_MISO
	#define	A7 GPIO_MOSI
	#define	A8 GPIO_21
	#define	A9 GPIO_20
	#define	A10 GPIO_16
	#define	A11 GPIO_12

#endif

/////////////////CART PINOUT/////////////////
// GND.5V.A8.A9.A11.A10.EN.D7.D6.D5.D4.D3  //
// A7..A6.A5.A4.A3..A2..A1.A0.D0.D1.D2.GND //
/////////////////////////////////////////////

class CartDumper {
	public:
	CartDumper();
	static void setup();
	static const char* getMapperName(Mapper mapper);
	static Mapper detectMapper();
	static Cart* findCart();
	static uint16_t dump(Cart& cart, uint8_t* rom, uint16_t offset, uint16_t nbytes);
	static uint8_t readByte(Cart* cart, uint16_t addr);
	static uint16_t readNBytes(Cart* cart, uint8_t* buf, uint16_t addr, uint16_t nbytes);
	static void setAddress(uint16_t addr);
	static void accessHotspot(uint16_t addr);
	static bool isE0();
	static bool isE7();
	static bool isF4();
	static bool isF6();
	static bool isF8();
	static bool isFE();
	static bool is2K();
};

#endif //CartDumper_H
