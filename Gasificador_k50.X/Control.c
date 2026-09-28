#include "Control.h"
#include "Uart.h"
#include "ADC.h"

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

void Proceso_Arranque(void) {
    if (Arranque_Flag == 0) {
        Arranque_Count++;
        if (Arranque_Count >= 200) {
//            /*INICIO DE LA EEPROM*/
//            if (Eeprom_Leer(1) == 0x10) {
//                Set_Channel_Heater(0);
//                Set_Tiempo_Damper(30000);
//                Set_Tiempo_Iny_Gas(2000);
//                Set_Tiempo_Iny_Liquido(1200);
//            } else {
//                Channel_Heater = Get_Channel_Heater();
//                Tiempo_Damper = Get_Tiempo_Damper();
//                Tiempo_Iny_Liquido = Get_Tiempo_Iny_Liquido();
//                Tiempo_Iny_Gas = Get_Tiempo_Iny_Gas();
//                Eeprom_Escribir(1, 0x10);
//            }
            Armado_Cerrado();
            Arranque_Flag = 1;
        }
    }
}

void Armado_Cerrado(void) {
    Power_Rele(Damper_Close, (uint16_t)(Tiempo_Damper * 1.5));
    Power_Rele(Valve_Gas_Close, (uint16_t)(Tiempo_Iny_Gas * 1.5));
    Power_Rele(Valve_Liq_Close, (uint16_t)(Tiempo_Iny_Liquido * 1.5));
}

void Armado_Stop(void){
    Armado_Cerrado();
    Set_Rele(Evaporador,RELE_OFF);
    Set_Rele(Ventilador,RELE_OFF);
    En_Sistem = 0;
}

void Proceso_Heater(void) {
    if (Arranque_Flag == 1) {
        Tiempo_Heater++;
        if (Tiempo_Heater >= 200) {
            if (Temp1 > SP_Temp + 10){
                Set_Rele(Heater1,RELE_OFF);
                Set_Rele(Heater2,RELE_OFF);
            }
            if (Temp1 < SP_Temp - 10) {
                if (Channel_Heater == 0) Set_Rele(Heater1,RELE_ON);
                else Set_Rele(Heater2,RELE_ON);
            }
            Tiempo_Heater = 0;
        }
    }
}

void Inyecta_Liquido(void) {
    Power_Rele(Valve_Liq_Close, Tiempo_Iny_Liquido);
    for (int i = 0; i < Tiempo_Iny_Liquido; i++) {
        __delay_ms(1);
        CLRWDT();
    }
    Power_Rele(Valve_Liq_Open, (uint16_t)(Tiempo_Iny_Liquido * 1.5));
    Respuestas_RS485("INYECCION_LIQUIDO_OK");
}

void Inyecta_Gas(void) {
    Power_Rele(Valve_Gas_Open, Tiempo_Iny_Gas);
    for (int i = 0; i < Tiempo_Iny_Gas; i++) {
        __delay_ms(1);
        CLRWDT();
    }
    Power_Rele(Valve_Gas_Open, (uint16_t)(Tiempo_Iny_Gas * 1.5));
    Respuestas_RS485("INYECCION_GAS_OK");
}

void Damper_Abrir(void) {
    Power_Rele(Damper_Open, Tiempo_Damper);
    Respuestas_RS485("DAMPER_OPEN_OK");
}

void Damper_Cerrar(void) {
    Power_Rele(Damper_Close, (uint16_t)(Tiempo_Damper * 1.5));
    Respuestas_RS485("DAMPER_CLOSE_OK");
}

void Evaporador_On(void) {
    Set_Rele(Evaporador,RELE_ON);
    Respuestas_RS485("EVAPORADOR_ON_OK");
}

void Evaporador_Off(void) {
    Set_Rele(Evaporador,RELE_OFF);
    Respuestas_RS485("EVAPORADOR_OFF_OK");
}

void Ventilador_On(void) {
    Set_Rele(Ventilador,RELE_ON);
    Respuestas_RS485("VENTILADOR_ON_OK");
}

void Ventilador_Off(void) {
    Set_Rele(Ventilador,RELE_OFF);
    Respuestas_RS485("VENTILADOR_OFF_OK");
}

uint8_t Eeprom_Leer(uint8_t dir) {
    while (EECON1bits.WR); // Esperar si hay una escritura en curso

    EEADR = dir; // En el 45K50 EEADR es de 8 bits (no hay EEADRH)
    EECON1bits.EEPGD = 0; // Memoria de datos (no Flash)
    EECON1bits.CFGS = 0; // No configuracion
    EECON1bits.RD = 1; // Lectura inmediata
    return EEDATA;
}

void Eeprom_Escribir(uint8_t dir, uint8_t dato) {
    if (Eeprom_Leer(dir) == dato) return;
    EEADR = dir;
    EEDATA = dato;
    EECON1bits.EEPGD = 0;
    EECON1bits.CFGS = 0;
    EECON1bits.WRERR = 0;
    EECON1bits.WREN = 1; // Habilitar escritura
    uint8_t gie = INTCONbits.GIE;
    INTCONbits.GIE = 0; // La secuencia no puede interrumpirse
    EECON2 = 0x55;
    EECON2 = 0xAA;
    EECON1bits.WR = 1; // Inicia la escritura
    if (gie)
        INTCONbits.GIE = 1; // UART y timers siguen funcionando durante los ~4 ms
    while (EECON1bits.WR) // Esperar fin de escritura
        CLRWDT();
    EECON1bits.WREN = 0; // Proteger contra escrituras accidentales
    PIR2bits.EEIF = 0;
}

void Respuestas_RS485(char *Mensaje) {
    MOD485 = 1;
    __delay_ms(5);
    UART_Print("{\"i\":\"AGROLATINA_G01\",\"rs\":\"Respuesta:");
    UART_Print(Mensaje);
    UART_Print("\"}");
    __delay_ms(5);
    MOD485 = 0;
}

uint8_t Get_Channel_Heater(void) {//0x10
    uint8_t Dato = Eeprom_Leer(0x10);
    return Dato;
}

uint16_t Get_Tiempo_Damper(void) {//0x11
    uint16_t Dato = Eeprom_Leer(0x11);
    Dato = (Dato << 8) | Eeprom_Leer(0x12);
    return Dato;
}

uint16_t Get_Tiempo_Iny_Liquido(void) {//0x13
    uint16_t Dato = Eeprom_Leer(0x13);
    Dato = (Dato << 8) | Eeprom_Leer(0x14);
    return Dato;
}

uint16_t Get_Tiempo_Iny_Gas(void) {
    uint16_t Dato = Eeprom_Leer(0x15);
    Dato = (Dato << 8) | Eeprom_Leer(0x16);
    return Dato;
}

void Set_Channel_Heater(uint8_t Dato) {
    Eeprom_Escribir(0x10,Dato);
    Channel_Heater = Dato;
}

void Set_Tiempo_Damper(uint16_t Dato) {
    Eeprom_Escribir(0x11,(uint8_t)(Dato >> 8));
    Eeprom_Escribir(0x12,(uint8_t)(Dato & 0xFF));
    Tiempo_Damper = Dato;
}

void Set_Tiempo_Iny_Liquido(uint16_t Dato) {
    Eeprom_Escribir(0x13,(uint8_t)(Dato >> 8));
    Eeprom_Escribir(0x14,(uint8_t)(Dato & 0xFF));
    Tiempo_Iny_Liquido = Dato;
}

void Set_Tiempo_Iny_Gas(uint16_t Dato) {
    Eeprom_Escribir(0x15,(uint8_t)(Dato >> 8));
    Eeprom_Escribir(0x16,(uint8_t)(Dato & 0xFF));
    Tiempo_Iny_Gas = Dato;
}