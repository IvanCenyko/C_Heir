#define F_CPU 1000000ul
#include <xc.h>
#include <util/delay.h>
#include <avr/io.h>


void ADC_Configuracion()
{
	ADMUX = (1<<REFS0); // VCC como referencia
	ADCSRA = (1<<ADEN)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0); //ADC habilitado (ADEN), Clock de conversion configurado
}

unsigned int ADC_Lectura(int canal){
    if(canal < 0 || canal > 7){ // la funcion devuelve 1 cuando el canal elegido no es valido
        return 1;
    }
    canal &= 0x7;  // solo se toman en cuenta numeros menores o iguales a 7
    ADMUX = (ADMUX & 0xF8)|canal; // justifico a la izquierda, elijo el canal al hacer 'OR', añadiendo a ADMUX el valor del canal elegido
    ADCSRA |= (1<<ADSC);  // inicio conversion
    while(ADCSRA & (1<<ADSC)); // bucle hasta que termine de medir
    return (ADC); // devuelvo el valor en 10 bits, 'ADC' es un acorte de AVR para no tener que hacer 'OR' con ADCL y ADCH
}
int main(void)
{
  unsigned int lectura;               
  ADC_Configuracion();
  DDRB = 0b11111111; //PORTB como salidas
  DDRC = 0b00000011; //PORTC PC0 y PC1 como salidas
  
  while(1){
      lectura = ADC_Lectura(2); // leo el ADC en canal 2
      PORTB = lectura;          //enciendo los LEDs segun lo recibido del ADC
      PORTC = lectura >> 8;    
  }

}