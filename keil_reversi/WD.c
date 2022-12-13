#include <LPC210x.H>                       /* LPC210x definitions */
#include "WD.h"
#include "funciones_swi.h"

void WT_init(int sec){ 
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
    uint32_t leidoIRQ = read_IRQ_bit();
    uint32_t leidoFIQ = read_FIQ_bit();
    disable_irq_fiq();  //Deshabilitamos las interrupciones
    WDFEED = 0xAA;						   
    WDFEED = 0x55;
    if (leidoIRQ == 0 && leidoFIQ == 0){
        enable_irq_fiq();
    } else if (leidoIRQ == 0){
        enable_irq();
    }
}
