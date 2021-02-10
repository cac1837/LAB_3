/*
 * File:   LCD.c
 * Author: Katherine Caceros
 *
 * Created on 9 de febrero de 2021, 09:09 PM
 */


#include "LCD.h"
#include <xc.h>
#include <stdint.h>
#define RS      PORTDbits.RD6
#define E       PORTDbits.RD7
#define TRISRS  TRISDbits.TRISD6
#define TRISEN  TRISDbits.TRISD7
#define POR     PORTB
#define PORTRI  TRISB


void inLCD (void){
    TRISRS   = 0;
    TRISEN   = 0;
    PORTRI   = 0;
    RS       = 0;
    E        = 0;
    POR      = 0;
    _delay_ms(50);
    LCDCON(0x02);
    LCDCON(0x38);
    LCDCON(0x0C);
    LCDCON(0x06);  
}

void _delay_ms (unsigned int dms){
    for (int i = 0; i<dms; i++){for(int j = 0; j<255; j++);}}

void LCDCON (char C){
    RS       = 0;
    POR      = C;
    LCDTIM();
    _delay_ms(2);}

void LCDTIM(void){
    E = 1;
    delay_us (5);
    E = 0;
    delay_us(5);}

void delay_us (unsigned int dms){for (int i = 0; i<dms; i++);}

void LCDPLAY (char D){
    RS       = 1;
    POR      = D;
    LCDTIM();
    delay_us(50);}

void LCDclear (void){
    LCDCON(0x01);
}

void LCDP (char *D){
    while (*D){
        LCDPLAY(*D);
        D++;
    }
}

void LCDPO (int x, int y, char *D){
    char POS;
    switch (y){
        case 1:
            POS = 0x80 + x;
            break;
        case 2:
            POS = 0xC0 + x;
            break;            
        default:
            POS = 0x80 + x;
            break;
    }
    LCDCON(POS);
    LCDP(D);   
}

void LCDVAL1 (int x, int Z){
    switch (Z){
        case 0:
            LCDPO (x,2,"0");
            break;
        case 1:
            LCDPO (x,2,"1");
            break;
        case 2:
            LCDPO (x,2,"2");
            break;
        case 3:
            LCDPO (x,2,"3");
            break;
        case 4:
            LCDPO (x,2,"4");
            break;
        case 5:
            LCDPO (x,2,"5");
            break;
        case 6:
            LCDPO (x,2,"6");
            break;
        case 7:
            LCDPO (x,2,"7");
            break;
        case 8:
            LCDPO (x,2,"8");
            break;
        case 9:
            LCDPO (x,2,"9");
            break;
        case 10:
            LCDPO (x,2,"A");
            break;
        case 11:
            LCDPO (x,2,"B");
            break;
        case 12:
            LCDPO (x,2,"C");
            break;
        case 13:
            LCDPO (x,2,"D");
            break;
        case 14:
            LCDPO (x,2,"E");
            break;
        case 15:
            LCDPO (x,2,"F");
            break;
        default:
            LCDPO (x,2,"0");
            break;
    }
}