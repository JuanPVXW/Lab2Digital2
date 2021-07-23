/*
 * File:   ADC_2canales.c
 * Author: acer
 *
 * Created on 22 de julio de 2021, 11:09 AM
 */
#define _XTAL_FREQ 4000000
#include <stdint.h>
#include <pic16f887.h>
#include "EjADC.h"
int var1 = 0;
int var2 = 0;
void recibir_valoresADC(void)
{
    if (PIR1bits.ADIF == 1)
    {
        if(ADCON0bits.CHS == 5)
        {
            var1 = ADRESH;
        }
        else
        {
            var2 = ADRESH;
        }
    }
    PIR1bits.ADIF = 0;
}
void inicio_conversionADC(void)
{
    if(ADCON0bits.GO == 0)
    {
        if(ADCON0bits.CHS == 5)
        {
            ADCON0bits.CHS = 6;
        }
        else
        {
            ADCON0bits.CHS = 5;
        }
        //__delay_us(50);
    }
    __delay_us(50);
    ADCON0bits.GO = 1;
}
void alarmaADC(void)
{
    if(var1 >= var2)       //si el contador de ADC es mayor a contador IOCB 
    {
        RB0 = 1;            //Encender RC7 = LED
    }
    else
    {
        RB0 = 0;
    }
}
