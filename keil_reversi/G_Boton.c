#include "G_Boton.h"
#include "msg.h"
#include "cola_asyn.h"
// mensaje: 
//  boton pulsado = 2   |     10
//  periodica = 1 
//  retardo = 10
//                             ID        P  23                          Decimal
// mensaje final boton 1:      0000 0011 1 0000 0000 0000 0000 0001 010 = 58720266
// mensaje final boton 2:      0000 0100 1 0000 0000 0000 0000 0001 010 = 75497482
void gestor_botones(uint32_t auxData){
    uint32_t mensaje;
    if(auxData == 1){
        mensaje = 58720266;
    }
    else{
        mensaje = 75497482;
    }
    cola_encolar_mensaje(Set_Alarma, mensaje);
}
// queremos cancelar una alarma cuyo ID ya existe 
// para ello el ID que le pasamos es el misom, y el retardo será 0 
//                               ID       P                       23       Decimal
// mensaje final boton 1:        0000 0011 0 0000 0000 0000 0000 0000 000 = 50331648
// mensaje final boton 2:        0000 0100 0 0000 0000 0000 0000 0000 000 = 67108864
void desactivarAlarma(uint32_t auxData){
    uint32_t mensaje;
    if(auxData == 1){
        mensaje = 50331648;
    }
    else{
        mensaje = 67108864;
    }
    cola_encolar_mensaje(Set_Alarma, mensaje);
}
