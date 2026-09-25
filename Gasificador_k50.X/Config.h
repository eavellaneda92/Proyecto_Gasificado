/* 
 * File:   Config.h
 * Author: Eus
 *
 * Created on September 22, 2026, 9:00 PM
 */

#ifndef CONFIG_H
#define	CONFIG_H

#ifdef	__cplusplus
extern "C" {
#endif

    // CONFIG1L
#pragma config PLLSEL = PLL3X       // Irrelevante (PLL deshabilitado)
#pragma config CFGPLLEN = OFF       // PLL deshabilitado
#pragma config CPUDIV = NOCLKDIV    // CPU = Fosc (sin division)
#pragma config LS48MHZ = SYS24X4    // Solo afecta USB

    // CONFIG1H
#pragma config FOSC = INTOSCIO      // Oscilador interno, RA6/RA7 como I/O
#pragma config PCLKEN = ON
#pragma config FCMEN = OFF
#pragma config IESO = OFF

    // CONFIG2L
#pragma config nPWRTEN = ON         // Power-up timer habilitado
#pragma config BOREN = SBORDIS      // Brown-out por hardware
#pragma config BORV = 250           // Ajustar segun tu Vdd (190/220/250/285)
#pragma config nLPBOR = OFF

    // CONFIG2H
#pragma config WDTEN = ON           // WDT SIEMPRE activo (no se puede apagar por software)
#pragma config WDTPS = 128          // ~512 ms nominal

    // CONFIG3H
#pragma config CCP2MX = RC1
#pragma config PBADEN = OFF         // PORTB digital al reset
#pragma config T3CMX = RC0
#pragma config SDOMX = RB3
#pragma config MCLRE = ON

    // CONFIG4L
#pragma config STVREN = ON
#pragma config LVP = ON
#pragma config ICPRT = OFF
#pragma config XINST = OFF

    // CONFIG5L / 5H
#pragma config CP0 = OFF
#pragma config CP1 = OFF
#pragma config CP2 = OFF
#pragma config CP3 = OFF
#pragma config CPB = OFF
#pragma config CPD = OFF

    // CONFIG6L / 6H
#pragma config WRT0 = OFF
#pragma config WRT1 = OFF
#pragma config WRT2 = OFF
#pragma config WRT3 = OFF
#pragma config WRTC = OFF
#pragma config WRTB = OFF
#pragma config WRTD = OFF

    // CONFIG7L / 7H
#pragma config EBTR0 = OFF
#pragma config EBTR1 = OFF
#pragma config EBTR2 = OFF
#pragma config EBTR3 = OFF
#pragma config EBTRB = OFF

#define _XTAL_FREQ 16000000UL

#include <xc.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

    char txt[20];
    void GPIO_Init(void);


#ifdef	__cplusplus
}
#endif

#endif	/* CONFIG_H */

