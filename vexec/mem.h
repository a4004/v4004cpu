#pragma once

#include "coredef.h"

static constexpr WORD DATA_SIZE = 1024;
static constexpr WORD STACK_SIZE = 128;

class mem {
public:
	void initialise();
	void load(PBYTE data);
	void debug_dump();

	BYTE io_read(WORD addr);
	int io_write(WORD addr, BYTE data);
private:
	BYTE DATA[DATA_SIZE];
};