# stm32f4-c-template

C template for an Cortex-M4 based board. This project is just a template to show the necessary steps for blinking an LED on an ARM Cortex M4-based board. It is intended to be used for educational purposes.

It only includes structs for `GPIO` and `RCC` peripherals for the STM32F4 chip. Other peripherals are not added.

Tested on STM32F4 Discovery board with STM32F407VG chip.

## Install
You will need an ARM compiler, a programmer & debugger and the `make` utility on Windows only.
Make sure they are added to your `PATH`.

* Toolchain - [GNU ARM Toolchain](https://developer.arm.com/open-source/gnu-toolchain/gnu-rm/downloads)
* (Windows only) - [MinGW and MSYS ](http://www.mingw.org/)
* Programmer & Debugger - [STLink](https://github.com/texane/stlink)

## Compile

Browse into the directory and run `make` to compile.

```
Cleaning...
Building template.c
   text    data     bss     dec     hex filename
    428      16      16     460     1cc template.elf
Successfully finished...
```

## Program

Run `make burn` to program the chip.

```
...
Flash written and verified! jolly good!
```

# Debug

You can debug your application using `gdb` by following the steps.

* Run `st-util` from one terminal
* Run `make debug` from a second terminal to debug the program.
