#include "entrada.h"

/* *****************************************************************************
 * macros internas acceso a la E/S 
 * modificando el contenido de la memoria desde el depurador
 */

//hay nueva jugada? (alguien ha modificado asincronamente el contenido de memoria) 
uint8_t entrada_nueva(volatile uint8_t entrada[ALIGN_8]) { return entrada[0];};

//leer columna
uint8_t entrada_leer(volatile uint8_t entrada[ALIGN_8]) { return entrada[1];};

//marcar que ya se ha realizado la jugada
void entrada_inicializar(volatile uint8_t entrada[ALIGN_8]) {
		entrada[0] = 0;
		entrada[1] = 0;
};
