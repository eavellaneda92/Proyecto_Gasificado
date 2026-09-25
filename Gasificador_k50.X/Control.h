/* 
 * File:   Control.h
 * Author: LENOVO
 *
 * Created on 24 de septiembre de 2026, 05:00 PM
 */

#ifndef CONTROL_H
#define	CONTROL_H

#ifdef	__cplusplus
extern "C" {
#endif
    
#include "Config.h"
#include "Timers.h"

    const unsigned char Get_PPM[5] = {0x01, 0x20, 0x00, 0x39, 0xc0};
    uint8_t Count_Control = 0;
    uint8_t Paso_Control = 0;
    uint8_t Flag_Sensor = 0;
    uint8_t Muestras_Sensor = 0;
    uint8_t Wait_Sensor = 0;
    uint16_t PPM_Muestra = 0;
    uint16_t PPM_Old = 0;
    int16_t PPM = 0;
    void Proceso_Control(void);
    void Pedir_PPM(void);
    float Cargar_PPM(void);
    
    /*INICIO DE PARAMETROS*/
    uint8_t Arranque_Count= 0;
    uint8_t Arranque_Flag= 0;
    void Proceso_Arranque(void);
    
#define Evaporador RELAY1
#define Ventilador RELAY2
#define Heater1 RELAY3
#define Heater2 RELAY4
#define Damper_Open RELAY5
#define Damper_Close RELAY6
#define Valve_Gas_Open RELAY7
#define Valve_Gas_Close RELAY8
#define Valve_Liq_Open RELAY9
#define Valve_Liq_Close RELAY10
    
    void Armado_Cerrado(void);
    void Armado_Ventila(void);
    
    /*CONTROL PROCESO*/
    int16_t SP_Temp = 0; //Para control de 
    uint16_t SP_Ppm = 0; //Para SP de SO2
    uint8_t En_Sistem = 0; //Para encender = 0
    uint16_t SP_Minutos = 0; //Minutos
    
    //Variables globales
    uint8_t V_Minutos = 0;
    uint8_t V_Segundos = 0;
    
    /*PINES DE CONTROL*/
    uint8_t Channel_Heater = 0;//Selecciona que Heater se va activar
    uint16_t Tiempo_Damper = 0;//Tiempo de apertura y cierre
    uint16_t Tiempo_Iny_Liquido = 0;
    uint16_t Tiempo_Iny_Gas = 0;
    
    
    
    
    
#ifdef	__cplusplus
}
#endif

#endif	/* CONTROL_H */

