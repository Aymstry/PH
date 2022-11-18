#include <LPC210x.H> 
#include "gpio.h"

enum {MAX = 32}; 

void GPIO_iniciar(void){
    IODIR = 0xFFFFFFFF;         // establecemos todos los bits de la GPIO en modo output (escritura y lectura)
    IOCLR = 0XFFFFFFFF;         // ponemos a cero todos los leds
}

/*Dados bit_inicial y el num_bits a leer, devuleve el valor leido desde bit_inicial hasta bit_inicial+num_bits
Ejemplo de ejecucion:
    valor inicial: 0000 1111 0000 1111 1010 1111 1111 0000
    Queremos los bits 12 a 15
    Desplazamiento 1: (IOPIN >> bit_inicial)
                0000 0000 0000 0000 1111 0000 1111 1010
                quedan los bits que nos interesan a la derecha
    Desplazamiento 2: ((IOPIN >> bit_inicial) << (MAX - num_bits))
                1010 0000 0000 0000 0000 0000 0000 0000	
    Desplazamiento 3: quitamos ceros 
                0000 0000 0000 0000 0000 0000 0000 1010
*/
uint8_t GPIO_leer(uint32_t bit_inicial, uint32_t num_bits){
    uint8_t leidos = ((IOPIN >> bit_inicial) << (MAX - num_bits)) >> (MAX - num_bits);
    return leidos;
}


// Igual que la anterior pero escribir valor en los num_bits
// 0000 0000 0000 0000 0001 0000 0000 0000 = 0x00001000 
// uint32_t = 1 << bit_inicial
// 0000 0000 0000 0000 1000 0000 0000 0000 = 0x00080000 bit_inicial + num_bits -1
// limpiado OR valor
// valor = 4 desplazamiento inicial para colocarlo en su sitio y poder aplicar una OR 
// 1100 0000 0000 0000 0000
void GPIO_escribir(uint32_t bit_inicial, uint32_t num_bits,uint32_t valor){
    uint32_t inicial = 1 << bit_inicial;           // colocamos un 1 en el bit inicial en binario para saber donde empezar el bucle
    uint32_t final = 1 << (bit_inicial + num_bits - 1); //colocamos un 1 en el bit final a leer
    uint32_t i;
    for (i = inicial; i <= final; i <<= 1){     // con el 1 desplazamos 1 bit hacia la izq y con el = actualizamos el valor de j
        if((inicial == final) && (inicial == 0x80000000)){
            IOCLR = final;
						break;
        } else{
            IOCLR = i;          // liampiamos los bits que vamos a sobreescribir 
        }
    }
    // valor << bit_inicial es el valor en su correspondiente posición de bits 
    IOSET = (IOPIN | (valor << bit_inicial )); // colocamos el valor solicitado haciendo una or con el valor actual del gpio, y el que queremos introducir 

}

void GPIO_marcar_entrada(uint32_t bit_inicial, uint32_t num_bits){
    uint32_t izq = (IODIR >> (bit_inicial + num_bits)) << (bit_inicial + num_bits); // nos quedamos con los bits de la izq a los que queremos modificar
    uint32_t der = (IODIR << (MAX - bit_inicial)) >> (MAX - bit_inicial); // nos quedamos con los bits de la drcha a los que queremos modificar

    IODIR = izq | der;     // quedan a 0 los bits que queremos utilizar
}


// poner a 1 = stdout 
void GPIO_marcar_salida(uint32_t bit_inicial, uint32_t num_bits){
    uint32_t izq = (IODIR >> (bit_inicial + num_bits)) << (bit_inicial + num_bits); // nos quedamos con los bits de la izq a los que queremos modificar
    uint32_t der = (IODIR << (MAX - bit_inicial)) >> (MAX - bit_inicial); // nos quedamos con los bits de la drcha a los que queremos modificar
    uint32_t valor;
    unsigned int i;
    for(i = 0; i < num_bits; i++){ //segun num_bits desplazamos 1 y sumamos 1 
        valor = (valor << 1) + 1;
    }
    valor = valor << bit_inicial; 
    IODIR = izq | valor | der;     // quedan a 0 los bits que queremos utilizar
}
