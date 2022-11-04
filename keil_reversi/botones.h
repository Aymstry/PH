#ifndef __BOTONES_H__
#define __BOTONES_H__

#include "cola_asyn.h"

void comprobar_eint1(void);
void comprobar_eint2(void);
void eint1_clear_nueva_pulsacion(void);
void eint2_clear_nueva_pulsacion(void);
unsigned int eint1_read_nueva_pulsacion(void);
unsigned int eint2_read_nueva_pulsacion(void);
unsigned int eint1_read_cuenta(void);
unsigned int eint2_read_cuenta(void);
void eint_init (void);

#endif 

