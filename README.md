# stm32f4-c-template

C template for a Cortex-M4 based board. This project is just a template to show the necessary steps for blinking an LED on STM32F4 Discovery board. It is intended to be used for educational purposes.

- This is a simple template to demonstrate the necessary items to write to RCC and GPIO registers.
- All other register locations can be found on the relevant processor datasheet.
- It does not need any additional libraries or startup codes.
- Clock setting is set to HSI.
- Tested on STM32F4 Discovery board.

## Install

- You will need a **toolchain** (for compiler and binutils). Get one free from - [GNU Arm Embedded Toolchain](https://developer.arm.com/open-source/gnu-toolchain/gnu-rm/downloads)
- *For windows only*, you will need **make** tool. You can just get the make tool from [gnuwin32](http://gnuwin32.sourceforge.net/packages/make.htm). Alternatively you can install the minimalist GNU tools for windows from [mingw](https://mingw-w64.org/) and [MSYS](https://www.msys2.org/)
- For the **programmer/debugger**, you can use - [stlink](https://github.com/texane/stlink) or [OpenOCD](http://openocd.org/). Though only stlink utility support is added.
- You can use your favorite code editor to view/edit the contents. Here is an open source one: [Visual Studio Code](https://code.visualstudio.com/).

## Compile

[makefile](makefile) contains the necessary build scripts and compiler flags.

Browse into the directory and run `make` to compile. You should see a similar output as shown below.

```
Cleaning...
Building template.c
   text    data     bss     dec     hex filename
    428      16      16     460     1cc template.elf
Successfully finished...
```

If you see any errors about command not found, make sure the toolchain binaries are in your `PATH`. On Windows check the *Environment Variables* for your account. On Linux/macOS run `echo $PATH` to verify your installation.

## Program

Run `make burn` to program the chip.

```
...
Flash written and verified! jolly good!
```

Install the [ST LINK](https://www.st.com/en/development-tools/st-link-v2.html) drivers if you cannot see your board when `make burn` is run.

## Disassemble

Run `make disass` / `make disass-all` to disassamble.

## Debug

In order to debug your code, connect your board to the PC, run `st-util` (comes with stlink utility) from one terminal, and from another terminal within the project directory run `make debug`. You can then use general **gdb** commands to browse through the code.

Alternatively, you can install *Cortex-Debug* plug-in from marus25 on Visual Studio Code, and debug using the VSCode interface. No need for additional terminals. An example launch script is given with the code.
