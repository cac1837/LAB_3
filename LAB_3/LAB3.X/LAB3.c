/*
 * File:   LAB3.c
 * Author: Katherine Caceros
 *
 * Created on 5 de febrero de 2021, 09:31 AM
 */

// CONFIG1
#pragma config FOSC = INTRC_CLKOUT// Oscillator Selection bits (INTOSC oscillator: CLKOUT function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = ON       // RE3/MCLR pin function select bit (RE3/MCLR pin function is MCLR)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = ON       // Brown Out Reset Selection bits (BOR enabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>
#include <stdint.h>
#define _XTAL_FREQ 4000000
#include "ADC_3.h" // LIBRERIAS A UTILIZAR
#include "LCD.h"

//------------------------------------------------------------------------------
//                       Declaracion de variables  
//------------------------------------------------------------------------------
uint16_t b;             //bandera propia para el ADC //Es un entero corto sin signo,variable en la que guardare valor
uint8_t camb1;        //valor convertido del ADC1
int camb2;            //valor convertido del ADC2
int send1;              //nibble 1 del ADC1 
int send2;              //nibble 2 del ADC1
int send3;              //nibble 1 del ADC2
int send4;              //nibble 2 del ADC2


//------------------------------------------------------------------------------
//                         Declaracion de las funciones
//------------------------------------------------------------------------------

void change (void);
void delay_ms (unsigned int dms);
void separate(void);
void Setup(void);


//------------------------------------------------------------------------------
//                          Interrupcion
//------------------------------------------------------------------------------
void __interrupt() isr(void){
    if (PIR1bits.ADIF == 1){       //interrupcion del ADC para cambio de canal y lectura //habilita interrupcion de conversion a/d
        if (b == 1){b = 0;}
        else if (b == 0){b = 1;}
        else {b = 1;}
        PIR1bits.ADIF = 0;}         // Es una bandera de interrupcion 0 converion A/D no completada
}
//******************************************************************************
//MAIN
//******************************************************************************
void main(void) {
    Setup();          // seteo inicial
    inLCD();         // inicio de la LCD
    LCDclear();      // limpieza de la LCD
    LCDPO(0,1,"Kate Caceros");     //Mensaje inicial, mostrara en fila 0, columna 1
    LCDPO(5,2,"18307");            //Mostrara en fila 5, columna 2 
    delay_ms(500);
    LCDPO(0,1," ADC1 ADC2 CONT");   //Mensaje permanente
    LCDPO(0,2,"                "); // mostrar el valor del ADC
    BACK:
    change();      //Se cambia el ADC
    separate();   //separacion en nibbles del ADC
    LCDVAL1 (3, send1); //envio de nibbles para LCD
    LCDVAL1 (2, send2);
    LCDVAL1 (8, send3);
    LCDVAL1 (7, send4);
    goto BACK;
}
void Setup(void){         // seteo de entradas analogicas y de interrupciones 
    TRISA = 0b00000111;     // puerto A se coloco como entrada con interrupcion 
    ANSEL = 0b00000111;
    ADC(2);             // ADC desde libreria
    delay_ms(50);
    b = 1;
   
    INTCONbits.PEIE = 1; // habilitacion perifica de interrupciones
    PIE1bits.ADIE = 1;
    PIR1bits.ADIF = 0;
    INTCONbits.GIE = 1; //habilitacion global de interrupciones
    ADCON0bits.GO_DONE = 1;
}

//------------------------------------------------------------------------------
//                              FUNCIONES
//------------------------------------------------------------------------------
void change (void){     //cambio de ADC entre canales 
    if (b == 1){        // cuando B=1 canal 1 cambia ADC a 2
        camb1 = ADRESH;
        ADC(2);
        delay_ms(50);
        ADCON0bits.GO_DONE = 1;
    }
    else if (b == 0){      // cuando B=0 canal 1 cambia ADC a 1
        camb2 = ADRESH;
        ADC(1);
        delay_ms(50);
        ADCON0bits.GO_DONE = 1;
    }
}

void delay_ms (unsigned int dms){
    for (int i = 0; i<dms; i++){   // for repetira 
        for(int j = 0; j<255; j++);
    }
}

void separate (void){         //separacion de nibbles entre mas significativo y menos significativo 
    send1 = camb1 & 0b00001111;
    send2 = camb1 & 0b11110000;
    send2 = send2 >> 4;
    send3 = camb2 & 0b00001111;
    send4 = camb2 & 0b11110000;
    send4 = send4 >> 4;
}
