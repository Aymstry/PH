#ifndef TABLEROS_H
#define TABLEROS_H

//tableros en memoria a mitad de una partida, toca jugar blancas
#include "celda.h"
static CELDA
cuadricula_1[7][8] =
/*{
0, 0XC1, 0XC2, 0XC3, 0XC4, 0XC5, 0Xc6, 0XC7,
0XF1, 6, 5, 6, 5, 6, 6, 0,
0XF2, 5, 6, 5, 5, 0, 5, 0,
0XF3, 6, 0, 0, 6, 0, 5, 0,
0XF4, 5, 0, 0, 6, 0, 5, 0,
0XF5, 6, 0, 0, 5, 0, 0, 0,
0XF6, 0, 0, 0, 6, 0, 0, 0};*/
{
0, 0XC1, 0XC2, 0XC3, 0XC4, 0XC5, 0Xc6, 0XC7,
0XF1, 6, 5, 6, 5, 6, 6, 5,
0XF2, 5, 6, 5, 5, 6, 5, 6,
0XF3, 6, 6, 5, 6, 6, 5, 5,
0XF4, 5, 5, 5, 6, 5, 5, 6,
0XF5, 6, 6, 6, 5, 6, 6, 5,
0XF6, 6, 5, 5, 6, 6, 5, 0};

// se pueden definir otros tableros para comprobar casos

//static CELDA
//cuadricula_2[7][8] =
//{
//0, 0XC1, 0XC2, 0XC3, 0XC4, 0XC5, 0Xc6, 0XC7,
//0XF1, 0, 0, 0, 0, 0, 0, 0,
//0XF2, 0, 0, 0, 0, 0, 0, 0,
//0XF3, 0, 0, 0, 0, 0, 0, 0,
//0XF4, 0, 0, 0, 0, 0, 0, 0,
//0XF5, 0, 0, 0, 0, 0, 0, 0,
//0XF6, 0, 0, 0, 0, 0, 0, 0};

#endif
