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

## 🚧 More Stuff Coming Soon 🚧 
Over the next few weeks and months you may see new content being added including the C# assembler that will allow users to write programs in the V4004 assembly language for this virtual CPU. This CPU won't be expanded beyond its current instruction set and was made for educational purposes both for myself and others who are interested. If you're interested in contributing or making changes to it then feel free to fork this repo and do just that.
