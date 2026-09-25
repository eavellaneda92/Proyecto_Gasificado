/* 
 * File:   ADC.h
 * Author: LENOVO
 *
 * Created on 24 de septiembre de 2026, 02:04 PM
 */

#ifndef ADC_H
#define	ADC_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "Config.h"

#define ADC_CANAL_MAX     5u
#define ADC_ERROR         0xFFFFu   // Canal fuera de rango
    void ADC_Init(void);
    uint16_t ADC_Leer(uint8_t canal); // Devuelve 0..1023 (10 bits)

    uint8_t Paso_ADC = 0;
    uint8_t Count_ADC = 0;

    void Proceso_ADC(void);

    uint16_t Current1 = 0;
    uint16_t Current2 = 0;
    int16_t Temp1 = 0;
    int16_t Temp2 = 0;

#define NTC_ABIERTO     ((int16_t)-9990)   // Sensor desconectado / cable abierto
#define NTC_CORTO       ((int16_t) 9990)   // Sensor en corto
#define NTC_BAJO_RANGO  ((int16_t)-9980)   // Por debajo de -40 C
#define NTC_SOBRE_RANGO ((int16_t) 9980)   // Por encima de +45 C

    // Convierte una suma de 16 lecturas ADC (0..16368) a decimas de grado C
    int16_t Ntc_ConvertirX16(uint16_t adc_x16);

    // Lee el canal (16 muestras) y devuelve decimas de grado C (ej. 235 = 23.5 C)
    int16_t Ntc_LeerTemperatura(uint8_t canal);

#ifdef	__cplusplus
}
#endif

#endif	/* ADC_H */

