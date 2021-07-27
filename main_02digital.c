/*
 * File:   main_02digital.c
 * Author: acer
 *
 * Created on 22 de julio de 2021, 11:05 AM
 */
//Configuración PIC16F887
// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (RCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, RC on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // RE3/MCLR pin function select bit (RE3/MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)
// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Progra
//****DirectivasCompliador*****//
#define _XTAL_FREQ 4000000
//*****IMPORTACION DE LIBRERIAS******
#include <xc.h>
#include <stdint.h>
#include <stdio.h>
#include <pic16f887.h>
#include "ADC_2canales.h"      //Importar libreria creada para ADC
#include "LCD.h"
#include <math.h>
#include <stdlib.h>
#include "UARTx.h"
#include <string.h>
uint8_t contador = 0;       /*Declaración variables*/
int var1 = 0;
int var2 = 0;
int banderas = 0;
float v1 = 0;
float v2 = 0;
float v3 = 0;
char s[20];
//***********Prototipos de funciones************
void setup(void);           /*funcion principal */
void valorsensores(void);
void configuracionUART(void);
//**************Interrupciones**************
void __interrupt()isr(void) /*interrupciones*/
{
    recibir_valoresADC();   //Funcion de libreria_Reibe valores del Adresh
    alarmaADC();
}
//*********************************funcionPrincipal**********
void main(void) {
    setup();                /*funcion de configuracion principal*/
    __delay_us(50);
    ADCON0bits.GO = 1;
    unsigned int a;
    TRISD = 0x00;           //PUERTO LCD
    Lcd_Init();             //Inicialización para 8 bits
    //*********************************LoopPrincipal*********
    while(1)
    {
        v1 = (var1*0.0196);             //Operacion para rango 0-5v
        v2 = (var2*0.0196);             
        sprintf(s, "%.2fv", v1);        //convertir valor float a caracteres
        Lcd_Set_Cursor(2,1);            //Posicion LCD
        Lcd_Write_String(s);            //Hacia pantall
        //Lcd_Clear();
        Lcd_Set_Cursor(1,2);            //Posicion LCD
        Lcd_Write_String("S1");         //Caracter en pantalla
        Lcd_Set_Cursor(1,8);            //Posicion LCD
        Lcd_Write_String("S2");         //Mostrar S2 en pantalla
        Lcd_Set_Cursor(1,14);
        Lcd_Write_String("S3");
        
        sprintf(s, "%.2fv", v2);        //convertir valor float con 2 decimales
        Lcd_Set_Cursor(2,7);            //posicion cursor
        Lcd_Write_String(s);            //
        
        sprintf(s, "%d", contador);     //convertir valor entero a caracter
        Lcd_Set_Cursor(2,14);           //Posicion LCD
        Lcd_Write_String(s);
        __delay_ms(80);
        
        if (PIR1bits.RCIF)
        {
            char var3 = RCREG;
            if (var3 == 43)
            {
                contador++;
            }
            if (var3 == 45)
            {
                contador--;
            }
        }
        PORTA = contador;
        
        valorsensores();
        inicio_conversionADC();     //Funcion de libreria 
        //da inicio a la conversión ADC
    }
}
//*************Funciones************************
void setup(void)
{ 
    //*******configuración io entradas y salidas****
    ANSEL = 0b01100000;
    ANSELH = 0x00;     //Salidas Digitales
    
    TRISE = 0x03;  
    TRISA = 0x00;
    TRISB = 0x00;
    
    PORTA = 0x00; 
    //PORTC = 0x00; 
    PORTE = 0x00;
    //configuración del reloj (oscilador interno)
    OSCCONbits. IRCF2 = 1; 
    OSCCONbits. IRCF1 = 1;
    OSCCONbits. IRCF0 = 0;  //4Mhz
    OSCCONbits. SCS = 1;
    //configuración Tmr0

    //CONFIGURACIOS ADC
    ADCON1bits.ADFM = 0;        //justificado a la izquierda 
    ADCON1bits.VCFG0 = 0;       //Voltaje VDD referencia
    ADCON1bits.VCFG1 = 0;       //Voltaje Vss referencia
    
    ADCON0bits.ADCS = 1;        //ADC Clock FOSC = 4Mhz/8
    ADCON0bits.CHS = 5;         //Canal 5
    __delay_us(100);
    ADCON0bits.ADON = 1;        //Habiliar Modulo de ADC
    //*****ConfiguraciónUART TX Y RX ***********
    configuracionUART();
    //configuración interrupción
    INTCONbits. GIE = 1;        //Globales
    INTCONbits. RBIE = 1;       //IOCB
    INTCONbits. RBIF = 0;
    INTCONbits.PEIE = 1;        //Perifericas
    PIE1bits.ADIE = 1;          //ADC
    PIR1bits.ADIF = 0;
}
void configuracionUART(void)
{
    TXSTAbits.SYNC = 0;             //Modo asíncrono
    TXSTAbits.BRGH = 1;             //Seleccion BAUD RATE
    BAUDCTLbits.BRG16 = 0; 
    
    SPBRG = 25;                     //Registros para valor BAUD RATE
    SPBRGH = 0; 
    
    RCSTAbits.SPEN = 1;         //Habilitar puerto serial asíncrono
    RCSTAbits.RX9 = 0;
    RCSTAbits.CREN = 1;         //Habilitar recepción de datos 

    TXSTAbits.TXEN = 1;         //Habilitar transmision
}