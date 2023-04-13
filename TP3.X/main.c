// CONFIG
#pragma config FOSC = HS // Oscillator Selection bits (HS oscillator)
#pragma config WDTE = OFF // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = OFF // Brown-out Reset Enable bit (BOR disabled)
#pragma config LVP = OFF // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit (RB3 is digital I/O, HV on MCLR
#pragma config CPD = OFF // Data EEPROM Memory Code Protection bit (Data EEPROM code protection off)
#pragma config WRT = OFF // Flash Program Memory Write Enable bits (Write protection off; all program memory may be written to by
#pragma config CP = OFF // Flash Program Memory Code Protection bit (Code protection off)

#define _XTAL_FREQ 8000000

#include <xc.h>



void GPIO_Configuracion(){ //config puertos del micro
    TRISB = 0;
    TRISC = 0;
    TRISD = 0;
}

void ADC_Configuracion()
{
    /*ADCON1*/
    ADCON1bits.PCFG = 0b0000; // según la tabla, todos lo pines analógicos
    ADCON1bits.ADFM = 0b1;    // ajuste a la derecha
    ADCON1bits.ADCS2 = 0b0;    // clock dividido en 32
    /*ADCON0*/
    ADCON0bits.ADCS = 0b010;  // clock dividido en 32
    ADCON0bits.CHS  = 0b000;   // canal 0
    ADCON0bits.GO   = 0b0;  //inicio detenido
    ADCON0bits.ADON = 0b1;  // ADC habilitado
}

unsigned int ADC_Lectura(unsigned char canal)
{
  if(canal <= 7) {         //Canal mayor a 7 es ignorado
	  ADCON0 &= 0xC5;        //ADCON0bits.CHS = 0b000;
	  ADCON0 |= canal<<3;    //ADCON0bits.CHS = channel;
	  __delay_ms(2);         //tiempo de espera para carga de capacitor
	  ADCON0 |= 1<<2;        //ADCON0bits.GO   = 0b0
	  while(ADCON0 & (1<<2));       //ADCON0bits.nDONE
	  return ((ADRESH<<8)+ADRESL); //devuelvo resultado (2 bytes)
	} else {
		return 0;
	}
}

float Temp_Lectura(){ //lectura y calculo de temperatura
    float lec = ADC_Lectura(0); // lectura
    float volt = lec * 5 / 1023;    // paso a voltaje
    float temperatura = volt / 0.01;    // voltaje a temp
    return temperatura;                     // devuelvo temp
}

void Sseg_Print(uint8_t temp) {     // funcion de print en 7seg
    if (temp < 100 && temp >= 0){   // si el valor es de 0-99
        int val_sup = temp / 10;    // separo digitos y escribo en 7seg
        int val_inf = temp - val_sup * 10;

        PORTB = val_sup;
        PORTC = val_inf;
    }
}


void main(void) {
    
    GPIO_Configuracion();
    ADC_Configuracion();
    int temp;
    int temp_max = 6 * (20-6);
    
    while (1){
        temp = Temp_Lectura(); // lectura
        Sseg_Print(temp);   // print
        
        if (temp > temp_max){ // si temp > 84 enciendo led
            RD0 = 1;
        }
        else {
            RD0 = 0;
        }
    }
    return;
}
