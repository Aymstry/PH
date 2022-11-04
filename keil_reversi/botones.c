#include <LPC210X.H>                            // LPC21XX Peripheral Registers
#include "botones.h"

// variable para comprobar que se hacen las interrupciones que deberan hacerse
static volatile int eint1_cuenta = 0;
static volatile int eint2_cuenta = 0;
// variable que se activa al detectar una nueva pulsacion
static volatile int eint1_nueva_pulsacion = 0;
static volatile int eint2_nueva_pulsacion = 0;


void eint1_ISR (void) __irq {
	eint1_cuenta++;
	VICIntEnClr = 0x00008000;    //deshabilitamos eint1 
    cola_encolar_evento(BotonPulsado, eint1_cuenta, 1);              
	VICVectAddr = 0;             // Acknowledge Interrupt
	eint1_nueva_pulsacion = 1;
}

void eint2_ISR (void) __irq {
	eint2_cuenta++;
	VICIntEnClr = 0x00010000;    //deshabilitamos eint2
	//EXTINT =  EXTINT | 4;        // clear interrupt flag
    cola_encolar_evento(BotonPulsado, eint2_cuenta, 2);        
	VICVectAddr = 0;             // Acknowledge Interrupt
	eint2_nueva_pulsacion = 1;
}

void comprobar_eint1(void){
    if((eint1_clear_nueva_pulsacion == 1) && ){   // si el boton no esta pulsado reiniciamos, si esta pulsado no hacemos nada
        EXTINT =  EXTINT | 2;        // clear interrupt flag 
        VICVectCntl2 = 0x20 | 15;                   
        VICIntEnable = VICIntEnable | 0x00008000;   // Activamos la interrupcion de eint1
        eint1_clear_nueva_pulsacion();
    }
}

// funciones para inicializar el marcador de pulsacion 
void eint1_clear_nueva_pulsacion(void){
	eint1_nueva_pulsacion = 0;
};
void eint2_clear_nueva_pulsacion(void){
	eint1_nueva_pulsacion = 0;
};

// funciones para saber si la pulsación ya estaba activa
unsigned int eint1_read_nueva_pulsacion(void){
	return eint1_nueva_pulsacion;
};

unsigned int eint2_read_nueva_pulsacion(void){
	return eint2_nueva_pulsacion;
};

// funciones para saber cauntas veces se ha pulsado el boton
unsigned int eint1_read_cuenta(void){
	return eint1_cuenta;
};

unsigned int eint2_read_cuenta(void){
	return eint2_cuenta; 
};

// funcion para inicializar los pulsadores
void eint_init (void) {
    // Inicializacion variables
	eint1_nueva_pulsacion = 0;
	eint2_nueva_pulsacion = 0;
    eint1_cuenta = 0;
	eint2_cuenta = 0;

    // Inicializamos el boton eint1
    VICVectAddr2 = (unsigned long)eint1_ISR;    // set interrupt vector in 0
	PINSEL0 = PINSEL0 & 0xCfffffff;	            // ponemos a cero los valores que vamos a modificar
    PINSEL0 = PINSEL0 | 536870912;			    // Activamos el eint1 con los bits 28:29 0010 0000 0000 0000 0000 0000 0000 0000 
    EXTINT =  EXTINT | 2;                       // Limpias la interrupcion de eint1 (p.21)   
	VICVectCntl2 = 0x20 | 15;                   
    VICIntEnable = VICIntEnable | 0x00008000;   // Activamos la interrupcion de eint1

    // Inicialización eint2
    VICVectAddr3 = (unsigned long)eint2_ISR;    // set interrupt vector in 0
    PINSEL0 = PINSEL0 & 0x3fffffff;	            // ponemos a cero los valores que vamos a modificar
    PINSEL0 = PINSEL0 | 2147483648;			    // Activamos el eint2 con los bits 31:30 1000 0000 0000 0000 0000 0000 0000 0000 
    EXTINT =  EXTINT | 4;                       // Limpias la interrupcion de eint2 (p.21)   
	VICVectCntl3 = 0x20 | 16;                   
    VICIntEnable = VICIntEnable | 0x00010000;   // Activamos la interrupcion de eint2

}
