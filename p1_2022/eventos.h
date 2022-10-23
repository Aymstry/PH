#ifndef __EVENTOS_H__
#define __EVENTOS_H__

#include <inttypes.h>
#include <stdbool.h>

// Tenemos 4 tipos de eventos 
// 1- gpio (leds)
// 2- boton 
// 3- power (suspension)
// 4- tiempo (reloj)

// estructura básica para los eventos 
typedef struct{
    uint32_t numInt;        // 4B = 4 casillas de memoria 
    uint32_t auxData;
    uint8_t id;             // 1B = 1 casilla de memoria se pone el último para que los campos esten alineados en memoria
}eventos; 


#endif