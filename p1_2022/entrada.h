#ifndef __ENTRADA_H__
#define __ENTRADA_H__

#include <inttypes.h>

enum {ALIGN_8 = 8};

//hay nueva jugada? (alguien ha modificado asincronamente el contenido de memoria) 
uint8_t entrada_nueva(volatile uint8_t entrada[ALIGN_8]);

//leer columna
uint8_t entrada_leer(volatile uint8_t entrada[ALIGN_8]);

//marcar que ya se ha realizado la jugada
void entrada_inicializar(volatile uint8_t entrada[ALIGN_8]);

#endif
