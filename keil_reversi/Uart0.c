#include <LPC210x.H>                     /* LPC21xx definitions               */
#include "Uart0.h"
#include "eventos.h"
#include "cola_asyn.h"

#define CR     0x0D  // 0000 1101

void uart0_ISR (void) __irq; 

void uart0_init(void){                  // Initialize Serial Interface       
  PINSEL0 = PINSEL0 & 0xfffffffA;	      // ponemos a cero los valores que vamos a modificar
  PINSEL0 = PINSEL0|0x00000005;         // Enable RxD1 and TxD1  
  //PINSEL0 =  0x00000005;     
  // ------------------------------------- ?      
  U0LCR = 0x83;                         // 8 bits, no Parity, 1 Stop bit     
  U0DLL = 97;                           // 9600 Baud Rate @ 15MHz VPB Clock  
  U0LCR = 0x03;                         // DLAB = 0                          
  U0IER = 0x03;                         // activamos las interrupciones de lectura y escritura 
  VICVectAddr5 = (unsigned long)uart0_ISR;    // set interrupt vector in 0
  VICVectCntl5 = 0x20 | 6;                   
  VICIntEnable = VICIntEnable | 0x00000040;   // Activamos la interrupcion de eint1

}

// Nota THRE son las interrupciones de transmisión 
//      RBR (recive data)             de recepción 
// (U0LSR & 0x01) esto nos indica que hay un caracter nuevo, no hace falta comprobarlo porque ya no es una espera activa

void uart0_ISR (void) __irq {    
                 
  // U0IIR nos va a indicar que tipo de interrupción tenemos 
  if ((U0IIR & 0x4)&&(U0LSR & 0x01)){                    // recibimos una interrupcion de recepción  
    sendchar(U0RBR);
    cola_encolar_evento(UART0,0, U0RBR);   // U0RBR = carácter leido por la UART
  }
  
  if (U0IIR & 0x1) {           // recibimos una interrupcion de transmisión  
    while(1);
    
  }
		VICVectAddr = 0;                       // Acknowledge Interrupt

}



/* implementation of putchar (also used by printf function to output data)    */
int sendchar (int ch)  {                 /* Write character to Serial Port    */
  if (ch == '\n')  {
    U0THR = CR;                          /* output CR */
  }
  return (U0THR = ch);                  // U0THR es el registro en el que debemos almacenar el valor que queremos escribir
}

