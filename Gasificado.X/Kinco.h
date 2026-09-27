/* 
 * File:   Kinco.h
 * Author: LENOVO
 *
 * Created on 27 de agosto de 2026, 02:25 PM
 */

#ifndef KINCO_H
#define	KINCO_H

#ifdef	__cplusplus
extern "C" {
#endif


    /*CONTROL*/
#define S_RUN 1
#define S_STOP 2
#define S_OFF 3
#define S_SAVE 4
#define S_FABRICA 5
#define S_HEATER 6
#define S_EVAPORADOR 7
#define S_EXTRACTOR 8
#define S_LIQUIDO 9
#define S_GAS 10
#define S_OPEN 11
#define S_CLOSE 12
#define S_STATUS 14

    /*VARIABLES*/
#define T_MINUTOS 18
#define T_HORA 17
#define SP_MINUTOS 16
#define SP_HORA 15
#define USDA4 14
#define USDA3 13
#define USDA2 12
#define USDA1 11
#define V_CO2 7
#define SP_CO2 6
#define V_HUMEDAD 5
#define SP_HUMEDAD 4
#define V_RETURN 3
#define V_SUPPLY 2
#define SP_TEMP 1

#define SP_PPM 1
#define V_PPM 2
#define SP_TEMP 3
#define V_TEMP 4
#define SP_MIN 5
#define V_MIN 6
#define SP_SEG 7
#define V_SEG 8

    /*Plano de MODBUS*/
#define F_Read_Bit  1
#define F_Read_Byte 3
#define F_Write_Bit 5
#define F_Write_Byte 6

    /*CREACION DE OBJETO*/
    typedef struct {
        unsigned char Control[15];
        unsigned int Variables[9];
    } Container;

    extern Container Gasificado;

    char M_ID = 0;
    char M_Funcion = 0;
    unsigned int M_Direccion = 0;
    unsigned int M_Cantidad = 0;
    unsigned char M_LR = 0;

    void Read_Cmd(unsigned char *Data);
    unsigned char HexToNum(char c);
    void Read_X0(unsigned int Direccion, unsigned int Cantidad);
    void Read_X4(unsigned int Direccion, unsigned int Cantidad);
    void Write_X0(unsigned int Direccion, unsigned int Valor);
    void Write_X4(unsigned int Direccion, unsigned int Valor);
    unsigned char Leer_1Bit(unsigned int Direccion);
    unsigned int Leer_1Byte(unsigned int Direccion);

#ifdef	__cplusplus
}
#endif

#endif	/* KINCO_H */

