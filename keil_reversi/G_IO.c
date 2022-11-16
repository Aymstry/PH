#include "G_IO.h"
#include "gpio.h"
#include "msg.h"
#include "cola_asyn.h"

int jugadorActual(){
  return 1;  
}

int leercolumna(void){
    uint8_t leido = GPIO_leer(3, 7); 
    uint8_t aux = leido; 
    uint8_t unos = 0; 
    unsigned int i = 0;
    for (i = 1; i <= 7 && (unos <= 1); i++){        // if leido % 2 == 0 es valido -> (leido)AND(modulo-1) 
        if ((aux & 1) == 1 ){
            unos++; 
        } 
        aux = aux >> 1; 
    }
    if (unos > 1) return -1;
    
    i = 1;
    unos = 0; 
    while (i <= 7 && (unos < 1)){        // if leido % 2 == 0 es valido -> (leido)AND(modulo-1) 
        i++; 
        if ((leido & 1) == 1 ){
            unos++; 
        } 
        leido = leido >> 1; 
    }
    return i-1; 
}

uint8_t cambioColor(uint8_t colour){
    uint8_t nuevoJugador = C4_alternar_color(colour);

    if(nuevoJugador == 1){
        GPIO_escribir(1, 2 ,1);
    } else {
        GPIO_escribir(1, 2 ,2);
    }
    return nuevoJugador;
}

void actualizarJugada(CELDA cuadricula[TAM_FILS][TAM_COLS],uint8_t row, uint8_t column,uint8_t colour){
    C4_actualizar_tablero(cuadricula,row,column,colour);
    // codificamos el mensaje para que suene una alarma en dos segundos
    // ID 7 = JugadaRealizada        ID=7     P  23                          Decimal
    // mensaje final:               0000 0111 0 0000 0000 0000 1111 1010 000 = 117442512
    uint32_t mensaje = 117442512; 
    cola_encolar_mensaje(Set_Alarma, mensaje);
    GPIO_escribir(16,1,1);
    
}

void ApagarLedConfirmacion(void){
    GPIO_escribir(16,1,0);
}

void endgame(uint8_t resultado){
    GPIO_escribir(18,1,1);
    if( resultado == 3){        // empate
       GPIO_escribir(1, 2 ,3); 
    } else {
        if(resultado == 1){
            GPIO_escribir(1, 2 ,1);
        } else {
            GPIO_escribir(1, 2 ,2);
        }
    }
}

 void jugadaNoValidaInit(void){          
    // codificamos el mensaje para que suene una alarma 10 veces por segundo = cada 100 ms 
    // ID 8 =  JugadaNoValida        ID=8     P  23                          Decimal 1100 100
    // mensaje final:               0000 1000 1 0000 0000 0000 0000 1100 100 = 142606436
    uint32_t mensaje = 142606436;
    cola_encolar_mensaje(Set_Alarma, mensaje); 
 } 

void actualizarAviso(CELDA cuadricula[TAM_FILS][TAM_COLS]){
    // leer columna 
    uint8_t columnAux = leercolumna(); 
    // calcular fila
    uint8_t rowAux = C4_calcular_fila(cuadricula, columnAux);
        if(C4_fila_valida(rowAux) && C4_columna_valida(columnAux)) {
            // es valida 
            GPIO_escribir(17, 1 ,0);
        } else {
            // no es valida 
            GPIO_escribir(17, 1 ,1);
        }
}

void initgame(void){
    GPIO_iniciar();
    // recuerda que en las entradas de los botones pin a 0 es activo y a 1 desactivado 
    // escribimos por ello las posiciones de los botones para que no salte una interrupción 
    GPIO_escribir(14,2,3);
    // marcamos las entradas de los botones y las columnas
    GPIO_marcar_entrada(14, 2);
    GPIO_marcar_entrada(3, 7);

    cambioColor(2);
}
