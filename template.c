/*
 * author: Furkan Cayci
 * description: STM32F4 - C template
 *   toggles LEDs on GPIOD Pins 12, 13, 14, 15
 *   compatible STM32F4 Discovery board
 *   reference document: RM0090
 *
 * This file is just a template to show the necessary steps
 *   for blinking an LED on an ARM CM4-based board.
 *   It only includes structs for GPIO and RCC peripherals.
 */
/*************************************************
* Definitions
*************************************************/
// Define some types for readability
#define int8_t          char
#define int16_t         short
#define int32_t         int
#define uint8_t         unsigned char
#define uint16_t        unsigned short
#define uint32_t        unsigned int

// Define the base addresses for RCC and GPIOD peripherals.
// You can calculate the addresses relative to periph_base,
// or directly assign them
// Base addresses can be found on page 64 in RM0090
#define PERIPH_BASE     ((uint32_t) 0x40000000)

#define GPIOD_BASE      (PERIPH_BASE + 0x20C00) // GPIOD base address is 0x40020C00
#define RCC_BASE        (PERIPH_BASE + 0x23800) //   RCC base address is 0x40023800

// Project related defs.
#define LEDDELAY        800000

/*
 * Register Addresses
 * We will use the registers from RCC and GPIO, thus we created a struct to
 * bring together all the registers in a module. The list is in order.
 *
 * Note: This part is included with a processor specific header.
 */

/*
 * Reset and Clock Control (RCC)
 *   page 267 from rm0090
 */
typedef struct {
	uint32_t CR;         /* Address offset: 0x00 */
	uint32_t PLLCFGR;    /* Address offset: 0x04 */
	uint32_t CFGR;       /* Address offset: 0x08 */
	uint32_t CIR;        /* Address offset: 0x0C */
	uint32_t AHB1RSTR;   /* Address offset: 0x10 */
	uint32_t AHB2RSTR;   /* Address offset: 0x14 */
	uint32_t AHB3RSTR;   /* Address offset: 0x18 */
	uint32_t RESERVED1;  /* Address offset: 0x1C */
	uint32_t APB1RSTR;   /* Address offset: 0x20 */
	uint32_t APB2RSTR;   /* Address offset: 0x24 */
	uint32_t RESERVED2;  /* Address offset: 0x28 */
	uint32_t RESERVED3;  /* Address offset: 0x2C */
	uint32_t AHB1ENR;    /* Address offset: 0x30 */
	uint32_t AHB2ENR;    /* Address offset: 0x34 */
	uint32_t AHB3ENR;    /* Address offset: 0x38 */
	uint32_t RESERVED4;  /* Address offset: 0x3C */
	uint32_t APB1ENR;    /* Address offset: 0x40 */
	uint32_t APB2ENR;    /* Address offset: 0x44 */
	uint32_t RESERVED5;  /* Address offset: 0x48 */
	uint32_t RESERVED6;  /* Address offset: 0x4C */
	uint32_t AHB1LPENR;  /* Address offset: 0x50 */
	uint32_t AHB2LPENR;  /* Address offset: 0x54 */
	uint32_t AHB3LPENR;  /* Address offset: 0x58 */
	uint32_t RESERVED7;  /* Address offset: 0x5C */
	uint32_t APB1LPENR;  /* Address offset: 0x60 */
	uint32_t APB2LPENR;  /* Address offset: 0x64 */
	uint32_t RESERVED8;  /* Address offset: 0x68 */
	uint32_t RESERVED9;  /* Address offset: 0x6C */
	uint32_t BDCR;       /* Address offset: 0x70 */
	uint32_t CSR;        /* Address offset: 0x74 */
	uint32_t RESERVED10; /* Address offset: 0x78 */
	uint32_t RESERVED11; /* Address offset: 0x7C */
	uint32_t SSCGR;      /* Address offset: 0x80 */
	uint32_t PLLI2SCFGR; /* Address offset: 0x84 */
} RCC_type;

 /*
  * General-purpose I/Os (GPIO)
  *   page 288 from rm0090
  */
typedef struct {
	uint32_t MODER;      /* Address offset: 0x00 */
	uint32_t OTYPER;     /* Address offset: 0x04 */
	uint32_t OSPEEDR;    /* Address offset: 0x08 */
	uint32_t PUPDR;      /* Address offset: 0x0C */
	uint32_t IDR;        /* Address offset: 0x10 */
	uint32_t ODR;        /* Address offset: 0x14 */
	uint32_t BSRR;       /* Address offset: 0x18 */
	uint32_t LCKR;       /* Address offset: 0x1C */
	uint32_t AFRL;       /* Address offset: 0x20 */
	uint32_t AFRH;       /* Address offset: 0x24 */
} GPIO_type;

// Create pointers based at the given addresses
// You can access to each register by using ->
// e.g. GPIOD->MODER
// Define one for each of the registers you want to use
#define GPIOD  ((GPIO_type  *) GPIOD_BASE)
#define RCC    ((RCC_type   *)   RCC_BASE)

// Function declarations
void Reset_Handler(void);
void _init_data(void);
void Default_Handler(void);
int32_t main(void);
void delay(volatile uint32_t);

typedef void (* const intfunc)(void);
// _stack is defined in the linker script
extern unsigned long __stack;

/*************************************************
* Vector Table
*************************************************/
// Attribute puts table in beginning of .vector section
//   which is the beginning of .text section in the linker script
// Change function name to a given exception, or add
//   processor specific interrupts in order here.
// For STM32F407 - Vector table can be found on page 374 in RM0090
__attribute__ ((section(".vectors")))
void (* const vector_table[])(void) = {
	(intfunc)((unsigned long)&__stack), /* 0x000 Stack Pointer */
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

	/* Disable all interrupts */
	RCC->CIR = 0x00000000;

	// main function
	main();

	// Wait forever
	for (;;);
}

/*************************************************
* Copy the data contents from LMA to VMA
* Initializes data and bss sections
*************************************************/
void _init_data(void)
{
	extern unsigned long __etext, __data_start__, __data_end__, __bss_start__, __bss_end__;
	unsigned long *src = &__etext;
	unsigned long *dst = &__data_start__;

	/* ROM has data at end of text; copy it.  */
	while (dst < &__data_end__)
		*dst++ = *src++;

	/* zero bss.  */
	for (dst = &__bss_start__; dst< &__bss_end__; dst++)
		*dst = 0;
}

// Default handler function
void Default_Handler(void)
{
	for (;;);  // Wait forever
}

// initialized data
volatile uint32_t iarray[4] = {2, 9, 8, 1};
// uninitialized data
volatile uint32_t uarray[4];
// read-only data
const uint32_t carray[4] = {1, 4, 1, 4};

/*************************************************
* Main code starts from here
*************************************************/
int32_t main(void)
{
	// dummy copy. doesn't actually do anything
	// added to show where the variables are placed
	for(int i=0; i<4; i++){
		iarray[i] = carray[i];
		uarray[i] = carray[i];
	}

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
	// GPIOD->MODER &= ~(0xFF << 24); //or GPIOD->MODER &= ~(0b11111111 << 24);
	// GPIOD->MODER |=  (0x55 << 24); //or GPIOD->MODER |=  (0b01010101 << 24);

	// Set Pins 12-15 to 1 to turn on all LEDs
	// ODR: 1111 XXXX XXXX XXXX
	GPIOD->ODR |= 0xF000;

	// You can do the same setup with shifting
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
	for(s; s>0; s--){
		// Do nothing
	}
}
