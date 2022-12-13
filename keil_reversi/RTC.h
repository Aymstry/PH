#ifndef RTC_H
#define RTC_H

void RTC_init(void); 
int RTC_read_time(void);
void RTC_leer(uint8_t *segundos, uint8_t *minutos);

#endif

