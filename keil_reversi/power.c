#include <LPC210x.H>                       /* LPC210x definitions */
// Set the processor into power down state 
// The watchdog cannot wake up the processor from power down
void power_down(void)  {
    EXTWAKE = 6; // hacemos que cualquiera de los dos botones nos despierte del modo power_down
	PCON |= 0x02; 
}

// suspender procesardor pero dejamos activas las interrupciones
void idle(void)  {
	PCON |= 0x01; 
}
