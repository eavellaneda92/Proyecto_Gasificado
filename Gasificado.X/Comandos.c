#include "Comandos.h"
#include "Uarts.h"
#include "Leds.h"
#include "Control.h"

void Get_Comando(unsigned char *Data) {
    float rEthy = 0;
    int iCntEthy = 0;
    char cFlagEthy = 0;
    int iBuffer[10];
    for (int i = 0; i < Length_U1; i++) {
        if (Buffer_U2[i] == 0x20) cFlagEthy = 1;
        if (cFlagEthy == 1) {
            iBuffer[iCntEthy] = Buffer_U2[i];
            iCntEthy++;
        }
        CLRWDT();
    }
    if (cFlagEthy == 1) {
        unsigned int uE = (unsigned int) iBuffer[3];
        uE = uE << 8;
        uE |= (unsigned int) iBuffer[2];
        rEthy = uE;
        rEthy /= 10;
        PPM = (int16_t)rEthy + 1;
        CLRWDT();
    }
    
    uint8_t Len = Texto_Length(Data);
    int Index = IndexOf_Str(Data, "MOD_RELE:");
    if (Index >= 0) {      
        Valor_Salida = (unsigned char)Data;
        if(M_Led == WAIT) Set_Led_Conectado();
        M_Led = CONECTADO;
        Z_Led = 0;
    }
}

