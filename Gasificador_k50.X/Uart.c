#include "Uart.h"
#include "Timers.h"
#include "ADC.h"
#include "Control.h"

void UART_Init(void) {
    ANSELCbits.ANSC6 = 0; // RC6/TX digital
    ANSELCbits.ANSC7 = 0; // RC7/RX digital
    TRISCbits.TRISC6 = 1; // El EUSART controla el pin
    TRISCbits.TRISC7 = 1;

    BAUDCON1bits.BRG16 = 1;
    TXSTA1bits.BRGH = 1;
    TXSTA1bits.SYNC = 0; // Asincrono
    SPBRGH1 = UART_BRG_H;
    SPBRG1 = UART_BRG_L;

    RCSTA1bits.SPEN = 1; // Habilita puerto serie
    RCSTA1bits.CREN = 1; // Recepcion continua
    TXSTA1bits.TXEN = 1; // Transmision

    PIR1bits.RCIF = 0;
    PIE1bits.RCIE = 1; // Interrupcion por recepcion
}

void UART_Write(uint8_t dato) {
    while (!TXSTA1bits.TRMT);
    TXREG1 = dato;
}

void UART_Print(const char *cad) {
    while (*cad) UART_Write((uint8_t) * cad++);
}

/*FUNCIONES DE TEXTO*************************************************/
int Texto_Length(char *texto) {
    int contador = 0;
    while (*texto) {
        CLRWDT();
        contador++;
        texto++;
    }
    return contador;
}

int Texto_IndexOf(char *texto, char *valor) {
    int length_texto = (int) Texto_Length(texto);
    int length_valor = (int) Texto_Length(valor);
    if (length_texto >= length_valor) {
        for (int i = 0; i <= length_texto - length_valor; i++) {
            if (texto[i] == valor[0]) {
                int error = 1;
                for (int j = 0; j < length_valor; j++) {
                    if (texto[j + i] != valor[j]) error = 0;
                }
                if (error == 1) return i;
            }
        }
    }
    return -1;
}

void Get_Comando(char *Data) {
    if (Texto_IndexOf(Data, "AGROLATINA_G01") >= 0) {
        if (Texto_IndexOf(Data, "CB01") >= 0) {
            int Index = 0;
            /*ACTIVAR RELE TEMPORIZADO*/
            Index = Texto_IndexOf(Data, "RT(");
            if (Index >= 0) {
                int Fin = Texto_IndexOf(Data, ")");
                unsigned int Tiempo = 0;
                unsigned char Rele = 0;
                unsigned char Coma = 0;
                for (int i = Index; i < Fin; i++) {
                    char c = Data[i];
                    if (c == ',') Coma++;
                    if (c >= '0' && c <= '9') {
                        if (Coma == 0) Rele = Rele * 10 + c - 48;
                        if (Coma == 1) Tiempo = Tiempo * 10 + c - 48;
                    }
                }
                Power_Rele(Rele, Tiempo);
            }
            /*ACTIVAR RELE PERMANENTE*/
            Index = Texto_IndexOf(Data, "RP(");
            if (Index >= 0) {
                int Fin = Texto_IndexOf(Data, ")");
                int Tiempo = 0;
                unsigned char Rele = 0;
                unsigned char Coma = 0;
                for (int i = Index; i < Fin; i++) {
                    char c = Data[i];
                    if (c == ',') Coma++;
                    if (c >= '0' && c <= '9') {
                        if (Coma == 0) Rele = Rele * 10 + c - 48;
                        if (Coma == 1) Tiempo = Tiempo * 10 + c - 48;
                    }
                }
                uint8_t Estado = 0;
                if(Tiempo == 0) Estado = RELE_OFF;
                else Estado = RELE_ON;
                switch (Rele) {
                    case 0: RELAY1 = Estado;
                        break;
                    case 1: RELAY2 = Estado;
                        break;
                    case 2: RELAY3 = Estado;
                        break;
                    case 3: RELAY4 = Estado;
                        break;
                    case 4: RELAY5 = Estado;
                        break;
                    case 5: RELAY6 = Estado;
                        break;
                    case 6: RELAY7 = Estado;
                        break;
                    case 7: RELAY8 = Estado;
                        break;
                    case 8: RELAY9 = Estado;
                        break;
                    case 9: RELAY10 = Estado;
                        break;
                }
            }
            /*VALOR DE ANALOG Y ENTRADAS*/
            Index = Texto_IndexOf(Data, "STATUS");
            if (Index >= 0) {
                Imprime_Estado();
            }
            
            /*COMANDO PARA INYECTAR LIQUIDO*/
            Index = Texto_IndexOf(Data, "INY_LIQUIDO");
            if (Index >= 0) {
                Inyecta_Liquido();
            }
            /*COMANDO PARA INYECTAR GAS*/
            Index = Texto_IndexOf(Data, "INY_GAS");
            if (Index >= 0) {
                Inyecta_Gas();
            }
            /*COMANDO PARA ABRIR LO DAMPER*/
            Index = Texto_IndexOf(Data, "DAMPER_OPEN");
            if (Index >= 0) {
                Damper_Abrir();
            }
            /*COMANDO PARA CERRAR LO DAMPER*/
            Index = Texto_IndexOf(Data, "DAMPER_CLOSE");
            if (Index >= 0) {
                Damper_Cerrar();
            }
            /*COMANDO PARA ACTIVAR EVAPORADOR*/
            Index = Texto_IndexOf(Data, "EVAPORADOR_ON");
            if (Index >= 0) {
                Evaporador_On();
            }
            /*COMANDO PARA DESACTIVAR EVAPORADOR*/
            Index = Texto_IndexOf(Data, "EVAPORADOR_OFF");
            if (Index >= 0) {
                Evaporador_Off();
            }
        }
    }
}

void Imprime_Estado(void) {
    char R1 = RELAY1;
    char R2 = RELAY2;
    char R3 = RELAY3;
    char R4 = RELAY4;
    char R5 = RELAY5;
    char R6 = RELAY6;
    char R7 = RELAY7;
    char R8 = RELAY8;
    char R9 = RELAY9;
    char R10 = RELAY10;
    MOD485 = 1;
    __delay_ms(5);
    UART_Print("{\"i\":\"AGROLATINA_G01\",\"rs\":\"STATUS:");
    for (int i = 0; i < 10; i++) {
        sprintf(txt, "%d", Status_In[i]);
        UART_Print(txt);
    }
    UART_Print(",");
    sprintf(txt, "%d%d%d%d%d%d%d%d%d%d", R1, R2, R3, R4, R5, R6, R7, R8, R9, R10);
    UART_Print(txt);
    UART_Print(",");
    sprintf(txt, "%d,%d,%d,%d,%d", Current1, Current2, Temp1, Temp2, PPM);
    UART_Print(txt);
    UART_Print(",");
    sprintf(txt, "%d,%d,%d,%d", Channel_Heater, Tiempo_Damper, Tiempo_Iny_Liquido, Tiempo_Iny_Gas);
    UART_Print("\"}");
    __delay_ms(5);
    MOD485 = 0;
}