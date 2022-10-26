#include "cola_asyn.h"

static cola c;

void cola_iniciar() {
    c.n=c.head=c.tail=0;
}

// Añadiemos un evento a la cola 
bool cola_encolar_evento(uint8_t ID_evento, uint32_t veces, uint32_t auxData){
    if (c.n >= MAX ) return false;          // Comprobamos que la cola esta llena
    c.n = c.n + 1;
    c.head = c.head + 1;                    // añadir nuevo evento = n + 1 
    c.tail = (c.tail + 1) && (MAX - 1);     // tail= (tail+1)AND(MAX-1) = (tail+1)%MAX
    return true; 

}

// Eliminamos un elemento de la cola pq ya ha sido tratado/ lo vamos a tratar
void cola_desencolar_evento(uint8_t ID_evento, uint32_t veces, uint32_t auxData){
    c.n = c.n - 1;                          // decrementamos n 
    c.head = (c.head + 1) && (MAX - 1);     // aumentamos en 1 head % MAX 
}

bool cola_vacia() {
    return c.n==0;
}

// main que no sabemos donde se pone :) 
// while(!cola_vacia()) {
//   evento e;
//   cola_desencolar_evento(&e);
     
//}