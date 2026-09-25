/*
 * File:   Main.c
 * Author: Eus
 *
 * Created on September 22, 2026, 8:59 PM
 */


#include "Config.h"
#include "Timers.h"
#include "Uart.h"
#include "ADC.h"
#include "Control.h"

void __interrupt() ISR(void) {
    if (PIR1bits.RCIF) {
        uint8_t dato = RCREG1;
        if (BufferIndex < BUFFER_SIZE) Buffer[BufferIndex++] = dato;
        Tmr0_Reset();
    }
    if (INTCONbits.TMR0IF) {
        T0CONbits.TMR0ON = 0;
        INTCONbits.TMR0IF = 0;
        FlagBuffer = true; // Trama completa lista en Buffer[]
    }
    // ---------- Timer1: tick permanente de 10 ms ----------
    if (PIE1bits.TMR1IE && PIR1bits.TMR1IF) {
        TMR1H = TMR1_PRE_H;
        TMR1L = TMR1_PRE_L;
        PIR1bits.TMR1IF = 0;
        Flag10ms = true;
    }
}

void main(void) {
    Tmr_Oscilador_Init();
    GPIO_Init();
    UART_Init();
    Tmr0_Init();
    Tmr1_Init();

    RCONbits.IPEN = 0;
    INTCONbits.PEIE = 1;
    INTCONbits.GIE = 1;

    Refresh_In();
    
    while (1) {
        /*CONSULTA POR BUFFER*/
        if (FlagBuffer) {
            if (Flag_Sensor == 0) Get_Comando((char *) Buffer);
            else {
                PPM_Muestra = (uint16_t) Cargar_PPM();
                if (PPM_Muestra < PPM_Old) PPM_Old = PPM_Muestra;
                Muestras_Sensor++;
                if (Muestras_Sensor >= 5) {
                    PPM = (int16_t) PPM_Old;
                    PPM_Old = 1000;
                    Muestras_Sensor = 0;
                }
                Flag_Sensor = 0;
            }
            //AQUI ANALIZA SI HAY UN COMANDO
            for (uint8_t i = 0; i < BUFFER_SIZE; i++) Buffer[i] = '\0';
            BufferIndex = 0;
            FlagBuffer = 0;
        }

        /*TIEMPO CADA 10 MS*/
        if (Flag10ms) {
            Refresh_In();
            Proceso_ADC();
            Proceso_Rele();
            Proceso_Control();
            Proceso_Arranque();
            Flag10ms = 0;
        }
        CLRWDT();
    }
}
