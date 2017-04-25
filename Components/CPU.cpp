#include <iostream>
#include <stdlib.h>
#include <cmath>

using namespace std;

//instruction simulations
class Z80 {
public:
	int m = 0, t = 0, a = 0, b = 0, c = 0, d = 0, e = 0, h = 0, l = 0, f = 0, pc = 0, sp = 0;
	int clock[2] = {m,t};
	int r[12] = {a,b,c,d,e,h,l,f,pc,sp,m,t};
	int halt = 0;
	int stop = 0;

	//r[0] = register a
	//r[1] = register b
	//r[2] = register c
	//r[3] = register d
	//r[4] = register e
	//r[5] = register h
	//r[6] = register l
	//r[7] = f
	//r[8] = pc
	//r[9] = sp
	//r[10] = m
	//r[11] = t

	MMU mm;

	void Add_r_e() {
		r[0] += r[4]; // a += e
		r[7] = 0;
		if (!(r[0] & 255)) {
			r[7] |= 0x80;
		}
		if (r[0] > 255) {
			r[7] |= 0x10;
		}
		r[0] &= 255;
		r[10] = 1;
		r[11] = 4;
	}
	void CPr_b() {
		int i = r[0];
		i -= r[1];
		r[7] != 0x40;
		if (!(i & 255)) {
			r[7] |= 0x80;
		}
		if (i < 0) {
			r[7] = 0x10;
		}
		r[10] = 1;
		r[11] = 4;
	}
	void no_op() {
		r[10] = 1;
		r[11] = 4;
	}
	//Push registers B and C on the stack
	void Push_bc() {
		r[9] = r[9] - 1;
		mm.wb(r[9], r[1]);
		r[9] = r[9] - 1;
		mm.wb(r[9], r[1]);
		r[10] = 3;
		r[11] = 12;
	}
	//Pop registers H and L from the stack
	void Pop_hl() {
		r[6] = mm.rb(r[9]);
		r[9] = r[9] + 1;
		r[5] = mm.rb(z.r[9]);
		r[9] = r[9] + 1;
		r[10] = 3;
		r[11] = 12;
	}
	//Read a byte from absolute loaction into A
	void LDAmm() {
		int addr = mm.rw(r[8]);
		r[8] = z.r[8] + 2;
		r[0] = m.rb(addr);
		r[10] = 4;
		r[11] = 16;
	}

	void reset() {
		for (int i = 0; i < 12; ++i) {
			r[i] = 0;
		}
		clock[0] = 0;
		clock[1] = 0;
		halt = 0;
		stop = 0;
	}
	//void exec()

	//Below are all the opcode instructions
	//Load instructions
	void LDrr_bb() {
		r[1] = r[1];
		r[10] = 1;
	}
	void LDrr_bc() {
		r[1] = r[2];
		r[10] = 1;
	}
	void LDrr_bd() {
		r[1] = r[3];
		r[10] = 1;
	}
	void LDrr_be() {
		r[1] = r[4];
		r[10] = 1;
	}
	void LDrr_bh() {
		r[1] = r[5];
		r[10] = 1;
	}
	void LDrr_bl() {
		r[1] = r[6];
		r[10] = 1;
	}
	void LDrr_ba() {
		r[1] = r[0];
		r[10] = 1;
	}
	void LDrr_ca() {
		r[2] = r[1];
		r[10] = 1;
	}
	void LDrr_cc() {
		r[2] = r[2];
		r[10] = 1;
	}
	void LDrr_cd() {
		r[2] = r[3];
		r[10] = 1;
	}
	void LDrr_ce() {
		r[2] = r[4];
		r[10] = 1;
	}
	void LDrr_ch() {
		r[2] = r[5];
		r[10] = 1;
	}
	void LDrr_cl() {
		r[2] = r[6];
		r[10] = 1;
	}
	void LDrr_ca() {
		r[2] = r[0];
		r[10] = 1;
	}
	void LDrr_db() {
		r[3] = r[1];
		r[10] = 1;
	}
	void LDrr_dc() {
		r[3] = r[2];
		r[10] = 1;
	}
	void LDrr_dd() {
		r[3] = r[3];
		r[10] = 1;
	}
	void LDrr_de() {
		r[3] = r[4];
		r[10] = 1;
	}
	void LDrr_dh() {
		r[3] = r[5];
		r[10] = 1;
	}
	void LDrr_dl() {
		r[3] = r[6];
		r[10] = 1;
	}
	void LDrr_da() {
		r[3] = r[0];
		r[10] = 1;
	}
	void LDrr_eb() {
		r[4] = r[1];
		r[10] = 1;
	}
	void LDrr_ec() {
		r[4] = r[2];
		r[10] = 1;
	}
	void LDrr_ed() {
		r[4] = r[3];
		r[10] = 1;
	}
	void LDrr_ee() {
		r[10] = 1;
	}
	void LDrr_eh() {
		r[4] = r[5];
		r[10] = 1;
	}
	void LDrr_el() {
		r[4] = r[6];
		r[10] = 1;
	}
	void LDrr_ea() {
		r[4] = r[0];
		r[10] = 1;
	}
	void LDrr_hb() {
		r[5] = r[1];
		r[10] = 1;
	}
	void LDrr_hc() {
		r[5] = r[2];
		r[10] = 1;
	}
	void LDrr_hd() {
		r[5] = r[3];
		r[10] = 1;
	}
	void LDrr_he() {
		r[5] = r[4];
		r[10] = 1;
	}
	void LDrr_hh() {
		r[10] = 1;
	}
	void LDrr_hl() {
		r[5] = r[6];
		r[10] = 1;
	}
	void LDrr_ha() {
		r[5] = r[0];
		r[10] = 1;
	}
	void LDrr_lb() {
		r[6] = r[1];
		r[10] = 1;
	}
	void LDrr_lc() {
		r[6] = r[2];
		r[10] = 1;
	}
	void LDrr_ld() {
		r[6] = r[3];
		r[10] = 1;
	}
	void LDrr_le() {
		r[6] = r[4];
		r[10] = 1;
	}
	void LDrr_lh() {
		r[6] = r[5];
		r[10] = 1;
	}
	void LDrr_ll() {
		r[10] = 1;
	}
	void LDrr_la() {
		r[6] = r[0];
		r[10] = 1;
	}
	void LDrr_ab() {
		r[0] = r[1];
		r[10] = 1;
	}
	void LDrr_ac() {
		r[0] = r[2];
		r[10] = 1;
	}
	void LDrr_ad() {
		r[0] = r[3];
		r[10] = 1;
	}
	void LDrr_ae() {
		r[0] = r[4];
		r[10] = 1;
	}
	void LDrr_ah() {
		r[0] = r[5];
		r[10] = 1;
	}
	void LDrr_al() {
		r[0] = r[6];
		r[10] = 1;
	}
	void LDrr_aa() {
		r[10] = 1;
	}


	void LDrHLm_b() {
		r[1] = mm.rb((r[5]<<8)+r[6]);
		r[10] = 2;
	}
	void LDrHLm_c() {
		r[2] = mm.rb((r[5]<<8)+r[6]);
		r[10] = 2;
	}
	void LDrHLm_d() {
		r[3] = mm.rb((r[5]<<8)+r[6]);
		r[10] = 2;
	}
	void LDrHLm_e() {
		r[4] = mm.rb((r[5]<<8)+r[6]);
		r[10] = 2;
	}
	void LDrHLm_h() {
		r[5] = mm.rb((r[5]<<8)+r[6]);
		r[10] = 2;
	}
	void LDrHLm_l() {
		r[6] = mm.rb((r[5]<<8)+r[6]);
		r[10] = 2;
	}
	void LDrHLm_a() {
		r[0] = mm.rb((r[5]<<8)+r[6]);
		r[10] = 2;
	}

	void LDHLmr_b() {
		mm.wb((r[5]<<8)+r[6], r[1]);
		r[10] = 2;
	}
	void LDHLmr_c() {
		mm.wb((r[5]<<8)+r[6], r[2]);
		r[10] = 2;
	}
	void LDHLmr_d() {
		mm.wb((r[5]<<8)+r[6], r[3]);
		r[10] = 2;
	}
	void LDHLmr_e() {
		mm.wb((r[5]<<8), r[4]);
		r[10] = 2;
	}
	void LDHLmr_h() {
		mm.wb((r[5]<<8)+r[6], r[5]);
		r[10] = 2;
	}
	void LDHLmr_l() {
		mm.wb((r[5]<<8)+r[6], r[6]);
		r[10] = 2;
	}
	void LDHLmr_a() {
		mm.wb((r[5]<<8)+r[6], r[0]);
		r[10] = 2;
	}


	void LDrn_b() {
		r[1] = mm.rb(r[8]); //register b
		r[8] = r[8] + 1;	//increment register pc
		r[10] = 2;
	}
	void LDrn_c() {
		r[2] = mm.rb(r[8]);
		r[8] = r[8] + 1;
		r[10] = 2;
	}
	void LDrn_d() {
		r[3] = mm.rb(r[8]);
		r[8] = r[8] + 1;
		r[10] = 2;
	}
	void LDrn_e() {
		r[4] = mm.rb(r[8]);
		r[8] = r[8] + 1;
		r[10] = 2;
	}
	void LDrn_h() {
		r[5] = mm.rb(r[8]);
		r[8] = r[8] + 1;
		r[10] = 2;
	}
	void LDrn_l() {
		r[6] = mm.rb(r[8]);
		r[8] = r[8] + 1;
		r[10] = 2;
	}
	void LDrn_a() {
		r[0] = mm.rb(r[8]);
		r[8] = r[8] + 1;
		r[10] = 2;
	}

	void LDHLmn() {
		mm.wb((r[5]<<8)+r[6], mm.rb(r[8]));
		r[8] = r[8] + 1;
		r[10] = 3;
	}
	void LDBCmA() {
		mm.wb((r[1]<<8)+r[2], r[0]);
		r[10] = 2;
	}
	void LDDEmA() {
		mm.wb((r[3]<<8)+r[4], r[0]);
		r[10] = 2;
	}
	void LDmmA() {
		mm.wb(mm.rw(r[8]), r[0]);
		r[8] = r[8] + 2;
		r[10] = 4;
	}

	void LDABCm() {
		r[0] = mm.rb((r[1]<<8)+r[2]);
	}
	void LDADEm() {
		r[0] = mm.rb((r[3]<<8)+r[4]);
		r[10] = 2;
	}
	void LDAmm() {
		r[0] = mm.rb((mm.rw(r[8])));
		r[10] = 4;
	}
	void LDBCnn() {
		r[2] = mm.rb(r[8]);
		r[1] = mm.rb(r[8]+1);
		r[8] = r[8] + 2;
		r[10] = 3;
	}
	void LDDEnn() {
		r[4] = mm.rb(r[8]);
		r[3] = mm.rb(r[8]+1);
		r[8] += 2;
		r[10] = 3;
	}
	void LDHLnn() {
		r[6] = mm.rb(r[8]);
		r[5] = mm.rb(r[8]+1);
		r[8] += 2;
		r[10] = 3;
	}
	void LDSPnn() {
		r[9] = mm.rw(r[8]);
		r[8] += 2;
		r[10] = 3;
	}

	void LDHLmm() {
		int i = mm.rw(r[8]);
		r[8] += 2;
		r[6] = mm.rb(i);
		r[5] = mm.rb(i+1);
		r[10] = 5;
	}
	void LDmmHL() {
		int i = mm.rw(r[8]);
		r[8] += 2;
		mm.ww(i,(r[5]<<8)+r[6]);
		r[10] = 5;
	}

	void LDHLIA() {
		mm.wb((r[5]<<8)+r[6], r[0]);
		r[6] = (r[6]+1) & 255;
		if (!r[6]) {
			r[5] = (r[5]+1) & 255;
		}
		r[10] = 2;
	}
	void LDAHLI() {
		r[0] = mm.rb((r[5]<<8)+r[6]);
		r[6] = (r[6]+1) & 255;
		if (!r[6]) {
			r[5] = (r[5]+1) & 255;
		}
		r[10] = 2;
	}

	void LDHLDA() {
		mm.wb((r[5]<<8)+r[6], r[0]);
		r[6] = (r[6]-1) & 255;
		if (r[6] == 255) {
			r[5] = (r[5]-1) & 255;
		}
		r[10] = 2;
	}
	void LDAHLD() {
		r[0] = mm.rb((r[5]<<8)+r[6]);
		r[6] = (r[6]-1) & 255;
		if (r[6] == 255) {
			r[5] = (r[5]-1) & 255;
		}
		r[10] = 2;
	}

	void LDAIOn() {
		r[0] = mm.rb(0xFF00+mm.rb(r[8]));
		++r[8];
		r[10] = 3;
	}
	void LDAIOnA() {
		mm.wb(0xFF00+mm.rb(r[8]),r[0]);
		++r[8];
		r[10] = 3;
	}
	void LDAIOC() {
		r[0] = mm.rb(0xFF00+r[2]);
		r[10] = 2;
	}
	void LDAIOA() {
		mm.wb(0xFF00+r[2], r[0]);
		r[10] = 2;
	}

	void LDHLSPn() {
		int i = mm.rb(r[8]);
		if (i > 127) {
			i = -((~i+1)&255);
		}
		++r[8];
		i += r[9];
		r[5] = (i >> 8) & 255;
		r[6] = i & 255;
		r[10] = 3;
	}

	void SWAPr_b() {
		int x = r[1];
		r[1] = ((x&0xF) << 4) | ((x&0xF) >> 4);
		r[7] = r[1]?0x80;
		r[10] = 1;
	}
	void SWAPr_c() {
		int x = r[2];
		r[2] = ((x&0xF) << 4) | ((x&0xF) >> 4);
		r[7] = r[2]?0x80;
		r[10] = 1;
	}
	void SWAPr_d() {
		int x = r[3];
		r[3] = ((x&0xF) << 4) | ((x&0xF) >> 4);
		r[7] = r[3]?0x80;
		r[10] = 1;
	}
	void SWAPr_e() {
		int x = r[4];
		r[4] = ((x&0xF) << 4) | ((x&0xF) >> 4);
		r[7] = r[4]?0x80;
		r[10] = 1;
	}
	void SWAPr_h() {
		int x = r[5];
		r[5] = ((x&0xF) << 4) | ((x&0xF) >> 4);
		r[7] = r[5]?0x80;
		r[10] = 1;
	}
	void SWAPr_l() {
		int x = r[6];
		r[6] = ((x&0xF) << 4) | ((x&0xF) >> 4);
		r[7] = r[6]?0x80;
		r[10] = 1;
	}
	void SWAPr_a() {
		int x = r[0];
		r[0] = ((x&0xF) << 4) | ((x&0xF) >> 4);
		r[7] = r[0]?0x80;
		r[10] = 1;
	}

	//Data processing
	void ADDr_b() {
		int x = r[0];
		r[0] += r[1];
		r[7] = (r[0] > 255)?0x10;
		r[0] &= 255;
		if (!r[0]) {
			r[7] |= 0x80;
		}
		if((r[0]^r[1]^x) & 0x10) {
			r[7] |= 0x20;
		}
		r[10] = 1;
	}
	void ADDr_c() {
		int x = r[0];
		r[0] += r[2];
		r[7] = (r[0] > 255)?0x10;
		r[0] &= 255;
		if (!r[0]) {
			r[7] |= 0x80;
		}
		if ((r[0]^r[2]^x) & 0x10) {
			r[7] |= 0x20;
		}
		r[10] = 1;
	}
	void ADDr_d() {
		int x = r[0];
		r[0] += r[3];
		r[7] = (r[0] > 255)?0x10;
		r[0] &= 255;
		if (!r[0]) {
			r[7] |= 0x80;
		}
		if ((r[0]^r[3]^x) & 0x10) {
			r[7] |= 0x20;
		}
		r[10] = 1;
	}
	void ADDr_e() {
		int x = r[0];
		r[0] += r[4];
		r[7] = (r[0] > 255)?0x10;
		r[0] &= 255;
		if (!r[0]) {
			r[7] |= 0x80;
		}
		if ((r[0]^r[4]^x) & 0x10) {
			r[7] |= 0x20;
		}
		r[10] = 1;
	}
	void ADDr_h() {
		int x = r[0];
		r[0] += r[5];
		r[7] = (r[0] > 255)?0x10;
		r[0] &= 255;
		if (!r[0]) {
			r[7] |= 0x80;
		}
		if ((r[0]^r[5]^x) & 0x10) {
			r[7] |= 0x20;
		}
		r[10] = 1;
	}
	void ADDr_l() {
		int x = r[0];
		r[0] += r[6];
		r[7] = (r[0] > 255)?0x10;
		r[0] &= 255;
		if (!r[0]) {
			r[7] |= 0x80;
		}
		if ((r[0]^r[6]^x) & 0x10) {
			r[7] |= 0x20;
		}
		r[10] = 1;
	}
	void ADDr_a() {
		int x = r[0];
		r[0] += r[0];
		r[7] = (r[0] > 255)?0x10;
		r[0] &= 255;
		if (!r[0]) {
			r[7] |= 0x80;
		}
		if ((r[0]^r[0]^x) & 0x10) {
			r[7] |= 0x20;
		}
		r[10] = 1;
	}
	void ADDHL() {
		int x = r[0];
		int y = mm.rb((r[5]<<8)+r[6]);
		r[0] += y;
		r[7] = (r[0] > 255)?0x10;
		r[0] &= 255;
		if (!r[0]) {
			r[7] |= 0x80;
		}
		if ((r[0]^x^y) & 0x10) {
			r[7] |= 0x20;
		}
		r[10] = 2;
	}
	void ADDn() {
		int x = r[0];
		int y = mm.rb(r[8]);
		r[0] += y;
		++r[8];
		r[7] = (r[0] > 255)?0x10;
		r[0] &= 255;
		if (!r[0]) {
			r[7] |= 0x80;
		}
		if ((r[0]^x^y) & 0x10) {
			r[7] |= 0x20;
		}
		r[10] = 2;
	}
	void ADDHLBC() {
		int hl = (r[5]<<8)+r[6];
		hl += (r[1] << 8) + r[2];
		if (hl > 65535) {
			r[7] |= 0x10;
		}
		else {
			r[7] &= 0xEF;
		}
		r[5] = (h1>>255);
		r[6] = hl & 255;
		r[10] = 3;
	}
	void ADDHLDE() {
		int hl = (r[5]<<8)+r[6];
		hl += (r[3] << 8) + r[4];
		if (hl > 65535) {
			r[7] |= 0x10;
		}
		else {
			r[7] &= 0xEF;
		}
		r[5] = (hl >> 255);
		r[6] = hl & 255;
		r[10] = 3;
	}
	void ADDHLHL() {
		int hl = (r[5] << 8) + r[6];
		hl += (r[5] << 8) + r[6];
		if (hl > 65535) {
			r[7] |= 0x10;
		}
		else {
			r[7] &= 0xEF;
		}
		r[5] = (hl >> 255);
		r[6] = hl & 255;
		r[10] = 3;
	}
	void ADDHLSP() {
		int hl = (r[5] << 8) + r[6];
		hl += r[9];
		if (hl > 65535) {
			r[7] |= 0x10;
		}
		else {
			r[7] &= 0xEF;
		}
		r[5] = (hl >> 255);
		r[6] = hl & 255;
		r[10] = 3;
	}
	void ADDSPn() {
		int i = mm.rb(r[8]);
		if (i > 127) {
			i = -((~i+1)&255);
		}
		++r[8];
		r[9] += i;
		r[10] = 4;
	}

	void ADCr_b() {
		int x = r[0];
		r[0] += r[1];
		r[0] += (r[7] & 0x10)?1;
		r[7] = (r[0] > 255)?0x10;
		r[0] &= 255;
		if(!r[0]) {
			r[7] |= 0x80;
		}
		if ((r[0]^r[1]^x)&0x10) {
			r[7] |= 0x20;
		}
		r[10] = 1;
	}
	void ADCr_c() {
		int x = r[0];
		r[0] += r[2];
		r[0] += (r[7] & 0x10) ? 1;
		r[7] = (r[0] > 255) ? 0x10;
		r[0] &= 255;
		if (!r[0]) {
			r[7] |= 0x80;
		}
		if ((r[0]^r[2]^x) & 0x10) {
			r[7] |= 0x20;
		}
		r[10] = 1;
	}
	void ADCr_d() {
		int x = r[0];
		r[0] += r[3];
		r[0] += (r[7] & 0x10) ? 1;
		r[7] = (r[0] > 255) ? 0x10;
		r[0] &= 255;
		if (!r[0]) {
			r[7] |= 0x80;
		}
		if ((r[0]^r[3]^x) & 0x10) {
			r[7] |= 0x20;
		}
		r[10] = 1;
	}
	void ADCr_e() {
		int x = r[0];
		r[0] += r[4];
		r[0] += (r[7] & 0x10) ? 1;
		r[7] = (r[0] > 255) ? 0x10;
		r[0] &= 255;
		if (!r[0]) {
			r[7] |= 0x80;
		}
		if ((r[0]^r[4]^x) & 0x10) {
			r[7] |= 0x20;
		}
		r[10] = 1;
	}
	void ADCr_h() {
		int x = r[0];
		r[0] += r[5];
		r[0] += (r[7] & 0x10) ? 1;
		r[7] = (r[0] > 255) ? 0x10;
		r[0] &= 255;
		if (!r[0]) {
			r[7] |= 0x80;
		}
		if ((r[0]^r[5]^x) & 0x10) {
			r[7] |= 0x20;
		}
		r[10] = 1;
	}
	void ADCr_l() {
		int x = r[0];
		r[0] += r[6];
		r[0] += (r[7] & 0x10) ? 1;
		r[7] = (r[0] > 255) ? 0x10;
		r[0] &= 255;
		if (!r[0]) {
			r[7] |= 0x80;
		}
		if ((r[0]^r[6]^x) & 0x10) {
			r[7] |= 0x20;
		}
		r[10] = 1;
	}
	void ADCr_a() {
		int x = r[0];
		r[0] += r[0];
		r[0] += (r[7] & 0x10) ? 1;
		r[7] = (r[0] > 255) ? 0x10;
		r[0] &= 255;
		if (!r[0]) {
			r[7] |= 0x80;
		}
		if ((r[0]^r[0]^x) & 0x10) {
			r[7] |= 0x20;
		}
		r[10] = 1;
	}
	void ADCHL() {
		int x = r[0];
		int y = mm.rb((r[5]<<8)+r[6]);
		r[0] += y;
		r[0] += (r[7] & 0x10) ? 1;
		r[7] = (r[0] > 255) ? 0x10;
		if (!r[0]) {
			r[7] |= 0x80;
		}
		if ((r[0]^y^x) & 0x10) {
			r[7] |= 0x20;
		}
		r[10] = 2;
	}
	void ADCn() {
		int x = r[0];
		int y = mm.rb((r[8]);
		r[0] += y;
		++r[8];
		r[0] += (r[7] & 0x10) ? 1;
		r[7] = (r[0] > 255) ? 0x10;
		if (!r[0]) {
			r[7] |= 0x80;
		}
		if ((r[0]^y^x) & 0x10) {
			r[7] |= 0x20;
		}
		r[10] = 2;
	}

	//subtraction instructions
	void SUBr_b() {
		int x = r[0];
		r[0] -= r[1];
		r[7] = (r[0] < 0) ? 0x50:0x40;
		r[0] &= 255;
		if (!r[0]) {
			r[7] |= 0x80;
		}
		if ((r[0]^r[1]^x) & 0x10) {
			r[7] |= 0x20;
		}
		r[10] = 1;
	}
	void SUBr_c() {
		int x = r[0];
		r[0] -= r[2];
		r[7] = (r[0] < 0) ? 0x50:0x40;
		r[0] &= 255;
		if (!r[0]) {
			r[7] |= 0x80;
		}
		if ((r[0]^r[2]^x) & 0x10) {
			r[7] |= 0x20;
		}
		r[10] = 1;
	}
	void SUBr_d() {
		int x = r[0];
		r[0] -= r[3];
		r[7] = (r[0] < 0) ? 0x50:0x40;
		r[0] &= 255;
		if (!r[0]) {
			r[7] |= 0x80;
		}
		if ((r[0]^r[3]^x) & 0x10) {
			r[7] |= 0x20;
		}
		r[10] = 1;
	}
	void SUBr_e() {
		int x = r[0];
		r[0] -= r[4];
		r[7] = (r[0] < 0) ? 0x50:0x40;
		r[0] &= 255;
		if (!r[0]) {
			r[7] |= 0x80;
		}
		if ((r[0]^r[4]^x) & 0x10) {
			r[7] |= 0x20;
		}
		r[10] = 1;
	}
	void SUBr_h() {
		int x = r[0];
		r[0] -= r[5];
		r[7] = (r[0] < 0) ? 0x50:0x40;
		r[0] &= 255;
		if (!r[0]) {
			r[7] |= 0x80;
		}
		if ((r[0]^r[5]^x) & 0x10) {
			r[7] |= 0x20;
		}
		r[10] = 1;
	}
	void SUBr_l() {
		int x = r[0];
		r[0] -= r[6];
		r[7] = (r[0] < 0) ? 0x50:0x40;
		r[0] &= 255;
		if (!r[0]) {
			r[7] |= 0x80;
		}
		if ((r[0]^r[6]^x) & 0x10) {
			r[7] |= 0x20;
		}
		r[10] = 1;
	}
	void SUBr_a() {
		int x = r[0];
		r[0] -= r[0];
		r[7] = (r[0] < 0) ? 0x50:0x40;
		r[0] &= 255;
		if (!r[0]) {
			r[7] |= 0x80;
		}
		if ((r[0]^r[0]^x) & 0x10) {
			r[7] |= 0x20;
		}
		r[10] = 1;
	}
};
