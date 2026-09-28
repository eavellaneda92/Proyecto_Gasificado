#include "Timers.h"
#include "Uarts.h"
#include "Leds.h"
#include "Control.h"

void Timer_Interrupt(void) {
    if (INTCONbits.TMR0IF) {
        Flag_U1 = 1;
        T0CONbits.TMR0ON = 0;
        INTCONbits.TMR0IF = 0;
    }
    if (PIR1bits.TMR1IF) {
        Flag_U2 = 1;
        T1CONbits.TMR1ON = 0;
        PIR1bits.TMR1IF = 0;
    }
    if (PIR1bits.TMR2IF) {
        Flag_100ms++;
        if(Flag_100ms >= 10){
            T_Led++;
            Flag_100ms = 0;
        }
        Flag_1000ms++;
        if (Flag_1000ms >= 100) {
            Z_Led++;
            Tiempo_Control++;
            Flag_1000ms = 0;
        }
        PIR1bits.TMR2IF = 0;
    }
}

void Timer0_Init(void) {
    T0CON = 0x03;
    TMR0H = (uint8_t) (TMR0_50MS >> 8);
    TMR0L = (uint8_t) (TMR0_50MS & 0xFF);
    T0CONbits.TMR0ON = 0;
    INTCONbits.TMR0IE = 1;
    INTCONbits.TMR0IF = 0;
}

void Timer1_Init(void) {
    T1CON = 0x32;
    T1GCON = 0x00;
    TMR1H = (uint8_t) (TMR1_50MS >> 8);
    TMR1L = (uint8_t) (TMR1_50MS & 0xFF);
    PIR1bits.TMR1IF = 0;
    PIE1bits.TMR1IE = 1;
    T1CONbits.TMR1ON = 0;
}

void Timer2_Init(void) {
    T2CON = 0x00;
    PR2 = 249;
    TMR2 = 0;
    PIR1bits.TMR2IF = 0;
    PIE1bits.TMR2IE = 1;
    T2CON = 0x4E;
}

