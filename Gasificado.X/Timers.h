/* 
 * File:   Timers.h
 * Author: LENOVO
 *
 * Created on 4 de agosto de 2026, 03:57 PM
 */

#ifndef TIMERS_H
#define	TIMERS_H

#ifdef	__cplusplus
extern "C" {
#endif


#include "Config.h"

#define TMR0_50MS  53036
#define TMR1_50MS  40536 

    void Timer_Interrupt(void);
    void Timer0_Init(void);
    void Timer1_Init(void);
    void Timer2_Init(void);
    void Timer_Proceso(void);
    char Flag_1000ms = 0;
    char Flag_100ms = 0;

#ifdef	__cplusplus
}
#endif

#endif	/* TIMERS_H */

