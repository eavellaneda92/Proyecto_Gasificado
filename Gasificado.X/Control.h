/* 
 * File:   Control.h
 * Author: Eus
 *
 * Created on 27 de septiembre de 2026, 12:03 PM
 */

#ifndef CONTROL_H
#define	CONTROL_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "Config.h"
    
    /*VARIABLES DE CONTROL*/
    void Consulta_Sensor(void);
    void Consulta_Relay(void);
    void Consulta_Server(void);
    
    /*VARIABLES*/
    uint8_t Paso_Control = 0;
    uint8_t Tiempo_Control = 0;
    
    uint16_t SP_PPM = 0;
    uint16_t SP_Agua = 0;
    int16_t PPM = 0;
    

#ifdef	__cplusplus
}
#endif

#endif	/* CONTROL_H */

