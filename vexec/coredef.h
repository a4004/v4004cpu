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
/// Word (16-bits)
/// </summary>
using WORD = unsigned short;
/// <summary>
/// Byte Pointer
/// </summary>
using PBYTE = BYTE*;
/// <summary>
/// Word Pointer
/// </summary>
using PWORD = WORD*;


#define end	0x0000				

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

#define adda_imm 0x0DA0
#define adda_addr 0x0DA1
#define adda_a 0x0DAA
#define adda_b 0x0DAB
#define adda_c 0x0DAC
#define adda_d 0x0DAD
#define addb_imm 0x0DB0
#define addb_addr 0x0DB1
#define addb_a 0x0DBA
#define addb_b 0x0DBB
#define addb_c 0x0DBC
#define addb_d 0x0DBD
#define addc_imm 0x0DC0
#define addc_addr 0x0DC1
#define addc_a 0x0DCA
#define addc_b 0x0DCB
#define addc_c 0x0DCC
#define addc_d 0x0DCD
#define addd_imm 0x0DD0
#define addd_addr 0x0DD1
#define addd_a 0x0DDA
#define addd_b 0x0DDB
#define addd_c 0x0DDC
#define addd_d 0x0DDD

#define suba_imm 0x0CA0
#define suba_addr 0x0CA1
#define suba_a 0x0CAA
#define suba_b 0x0CAB
#define suba_c 0x0CAC
#define suba_d 0x0CAD
#define subb_imm 0x0CB0
#define subb_addr 0x0CB1
#define subb_a 0x0CBA
#define subb_b 0x0CBB
#define subb_c 0x0CBC
#define subb_d 0x0CBD
#define subc_imm 0x0CC0
#define subc_addr 0x0CC1
#define subc_a 0x0CCA
#define subc_b 0x0CCB
#define subc_c 0x0CCC
#define subc_d 0x0CCD
#define subd_imm 0x0CD0
#define subd_addr 0x0CD1
#define subd_a 0x0CDA
#define subd_b 0x0CDB
#define subd_c 0x0CDC
#define subd_d 0x0CDD

#define diva_imm 0x0BA0
#define diva_addr 0x0BA1
#define diva_a 0x0BAA
#define diva_b 0x0BAB
#define diva_c 0x0BAC
#define diva_d 0x0BAD
#define divb_imm 0x0BB0
#define divb_addr 0x0BB1
#define divb_a 0x0BBA
#define divb_b 0x0BBB
#define divb_c 0x0BBC
#define divb_d 0x0BBD
#define divc_imm 0x0BC0
#define divc_addr 0x0BC1
#define divc_a 0x0BCA
#define divc_b 0x0BCB
#define divc_c 0x0BCC
#define divc_d 0x0BCD
#define divd_imm 0x0BD0
#define divd_addr 0x0BD1
#define divd_a 0x0BDA
#define divd_b 0x0BDB
#define divd_c 0x0BDC
#define divd_d 0x0BDD

#define mula_imm 0x0AA0
#define mula_addr 0x0AA1
#define mula_a 0x0AAA
#define mula_b 0x0AAB
#define mula_c 0x0AAC
#define mula_d 0x0AAD
#define mulb_imm 0x0AB0
#define mulb_addr 0x0AB1
#define mulb_a 0x0ABA
#define mulb_b 0x0ABB
#define mulb_c 0x0ABC
#define mulb_d 0x0ABD
#define mulc_imm 0x0AC0
#define mulc_addr 0x0AC1
#define mulc_a 0x0ACA
#define mulc_b 0x0ACB
#define mulc_c 0x0ACC
#define mulc_d 0x0ACD
#define muld_imm 0x0AD0
#define muld_addr 0x0AD1
#define muld_a 0x0ADA
#define muld_b 0x0ADB
#define muld_c 0x0ADC
#define muld_d 0x0ADD

#define anda_imm 0x09A0
#define anda_addr 0x09A1
#define anda_a 0x09AA
#define anda_b 0x09AB
#define anda_c 0x09AC
#define anda_d 0x09AD
#define andb_imm 0x09B0
#define andb_addr 0x09B1
#define andb_a 0x09BA
#define andb_b 0x09BB
#define andb_c 0x09BC
#define andb_d 0x09BD
#define andc_imm 0x09C0
#define andc_addr 0x09C1
#define andc_a 0x09CA
#define andc_b 0x09CB
#define andc_c 0x09CC
#define andc_d 0x09CD
#define andd_imm 0x09D0
#define andd_addr 0x09D1
#define andd_a 0x09DA
#define andd_b 0x09DB
#define andd_c 0x09DC
#define andd_d 0x09DD

#define ora_imm 0x08A0
#define ora_addr 0x08A1
#define ora_a 0x08AA
#define ora_b 0x08AB
#define ora_c 0x08AC
#define ora_d 0x08AD
#define orb_imm 0x08B0
#define orb_addr 0x08B1
#define orb_a 0x08BA
#define orb_b 0x08BB
#define orb_c 0x08BC
#define orb_d 0x08BD
#define orc_imm 0x08C0
#define orc_addr 0x08C1
#define orc_a 0x08CA
#define orc_b 0x08CB
#define orc_c 0x08CC
#define orc_d 0x08CD
#define ord_imm 0x08D0
#define ord_addr 0x08D1
#define ord_a 0x08DA
#define ord_b 0x08DB
#define ord_c 0x08DC
#define ord_d 0x08DD

#define xora_imm 0x07A0
#define xora_addr 0x07A1
#define xora_a 0x07AA
#define xora_b 0x07AB
#define xora_c 0x07AC
#define xora_d 0x07AD
#define xorb_imm 0x07B0
#define xorb_addr 0x07B1
#define xorb_a 0x07BA
#define xorb_b 0x07BB
#define xorb_c 0x07BC
#define xorb_d 0x07BD
#define xorc_imm 0x07C0
#define xorc_addr 0x07C1
#define xorc_a 0x07CA
#define xorc_b 0x07CB
#define xorc_c 0x07CC
#define xorc_d 0x07CD
#define xord_imm 0x07D0
#define xord_addr 0x07D1
#define xord_a 0x07DA
#define xord_b 0x07DB
#define xord_c 0x07DC
#define xord_d 0x07DD


#define cpya_a 0x06AA
#define cpya_b 0x06AB
#define cpya_c 0x06AC
#define cpya_d 0x06AD
#define cpyb_a 0x06BA
#define cpyb_b 0x06BB
#define cpyb_c 0x06BC
#define cpyb_d 0x06BD
#define cpyc_a 0x06CA
#define cpyc_b 0x06CB
#define cpyc_c 0x06CC
#define cpyc_d 0x06CD
#define cpyd_a 0x06DA
#define cpyd_b 0x06DB
#define cpyd_c 0x06DC
#define cpyd_d 0x06DD

#define nota 0x050A
#define notb 0x050B
#define notc 0x050C
#define notd 0x050D

#define lsha 0x05AA
#define rsha 0x05BA
#define lshb 0x05AB
#define rshb 0x05BB
#define lshc 0x05AC
#define rshc 0x05BC
#define lshd 0x05AD
#define rshd 0x05BD

#define cmpa_a 0x04AA
#define cmpa_b 0x04AB
#define cmpa_c 0x04AC
#define cmpa_d 0x04AD
#define cmpb_a 0x04BA
#define cmpb_b 0x04BB
#define cmpb_c 0x04BC
#define cmpb_d 0x04BD
#define cmpc_a 0x04CA
#define cmpc_b 0x04CB
#define cmpc_c 0x04CC
#define cmpc_d 0x04CD
#define cmpd_a 0x04DA
#define cmpd_b 0x04DB
#define cmpd_c 0x04DC
#define cmpd_d 0x04DD

#define jmp_addr 0x03F1
#define jmz_addr 0x03E1
#define jmn_addr 0x03D1
#define jmo_addr 0x03C1
#define jpp_addr 0x03B1
#define nop 0x0300

#define pusha 0x020A
#define pushb 0x020B
#define pushc 0x020C
#define pushd 0x020D
#define popa 0x021A
#define popb 0x021B
#define popc 0x021C
#define popd 0x021D

#define call 0x0100
#define ret 0x0101
