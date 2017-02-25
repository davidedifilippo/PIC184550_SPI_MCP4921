/* 
 * File:   spi_main.c
 * Author: Davide
 *
 * Created on 25 febbraio 2017, 10.57
 */

#include <xc.h> 
#include <plib/spi.h>
#include <stdlib.h> 

#define _XTAL_FREQ 8000000



// Chip select on pin RC0 -> output
#define ChipSelect PORTCbits.RC0 
#define Chip_Select_Direction TRISCbits.TRISC0  

// SPI bus signal direcons -> configurare come output
#define SCK_Direcon TRISBbits.RB1 
#define SDO_Direcon TRISCbits.RC7

float Sample = 0.0;

// This function sends 12 bits digital data to the DAC

void DAC(unsigned int value) {  
char temp; 
   
// Enable DAC chip
PORTCbits.RC0  = 0;                            
  
// Send High Byte 
// Store bits 8:11 to temp  
temp = (value >> 8) & 0x0F;    
   
// De?ne DAC setting (choose 1x gain)    
temp |= 0x30;
   
// Send high byte via SPI  
WriteSPI(temp);  

//--------------end high byte---------------------
   
   
// Send Low Byte   
temp = value;             // Store bits 0:7 to temp   
WriteSPI(temp);         // Send low byte via SPI 
PORTCbits.RC0 = 1;        // Disable DAC chip   
}

/* Timer interrupt service routine. Program jumps here at every 10 ms */

void interrupt send(void) {             

    unsigned int DAC_Value;
   
TMR0L = 28;             // Reload timer register       
INTCONbits.TMR0IF = 0;  // Clear timer interrupt ?ag 
   
// Generate the Sawtooth waveform //        
DAC_Value = Sample * 4095;         
DAC(DAC_Value); // Send to DAC converter       
   
Sample = Sample + 0.1;              // Next sample         
if(Sample > 1.0) Sample = 0.0; 
}


int main(void)
{
    
     ChipSelect = 1;                        
     // Disable DAC      
     Chip_Select_Direction = 0;             
     // Set CS as output      
     
     OpenSPI(SPI_FOSC_4, MODE_00, SMPMID);  // Inialize SPI module 
   
     T0CON = 0xC2;  // TIMER0 in 8-bit mode     1 -> enable 1 -> 8 bit mode 00.... 0000 divido per 8 con prescaler    
     TMR0L = 28;    // Load Timer register      1011000
     INTCON = 0xA0; // Enable global and Timer0 interrupts 
     while(1)                                               // Wait for interrupts     
     {      
     
     }
   
   }

 