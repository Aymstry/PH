#include "conecta4_2022.h"
#include "entrada.h"

extern uint8_t conecta4_buscar_alineamiento_arm(CELDA cuadricula[TAM_FILS][TAM_COLS], uint8_t
	fila, uint8_t columna, uint8_t color, int8_t delta_fila, int8_t
	delta_columna); 
	
int C4_verificar_4_en_linea_test(CELDA cuadricula[TAM_FILS][TAM_COLS], uint8_t fila, uint8_t columna, uint8_t color){
	// en esta funcion es donde se debe verificar que todas las optimizaciones dan el mismo resultado
	uint8_t resultado_c_c = conecta4_hay_linea_c_c(cuadricula, fila, columna, color);
	uint8_t resultado_c_arm = conecta4_hay_linea_c_arm(cuadricula, fila, columna, color);
	uint8_t resultado_arm_c = conecta4_hay_linea_arm_c(cuadricula, fila, columna, color);
	uint8_t resultado_arm_arm = conecta4_hay_linea_arm_arm(cuadricula, fila, columna, color);
	if ( resultado_c_c !=  resultado_c_arm){
		while(1);
	} else if (resultado_c_c != resultado_arm_c){
		while(1);
	} else if (resultado_c_c !=  resultado_arm_arm){
		while(1);
	}	
	return resultado_c_c;	
}

int ejecutar(uint8_t column, CELDA cuadriculass[TAM_FILS][TAM_COLS], int acabado, uint8_t colour){
	uint8_t row = C4_calcular_fila(cuadriculass, column); // returns 0 if is not in range
		if(C4_fila_valida(row) && C4_columna_valida(column)) { //comprueba si puede colocar la ficha segun la fila y la columna
			C4_actualizar_tablero(cuadriculass,row,column,colour); //actualiza el tablero
			if(C4_verificar_4_en_linea_test(cuadriculass, row, column, colour)) {
				acabado = TRUE;   //ganas la partida
			}
			if (C4_comprobar_empate(cuadriculass)){
				acabado = TRUE; //quedan en empate los dos jugadores
			}
		}
	return acabado;
}	

void conecta4_jugar_test(void){
	// new, column, padding to prevent desalinating to 8 bytes
	static volatile uint8_t entrada[8] = {0, 0, 0, 0, 0, 0, 0, 0 }; 
	
	#include "tableros.h"
	
	uint8_t colour = 1; // empiezan jugador 1 (blancas)


	int i = 0; 
	int acabado = FALSE;
	while (i<=5){
		acabado = FALSE;
		colour = 1;
		if(i==0){			// Caso empate mismo color
			acabado = ejecutar(7, cuadricula_color, acabado, colour);
		}
		else if(i==1){
			while ( acabado == FALSE){
				acabado = ejecutar(1,cuadricula_columna, acabado, colour);
				colour = C4_alternar_color(colour);			
			}		
		}
		else if(i==2){
			while ( acabado == FALSE){
				acabado = ejecutar(3, cuadricula_fila, acabado, colour);	
				colour = C4_alternar_color(colour);			
			}		
		}
		else if(i==3){
			while ( acabado == FALSE){
				acabado = ejecutar(7, cuadricula_victoria_di, acabado, colour);
			  colour = C4_alternar_color(colour);				
			}		
		}
		else if(i==4){
			while ( acabado == FALSE){
				acabado = ejecutar(3, cuadricula_victoria_dd, acabado, colour);
			  colour = C4_alternar_color(colour);				
			}		
		}
		else if(i==5){
			while ( acabado == FALSE){
				acabado = ejecutar(7, cuadricula_victoria_v, acabado, colour);
			  colour = C4_alternar_color(colour);				
			}		
		}
		i++;
		
	}
}
