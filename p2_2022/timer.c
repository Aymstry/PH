#include <LPC210X.H>                            // LPC21XX Peripheral Registers
#include "timer.h"
// variable para contabilizar el número de interrupciones
static volatile unsigned int timer0_int_count = 0;
static volatile unsigned int timer1_int_count = 0;


void timer0_ISR (void) __irq;    // Generate Interrupt 

/* función que programa el contador t1 para que pueda ser utilizado. */
void temporizador_iniciar(int time0){
    // establecemos los contadores de interrupciones 
    timer0_int_count = 0;	
    timer1_int_count = 0;
    // establecemos el tiempo en ms antes de que salte la interrupción 
    T0MR0 = time0; 
    T0PR = 15000;
    T0TCR = 1;                     // Activamos el contador (bit 0)
    T0MCR = 3;                      // activa las interrupciones
                                    // cada vez que se lanza la interrupcion la resetea
    T1MR0 = 0xFFFFFFFE;             // Lo ponemos en su máximo valor para asi tener el mínimo nº de interrupciones 
    T1PR = 0xFFFFFFFE; 
    T1MCR = 3;

    // VIC T0
    VICVectAddr0 = (unsigned long)timer0_ISR; //llama a la funcion que se ejecuta cuando se realiza la interrupción
    VICVectCntl0 = 0x20 | 4;                   
    VICIntEnable = VICIntEnable | 0x00000010; 
    // VIC T1
    VICVectAddr1 = (unsigned long)timer1_ISR; 
    VICVectCntl1 = 0x20 | 5;                   
    VICIntEnable = VICIntEnable | 0x00000020; // damos permiso a la interrupcion del timer 1
    // Ahora mismo el T0 tiene mayor prioridad que el T1 
}

// FUNCIONES PARA T1 
/* función que inicia la cuenta de un contador de forma indefinida. */
void temporizador_empezar(void){
    // poner a cero el contador 
    // iniciar el contador 
    T1TCR = 2;                     // Reeseteamos el valor del timer1 (bit 1)
    T1TCR = 1;                     // Activamos el contador (bit 0)
  
}

/* función que lee el tiempo que lleva contando el contador desde la última vez que se ejecutó
temporizador_empezar y lo devuelve en microsegundos. */
uint32_t temporizador_leer(){
    return (T1MR0 * (T1TC/T1MR0) + T1PR * T1TC)/15 
}

/* Detiene el contador y devuelve el tiempo transcurrido desde temporizador_empezar */
uint32_t temporizador_parar(){
    T1TCR = 0;                  //Desahbilitamos/paramos el contador
    return (T1MR0 * (T1TC/T1MR0) + T1PR * T1TC)/15
}

void timer1_ISR (void) __irq {
    timer1_int_count++;
    T1IR = 1;                              // Clear interrupt flag
    VICVectAddr1 = 0;                      // Acknowledge Interrupt
}

// FUNCIONES PARA T0
void temporizador_reloj (int periodo){
    T0MR0 = periodo                        // Lo ponemos en su máximo valor para asi tener el mínimo nº de interrupciones 
    
     
}

/*
unsigned int timer0_read_int_count(void){
	return timer0_int_count;
};*/


// NOTAS: 
// inicializar lo llama el main para comenzar los eventos
// si periodo es 0 se para (los timers)
// CAMBIAR EL startup.S QUE TE DAN 