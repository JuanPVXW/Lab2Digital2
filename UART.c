/*
 * File:   UART.c
 * Author: acer
 *
 * Created on 27 de julio de 2021, 11:58 AM
 */
//****DirectivasCompliador*****//
#define _XTAL_FREQ 4000000
//*****IMPORTACION DE LIBRERIAS******
#include <xc.h>
#include <stdint.h>
#include <stdio.h>
#include <pic16f887.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
uint8_t contador = 0;       /*Declaración variables*/
float v1 = 0;
float v2 = 0;
float v3 = 0;
char s[20];
#include "UARTx.h"
void Envio_caracter(char caracter)
{
    while(!TXIF);       //Espera que envie dato TXIF = 1 siempre
    TXREG = caracter;   //Carga el caracter a TXREG y envía 
    return; 
}
void cadena_caracteres(char st[])
{
    int i = 0;          //i igual 0 posicion 
    while (st[i] !=0)   //revisar la posicion de valor de i 
    {
        Envio_caracter(st[i]); //enviar caracter de esa posicion 
        i++;                //incrementar variable para pasar a otra posicion 
    }                       //de cadena 
    return;
}
void valorsensores(void)
{
    sprintf(s, "\r valorS1=%.2f \n", v1);
    cadena_caracteres(s);
    
    sprintf(s, "\n valorS2=%.2f\n", v2);
    cadena_caracteres(s);
    
    sprintf(s, "\n contador=%d", contador);
    cadena_caracteres(s);
}

