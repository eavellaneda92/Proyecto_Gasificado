#include "Control.h"
#include "Uart.h"

void Pedir_PPM(void) {
    MOD485 = 1;
    __delay_ms(5);
    for (int i = 0; i < 5; i++) {
        UART_Write(Get_PPM[i]);
        if (i == 4) MOD485 = 0;
        __delay_us(30);
        CLRWDT();
    }
    MOD485 = 0;
}

float Cargar_PPM(void) {
    float rEthy = 0;
    int iCntEthy = 0;
    char cFlagEthy = 0;
    int iBuffer[10];
    for (int i = 0; i < BufferIndex; i++) {
        if (Buffer[i] == 0x20) cFlagEthy = 1;
        if (cFlagEthy == 1) {
            iBuffer[iCntEthy] = Buffer[i];
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
        CLRWDT();
    }
    return rEthy;
}

void Proceso_Control(void) {
    Count_Control++;
    if (Count_Control >= 200) {
        //CONSULTA POR SENSOR
        if (Paso_Control == 0) {
            Wait_Sensor = 0;
            Pedir_PPM();
        }

        //CONSULTA POR PANTALLA
        if (Paso_Control == 1) {
            Imprime_Estado();
        }

        Paso_Control++;
        if (Paso_Control >= 2) {
            Paso_Control = 0;
        }
        Count_Control = 0;
    }
    if (Flag_Sensor == 1) {
        Wait_Sensor++;
        if (Wait_Sensor > 50) {
            PPM_Old = 1000;
            PPM = -1;
            Wait_Sensor = 0;
            Flag_Sensor = 0;
        }
    }
}

void Proceso_Arranque(void) {
    if (Arranque_Flag == 0) {
        Arranque_Count++;
        if (Arranque_Count >= 200) {
            Armado_Cerrado();
            Arranque_Flag = 1;
        }
    }
}

void Armado_Cerrado(void) {
    Power_Rele(Damper_Close,30000);
    Power_Rele(Valve_Gas_Close,20000);
    Power_Rele(Valve_Liq_Close,20000);
}

void Armado_Ventila(void) {
    Power_Rele(Damper_Open,30000);
}