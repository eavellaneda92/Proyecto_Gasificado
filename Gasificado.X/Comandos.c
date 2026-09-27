#include "Comandos.h"
#include "Uarts.h"
#include "Leds.h"

void Get_Comando(unsigned char *Data) {
    uint8_t Len = Texto_Length(Data);
    int Index = IndexOf_Str(Data, "MOD_RELE:");
    if (Index >= 0) {
        unsigned int Dat = 0;
        for (int i = Index; i < Len; i++) {
            unsigned char c = Data[i];
            if (c >= '0' && c <= '9') {
                Dat = Dat * 10 + c - 48;
            }
        }
        if (Dat > 255) Dat = 0b11111111;
        MOD485 = 1;
        __delay_ms(10);
        UART1_WriteString("{\"i\":\"GASIFICADO01\",\"rs\":\"OK->");
        sprintf(txt, "%d\"}\n\r",Dat);
        UART1_WriteString(txt);
        RELE1 = Dat & 0b1;
        RELE2 = (Dat >> 1) & 0b1;
        RELE3 = (Dat >> 2) & 0b1;
        RELE4 = (Dat >> 3) & 0b1;
        RELE5 = (Dat >> 4) & 0b1;
        RELE6 = (Dat >> 5) & 0b1;
        RELE7 = (Dat >> 6) & 0b1;
        RELE8 = (Dat >> 7) & 0b1;
        __delay_ms(5);
        MOD485 = 0;
        Valor_Salida = (unsigned char)Dat;
        if(M_Led == WAIT) Set_Led_Conectado();
        M_Led = CONECTADO;
        Z_Led = 0;
    }
    Index = IndexOf_Str(Data, "MOD_STATUS");
    if (Index >= 0) {
        MOD485 = 1;
        __delay_ms(5);
        UART1_WriteString("{\"i\":\"GASIFICADO01\",\"rs\":\"STATUS:");
        sprintf(txt, "%d\"}\n\r",Valor_Salida);
        UART1_WriteString(txt);
        __delay_ms(5);
        MOD485 = 0;
        if(M_Led == WAIT) Set_Led_Conectado();
        M_Led = CONECTADO;
        Z_Led = 0;
    }
}

