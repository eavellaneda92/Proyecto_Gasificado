#include "ADC.h"

void ADC_Init(void)
{
    // Pines como entrada
    TRISAbits.TRISA0 = 1;
    TRISAbits.TRISA1 = 1;
    TRISAbits.TRISA2 = 1;
    TRISAbits.TRISA3 = 1;
    TRISAbits.TRISA5 = 1;
    TRISEbits.TRISE0 = 1;
 
    // Pines como analogicos (solo estos bits; el resto no se toca)
    ANSELAbits.ANSA0 = 1;
    ANSELAbits.ANSA1 = 1;
    ANSELAbits.ANSA2 = 1;
    ANSELAbits.ANSA3 = 1;
    ANSELAbits.ANSA5 = 1;
    ANSELEbits.ANSE0 = 1;
 
    // ADCON1: TRIGSEL=0, PVCFG=00 (VDD), NVCFG=00 (VSS)
    ADCON1 = 0x00;
 
    // ADCON2: ADFM=1 (derecha), ACQT=010 (4 TAD), ADCS=101 (Fosc/16)
    ADCON2 = 0b10010101;
 
    // ADCON0: canal AN0, ADC encendido
    ADCON0 = 0x00;
    ADCON0bits.ADON = 1;
 
    __delay_us(10);                     // Estabilizacion del modulo
}
 
// ============================================================
//  ADC_Leer
//  canal: 0..5  -> devuelve 0..1023
//  canal invalido -> devuelve ADC_ERROR (0xFFFF)
// ============================================================
uint16_t ADC_Leer(uint8_t canal)
{
    if (canal > ADC_CANAL_MAX)
        return ADC_ERROR;
 
    ADCON0bits.CHS = canal;             // Seleccionar canal
    ADCON0bits.GO_nDONE = 1;            // Inicia: adquisicion (4 TAD) + conversion
    while (ADCON0bits.GO_nDONE);        // ~15 us en total
 
    return ((uint16_t)ADRESH << 8) | ADRESL;
}

void Proceso_ADC(void){
    Count_ADC++;
    if(Count_ADC >5){
        /*LEE SENSOR CORRIENTE*/
        if(Paso_ADC == 0){
            Current1 = (uint16_t)ADC_Leer(0);
        }
        /*LEE SENSOR CORRIENTE*/
        if(Paso_ADC == 1){
            Current1 = (uint16_t)ADC_Leer(1);
        }
        /*LEE SENSOR TEMPERATURA*/
        if(Paso_ADC == 2){
            Temp1 = Ntc_LeerTemperatura(2);
        }
        /*LEE SENSOR TEMPERATURA*/
        if(Paso_ADC == 3){
            Temp2 = Ntc_LeerTemperatura(3);
        }
        Paso_ADC++;
        if(Paso_ADC > 3) Paso_ADC = 0;
        Count_ADC = 0;
    }
}


// ============================================================
#define NTC_PUNTOS  41u
 
static const int16_t ntcTemp[NTC_PUNTOS] = {   // Decimas de grado C
    -400, -350, -300, -250, -200, -150, -120, -100,
     -80,  -60,  -40,  -20,    0,   20,   40,   60,
      80,  100,  120,  140,  160,  180,  200,  220,
     240,  260,  280,  300,  320,  340,  360,  380,
     400,  420,  440,  450,  500,  550,  600,  650,
     700
};
 
static const uint16_t ntcAdc[NTC_PUNTOS] = {   // Cuentas ADC x16
    15634, 15411, 15134, 14798, 14398, 13922, 13603, 13376,  // -40 .. -10
    13137, 12888, 12628, 12358, 12080, 11793, 11497, 11196,  //  -8 ..   6
    10826, 10576, 10261,  9942,  9621,  9301,  8980,  8662,  //   8 ..  22 (*)
     8343,  8028,  7719,  7416,  7118,  6824,  6537,  6261,  //  24 ..  38
     5989,  5726,  5471,  5346,                              //  40 ..  45
     4758,  4224,  3743,  3314,  2932                        //  50 ..  70 (**)
};
// (**) 50..70 C NO estan en la tabla del fabricante. Se extrapolaron con un
//      ajuste Steinhart-Hart sobre los puntos de 20..45 C (residuo < 0.02 C):
//        50 C = 820 ohm   55 C = 696 ohm   60 C = 593 ohm
//        65 C = 508 ohm   70 C = 436 ohm
//      Error de interpolacion con pasos de 5 C: < 0.08 C.
// (*) 22 C: la tabla indica 2347 ohm, que no sigue la curva (20 C = 2431,
//     24 C = 2079). Se usa 2248 ohm (media geometrica de los vecinos).
//     Con el valor original seria 8837.
 
// Limites de falla (en cuentas x16)
#define ADC_X16_ABIERTO   (16u * 1010u)   // R > ~150 k -> cable abierto
#define ADC_X16_CORTO     (16u * 20u)     // R < ~40 ohm -> corto a GND
 
// ============================================================
//  Ntc_ConvertirX16
//  Busqueda del tramo + interpolacion lineal en enteros
// ============================================================
int16_t Ntc_ConvertirX16(uint16_t adc)
{
    if (adc >= ADC_X16_ABIERTO) return NTC_ABIERTO;
    if (adc <= ADC_X16_CORTO)   return NTC_CORTO;
    if (adc >  ntcAdc[0])               return NTC_BAJO_RANGO;
    if (adc <  ntcAdc[NTC_PUNTOS - 1u]) return NTC_SOBRE_RANGO;
 
    uint8_t i = 1;
    while (adc < ntcAdc[i])      // Avanzar hasta el tramo [i-1, i]
        i++;
 
    // T = T1 + (T2 - T1) * (A1 - adc) / (A1 - A2)
    int32_t dT   = (int32_t)(ntcTemp[i] - ntcTemp[i - 1u]);
    int32_t dA   = (int32_t)ntcAdc[i - 1u] - (int32_t)ntcAdc[i];
    int32_t num  = dT * ((int32_t)ntcAdc[i - 1u] - (int32_t)adc);
 
    return (int16_t)(ntcTemp[i - 1u] + (int16_t)((num + dA / 2) / dA));  // Redondeo
}
 
// ============================================================
//  Ntc_LeerTemperatura
//  Suma 16 lecturas (promedio) y convierte
// ============================================================
int16_t Ntc_LeerTemperatura(uint8_t canal)
{
    uint16_t suma = 0;
 
    for (uint8_t k = 0; k < 16u; k++)
    {
        uint16_t v = ADC_Leer(canal);
        if (v == ADC_ERROR)
            return NTC_ABIERTO;
        suma += v;                   // Max 16 * 1023 = 16368, cabe en 16 bits
    }
    return Ntc_ConvertirX16(suma);
}