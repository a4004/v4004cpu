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
    printf("--- BEGIN CPU STATE ---\nIP: %04x\tIR: %04x\tSP: %04x\tMDR: %04x\tMAR: %04x\nSTATUS REGISTERS:\tZ=%x\tN=%x\tO=%x\tP=%x\nrA: %04x\trB: %04x\trC: %04x\trD: %04x\nCACHE MEMORY:\t%04x\t%04x\t%04x\t%04x\t%04x\t%04x\t%04x\t%04x\n--- END CPU STATE ---\n",
        registers.special.ip, registers.special.ir, registers.special.sp, registers.special.mdr, registers.special.mar, registers.special.status.zo, registers.special.status.ng, registers.special.status.of,
        registers.special.status.py, registers.rA, registers.rB, registers.rC, registers.rD, cache[0], cache[1], cache[2], cache[3], cache[4], cache[5], cache[6], cache[7]);
}
void cpu::execute(int cycles, mem& memory, int clock_speed_hz, bool& allowExecute) {
    while (cycles > 0 && allowExecute) {
        std::this_thread::sleep_for(std::chrono::milliseconds((int)((float)((float)1 / (float)clock_speed_hz) * 1000)));
        registers.special.ir = fetch_next_word(cycles, memory);
        //printf("\nExecuting instruction: %04x at %04x\n", registers.special.ir, registers.special.ip - 2);
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
        case add_a_imm:
            cache[0] = fetch_next_word(cycles, memory);
            setFlagsAdd(registers.rA + cache[0], registers.rA, cache[0]);
            registers.rA += cache[0];
            break;
        case add_a_addr:
            cache[0] = fetch_next_word(cycles, memory);
            registers.special.mar = cache[0];
            cache[1] = read_word(cycles, memory);
            setFlagsAdd(registers.rA + cache[1], registers.rA, cache[1]);
            registers.rA += cache[1];
            break;
        case add_a_a:
            setFlagsAdd(registers.rA + registers.rA, registers.rA, registers.rA);
            registers.rA += registers.rA;
            break;
        case add_a_b:
            setFlagsAdd(registers.rA + registers.rB, registers.rA, registers.rB);
            registers.rA += registers.rB;
            break;
        case add_a_c:
            setFlagsAdd(registers.rA + registers.rC, registers.rA, registers.rC);
            registers.rA += registers.rC;
            break;
        case add_a_d:
            setFlagsAdd(registers.rA + registers.rD, registers.rA, registers.rD);
            registers.rA += registers.rD;
            break;
        case add_b_imm:
            cache[0] = fetch_next_word(cycles, memory);
            setFlagsAdd(registers.rB + cache[0], registers.rB, cache[0]);
            registers.rB += cache[0];
            break;
        case add_b_addr:
            cache[0] = fetch_next_word(cycles, memory);
            registers.special.mar = cache[0];
            cache[1] = read_word(cycles, memory);
            setFlagsAdd(registers.rB + cache[1], registers.rB, cache[1]);
            registers.rB += cache[1];
            break;
        case add_b_a:
            setFlagsAdd(registers.rB + registers.rA, registers.rB, registers.rA);
            registers.rB += registers.rA;
            break;
        case add_b_b:
            setFlagsAdd(registers.rB + registers.rB, registers.rB, registers.rB);
            registers.rB += registers.rB;
            break;
        case add_b_c:
            setFlagsAdd(registers.rB + registers.rC, registers.rB, registers.rC);
            registers.rB += registers.rC;
            break;
        case add_b_d:
            setFlagsAdd(registers.rB + registers.rD, registers.rB, registers.rD);
            registers.rB += registers.rD;
            break;
        case add_c_imm:
            cache[0] = fetch_next_word(cycles, memory);
            setFlagsAdd(registers.rC + cache[0], registers.rC, cache[0]);
            registers.rC += cache[0];
            break;
        case add_c_addr:
            cache[0] = fetch_next_word(cycles, memory);
            registers.special.mar = cache[0];
            cache[1] = read_word(cycles, memory);
            setFlagsAdd(registers.rC + cache[1], registers.rC, cache[1]);
            registers.rC += cache[1];
            break;
        case add_c_a:
            setFlagsAdd(registers.rC + registers.rA, registers.rC, registers.rA);
            registers.rC += registers.rA;
            break;
        case add_c_b:
            setFlagsAdd(registers.rC + registers.rB, registers.rC, registers.rB);
            registers.rC += registers.rB;
            break;
        case add_c_c:
            setFlagsAdd(registers.rC + registers.rC, registers.rC, registers.rC);
            registers.rC += registers.rC;
            break;
        case add_c_d:
            setFlagsAdd(registers.rC + registers.rD, registers.rC, registers.rD);
            registers.rC += registers.rD;
            break;
        case add_d_imm:
            cache[0] = fetch_next_word(cycles, memory);
            setFlagsAdd(registers.rD + cache[0], registers.rD, cache[0]);
            registers.rD += cache[0];
            break;
        case add_d_addr:
            cache[0] = fetch_next_word(cycles, memory);
            registers.special.mar = cache[0];
            cache[1] = read_word(cycles, memory);
            setFlagsAdd(registers.rD + cache[1], registers.rD, cache[1]);
            registers.rD += cache[1];
            break;
        case add_d_a:
            setFlagsAdd(registers.rD + registers.rA, registers.rD, registers.rA);
            registers.rD += registers.rA;
            break;
        case add_d_b:
            setFlagsAdd(registers.rD + registers.rB, registers.rD, registers.rB);
            registers.rD += registers.rB;
            break;
        case add_d_c:
            setFlagsAdd(registers.rD + registers.rC, registers.rD, registers.rC);
            registers.rD += registers.rC;
            break;
        case add_d_d:
            setFlagsAdd(registers.rD + registers.rD, registers.rD, registers.rD);
            registers.rD += registers.rD;
            break;
        case sub_a_imm:
            cache[0] = fetch_next_word(cycles, memory);
            setFlagsSub(registers.rA - cache[0], registers.rA, cache[0]);
            registers.rA -= cache[0];
            break;
        case sub_a_addr:
            cache[0] = fetch_next_word(cycles, memory);
            registers.special.mar = cache[0];
            cache[1] = read_word(cycles, memory);
            setFlagsSub(registers.rA - cache[1], registers.rA, cache[1]);
            registers.rA -= cache[1];
            break;
        case sub_a_a:
            setFlagsSub(registers.rA - registers.rA, registers.rA, registers.rA);
            registers.rA -= registers.rA;
            break;
        case sub_a_b:
            setFlagsSub(registers.rA - registers.rB, registers.rA, registers.rB);
            registers.rA -= registers.rB;
            break;
        case sub_a_c:
            setFlagsSub(registers.rA - registers.rC, registers.rA, registers.rC);
            registers.rA -= registers.rC;
            break;
        case sub_a_d:
            setFlagsSub(registers.rA - registers.rD, registers.rA, registers.rD);
            registers.rA -= registers.rD;
            break;
        case sub_b_imm:
            cache[0] = fetch_next_word(cycles, memory);
            setFlagsSub(registers.rB - cache[0], registers.rB, cache[0]);
            registers.rB -= cache[0];
            break;
        case sub_b_addr:
            cache[0] = fetch_next_word(cycles, memory);
            registers.special.mar = cache[0];
            cache[1] = read_word(cycles, memory);
            setFlagsSub(registers.rB - cache[1], registers.rB, cache[1]);
            registers.rB -= cache[1];
            break;
        case sub_b_a:
            setFlagsSub(registers.rB - registers.rA, registers.rB, registers.rA);
            registers.rB -= registers.rA;
            break;
        case sub_b_b:
            setFlagsSub(registers.rB - registers.rB, registers.rB, registers.rB);
            registers.rB -= registers.rB;
            break;
        case sub_b_c:
            setFlagsSub(registers.rB - registers.rC, registers.rB, registers.rC);
            registers.rB -= registers.rC;
            break;
        case sub_b_d:
            setFlagsSub(registers.rB - registers.rD, registers.rB, registers.rD);
            registers.rB -= registers.rD;
            break;
        case sub_c_imm:
            cache[0] = fetch_next_word(cycles, memory);
            setFlagsSub(registers.rC - cache[0], registers.rC, cache[0]);
            registers.rC -= cache[0];
            break;
        case sub_c_addr:
            cache[0] = fetch_next_word(cycles, memory);
            registers.special.mar = cache[0];
            cache[1] = read_word(cycles, memory);
            setFlagsSub(registers.rC - cache[1], registers.rC, cache[1]);
            registers.rC -= cache[1];
            break;
        case sub_c_a:
            setFlagsSub(registers.rC - registers.rA, registers.rC, registers.rA);
            registers.rC -= registers.rA;
            break;
        case sub_c_b:
            setFlagsSub(registers.rC - registers.rB, registers.rC, registers.rB);
            registers.rC -= registers.rB;
            break;
        case sub_c_c:
            setFlagsSub(registers.rC - registers.rC, registers.rC, registers.rC);
            registers.rC -= registers.rC;
            break;
        case sub_c_d:
            setFlagsSub(registers.rC - registers.rD, registers.rC, registers.rD);
            registers.rC -= registers.rD;
            break;
        case sub_d_imm:
            cache[0] = fetch_next_word(cycles, memory);
            setFlagsSub(registers.rD - cache[0], registers.rD, cache[0]);
            registers.rD -= cache[0];
            break;
        case sub_d_addr:
            cache[0] = fetch_next_word(cycles, memory);
            registers.special.mar = cache[0];
            cache[1] = read_word(cycles, memory);
            setFlagsSub(registers.rD - cache[1], registers.rD, cache[1]);
            registers.rD -= cache[1];
            break;
        case sub_d_a:
            setFlagsSub(registers.rD - registers.rA, registers.rD, registers.rA);
            registers.rD -= registers.rA;
            break;
        case sub_d_b:
            setFlagsSub(registers.rD - registers.rB, registers.rD, registers.rB);
            registers.rD -= registers.rB;
            break;
        case sub_d_c:
            setFlagsSub(registers.rD - registers.rC, registers.rD, registers.rC);
            registers.rD -= registers.rC;
            break;
        case sub_d_d:
            setFlagsSub(registers.rD - registers.rD, registers.rD, registers.rD);
            registers.rD -= registers.rD;
            break;
        case div_a_imm:
            cache[0] = fetch_next_word(cycles, memory);
            setFlagsDiv(registers.rA / cache[0], registers.rA, cache[0]);
            registers.rA /= cache[0];
            break;
        case div_a_addr:
            cache[0] = fetch_next_word(cycles, memory);
            registers.special.mar = cache[0];
            cache[1] = read_word(cycles, memory);
            setFlagsDiv(registers.rA / cache[1], registers.rA, cache[1]);
            registers.rA /= cache[1];
            break;
        case div_a_a:
            setFlagsDiv(registers.rA / registers.rA, registers.rA, registers.rA);
            registers.rA /= registers.rA;
            break;
        case div_a_b:
            setFlagsDiv(registers.rA / registers.rB, registers.rA, registers.rB);
            registers.rA /= registers.rB;
            break;
        case div_a_c:
            setFlagsDiv(registers.rA / registers.rC, registers.rA, registers.rC);
            registers.rA /= registers.rC;
            break;
        case div_a_d:
            setFlagsDiv(registers.rA / registers.rD, registers.rA, registers.rD);
            registers.rA /= registers.rD;
            break;
        case div_b_imm:
            cache[0] = fetch_next_word(cycles, memory);
            setFlagsDiv(registers.rB / cache[0], registers.rB, cache[0]);
            registers.rB /= cache[0];
            break;
        case div_b_addr:
            cache[0] = fetch_next_word(cycles, memory);
            registers.special.mar = cache[0];
            cache[1] = read_word(cycles, memory);
            setFlagsDiv(registers.rB / cache[1], registers.rB, cache[1]);
            registers.rB /= cache[1];
            break;
        case div_b_a:
            setFlagsDiv(registers.rB / registers.rA, registers.rB, registers.rA);
            registers.rB /= registers.rA;
            break;
        case div_b_b:
            setFlagsDiv(registers.rB / registers.rB, registers.rB, registers.rB);
            registers.rB /= registers.rB;
            break;
        case div_b_c:
            setFlagsDiv(registers.rB / registers.rC, registers.rB, registers.rC);
            registers.rB /= registers.rC;
            break;
        case div_b_d:
            setFlagsDiv(registers.rB / registers.rD, registers.rB, registers.rD);
            registers.rB /= registers.rD;
            break;
        case div_c_imm:
            cache[0] = fetch_next_word(cycles, memory);
            setFlagsDiv(registers.rC / cache[0], registers.rC, cache[0]);
            registers.rC /= cache[0];
            break;
        case div_c_addr:
            cache[0] = fetch_next_word(cycles, memory);
            registers.special.mar = cache[0];
            cache[1] = read_word(cycles, memory);
            setFlagsDiv(registers.rC / cache[1], registers.rC, cache[1]);
            registers.rC /= cache[1];
            break;
        case div_c_a:
            setFlagsDiv(registers.rC / registers.rA, registers.rC, registers.rA);
            registers.rC /= registers.rA;
            break;
        case div_c_b:
            setFlagsDiv(registers.rC / registers.rB, registers.rC, registers.rB);
            registers.rC /= registers.rB;
            break;
        case div_c_c:
            setFlagsDiv(registers.rC / registers.rC, registers.rC, registers.rC);
            registers.rC /= registers.rC;
            break;
        case div_c_d:
            setFlagsDiv(registers.rC / registers.rD, registers.rC, registers.rD);
            registers.rC /= registers.rD;
            break;
        case div_d_imm:
            cache[0] = fetch_next_word(cycles, memory);
            setFlagsDiv(registers.rD / cache[0], registers.rD, cache[0]);
            registers.rD /= cache[0];
            break;
        case div_d_addr:
            cache[0] = fetch_next_word(cycles, memory);
            registers.special.mar = cache[0];
            cache[1] = read_word(cycles, memory);
            setFlagsDiv(registers.rD / cache[1], registers.rD, cache[1]);
            registers.rD /= cache[1];
            break;
        case div_d_a:
            setFlagsDiv(registers.rD / registers.rA, registers.rD, registers.rA);
            registers.rD /= registers.rA;
            break;
        case div_d_b:
            setFlagsDiv(registers.rD / registers.rB, registers.rD, registers.rB);
            registers.rD /= registers.rB;
            break;
        case div_d_c:
            setFlagsDiv(registers.rD / registers.rC, registers.rD, registers.rC);
            registers.rD /= registers.rC;
            break;
        case div_d_d:
            setFlagsDiv(registers.rD / registers.rD, registers.rD, registers.rD);
            registers.rD /= registers.rD;
            break;
        case mul_a_imm:
            cache[0] = fetch_next_word(cycles, memory);
            setFlagsMul(registers.rA * cache[0], registers.rA, cache[0]);
            registers.rA *= cache[0];
            break;
        case mul_a_addr:
            cache[0] = fetch_next_word(cycles, memory);
            registers.special.mar = cache[0];
            cache[1] = read_word(cycles, memory);
            setFlagsMul(registers.rA * cache[1], registers.rA, cache[1]);
            registers.rA *= cache[1];
            break;
        case mul_a_a:
            setFlagsMul(registers.rA * registers.rA, registers.rA, registers.rA);
            registers.rA *= registers.rA;
            break;
        case mul_a_b:
            setFlagsMul(registers.rA * registers.rB, registers.rA, registers.rB);
            registers.rA *= registers.rB;
            break;
        case mul_a_c:
            setFlagsMul(registers.rA * registers.rC, registers.rA, registers.rC);
            registers.rA *= registers.rC;
            break;
        case mul_a_d:
            setFlagsMul(registers.rA * registers.rD, registers.rA, registers.rD);
            registers.rA *= registers.rD;
            break;
        case mul_b_imm:
            cache[0] = fetch_next_word(cycles, memory);
            setFlagsMul(registers.rB * cache[0], registers.rB, cache[0]);
            registers.rB *= cache[0];
            break;
        case mul_b_addr:
            cache[0] = fetch_next_word(cycles, memory);
            registers.special.mar = cache[0];
            cache[1] = read_word(cycles, memory);
            setFlagsMul(registers.rB * cache[1], registers.rB, cache[1]);
            registers.rB *= cache[1];
            break;
        case mul_b_a:
            setFlagsMul(registers.rB * registers.rA, registers.rB, registers.rA);
            registers.rB *= registers.rA;
            break;
        case mul_b_b:
            setFlagsMul(registers.rB * registers.rB, registers.rB, registers.rB);
            registers.rB *= registers.rB;
            break;
        case mul_b_c:
            setFlagsMul(registers.rB * registers.rC, registers.rB, registers.rC);
            registers.rB *= registers.rC;
            break;
        case mul_b_d:
            setFlagsMul(registers.rB * registers.rD, registers.rB, registers.rD);
            registers.rB *= registers.rD;
            break;
        case mul_c_imm:
            cache[0] = fetch_next_word(cycles, memory);
            setFlagsMul(registers.rC * cache[0], registers.rC, cache[0]);
            registers.rC *= cache[0];
            break;
        case mul_c_addr:
            cache[0] = fetch_next_word(cycles, memory);
            registers.special.mar = cache[0];
            cache[1] = read_word(cycles, memory);
            setFlagsMul(registers.rC * cache[1], registers.rC, cache[1]);
            registers.rC *= cache[1];
            break;
        case mul_c_a:
            setFlagsMul(registers.rC * registers.rA, registers.rC, registers.rA);
            registers.rC *= registers.rA;
            break;
        case mul_c_b:
            setFlagsMul(registers.rC * registers.rB, registers.rC, registers.rB);
            registers.rC *= registers.rB;
            break;
        case mul_c_c:
            setFlagsMul(registers.rC * registers.rC, registers.rC, registers.rC);
            registers.rC *= registers.rC;
            break;
        case mul_c_d:
            setFlagsMul(registers.rC * registers.rD, registers.rC, registers.rD);
            registers.rC *= registers.rD;
            break;
        case mul_d_imm:
            cache[0] = fetch_next_word(cycles, memory);
            setFlagsMul(registers.rD * cache[0], registers.rD, cache[0]);
            registers.rD *= cache[0];
            break;
        case mul_d_addr:
            cache[0] = fetch_next_word(cycles, memory);
            registers.special.mar = cache[0];
            cache[1] = read_word(cycles, memory);
            setFlagsMul(registers.rD * cache[1], registers.rD, cache[1]);
            registers.rD *= cache[1];
            break;
        case mul_d_a:
            setFlagsMul(registers.rD * registers.rA, registers.rD, registers.rA);
            registers.rD *= registers.rA;
            break;
        case mul_d_b:
            setFlagsMul(registers.rD * registers.rB, registers.rD, registers.rB);
            registers.rD *= registers.rB;
            break;
        case mul_d_c:
            setFlagsMul(registers.rD * registers.rC, registers.rD, registers.rC);
            registers.rD *= registers.rC;
            break;
        case mul_d_d:
            setFlagsMul(registers.rD * registers.rD, registers.rD, registers.rD);
            registers.rD *= registers.rD;
            break;
        case and_a_imm:
            cache[0] = fetch_next_word(cycles, memory);
            registers.rA &= cache[0];
            break;
        case and_a_addr:
            cache[0] = fetch_next_word(cycles, memory);
            registers.special.mar = cache[0];
            cache[1] = read_word(cycles, memory);
            registers.rA &= cache[1];
            break;
        case and_a_a:
            registers.rA &= registers.rA;
            break;
        case and_a_b:
            registers.rA &= registers.rB;
            break;
        case and_a_c:
            registers.rA &= registers.rC;
            break;
        case and_a_d:
            registers.rA &= registers.rD;
            break;
        case and_b_imm:
            cache[0] = fetch_next_word(cycles, memory);
            registers.rB &= cache[0];
            break;
        case and_b_addr:
            cache[0] = fetch_next_word(cycles, memory);
            registers.special.mar = cache[0];
            cache[1] = read_word(cycles, memory);
            registers.rB &= cache[1];
            break;
        case and_b_a:
            registers.rB &= registers.rA;
            break;
        case and_b_b:
            registers.rB &= registers.rB;
            break;
        case and_b_c:
            registers.rB &= registers.rC;
            break;
        case and_b_d:
            registers.rB &= registers.rD;
            break;
        case and_c_imm:
            cache[0] = fetch_next_word(cycles, memory);
            registers.rC &= cache[0];
            break;
        case and_c_addr:
            cache[0] = fetch_next_word(cycles, memory);
            registers.special.mar = cache[0];
            cache[1] = read_word(cycles, memory);
            registers.rC &= cache[1];
            break;
        case and_c_a:
            registers.rC &= registers.rA;
            break;
        case and_c_b:
            registers.rC &= registers.rB;
            break;
        case and_c_c:
            registers.rC &= registers.rC;
            break;
        case and_c_d:
            registers.rC &= registers.rD;
            break;
        case and_d_imm:
            cache[0] = fetch_next_word(cycles, memory);
            registers.rD &= cache[0];
            break;
        case and_d_addr:
            cache[0] = fetch_next_word(cycles, memory);
            registers.special.mar = cache[0];
            cache[1] = read_word(cycles, memory);
            registers.rD &= cache[1];
            break;
        case and_d_a:
            registers.rD &= registers.rA;
            break;
        case and_d_b:
            registers.rD &= registers.rB;
            break;
        case and_d_c:
            registers.rD &= registers.rC;
            break;
        case and_d_d:
            registers.rD &= registers.rD;
            break;
        case or_a_imm:
            cache[0] = fetch_next_word(cycles, memory);
            registers.rA |= cache[0];
            break;
        case or_a_addr:
            cache[0] = fetch_next_word(cycles, memory);
            registers.special.mar = cache[0];
            cache[1] = read_word(cycles, memory);
            registers.rA |= cache[1];
            break;
        case or_a_a:
            registers.rA |= registers.rA;
            break;
        case or_a_b:
            registers.rA |= registers.rB;
            break;
        case or_a_c:
            registers.rA |= registers.rC;
            break;
        case or_a_d:
            registers.rA |= registers.rD;
            break;
        case or_b_imm:
            cache[0] = fetch_next_word(cycles, memory);
            registers.rB |= cache[0];
            break;
        case or_b_addr:
            cache[0] = fetch_next_word(cycles, memory);
            registers.special.mar = cache[0];
            cache[1] = read_word(cycles, memory);
            registers.rB |= cache[1];
            break;
        case or_b_a:
            registers.rB |= registers.rA;
            break;
        case or_b_b:
            registers.rB |= registers.rB;
            break;
        case or_b_c:
            registers.rB |= registers.rC;
            break;
        case or_b_d:
            registers.rB |= registers.rD;
            break;
        case or_c_imm:
            cache[0] = fetch_next_word(cycles, memory);
            registers.rC |= cache[0];
            break;
        case or_c_addr:
            cache[0] = fetch_next_word(cycles, memory);
            registers.special.mar = cache[0];
            cache[1] = read_word(cycles, memory);
            registers.rC |= cache[1];
            break;
        case or_c_a:
            registers.rC |= registers.rA;
            break;
        case or_c_b:
            registers.rC |= registers.rB;
            break;
        case or_c_c:
            registers.rC |= registers.rC;
            break;
        case or_c_d:
            registers.rC |= registers.rD;
            break;
        case or_d_imm:
            cache[0] = fetch_next_word(cycles, memory);
            registers.rD |= cache[0];
            break;
        case or_d_addr:
            cache[0] = fetch_next_word(cycles, memory);
            registers.special.mar = cache[0];
            cache[1] = read_word(cycles, memory);
            registers.rD |= cache[1];
            break;
        case or_d_a:
            registers.rD |= registers.rA;
            break;
        case or_d_b:
            registers.rD |= registers.rB;
            break;
        case or_d_c:
            registers.rD |= registers.rC;
            break;
        case or_d_d:
            registers.rD |= registers.rD;
            break;
        case xor_a_imm:
            cache[0] = fetch_next_word(cycles, memory);
            registers.rA -= cache[0];
            break;
        case xor_a_addr:
            cache[0] = fetch_next_word(cycles, memory);
            registers.special.mar = cache[0];
            cache[1] = read_word(cycles, memory);
            registers.rA -= cache[1];
            break;
        case xor_a_a:
            registers.rA ^= registers.rA;
            break;
        case xor_a_b:
            registers.rA ^= registers.rB;
            break;
        case xor_a_c:
            registers.rA ^= registers.rC;
            break;
        case xor_a_d:
            registers.rA ^= registers.rD;
            break;
        case xor_b_imm:
            cache[0] = fetch_next_word(cycles, memory);
            registers.rB ^= cache[0];
            break;
        case xor_b_addr:
            cache[0] = fetch_next_word(cycles, memory);
            registers.special.mar = cache[0];
            cache[1] = read_word(cycles, memory);
            registers.rB ^= cache[1];
            break;
        case xor_b_a:
            registers.rB ^= registers.rA;
            break;
        case xor_b_b:
            registers.rB ^= registers.rB;
            break;
        case xor_b_c:
            registers.rB ^= registers.rC;
            break;
        case xor_b_d:
            registers.rB ^= registers.rD;
            break;
        case xor_c_imm:
            cache[0] = fetch_next_word(cycles, memory);
            registers.rC ^= cache[0];
            break;
        case xor_c_addr:
            cache[0] = fetch_next_word(cycles, memory);
            registers.special.mar = cache[0];
            cache[1] = read_word(cycles, memory);
            registers.rC ^= cache[1];
            break;
        case xor_c_a:
            registers.rC ^= registers.rA;
            break;
        case xor_c_b:
            registers.rC ^= registers.rB;
            break;
        case xor_c_c:
            registers.rC ^= registers.rC;
            break;
        case xor_c_d:
            registers.rC ^= registers.rD;
            break;
        case xor_d_imm:
            cache[0] = fetch_next_word(cycles, memory);
            registers.rD ^= cache[0];
            break;
        case xor_d_addr:
            cache[0] = fetch_next_word(cycles, memory);
            registers.special.mar = cache[0];
            cache[1] = read_word(cycles, memory);
            registers.rD ^= cache[1];
            break;
        case xor_d_a:
            registers.rD ^= registers.rA;
            break;
        case xor_d_b:
            registers.rD ^= registers.rB;
            break;
        case xor_d_c:
            registers.rD ^= registers.rC;
            break;
        case xor_d_d:
            registers.rD ^= registers.rD;
            break;
        case cpy_a_a:
            registers.rA = registers.rA;
            break;
        case cpy_a_b:
            registers.rA = registers.rB;
            break;
        case cpy_a_c:
            registers.rA = registers.rC;
            break;
        case cpy_a_d:
            registers.rA = registers.rD;
            break;
        case cpy_b_a:
            registers.rB = registers.rA;
            break;
        case cpy_b_b:
            registers.rB = registers.rB;
            break;
        case cpy_b_c:
            registers.rB = registers.rC;
            break;
        case cpy_b_d:
            registers.rB = registers.rD;
            break;
        case cpy_c_a:
            registers.rC = registers.rA;
            break;
        case cpy_c_b:
            registers.rC = registers.rB;
            break;
        case cpy_c_c:
            registers.rC = registers.rC;
            break;
        case cpy_c_d:
            registers.rC = registers.rD;
            break;
        case cpy_d_a:
            registers.rD = registers.rA;
            break;
        case cpy_d_b:
            registers.rD = registers.rB;
            break;
        case cpy_d_c:
            registers.rD = registers.rC;
            break;
        case cpy_d_d:
            registers.rD = registers.rD;
            break;
        case not_a:
            registers.rA = ~registers.rA;
            break;
        case not_b:
            registers.rB = ~registers.rB;
            break;
        case not_c:
            registers.rC = ~registers.rC;
            break;
        case not_d:
            registers.rD = ~registers.rD;
            break;
        case lsh_a:
            registers.rA = (registers.rA << 1);
            break;
        case rsh_a:
            registers.rA = (registers.rA >> 1);
            break;
        case lsh_b:
            registers.rB = (registers.rB << 1);
            break;
        case rsh_b:
            registers.rB = (registers.rB >> 1);
            break;
        case lsh_c:
            registers.rC = (registers.rC << 1);
            break;
        case rsh_c:
            registers.rC = (registers.rC >> 1);
            break;
        case lsh_d:
            registers.rD = (registers.rD << 1);
            break;
        case rsh_d:
            registers.rD = (registers.rD >> 1);
            break;
        case cmp_a_a:
            setFlagsSub(registers.rA + registers.rA, registers.rA, registers.rA);
            break;
        case cmp_a_b:
            setFlagsSub(registers.rA + registers.rB, registers.rA, registers.rB);
            break;
        case cmp_a_c:
            setFlagsSub(registers.rA + registers.rC, registers.rA, registers.rC);
            break;
        case cmp_a_d:
            setFlagsSub(registers.rA + registers.rD, registers.rA, registers.rD);
            break;
        case cmp_b_a:
            setFlagsSub(registers.rB + registers.rA, registers.rB, registers.rA);
            break;
        case cmp_b_b:
            setFlagsSub(registers.rB + registers.rB, registers.rB, registers.rB);
            break;
        case cmp_b_c:
            setFlagsSub(registers.rB + registers.rC, registers.rB, registers.rC);
            break;
        case cmp_b_d:
            setFlagsSub(registers.rB + registers.rD, registers.rB, registers.rD);
            break;
        case cmp_c_a:
            setFlagsSub(registers.rC + registers.rA, registers.rC, registers.rA);
            break;
        case cmp_c_b:
            setFlagsSub(registers.rC + registers.rB, registers.rC, registers.rB);
            break;
        case cmp_c_c:
            setFlagsSub(registers.rC + registers.rC, registers.rC, registers.rC);
            break;
        case cmp_c_d:
            setFlagsSub(registers.rC + registers.rD, registers.rC, registers.rD);
            break;
        case cmp_d_a:
            setFlagsSub(registers.rD + registers.rA, registers.rD, registers.rA);
            break;
        case cmp_d_b:
            setFlagsSub(registers.rD + registers.rB, registers.rD, registers.rB);
            break;
        case cmp_d_c:
            setFlagsSub(registers.rD + registers.rC, registers.rD, registers.rC);
            break;
        case cmp_d_d:
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
        case push_a:
            registers.special.mdr = registers.rA;
            registers.special.mar = registers.special.sp;
            write_word(cycles, memory);
            registers.special.sp -= 2;
            if (registers.special.sp < DATA_SIZE - STACK_SIZE) {
                printf("STACK OVERFLOW ERROR! - Write operation occured at %04x outside minimum boundary.", registers.special.mar);
            }
            break;
        case pop_a:
            registers.special.sp += 2;
            registers.special.mar = registers.special.sp;
            cache[0] = read_word(cycles, memory);
            registers.rA = cache[0];
            if (registers.special.sp >= DATA_SIZE - 1) {
                printf("STACK OVERFLOW ERROR! - Read operation occured at %04x outside minimum boundary.", registers.special.mar);
            }
            break;
        case push_b:
            registers.special.mdr = registers.rB;
            registers.special.mar = registers.special.sp;
            write_word(cycles, memory);
            registers.special.sp -= 2;
            if (registers.special.sp < DATA_SIZE - STACK_SIZE) {
                printf("STACK OVERFLOW ERROR! - Write operation occured at %04x outside minimum boundary.", registers.special.mar);
            }
            break;
        case pop_b:
            registers.special.sp += 2;
            registers.special.mar = registers.special.sp;
            cache[0] = read_word(cycles, memory);
            registers.rB = cache[0];
            if (registers.special.sp >= DATA_SIZE - 1) {
                printf("STACK OVERFLOW ERROR! - Read operation occured at %04x outside minimum boundary.", registers.special.mar);
            }
            break;
        case push_c:
            registers.special.mdr = registers.rC;
            registers.special.mar = registers.special.sp;
            write_word(cycles, memory);
            registers.special.sp -= 2;
            if (registers.special.sp < DATA_SIZE - STACK_SIZE) {
                printf("STACK OVERFLOW ERROR! - Write operation occured at %04x outside minimum boundary.", registers.special.mar);
            }
            break;
        case pop_c:
            registers.special.sp += 2;
            registers.special.mar = registers.special.sp;
            cache[0] = read_word(cycles, memory);
            registers.rC = cache[0];
            if (registers.special.sp >= DATA_SIZE - 1) {
                printf("STACK OVERFLOW ERROR! - Read operation occured at %04x outside minimum boundary.", registers.special.mar);
            }
            break;
        case push_d:
            registers.special.mdr = registers.rD;
            registers.special.mar = registers.special.sp;
            write_word(cycles, memory);
            registers.special.sp -= 2;
            if (registers.special.sp < DATA_SIZE - STACK_SIZE) {
                printf("STACK OVERFLOW ERROR! - Write operation occured at %04x outside minimum boundary.", registers.special.mar);
            }
            break;
        case pop_d:
            registers.special.sp += 2;
            registers.special.mar = registers.special.sp;
            cache[0] = read_word(cycles, memory);
            registers.rD = cache[0];
            if (registers.special.sp >= DATA_SIZE - 1) {
                printf("STACK OVERFLOW ERROR! - Read operation occured at %04x outside minimum boundary.", registers.special.mar);
            }
            break;
        case call_addr:
            cache[0] = fetch_next_word(cycles, memory);
            cache[1] = registers.special.ip;
            registers.special.mdr = cache[1];
            registers.special.mar = registers.special.sp;
            registers.special.ip = cache[0];
            write_word(cycles, memory);
            registers.special.sp -= 2;
            if (registers.special.sp < DATA_SIZE - STACK_SIZE) {
                printf("STACK OVERFLOW ERROR! - Write operation occured at %04x outside minimum boundary.", registers.special.mar);
            }
            break;
        case ret:
            registers.special.sp += 2;
            registers.special.mar = registers.special.sp;
            registers.special.ip = read_word(cycles, memory);
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

BYTE cpu::fetch_next_byte(int& cycles, mem& memory) {
    registers.special.mar = registers.special.ip;
    registers.special.mdr = memory.io_read(registers.special.mar);
    registers.special.ip++;
    cycles--;
    return registers.special.mdr;
}
WORD cpu::fetch_next_word(int& cycles, mem& memory) {
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

int cpu::write_word(int& cycles, mem& memory) {
    cache[0] = registers.special.mdr;
    cache[1] = registers.special.mdr >> 8;
    int result = 0;
    cycles--;
    result += memory.io_write(registers.special.mar, cache[0] & 0xFF);
    cycles--;
    result += memory.io_write(registers.special.mar + 1, cache[1] & 0xFF);
    cycles--;
    return result;
}
WORD cpu::read_word(int& cycles, mem& memory) {
    registers.special.mdr = memory.io_read(registers.special.mar);
    cycles--;
    registers.special.mdr |= ((WORD)memory.io_read(registers.special.mar + 1)) << 8;
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