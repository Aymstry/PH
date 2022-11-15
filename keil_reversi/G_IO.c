#include "G_IO.h"
#include "gpio.h"

int jugadorActual(){
  return 1;  
}

int leercolumna(void){
    uint8_t leido = GPIO_leer(3, 7); 
    uint8_t aux = leido; 
    uint8_t unos = 0; 
    unsigned int i = 0;
    for (i = 1; i <= 7 && (unos <= 1); i++){        // if leido % 2 == 0 es valido -> (leido)AND(modulo-1) 
        if ((aux & 1) == 1 ){
            unos++; 
        } 
        aux = aux >> 1; 
    }
    if (unos > 1) return -1;
    
    i = 1;
    unos = 0; 
    while (i <= 7 && (unos < 1)){        // if leido % 2 == 0 es valido -> (leido)AND(modulo-1) 
        i++; 
        if ((leido & 1) == 1 ){
            unos++; 
        } 
        leido = leido >> 1; 
    }
    return i-1; 
}

uint8_t cambioColor(uint8_t colour){
    uint8_t nuevoJugador = C4_alternar_color(colour);

    if(nuevoJugador == 1){
        GPIO_escribir(1, 2 ,1);
    } else {
        GPIO_escribir(1, 2 ,2);
    }
    return nuevoJugador;
}
/*
void avisoError(){
    // completar 
    cola_encolar_mensaje(Set_Alarma, mensaje);
}*/

//Se programará una alarma periódica para que 10 veces por segundo se
//actualice la visualización de si la columna marcada está llena.
	
