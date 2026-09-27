#include "Kinco.h"
#include "Uarts.h"

Container Gasificado;

void Read_Cmd(unsigned char *Data) {
    int Index = IndexOf_Str((unsigned char*) Data, ":");
    if (Index == -1) return;

    // LIMPIAR VARIABLES
    M_ID = 0;
    M_Funcion = 0;
    M_Direccion = 0;
    M_Cantidad = 0;
    M_LR = 0;

    unsigned int alto, bajo, lrc = 0;

    // ID (posición 1-2 después de :)
    alto = HexToNum(Data[Index + 1]);
    bajo = HexToNum(Data[Index + 2]);
    M_ID = (char) ((alto << 4) | bajo);
    lrc += (alto << 4) | bajo;
    lrc &= 0xFF;

    // Función (posición 3-4)
    alto = HexToNum(Data[Index + 3]);
    bajo = HexToNum(Data[Index + 4]);
    M_Funcion = (char) ((alto << 4) | bajo);
    lrc += (alto << 4) | bajo;
    lrc &= 0xFF;

    // Dirección (posición 5-8) - 2 bytes
    alto = HexToNum(Data[Index + 5]);
    bajo = HexToNum(Data[Index + 6]);
    lrc += (alto << 4) | bajo;
    lrc &= 0xFF;
    M_Direccion = ((alto << 4) | bajo) << 8;

    alto = HexToNum(Data[Index + 7]);
    bajo = HexToNum(Data[Index + 8]);
    lrc += (alto << 4) | bajo;
    lrc &= 0xFF;
    M_Direccion |= (alto << 4) | bajo;

    // Cantidad (posición 9-12) - 2 bytes
    alto = HexToNum(Data[Index + 9]);
    bajo = HexToNum(Data[Index + 10]);
    lrc += (alto << 4) | bajo;
    lrc &= 0xFF;
    M_Cantidad = ((alto << 4) | bajo) << 8;

    alto = HexToNum(Data[Index + 11]);
    bajo = HexToNum(Data[Index + 12]);
    lrc += (alto << 4) | bajo;
    lrc &= 0xFF;
    M_Cantidad |= (alto << 4) | bajo;

    // LRC recibido (posición 13-14)
    alto = HexToNum(Data[Index + 13]);
    bajo = HexToNum(Data[Index + 14]);
    M_LR = (char) ((alto << 4) | bajo);

    // Complemento a 2 del LRC calculado
    lrc = (unsigned char) (~lrc + 1);

    if (M_ID == 1) {
        if (M_LR == lrc) {
            switch (M_Funcion) {
                case F_Read_Bit://UART1_Print("X0 "); 
                    Read_X0(M_Direccion + 1, M_Cantidad);
                    break;
                case F_Read_Byte: //UART1_Print("X4 ");
                    Read_X4(M_Direccion + 1, M_Cantidad);
                    break;
                case F_Write_Bit:
                    Write_X0(M_Direccion + 1, M_Cantidad);
                    break;
                case F_Write_Byte: Write_X4(M_Direccion + 1, M_Cantidad);
                    break;
            }
        }
    }
}

void Read_X0(unsigned int Direccion, unsigned int Cantidad) {
    unsigned char data = 0;

    for (unsigned int i = 0; i < Cantidad; i++) {
        unsigned char bit = Leer_1Bit(Direccion + i);
        data |= (unsigned char) (bit << i);
    }

    unsigned char nb = (unsigned char) ((Cantidad + 7) / 8);
    unsigned char lrc = 0;
    lrc += (unsigned char) M_ID;
    lrc += 0x01;
    lrc += nb;
    lrc += data;
    lrc = (unsigned char) (~lrc + 1);

    sprintf(txt, ":%02X01%02X%02X%02X\r\n", (unsigned char) M_ID, nb, data, lrc);
    UART2_WriteString(txt);
}

void Read_X4(unsigned int Direccion, unsigned int Cantidad) {
    unsigned char nb = (unsigned char) (Cantidad * 2);
    unsigned char lrc = 0;

    lrc += (unsigned char) M_ID;
    lrc += 0x03;
    lrc += nb;
    /* Encabezado */
    __delay_ms(5); /* <- dar tiempo a la Kinco antes de responder */
    sprintf(txt, ":%02X03%02X", (unsigned char) M_ID, nb);
    UART2_WriteString(txt);

    /* Un registro a la vez */
    for (unsigned int i = 0; i < Cantidad; i++) {
        unsigned int val = Leer_1Byte(Direccion + i);
        unsigned char hi = (unsigned char) ((val >> 8) & 0xFF);
        unsigned char lo = (unsigned char) (val & 0xFF);
        lrc += hi;
        lrc += lo;
        sprintf(txt, "%04X", val);
        UART2_WriteString(txt);
    }

    lrc = (unsigned char) (~lrc + 1);
    sprintf(txt, "%02X\r\n", lrc);
    UART2_WriteString(txt);
}

void Write_X0(unsigned int Direccion, unsigned int Valor) {

    if (Direccion < Len_Control + 1) {
        if (Valor == 0xFF00) {
            Gasificado.Control[Direccion] = 1;
        }
        if (Valor == 0x0000) {
            Gasificado.Control[Direccion] = 0;
        }
    }

    /* Echo: reenviar la misma trama recibida */
    for (int i = 0; i < 15; i++) UART2_Write(Buffer_U2[i]);
    UART2_Write(0x0D);
    UART2_Write(0x0A);
}

void Write_X4(unsigned int Direccion, unsigned int Valor) {
    if (Direccion < Len_Variable + 1) {
        Gasificado.Variables[Direccion] = Valor;
    }

    /* Echo: reenviar la misma trama recibida */
    for (int i = 0; i < 15; i++) UART2_Write(Buffer_U2[i]);
    UART2_Write(0x0D);
    UART2_Write(0x0A);
}

unsigned char Leer_1Bit(unsigned int Direccion) {
    if (Direccion < Len_Control + 1) {
        return Gasificado.Control[Direccion];
    }
    return 0;
}

unsigned int Leer_1Byte(unsigned int Direccion) {
    if (Direccion < Len_Variable + 1) {
        return Gasificado.Variables[Direccion];
    }
    return 0;
}

unsigned char HexToNum(char c) {
    if (c >= '0' && c <= '9') return (unsigned char) (c - '0');
    if (c >= 'A' && c <= 'F') return (unsigned char) (c - 'A' + 10);
    if (c >= 'a' && c <= 'f') return (unsigned char) (c - 'a' + 10);
    return 0;
}