#pragma once

/// <summary>
/// Byte (8-bits)
/// </summary>
using BYTE = unsigned char;
/// <summary>
/// Bit (can only be used in a C++ bit field)
/// </summary>
using BIT = unsigned char;
/// <summary>
/// Wor_d (16-bits)
/// </summary>
using WORD = unsigned short;
/// <summary>
/// Byte Pointer
/// </summary>
using PBYTE = BYTE*;
/// <summary>
/// Wor_d Pointer
/// </summary>
using PWORD = WORD*;

// Single Mnemonics
#define end	0x0000			
#define call 0x0100
#define ret 0x0101
#define nop 0x0300

// Double Mnemonics
#define tin_addr 0xF000
#define tout_imm 0xF100
#define tout_addr 0xF101
#define tout_a 0xF10A
#define tout_b 0xF10B
#define tout_c 0xF10C
#define tout_d 0xF10D

#define lda_imm 0x0FA0
#define lda_addr 0x0FA1
#define ldb_imm 0x0FB0
#define ldb_addr 0x0FB1
#define ldc_imm 0x0FC0
#define ldc_addr 0x0FC1
#define ldd_imm 0x0FD0
#define ldd_addr 0x0FD1

#define sta_addr 0x0EA1
#define stb_addr 0x0EB1
#define stc_addr 0x0EC1
#define std_addr 0x0ED1

#define push_a 0x020A
#define push_b 0x020B
#define push_c 0x020C
#define push_d 0x020D
#define pop_a 0x021A
#define pop_b 0x021B
#define pop_c 0x021C
#define pop_d 0x021D

#define jmp_addr 0x03F1
#define jmz_addr 0x03E1
#define jmn_addr 0x03D1
#define jmo_addr 0x03C1
#define jpp_addr 0x03B1

#define not_a 0x050A
#define not_b 0x050B
#define not_c 0x050C
#define not_d 0x050D

#define lsh_a 0x05AA
#define rsh_a 0x05BA
#define lsh_b 0x05AB
#define rsh_b 0x05BB
#define lsh_c 0x05AC
#define rsh_c 0x05BC
#define lsh_d 0x05AD
#define rsh_d 0x05BD

// Triple Mnemonics
#define add_a_imm 0x0DA0
#define add_a_addr 0x0DA1
#define add_a_a 0x0DAA
#define add_a_b 0x0DAB
#define add_a_c 0x0DAC
#define add_a_d 0x0DAD
#define add_b_imm 0x0DB0
#define add_b_addr 0x0DB1
#define add_b_a 0x0DBA
#define add_b_b 0x0DBB
#define add_b_c 0x0DBC
#define add_b_d 0x0DBD
#define add_c_imm 0x0DC0
#define add_c_addr 0x0DC1
#define add_c_a 0x0DCA
#define add_c_b 0x0DCB
#define add_c_c 0x0DCC
#define add_c_d 0x0DCD
#define add_d_imm 0x0DD0
#define add_d_addr 0x0DD1
#define add_d_a 0x0DDA
#define add_d_b 0x0DDB
#define add_d_c 0x0DDC
#define add_d_d 0x0DDD

#define sub_a_imm 0x0CA0
#define sub_a_addr 0x0CA1
#define sub_a_a 0x0CAA
#define sub_a_b 0x0CAB
#define sub_a_c 0x0CAC
#define sub_a_d 0x0CAD
#define sub_b_imm 0x0CB0
#define sub_b_addr 0x0CB1
#define sub_b_a 0x0CBA
#define sub_b_b 0x0CBB
#define sub_b_c 0x0CBC
#define sub_b_d 0x0CBD
#define sub_c_imm 0x0CC0
#define sub_c_addr 0x0CC1
#define sub_c_a 0x0CCA
#define sub_c_b 0x0CCB
#define sub_c_c 0x0CCC
#define sub_c_d 0x0CCD
#define sub_d_imm 0x0CD0
#define sub_d_addr 0x0CD1
#define sub_d_a 0x0CDA
#define sub_d_b 0x0CDB
#define sub_d_c 0x0CDC
#define sub_d_d 0x0CDD

#define div_a_imm 0x0BA0
#define div_a_addr 0x0BA1
#define div_a_a 0x0BAA
#define div_a_b 0x0BAB
#define div_a_c 0x0BAC
#define div_a_d 0x0BAD
#define div_b_imm 0x0BB0
#define div_b_addr 0x0BB1
#define div_b_a 0x0BBA
#define div_b_b 0x0BBB
#define div_b_c 0x0BBC
#define div_b_d 0x0BBD
#define div_c_imm 0x0BC0
#define div_c_addr 0x0BC1
#define div_c_a 0x0BCA
#define div_c_b 0x0BCB
#define div_c_c 0x0BCC
#define div_c_d 0x0BCD
#define div_d_imm 0x0BD0
#define div_d_addr 0x0BD1
#define div_d_a 0x0BDA
#define div_d_b 0x0BDB
#define div_d_c 0x0BDC
#define div_d_d 0x0BDD

#define mul_a_imm 0x0AA0
#define mul_a_addr 0x0AA1
#define mul_a_a 0x0AAA
#define mul_a_b 0x0AAB
#define mul_a_c 0x0AAC
#define mul_a_d 0x0AAD
#define mul_b_imm 0x0AB0
#define mul_b_addr 0x0AB1
#define mul_b_a 0x0ABA
#define mul_b_b 0x0ABB
#define mul_b_c 0x0ABC
#define mul_b_d 0x0ABD
#define mul_c_imm 0x0AC0
#define mul_c_addr 0x0AC1
#define mul_c_a 0x0ACA
#define mul_c_b 0x0ACB
#define mul_c_c 0x0ACC
#define mul_c_d 0x0ACD
#define mul_d_imm 0x0AD0
#define mul_d_addr 0x0AD1
#define mul_d_a 0x0ADA
#define mul_d_b 0x0ADB
#define mul_d_c 0x0ADC
#define mul_d_d 0x0ADD

#define and_a_imm 0x09A0
#define and_a_addr 0x09A1
#define and_a_a 0x09AA
#define and_a_b 0x09AB
#define and_a_c 0x09AC
#define and_a_d 0x09AD
#define and_b_imm 0x09B0
#define and_b_addr 0x09B1
#define and_b_a 0x09BA
#define and_b_b 0x09BB
#define and_b_c 0x09BC
#define and_b_d 0x09BD
#define and_c_imm 0x09C0
#define and_c_addr 0x09C1
#define and_c_a 0x09CA
#define and_c_b 0x09CB
#define and_c_c 0x09CC
#define and_c_d 0x09CD
#define and_d_imm 0x09D0
#define and_d_addr 0x09D1
#define and_d_a 0x09DA
#define and_d_b 0x09DB
#define and_d_c 0x09DC
#define and_d_d 0x09DD

#define or_a_imm 0x08A0
#define or_a_addr 0x08A1
#define or_a_a 0x08AA
#define or_a_b 0x08AB
#define or_a_c 0x08AC
#define or_a_d 0x08AD
#define or_b_imm 0x08B0
#define or_b_addr 0x08B1
#define or_b_a 0x08BA
#define or_b_b 0x08BB
#define or_b_c 0x08BC
#define or_b_d 0x08BD
#define or_c_imm 0x08C0
#define or_c_addr 0x08C1
#define or_c_a 0x08CA
#define or_c_b 0x08CB
#define or_c_c 0x08CC
#define or_c_d 0x08CD
#define or_d_imm 0x08D0
#define or_d_addr 0x08D1
#define or_d_a 0x08DA
#define or_d_b 0x08DB
#define or_d_c 0x08DC
#define or_d_d 0x08DD

#define xor_a_imm 0x07A0
#define xor_a_addr 0x07A1
#define xor_a_a 0x07AA
#define xor_a_b 0x07AB
#define xor_a_c 0x07AC
#define xor_a_d 0x07AD
#define xor_b_imm 0x07B0
#define xor_b_addr 0x07B1
#define xor_b_a 0x07BA
#define xor_b_b 0x07BB
#define xor_b_c 0x07BC
#define xor_b_d 0x07BD
#define xor_c_imm 0x07C0
#define xor_c_addr 0x07C1
#define xor_c_a 0x07CA
#define xor_c_b 0x07CB
#define xor_c_c 0x07CC
#define xor_c_d 0x07CD
#define xor_d_imm 0x07D0
#define xor_d_addr 0x07D1
#define xor_d_a 0x07DA
#define xor_d_b 0x07DB
#define xor_d_c 0x07DC
#define xor_d_d 0x07DD

#define cpy_a_a 0x06AA
#define cpy_a_b 0x06AB
#define cpy_a_c 0x06AC
#define cpy_a_d 0x06AD
#define cpy_b_a 0x06BA
#define cpy_b_b 0x06BB
#define cpy_b_c 0x06BC
#define cpy_b_d 0x06BD
#define cpy_c_a 0x06CA
#define cpy_c_b 0x06CB
#define cpy_c_c 0x06CC
#define cpy_c_d 0x06CD
#define cpy_d_a 0x06DA
#define cpy_d_b 0x06DB
#define cpy_d_c 0x06DC
#define cpy_d_d 0x06DD

#define cmp_a_a 0x04AA
#define cmp_a_b 0x04AB
#define cmp_a_c 0x04AC
#define cmp_a_d 0x04AD
#define cmp_b_a 0x04BA
#define cmp_b_b 0x04BB
#define cmp_b_c 0x04BC
#define cmp_b_d 0x04BD
#define cmp_c_a 0x04CA
#define cmp_c_b 0x04CB
#define cmp_c_c 0x04CC
#define cmp_c_d 0x04CD
#define cmp_d_a 0x04DA
#define cmp_d_b 0x04DB
#define cmp_d_c 0x04DC
#define cmp_d_d 0x04DD
