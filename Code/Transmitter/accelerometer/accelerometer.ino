#include <avr/io.h>   /* Include AVR std. library file */
#include <util/delay.h>   /* Include defined delay header file */
void ADC_Init()     /* ADC Initialization function */
{
  ADMUX =(1<<REFS0)|(1<<ADLAR);   /* Vref: Avcc, ADC channel: 0 */
  ADCSRA = (1<<ADEN)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);
}

unsigned char ADC_Read(unsigned char channel)  /* ADC Read function */
{
  ADMUX =(1<<REFS0)|(1<<ADLAR)|(channel & 0x07);/* set input channel to read */
  ADCSRA |= (1<<ADSC);  /* Start ADC conversion */
  while (!(ADCSRA & (1<<ADIF)));  /* Wait until end of conversion by polling ADC interrupt flag */
  ADCSRA |= (1<<ADIF);  /* Clear interrupt flag */
  return ADCH;    /* Return ADC word */
}
int main(void)
{
  unsigned char X,Y,Z;
  DDRB=0xFF;
  Serial.begin(9600);
  ADC_Init();   /* Initialize ADC */

  while(1)
  {
    X= ADC_Read(0);  /* Read X, Y, Z axis ADC value */
    Y= ADC_Read(1);
    Z= ADC_Read(2);
    if(X<73&&Z<90){
      PORTB|=0b00100000;
    }
    Serial.print("X: ");
    Serial.print(X);Serial.print('\t');
    Serial.print("Y: ");
    Serial.print(Y);Serial.print('\t');
    Serial.print("Z:");
    Serial.println(Z);
    _delay_ms(100);
  }
}
