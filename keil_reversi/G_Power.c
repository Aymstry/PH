#include "msg.h"
#include "cola_asyn.h"
// ID 2 = sleep                ID=2       P  23                          Decimal
// mensaje final boton 1:      0000 0010 0 0000 0000 0000 0000 0110 010 = 33554482
void introducir_power(void){
    uint32_t mensaje = 33554482;
    cola_encolar_mensaje(Set_Alarma, mensaje);
}

void dormir(void){
    power_down();
}
