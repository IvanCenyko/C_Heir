#include <xc.h>

#define _XTAL_FREQ 8000000


void ADC_Configuracion()
{
    /*ADCON1*/
    ADCON1bits.PCFG = 0b0000; // según la tabla, todos lo pines analógicos
    ADCON1bits.ADFM = 0b1;    // ajuste a la derecha
    ADCON1bits.ADCS2 = 0b0;    // clock dividido en 32
    /*ADCON0*/
    ADCON0bits.ADCS = 0b010;  // clock dividido en 32
    ADCON0bits.CHS  = 0b000;   // canal 0
    ADCON0bits.GO   = 0b0;
    ADCON0bits.ADON = 0b1;
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

void main()
{
  unsigned int lectura;   // variable para guardar la lectura del ADC
  ADC_Configuracion();    //Configuracion del ADC
  TRISB = 0b00000000; //pines del puerto B configurados como salidas
  TRISC &= 0b1111100; //pines RC0 y RC1 del puerto C configurados como salidas
  
  while(1){
      lectura = ADC_Lectura(0); // leo el ADC en el canal 0
      PORTB = lectura;          // guardo los 8 bits mas ligeros en PORTB para encender los LEDs
      PORTC = lectura >> 8;     // guardo los 2 bits restantes en PORTC para encender los LEDs
  }
}