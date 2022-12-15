#ifndef __EVENTOS_H__
#define __EVENTOS_H__

#include <inttypes.h>
#include <stdbool.h>

// Tenemos 4 tipos de eventos 
enum {T0, T1, BotonPulsado, BP1, BP2, CancelarAlarma, Suspender, JugadaRealizada, JugadaNoValida, MIdle, UART0};

// estructura básica para los eventos 2
typedef struct elemento{
    uint32_t numInt;        // 4B = 4 casillas de memoria 
    uint32_t auxData;
    uint8_t id;             // 1B = 1 casilla de memoria se pone el último para que los campos esten alineados en memoria
}elemento; 


#endif 
