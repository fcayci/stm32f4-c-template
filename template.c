/*
 * STM32F407 - C template
 * Toggles LEDs on GPIOD Pins 12, 13, 14, 15.
 */

/*************************************************
* Definitions
*************************************************/
// Define some types for readibility
#define int32_t         int
#define int16_t         short
#define int8_t          char
#define uint32_t        unsigned int
#define uint16_t        unsigned short
#define uint8_t         unsigned char

// Define the base addresses for RCC and GPIOD peripherals.
// You can calculate the addresses relative to periph_base,
// or directly assign them
// Base addresses can be found on page 64 in RM0090
#define PERIPH_BASE     ((uint32_t) 0x40000000)

#define GPIOD_BASE      (PERIPH_BASE + 0x20C00) // GPIOD base address is 0x40020C00
#define RCC_BASE        (PERIPH_BASE + 0x23800) //   RCC base address is 0x40023800

#define STACKINIT       0x20008000
#define LEDDELAY        800000

/*
 * Register Addresses
 * We will use the registers from RCC and GPIO, thus we created a struct to
 * bring together all the registers in a module. The list is in order.
 */

 /* General-purpose I/Os (GPIO)
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

/* Reset and clock control for STM32F405xx/07xx and STM32F415xx/17xx(RCC)
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

// Function declarations. Add your functions here
void copy_data(void);
void nmi_handler(void);
int32_t main(void);
void delay(volatile uint32_t s);

// This is what we will use to write to ports
// You can access to each register by using ->
// e.g. GPIOD->MODER
// Define one for each of the registers you want to use
#define GPIOD  ((GPIO_type  *) GPIOD_BASE)
#define RCC    ((RCC_type   *)   RCC_BASE)

/*************************************************
* Vector Table
*************************************************/
// Attribute puts table in beginning of .vector section
//   which is the beginning of .text section in the linker script
// Add other vectors in order here
// Vector table can be found on page 374 in RM0090
uint32_t (* const vector_table[])
__attribute__ ((section(".vectors"))) = {
	((uint32_t *) STACKINIT),          /* 0x00 Stack Pointer */
	((uint32_t *) main),               /* 0x04 Reset         */
	((uint32_t *) nmi_handler),        /* 0x08 NMI           */
	0,                                 /* 0x0C Hardfault     */
	0,                                 /* 0x10 MemManage     */
};

/*************************************************
* Copy the data contents from LMA to VMA
*************************************************/
void copy_data(void)
{
	extern char _etext, _sdata, _edata, _sbss, _ebss;
	char *src = &_etext;
	char *dst = &_sdata;

	/* ROM has data at end of text; copy it.  */
	while (dst < &_edata)
		*dst++ = *src++;

	/* Zero bss.  */
	for (dst = &_sbss; dst< &_ebss; dst++)
		*dst = 0;
}

// Non-maskable interrupt handler function
void nmi_handler(void)
{
	for (;;);  // Wait forever
}

/*************************************************
* Main code starts from here
*************************************************/
int32_t main(void)
{
	// Copy LMA to VMA for data section
	copy_data();
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

	__asm__("NOP"); // Assembly inline can be used if needed

	// Should never reach here
	return 0;
}

// A simple and not accurate delay function
void delay(volatile uint32_t s)
{
	for(s; s>0; s--){
		// Do nothing
	}
}
