#ifndef G_SERIE_H
#define G_SERIE_H

#include <inttypes.h>
#include <stdbool.h>
enum {MAX_A = 400}; 
// estructura básica para los eventos 
typedef struct{
    char buffer[MAX_A];        // vector tamano del buffer 
    uint32_t head;            // indice del primer elemento sin procesar  
    uint32_t tail;            // indice del último evento unido + 1  
    uint32_t n;               // numero de eventos sin procesar
    
}buffer; 
void GSERIE_procesarEntrada(int ch);

// Funciones UART0
void uart0_enviar_array(char mensaje[MAX_A]);
void uart0_continuar_envio(void);

// Funciones para el buffer circular
void buffer_inicio(void);
void buffer_encolar(char mensaje[MAX_A]);
char buffer_desencolar(void);

#endif 

