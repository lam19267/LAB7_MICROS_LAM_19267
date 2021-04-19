/*
 * File:   lab7.c
 * Author: gelp0
 *
 * Created on 13 de abril de 2021, 01:56 PM
 * 
 * ; Laboratorio 05
 * Archivo:      lab4.S
 * Dispositivo:	PIC16F887
 * Autor:	Guillermo Lam
 * Compilador:	pic-as (v2.30), MPLABX V5.45
 * 
 * Programa:	contador con 7 segmentos
 * Hardware:	pullup push buttons
 *
 * Creado: 13 abr, 2021
 * Ultima modificacion: 18 abr, 2021
 */

//CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT        // Oscillator Selection bits 
#pragma config WDTE = OFF       // Watchdog Timer Enable bit 
#pragma config PWRTE = ON       // Power-up Timer Enable bit (PWRT enabled)
#pragma config MCLRE = OFF      // RE3/MCLR pin function select bit 
#pragma config CP = OFF          // Code Protection bit 
#pragma config CPD = OFF         // Data Code Protection bit 
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit 
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit 
#pragma config LVP = ON        // Low Voltage Programming Enable bit 

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit 
#pragma config WRT = OFF       // Flash Program Memory Self Write Enable bits 

#include <xc.h>
#include <stdint.h>

//VARIABLES A USAR
char sietes[10] = {0b00111111, 0b00000110, 0b01011011, 0b01001111, 0b01100110,
    0b01101101, 0b01111101, 0b00000111, 0b01111111, 0b01101111}; 
//TABLA 7 SEGMENTOS
char centenas;  //variable de centenas
char decenas;   //variable de decenas
char unidades;  //variable de unidades
int  banderas;  //variable de banderas
char num;       //variable de num
char resultado; //variable de resultado
char resultados;//variable deresultados


void config(void);//funcion de setup
char division(void);//funcion de divisiom

void __interrupt() isr(void){
    if(T0IF == 1){   
        PORTEbits.RE0 = 0;      // Apago el transistor 2
        PORTEbits.RE1 = 1;      // Prendo transistor 0
        PORTD = (sietes[decenas]);  // Ingreso de centenas
        banderas = 1;     // Prendo una flag
        if (banderas == 1){
            PORTEbits.RE1 = 0;  // Prendo un tnsistor y apgo el otro
            PORTEbits.RE2 = 1;
            PORTD = (sietes[centenas]);   // Despliego decenas
            banderas = 2;}
        if (banderas == 2){
            PORTEbits.RE2 = 0;      // Prendo un transistor y apago otro
            PORTEbits.RE0 = 1;
            PORTD = (sietes[unidades]);  // Muevo unidades a un dispay
            banderas = 0;}
        INTCONbits.T0IF = 0;    // Limpio la interrupcion del timer0
        TMR0 = 255;}
    
    if (RBIF == 1){
        if (PORTBbits.RB0 == 0){
            PORTC = PORTC + 1;}
        if  (PORTBbits.RB1 == 0){
            PORTC = PORTC - 1;}
        INTCONbits.RBIF = 0;}}

void main(void) {
    config();
    while(1)
        division();
    {}}

void config(void){
    ANSEL = 0x00;
    ANSELH = 0x00;
  
    TRISBbits.TRISB0 = 1;
    TRISBbits.TRISB1 = 1;
    TRISC = 0x00;
    TRISD = 0x00;
    TRISE = 0x00;
    
    PORTB = 0x00;
    PORTC = 0x00;
    PORTD = 0x00;
    PORTE = 0x00;
    
    OSCCONbits.IRCF2 = 0;
    OSCCONbits.IRCF1 = 1;
    OSCCONbits.IRCF0 = 0;   
    OSCCONbits.SCS = 1;
    
    OPTION_REGbits.T0CS = 0;
    OPTION_REGbits.PSA = 0;
    OPTION_REGbits.PS2 = 1;
    OPTION_REGbits.PS1 = 1;
    OPTION_REGbits.PS0 = 1;
   
    OPTION_REGbits.nRBPU = 0;
    WPUB = 0b00000011;
    IOCBbits.IOCB0 = 1;
    IOCBbits.IOCB1 = 1;
    
    INTCONbits.GIE = 1;
    INTCONbits.RBIE = 1;
    INTCONbits.RBIF = 0;
    INTCONbits.T0IE = 1;
    INTCONbits.T0IF = 0;}

char division(void){
    num = PORTC;
    centenas = num/100;
    resultado = num%100;
    decenas = resultado/10;
    resultados = resultado%10;
    unidades = resultados;
}

