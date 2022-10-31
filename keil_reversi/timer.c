#include <LPC210X.H>                            // LPC21XX Peripheral Registers
#include "cola_asyn.h"
#include "timer.h"

// variable para contabilizar el número de interrupciones
static volatile unsigned int timer0_int_count = 0;
static volatile unsigned int timer1_int_count = 0;
static int alarma = 0; 

void timer1_ISR (void) __irq;    // Generate Interrupt 
void timer0_ISR (void) __irq;

/* función que programa el contador t1 para que pueda ser utilizado. */
void temporizador_iniciar(void){
    // establecemos los contadores de interrupciones 
    timer0_int_count = 0;	
    timer1_int_count = 0;
    // establecemos el tiempo en ms antes de que salte la interrupción 
    T0MR0 = 999;                    // 1000 - 1 asi nos da una interrupción cada ms                    
    T0PR = 15;                      // dada la frecuencia a la que se ejecuta el proyecto 
    T0TCR = 1;                      // Activamos el contador (bit 0)
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
uint32_t temporizador_leer(void){
    int nTicks = T1PC + (T1TC * T1PR) + timer1_int_count * (T1PR * T1MR0);
    return nTicks/15;             // 15 es la frecuenia a la que se ejecuta el procesador 
}

/* Detiene el contador y devuelve el tiempo transcurrido desde temporizador_empezar */
uint32_t temporizador_parar(void){
    T1TCR = 0;                  //Desahbilitamos/paramos el contador
    return temporizador_leer(); 
}

void timer1_ISR (void) __irq {
    timer1_int_count++;
    T1IR = 1;                              // Clear interrupt flag
    VICVectAddr = 0;                      // Acknowledge Interrupt
}


void timer0_ISR (void) __irq {
    timer0_int_count++;
    if(timer0_int_count && (alarma - 1)){          // timer0 % periodo para contar el número de interrupciones
        cola_encolar_evento(timer0_int_count, 1, 0);      
    } 
    T0IR = 1;                              // Clear interrupt flag
    VICVectAddr = 0;                      // Acknowledge Interrupt  
}

// FUNCIONES PARA T0
void temporizador_reloj (int periodo){
    alarma = periodo; 
}






