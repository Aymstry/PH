#include "G_Power.h"
#include "msg.h"
#include "cola_asyn.h"
// ID 2 = sleep                ID=2       P  23                          Decimal
// mensaje final boton 1:      0000 0010 0 0000 0000 0100 1110 0010 000 = 33554482 
//                             0000 0010 0000 0000 0010 0111 0001 0000
//                               0    2    0    0    2    7    1    0    =  02002710
void introducir_power(void){
    uint32_t mensaje = 0x02002710;
    cola_encolar_mensaje(Set_Alarma, mensaje);
}

void dormir(void){
    power_down();
}
