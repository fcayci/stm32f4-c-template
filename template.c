/*
 * author: Furkan Cayci
 * description: STM32F4 - C template
 *   toggles LEDs on GPIOD Pins 12, 13, 14, 15
 *   compatible STM32F4 Discovery board
 *   reference document: RM0090
 *
 * This file is just a template to show the necessary steps
 *   for blinking an LED on an ARM CM4-based board. Should
 *   not be used in production.
 *
 * It only includes structures for GPIO and RCC peripherals.
 *   Other structures are not included, but should be a nice
 *   guideline on how things work
 */

/*************************************************
* Definitions
*************************************************/
// Define some types for readability
// Normally, should use these from stdint.h

#define int8_t          char
#define int16_t         short
#define int32_t         int
#define uint8_t         unsigned char
#define uint16_t        unsigned short
#define uint32_t        unsigned int

// Define the base addresses for RCC and GPIOD peripherals.
// You can calculate the addresses relative to periph_base,
//   or directly assign them
// Base addresses can be found on page 64 in RM0090

#define PERIPH_BASE     ((uint32_t) 0x40000000)
#define GPIOD_BASE      (PERIPH_BASE + 0x20C00) // GPIOD base address is 0x40020C00
#define RCC_BASE        (PERIPH_BASE + 0x23800) //   RCC base address is 0x40023800

// Project related defines
#define LEDDELAY        800000

/*************************************************
* Register Addresses
*************************************************/
/*
 * In this template code, we will only turn on some LEDs which require at least
 * RCC and GPIO registers.
 * RCC is used to turn on the clock to the GPIOD module
 * GPIOD is used to drive the LED pins
 * In order to use the registers, we can just create a preprocessor directive and
 * assign the actual address directly, or create a new data type for each module
 * containing the registers.
 *
 * Usually these definitions are given by the processor manufacturer, and included in the code.
 * For the sake of simplicity and understanding, we will declare them here.
 *
 * rm0090 is the document for STM32F405/407 processors, and end of RCC and GPIO sections include
 * the register list in order.
 *
 * Each of these items should also be guarded with volatile for optimization misbehavior
 */

/*
 * Reset and Clock Control (RCC) module
 *   page 267 from rm0090
 */
typedef struct {
	volatile uint32_t CR;         /* Address offset: 0x00 */
	volatile uint32_t PLLCFGR;    /* Address offset: 0x04 */
	volatile uint32_t CFGR;       /* Address offset: 0x08 */
	volatile uint32_t CIR;        /* Address offset: 0x0C */
	volatile uint32_t AHB1RSTR;   /* Address offset: 0x10 */
	volatile uint32_t AHB2RSTR;   /* Address offset: 0x14 */
	volatile uint32_t AHB3RSTR;   /* Address offset: 0x18 */
	volatile uint32_t RESERVED1;  /* Address offset: 0x1C */
	volatile uint32_t APB1RSTR;   /* Address offset: 0x20 */
	volatile uint32_t APB2RSTR;   /* Address offset: 0x24 */
	volatile uint32_t RESERVED2;  /* Address offset: 0x28 */
	volatile uint32_t RESERVED3;  /* Address offset: 0x2C */
	volatile uint32_t AHB1ENR;    /* Address offset: 0x30 */
	volatile uint32_t AHB2ENR;    /* Address offset: 0x34 */
	volatile uint32_t AHB3ENR;    /* Address offset: 0x38 */
	volatile uint32_t RESERVED4;  /* Address offset: 0x3C */
	volatile uint32_t APB1ENR;    /* Address offset: 0x40 */
	volatile uint32_t APB2ENR;    /* Address offset: 0x44 */
	volatile uint32_t RESERVED5;  /* Address offset: 0x48 */
	volatile uint32_t RESERVED6;  /* Address offset: 0x4C */
	volatile uint32_t AHB1LPENR;  /* Address offset: 0x50 */
	volatile uint32_t AHB2LPENR;  /* Address offset: 0x54 */
	volatile uint32_t AHB3LPENR;  /* Address offset: 0x58 */
	volatile uint32_t RESERVED7;  /* Address offset: 0x5C */
	volatile uint32_t APB1LPENR;  /* Address offset: 0x60 */
	volatile uint32_t APB2LPENR;  /* Address offset: 0x64 */
	volatile uint32_t RESERVED8;  /* Address offset: 0x68 */
	volatile uint32_t RESERVED9;  /* Address offset: 0x6C */
	volatile uint32_t BDCR;       /* Address offset: 0x70 */
	volatile uint32_t CSR;        /* Address offset: 0x74 */
	volatile uint32_t RESERVED10; /* Address offset: 0x78 */
	volatile uint32_t RESERVED11; /* Address offset: 0x7C */
	volatile uint32_t SSCGR;      /* Address offset: 0x80 */
	volatile uint32_t PLLI2SCFGR; /* Address offset: 0x84 */
} RCC_type;

 /*
  * General-purpose I/Os (GPIO) module
  *   page 288 from rm0090
  */
typedef struct {
	volatile uint32_t MODER;      /* Address offset: 0x00 */
	volatile uint32_t OTYPER;     /* Address offset: 0x04 */
	volatile uint32_t OSPEEDR;    /* Address offset: 0x08 */
	volatile uint32_t PUPDR;      /* Address offset: 0x0C */
	volatile uint32_t IDR;        /* Address offset: 0x10 */
	volatile uint32_t ODR;        /* Address offset: 0x14 */
	volatile uint32_t BSRR;       /* Address offset: 0x18 */
	volatile uint32_t LCKR;       /* Address offset: 0x1C */
	volatile uint32_t AFRL;       /* Address offset: 0x20 */
	volatile uint32_t AFRH;       /* Address offset: 0x24 */
} GPIO_type;

/*
 * Create pointers based at the given addresses
 * You can access to each register by using -> operator
 * e.g. GPIOD->MODER
 *
 * Now we can access to the memory locations of these peripherals
 */
#define GPIOD  ((GPIO_type  *) GPIOD_BASE)
#define RCC    ((RCC_type   *)   RCC_BASE)

// Function declarations
void Reset_Handler(void);
void _init_data(void);
void Default_Handler(void);
int main(void);
void delay(volatile uint32_t);

// just to make compiler happy
typedef void (* const intfunc)(void);

// __StackTop is defined in the linker script
// which should point to the top of the stack
extern unsigned long __StackTop;

/*************************************************
* Vector Table
*************************************************/
/*
 * attribute keyword puts table in beginning of .vectors section
 * which is the beginning of .text section in the linker script
 * Change function name to a given exception, or add
 *   processor specific interrupts in order here.
 * For STM32F407 - Vector table can be found on page 374 in rm0090
 */
__attribute__ ((section(".vectors")))
void (* const vector_table[])(void) = {
	(intfunc)((unsigned long)&__StackTop), /* 0x000 Stack Pointer */
	Reset_Handler,                      /* 0x004 Reset         */
	Default_Handler,                    /* 0x008 NMI           */
	Default_Handler,                    /* 0x00C HardFaullt    */
	Default_Handler,                    /* 0x010 MemManage     */
	Default_Handler,                    /* 0x014 BusFault      */
	Default_Handler,                    /* 0x018 UsageFault    */
	0,                                  /* 0x01C Reserved      */
	0,                                  /* 0x020 Reserved      */
	0,                                  /* 0x024 Reserved      */
	0,                                  /* 0x028 Reserved      */
	Default_Handler,                    /* 0x02C SVCall        */
	Default_Handler,                    /* 0x030 Debug Monitor */
	0,                                  /* 0x034 Reserved      */
	Default_Handler,                    /* 0x038 PendSV        */
	Default_Handler                     /* 0x03C SysTick       */
	                                    /* processor specific interrupts */
};

/*
 * Entry point for the program
 * initializes data, bss sections and
 * calls main function.
 * It doesn't do any FPU initialization
 */
void Reset_Handler(void)
{
	/* initialize data and bss sections */
	_init_data();

	/* Reset the RCC clock configuration to the default reset state */
	/* Set HSION bit */
	RCC->CR |= (1 << 0);

	/* Reset CFGR register */
	RCC->CFGR = 0x00000000;

	/* Reset HSEON (16), CSSON (19) and PLLON (24) bits */
	RCC->CR &= ~(uint32_t)((1 << 16) | (1 << 19) | (1 << 24));

	/* Reset PLLCFGR register to reset value */
	RCC->PLLCFGR = 0x24003010;

	/* Reset HSEBYP bit */
	RCC->CR &= ~(uint32_t)(1 << 18);

	// The processor is now running from the HSI clock which is the internal
	//   high speed clock

	/* Disable all interrupts */
	RCC->CIR = 0x00000000;

	// call main function
	main();

	// Wait forever in case the code exit from main
	for (;;);
}

/*************************************************
* Copy the data contents from LMA (load address) to VMA (run address)
* Initialize data and bss sections
*************************************************/
void _init_data(void)
{
	// these are symbols from linker script
	extern unsigned long __etext, __data_start__, __data_end__, __bss_start__, __bss_end__;
	unsigned long *src = &__etext;
	unsigned long *dst = &__data_start__;

	/* ROM has data at end of text. copy it */
	while (dst < &__data_end__)
		*dst++ = *src++;

	/* zero bss */
	for (dst = &__bss_start__; dst< &__bss_end__; dst++)
		*dst = 0;
}

/*
 *  Default handler function
 *  All the exceptions will hit here since
 *    they are all pointed to this function
 *
 */
void Default_Handler(void)
{
	for (;;);  // Wait forever
}

/*************************************************
* Main code starts from here
*************************************************/
int main(void)
{

	// Each module is powered separately. In order to turn on a module
	// we need to enable the relevant clock.
	// Set Bit 3 to enable GPIOD clock in AHB1ENR
	// AHB1ENR: XXXX XXXX XXXX XXXX XXXX XXXX XXXX 1XXX
	RCC->AHB1ENR |= 0x00000008;

	// Another way to write a 1 to a bit location is to shift it that much
	// Meaning shift number 1, 3 times to the left. Which would result in
	// 0b1000 or 0x8
	// RCC->AHB1ENR |= (1 << 3);

	// In order to make a pin output, we need to write 01 to the relevant
	// section in MODER register
	// We first need to AND it to reset them, then OR it to set them.
	//                     bit31                                         bit0
	// MODER register bits : 01 01 01 01 XX XX XX XX XX XX XX XX XX XX XX XX
	//                      p15                                           p0

	GPIOD->MODER &= 0x00FFFFFF;   // Reset bits 31-24 to clear old values
	GPIOD->MODER |= 0x55000000;   // Set MODER bits to 01 (0101 is 5 in hex)

	// You can do the same setup with shifting
	// GPIOD->MODER &= ~(0xFF << 24);
	// GPIOD->MODER |=  (0x55 << 24);

	// Set Pins 12-15 to 1 to turn on all LEDs
	// ODR: 1111 XXXX XXXX XXXX
	GPIOD->ODR |= 0xF000;

	// We can do the same setup with shifting
	// GPIOD->ODR |= (0xF << 12);

	while(1){
		delay(LEDDELAY);
		GPIOD->ODR ^= 0xF000;  // Toggle LEDs
	}

	__asm("NOP"); // Assembly inline can be used if needed
	return 0;
}

// A simple and not accurate delay function
void delay(volatile uint32_t s)
{
	for(; s>0; s--){
		// Do nothing
	}
}
