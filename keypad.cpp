#include <iostream>
#include <stdlib.h>

using namespace std;

//Implementation of the Gameboy keypad

class Key {
	int key_1 = 0x0F;
	int key_2 = 0x0F;
	int col = 0;

	void reset() {
		key_1 = 0x0F;
		key_2 = 0x0F;
		col = 0;
	}
	int rb() {
		switch(colidx) {
			case 0x00:
				return 0x00;
			case 0x10:
				return key_1;
			case 0x20:
				return key_2;
			default:
				return 0x00;
		}
	}
	void wb(int v) {
		col = v & 0x30;
	}
	//resets the appropriate bit
	void keydown(int e) {
		switch(e) {
			case 39:
				key_2 &= 0xE;
				break;
			case 37:
				key_2 &= 0xD;
				break;
			case 38:
				key_2 &= 0xB;
				break;
			case 40:
				key_2 &= 0x7;
				break;
			case 90:
				key_1 &= 0xE;
				break;
			case 88:
				key_1 &= 0xD;
				break;
			case 32:
				key_1 &= 0xB;
				break;
			case 13:
				key_1 &= 0x7;
				break;
		}
	}
	//sets the appropriate bit
	void keyup(int e) {
		switch(e) {
			case 39:
				key_2 |= 0x1;
				break;
			case 37:
				key_2 |= 0x2;
				break;
			case 38:
				keys_2 |= 0x4;
				break;
			case 40:
				key_2 |= 0x8;
				break;
			case 90:
				key_1 |= 0x1;
				break;
			case 88:
				key_1 |= 0x2;
				break;
			case 32:
				key_1 |= 0x5;
				break;
			case 13:
				key_1 |= 0x8;
				break;
		}
	}
};