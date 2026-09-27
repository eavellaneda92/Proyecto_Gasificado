/* 
 * File:   Uarts.h
 * Author: LENOVO
 *
 * Created on 30 de julio de 2026, 03:49 PM
 */

#ifndef UARTS_H
#define	UARTS_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "Config.h"

#define Len_Max_U1 100 //RS485
#define Len_Max_U2 200 //RS232

    unsigned char Buffer_U1[Len_Max_U1 + 1];
    unsigned char Length_U1 = 0;
    unsigned char Flag_U1 = 0;
    unsigned char Buffer_U2[Len_Max_U2 + 1];
    unsigned char Length_U2 = 0;
    unsigned char Flag_U2 = 0;
    
    void UART_Interrupt(void);
    void UART_Read(void);

    void UART1_Init(unsigned long Baud);
    void UART2_Init(unsigned long Baud);
    
    void UART1_Write(uint8_t dato);
    void UART2_Write(uint8_t dato);
    
    void UART1_WriteString(const char *cad);
    void UART2_WriteString(const char *cad);
    
    int8_t IndexOf_Str(const uint8_t *buf, char *patron);
    uint8_t Texto_Length(unsigned char *texto);
    

#ifdef	__cplusplus
}
#endif

#endif	/* UARTS_H */

