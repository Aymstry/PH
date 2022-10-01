#include "conecta4_2022.h"
#include "entrada.h"
extern uint8_t conecta4_buscar_alineamiento_arm(CELDA cuadricula[TAM_FILS][TAM_COLS], uint8_t
	fila, uint8_t columna, uint8_t color, int8_t delta_fila, int8_t
	delta_columna);

// devuelve ERROR en caso de no encontrar fila
uint8_t C4_calcular_fila(CELDA cuadricula[TAM_FILS][TAM_COLS], uint8_t columna){
	uint8_t fila = 1; // row 0 is used to enumerate the columns

	if ((columna < 1 ) || (columna >  NUM_COLUMNAS)) {
		return ERROR;
	} // Out of the board. Columna must go form 1 to 7

	while((fila <= NUM_FILAS)
		&& (celda_vacia(cuadricula[fila][columna]) == FALSE)){
	    fila++;
	}
	return fila <= NUM_FILAS ? fila : ERROR;
};

// devuelve la longitud de la línea más larga
uint8_t conecta4_buscar_alineamiento_c(CELDA cuadricula[TAM_FILS][TAM_COLS], uint8_t
	fila, uint8_t columna, uint8_t color, int8_t delta_fila, int8_t
	delta_columna)
{
    // avanzar hasta que celda esté vacía, sea distinto color o lleguemos al borde
    if (!C4_fila_valida(fila) || ! C4_columna_valida(columna)) {
			return 0;
    }

    // posicion valida y mismo color
    if(celda_vacia(cuadricula[fila][columna]) || (celda_color(cuadricula[fila][columna]) != color)) { 
				return 0;
    }				

    // avanzar índices
    uint8_t nueva_fila = fila + delta_fila;
    uint8_t nueva_columna = columna + delta_columna;

    // incrementar longitud y visitar celda vecina
    return 1 + conecta4_buscar_alineamiento_c(cuadricula, nueva_fila, nueva_columna, color, delta_fila, delta_columna);
}

// devuelve true si encuentra una línea de longitud mayor o igual a 4
//Dice si se ha ganado la partida
uint8_t conecta4_hay_linea_c_c(CELDA cuadricula[TAM_FILS][TAM_COLS], uint8_t fila, uint8_t columna, uint8_t color)
{
   int8_t deltas_fila[4] = {0, -1, -1, 1};
   int8_t deltas_columna[4] = {-1, 0, -1, -1};
   unsigned int i = 0;
   uint8_t linea = FALSE;
   uint8_t long_linea = 0;

   // buscar linea en fila, columna y 2 diagonales
   for(i=0; (i < 4) && (linea == FALSE); ++i) {
       // buscar sentido
       long_linea = conecta4_buscar_alineamiento_c(cuadricula, fila, columna, color, deltas_fila[i], deltas_columna[i]);
       linea = long_linea >= 4;
       if (linea) {
         continue;
       }
       // buscar sentido inverso
       long_linea += conecta4_buscar_alineamiento_c(cuadricula, fila-deltas_fila[i],
	       columna-deltas_columna[i], color, -deltas_fila[i], -deltas_columna[i]);
       linea = long_linea >= 4;
   }
   return linea;
}

//funcion en c que invoca a la función alineamiento en arm
uint8_t conecta4_hay_linea_c_arm(CELDA cuadricula[TAM_FILS][TAM_COLS], uint8_t fila, uint8_t columna, uint8_t color)
{
   int8_t deltas_fila[4] = {0, -1, -1, 1};
   int8_t deltas_columna[4] = {-1, 0, -1, -1};
   unsigned int i = 0;
   uint8_t linea = FALSE;
   uint8_t long_linea = 0;

   // buscar linea en fila, columna y 2 diagonales
   for(i=0; (i < 4) && (linea == FALSE); ++i) {
       // buscar sentido
       long_linea = conecta4_buscar_alineamiento_arm(cuadricula, fila, columna, color, deltas_fila[i], deltas_columna[i]);
       linea = long_linea >= 4;
       if (linea) {
         continue;
       }
       // buscar sentido inverso
       long_linea += conecta4_buscar_alineamiento_arm(cuadricula, fila-deltas_fila[i],
	       columna-deltas_columna[i], color, -deltas_fila[i], -deltas_columna[i]);
       linea = long_linea >= 4;
   }
   return linea;
}

void C4_actualizar_tablero(CELDA cuadricula[TAM_FILS][TAM_COLS], uint8_t fila,
	uint8_t columna, uint8_t val)
{
    celda_poner_valor(&cuadricula[(fila)][(columna)], val);
}

int C4_comprobar_empate(CELDA cuadricula[TAM_FILS][TAM_COLS]){
//TO DO comprobar si esta jugada llena todo el tablero y hay empate
	unsigned int i = 0;
	for (i=0; i<=NUM_COLUMNAS; ++i){
		if(celda_vacia(cuadricula[NUM_FILAS][i])){
			return(0); //no hay empate
		}
	}
	
	return(1);//hay empate
}

int C4_verificar_4_en_linea(CELDA cuadricula[TAM_FILS][TAM_COLS], uint8_t fila, uint8_t columna, uint8_t color){
	// en esta funcion es donde se debe verificar que todas las optimizaciones dan el mismo resultado
	//uint8_t resultado_c_c = conecta4_hay_linea_c_c(cuadricula, fila, columna, color);
	//uint8_t resultado_c_c = conecta4_hay_linea_c_arm(cuadricula, fila, columna, color);
	uint8_t resultado_c_c = conecta4_hay_linea_arm_c(cuadricula, fila, columna, color);
	return resultado_c_c;
}

	
void conecta4_jugar(void){
	// new, column, padding to prevent desalinating to 8 bytes
	static volatile uint8_t entrada[8] = {0, 0, 0, 0, 0, 0, 0, 0 }; 

	#include "tableros.h"
	
	uint8_t column, row, colour;
	colour = 1; // empiezan jugador 1 (blancas)
	
	while (1){
		while (entrada_nueva(entrada) == 0){};
		column = entrada_leer (entrada); //coge de memoria la columna donde indicas que quieres añadir la ficha
		row = C4_calcular_fila(cuadricula_1, column); // returns 0 if is not in range
		if(C4_fila_valida(row) && C4_columna_valida(column)) { //comprueba si puede colocar la ficha segun la fila y la columna
			C4_actualizar_tablero(cuadricula_1,row,column,colour); //actualiza el tablero
			if(C4_verificar_4_en_linea(cuadricula_1, row, column, colour)) {
				while(1); //ganas la partida
			}
			if (C4_comprobar_empate(cuadricula_1)){
				while(1); //quedan en empate los dos jugadores
			}
			colour = C4_alternar_color(colour);		//cambia el color de la ficha para que se vayan intercambiando	
		}
		entrada_inicializar (entrada);
	}
}
