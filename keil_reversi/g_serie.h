#ifndef G_SERIE_H
#define G_SERIE_H

#include <inttypes.h>

// estructura básica para los eventos 
typedef struct{
    char buffer[160];        // vector de eventos 
    uint8_t head;            // indice del primer elemento sin procesar  
    uint8_t tail;            // indice del último evento unido + 1  
    uint8_t n;               // numero de eventos sin procesar
    
}buffer; 
void GSERIE_procesarEntrada(int ch);

// Funciones UART0
void uart0_enviar_array(char mensaje[150]);
void uart0_continuar_envio(void);

// Funciones para el buffer circular
void buffer_inicio(void);
void buffer_encolar(char mensaje[150]);
char buffer_desencolar(void);

#endif 

