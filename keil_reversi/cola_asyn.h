#ifndef __COLA_H__
#define __COLA_H__

#include "eventos.h"
const int MAX_C = 50;

// estructura básica para los eventos 
typedef struct{
    elemento evs[MAX_C];       // vector de eventos 
    uint8_t head;            // indice del primer elemento sin procesar  
    uint8_t tail;           // indice del último evento unido + 1  
    uint8_t n;              // numero de eventos sin procesar
    
}cola; 

void cola_iniciar(void); 
void cola_encolar_evento(uint8_t ID_evento, uint32_t veces, uint32_t auxData); 
void cola_desencolar_evento(uint8_t *ID_evento, uint32_t *auxData); 
bool cola_vacia(void);
void cola_encolar_mensaje(uint8_t ID_msg, uint32_t mensaje);
void cola_desencolar_mensaje(uint8_t *ID_msg, uint32_t *mensaje);
bool cola_vacia_mensaje(void);

#endif
