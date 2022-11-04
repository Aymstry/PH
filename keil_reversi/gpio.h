#ifndef __GPIO_H__
#define __GPIO_H__

#include <inttypes.h>

void GPIO_iniciar(void);

uint8_t GPIO_leer(uint32_t bit_inicial, uint32_t num_bits);

void GPIO_escribir(uint32_t bit_inicial, uint32_t num_bits, uint32_t valor);

void GPIO_marcar_entrada(uint32_t bit_inicial, uint32_t num_bits);

void GPIO_marcar_salida(uint32_t bit_inicial,uint32_t num_bits);

#endif 
