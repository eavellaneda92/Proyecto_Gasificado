#include "Timers.h"

void Tmr_Oscilador_Init(void) {
    OSCCONbits.IRCF = 0b111; // HFINTOSC = 16 MHz
    OSCCONbits.SCS = 0b00; // Reloj definido por FOSC (INTOSCIO)
    while (!OSCCONbits.HFIOFS); // Esperar oscilador estable
}

void Tmr0_Init(void) {
    T0CON = 0b00000001;
    TMR0H = TMR0_PRE_H;
    TMR0L = TMR0_PRE_L;
    INTCONbits.TMR0IF = 0;
    INTCONbits.TMR0IE = 1; // Queda apagado hasta que llegue el primer byte
}

void Tmr0_Reset(void) {
    T0CONbits.TMR0ON = 0;
    TMR0H = TMR0_PRE_H;
    TMR0L = TMR0_PRE_L;
    INTCONbits.TMR0IF = 0;
    T0CONbits.TMR0ON = 1;
}

void Tmr1_Init(void) {
    T1GCON = 0x00; // Sin gate
    // TMR1CS=00 (Fosc/4), T1CKPS=00 (1:1), SOSCEN=0, RD16=1, TMR1ON=0
    T1CON = 0b00000010;
    TMR1H = TMR1_PRE_H; // Con RD16 escribir primero H, luego L
    TMR1L = TMR1_PRE_L;
    PIR1bits.TMR1IF = 0;
    PIE1bits.TMR1IE = 1;
    T1CONbits.TMR1ON = 1; // Permanente
}

void Set_Rele(unsigned char Rele, unsigned char Valor){
    switch (Rele) {
            case 0: RELAY1 = Valor;
                break;
            case 1: RELAY2 = Valor;
                break;
            case 2: RELAY3 = Valor;
                break;
            case 3: RELAY4 = Valor;
                break;
            case 4: RELAY5 = Valor;
                break;
            case 5: RELAY6 = Valor;
                break;
            case 6: RELAY7 = Valor;
                break;
            case 7: RELAY8 = Valor;
                break;
            case 8: RELAY9 = Valor;
                break;
            case 9: RELAY10 = Valor;
                break;
        }
}

void Power_Rele(unsigned char Rele, unsigned int Tiempo) {
    Tiempo = Tiempo / 10;
    if (Rele >= 1 && Rele <= 10) {
        Tiempo_Rele[Rele] = 0;
        Wait_Rele[Rele] = Tiempo;
        Enable_Rele[Rele] = 1;
        switch (Rele) {
            case 0: RELAY1 = RELE_ON;
                break;
            case 1: RELAY2 = RELE_ON;
                break;
            case 2: RELAY3 = RELE_ON;
                break;
            case 3: RELAY4 = RELE_ON;
                break;
            case 4: RELAY5 = RELE_ON;
                break;
            case 5: RELAY6 = RELE_ON;
                break;
            case 6: RELAY7 = RELE_ON;
                break;
            case 7: RELAY8 = RELE_ON;
                break;
            case 8: RELAY9 = RELE_ON;
                break;
            case 9: RELAY10 = RELE_ON;
                break;
        }
    }
}

void Proceso_Rele(void) {
    for (int i = 0; i < 10; i++) {
        if (Enable_Rele[i] == 1) {
            Tiempo_Rele[i]++;
            if (Tiempo_Rele[i] >= Wait_Rele[i]) {
                switch (i) {
                    case 0: RELAY1 = RELE_OFF;
                        break;
                    case 1: RELAY2 = RELE_OFF;
                        break;
                    case 2: RELAY3 = RELE_OFF;
                        break;
                    case 3: RELAY4 = RELE_OFF;
                        break;
                    case 4: RELAY5 = RELE_OFF;
                        break;
                    case 5: RELAY6 = RELE_OFF;
                        break;
                    case 6: RELAY7 = RELE_OFF;
                        break;
                    case 7: RELAY8 = RELE_OFF;
                        break;
                    case 8: RELAY9 = RELE_OFF;
                        break;
                    case 9: RELAY10 = RELE_OFF;
                        break;
                }
                Enable_Rele[i] = 0;
            }
        }
    }
}

void Refresh_In(void) {
    Status_In[0] = IN1;
    Status_In[1] = IN2;
    Status_In[2] = IN3;
    Status_In[3] = IN4;
    Status_In[4] = IN5;
    Status_In[5] = IN6;
    Status_In[6] = IN7;
    Status_In[7] = IN8;
    Status_In[8] = IN9;
    Status_In[9] = IN10;

    for (int i = 0; i < 10; i++) {
        if (Status_In[i] != Old_In[i]) {
            Old_In[i] = Status_In[i];
            Cambio_Estado = 1;
        }
    }
}