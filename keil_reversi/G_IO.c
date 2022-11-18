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
    // mensaje final:               0000 0111  0000 0000 0000 0111 1101 0000 = 117442512
    //                                0   7      0    0    0     7   D    0     
    uint32_t mensaje = 0x070007D0; 
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
    // mensaje final:               0000 1000 1000 0000 0000 0000 0110 0100 = 142606436 
    //                               0    8     8    0    0    0    6    4   = 08800064
    uint32_t mensaje = 0x08800064;
    cola_encolar_mensaje(Set_Alarma, mensaje); 
 } 

bool actualizarAviso(CELDA cuadricula[TAM_FILS][TAM_COLS]){
    static uint8_t antiguoLeido = 0; 
    bool permiso = false;
    // leer columna 
    uint8_t columnAux = leercolumna(); 
    if (antiguoLeido != columnAux){
        cola_encolar_evento(Suspender, 0, 0); // reprogramar alarma con cambios en la gpio
        antiguoLeido = columnAux; 
        permiso = terminarLatido();
    }
    // calcular fila
    uint8_t rowAux = C4_calcular_fila(cuadricula, columnAux);
        if(C4_fila_valida(rowAux) && C4_columna_valida(columnAux)) {
            // es valida 
            GPIO_escribir(17, 1 ,0);
        } else {
            // no es valida 
            GPIO_escribir(17, 1 ,1);
        }
    return permiso;
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

void empezarLatido(void){
    // codificamos el mensaje para que suene una alarma cada 250 ms 
    // ID 9 =  MIdle                ID=9     P  23                             Hexadecimal 
    // mensaje final:               0000 1001 1 0000 0000 0000 0001 1111 010 
    //                              0000 1001 1000 0000 0000 0000 1111 1010 
    //                               0    9     8    0    0    0    F    A   = 098000FA
    uint32_t mensaje = 0x098000FA;
    cola_encolar_mensaje(Set_Alarma, mensaje); 
}

bool terminarLatido(void){
    // codificamos el mensaje para cancelar la alarma que suena 250 ms 
    // ID 9 =  MIdle                ID=9     P  23                             Hexadecimal 
    // mensaje final:               0000 1001 0 0000 0000 0000 0000 0000 000 
    //                              0000 1001 0000 0000 0000 0000 0000 0000 
    //                               0    9     0    0    0    0    0    0   = 09800000
    uint32_t mensaje = 0x09000000;
    cola_encolar_mensaje(Set_Alarma, mensaje); 
    GPIO_escribir(31,1,0);
    return true; 
}

void parpadeoBlinBlin(void){
    static int estado = 0; 
    if (estado == 0){
        estado = 1;
        GPIO_escribir(31,1,1);
    } else{
        estado = 0;
        GPIO_escribir(31,1,0);
    }
}
