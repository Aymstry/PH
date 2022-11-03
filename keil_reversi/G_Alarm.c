#include <inttypes.h>
#include <stdbool.h>
#include "msg.h"

// volatile??? si que se modifica cada vez que se realiza una interrupción, 
// pero somos nosotros los que lo modificamos no la subrutima entonces no lo tenemos claro
static volatile uint8_t ID[8];                  // id de a quien le vamos a tener que devolver el mensaje. 
static volatile uint8_t periodica[8];           // 0-> no es periodica 1-> es periodica 
static volatile uint8_t activa[8];              // 0-> no esta activa la alarma 1-> esta activa
static volatile uint32_t retardo[8];
static volatile uint32_t retardo_inicial[8];    // Guardamos el valor del retardo inicial por si es periodica poder restaurarlo

static uint8_t i = 0;
/*
0-8 bits de mayor peso el mensaje a generar cuando venza la alarma que va a ser el ID de quien ha generado el evento 
9 si es alarma periódica o no
10-32 milisegundos de retardo. (23 bits en este campo)
Por ejemplo, el valor 1000 indicará un segundo. Si el
retardo es cero se cancelará la alarma. Si se reprograma un determinado
evento antes de que salte se reprogramará al nuevo valor. El gestor tendrá 8
posibles alarmas.
IIII IIII P 000 0000 0000 0000 0000 0000
   ID     P retardo
*/

void organizador_alarmas(uint32_t mensaje){

    // comprobamos que no existe una alarma del mismo evento, si es asi la reprogramamos
    uint8_t id = mensaje >> 24;    // nos quedamos con los 8 bits de mas peso(bits izq) y pone 0's a la derecha
    for(int indice = 0; indice < 8; indice++){
        if(ID[indice] == id){
            i = indice;
        }
    }
    // Comprobamos que es espacio no esta ocupado por una alarma activa
    while(activa[i] == 1 && ID[i] != id){
        i++;
    }
    // Tratamos el mensaje para poder usar los datos de manera independiente
    ID[i] = id;
    periodica[i] = (mensaje << 8) >> 31;  // la op. << pone 0's a la derecha 
    retardo_inicial[i] = mensaje >> 23;
    retardo[i] = retardo_inicial[i];
    activa[i] = 1; 

    i++; 
}

void gestor_alarmas(){
    for(int indice = 0; indice < 8; indice++){
        if(activa[indice] == 1){
            retardo[indice]--;
            if(retardo[indice] == 0){   // comprobamos si ha saltado la alarma 
                cola_encolar_mensaje(Alarma_Vencida, ID[indice]); // encolamos msg
                if(periodica[indice] == 1){ // si es periodica volvemos a establecer el valor de retardo
                    retardo[indice] = retardo_inicial[indice];
                }
                else{  // si hemos acabado y no es periodica desactivamos la alarma
                    activa[indice] = 0;
                }
                
            }
        }
    }
}