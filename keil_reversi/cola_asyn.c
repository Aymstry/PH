#include "cola_asyn.h"
#include "gpio.h"
#include "funciones_swi.h"

static cola c;
static cola msg;

void cola_iniciar(void) {
    c.n=c.head=c.tail=0;
}

// Añadiemos un evento a la cola 
void cola_encolar_evento(uint8_t ID_evento, uint32_t veces, uint32_t auxData){
    uint32_t leidoFIQ = read_FIQ_bit();
    if (leidoFIQ == 0){
        disable_fiq(); 
    }
    if (c.n >= MAX_C ) {// Comprobamos que la cola esta llena
        GPIO_escribir(30,1,1);
        while(1);
    }          
    c.evs[c.tail].auxData = auxData;
    c.evs[c.tail].id = ID_evento;
    c.evs[c.tail].numInt = veces;
    c.n = c.n + 1; 
    c.tail = (c.tail + 1) % (MAX_C);     // tail= (tail+1)AND(MAX-1) = (tail+1)%MAX
    if (leidoFIQ == 0){
        enable_fiq(); 
    } 
}

// Añadiemos un evento a la cola 
void cola_encolar_FIQ(uint8_t ID_evento, uint32_t veces, uint32_t auxData){
    if (c.n >= MAX_C ) {// Comprobamos que la cola esta llena
        GPIO_escribir(30,1,1);
        while(1);
    }          
    c.evs[c.tail].auxData = auxData;
    c.evs[c.tail].id = ID_evento;
    c.evs[c.tail].numInt = veces;
    c.n = c.n + 1; 
    c.tail = (c.tail + 1) % (MAX_C);     // tail= (tail+1)AND(MAX-1) = (tail+1)%MAX
}

// Eliminamos un elemento de la cola pq ya ha sido tratado/ lo vamos a tratar
void cola_desencolar_evento(uint8_t *ID_evento, uint32_t *auxData){
    *ID_evento = (c.evs[c.head]).id;
    *auxData = (c.evs[c.head]).auxData;
    c.n = c.n - 1;                          // decrementamos n 
    c.head = (c.head + 1) % (MAX_C);     // aumentamos en 1 head % MAX 
}

bool cola_vacia(void) {
    return c.n==0;
}

// Añadiemos un mensaje a la cola de mensajes 
void cola_encolar_mensaje(uint8_t ID_msg, uint32_t mensaje){
    if (msg.n >= MAX_C ) {// Comprobamos que la cola esta llena
        while(1);
    }          
    msg.evs[msg.tail].id = ID_msg;
    msg.evs[msg.tail].auxData = mensaje;
    msg.n = msg.n + 1; 
    msg.tail = (msg.tail + 1) % (MAX_C);     // tail= (tail+1)AND(MAX-1) = (tail+1)%MAX
}

// Eliminamos un elemento de la cola pq ya ha sido tratado/ lo vamos a tratar
void cola_desencolar_mensaje(uint8_t *ID_msg, uint32_t *mensaje){
    *ID_msg = (msg.evs[msg.head]).id;
    *mensaje = (msg.evs[msg.head]).auxData;
    msg.n = msg.n - 1;                          // decrementamos n 
    msg.head = (msg.head + 1) % (MAX_C);     // aumentamos en 1 head % MAX 
}

bool cola_vacia_mensaje(void) {
    return (msg.n == 0);
}


// main que no sabemos donde se pone :) ahor lo sabemos, se pone en el planificador 
// while(!cola_vacia()) {
//   evento e;
//   cola_desencolar_evento(&e);
     
//}
