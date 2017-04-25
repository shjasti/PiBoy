#include "CPU.cpp"

using namespace std;

class Timer {
	Z80 z;
	MMU m;
	int div = 0;
	int tma = 0;
	int tima = 0;
	int tac = 0;

	int clock[3] = {0};

	void reset() {
		div = 0;
		tma = 0;
		tima = 0;
		tac = 0;
		clock[0] = 0;
		clock[1] = 0;
		clock[2] = 0;
	}
	void step() {
		++tima;
		clock[0] = 0;
		if (tima > 255) {
			tima = tma;
			m.if != 4;
		}
	}
	void inc() {
		int oldclk = clock[0];
		clock[1] = z.r[10];
		if (clock[1] > 3) {
			clock[0] = clock[0] + 1;
			clock[1] = clock[1] - 4;
			clock[2] = clock[2] + 1;
			if (clock[2] == 16) {
				clock[2] = 0;
				++div;
				div &= 255;
			}
		}
		if (tac & 4) {
			switch(tac & 3) {
				case 0:
					if (clock[0] >= 64) {
						step();
					}
					break;
				case 1:
					if (clock[0] >= 1) {
						step();
					}
					break;
				case 2:
					if (clock[0] >= 4) {
						step();
					}
					break;
				case 3:
					if (clock[0] >= 16) {
						step();
					}
					break;
			}
		}
	}
	int rb(int addr) {
		switch(addr) {
			case 0xFF04: 
				return div;
			case 0xFF05:
				return tima;
			case 0xFF06:
				return tma;
			case 0xFF07:
				return tac;
		}
	}
	void wb(int addr, int val) {
		switch(addr) {
			case 0xFF04:
				div = 0;
				break;
			case 0xFF05:
				tima = val;
				break;
			case 0xFF06:
				tma = val;
				break;
			case 0xFF07:
				tac = val & 7;
				break;
		}
	}
};