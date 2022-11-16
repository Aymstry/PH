#ifndef TABLEROS_H
#define TABLEROS_H

//tableros en memoria a mitad de una partida, toca jugar blancas
#include "celda.h"
static CELDA
cuadricula_1[7][8] =
{ 
	//Inicial
0, 0XC1, 0XC2, 0XC3, 0XC4, 0XC5, 0Xc6, 0XC7,
0XF1, 6, 5, 6, 5, 6, 6, 0,
0XF2, 5, 6, 5, 5, 0, 5, 0,
0XF3, 6, 0, 0, 6, 0, 5, 0,
0XF4, 5, 0, 0, 6, 0, 5, 0,
0XF5, 6, 0, 0, 5, 0, 0, 0,
0XF6, 0, 0, 0, 6, 0, 0, 0};
	//Empate mismo color

static CELDA
cuadricula_color[7][8] =
{
	0, 0XC1, 0XC2, 0XC3, 0XC4, 0XC5, 0Xc6, 0XC7,
0XF1, 6, 5, 6, 5, 6, 6, 6,
0XF2, 5, 6, 5, 5, 6, 5, 6,
0XF3, 6, 6, 5, 6, 6, 5, 5,
0XF4, 5, 6, 5, 6, 5, 5, 6,
0XF5, 6, 5, 6, 5, 6, 6, 5,
0XF6, 5, 5, 6, 6, 6, 5, 0};
	//Columna válida
static CELDA
cuadricula_columna[7][8] =
{
	0, 0XC1, 0XC2, 0XC3, 0XC4, 0XC5, 0Xc6, 0XC7,
0XF1, 6, 5, 6, 5, 6, 6, 6,
0XF2, 5, 6, 5, 5, 6, 5, 6,
0XF3, 6, 6, 5, 6, 6, 5, 5,
0XF4, 0, 6, 5, 6, 5, 5, 6,
0XF5, 0, 5, 6, 5, 6, 6, 5,
0XF6, 0, 5, 6, 6, 6, 5, 6};
     // Comprobamos fla válida y celda vacia (em el otro sentido) 
static CELDA
cuadricula_fila[7][8] =
{
	0, 0XC1, 0XC2, 0XC3, 0XC4, 0XC5, 0Xc6, 0XC7,
0XF1, 6, 5, 0, 5, 6, 6, 6,
0XF2, 5, 6, 0, 5, 6, 5, 6,
0XF3, 6, 6, 0, 6, 6, 5, 5,
0XF4, 6, 6, 0, 6, 5, 5, 6,
0XF5, 5, 5, 0, 5, 6, 6, 5,
0XF6, 5, 5, 0, 6, 6, 5, 6};

	// Comprobamos que el caso de victoria diagonal izquierda
static CELDA
cuadricula_victoria_di[7][8] =
{
	0, 0XC1, 0XC2, 0XC3, 0XC4, 0XC5, 0Xc6, 0XC7,
0XF1, 6, 5, 6, 5, 6, 6, 6,
0XF2, 5, 6, 5, 5, 6, 5, 0,
0XF3, 6, 6, 5, 6, 6, 5, 0,
0XF4, 5, 6, 5, 6, 5, 5, 0,
0XF5, 6, 5, 6, 5, 6, 6, 0,
0XF6, 5, 5, 6, 6, 6, 5, 0};

	// Comprobamos que el caso de victoria diagonal derecha
static CELDA
cuadricula_victoria_dd[7][8] =
{
	0, 0XC1, 0XC2, 0XC3, 0XC4, 0XC5, 0Xc6, 0XC7,
0XF1, 6, 5, 6, 5, 6, 6, 6,
0XF2, 5, 6, 0, 5, 6, 5, 0,
0XF3, 6, 6, 0, 5, 6, 5, 0,
0XF4, 5, 6, 0, 6, 5, 5, 0,
0XF5, 6, 5, 0, 5, 6, 6, 0,
0XF6, 5, 5, 0, 6, 6, 5, 0};

// Comprobamos que el caso de victoria vertical
static CELDA
cuadricula_victoria_v[7][8] =
{
	0, 0XC1, 0XC2, 0XC3, 0XC4, 0XC5, 0Xc6, 0XC7,
0XF1, 6, 5, 6, 5, 6, 6, 5,
0XF2, 5, 6, 0, 5, 6, 5, 5,
0XF3, 6, 6, 0, 5, 6, 6, 5,
0XF4, 5, 6, 0, 6, 5, 5, 0,
0XF5, 6, 5, 0, 5, 6, 6, 0,
0XF6, 5, 5, 0, 6, 6, 5, 0};

// Comprobamos que el caso de victoria vertical
static CELDA
cuadricula_victoria_j2[7][8] =
{
	0, 0XC1, 0XC2, 0XC3, 0XC4, 0XC5, 0Xc6, 0XC7,
0XF1, 6, 5, 6, 5, 6, 6, 5,
0XF2, 5, 6, 0, 5, 6, 5, 5,
0XF3, 6, 6, 0, 6, 6, 6, 6,
0XF4, 5, 6, 0, 6, 5, 5, 0,
0XF5, 6, 5, 0, 6, 6, 6, 0,
0XF6, 5, 5, 0, 6, 6, 5, 0};
#endif
