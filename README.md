# stm32f4-c-template

C template for an STM32F407-based board. (Cortex-M4)

Tested on STM32F4 Discovery board with STM32F407VG chip.

# Install
* Toolchain - [GNU ARM Toolchain](https://developer.arm.com/open-source/gnu-toolchain/gnu-rm/downloads)
* (Windows only) - [MinGW and MSYS ](http://www.mingw.org/)
* Programmer & Debugger - [STLink](https://github.com/texane/stlink)

# Compile
* Browse into the directory and run `make` to compile.
```
Cleaning...
Building template.c
   text	   data	    bss	    dec	    hex	filename
    228	      0	      0	    228	     e4	template.elf
Successfully finished...
```

# Program
* Run `make burn` to program the chip.
```
...
Flash written and verified! jolly good!
```

# Debug
* Run `st-util` from one terminal
* Run `make debug` from a second terminal to debug the program.
* You can turn off tui layout with `tui disable` if needed
