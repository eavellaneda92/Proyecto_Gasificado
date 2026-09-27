#include "Leds.h"

void Set_Led_Wait(void){
    S_Led = 0;
    T_Led = 0;
    W_Led = 1;//100ms
    M_Led = WAIT;
}

void Set_Led_Conectado(void){
    S_Led = 0;
    T_Led = 0;
    W_Led = 20;//2000ms
    M_Led = CONECTADO;
}

void Proceso_Led(void){
    if(Z_Led >= 20){
        if(M_Led == CONECTADO) Set_Led_Wait();
        Valor_Salida = 0;
        Z_Led = 0;
    }
    if(M_Led == CONECTADO){
        if(T_Led >= W_Led){
            if(S_Led == 0){
                LED5 = Valor_Salida & 0x01;
                LED4 = (Valor_Salida >> 1) & 0x01;
                LED3 = (Valor_Salida >> 2) & 0x01;
                LED2 = (Valor_Salida >> 3) & 0x01;
                LED1 = 0;
                S_Led = 1;
            }else{
                LED5 = (Valor_Salida >> 4) & 0x01;
                LED4 = (Valor_Salida >> 5) & 0x01;
                LED3 = (Valor_Salida >> 6) & 0x01;
                LED2 = (Valor_Salida >> 7) & 0x01;
                LED1 = 1;
                S_Led = 0;
            }
            T_Led = 0;
        }
    }
    if(M_Led == WAIT){
        if(T_Led >= W_Led){
            if(S_Led == 0){
                LED1 = 1;
                LED2 = 1;
                LED3 = 1;
                LED4 = 1;
                LED5 = 1;
                S_Led = 1;
            }else{
                LED1 = 0;
                LED2 = 0;
                LED3 = 0;
                LED4 = 0;
                LED5 = 0;
                S_Led = 0;
            }
            T_Led = 0;
        }
    }
}
