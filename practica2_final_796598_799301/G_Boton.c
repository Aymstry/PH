#include "G_Boton.h"
#include "msg.h"
#include "cola_asyn.h"
// mensaje: 
//  boton pulsado = 2   |     10
//  periodica = 1 
//  retardo = 10
//                             ID        P  23                          Decimal
// mensaje final boton 1:      0000 0011 1 0000 0000 0000 0000 0001 010 = 58720266
//                             0000 0011 1000 0000 0000 0000 0000 1010 
//                              0     3   8    0    0    0     0    A    = 0380000A
// mensaje final boton 2:      0000 0100 1 0000 0000 0000 0000 0001 010 = 75497482
//                             0000 0100 1000 0000 0000 0000 0000 1010
//                              0     4    8   0     0   0    0     A   = 0480000A
void gestor_botones(uint32_t auxData){
    uint32_t mensaje;
    if(auxData == 1){
        mensaje = 0x0380000A;
    }
    else{
        mensaje = 0x0480000A;
    }
    cola_encolar_mensaje(Set_Alarma, mensaje);
}
// queremos cancelar una alarma cuyo ID ya existe 
// para ello el ID que le pasamos es el misom, y el retardo será 0 
//                               ID       P                       23       Decimal
// mensaje final boton 1:        0000 0011 0 0000 0000 0000 0000 0000 000 = 50331648
//                               0000 0011 0000 0000 0000 0000 0000 0000 
//                                 0    3   0     0    0    0    0    0   = 03000000
// mensaje final boton 2:        0000 0100 0 0000 0000 0000 0000 0000 000 = 67108864
//                               0000 0100 0000 0000 0000 0000 0000 0000
//                                0    4     0    0    0    0   0    0    = 04000000
void desactivarAlarma(uint32_t auxData){
    uint32_t mensaje;
    if(auxData == 1){
        mensaje = 0x03000000;
    }
    else{
        mensaje = 0x04000000;
    }
    cola_encolar_mensaje(Set_Alarma, mensaje);
}
