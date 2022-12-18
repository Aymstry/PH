#ifndef __PLANIFICADOR_H__
#define __PLANIFICADOR_H__

#include "cola_asyn.h"
#include "msg.h"
#include "eventos.h"
#include "G_Alarm.h"
#include "timer.h"
#include "G_Boton.h"
#include "botones.h"
#include "G_Power.h"
#include "power.h"
#include "G_IO.h"
#include "Uart0.h"
#include "tableros.h"
#include "conecta4_2022.h"
#include "funciones_swi.h"
#include "g_serie.h"
#include "RTC.h"
#include <inttypes.h>
#include <stdio.h>

void planificador(void);

#endif
