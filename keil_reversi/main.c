                  
#include <LPC210x.H>                       /* LPC210x definitions */
#include "timer.h"


int main(){
    temporizador_iniciar();
    temporizador_empezar();
    temporizador_reloj(10); 
    while(1){}
    // temporizador_parar();
     /* Variables para medir los timers :D 
        timer1_int_count
        timer0_int_count
        T0TCR
        T1TCR*/
    
}

//int main (void) {
//  	
//  reversi8();
//	
//}
