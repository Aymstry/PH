#ifndef __TIMER_H__
#define __TIMER_H__
#include <inttypes.h>
void temporizador_iniciar(void);
void temporizador_empezar(void);
uint32_t temporizador_leer(void);
uint32_t temporizador_parar(void);
void temporizador_reloj (int periodo);
//unsigned int timer0_read_int_count(void);
uint32_t __swi(0) clock_get_us(void);
uint32_t __swi(1) clock_gettime(void);

#endif
