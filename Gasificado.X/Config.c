
#include "Config.h"

void Pic_Clock_Init(void){
    uint16_t guarda;

    OSCCON  = 0x72;    /* IRCF = 111 (HFINTOSC 16 MHz), SCS = 10 (internal osc block) */
    OSCCON2 = 0x04;    /* PRISD = 1, MFIOSEL = 0, SOSCGO = 0                          */
    OSCTUNE = 0x00;    /* PLLEN = 0 -> sin PLL, Fosc = 16 MHz. TUN = 0 (sin ajuste)   */

    /* espera a que el HFINTOSC este estable */
    guarda = 0;
    while (!OSCCONbits.HFIOFS && ++guarda < 10000) { CLRWDT(); }
}

void Pic_Gpio_Init(void){
    /**
    LATx registers
    */
    LATE = 0x00;
    LATD = 0x00;
    LATA = 0x00;
    LATB = 0x00;
    LATC = 0x00;

    /**
    TRISx registers
    */
    TRISE = 0xFF;
    TRISA = 0xFF;
    TRISB = 0xFF;
    TRISC = 0xFF;
    TRISD = 0xFF;

    /**
    ANSELx registers
    */
    ANSELD = 0x00;
    ANSELC = 0x00;
    ANSELB = 0x00;
    ANSELE = 0x00;
    ANSELA = 0x00;

    /**
    WPUx registers
    */
    WPUB = 0x00;
    INTCON2bits.nRBPU = 1;
    
    // CCP5M off/reset; DC5B 0; 
	CCP5CON = 0x00;    
	// CCPR5L 0; 
	CCPR5L = 0x00;   
	// CCPR5H 0; 
	CCPR5H = 0x00;    
    
    /*CONFIGURACION PARA SALIDAS DE RELE*/
    C_RELE1 = 0;
    C_RELE2 = 0;
    C_RELE3 = 0;
    C_RELE4 = 0;
    C_RELE5 = 0;
    C_RELE6 = 0;
    C_RELE7 = 0;
    C_RELE8 = 0;
    RELE1 = RELE_ON;
    RELE2 = RELE_OFF;
    RELE3 = RELE_ON;
    RELE4 = RELE_OFF;
    RELE5 = RELE_ON;
    RELE6 = RELE_OFF;
    RELE7 = RELE_ON;
    RELE8 = RELE_OFF;
    
    /*CONFIGURACION PARA LEDS*/
    C_LED1 = 0;
    C_LED2 = 0;
    C_LED3 = 0;
    C_LED4 = 0;
    C_LED5 = 0;
    LED1 = 0;
    LED2 = 0;
    LED3 = 0;
    LED4 = 0;
    LED5 = 0;
    
    /*CONFIGURAR EL VALOR ANALOGICO*/
    ANSELAbits.ANSA2 = 1;
    
    /*CONTROL DE MODBUS*/
    TRISDbits.RD5 = 0;
    LATDbits.LATD5 = 0;
}

