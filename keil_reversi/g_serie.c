#include "g_serie.h"
#include "cola_asyn.h"
#include "eventos.h"


volatile char palabra[4];  // tamaño = 4 para guardar el carácter \0 de fin de vector
volatile buffer llegada; 

void GSERIE_procesarEntrada(int ch){
    static int numChar=0;
    static bool error = false; 
    if (ch == '#') {    // nueva palabra
        if (error){     // teniamos una palabra inválida 
            numChar = 0;
            // apagamos error en la gpio 
            error = false;  
        }     
        numChar = numChar + 1; 
        
    } else if (ch == '!' && !error){    // fin palabra
        //Enviamos el comando al juego para que lo trate
        uint32_t comando;
        // preparamos el envio del mensaje 
        if(numChar == 2){
            comando = palabra[0]; 
        } else {
            comando = palabra[0]; 
            comando = comando << 8; 
            comando |= palabra[1]; 
            comando = comando << 8; 
            comando |= palabra[2]; 
            comando = comando << 8; 
        }
        
        numChar = 0;
        cola_encolar_mensaje(COMANDO, comando);

    } else if(numChar >=1){     // nuevo carácter 
        if (numChar > 3){
            if (!error){        // primera vez que se detecta el error
                // encendemos la gpio 
            }
            error = true;
        } else{
            palabra[numChar - 1] = ch; 
            palabra[numChar] = '\0'; 
            numChar = numChar + 1; 
        }
    }
    // funcion echo 
		char c[3]; 
		if (ch == '!'){
			c[0]= ch;
			c[1]= '\n';
			c[2]='\0';
		} else {
			c[0]= ch;
			c[1]='\0';
		}
  
    
    GSERIE_enviar_array(c);
}

void GSERIE_enviar_array( char mensaje[] ){
    //buffer
    buffer_encolar(mensaje);
    // imprimimos el primer carácter 
    GSERIE_continuar_envio();
}

void GSERIE_continuar_envio(void){
    char c = buffer_desencolar(); 
    // encolamos el mensaje para que la UART0 escriba el carácter. 
    if(c != '\0'){
        uint32_t mensaje = c; 
        cola_encolar_mensaje(GSERIE_IMPRIMIR, mensaje);
    } else {
			for(int i = 0; i < 160; i++){
				llegada.buffer[i] = '0';
			}
		}
}

void buffer_inicio(void) {
    llegada.n=llegada.head=llegada.tail=0;
}

// Añadiemos un evento a la cola 
void buffer_encolar(char mensaje[]){
    if (llegada.n >= MAX_A ) {// Comprobamos que la cola esta llena
        while(1);
    }  
    int indice = 0;
    while(mensaje[indice] != '\0'){
        if ( (llegada.buffer[llegada.tail-1] == '\0') && (llegada.n != 0)){
						llegada.buffer[llegada.tail-1] = mensaje[indice];
        } else{
					llegada.buffer[llegada.tail] = mensaje[indice];
					llegada.n++;
					llegada.tail = (llegada.tail + 1) % (MAX_A);
				}      
        indice++;
    }
    llegada.buffer[llegada.tail] = '\0';
    llegada.n++;
    llegada.tail = (llegada.tail + 1) % (MAX_A);      // tail= (tail)AND(MAX-1) = (tail+1)%MAX
  
}

// Eliminamos un elemento de la cola pq ya ha sido tratado/ lo vamos a tratar
char buffer_desencolar(void){
    char c; 
    c = llegada.buffer[llegada.head];
    llegada.n = llegada.n - 1;                          // decrementamos n 
    llegada.head = (llegada.head + 1) % (MAX_A );  // aumentamos en 1 head % MAX 
    
    return c; 
}

