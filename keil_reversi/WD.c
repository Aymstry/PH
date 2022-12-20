#include <LPC210x.H>                       /* LPC210x definitions */
#include "WD.h"
#include "funciones_swi.h"
#include "cola_asyn.h"
#include "msg.h"

void WD_init(int sec){ 
	// first, it checks for a previous watchdog time out  
    if( WDMOD & 0x04 ) {					   /* Check for watchdog time out. El bit 2 se activa si se ha disparado el watchdog*/
		WDMOD &= ~0x04;						   /* Clear time out flag           */
    }
	// Time out: Pclk*WDTC*4 lo multiplica directamente por pclk y por 4?¿?
	// valor minimo WTC= 256; valor maximo 2^32 (si escribes algo menos que FF, se escribe FF)

    //                periodo    		  
    WDTC = sec*15000000; // Set watchdog time out value
    
    WDMOD = 0x03;                         // Enable watchdog timer (bit 0) and reset (bit 1).  
}

void WD_feed(void){
    disable_irq_fiq();  //Deshabilitamos las interrupciones
    WDFEED = 0xAA;						   
    WDFEED = 0x55;
    enable_irq_fiq();
}

 void alimentarWD(void){          
    // codificamos el mensaje para que suene una alarma 10 veces por segundo = cada 100 ms 
    // ID 16 =  WATCHDOG        ID=16     P  23                          Decimal 1100 100
    // mensaje final:               0001 0000 1 0000 0000 0000 ---
    // mensaje final:               0001 0000 1000 0000 0000 --- 
    //                               1    0     8    0    0    0    6    4   = 0x108003E8
    uint32_t mensaje = 0x108003E8;
    cola_encolar_mensaje(Set_Alarma, mensaje); 
 } 
