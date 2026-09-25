/* 
 * File:   Uart.h
 * Author: Eus
 *
 * Created on September 22, 2026, 10:22 PM
 */

#ifndef UART_H
#define	UART_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "Config.h"

#define UART_BRG_H   0x01
#define UART_BRG_L   0xA0
#define BUFFER_SIZE  150u
    
#define MOD485 LATBbits.LATB6

    volatile uint8_t Buffer[BUFFER_SIZE];
    volatile uint8_t BufferIndex = 0;
    volatile bool FlagBuffer = false; // true = trama completa (50 ms sin datos)
    volatile bool FlagOverflow = false; // true = llegaron mas de 150 bytes

    void UART_Init(void);
    void UART_Write(uint8_t dato);
    void UART_Print(const char *cad);
    
    void Get_Comando(char *Data);
    int Texto_Length(char *texto);
    int Texto_IndexOf(char *texto, char *valor);
    
    void Imprime_Estado(void);


#ifdef	__cplusplus
}
#endif

#endif	/* UART_H */

