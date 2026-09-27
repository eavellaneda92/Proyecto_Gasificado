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
#define S_HEATER 2
#define S_EVAPORADOR 3
#define S_VENTILADOR 4
#define S_LIQUIDO 5
#define S_GAS 6
#define S_DAMPER_OPEN 7
#define S_DAMPER_CLOSE 8
#define S_DOWNLOAD 9
#define S_STOP 10
#define S_VENTILA 11

    /*VARIABLES*/
#define V_SP_ETILENO 1
#define V_ETILENO 2
#define V_SP_TEMPERATURA 3
#define V_TEMPERATURA 4
#define V_SP_MINUTOS 5
#define V_MINUTOS 6
#define V_SP_SEGUNDOS 7
#define V_SEGUNDOS 8
    
#define Len_Control 11
#define Len_Variable 8

    /*Plano de MODBUS*/
#define F_Read_Bit  1
#define F_Read_Byte 3
#define F_Write_Bit 5
#define F_Write_Byte 6

    /*CREACION DE OBJETO*/
    typedef struct {
        unsigned char Control[Len_Control + 1];
        unsigned int Variables[Len_Variable + 1];
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

