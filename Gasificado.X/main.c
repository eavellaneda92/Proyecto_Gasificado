/*
 * File:   main.c
 * Author: LENOVO
 *
 * Created on 27 de agosto de 2026, 01:31 PM
 */


#include "Config.h"
#include "Uarts.h"
#include "Timers.h"
#include "Leds.h"

void __interrupt() Interrupcion(void) {
    UART_Interrupt();
    Timer_Interrupt();
}

void main(void) {
    Pic_Clock_Init();
    Pic_Gpio_Init();
    Timer0_Init();
    Timer1_Init();
    Timer2_Init();
    UART1_Init(9600);
    UART2_Init(9600);
    INTCONbits.PEIE = 1;
    INTCONbits.GIE = 1;
    //Set_Led_Wait();
    Set_Led_Conectado();
    while (1) {
        UART_Read();
        Proceso_Led();
        CLRWDT();
    }
}