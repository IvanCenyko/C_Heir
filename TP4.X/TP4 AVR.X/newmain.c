#include <xc.h>
#include <avr/interrupt.h>

// Constantes ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
const unsigned long CLOCK_FREQ = 4000000;
const unsigned char TMR0_INIT_VAL = 65;

// Variables globales ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
volatile unsigned int timerCont;

// Prototipos de funciones ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void gpioSetup(void);
void timer0Setup(void);
ISR(TIMER0_OVF_vect);

// Main ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void main(void)
{
    cli(); // deshabilito interrupciones globalmente

    gpioSetup();
    timer0Setup();

    sei();  // habilito interrupciones globalmente

    while (1)
    {
        if (!(timerCont % 1000)) // si paso 1 seg
        {
            PORTD |= ~PD0; // invierto estado del led
        }

        if (!(timerCont % 500)) // si paso medio seg
        {
            PORTD |= ~PD1;
        }

        if (!(timerCont % 250)) // si paso un cuarto de seg
        {
            PORTD |= ~PD2;
        }

        if (!(timerCont % 125)) // si paso un octavo de seg
        {
            PORTD |= ~PD3;
        }
    }

    return;
}

// Definiciones de funciones ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void gpioSetup()
{
    DDRD = 0xFF;
    PORTD = 0;
}

void timer0Setup()
{
    TIMSK0 |= 1 << TOIE0; // habilito interrupcion por overflow en timer0
    TCCR0B |= 0b011; // prescaler en 1:64, se supone que al timer le llegan 4MHz
    TCNT0 = TMR0_INIT_VAL; // establezco un 'piso'
}

ISR(TIMER0_OVF_vect)
{

    timerCont++; // sumo 1 a la variable contador
    TCNT0 = TMR0_INIT_VAL;  // establezco nuevamente el piso del timer
    
 // el flag se limpia por hardware
}