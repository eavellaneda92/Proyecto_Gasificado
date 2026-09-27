/* 
 * File:   Config.h
 * Author: LENOVO
 *
 * Created on 21 de julio de 2026, 04:27 PM
 */

#ifndef CONFIG_H
#define	CONFIG_H

#ifdef	__cplusplus
extern "C" {
#endif

    // CONFIG1H
#pragma config FOSC = INTIO67    // Oscillator Selection bits->Internal oscillator block
#pragma config PLLCFG = ON    // 4X PLL Enable->Oscillator multiplied by 4
#pragma config PRICLKEN = ON    // Primary clock enable bit->Primary clock is always enabled
#pragma config FCMEN = OFF    // Fail-Safe Clock Monitor Enable bit->Fail-Safe Clock Monitor disabled
#pragma config IESO = OFF    // Internal/External Oscillator Switchover bit->Oscillator Switchover mode disabled

    // CONFIG2L
#pragma config PWRTEN = OFF    // Power-up Timer Enable bit->Power up timer disabled
#pragma config BOREN = SBORDIS    // Brown-out Reset Enable bits->Brown-out Reset enabled in hardware only (SBOREN is disabled)
#pragma config BORV = 190    // Brown Out Reset Voltage bits->VBOR set to 1.90 V nominal

    // CONFIG2H
#pragma config WDTEN = ON    // Watchdog Timer Enable bits->WDT is always enabled. SWDTEN bit has no effect
#pragma config WDTPS = 128    // Watchdog Timer Postscale Select bits->1:128

    // CONFIG3H
#pragma config CCP2MX = PORTC1    // CCP2 MUX bit->CCP2 input/output is multiplexed with RC1
#pragma config PBADEN = ON    // PORTB A/D Enable bit->PORTB<5:0> pins are configured as analog input channels on Reset
#pragma config CCP3MX = PORTB5    // P3A/CCP3 Mux bit->P3A/CCP3 input/output is multiplexed with RB5
#pragma config HFOFST = ON    // HFINTOSC Fast Start-up->HFINTOSC output and ready status are not delayed by the oscillator stable status
#pragma config T3CMX = PORTC0    // Timer3 Clock input mux bit->T3CKI is on RC0
#pragma config P2BMX = PORTD2    // ECCP2 B output mux bit->P2B is on RD2
#pragma config MCLRE = EXTMCLR    // MCLR Pin Enable bit->MCLR pin enabled, RE3 input pin disabled

    // CONFIG4L
#pragma config STVREN = ON    // Stack Full/Underflow Reset Enable bit->Stack full/underflow will cause Reset
#pragma config LVP = ON    // Single-Supply ICSP Enable bit->Single-Supply ICSP enabled if MCLRE is also 1
#pragma config XINST = OFF    // Extended Instruction Set Enable bit->Instruction set extension and Indexed Addressing mode disabled (Legacy mode)
#pragma config DEBUG = OFF    // Background Debug->Disabled

    // CONFIG5L
#pragma config CP0 = OFF    // Code Protection Block 0->Block 0 (000800-003FFFh) not code-protected
#pragma config CP1 = OFF    // Code Protection Block 1->Block 1 (004000-007FFFh) not code-protected
#pragma config CP2 = OFF    // Code Protection Block 2->Block 2 (008000-00BFFFh) not code-protected
#pragma config CP3 = OFF    // Code Protection Block 3->Block 3 (00C000-00FFFFh) not code-protected

    // CONFIG5H
#pragma config CPB = OFF    // Boot Block Code Protection bit->Boot block (000000-0007FFh) not code-protected
#pragma config CPD = OFF    // Data EEPROM Code Protection bit->Data EEPROM not code-protected

    // CONFIG6L
#pragma config WRT0 = OFF    // Write Protection Block 0->Block 0 (000800-003FFFh) not write-protected
#pragma config WRT1 = OFF    // Write Protection Block 1->Block 1 (004000-007FFFh) not write-protected
#pragma config WRT2 = OFF    // Write Protection Block 2->Block 2 (008000-00BFFFh) not write-protected
#pragma config WRT3 = OFF    // Write Protection Block 3->Block 3 (00C000-00FFFFh) not write-protected

    // CONFIG6H
#pragma config WRTC = OFF    // Configuration Register Write Protection bit->Configuration registers (300000-3000FFh) not write-protected
#pragma config WRTB = OFF    // Boot Block Write Protection bit->Boot Block (000000-0007FFh) not write-protected
#pragma config WRTD = OFF    // Data EEPROM Write Protection bit->Data EEPROM not write-protected

    // CONFIG7L
#pragma config EBTR0 = OFF    // Table Read Protection Block 0->Block 0 (000800-003FFFh) not protected from table reads executed in other blocks
#pragma config EBTR1 = OFF    // Table Read Protection Block 1->Block 1 (004000-007FFFh) not protected from table reads executed in other blocks
#pragma config EBTR2 = OFF    // Table Read Protection Block 2->Block 2 (008000-00BFFFh) not protected from table reads executed in other blocks
#pragma config EBTR3 = OFF    // Table Read Protection Block 3->Block 3 (00C000-00FFFFh) not protected from table reads executed in other blocks

    // CONFIG7H
#pragma config EBTRB = OFF    // Boot Block Table Read Protection bit->Boot Block (000000-0007FFh) not protected from table reads executed in other blocks

#define _XTAL_FREQ 16000000

#include <xc.h>
#include <stdio.h>

#define IMEI_DEFAULT    "MBZT001"    

    char txt[20];
    void Pic_Clock_Init(void);
    void Pic_Gpio_Init(void);

    /*SALIDAS PARA CONTROL DE RELE*/
#define RELE_ON 1
#define RELE_OFF 0

#define CMOD485 TRISDbits.RD5
#define MOD485 LATDbits.LATD5

    //PUERTO 1
#define C_RELE1 TRISBbits.RB0
#define C_RELE2 TRISBbits.RB1
#define C_RELE3 TRISBbits.RB2
#define C_RELE4 TRISBbits.RB3
    //PUERTO 2
#define C_RELE5 TRISDbits.RD0
#define C_RELE6 TRISDbits.RD1
#define C_RELE7 TRISDbits.RD2
#define C_RELE8 TRISDbits.RD3
    //PUERTO 1
#define RELE1   LATBbits.LATB0
#define RELE2   LATBbits.LATB1
#define RELE3   LATBbits.LATB2
#define RELE4   LATBbits.LATB3
    //PUERTO 2
#define RELE5   LATDbits.LATD3
#define RELE6   LATDbits.LATD2
#define RELE7   LATDbits.LATD1
#define RELE8   LATDbits.LATD0

#define C_LED1  TRISCbits.RC0
#define C_LED2  TRISCbits.RC1
#define C_LED3  TRISCbits.RC2
#define C_LED4  TRISCbits.RC3
#define C_LED5  TRISCbits.RC4

#define LED1    LATCbits.LATC0
#define LED2    LATCbits.LATC1
#define LED3    LATCbits.LATC2
#define LED4    LATCbits.LATC3
#define LED5    LATCbits.LATC4

#ifdef	__cplusplus
}
#endif

#endif	/* CONFIG_H */

