                  
#include <LPC210x.H>                       /* LPC210x definitions */
#include "timer.h"
#include "planificador.h"
#include "cola_asyn.h"
#include "botones.h"
#include "G_IO.h"
#include "funciones_swi.h"

/*
int main(){	
  initgame(); 
  eint_init();
	temporizador_iniciar();
  planificador(); 
}
*/

int main(){
  disable_irq(); 
  uint32_t leido = read_IRQ_bit();
	if(leido == 1){
		while(1);
	}
	while(1);
  enable_irq();
  read_IRQ_bit(); 
  enable_irq_fiq();
  read_FIQ_bit();
  disable_irq_fiq();
  read_FIQ_bit();
  read_IRQ_bit();
}

/*  
  -- MAIN PRUEBAS -- 
	// unsigned int j;
	//uint8_t leido;
	 
	//GPIO_iniciar();
	temporizador_iniciar();
	eint_init();
	planificador();
  GPIO_marcar_entrada(12, 4);
  GPIO_marcar_entrada(16, 4);
  GPIO_marcar_salida(12, 8);
  GPIO_escribir(8, 4, 15);
  leido = GPIO_leer(8,4);
  if(leido == 15) while(1);*/

   /* 
   		for (j = 0x010000; j < 0x800000; j <<= 1) {  Blink LED 0,1,2,3,4,5,6 
      // Nota la gesti?n del GPIO vosotros la debe?s hacer en GPIO.c no en el main o en el reversi
      IOSET = j;                               // Turn on LED /
      //IOCLR = j;                               // Turn off LED 
    }
		
   
    temporizador_iniciar();
    temporizador_empezar();
    temporizador_reloj(10); 
    //  0000 0000 0 0001 0011 1000 1000 (5000)
    cola_encolar_mensaje(Set_Alarma, 5);
    planificador();
    
    while(1){}
    // temporizador_parar();*/


    /* //MAIN PRACTICA 1
    #include "conecta4_2022.h"
    #include "pruebas.h"
    #include "entrada.h"

    // MAIN 
    int main (void) {
      
      // comente la linea que desea que no se ejecute 

      //jugar
      conecta4_jugar();
      // realización de las pruebas 
      conecta4_jugar_test();
      while(1); //no hay S.O., no se retorna
    }
    */
