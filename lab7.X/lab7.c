/*
 * File:   lab7.c
 * Author: gelp0
 *
 * Created on 13 de abril de 2021, 01:56 PM
 * 
 * Laboratorio 07
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


void config(void);//funcion de config
char division(void);//funcion de division

void __interrupt() isr(void){ //interrupciones
    if(T0IF == 1){   
        PORTEbits.RE0 = 0;  //se prenden y se apagan los transistores      
        PORTEbits.RE1 = 1;      
        PORTD = (sietes[decenas]);  //se depliega decenas en el disp
        banderas = 1;               //se le asigna un valor a banderas para
        if (banderas == 1){         //seguir prendiendo los otros displays
            PORTEbits.RE1 = 0;  
            PORTEbits.RE2 = 1;
            PORTD = (sietes[centenas]); //se despliega centenas en el disp   
            banderas = 2;}
        if (banderas == 2){
            PORTEbits.RE2 = 0;      
            PORTEbits.RE0 = 1;
            PORTD = (sietes[unidades]);  //se despliega unidades en el disp
            banderas = 0;}
        INTCONbits.T0IF = 0;    
        TMR0 = 255;}                    //se hace el reset del TMR0
    
    if (RBIF == 1){ //se hace la interrupcion del PORTB
        if (PORTBbits.RB0 == 0){
            PORTC = PORTC+1;} //se agrega una unidad al PORTC
        if  (PORTBbits.RB1 == 0){
            PORTC = PORTC-1;} //se quita una unidad al PORTC
        INTCONbits.RBIF = 0;}}

void main(void) {   //se pone el main
    config();       //se llama a la configuracion del TMR0 e interrupciones
    while(1)
        division();//se pone la division en el loop
    {}}

void config(void){
    ANSEL = 0x00;   //se ponen los pines digitales
    ANSELH = 0x00;
  
    TRISBbits.TRISB0 = 1;   //se activan los inputs del PORTB
    TRISBbits.TRISB1 = 1;
    TRISC = 0x00;           //se asignan las salidas
    TRISD = 0x00;
    TRISE = 0x00;
    
    PORTB = 0x00;           //se limpian los PORTS
    PORTC = 0x00;
    PORTD = 0x00;
    PORTE = 0x00;
    
    OSCCONbits.IRCF2 = 0;   //se configura el oscilador
    OSCCONbits.IRCF1 = 1;
    OSCCONbits.IRCF0 = 0;   
    OSCCONbits.SCS = 1;
    
    OPTION_REGbits.T0CS = 0;    //se configura el PreScaler del TMR0
    OPTION_REGbits.PSA = 0;
    OPTION_REGbits.PS2 = 1;
    OPTION_REGbits.PS1 = 1;
    OPTION_REGbits.PS0 = 1;
   
    OPTION_REGbits.nRBPU = 0;   //se ponen los bits del PORTB como pull-up
    WPUB = 0b00000011;
    IOCBbits.IOCB0 = 1;
    IOCBbits.IOCB1 = 1;
    
    INTCONbits.GIE = 1;         //se activan las interrupciones
    INTCONbits.RBIE = 1;
    INTCONbits.RBIF = 0;
    INTCONbits.T0IE = 1;
    INTCONbits.T0IF = 0;}

char division(void){            //se crea la funcion de division
    num = PORTC;                //se mueve el valor del PORTC a una variable
    centenas = num/100;         //se divide esta variable por 100 y se asigna
    resultado = num%100;        //se guarda el residuo en otra variable
    decenas = resultado/10;     //se divide esta variable por 10 y se asigna
    resultados = resultado%10;  //se guarda el residuo en otra variable
    unidades = resultados;      //se asigna esta variable
}

