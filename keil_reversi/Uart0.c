#include <LPC210x.H>                     /* LPC21xx definitions               */
#include "Uart0.h"
#include "eventos.h"

#define CR     0x0D  // 0000 1101

void uart0_ISR (void) __irq; 

void uart0_init(void){                  // Initialize Serial Interface       
  PINSEL0 = PINSEL0 & 0xfffAffff;	      // ponemos a cero los valores que vamos a modificar
  PINSEL0 = PINSEL0|0x00050000;         // Enable RxD1 and TxD1       
  // ------------------------------------- ?      
  U0LCR = 0x83;                         // 8 bits, no Parity, 1 Stop bit     
  U0DLL = 97;                           // 9600 Baud Rate @ 15MHz VPB Clock  
  U0LCR = 0x03;                         // DLAB = 0                          
 
  VICVectAddr5 = (unsigned long)uart0_ISR;    // set interrupt vector in 0
  VICVectCntl5 = 0x20 | 6;                   
  VICIntEnable = VICIntEnable | 0x00000040;   // Activamos la interrupcion de eint1

}

void uart0_ISR (void) __irq {    
                 
	VICVectAddr = 0;             // Acknowledge Interrupt
  if(U0LSR & 0x01){
    // si entramos es porque hemos leido un mensaje 
    // U0RBR // es el carácter que hemos leido 
  }
  if(U0RBR == 0){
    while(1);
  }
  while(1);
  //cola_encolar_evento(UART0, );
}



/* implementation of putchar (also used by printf function to output data)    */
int sendchar (int ch)  {                 /* Write character to Serial Port    */

  if (ch == '\n')  {
    while (!(U1LSR & 0x20));
    U1THR = CR;                          /* output CR */
  }
  while (!(U1LSR & 0x20));
  return (U1THR = ch);
}


int getchar (void)  {                     /* Read character from Serial Port   */

  while (!(U1LSR & 0x01));

  return (U1RBR);
}
