#include "Config.h"
#include "ADC.h"

void GPIO_Init(void){
    ANSELA = 0x00;                          // Ajusta segun tus entradas analogicas
    ANSELB = 0x00;
    ANSELC = 0x00;
    ANSELD = 0x00;
    ANSELE = 0x00;
    
    /*CONFIGURACION DE SALIDAS*/
    TRISB = 0b10000000;
    TRISE = 0b1111;
    TRISD = 0b00001111;
    TRISC = 0b11111111; 
    
    LATB = 0xFF;
    LATE = 0xFF;
    LATD = 0xFF;
    
    LATBbits.LATB6 = 0;
    
    ADC_Init();
}