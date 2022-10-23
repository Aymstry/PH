//Tipo Abstracto de Datos. Encapsula el formato de celda

/* guarda para evitar inclusiones múltiples (include guard) */
#ifndef CELDA_H
#define CELDA_H

#include <inttypes.h>

/* Cada celda se codifica en 8 bits. Sólo se usan los 3 menos significativos
 * bit 0: 1: ficha blanca; 0 : no hay ficha blanca
 * bit 1: 1: ficha negra; 0 : no hay ficha negra
 * bit 2: 0: posición vacía; 1: ocupada 
 */

typedef uint8_t CELDA;

/* *****************************************************************************
 * modifica el valor almacenado en la celda indicada 
 Valores válidos:
 1: ficha blanca
 2: ficha negra
 */
 
__inline static void
celda_poner_valor(CELDA *celdaptr, uint8_t val){
    if ((val == 1/*blanca*/)||(val == 2 /*negra*/)){
      *celdaptr = 0x04 /*ocupado*/ + val;
    }
}

/* *****************************************************************************
 * Devuelve 1 si la celda está vacía
*/
__inline static uint8_t
celda_vacia(CELDA celda){
  return (celda & 0x4) == 0;
}

/* *****************************************************************************
 * Devuelve color celda y si vacia 0
*/
__inline static uint8_t
celda_color(CELDA celda){
  return (celda & 0x03);
}
/* *****************************************************************************
 * Devuelve 1 si la celda es blanca y valida
*/
__inline static uint8_t
celda_blanca(CELDA celda){
	return celda & 0x05;
}

/* *****************************************************************************
 * Devuelve 1 si la celda es negra y valida
*/
__inline static uint8_t
celda_negra(CELDA celda){
	return celda & 0x06;
}
#endif // CELDA_H
