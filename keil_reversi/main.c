                  
#include <LPC210x.H>                       /* LPC210x definitions */
#include "timer.h"
#include "planificador.h"
#include "cola_asyn.h"


int main(){
    temporizador_iniciar();
    temporizador_empezar();
    temporizador_reloj(10); 
    //  0000 0000 0 0001 0011 1000 1000 (5000)
    cola_encolar_mensaje(Set_Alarma, 5);
    planificador();
    
    while(1){}
    // temporizador_parar();
    
}

//int main (void) {
//  	
//  reversi8();
//	
//}
