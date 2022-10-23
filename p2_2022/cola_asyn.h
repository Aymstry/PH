#ifndef __COLA_H__
#define __COLA_H__

#include "eventos.h"
enum {MAX = 32}; 
// Tenemos 4 tipos de eventos 
// 1- gpio (leds)
// 2- boton 
// 3- power (suspension)
// 4- tiempo (reloj)

// estructura básica para los eventos 
typedef struct{
    eventos evs[MAX];       // vector de eventos 
    size_t head;            // indice del primer elemento sin procesar  
    size_t tail;           // indice del último evento unido + 1  
    size_t n;              // numero de eventos sin procesar
    
}cola; 

void cola_encolar_eventos(uint8_t ID_evento, uint32_t veces, uint32_t auxData); 
void cola_desencolar_eventos(uint8_t ID_evento, uint32_t veces, uint32_t auxData); 

#endif