#pragma once

#include "coredef.h"
#include "mem.h"

struct V_STATUS_REGISTER_FLAGS {
    BIT zo : 1;  // Zero flag
    BIT ng : 1;  // Negative flag
    BIT of : 1;  // Overflow flag
    BIT py : 1;  // Parity flag

    BIT __reserved_unused_padding : 4;
};
struct V_SPECIAL_REGISTERS {
    WORD ip;  // Instruction pointer register
    WORD ir;  // Instruction register
    WORD sp;  // Stack pointer
    WORD mdr; // Memory data/buffer register
    WORD mar; // Memory address register

    V_STATUS_REGISTER_FLAGS status; // Status register
};
struct V_REGISTERS {
    WORD rA;  // A register
    WORD rB;  // B register
    WORD rC;  // C register
    WORD rD;  // D register

    V_SPECIAL_REGISTERS special; // Special purpose registers
};

static constexpr WORD CACHE_SIZE = 8;

class cpu {
public:
    void reset_cpu(mem& memory);
    void debug_dump();
    void execute(int cycles, mem& memory, int clock_speed_hz, bool& allowExecute);
private:
    void setFlagsAdd(WORD value, WORD a, WORD b);
    void setFlagsSub(WORD value, WORD a, WORD b);
    void setFlagsMul(WORD value, WORD a, WORD b);
    void setFlagsDiv(WORD value, WORD a, WORD b);

    BYTE fetch_next_byte(int& cycles, mem& memory);
    WORD fetch_next_word(int& cycles, mem& memory);
    int write_word(int& cycles, mem& memory);
    WORD read_word(int& cycles, mem& memory);

    V_REGISTERS registers;
    WORD cache[CACHE_SIZE];
};