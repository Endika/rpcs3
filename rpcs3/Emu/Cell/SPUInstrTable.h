#pragma once
#include "PPCDecoder.h"
#include "SPUOpcodes.h"

namespace SPU_instr
{
	static CodeField<4, 10> RC(FIELD_R_GPR);
	static CodeField<25, 31> RT(FIELD_R_GPR);
	static CodeField<18, 24> RA(FIELD_R_GPR);
	static CodeField<11, 17> RB(FIELD_R_GPR);

	static CodeField<11, 17> i7;
	static CodeField<10, 17> i8;
	static CodeField<8, 17> i10;
	static CodeField<9, 24> i16;
	static CodeField<7, 24> i18;

	static CodeFieldSigned<11, 17> si7;
	static CodeFieldSigned<10, 17> si8;
	static CodeFieldSigned<8, 17> si10;
	static CodeFieldSignedOffset<8, 17, 4> si10s4;
	static CodeFieldSigned<9, 24> si16;
	static CodeFieldOffset<9, 24, 2> i16s2;
	static CodeFieldSigned<7, 24> si18;

	static CodeField<16, 17> ROH;
	static CodeField<25, 31> ROL;
	static DoubleCodeField<16, 17, 25, 31, 8> RO;

	static CodeField<0, 3> RRR;
	static CodeField<0, 6> RI18;
	static CodeField<0, 7> RI10;
	static CodeField<0, 8> RI16;
	static CodeField<0, 9> RI8;
	static CodeField<0, 10> RI7;
	static CodeField<0, 10> RR;

	static CodeField<18, 31> L_18_31;
	static CodeField<11> L_11;

	// static auto rrr_list = new_list<SPUOpcodes>(RRR);
	static InstrList<1 << CodeField<0, 3>::size, SPUOpcodes> rrr_list_obj(RRR, nullptr);
	static auto rrr_list = &rrr_list_obj;
	static auto ri18_list = new_list(rrr_list, RI18);
	static auto ri10_list = new_list(ri18_list, RI10);
	static auto ri16_list = new_list(ri10_list, RI16);
	static auto ri8_list = new_list(ri16_list, RI8);
	static auto ri7_list = new_list(ri8_list, RI7, instr_bind(&SPUOpcodes::UNK, GetCode, RRR, RI7));

	#define bind_instr(list, name, ...) \
		static const auto& name = make_instr<SPU_opcodes::name>(list, #name, &SPUOpcodes::name, ##__VA_ARGS__)

	bind_instr(ri7_list, STOP, L_18_31);
	bind_instr(ri7_list, LNOP);
	bind_instr(ri7_list, SYNC, L_11);
	bind_instr(ri7_list, DSYNC);
	bind_instr(ri7_list, MFSPR, RT, RA);
	bind_instr(ri7_list, RDCH, RT, RA);
	bind_instr(ri7_list, RCHCNT, RT, RA);
	bind_instr(ri7_list, SF, RT, RA, RB);
	bind_instr(ri7_list, OR, RT, RA, RB);
	bind_instr(ri7_list, BG, RT, RA, RB);
	bind_instr(ri7_list, SFH, RT, RA, RB);
	bind_instr(ri7_list, NOR, RT, RA, RB);
	bind_instr(ri7_list, ABSDB, RT, RA, RB);
	bind_instr(ri7_list, ROT, RT, RA, RB);
	bind_instr(ri7_list, ROTM, RT, RA, RB);
	bind_instr(ri7_list, ROTMA, RT, RA, RB);
	bind_instr(ri7_list, SHL, RT, RA, RB);
	bind_instr(ri7_list, ROTH, RT, RA, RB);
	bind_instr(ri7_list, ROTHM, RT, RA, RB);
	bind_instr(ri7_list, ROTMAH, RT, RA, RB);
	bind_instr(ri7_list, SHLH, RT, RA, RB);
	bind_instr(ri7_list, ROTI, RT, RA, RB);
	bind_instr(ri7_list, ROTMI, RT, RA, RB);
	bind_instr(ri7_list, ROTMAI, RT, RA, RB);
	bind_instr(ri7_list, SHLI, RT, RA, i7);
	bind_instr(ri7_list, ROTHI, RT, RA, i7);
	bind_instr(ri7_list, ROTHMI, RT, RA, i7);
	bind_instr(ri7_list, ROTMAHI, RT, RA, i7);
	bind_instr(ri7_list, SHLHI, RT, RA, i7);
	bind_instr(ri7_list, A, RT, RA, RB);
	bind_instr(ri7_list, AND, RT, RA, RB);
	bind_instr(ri7_list, CG, RT, RA, RB);
	bind_instr(ri7_list, AH, RT, RA, RB);
	bind_instr(ri7_list, NAND, RT, RA, RB);
	bind_instr(ri7_list, AVGB, RT, RA, RB);
	bind_instr(ri7_list, MTSPR, RT, RA);
	bind_instr(ri7_list, WRCH, RA, RT);
	bind_instr(ri7_list, BIZ, RB, RT, RA);
	bind_instr(ri7_list, BINZ, RB, RT, RA);
	bind_instr(ri7_list, BIHZ, RB, RT, RA);
	bind_instr(ri7_list, BIHNZ, RB, RT, RA);
	bind_instr(ri7_list, STOPD, RT, RA, RB);
	bind_instr(ri7_list, STQX, RT, RA, RB);
	bind_instr(ri7_list, BI, RB, RA);
	bind_instr(ri7_list, BISL, RB, RT, RA);
	bind_instr(ri7_list, IRET, RA);
	bind_instr(ri7_list, BISLED, RB, RT, RA);
	bind_instr(ri7_list, HBR, L_11, RO, RA);
	bind_instr(ri7_list, GB, RT, RA);
	bind_instr(ri7_list, GBH, RT, RA);
	bind_instr(ri7_list, GBB, RT, RA);
	bind_instr(ri7_list, FSM, RT, RA);
	bind_instr(ri7_list, FSMH, RT, RA);
	bind_instr(ri7_list, FSMB, RT, RA);
	bind_instr(ri7_list, FREST, RT, RA);
	bind_instr(ri7_list, FRSQEST, RT, RA);
	bind_instr(ri7_list, LQX, RT, RA, RB);
	bind_instr(ri7_list, ROTQBYBI, RT, RA, RB);
	bind_instr(ri7_list, ROTQMBYBI, RT, RA, RB);
	bind_instr(ri7_list, SHLQBYBI, RT, RA, RB);
	bind_instr(ri7_list, CBX, RT, RA, RB);
	bind_instr(ri7_list, CHX, RT, RA, RB);
	bind_instr(ri7_list, CWX, RT, RA, RB);
	bind_instr(ri7_list, CDX, RT, RA, RB);
	bind_instr(ri7_list, ROTQBI, RT, RA, RB);
	bind_instr(ri7_list, ROTQMBI, RT, RA, RB);
	bind_instr(ri7_list, SHLQBI, RT, RA, RB);
	bind_instr(ri7_list, ROTQBY, RT, RA, RB);
	bind_instr(ri7_list, ROTQMBY, RT, RA, RB);
	bind_instr(ri7_list, SHLQBY, RT, RA, RB);
	bind_instr(ri7_list, ORX, RT, RA);
	bind_instr(ri7_list, CBD, RT, RA, si7);
	bind_instr(ri7_list, CHD, RT, RA, si7);
	bind_instr(ri7_list, CWD, RT, RA, si7);
	bind_instr(ri7_list, CDD, RT, RA, si7);
	bind_instr(ri7_list, ROTQBII, RT, RA, i7);
	bind_instr(ri7_list, ROTQMBII, RT, RA, i7);
	bind_instr(ri7_list, SHLQBII, RT, RA, i7);
	bind_instr(ri7_list, ROTQBYI, RT, RA, i7);
	bind_instr(ri7_list, ROTQMBYI, RT, RA, i7);
	bind_instr(ri7_list, SHLQBYI, RT, RA, i7);
	bind_instr(ri7_list, NOP, RT);
	bind_instr(ri7_list, CGT, RT, RA, RB);
	bind_instr(ri7_list, XOR, RT, RA, RB);
	bind_instr(ri7_list, CGTH, RT, RA, RB);
	bind_instr(ri7_list, EQV, RT, RA, RB);
	bind_instr(ri7_list, CGTB, RT, RA, RB);
	bind_instr(ri7_list, SUMB, RT, RA, RB);
	bind_instr(ri7_list, HGT, RT, RA, RB);
	bind_instr(ri7_list, CLZ, RT, RA);
	bind_instr(ri7_list, XSWD, RT, RA);
	bind_instr(ri7_list, XSHW, RT, RA);
	bind_instr(ri7_list, CNTB, RT, RA);
	bind_instr(ri7_list, XSBH, RT, RA);
	bind_instr(ri7_list, CLGT, RT, RA, RB);
	bind_instr(ri7_list, ANDC, RT, RA, RB);
	bind_instr(ri7_list, FCGT, RT, RA, RB);
	bind_instr(ri7_list, DFCGT, RT, RA, RB);
	bind_instr(ri7_list, FA, RT, RA, RB);
	bind_instr(ri7_list, FS, RT, RA, RB);
	bind_instr(ri7_list, FM, RT, RA, RB);
	bind_instr(ri7_list, CLGTH, RT, RA, RB);
	bind_instr(ri7_list, ORC, RT, RA, RB);
	bind_instr(ri7_list, FCMGT, RT, RA, RB);
	bind_instr(ri7_list, DFCMGT, RT, RA, RB);
	bind_instr(ri7_list, DFA, RT, RA, RB);
	bind_instr(ri7_list, DFS, RT, RA, RB);
	bind_instr(ri7_list, DFM, RT, RA, RB);
	bind_instr(ri7_list, CLGTB, RT, RA, RB);
	bind_instr(ri7_list, HLGT, RT, RA, RB);
	bind_instr(ri7_list, DFMA, RT, RA, RB);
	bind_instr(ri7_list, DFMS, RT, RA, RB);
	bind_instr(ri7_list, DFNMS, RT, RA, RB);
	bind_instr(ri7_list, DFNMA, RT, RA, RB);
	bind_instr(ri7_list, CEQ, RT, RA, RB);
	bind_instr(ri7_list, MPYHHU, RT, RA, RB);
	bind_instr(ri7_list, ADDX, RT, RA, RB);
	bind_instr(ri7_list, SFX, RT, RA, RB);
	bind_instr(ri7_list, CGX, RT, RA, RB);
	bind_instr(ri7_list, BGX, RT, RA, RB);
	bind_instr(ri7_list, MPYHHA, RT, RA, RB);
	bind_instr(ri7_list, MPYHHAU, RT, RA, RB);
	bind_instr(ri7_list, FSCRRD, RT);
	bind_instr(ri7_list, FESD, RT, RA);
	bind_instr(ri7_list, FRDS, RT, RA);
	bind_instr(ri7_list, FSCRWR, RT, RA);
	bind_instr(ri7_list, DFTSV, RT, RA, i7);
	bind_instr(ri7_list, FCEQ, RT, RA, RB);
	bind_instr(ri7_list, DFCEQ, RT, RA, RB);
	bind_instr(ri7_list, MPY, RT, RA, RB);
	bind_instr(ri7_list, MPYH, RT, RA, RB);
	bind_instr(ri7_list, MPYHH, RT, RA, RB);
	bind_instr(ri7_list, MPYS, RT, RA, RB);
	bind_instr(ri7_list, CEQH, RT, RA, RB);
	bind_instr(ri7_list, FCMEQ, RT, RA, RB);
	bind_instr(ri7_list, DFCMEQ, RT, RA, RB);
	bind_instr(ri7_list, MPYU, RT, RA, RB);
	bind_instr(ri7_list, CEQB, RT, RA, RB);
	bind_instr(ri7_list, FI, RT, RA, RB);
	bind_instr(ri7_list, HEQ, RT, RA, RB);

	bind_instr(ri8_list, CFLTS, RT, RA, i8);
	bind_instr(ri8_list, CFLTU, RT, RA, i8);
	bind_instr(ri8_list, CSFLT, RT, RA, i8);
	bind_instr(ri8_list, CUFLT, RT, RA, i8);

	bind_instr(ri16_list, BRZ, RT, si16);
	bind_instr(ri16_list, STQA, RT, si16);
	bind_instr(ri16_list, BRNZ, RT, si16);
	bind_instr(ri16_list, BRHZ, RT, si16);
	bind_instr(ri16_list, BRHNZ, RT, si16);
	bind_instr(ri16_list, STQR, RT, i16);
	bind_instr(ri16_list, BRA, si16);
	bind_instr(ri16_list, LQA, RT, si16);
	bind_instr(ri16_list, BRASL, RT, si16);
	bind_instr(ri16_list, BR, si16);
	bind_instr(ri16_list, FSMBI, RT, i16);
	bind_instr(ri16_list, BRSL, RT, si16);
	bind_instr(ri16_list, LQR, RT, si16);
	bind_instr(ri16_list, IL, RT, si16);
	bind_instr(ri16_list, ILHU, RT, i16);
	bind_instr(ri16_list, ILH, RT, i16);
	bind_instr(ri16_list, IOHL, RT, i16);

	bind_instr(ri10_list, ORI, RT, RA, si10);
	bind_instr(ri10_list, ORHI, RT, RA, si10);
	bind_instr(ri10_list, ORBI, RT, RA, i10);
	bind_instr(ri10_list, SFI, RT, RA, si10);
	bind_instr(ri10_list, SFHI, RT, RA, si10);
	bind_instr(ri10_list, ANDI, RT, RA, si10);
	bind_instr(ri10_list, ANDHI, RT, RA, si10);
	bind_instr(ri10_list, ANDBI, RT, RA, i10);
	bind_instr(ri10_list, AI, RT, RA, si10);
	bind_instr(ri10_list, AHI, RT, RA, si10);
	bind_instr(ri10_list, STQD, RT, si10s4, RA);
	bind_instr(ri10_list, LQD, RT, si10s4, RA);
	bind_instr(ri10_list, XORI, RT, RA, si10);
	bind_instr(ri10_list, XORHI, RT, RA, si10);
	bind_instr(ri10_list, XORBI, RT, RA, i10);
	bind_instr(ri10_list, CGTI, RT, RA, si10);
	bind_instr(ri10_list, CGTHI, RT, RA, si10);
	bind_instr(ri10_list, CGTBI, RT, RA, i10);
	bind_instr(ri10_list, HGTI, RT, RA, si10);
	bind_instr(ri10_list, CLGTI, RT, RA, si10);
	bind_instr(ri10_list, CLGTHI, RT, RA, si10);
	bind_instr(ri10_list, CLGTBI, RT, RA, i10);
	bind_instr(ri10_list, HLGTI, RT, RA, si10);
	bind_instr(ri10_list, MPYI, RT, RA, si10);
	bind_instr(ri10_list, MPYUI, RT, RA, si10);
	bind_instr(ri10_list, CEQI, RT, RA, si10);
	bind_instr(ri10_list, CEQHI, RT, RA, si10);
	bind_instr(ri10_list, CEQBI, RT, RA, i10);
	bind_instr(ri10_list, HEQI, RT, RA, si10);

	bind_instr(ri18_list, HBRA, RO, i16s2);
	bind_instr(ri18_list, HBRR, RO, si16);
	bind_instr(ri18_list, ILA, RT, i18);

	bind_instr(rrr_list, SELB, RC, RA, RB, RT);
	bind_instr(rrr_list, SHUFB, RC, RA, RB, RT);
	bind_instr(rrr_list, MPYA, RC, RA, RB, RT);
	bind_instr(rrr_list, FNMS, RC, RA, RB, RT);
	bind_instr(rrr_list, FMA, RC, RA, RB, RT);
	bind_instr(rrr_list, FMS, RC, RA, RB, RT);

	#undef bind_instr
};