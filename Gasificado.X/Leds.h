/* 
 * File:   Leds.h
 * Author: LENOVO
 *
 * Created on 20 de agosto de 2026, 12:17 PM
 */

#ifndef LEDS_H
#define	LEDS_H

#ifdef	__cplusplus
extern "C" {
#endif


#include "Config.h"
    
#define WAIT 0
#define CONECTADO 1
    
unsigned char Z_Led = 0; //TIempo de espera si no recibe comandos
unsigned char T_Led = 0; //Tiempo para secuencia
unsigned char W_Led = 0; //Tiempo de espera para renovar secuencia

unsigned char M_Led = 0; //Indica el modo de estado
unsigned char S_Led = 0; //Indica parte 1 o parte 2
unsigned char Valor_Salida = 0; //Indica el valor de salidas

void Proceso_Led(void);
void Set_Led_Wait(void);
void Set_Led_Conectado(void);


#ifdef	__cplusplus
}
#endif

#endif	/* LEDS_H */

