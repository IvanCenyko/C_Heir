/*
 * File:   main.c
 * Author: Alejandro Heir
 *
 * Sistemas de Control e Instrumentacion Virtual
 * EEST N7 Taller Regional Quilmes
 *
 * Created on September 2022
 */

// CONFIG
#pragma config FOSC = HS   // Oscillator Selection bits (HS oscillator)
#pragma config WDTE = OFF  // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = ON  // Power-up Timer Enable bit (PWRT enabled)
#pragma config BOREN = OFF // Brown-out Reset Enable bit (BOR disabled)
#pragma config LVP = OFF   // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit (RB3 is digital I/O, HV on MCLR must be used for programming)
#pragma config CPD = OFF   // Data EEPROM Memory Code Protection bit (Data EEPROM code protection off)
#pragma config WRT = OFF   // Flash Program Memory Write Enable bits (Write protection off; all program memory may be written to by EECON control)
#pragma config CP = OFF    // Flash Program Memory Code Protection bit (Code protection off)

#include <xc.h>

// Constantes ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
const unsigned long CLOCK_FREQ = 4000000;
const unsigned char TMR0_INIT_VAL = 5;

// Variables globales ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
volatile unsigned int timerCont;

// Prototipos de funciones ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void gpioSetup(void);
void timer0Setup(void);
void __interrupt() ISR();

// Main ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void main(void)
{
    INTCONbits.GIE = 0; // deshabilito interrupciones globalmente

    gpioSetup();
    timer0Setup();

    INTCONbits.PEIE = 1; // habilito interrupciones por periferico
    INTCONbits.GIE = 1;  // habilito interrupciones globalmente

    while (1)
    {
        if (!(timerCont % 1000)) // Si paso un segundo
        {
            PORTDbits.RD0 = ~PORTDbits.RD0; // invierto estado del LED
        }

        if (!(timerCont % 500)) // si paso medio segundo
        {
            PORTDbits.RD1 = ~PORTDbits.RD1;
        }

        if (!(timerCont % 250)) // si paso 1/4 de segundo
        {
            PORTDbits.RD2 = ~PORTDbits.RD2;
        }

        if (!(timerCont % 125)) // si paso 1/8 de segundo
        {
            PORTDbits.RD3 = ~PORTDbits.RD3;
        }
    }

    return;
}

// Definiciones de funciones ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void gpioSetup()
{
    TRISD = 0;
    PORTD = 0;
}

void timer0Setup()
{
    OPTION_REGbits.T0CS = 0;   // clock interno, y no el del pin RA4
    OPTION_REGbits.PSA = 0;    // asigno prescaler al timer0
    OPTION_REGbits.PS = 0b001; // prescaler 1:4

    INTCONbits.TMR0IF = 0; // pongo el flag de overflow a 0
    INTCONbits.TMR0IE = 1; // activo interrupcion por timer0

    TMR0 = TMR0_INIT_VAL; // establezco un 'piso'
}

void __interrupt() ISR()
{
    if (INTCONbits.TMR0IF) // si hay overflow
    {
        timerCont++; // sumo a la variable contador en mseg

        TMR0 = TMR0_INIT_VAL;  // vuelvo a establecer el piso
        INTCONbits.TMR0IF = 0; // limpio el flag
    }
}
