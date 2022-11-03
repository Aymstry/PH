#ifndef __COLA_H__
#define __COLA_H__

#include "eventos.h"
enum {MAX = 32}; 

// estructura básica para los eventos 
typedef struct{
    elemento evs[MAX];       // vector de eventos 
    uint8_t head;            // indice del primer elemento sin procesar  
    uint8_t tail;           // indice del último evento unido + 1  
    uint8_t n;              // numero de eventos sin procesar
    
}cola; 

void cola_encolar_evento(uint8_t ID_evento, uint32_t veces, uint32_t auxData); 
void cola_desencolar_eventos(uint8_t ID_evento, uint32_t auxData); 

#endif
