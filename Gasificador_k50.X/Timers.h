/* 
 * File:   Timers.h
 * Author: Eus
 *
 * Created on September 22, 2026, 9:00 PM
 */

#ifndef TIMERS_H
#define	TIMERS_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "Config.h"

    // Timer0: 16 bits, prescaler 1:4 -> 1 us por tick
    //         50 ms = 50000 ticks -> precarga = 65536 - 50000 = 15536 (0x3CB0)
#define TMR0_PRE_H   0x3C
#define TMR0_PRE_L   0xB0

    // Timer1: Fosc/4, prescaler 1:1 -> 0.25 us por tick
    //         10 ms = 40000 ticks -> precarga = 65536 - 40000 = 25536 (0x63C0)
#define TMR1_PRE_H   0x63
#define TMR1_PRE_L   0xC0

    uint8_t Flag10ms = 0;

    void Tmr_Oscilador_Init(void);
    void Tmr0_Init(void);
    void Tmr0_Reset(void);

    void Tmr1_Init(void);

    /*CONTROL DE LOS RELES*/
#define RELE_ON 0
#define RELE_OFF 1

#define RELAY1 LATBbits.LATB5
#define RELAY2 LATBbits.LATB4
#define RELAY3 LATBbits.LATB3
#define RELAY4 LATBbits.LATB2
#define RELAY5 LATBbits.LATB1
#define RELAY6 LATBbits.LATB0
#define RELAY7 LATDbits.LATD7
#define RELAY8 LATDbits.LATD6
#define RELAY9 LATDbits.LATD5
#define RELAY10 LATDbits.LATD4

    char Enable_Rele[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    unsigned int Wait_Rele[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    unsigned int Tiempo_Rele[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

    void Power_Rele(unsigned char Rele, unsigned int Tiempo);
    void Proceso_Rele(void);

#define IN1 PORTEbits.RE0
#define IN2 PORTEbits.RE1
#define IN3 PORTEbits.RE2
#define IN4 PORTCbits.RC0
#define IN5 PORTCbits.RC1
#define IN6 PORTCbits.RC2
#define IN7 PORTDbits.RD0
#define IN8 PORTDbits.RD1
#define IN9 PORTDbits.RD2
#define IN10 PORTDbits.RD3
    
    uint8_t Cambio_Estado = 0;
    uint8_t Status_In[10]={0,0,0,0,0,0,0,0,0,0};
    uint8_t Old_In[10]={0,0,0,0,0,0,0,0,0,0};
    void Refresh_In(void);

#ifdef	__cplusplus
}
#endif

#endif	/* TIMERS_H */

