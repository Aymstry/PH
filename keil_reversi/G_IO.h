#ifndef __IO_H__
#define __IO_H__

#include <inttypes.h>
#include "conecta4_2022.h"

int leercolumna(void);
uint8_t cambioColor(uint8_t colour);
void actualizarJugada(CELDA cuadricula[TAM_FILS][TAM_COLS],uint8_t row, uint8_t column,uint8_t colour);
void ApagarLedConfirmacion(void);
void endgame(uint8_t resultado); 
void jugadaNoValidaInit(void);
void actualizarAviso(CELDA cuadricula[TAM_FILS][TAM_COLS]);
void initgame(void);

#endif 

