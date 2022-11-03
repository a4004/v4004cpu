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
> **Another Important Note:** The project isn't entirely finished and is missing a working assembler at the time of writing. The releases section of this repository only includes the demo version of the executable and the example V4004 binary "Hello World!" program. The demo version of the emulator has a reduced memory size in order to make it easier to demonstrate it is working. If you require more than 128 bytes of memory, you will have to compile the emulator yourself or wait until the next release.
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
The payload can be difficult to understand in this format so I've rewritten them in a similar structure to assembly programs:

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


## 🚧 More Stuff Coming Soon 🚧 
Over the next few weeks and months you may see new content being added including the C# assembler that will allow users to write programs in the V4004 assembly language for this virtual CPU. This CPU won't be expanded beyond its current instruction set and was made for educational purposes both for myself and others who are interested. If you're interested in contributing or making changes to it then feel free to fork this repo and do just that.
