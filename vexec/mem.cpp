#include <stdio.h>
#include "mem.h"

void mem::initialise() {
	for (WORD i = 0; i < DATA_SIZE; i++) {
		DATA[i] = 0;
	}
}

void mem::load(PBYTE data) {
	WORD length = (WORD)(*data) | ((WORD)(*(data + 1)) << 8);
	for (WORD i = 0; i < length; i++) {
		DATA[i] = *(data + i + 2);
	}
}

BYTE mem::io_read(WORD addr) {
	try {
		if (addr < 0 || addr > DATA_SIZE - 1) {
			throw (addr < 0) - (addr > DATA_SIZE - 1);
		}
		BYTE data = DATA[addr];
		return data;
	}
	catch (int code) {
		printf("I/O ERROR! - Tried to READ from 0x%04x\t Code: 0x%02x\n", addr, code);
		return (BYTE)0;
	}
}

int mem::io_write(WORD addr, BYTE data) {
	try {
		if (addr < 0 || addr > DATA_SIZE - 1) {
			throw (addr < 0) - (addr > DATA_SIZE - 1);
		}
		DATA[addr] = data;
		return (int)0;
	}
	catch (int code) {
		printf("I/O ERROR! - Tried to READ from 0x%04x\t Code: 0x%02x\n", addr, code);
		return code;
	}
}

void mem::debug_dump() {
	printf("--- BEGIN MEMORY DUMP ---\n");
	for (WORD i = 0; i < DATA_SIZE; i += 16) {
		printf("%04x\t", i);
		for (WORD j = 0; j < 16; j++) {
			printf("%02x ", DATA[i + j]);
		}
		printf("\n");
	}
	printf("--- END MEMORY DUNP ---\n");
}
