#ifndef __IO_H__
#define __IO_H__

#include <inttypes.h>
#include <stdbool.h>
#include "conecta4_2022.h"

int leercolumna(void);
void cambioColor(uint8_t colour);
void actualizarJugada(CELDA cuadricula[TAM_FILS][TAM_COLS],uint8_t row, uint8_t column,uint8_t colour);
void ApagarLedConfirmacion(void);
void endgame(uint8_t resultado); 
void jugadaNoValidaInit(void);
bool actualizarAviso(CELDA cuadricula[TAM_FILS][TAM_COLS]);
void initgame(void);
void parpadeoBlinBlin(void);
bool terminarLatido(void); 
void empezarLatido(void);

#endif 

