#include "TM4C123.h" 
#include <stdlib.h>
#include <stdint.h> 

int main()  
{
	SYSCTL->RCGCSSI |= 0x1; // Enable and provide a clock to SPI0
 	SYSCTL->RCGCGPIO |= 0x21; // Enable and provide a clock to GPIO PortA and PortF 
	
	GPIOA->AFSEL |= 0x3C; // Enable alternate functions on PA2, PA3, PA4, PA5  
	GPIOA->PCTL |= 0x222200; // Assign SPI signals to PA2, PA3, PA4, PA5   
	GPIOA->DEN |= 0x3C; // Enable digital functions for PA2, PA3, PA4, PA5    
	GPIOA->DIR &= ~0x8; // Set PA3 as input 
	
	SSI0->CR1 = 0x4; // Disable SPI and configure it as a slave 
	SSI0->CC = 0x0; // Select the SPI Baud Clock Source as system clock 
	SSI0->CPSR = 0x10; // Set the clock frequency to 1 MHz 
	SSI0->CR0 = 0x7; // Freescale SPI mode, 1 Mbps bit rate, 8 data bits 
  	SSI0->CR1 |= 0x2; // Enable SPI    
	
	GPIOF->DIR |= 0xE; // Configure LED pins(PF1, PF2, PF3) to be an output
	GPIOF->DEN |= 0xE; // Enable digital functions for LED pins 
	GPIOF->DATA &= ~0xE; // Turn off the LEDs 
	
	while(1)
	{
			while((SSI0->SR & 0x4) == 0x0); // Wait until receive FIFO is not empty  
			if(SSI0->DR == 'R'){GPIOF->DATA = 0x2;} // If 'R' is received, turn on the red LED  
			else if (SSI0->DR == 'G'){GPIOF->DATA = 0x8;} // If 'G' is received, turn on the green LED 
			else if (SSI0->DR == 'B'){GPIOF->DATA = 0x4;} // If 'B' is received, turn on the blue LED 		
	}
}
