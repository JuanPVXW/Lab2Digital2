/*
 * File:   LCDxw.c
 * Author: acer
 *
 * Created on 22 de julio de 2021, 07:44 PM
 */

//LCD Functions Developed by electroSome

#include "EjLCD.h"

void Lcd_Cmd(char a){       //se cargan los comandos a la lcd para inicializacion asi como para limpiar la lcd 
	PORTBbits.RB7 =0;       //ponemos en 0 para poder escribir valores a la lcd 
	PORTD = a;          // cargamos lo que se desea escribir a la lcd 
    PORTBbits.RB6 = 1;     //seteamos en uno la lcd para que lea los valores cargados              
    __delay_ms(4);      
    PORTBbits.RB6 = 0;    //regresamos a 0 esperando leer algo mas               
}


void Lcd_Clear(void){ //se limpia la lcd utilizando los comandos de la misma 
	Lcd_Cmd(0);
	Lcd_Cmd(1);
}

void Lcd_Set_Cursor(char a, char b) //funcion para setear el cursor de la lcd 
{
	char temp,z,y; //variables char 
	if(a == 1)
	{
	  temp = 0x80 + b - 1; //calculo de la posicion del cursor 
		//z = temp>>8;
		//y = temp & 0x0F;
		Lcd_Cmd(temp); //mandamos los valores al lcd para leer la posicion del cursor 
		
	}
	else if(a == 2)
	{
		temp = 0xC0 + b - 1; //calculo de la posicion del cursor 
		//z = temp>>8;
		//y = temp & 0x0F;
		Lcd_Cmd(temp); //mandamos los valores al lcd para leer la posicion del cursor 
		
	}
}

void Lcd_Write_Char(char a){    
    PORTBbits.RB7 = 1; //mantenemos en 1 la lcd para que pueda leer el char que le mandemos           
    PORTD = a;         //mandamos el valor a la lcd        
    PORTBbits.RB6 = 1;   //leemos el valor del char                    
    __delay_us(40);              
    PORTBbits.RB6 = 0;    //esperamos que otro valor ingrese a la lcd                  
}

void Lcd_Write_String(char *a)
{
	int i;// inicialiamos la variable i
	for(i=0;a[i]!='\0';i++) //for que usamos para leer el strings dependiendo del tamano 
	    Lcd_Write_Char(a[i]); //mandamos el char a la lcd 
}

void Lcd_Init(){           
    
    PORTBbits.RB7 = 0;   //podemos el lcd en 0 para poder leer los comandos                
       
    Lcd_Cmd(0x00);      // utilizamos el conamdo 0x00       
    __delay_ms(15);         
    
   
    Lcd_Cmd(0x30);       // utilizamos el conamdo 0x30        
    __delay_us(160);        
    
    Lcd_Cmd(0x38);    // utilizamos el conamdo 0x38  
    Lcd_Cmd(0x01);     // utilizamos el conamdo 0x01         
    Lcd_Cmd(0x06);     // utilizamos el conamdo 0x06         
    Lcd_Cmd(0x0F);      // utilizamos el conamdo 0x0F         
   
}

void Lcd_Shift_Right(void) {
    Lcd_Cmd(0x01);
    Lcd_Cmd(0x0C);
}

void Lcd_Shift_Left(void) {
    Lcd_Cmd(0x01);
    Lcd_Cmd(0x08);
}
