#include "G_Boton.h"
#include "msg.h"
#include "cola_asyn.h"
// mensaje: 
//  boton pulsado = 2   |     10
//  periodica = 1 
//  retardo = 10
//                              32                              P ID          Decimal
// mensaje final boton 1:        0000 0000 0000 0000 0001 010   1 0000 0011 = 5379
// mensaje final boton 2:        0000 0000 0000 0000 0001 010   1 0000 0100 = 5380
void gestor_botones(uint32_t auxData){
    uint32_t mensaje;
    if(auxData == 1){
        mensaje = 5379;
    }
    else{
        mensaje = 5380;
    }
    cola_encolar_mensaje(Set_Alarma, mensaje);
}