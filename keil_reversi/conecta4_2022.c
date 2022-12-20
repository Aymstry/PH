#include "conecta4_2022.h"
#include "entrada.h"
#include "tableros.h"
#include "eventos.h"
#include "cola_asyn.h"
#include "G_IO.h"
#include "g_serie.h"
#include "msg.h"
#include "funciones_swi.h"
#include "RTC.h"
#include "timer.h"


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
	//uint8_t resultado = conecta4_hay_linea_c_c(cuadricula, fila, columna, color);
	//uint8_t resultado = conecta4_hay_linea_c_arm(cuadricula, fila, columna, color);
	//uint8_t resultado = conecta4_hay_linea_arm_c(cuadricula, fila, columna, color);
	uint8_t resultado = conecta4_hay_linea_arm_arm(cuadricula, fila, columna, color);
		
	return resultado;	
}

bool ganado_empate = false;

bool conecta4_ganado_empate(void){
	return ganado_empate;
}	
	
CELDA tablero[TAM_FILS][TAM_COLS];
	
void conecta4_resetear_juego(void){
	uint8_t  j, i;
	ganado_empate = false; 
	for(i = 0; i<TAM_FILS; i++){
		for(j = 0; j<TAM_COLS; j++){
			cuadricula[i][j] = tablero[i][j];
		}
	}
}

void conecta4_recuperar_tablero(void){
		uint8_t j, i;
		for(i = 0; i<TAM_FILS; i++){
			for(j = 0; j<TAM_COLS; j++){
				tablero[i][j]= cuadricula[i][j];
			}
		}
}

void C4_mostrarTablero(CELDA cuadricula[TAM_FILS][TAM_COLS]){
	char tablero[160];
	int indice = 0;
	char c;
	for(int i = 1; i < 7; i++){
		c = 48 + i;
		tablero[indice] = c;
		indice = indice +1;          
		tablero[indice] = '|';
		indice = indice +1;
		for(int x=1; x < 8; x++){
			if(cuadricula[i][x] == 0){
				tablero[indice] = ' ';
				indice = indice +1; 
				tablero[indice] = '|';
				indice = indice +1; 
			} else if(cuadricula[i][x] == 5){
				tablero[indice] = 'B';
				indice = indice +1; 
				tablero[indice] = '|';
				indice = indice +1; 
			} else if(cuadricula[i][x] == 6){
				tablero[indice] = 'N';
				indice = indice +1; 
				tablero[indice] = '|';
				indice = indice +1; 
			} else if (cuadricula[i][x] == 8){
				tablero[indice] = '*';
				indice = indice +1; 
				tablero[indice] = '|';
				indice = indice +1; 
			}
			if (x == 7){
				tablero[indice] = '\n'; 
				indice = indice +1; 
			} 
		}
	}
	for (int i=0; i <= 16; i++){
		tablero[indice] = '-';
		indice ++;
	}
	tablero[indice] = '\n'; 
	indice = indice +1;
	tablero[indice] = '-'; 
	indice = indice +1;
	for (char i=49; i < 56; i++){
		tablero[indice] = '|';
		indice ++;
		tablero[indice] = i;
		indice ++;
	}
	tablero[indice] = '|';
	indice ++;
	tablero[indice] = '\n';
	indice ++;
	tablero[indice] = '\0';
	uart0_enviar_array(tablero);

}

volatile uint8_t columna;
volatile uint8_t colorAnterior;
volatile uint8_t cancelada=2;
volatile uint8_t fila; 
volatile uint8_t colour;
volatile uint32_t TMedioMensajes;
volatile uint32_t numeroMensajesContados; 

void iniciarValores(void){
	colour = 1;
	cancelada = 1; 
	TMedioMensajes = 0;
	numeroMensajesContados=0;
}

void conecta4_jugar(uint8_t column){
	// new, column, padding to prevent desalinating to 8 bytes
	
	uint8_t row;

	row = C4_calcular_fila(cuadricula, column); 				// returns 0 if is not in range
	fila = row;
	if(C4_fila_valida(row) && C4_columna_valida(column)) {			 		//comprueba si puede colocar la ficha segun la fila y la columna
		colorAnterior = colour;
		columna = column;
		C4_columnaValida();
		G_IO_OkColumna();
		C4_actualizar_tablero(cuadricula,row,column,4); 	//actualiza el tablero
		actualizarJugada(cuadricula,row,column,4);
		cola_encolar_evento(Suspender, 0, 0); // Cuando se pulsa un boton se reprograma la alrma de power_down
		C4_mostrarTablero(cuadricula); 
		// encolamos la alarma de un segundo para permitir a los jugadores cancelar la jugada
		// codificamos el mensaje para que suene una alarma cada 1,5 s pq con 1 no da tiempo :) 
		// ID  =  CONECTA4                ID=15     P  23                             Hexadecimal 
		// mensaje final:               0000 1111 0 000 0000 0000 0101 1101 1100   
		//                              0000 1111 0000 0000 0000 0101 1101 1100 
		//                               0    F     0    0    0    5    D    C   = 0F0005DC - 0F0003E8
		uint32_t mensaje = 0x0F0003E8;
		cola_encolar_mensaje(Set_Alarma, mensaje); 	
		if ( cancelada == 1){		// se confirma la jugada por ello cambiamos de color 
			colour = C4_alternar_color(colour);
			cambioColor(colour); 
		}else if (cancelada ==0) {	
			//colour = colorAnterior;
			colour = C4_alternar_color(colorAnterior);
		} 
	} else {
		C4_columnaNoValida();
		G_IO_errorColumna();
	}
}

void conecta4_seguir(uint8_t confirmada){
	cancelada = confirmada;
	cola_encolar_evento(Suspender, 0, 0); // Cuando se pulsa un boton se reprograma la alrma de power_down
	if(confirmada == 0){		// movimiento cancelado 
		C4_cancelarMov();
		C4_actualizar_tablero(cuadricula,fila,columna,0); 	//actualiza el tablero
		actualizarJugada(cuadricula,fila,columna,0);
		cola_encolar_evento(Suspender, 0, 0); // Cuando se pulsa un boton se reprograma la alrma de power_down
		C4_mostrarTablero(cuadricula); 
	} else {
		C4_actualizar_tablero(cuadricula,fila,columna,colorAnterior); 	//actualiza el tablero
		actualizarJugada(cuadricula,fila,columna,colorAnterior);
		cola_encolar_evento(Suspender, 0, 0); // Cuando se pulsa un boton se reprograma la alrma de power_down
		C4_mostrarTablero(cuadricula); 

		if(C4_verificar_4_en_linea(cuadricula, fila, columna, colorAnterior)) {
			conecta4_leerTiempo();
			temporizador_parar();
			conecta4_mostrarTiempoMedio();
			endgame(colorAnterior);  												//ganas la partida
			colorAnterior = 2; 
			C4_acabarPorVictoria();
		} else if (C4_comprobar_empate(cuadricula)){
			conecta4_leerTiempo();
			temporizador_parar();
			conecta4_mostrarTiempoMedio();
			C4_acabarPorEmpate();
			colorAnterior = 2; 
			endgame(3);  													//quedan en empate los dos jugadores
		}

	}
}

// N=4E  E=45  W=57 ; E=45 N=4E D=44 ; C= 31-37
void conecta4_tratamientoComando(uint32_t comando){
	cola_encolar_evento(Suspender, 0, 0); // Cuando se pulsa un boton se reprograma la alrma de power_down
	if ( comando == 0x454E4400) {		 // END
		conecta4_leerTiempo();
		temporizador_parar();
		conecta4_mostrarTiempoMedio();
		cola_encolar_evento(FIN, 0, 1);
	} else if (comando == 0x4E455700){ // NEW
		conecta4_leerTiempo();
		iniciarValores();
		conecta4_resetear_juego();
		initgame();
		//buffer_inicio();
	} else if ((comando <= 0x39) && (comando >= 0x30)){ // C es un caracter numérico
		comando = comando - 48;
		conecta4_jugar(comando); 	// le pasamos la columna 
		// falta imprimir el tablero 
		if( conecta4_ganado_empate() == true){
			cola_encolar_evento(FIN, 0, 0);
        }
	}
}

void conecta4_init(void){
	char reglas[] = "Bienvenido a conecta 4 \nLas normas son las siguientes: \n para comenzar la partida escriba #NEW! \n para rendirse o acabar el juego escriba #END! \n mientras juega para introducir una ficha escriba #C!, \n siendo C un numero entre 0 y 9 \n SUERTE!\n"; 
	uart0_enviar_array(reglas);
}

void C4_columnaNoValida(void){
	char texto[]="Columna no valida\n";
	uart0_enviar_array(texto);
}

void C4_columnaValida(void){
	char texto[]="\nPulsa boton 1(GPIO14) para cancelar\n";
	uart0_enviar_array(texto);
}

void C4_cancelarMov(void){
	char texto[]="Movimiento cancelado\n";
	uart0_enviar_array(texto);
}

void conecta4_acabarPorBoton(void){
	char texto[]="Le diste al boton reiniciar,\n reiniciamos la partida\n";
	uart0_enviar_array(texto);
}

void C4_acabarPorEmpate(void){
	char texto[]="La partida ha terminado en empate!, bien jugado\n";
	uart0_enviar_array(texto);
}

void C4_acabarPorVictoria(void){
	char texto[]="La partida ha terminado en victoria!, bien jugado\n";
	uart0_enviar_array(texto);
}

void conecta4_leerTiempo(void){
	uint8_t segundos, minutos = 0;
	RTC_leer(&segundos, &minutos);
	char min[2], seg[2];
	min[0] = minutos + '0';
	min[1] = '\0';
	seg[0] = segundos + '0';
	seg[1] = '\0';
	char texto[]= "\n      El tiempo transcurrido es: ";
	uart0_enviar_array(texto);
	uart0_enviar_array(seg);
	uart0_enviar_array(" s y \0");
	uart0_enviar_array(min);
	uart0_enviar_array(" min. \n\0");

}

void conecta4_ActualizarTiempoMedio(void){
	uint32_t tiempo = clock_get_us();
	TMedioMensajes = TMedioMensajes + tiempo; 
	numeroMensajesContados++; 
}

void conecta4_mostrarTiempoMedio(void){
	//uint32_t tiempo = TMedioMensajes / numeroMensajesContados;
	char texto[]="El tiempo medio de procesamiento de los mensajes es de: \n";
	uart0_enviar_array(texto);
	char texto2[]=" microsegundos en esta partida. \n";
	uart0_enviar_array(texto2);
}
/*
void dividirEntero(uint32_t tiempo){
	int numIteraciones = 0; 
	int resto = tiempo;
	while (resto != 0) {
		resto = resto / 10;
		numIteraciones++; 
	}
	char numero[numIteraciones+1];
	resto = 0;
	int i;
	for(i = 0; i < numIteraciones; i++){
		resto = tiempo % 10; 
		tiempo = tiempo/10; 
		numero[numIteraciones-i] = resto + '0'; 
	}
	numero[i] = '\0';
	uart0_enviar_array(numero);
}*/
