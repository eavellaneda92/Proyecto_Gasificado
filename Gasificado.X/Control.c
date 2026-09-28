#include "Control.h"
#include "Uarts.h"
#include "Kinco.h"
const unsigned char Get_PPM[5] = {0x01, 0x20, 0x00, 0x39, 0xc0};

void Consulta_Sensor(void) {
    MOD485 = 1;
    __delay_ms(5);
    __delay_ms(5);
    for (int i = 0; i < 5; i++) {
        UART1_Write(Get_PPM[i]);
        if (i == 4) MOD485 = 0;
        __delay_us(30);
        CLRWDT();
    }
    __delay_ms(5);
    MOD485 = 0;
}

void Consulta_Relay(void) {
    MOD485 = 1;
    __delay_ms(5);
    __delay_ms(5);
    UART1_WriteString("AGROLATINA_G01_CB01_STATUS_");
    __delay_ms(5);
    MOD485 = 0;
}

void Consulta_Server(void) {
    MOD485 = 1;
    __delay_ms(5);
    __delay_ms(5);
    Gasificado.Variables[V_ETILENO] = PPM * 10;
    UART1_WriteString("PPM:");
    sprintf(txt,"%d",PPM);
    UART1_WriteString(txt);
    
    __delay_ms(5);
    MOD485 = 0;
}