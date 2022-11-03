#include <stdio.h>
#include <iostream>
#include <chrono>
#include <thread>
#include <conio.h>
#include "cpu.h"

void cpu::reset_cpu(mem& memory) {
    registers.special.ip = 0x0000;
    registers.special.ir = 0x0000;
    registers.special.sp = DATA_SIZE - 2;
    registers.special.mdr = 0x0000;
    registers.special.mar = 0x0000;

    registers.special.status.zo = 0;
    registers.special.status.ng = 0;
    registers.special.status.of = 0;
    registers.special.status.py = 0;

    registers.rA = 0x0000;
    registers.rB = 0x0000;
    registers.rC = 0x0000;
    registers.rD = 0x0000;

    for (BYTE i = 0; i < CACHE_SIZE; i++) {
        cache[i] = 0x0000;
    }

    memory.initialise();
}
void cpu::debug_dump() {
    printf("--- BEGIN CPU STATE ---\nIP: %04x\tIR: %04x\tSP: %04x\tMDR: %04x\tMAR: %04x\nSTATUS REGISTERS:\tZ=%x\tN=%x\tO=%x\tP=%x\nrA: %02x\t\trB: %02x\t\trC: %02x\t\trD: %02x\nCACHE MEMORY:\t%04x\t%04x\t%04x\t%04x\t%04x\t%04x\t%04x\t%04x\n--- END CPU STATE ---\n",
        registers.special.ip, registers.special.ir, registers.special.sp, registers.special.mdr, registers.special.mar, registers.special.status.zo, registers.special.status.ng, registers.special.status.of,
        registers.special.status.py, registers.rA, registers.rB, registers.rC, registers.rD, cache[0], cache[1], cache[2], cache[3], cache[4], cache[5], cache[6], cache[7]);
}
void cpu::execute(unsigned int cycles, mem& memory, int clock_speed_hz) {
    while (cycles > 0) {
        std::this_thread::sleep_for(std::chrono::milliseconds((int)((float)((float)1 / (float)clock_speed_hz) * 1000)));
        registers.special.ir = fetch_next_word(cycles, memory);
        switch (registers.special.ir) {
        case end:
            cycles = 0;
            break;
        case tin_addr:
            cache[0] = fetch_next_word(cycles, memory);
            printf("> ");
            cache[1] = _getch();
            printf("\n");
            memory.io_write(cache[0], cache[1]);
            memory.io_write(cache[0] + 1, 0x00);
            break;
        case tout_imm:
            cache[0] = fetch_next_word(cycles, memory);
            std::cout << (BYTE)cache[0];
            break;
        case tout_addr:
            cache[0] = fetch_next_word(cycles, memory);
            registers.special.mar = cache[0];
            cache[1] = read_word(cycles, memory);
            std::cout << (BYTE)cache[1];
            break;
        case tout_a:
            std::cout << (BYTE)registers.rA;
            break;
        case tout_b:
            std::cout << (BYTE)registers.rB;
            break;
        case tout_c:
            std::cout << (BYTE)registers.rC;
            break;
        case tout_d:
            std::cout << (BYTE)registers.rD;
            break;
        case lda_imm:
            registers.rA = fetch_next_word(cycles, memory);
            break;
        case lda_addr:
            cache[0] = fetch_next_word(cycles, memory);
            registers.special.mar = cache[0];
            registers.rA = read_word(cycles, memory);
            break;
        case ldb_imm:
            registers.rB = fetch_next_word(cycles, memory);
            break;
        case ldb_addr:
            cache[0] = fetch_next_word(cycles, memory);
            registers.special.mar = cache[0];
            registers.rC = read_word(cycles, memory);
            break;
        case ldc_addr:
            cache[0] = fetch_next_word(cycles, memory);
            registers.special.mar = cache[0];
            registers.rC = read_word(cycles, memory);
            break;
        case ldd_imm:
            registers.rD = fetch_next_word(cycles, memory);
            break;
        case ldd_addr:
            cache[0] = fetch_next_word(cycles, memory);
            registers.special.mar = cache[0];
            registers.rD = read_word(cycles, memory);
            break;
        case sta_addr:
            cache[0] = fetch_next_word(cycles, memory);
            registers.special.mar = cache[0];
            registers.special.mdr = registers.rA;
            write_word(cycles, memory);
            break;
        case stb_addr:
            cache[0] = fetch_next_word(cycles, memory);
            registers.special.mar = cache[0];
            registers.special.mdr = registers.rB;
            write_word(cycles, memory);
            break;
        case stc_addr:
            cache[0] = fetch_next_word(cycles, memory);
            registers.special.mar = cache[0];
            registers.special.mdr = registers.rC;
            write_word(cycles, memory);
            break;
        case std_addr:
            cache[0] = fetch_next_word(cycles, memory);
            registers.special.mar = cache[0];
            registers.special.mdr = registers.rD;
            write_word(cycles, memory);
            break;

        case adda_imm:
            cache[0] = fetch_next_word(cycles, memory);
            setFlagsAdd(registers.rA + cache[0], registers.rA, cache[0]);
            registers.rA += cache[0];
            break;
        case adda_addr:
            cache[0] = fetch_next_word(cycles, memory);
            registers.special.mar = cache[0];
            cache[1] = read_word(cycles, memory);
            setFlagsAdd(registers.rA + cache[1], registers.rA, cache[1]);
            registers.rA += cache[1];
            break;
        case adda_a:
            setFlagsAdd(registers.rA + registers.rA, registers.rA, registers.rA);
            registers.rA += registers.rA;
            break;
        case adda_b:
            setFlagsAdd(registers.rA + registers.rB, registers.rA, registers.rB);
            registers.rA += registers.rB;
            break;
        case adda_c:
            setFlagsAdd(registers.rA + registers.rC, registers.rA, registers.rC);
            registers.rA += registers.rC;
            break;
        case adda_d:
            setFlagsAdd(registers.rA + registers.rD, registers.rA, registers.rD);
            registers.rA += registers.rD;
            break;
        case addb_imm:
            cache[0] = fetch_next_word(cycles, memory);
            setFlagsAdd(registers.rB + cache[0], registers.rB, cache[0]);
            registers.rB += cache[0];
            break;
        case addb_addr:
            cache[0] = fetch_next_word(cycles, memory);
            registers.special.mar = cache[0];
            cache[1] = read_word(cycles, memory);
            setFlagsAdd(registers.rB + cache[1], registers.rB, cache[1]);
            registers.rB += cache[1];
            break;
        case addb_a:
            setFlagsAdd(registers.rB + registers.rA, registers.rB, registers.rA);
            registers.rB += registers.rA;
            break;
        case addb_b:
            setFlagsAdd(registers.rB + registers.rB, registers.rB, registers.rB);
            registers.rB += registers.rB;
            break;
        case addb_c:
            setFlagsAdd(registers.rB + registers.rC, registers.rB, registers.rC);
            registers.rB += registers.rC;
            break;
        case addb_d:
            setFlagsAdd(registers.rB + registers.rD, registers.rB, registers.rD);
            registers.rB += registers.rD;
            break;
        case addc_imm:
            cache[0] = fetch_next_word(cycles, memory);
            setFlagsAdd(registers.rC + cache[0], registers.rC, cache[0]);
            registers.rC += cache[0];
            break;
        case addc_addr:
            cache[0] = fetch_next_word(cycles, memory);
            registers.special.mar = cache[0];
            cache[1] = read_word(cycles, memory);
            setFlagsAdd(registers.rC + cache[1], registers.rC, cache[1]);
            registers.rC += cache[1];
            break;
        case addc_a:
            setFlagsAdd(registers.rC + registers.rA, registers.rC, registers.rA);
            registers.rC += registers.rA;
            break;
        case addc_b:
            setFlagsAdd(registers.rC + registers.rB, registers.rC, registers.rB);
            registers.rC += registers.rB;
            break;
        case addc_c:
            setFlagsAdd(registers.rC + registers.rC, registers.rC, registers.rC);
            registers.rC += registers.rC;
            break;
        case addc_d:
            setFlagsAdd(registers.rC + registers.rD, registers.rC, registers.rD);
            registers.rC += registers.rD;
            break;
        case addd_imm:
            cache[0] = fetch_next_word(cycles, memory);
            setFlagsAdd(registers.rD + cache[0], registers.rD, cache[0]);
            registers.rD += cache[0];
            break;
        case addd_addr:
            cache[0] = fetch_next_word(cycles, memory);
            registers.special.mar = cache[0];
            cache[1] = read_word(cycles, memory);
            setFlagsAdd(registers.rD + cache[1], registers.rD, cache[1]);
            registers.rD += cache[1];
            break;
        case addd_a:
            setFlagsAdd(registers.rD + registers.rA, registers.rD, registers.rA);
            registers.rD += registers.rA;
            break;
        case addd_b:
            setFlagsAdd(registers.rD + registers.rB, registers.rD, registers.rB);
            registers.rD += registers.rB;
            break;
        case addd_c:
            setFlagsAdd(registers.rD + registers.rC, registers.rD, registers.rC);
            registers.rD += registers.rC;
            break;
        case addd_d:
            setFlagsAdd(registers.rD + registers.rD, registers.rD, registers.rD);
            registers.rD += registers.rD;
            break;
        case suba_imm:
            cache[0] = fetch_next_word(cycles, memory);
            setFlagsSub(registers.rA - cache[0], registers.rA, cache[0]);
            registers.rA -= cache[0];
            break;
        case suba_addr:
            cache[0] = fetch_next_word(cycles, memory);
            registers.special.mar = cache[0];
            cache[1] = read_word(cycles, memory);
            setFlagsSub(registers.rA - cache[1], registers.rA, cache[1]);
            registers.rA -= cache[1];
            break;
        case suba_a:
            setFlagsSub(registers.rA - registers.rA, registers.rA, registers.rA);
            registers.rA -= registers.rA;
            break;
        case suba_b:
            setFlagsSub(registers.rA - registers.rB, registers.rA, registers.rB);
            registers.rA -= registers.rB;
            break;
        case suba_c:
            setFlagsSub(registers.rA - registers.rC, registers.rA, registers.rC);
            registers.rA -= registers.rC;
            break;
        case suba_d:
            setFlagsSub(registers.rA - registers.rD, registers.rA, registers.rD);
            registers.rA -= registers.rD;
            break;
        case subb_imm:
            cache[0] = fetch_next_word(cycles, memory);
            setFlagsSub(registers.rB - cache[0], registers.rB, cache[0]);
            registers.rB -= cache[0];
            break;
        case subb_addr:
            cache[0] = fetch_next_word(cycles, memory);
            registers.special.mar = cache[0];
            cache[1] = read_word(cycles, memory);
            setFlagsSub(registers.rB - cache[1], registers.rB, cache[1]);
            registers.rB -= cache[1];
            break;
        case subb_a:
            setFlagsSub(registers.rB - registers.rA, registers.rB, registers.rA);
            registers.rB -= registers.rA;
            break;
        case subb_b:
            setFlagsSub(registers.rB - registers.rB, registers.rB, registers.rB);
            registers.rB -= registers.rB;
            break;
        case subb_c:
            setFlagsSub(registers.rB - registers.rC, registers.rB, registers.rC);
            registers.rB -= registers.rC;
            break;
        case subb_d:
            setFlagsSub(registers.rB - registers.rD, registers.rB, registers.rD);
            registers.rB -= registers.rD;
            break;
        case subc_imm:
            cache[0] = fetch_next_word(cycles, memory);
            setFlagsSub(registers.rC - cache[0], registers.rC, cache[0]);
            registers.rC -= cache[0];
            break;
        case subc_addr:
            cache[0] = fetch_next_word(cycles, memory);
            registers.special.mar = cache[0];
            cache[1] = read_word(cycles, memory);
            setFlagsSub(registers.rC - cache[1], registers.rC, cache[1]);
            registers.rC -= cache[1];
            break;
        case subc_a:
            setFlagsSub(registers.rC - registers.rA, registers.rC, registers.rA);
            registers.rC -= registers.rA;
            break;
        case subc_b:
            setFlagsSub(registers.rC - registers.rB, registers.rC, registers.rB);
            registers.rC -= registers.rB;
            break;
        case subc_c:
            setFlagsSub(registers.rC - registers.rC, registers.rC, registers.rC);
            registers.rC -= registers.rC;
            break;
        case subc_d:
            setFlagsSub(registers.rC - registers.rD, registers.rC, registers.rD);
            registers.rC -= registers.rD;
            break;
        case subd_imm:
            cache[0] = fetch_next_word(cycles, memory);
            setFlagsSub(registers.rD - cache[0], registers.rD, cache[0]);
            registers.rD -= cache[0];
            break;
        case subd_addr:
            cache[0] = fetch_next_word(cycles, memory);
            registers.special.mar = cache[0];
            cache[1] = read_word(cycles, memory);
            setFlagsSub(registers.rD - cache[1], registers.rD, cache[1]);
            registers.rD -= cache[1];
            break;
        case subd_a:
            setFlagsSub(registers.rD - registers.rA, registers.rD, registers.rA);
            registers.rD -= registers.rA;
            break;
        case subd_b:
            setFlagsSub(registers.rD - registers.rB, registers.rD, registers.rB);
            registers.rD -= registers.rB;
            break;
        case subd_c:
            setFlagsSub(registers.rD - registers.rC, registers.rD, registers.rC);
            registers.rD -= registers.rC;
            break;
        case subd_d:
            setFlagsSub(registers.rD - registers.rD, registers.rD, registers.rD);
            registers.rD -= registers.rD;
            break;
        case diva_imm:
            cache[0] = fetch_next_word(cycles, memory);
            setFlagsDiv(registers.rA / cache[0], registers.rA, cache[0]);
            registers.rA /= cache[0];
            break;
        case diva_addr:
            cache[0] = fetch_next_word(cycles, memory);
            registers.special.mar = cache[0];
            cache[1] = read_word(cycles, memory);
            setFlagsDiv(registers.rA / cache[1], registers.rA, cache[1]);
            registers.rA /= cache[1];
            break;
        case diva_a:
            setFlagsDiv(registers.rA / registers.rA, registers.rA, registers.rA);
            registers.rA /= registers.rA;
            break;
        case diva_b:
            setFlagsDiv(registers.rA / registers.rB, registers.rA, registers.rB);
            registers.rA /= registers.rB;
            break;
        case diva_c:
            setFlagsDiv(registers.rA / registers.rC, registers.rA, registers.rC);
            registers.rA /= registers.rC;
            break;
        case diva_d:
            setFlagsDiv(registers.rA / registers.rD, registers.rA, registers.rD);
            registers.rA /= registers.rD;
            break;
        case divb_imm:
            cache[0] = fetch_next_word(cycles, memory);
            setFlagsDiv(registers.rB / cache[0], registers.rB, cache[0]);
            registers.rB /= cache[0];
            break;
        case divb_addr:
            cache[0] = fetch_next_word(cycles, memory);
            registers.special.mar = cache[0];
            cache[1] = read_word(cycles, memory);
            setFlagsDiv(registers.rB / cache[1], registers.rB, cache[1]);
            registers.rB /= cache[1];
            break;
        case divb_a:
            setFlagsDiv(registers.rB / registers.rA, registers.rB, registers.rA);
            registers.rB /= registers.rA;
            break;
        case divb_b:
            setFlagsDiv(registers.rB / registers.rB, registers.rB, registers.rB);
            registers.rB /= registers.rB;
            break;
        case divb_c:
            setFlagsDiv(registers.rB / registers.rC, registers.rB, registers.rC);
            registers.rB /= registers.rC;
            break;
        case divb_d:
            setFlagsDiv(registers.rB / registers.rD, registers.rB, registers.rD);
            registers.rB /= registers.rD;
            break;
        case divc_imm:
            cache[0] = fetch_next_word(cycles, memory);
            setFlagsDiv(registers.rC / cache[0], registers.rC, cache[0]);
            registers.rC /= cache[0];
            break;
        case divc_addr:
            cache[0] = fetch_next_word(cycles, memory);
            registers.special.mar = cache[0];
            cache[1] = read_word(cycles, memory);
            setFlagsDiv(registers.rC / cache[1], registers.rC, cache[1]);
            registers.rC /= cache[1];
            break;
        case divc_a:
            setFlagsDiv(registers.rC / registers.rA, registers.rC, registers.rA);
            registers.rC /= registers.rA;
            break;
        case divc_b:
            setFlagsDiv(registers.rC / registers.rB, registers.rC, registers.rB);
            registers.rC /= registers.rB;
            break;
        case divc_c:
            setFlagsDiv(registers.rC / registers.rC, registers.rC, registers.rC);
            registers.rC /= registers.rC;
            break;
        case divc_d:
            setFlagsDiv(registers.rC / registers.rD, registers.rC, registers.rD);
            registers.rC /= registers.rD;
            break;
        case divd_imm:
            cache[0] = fetch_next_word(cycles, memory);
            setFlagsDiv(registers.rD / cache[0], registers.rD, cache[0]);
            registers.rD /= cache[0];
            break;
        case divd_addr:
            cache[0] = fetch_next_word(cycles, memory);
            registers.special.mar = cache[0];
            cache[1] = read_word(cycles, memory);
            setFlagsDiv(registers.rD / cache[1], registers.rD, cache[1]);
            registers.rD /= cache[1];
            break;
        case divd_a:
            setFlagsDiv(registers.rD / registers.rA, registers.rD, registers.rA);
            registers.rD /= registers.rA;
            break;
        case divd_b:
            setFlagsDiv(registers.rD / registers.rB, registers.rD, registers.rB);
            registers.rD /= registers.rB;
            break;
        case divd_c:
            setFlagsDiv(registers.rD / registers.rC, registers.rD, registers.rC);
            registers.rD /= registers.rC;
            break;
        case divd_d:
            setFlagsDiv(registers.rD / registers.rD, registers.rD, registers.rD);
            registers.rD /= registers.rD;
            break;
        case mula_imm:
            cache[0] = fetch_next_word(cycles, memory);
            setFlagsMul(registers.rA * cache[0], registers.rA, cache[0]);
            registers.rA *= cache[0];
            break;
        case mula_addr:
            cache[0] = fetch_next_word(cycles, memory);
            registers.special.mar = cache[0];
            cache[1] = read_word(cycles, memory);
            setFlagsMul(registers.rA * cache[1], registers.rA, cache[1]);
            registers.rA *= cache[1];
            break;
        case mula_a:
            setFlagsMul(registers.rA * registers.rA, registers.rA, registers.rA);
            registers.rA *= registers.rA;
            break;
        case mula_b:
            setFlagsMul(registers.rA * registers.rB, registers.rA, registers.rB);
            registers.rA *= registers.rB;
            break;
        case mula_c:
            setFlagsMul(registers.rA * registers.rC, registers.rA, registers.rC);
            registers.rA *= registers.rC;
            break;
        case mula_d:
            setFlagsMul(registers.rA * registers.rD, registers.rA, registers.rD);
            registers.rA *= registers.rD;
            break;
        case mulb_imm:
            cache[0] = fetch_next_word(cycles, memory);
            setFlagsMul(registers.rB * cache[0], registers.rB, cache[0]);
            registers.rB *= cache[0];
            break;
        case mulb_addr:
            cache[0] = fetch_next_word(cycles, memory);
            registers.special.mar = cache[0];
            cache[1] = read_word(cycles, memory);
            setFlagsMul(registers.rB * cache[1], registers.rB, cache[1]);
            registers.rB *= cache[1];
            break;
        case mulb_a:
            setFlagsMul(registers.rB * registers.rA, registers.rB, registers.rA);
            registers.rB *= registers.rA;
            break;
        case mulb_b:
            setFlagsMul(registers.rB * registers.rB, registers.rB, registers.rB);
            registers.rB *= registers.rB;
            break;
        case mulb_c:
            setFlagsMul(registers.rB * registers.rC, registers.rB, registers.rC);
            registers.rB *= registers.rC;
            break;
        case mulb_d:
            setFlagsMul(registers.rB * registers.rD, registers.rB, registers.rD);
            registers.rB *= registers.rD;
            break;
        case mulc_imm:
            cache[0] = fetch_next_word(cycles, memory);
            setFlagsMul(registers.rC * cache[0], registers.rC, cache[0]);
            registers.rC *= cache[0];
            break;
        case mulc_addr:
            cache[0] = fetch_next_word(cycles, memory);
            registers.special.mar = cache[0];
            cache[1] = read_word(cycles, memory);
            setFlagsMul(registers.rC * cache[1], registers.rC, cache[1]);
            registers.rC *= cache[1];
            break;
        case mulc_a:
            setFlagsMul(registers.rC * registers.rA, registers.rC, registers.rA);
            registers.rC *= registers.rA;
            break;
        case mulc_b:
            setFlagsMul(registers.rC * registers.rB, registers.rC, registers.rB);
            registers.rC *= registers.rB;
            break;
        case mulc_c:
            setFlagsMul(registers.rC * registers.rC, registers.rC, registers.rC);
            registers.rC *= registers.rC;
            break;
        case mulc_d:
            setFlagsMul(registers.rC * registers.rD, registers.rC, registers.rD);
            registers.rC *= registers.rD;
            break;
        case muld_imm:
            cache[0] = fetch_next_word(cycles, memory);
            setFlagsMul(registers.rD * cache[0], registers.rD, cache[0]);
            registers.rD *= cache[0];
            break;
        case muld_addr:
            cache[0] = fetch_next_word(cycles, memory);
            registers.special.mar = cache[0];
            cache[1] = read_word(cycles, memory);
            setFlagsMul(registers.rD * cache[1], registers.rD, cache[1]);
            registers.rD *= cache[1];
            break;
        case muld_a:
            setFlagsMul(registers.rD * registers.rA, registers.rD, registers.rA);
            registers.rD *= registers.rA;
            break;
        case muld_b:
            setFlagsMul(registers.rD * registers.rB, registers.rD, registers.rB);
            registers.rD *= registers.rB;
            break;
        case muld_c:
            setFlagsMul(registers.rD * registers.rC, registers.rD, registers.rC);
            registers.rD *= registers.rC;
            break;
        case muld_d:
            setFlagsMul(registers.rD * registers.rD, registers.rD, registers.rD);
            registers.rD *= registers.rD;
            break;
        case anda_imm:
            cache[0] = fetch_next_word(cycles, memory);
            registers.rA &= cache[0];
            break;
        case anda_addr:
            cache[0] = fetch_next_word(cycles, memory);
            registers.special.mar = cache[0];
            cache[1] = read_word(cycles, memory);
            registers.rA &= cache[1];
            break;
        case anda_a:
            registers.rA &= registers.rA;
            break;
        case anda_b:
            registers.rA &= registers.rB;
            break;
        case anda_c:
            registers.rA &= registers.rC;
            break;
        case anda_d:
            registers.rA &= registers.rD;
            break;
        case andb_imm:
            cache[0] = fetch_next_word(cycles, memory);
            registers.rB &= cache[0];
            break;
        case andb_addr:
            cache[0] = fetch_next_word(cycles, memory);
            registers.special.mar = cache[0];
            cache[1] = read_word(cycles, memory);
            registers.rB &= cache[1];
            break;
        case andb_a:
            registers.rB &= registers.rA;
            break;
        case andb_b:
            registers.rB &= registers.rB;
            break;
        case andb_c:
            registers.rB &= registers.rC;
            break;
        case andb_d:
            registers.rB &= registers.rD;
            break;
        case andc_imm:
            cache[0] = fetch_next_word(cycles, memory);
            registers.rC &= cache[0];
            break;
        case andc_addr:
            cache[0] = fetch_next_word(cycles, memory);
            registers.special.mar = cache[0];
            cache[1] = read_word(cycles, memory);
            registers.rC &= cache[1];
            break;
        case andc_a:
            registers.rC &= registers.rA;
            break;
        case andc_b:
            registers.rC &= registers.rB;
            break;
        case andc_c:
            registers.rC &= registers.rC;
            break;
        case andc_d:
            registers.rC &= registers.rD;
            break;
        case andd_imm:
            cache[0] = fetch_next_word(cycles, memory);
            registers.rD &= cache[0];
            break;
        case andd_addr:
            cache[0] = fetch_next_word(cycles, memory);
            registers.special.mar = cache[0];
            cache[1] = read_word(cycles, memory);
            registers.rD &= cache[1];
            break;
        case andd_a:
            registers.rD &= registers.rA;
            break;
        case andd_b:
            registers.rD &= registers.rB;
            break;
        case andd_c:
            registers.rD &= registers.rC;
            break;
        case andd_d:
            registers.rD &= registers.rD;
            break;
        case ora_imm:
            cache[0] = fetch_next_word(cycles, memory);
            registers.rA |= cache[0];
            break;
        case ora_addr:
            cache[0] = fetch_next_word(cycles, memory);
            registers.special.mar = cache[0];
            cache[1] = read_word(cycles, memory);
            registers.rA |= cache[1];
            break;
        case ora_a:
            registers.rA |= registers.rA;
            break;
        case ora_b:
            registers.rA |= registers.rB;
            break;
        case ora_c:
            registers.rA |= registers.rC;
            break;
        case ora_d:
            registers.rA |= registers.rD;
            break;
        case orb_imm:
            cache[0] = fetch_next_word(cycles, memory);
            registers.rB |= cache[0];
            break;
        case orb_addr:
            cache[0] = fetch_next_word(cycles, memory);
            registers.special.mar = cache[0];
            cache[1] = read_word(cycles, memory);
            registers.rB |= cache[1];
            break;
        case orb_a:
            registers.rB |= registers.rA;
            break;
        case orb_b:
            registers.rB |= registers.rB;
            break;
        case orb_c:
            registers.rB |= registers.rC;
            break;
        case orb_d:
            registers.rB |= registers.rD;
            break;
        case orc_imm:
            cache[0] = fetch_next_word(cycles, memory);
            registers.rC |= cache[0];
            break;
        case orc_addr:
            cache[0] = fetch_next_word(cycles, memory);
            registers.special.mar = cache[0];
            cache[1] = read_word(cycles, memory);
            registers.rC |= cache[1];
            break;
        case orc_a:
            registers.rC |= registers.rA;
            break;
        case orc_b:
            registers.rC |= registers.rB;
            break;
        case orc_c:
            registers.rC |= registers.rC;
            break;
        case orc_d:
            registers.rC |= registers.rD;
            break;
        case ord_imm:
            cache[0] = fetch_next_word(cycles, memory);
            registers.rD |= cache[0];
            break;
        case ord_addr:
            cache[0] = fetch_next_word(cycles, memory);
            registers.special.mar = cache[0];
            cache[1] = read_word(cycles, memory);
            registers.rD |= cache[1];
            break;
        case ord_a:
            registers.rD |= registers.rA;
            break;
        case ord_b:
            registers.rD |= registers.rB;
            break;
        case ord_c:
            registers.rD |= registers.rC;
            break;
        case ord_d:
            registers.rD |= registers.rD;
            break;
        case xora_imm:
            cache[0] = fetch_next_word(cycles, memory);
            registers.rA -= cache[0];
            break;
        case xora_addr:
            cache[0] = fetch_next_word(cycles, memory);
            registers.special.mar = cache[0];
            cache[1] = read_word(cycles, memory);
            registers.rA -= cache[1];
            break;
        case xora_a:
            registers.rA ^= registers.rA;
            break;
        case xora_b:
            registers.rA ^= registers.rB;
            break;
        case xora_c:
            registers.rA ^= registers.rC;
            break;
        case xora_d:
            registers.rA ^= registers.rD;
            break;
        case xorb_imm:
            cache[0] = fetch_next_word(cycles, memory);
            registers.rB ^= cache[0];
            break;
        case xorb_addr:
            cache[0] = fetch_next_word(cycles, memory);
            registers.special.mar = cache[0];
            cache[1] = read_word(cycles, memory);
            registers.rB ^= cache[1];
            break;
        case xorb_a:
            registers.rB ^= registers.rA;
            break;
        case xorb_b:
            registers.rB ^= registers.rB;
            break;
        case xorb_c:
            registers.rB ^= registers.rC;
            break;
        case xorb_d:
            registers.rB ^= registers.rD;
            break;
        case xorc_imm:
            cache[0] = fetch_next_word(cycles, memory);
            registers.rC ^= cache[0];
            break;
        case xorc_addr:
            cache[0] = fetch_next_word(cycles, memory);
            registers.special.mar = cache[0];
            cache[1] = read_word(cycles, memory);
            registers.rC ^= cache[1];
            break;
        case xorc_a:
            registers.rC ^= registers.rA;
            break;
        case xorc_b:
            registers.rC ^= registers.rB;
            break;
        case xorc_c:
            registers.rC ^= registers.rC;
            break;
        case xorc_d:
            registers.rC ^= registers.rD;
            break;
        case xord_imm:
            cache[0] = fetch_next_word(cycles, memory);
            registers.rD ^= cache[0];
            break;
        case xord_addr:
            cache[0] = fetch_next_word(cycles, memory);
            registers.special.mar = cache[0];
            cache[1] = read_word(cycles, memory);
            registers.rD ^= cache[1];
            break;
        case xord_a:
            registers.rD ^= registers.rA;
            break;
        case xord_b:
            registers.rD ^= registers.rB;
            break;
        case xord_c:
            registers.rD ^= registers.rC;
            break;
        case xord_d:
            registers.rD ^= registers.rD;
            break;
        case cpya_a:
            registers.rA = registers.rA;
            break;
        case cpya_b:
            registers.rA = registers.rB;
            break;
        case cpya_c:
            registers.rA = registers.rC;
            break;
        case cpya_d:
            registers.rA = registers.rD;
            break;
        case cpyb_a:
            registers.rB = registers.rA;
            break;
        case cpyb_b:
            registers.rB = registers.rB;
            break;
        case cpyb_c:
            registers.rB = registers.rC;
            break;
        case cpyb_d:
            registers.rB = registers.rD;
            break;
        case cpyc_a:
            registers.rC = registers.rA;
            break;
        case cpyc_b:
            registers.rC = registers.rB;
            break;
        case cpyc_c:
            registers.rC = registers.rC;
            break;
        case cpyc_d:
            registers.rC = registers.rD;
            break;
        case cpyd_a:
            registers.rD = registers.rA;
            break;
        case cpyd_b:
            registers.rD = registers.rB;
            break;
        case cpyd_c:
            registers.rD = registers.rC;
            break;
        case cpyd_d:
            registers.rD = registers.rD;
            break;
        case nota:
            registers.rA = ~registers.rA;
            break;
        case notb:
            registers.rB = ~registers.rB;
            break;
        case notc:
            registers.rC = ~registers.rC;
            break;
        case notd:
            registers.rD = ~registers.rD;
            break;
        case lsha:
            cache[0] = fetch_next_word(cycles, memory);
            registers.rA = (registers.rA << cache[0]);
            break;
        case rsha:
            cache[0] = fetch_next_word(cycles, memory);
            registers.rA = (registers.rA >> cache[0]);
            break;
        case lshb:
            cache[0] = fetch_next_word(cycles, memory);
            registers.rB = (registers.rB << cache[0]);
            break;
        case rshb:
            cache[0] = fetch_next_word(cycles, memory);
            registers.rB = (registers.rB >> cache[0]);
            break;
        case lshc:
            cache[0] = fetch_next_word(cycles, memory);
            registers.rC = (registers.rC << cache[0]);
            break;
        case rshc:
            cache[0] = fetch_next_word(cycles, memory);
            registers.rC = (registers.rC >> cache[0]);
            break;
        case lshd:
            cache[0] = fetch_next_word(cycles, memory);
            registers.rD = (registers.rD << cache[0]);
            break;
        case rshd:
            cache[0] = fetch_next_word(cycles, memory);
            registers.rD = (registers.rD >> cache[0]);
            break;
        case cmpa_a:
            setFlagsSub(registers.rA + registers.rA, registers.rA, registers.rA);
            break;
        case cmpa_b:
            setFlagsSub(registers.rA + registers.rB, registers.rA, registers.rB);
            break;
        case cmpa_c:
            setFlagsSub(registers.rA + registers.rC, registers.rA, registers.rC);
            break;
        case cmpa_d:
            setFlagsSub(registers.rA + registers.rD, registers.rA, registers.rD);
            break;
        case cmpb_a:
            setFlagsSub(registers.rB + registers.rA, registers.rB, registers.rA);
            break;
        case cmpb_b:
            setFlagsSub(registers.rB + registers.rB, registers.rB, registers.rB);
            break;
        case cmpb_c:
            setFlagsSub(registers.rB + registers.rC, registers.rB, registers.rC);
            break;
        case cmpb_d:
            setFlagsSub(registers.rB + registers.rD, registers.rB, registers.rD);
            break;
        case cmpc_a:
            setFlagsSub(registers.rC + registers.rA, registers.rC, registers.rA);
            break;
        case cmpc_b:
            setFlagsSub(registers.rC + registers.rB, registers.rC, registers.rB);
            break;
        case cmpc_c:
            setFlagsSub(registers.rC + registers.rC, registers.rC, registers.rC);
            break;
        case cmpc_d:
            setFlagsSub(registers.rC + registers.rD, registers.rC, registers.rD);
            break;
        case cmpd_a:
            setFlagsSub(registers.rD + registers.rA, registers.rD, registers.rA);
            break;
        case cmpd_b:
            setFlagsSub(registers.rD + registers.rB, registers.rD, registers.rB);
            break;
        case cmpd_c:
            setFlagsSub(registers.rD + registers.rC, registers.rD, registers.rC);
            break;
        case cmpd_d:
            setFlagsSub(registers.rD + registers.rD, registers.rD, registers.rD);
            break;
        case jmp_addr:
            cache[0] = fetch_next_word(cycles, memory);
            registers.special.ip = cache[0];
            break;
        case jmz_addr:
            if (registers.special.status.zo) {
                cache[0] = fetch_next_word(cycles, memory);
                registers.special.ip = cache[0];
            }
            break;
        case jmn_addr:
            if (registers.special.status.ng) {
                cache[0] = fetch_next_word(cycles, memory);
                registers.special.ip = cache[0];
            }
            break;
        case jmo_addr:
            if (registers.special.status.of) {
                cache[0] = fetch_next_word(cycles, memory);
                registers.special.ip = cache[0];
            }
            break;
        case jpp_addr:
            if (registers.special.status.py) {
                cache[0] = fetch_next_word(cycles, memory);
                registers.special.ip = cache[0];
            }
            break;
        case nop:
            cycles--;
            break;
        case pusha:
            registers.special.mdr = registers.rA;
            registers.special.mar = registers.special.sp;
            write_word(cycles, memory);
            registers.special.sp -= 2;
            if (registers.special.sp < DATA_SIZE - STACK_SIZE) {
                printf("STACK OVERFLOW ERROR! - Write operation occured at %04x outside minimum boundary.", registers.special.mar);
            }
            break;
        case popa:
            registers.special.sp += 2;
            registers.special.mar = registers.special.sp;
            cache[0] = read_word(cycles, memory);
            registers.rA = cache[0];
            if (registers.special.sp >= DATA_SIZE - 1) {
                printf("STACK OVERFLOW ERROR! - Read operation occured at %04x outside minimum boundary.", registers.special.mar);
            }
            break;
        case pushb:
            registers.special.mdr = registers.rB;
            registers.special.mar = registers.special.sp;
            write_word(cycles, memory);
            registers.special.sp -= 2;
            if (registers.special.sp < DATA_SIZE - STACK_SIZE) {
                printf("STACK OVERFLOW ERROR! - Write operation occured at %04x outside minimum boundary.", registers.special.mar);
            }
            break;
        case popb:
            registers.special.sp += 2;
            registers.special.mar = registers.special.sp;
            cache[0] = read_word(cycles, memory);
            registers.rB = cache[0];
            if (registers.special.sp >= DATA_SIZE - 1) {
                printf("STACK OVERFLOW ERROR! - Read operation occured at %04x outside minimum boundary.", registers.special.mar);
            }
            break;
        case pushc:
            registers.special.mdr = registers.rC;
            registers.special.mar = registers.special.sp;
            write_word(cycles, memory);
            registers.special.sp -= 2;
            if (registers.special.sp < DATA_SIZE - STACK_SIZE) {
                printf("STACK OVERFLOW ERROR! - Write operation occured at %04x outside minimum boundary.", registers.special.mar);
            }
            break;
        case popc:
            registers.special.sp += 2;
            registers.special.mar = registers.special.sp;
            cache[0] = read_word(cycles, memory);
            registers.rC = cache[0];
            if (registers.special.sp >= DATA_SIZE - 1) {
                printf("STACK OVERFLOW ERROR! - Read operation occured at %04x outside minimum boundary.", registers.special.mar);
            }
            break;
        case pushd:
            registers.special.mdr = registers.rD;
            registers.special.mar = registers.special.sp;
            write_word(cycles, memory);
            registers.special.sp -= 2;
            if (registers.special.sp < DATA_SIZE - STACK_SIZE) {
                printf("STACK OVERFLOW ERROR! - Write operation occured at %04x outside minimum boundary.", registers.special.mar);
            }
            break;
        case popd:
            registers.special.sp += 2;
            registers.special.mar = registers.special.sp;
            cache[0] = read_word(cycles, memory);
            registers.rD = cache[0];
            if (registers.special.sp >= DATA_SIZE - 1) {
                printf("STACK OVERFLOW ERROR! - Read operation occured at %04x outside minimum boundary.", registers.special.mar);
            }
            break;
        case call:
            cache[0] = fetch_next_word(cycles, memory);
            cache[1] = registers.special.ip;
            registers.special.mdr = cache[1];
            registers.special.mar = registers.special.sp;
            write_word(cycles, memory);
            registers.special.sp -= 2;
            if (registers.special.sp < DATA_SIZE - STACK_SIZE) {
                printf("STACK OVERFLOW ERROR! - Write operation occured at %04x outside minimum boundary.", registers.special.mar);
            }
            registers.special.ip = cache[0];
            break;
        case ret:
            registers.special.sp += 2;
            registers.special.mar = registers.special.sp;
            cache[0] = read_word(cycles, memory);
            registers.special.ip = cache[0];
            if (registers.special.sp >= DATA_SIZE - 1) {
                printf("STACK OVERFLOW ERROR! - Read operation occured at %04x outside minimum boundary.", registers.special.mar);
            }
            break;
        default:
            printf("INSTRUCTION ERROR! - Unknown instruction 0x%02x at 0x%04x\n", registers.special.ir, registers.special.ip);
            cycles = 0;
            break;
        }
        cycles--;
    }
}

BYTE cpu::fetch_next_byte(unsigned int& cycles, mem& memory) {
    registers.special.mar = registers.special.ip;
    registers.special.mdr = memory.io_read(registers.special.mar);
    registers.special.ip++;
    cycles--;
    return registers.special.mdr;
}
WORD cpu::fetch_next_word(unsigned int& cycles, mem& memory) {
    registers.special.mar = registers.special.ip;
    registers.special.mdr = memory.io_read(registers.special.mar);
    registers.special.ip++;
    cycles--;
    registers.special.mar = registers.special.ip;
    registers.special.mdr |= (memory.io_read(registers.special.mar) << 8);
    registers.special.ip++;
    cycles--;
    return registers.special.mdr;
}

int cpu::write_word(unsigned int& cycles, mem& memory) {
    cache[0] = registers.special.mdr;
    cache[1] = registers.special.mdr >> 8;
    int result = 0;
    cycles--;
    result += memory.io_write(registers.special.mar, cache[0] & 0xFF);
    cycles--;
    result += memory.io_write(registers.special.mar, cache[1] & 0xFF);
    cycles--;
    return result;
}
WORD cpu::read_word(unsigned int& cycles, mem& memory) {
    registers.special.mdr = memory.io_read(registers.special.mar);
    cycles--;
    registers.special.mdr |= (memory.io_read(registers.special.mar + 1) << 8);
    cycles--;
    return registers.special.mdr;
}

void cpu::setFlagsAdd(WORD value, WORD a, WORD b) {
    registers.special.status.ng = (signed short)value < 0 ? 1 : 0;
    registers.special.status.zo = (signed short)value == 0 ? 1 : 0;
    registers.special.status.py = (signed short)value % 2 != 0 ? 1 : 0;
    registers.special.status.of = (((signed short)a < 0) == ((signed short)b < 0)) && (((signed short)(a + b) < 0) != ((signed short)a < 0)) ? 1 : 0;
}
void cpu::setFlagsSub(WORD value, WORD a, WORD b) {
    registers.special.status.ng = (signed short)value < 0 ? 1 : 0;
    registers.special.status.zo = (signed short)value == 0 ? 1 : 0;
    registers.special.status.py = (signed short)value % 2 != 0 ? 1 : 0;
    registers.special.status.of = (((signed short)a < 0) == ((signed short)b < 0)) && (((signed short)(a - b) < 0) != ((signed short)a < 0)) ? 1 : 0;
}
void cpu::setFlagsDiv(WORD value, WORD a, WORD b) {
    registers.special.status.ng = (signed short)value < 0 ? 1 : 0;
    registers.special.status.zo = (signed short)value == 0 ? 1 : 0;
    registers.special.status.py = (signed short)value % 2 != 0 ? 1 : 0;
    registers.special.status.of = (((signed short)a < 0) == ((signed short)b < 0)) && (((signed short)(a / b) < 0) != ((signed short)a < 0)) ? 1 : 0;
}
void cpu::setFlagsMul(WORD value, WORD a, WORD b) {
    registers.special.status.ng = (signed short)value < 0 ? 1 : 0;
    registers.special.status.zo = (signed short)value == 0 ? 1 : 0;
    registers.special.status.py = (signed short)value % 2 != 0 ? 1 : 0;
    registers.special.status.of = (((signed short)a < 0) == ((signed short)b < 0)) && (((signed short)(a * b) < 0) != ((signed short)a < 0)) ? 1 : 0;
}