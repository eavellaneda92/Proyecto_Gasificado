#include "Uarts.h"
#include "Timers.h"
#include "Comandos.h"
#include "Kinco.h"

void UART_Interrupt(void) {
    if (PIR1bits.RC1IF) {
        char c = RCREG1;
        if (Length_U1 < Len_Max_U1) {
            Buffer_U1[Length_U1] = c;
            Length_U1++;
        }
        T0CONbits.TMR0ON = 1;
        TMR0H = (uint8_t) (TMR0_50MS >> 8);
        TMR0L = (uint8_t) (TMR0_50MS & 0xFF);
        PIR1bits.RC1IF = 0;
    }
    if (PIR3bits.RC2IF) {
        char c = RCREG2;
        if (Length_U2 < Len_Max_U2) {
            Buffer_U2[Length_U2] = c;
            Length_U2++;
        }
        T1CONbits.TMR1ON = 1;
        TMR1H = (uint8_t) (TMR1_50MS >> 8);
        TMR1L = (uint8_t) (TMR1_50MS & 0xFF);
        PIR3bits.RC2IF = 0;
    }
}

void UART_Read(void) {
    /*UART1*/
    if (Flag_U1 == 1) {
        Get_Comando(Buffer_U1);
        /*LIMPIAR DATOS DE BUFFER 1*/
        for (int i = 0; i < Len_Max_U1; i++) {
            Buffer_U1[i] = '\0';
            CLRWDT();
        }
        Length_U1 = 0;
        Flag_U1 = 0;
    }
    /*UART2*/
    if (Flag_U2 == 1) {
        /*LIMPIAR DATOS DE BUFFER 2*/
        Read_Cmd(Buffer_U2);
        for (int i = 0; i < Len_Max_U2; i++) {
            Buffer_U2[i] = '\0';
            CLRWDT();
        }
        Length_U2 = 0;
        Flag_U2 = 0;
    }
}

void UART1_Init(unsigned long Baud) {
    uint16_t brg;
    if (Baud == 0UL) return;
    /* division con redondeo al entero mas cercano */
    brg = (uint16_t) ((((_XTAL_FREQ / 4UL) + (Baud / 2UL)) / Baud) - 1UL);
    /* RC6/RC7 deben salir de modo analogico (ANSELx arranca en analogico) */
    ANSELCbits.ANSC6 = 0;
    ANSELCbits.ANSC7 = 0;
    TRISCbits.TRISC6 = 0; /* TX1 salida*/
    TRISCbits.TRISC7 = 1; /* RX1 entrada*/

    RCSTA1 = 0x00;
    TXSTA1 = 0x00;
    BAUDCON1 = 0x00;

    BAUDCON1bits.BRG16 = 1;
    TXSTA1bits.BRGH = 1;
    SPBRGH1 = (uint8_t) (brg >> 8);
    SPBRG1 = (uint8_t) (brg & 0x00FF);

    TXSTA1bits.SYNC = 0; /* asincrono       */
    TXSTA1bits.TX9 = 0; /* 8 bits de datos */
    RCSTA1bits.RX9 = 0;
    RCSTA1bits.SPEN = 1; /* habilita modulo */
    TXSTA1bits.TXEN = 1;
    RCSTA1bits.CREN = 1;

    PIE1bits.RCIE = 1;
    PIR1bits.RCIF = 0;
}

void UART2_Init(unsigned long Baud) {
    uint16_t brg;
    if (Baud == 0UL) return;
    /* division con redondeo al entero mas cercano */
    brg = (uint16_t) ((((_XTAL_FREQ / 4UL) + (Baud / 2UL)) / Baud) - 1UL);
    /* RC6/RC7 deben salir de modo analogico (ANSELx arranca en analogico) */
    ANSELDbits.ANSD6 = 0;
    ANSELDbits.ANSD7 = 0;
    TRISDbits.TRISD6 = 0; /* TX2 salida  */
    TRISDbits.TRISD7 = 1; /* RX2 entrada */

    RCSTA2 = 0x00;
    TXSTA2 = 0x00;
    BAUDCON2 = 0x00;

    BAUDCON2bits.BRG16 = 1;
    TXSTA2bits.BRGH = 1;
    SPBRGH2 = (uint8_t) (brg >> 8);
    SPBRG2 = (uint8_t) (brg & 0x00FF);

    TXSTA2bits.SYNC = 0;
    TXSTA2bits.TX9 = 0;
    RCSTA2bits.RX9 = 0;
    RCSTA2bits.SPEN = 1;
    TXSTA2bits.TXEN = 1;
    RCSTA2bits.CREN = 1;

    PIE3bits.RC2IE = 1;
    PIR3bits.RC2IF = 0;
}

void UART1_Write(uint8_t dato) {
    uint16_t g = 0;
    while (!PIR1bits.TX1IF && ++g) {
        CLRWDT();
    } /* espera que TXREG este libre */
    TXREG1 = dato;
    g = 0;
    while (!PIR1bits.TX1IF && ++g) {
        CLRWDT();
    } /* el byte ya paso de TXREG al TSR */
    g = 0;
    while (!TXSTA1bits.TRMT && ++g) {
        CLRWDT();
    } /* el TSR termino de transmitirlo */
}

void UART2_Write(uint8_t dato) {
    uint16_t g = 0;
    while (!PIR3bits.TX2IF && ++g) {
        CLRWDT();
    } /* espera que TXREG este libre */
    TXREG2 = dato;
    g = 0;
    while (!PIR3bits.TX2IF && ++g) {
        CLRWDT();
    } /* el byte ya paso de TXREG al TSR */
    g = 0;
    while (!TXSTA2bits.TRMT && ++g) {
        CLRWDT();
    } /* el TSR termino de transmitirlo */
}

void UART1_WriteString(const char *cad) {
    while (*cad) UART1_Write((uint8_t) * cad++);
}

void UART2_WriteString(const char *cad) {
    while (*cad) UART2_Write((uint8_t) * cad++);
}

/* Posicion donde empieza 'patron', o -1 */
int8_t IndexOf_Str(const uint8_t *buf, char *patron) {
    uint8_t lp = 0, i, j;
    uint8_t len = Texto_Length((unsigned char *)buf);
    while (patron[lp]) lp++;
    if (lp == 0 || lp > len) return -1;

    for (i = 0; i <= (uint8_t) (len - lp); i++) {
        for (j = 0; j < lp; j++) {
            if (buf[i + j] != (uint8_t) patron[j]) break;
        }
        if (j == lp) return (int8_t) i;
    }
    return -1;
}

uint8_t Texto_Length(unsigned char *texto) {
    uint8_t contador = 0;
    while (contador < 200 && *texto++) contador++;
    return contador;
}

