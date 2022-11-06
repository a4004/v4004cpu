#include <iostream>
#include "coredef.h"
#include "mem.h"
#include "cpu.h"
#include <Windows.h>

bool allowExecute = true;

BOOL WINAPI ConsoleHandler(DWORD signal) {

	if (signal == CTRL_C_EVENT) {
		printf("\n=== CPU INTERRUPTED! HALTING PROGRAM ===\n");
		allowExecute = false;
	}

	return TRUE;
}

int wmain(int argc, const wchar_t* argv[])
{
	if (argc < 2) {
		std::cerr << "Usage: vexec <binary file> <optional: clock speed in Hz> <optional: max cycles>" << std::endl;
		return 1;
	}
	
	try
	{
		HANDLE hProgramFile = CreateFileW(argv[1], GENERIC_READ,
			FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		DWORD fileSize = GetFileSize(hProgramFile, NULL);

		PBYTE program = new BYTE[fileSize];
		if (!ReadFile(hProgramFile, program, fileSize, NULL, NULL)) {
			std::cerr << "A read error was encountered with the specified file." << std::endl;
			throw 1;
		}

		int clockSpeed = 0x1000;
		int clockCycles = 0x7FFFFFFF;

		if (argc > 2) {
			try {
				int cs = _wtoi(argv[2]);
				clockSpeed = cs;
			}
			catch (...)
			{
				std::cerr << "Non critical error: Unable to parse the 'clock speed' argument. Using default 1 kHz (1000 Hz)" << std::endl;
			}
		}

		if (argc > 3) {
			try {
				int cc = _wtoi(argv[3]);
				clockCycles = cc;
			}
			catch (...)
			{
				std::cerr << "Non critical error: Unable to parse the 'max cycles' argument. Using default limit of -1 (indefinite)" << std::endl;
			}
		}

		printf("Starting CPU emulator with the following properties:\n\tFile to Run: %ls\n\tClock Speed: %d Hz\n\tCycle Limit: %d ticks\n\n...\n", argv[1], clockSpeed, clockCycles);

		cpu cpu;
		mem mem;

		cpu.reset_cpu(mem);
		mem.load(program);

		printf("\\/ PROGRAM LOADED. DUMPING CPU AND MEMORY INFORMATION BELOW \\/\n");
		cpu.debug_dump();
		mem.debug_dump();

		if (!SetConsoleCtrlHandler(ConsoleHandler, TRUE)) {
			printf("NOTICE: Failed to assign a CTRL-C exit handler. You may need to kill the program using task manager if it enters an indefinite loop.\n\n");
		}

		cpu.execute(clockCycles, mem, clockSpeed, allowExecute);

		printf("\\/ PROGRAM FINISHED EXECUTING. DUMPING CPU AND MEMORY INFORMATION BELOW \\/\n");
		cpu.debug_dump();
		mem.debug_dump();
	}
	catch (int exitCode) {
		std::cerr << "Program error. Exit Code: " << exitCode << " Windows Error Code: " << GetLastError() << std::endl;
		return exitCode;
	}
	
	return 0;
}