# V4004 Virtual CPU 📦
A custom virtual CPU implementation in C++ bundled together with a compatible assembler written in C#.

<p>
  <img alt="Build Passing" src="https://img.shields.io/static/v1?label=Build&message=Passing&color=limegreen&style=flat-square&logo=cplusplus&logoColor=white"/>
  <img alt="Beta" src="https://img.shields.io/static/v1?label=Latest&message=0.1.1&color=green&style=flat-square"/>
</p>

<p>
  <a href="https://visualstudio.microsoft.com/vs/"><img alt="Visual Studio" src="https://github.com/a4004/a4004/raw/main/www/svg/vs19.svg"/></a>
  <a href="https://gcc.gnu.org/"><img alt="C++" src="https://github.com/a4004/a4004/raw/main/www/svg/cpp.svg"/></a>
  <a href="https://dotnet.microsoft.com/"><img alt="C#" src="https://github.com/a4004/a4004/raw/main/www/svg/csharp.svg"/></a>
  <a href="https://git-scm.com/"><img alt="Git" src="https://github.com/a4004/a4004/raw/main/www/svg/git.svg"/></a>
  <a href="https://www.microsoft.com/en-us/windows/"><img alt="Windows" src="https://github.com/a4004/a4004/raw/main/www/svg/w10.svg"/></a> 
</p>

> **Important Note**: This software was developed using *Windows* C++ libraries including `conio.h` that may not be available on POSIX systems like *GNU/Linux* or *macOS* and, if available, correct usage of any substitute libraries may not be present within the source code of this project. If you are building and/or running this software on those systems, you may need to make certain changes in order to get the intended functionality.

## Demonstration
![Demo Animation in APNG Format](https://raw.githubusercontent.com/a4004/v4004cpu/master/demo.png)

## Getting Started
1. Download the [latest release](https://github.com/a4004/v4004cpu/releases).
2. Shift-right click the file manager window and choose *Open PowerShell window here*.

Usage:  `.\vexec.exe` `<binary file>` `<optional: clock speed in Hz>` `<optional: maximum cycles>`
> Default clock speed is 1 kHz (1000 Hz) and the maximum number of cycles is -1 (No limit)

Example:
```
PS > .\vexec.exe .\example_hello_world.v4004
```
Example running at 5 Hz for 5000 ticks:
```
PS > .\vexec.exe .\example_hello_world.v4004 5 5000
```


## 🧩 Writing Assembly Code 🧩
> **Update 06/11/22:** The bulk of the assembler has been completed and has been released under a public preview alongside the full version of the current emulator. It's possible there are bugs within the assembler and/or CPU emulator that have not yet been discovered and if any are found, updates will be released to address them.

V4004 assembly code is pretty straightforward if you know the basics of generic assembly language as it does not have any exotic features found on very complex CPU architectures. Below you'll find a table of instructions, their corresponding machine code and description:

| Mnemonic | Operands | Machine Code | Description | Notes |
| -------- | -------- | ------------ | ----------- | ----- |
| end      |          | 0x0000       | Halts the processor. | Requires padding*. |
| ret      |          | 0x0101       | Returns control to the calling routine specified. | Requires padding*. |
| nop      |          | 0x0300       | Skips to the next instruction. | Used by the assembler (in conjunction with `end`) for padding. Requires padding if used on its own*.|
| call     | `<addr>` | 0x0100       | Hands control to the function at the specified address. | |
| tin      | `<addr>` | 0xF000       | Reads from terminal input and stores the character at the specified address. | |
| tout     | `<imm>`/`<addr>`/`rA/rB/rC/rD` | *See `coredef.h`* | Outputs the character stored at the specified location to the terminal. | |
| lda      | `<imm>`/`<addr>` | 0x0FA0 or 0x0FA1 | Loads the specified value from the immediate or memory address into the A register. | |
| ldb      | `<imm>`/`<addr>` | 0x0FB0 or 0x0FB1 | Loads the specified value from the immediate or memory address into the B register. | |
| ldc      | `<imm>`/`<addr>` | 0x0FC0 or 0x0FC1 | Loads the specified value from the immediate or memory address into the C register. | |
| ldd      | `<imm>`/`<addr>` | 0x0FD0 or 0x0FD1 | Loads the specified value from the immediate or memory address into the D register. | |
| sta      | `<addr>` | 0x0EA1 | Stores the value in the A register at the specified memory address. | |
| stb      | `<addr>` | 0x0EB1 | Stores the value in the B register at the specified memory address. | |
| stc      | `<addr>` | 0x0EC1 | Stores the value in the C register at the specified memory address. | |
| std      | `<addr>` | 0x0ED1 | Stores the value in the D register at the specified memory address. | |
| push     | `rA/rB/rC/rD` | 0x020A, 0x020B, 0x020C or 0x020D | Pushes the value stored in the specified register onto the stack in memory. | |
| pop      | `rA/rB/rC/rD` | 0x021A, 0x021B, 0x021C or 0x021D | Pops the value stored at the stack pointer in memory back into the specified register. | |
| jmp      | `<addr>` | 0x03F1 | Jumps to the specified memory address and begins executing. | Unlike `call` this does not store the return address so you cannot go back to the previous part in the code using `ret`. |
| jmz      | `<addr>` | 0x03E1 | Conditional jump only if the zero flag is set. | *See `jmp`* |
| jmn      | `<addr>` | 0x03D1 | Conditional jump only if the negative flag is set. | *See `jmp`* |
| jmo      | `<addr>` | 0x03C1 | Conditional jump only if the overflow flag is set. | *See `jmp`* |
| jpp      | `<addr>` | 0x03B1 | Conditional jump only if the parity flag is set. | *See `jmp`* |
| not      | `rA/rB/rC/rD` | 0x050A, 0x050B, 0x050C or 0x050D | Bitwise NOT operation in the specified register. | |
| lsh      | `rA/rB/rC/rD` | 0x05AA, 0x05AB, 0x05AC or 0x05AD | Bitwise LSH (Left Shift) of 1 operation in the specified register. | |
| rsh      | `rA/rB/rC/rD` | 0x05BA, 0x05BB, 0x05BC or 0x05BD | Bitwise RSH (Right Shift) of 1 operation in the specified register. | |
| add      | `rA/rB/rC/rD` `<imm>`/`<addr>`/`rA/rB/rC/rD` | *See `coredef.h`* | Adds the value in the second operand to the register in the first operand. | The result is stored in the first operand. |
| sub      | `rA/rB/rC/rD` `<imm>`/`<addr>`/`rA/rB/rC/rD` | *See `coredef.h`* | Subtracts the value in the second operand from the register in the first operand. | The result is stored in the first operand. |
| div     | `rA/rB/rC/rD` `<imm>`/`<addr>`/`rA/rB/rC/rD` | *See `coredef.h`* | Divides the value in the register in the first operand by the second operand. | The result is stored in the first operand. |
| mul     | `rA/rB/rC/rD` `<imm>`/`<addr>`/`rA/rB/rC/rD` | *See `coredef.h`* | Multiplies the value in the second operand by the value of the register in the first operand. | The result is stored in the first operand. |
| and      | `rA/rB/rC/rD` `<imm>`/`<addr>`/`rA/rB/rC/rD` | *See `coredef.h`* | Bitwise AND operation in the register in the first operand using the mask from the second operand. | The result is stored in the first operand. |
| or      | `rA/rB/rC/rD` `<imm>`/`<addr>`/`rA/rB/rC/rD` | *See `coredef.h`* | Bitwise OR operation in the register in the first operand using the mask from the second operand. | The result is stored in the first operand. |
| xor      | `rA/rB/rC/rD` `<imm>`/`<addr>`/`rA/rB/rC/rD` | *See `coredef.h`* | Bitwise XOR operation in the register in the first operand using the mask from the second operand. | The result is stored in the first operand. |
| cpy      | `rA/rB/rC/rD` `rA/rB/rC/rD` | *See `coredef.h`* | Copies the value in the second operand into the first operand. | |
| cmp      | `rA/rB/rC/rD` `rA/rB/rC/rD` | *See `coredef.h`* | Compares the two operands by subtracting the value in the second operand from the first and setting the arithmetic status flags accordingly. | This operation does not store the result of the subtraction. Only the status registers change if needed. |

\* - the assembler design follows the principle that every line is 4 bytes in length when translated into machine code. This allows for easier memory address calculations when translating between named memory regions (called *symbols*) and the actual addresses of the regions. For this reason, and because of the fact some instructions (combined with opcode and operands) are less than 4 bytes, 16-bit padding instructions may be added such as `nop` or `end` that ensure memory is continuous and consistent.

### Important Things to Remember:
- Operands starting with `&` denote a memory address in base-16.
- Operands starting with `_` denote a named **data** memory address like `_data1`.
- Operands starting with `#` denote an immediate value in base-16.
- Operand keywords `rA`, `rB`, `rC` and `rD` are reserved register names.
- Other operands are treated as named memory locations such as `print` (without the prefix `_`) but it's important to note that they are treated like **executable** memory addresses and will produce warnings if used within the wrong context and errors if they cannot be resolved to a valid address.


## 👨‍💻 Writing Binary Code 👨‍💻
Programs made for the V4004 CPU have a simple executable file structure which consists of two initial bytes that indicate the size of the program and then the program code itself. That's it. The contents of the executable file are stored using 16-bits in Little Endian format which includes the size header and the main executable payload of the file. This short section will break down the `example_hello_world.v4004` file to make it easier to understand. When reading this it may be a good idea to refer to `coredef.h` which contains the assembly code mnemonics as C++ `#define` preprocessor commands.
 
The following is a hexadecimal byte representation of the file:
```
       00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F
       -----------------------------------------------
0000 | 50 00 00 F1 48 00 00 F1 65 00 00 F1 6C 00 00 F1
0010 | 6C 00 00 F1 6F 00 00 F1 20 00 00 F1 57 00 00 F1
0020 | 6F 00 00 F1 72 00 00 F1 6C 00 00 F1 64 00 00 F1
0030 | 21 00 00 F1 0D 00 00 F1 0A 00 F1 03 00 00 00 00
0040 | 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
```
The first 2 bytes `50 00` indicate the size of the payload which is 80 bytes. The hexadecimal 16-bit representation when decoded from Little Endian is `0x0050`.
The payload can be difficult to understand in this format so I've rewritten them in a similar structure to assembly programs. Keep in mind that the bytes are written in Little Endian format so something like 00 F1 48 00 would actually be 0xF100 and 0x0048 (or 0x48).

```
---HEADER---
     5000     // 0x0050 or 80 in base-10 - tells the virtual RAM how many bytes to load into memory.
---PAYLOAD---
00F1 4800     -> tout_imm "H"   // outputs the immediate (next) value 0x0048 which is ASCII for the letter "H".
00F1 6500     -> tout_imm "e"   // same again...
00F1 6C00     -> tout_imm "l"
00F1 6C00     -> tout_imm "l"
00F1 6F00     -> tout_imm "o"
00F1 2000     -> tout_imm " "
00F1 5700     -> tout_imm "W"
00F1 6F00     -> tout_imm "o"
00F1 7200     -> tout_imm "r"
00F1 6C00     -> tout_imm "l"
00F1 6400     -> tout_imm "d"
00F1 2100     -> tout_imm "!"
00F1 0D00     -> tout_imm "\r"
00F1 0A00     -> tout_imm "\n"
F103 0000     -> jmp 0          // jumps to address 0x0000 (start of the program).
0000 0000     -> end            // signals to the CPU to stop executing (never hits but it's good practice to end with this and not random data)
...           // more zeroes for padding
```
> **Yet Another Important Note:** The address 0x0000 (and any addresses referenced in the assembly code a relative to the address space of the *virtual RAM* and not the byte-based indexes of the actual hex bytes in the file. For example, the address 0x0000 is actually 0x0002 in the file because the first two bytes are actually taken up by the size header which are not copied into the virtual RAM.
