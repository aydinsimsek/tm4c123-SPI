#include "TM4C123.h" 
#include <stdlib.h>
#include <stdint.h> 

void SPI_Transmit(char c);   
void delay(void); 

void SPI_Transmit(char c)
{
	GPIOA->DATA &= ~0x8; // Select the slave by making active-low slave select line low 
	while((SSI0->SR & 0x2) == 0x0); // wait until transmit FIFO is not full   
	SSI0->DR = c; // send the data 
	while((SSI0->SR & 0x10) == 0x10); //  wait until busy bit becomes 0 
  GPIOA->DATA |= 0x8; // Deselect the slave   
}

void delay()
{
	int i; 
	for(i=0; i<1000000; i++);  
} 

int main()  
{
	SYSCTL->RCGCSSI = 0x1; // Enable and provide a clock to SPI0
 	SYSCTL->RCGCGPIO = 0x1; // Enable and provide a clock to GPIO PortA 
	
	GPIOA->AFSEL |= 0x3C; // Enable alternate functions on PA2, PA3, PA4, PA5  
	GPIOA->PCTL |= 0x222200; // Assign SPI signals to PA2, PA3, PA4, PA5   
	GPIOA->DEN |= 0x3C; // Enable digital functions for PA2, PA3, PA4, PA5    
	GPIOA->DIR |= 0x8; // Set PA3 as output 
	GPIOA->DATA |= 0x8; // Make slave select line high when idle 
	
	SSI0->CR1 = 0x0; // Disable SPI and configure it as a master 
	SSI0->CC = 0x0; // Select the SPI Baud Clock Source as system clock 
	SSI0->CPSR = 0x10; // Set the clock frequency to 1 MHz 
	SSI0->CR0 = 0x7; // Freescale SPI mode, 1 Mbps bit rate, 8 data bits 
  SSI0->CR1 |= 0x2; // Enable SPI    
	
	char red = 'R';
	char green = 'G'; 
	char blue = 'B'; 
	
	while(1)
	{
		SPI_Transmit(red); 
		delay(); 
		SPI_Transmit(green); 
		delay(); 
		SPI_Transmit(blue);  
		delay(); 
	}
}
