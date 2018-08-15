#include<avr/io.h>
#include<avr/interrupt.h>
/*
 * Binary Values Used
 * Address: 01001101
 * LEFT: 01001111
 * RIGHT:01000110
 * FOR_LEFT:01101010
 * FOR_RIGHT:01001010
 * SHOOT: 01100110 
 * 
 * Hexadecimal Values
 * LEFT: B24DF20D
 * RIGHT: B24D629D
 * FOR_LEFT: B24D56A9
 * FOR_RIGHT: B24D52AD
 * SHOOT: B24D6699
 */
uint32_t code_LEFT=0b10110010010011011111001000001101;
uint32_t code_RIGHT=0b10110010010011010110001010011101;
uint32_t code_FOR_LEFT=0b10110010010011010101011010101001;
uint32_t code_FOR_RIGHT=0b10110010010011010101001010101101;
uint32_t code_SHOOT=0b10110010010011010110011010011001;
uint32_t code;

volatile int count=0;


ISR(TIMER0_COMPA_vect) {
  PORTD^=(1<<2);
}

ISR(TIMER1_COMPA_vect) {
  TCCR1B=0x00;
  TCCR0B=0x00;
  PORTD=0x00;
}

ISR(TIMER2_OVF_vect) {
  ++count;
  if(count>=2) {
    count=0;
    TCCR2B=0x00;
  }
}

void start_pulse() {
  OCR1A=18000;
  TCCR1B=(1<<CS11);
  TCCR0B=(1<<CS00);
  TCNT1=0;
  TCNT0=0;
  PORTD=0b00000100;
  while(TCCR1B&(1<<CS11));
}

void start_delay() {
  PORTD=0x00;
  OCR1A=9000;
  TCCR1B=(1<<CS11);
  TCNT1=0;
  while(TCCR1B&(1<<CS11));
}

void code_pulse() {
  OCR1A=9000;
  TCCR1B=(1<<CS10);
  TCCR0B=(1<<CS00);
  TCNT1=0;
  TCNT0=0;
  PORTD=0b00000100;
  while(TCCR1B&(1<<CS10));
}

void delay_bit(int time_tic) {
  PORTD=0x00;
  OCR1A=time_tic;
  TCCR1B=(1<<CS10);
  TCNT1=0;
  while(TCCR1B&(1<<CS10));
}

void delay_time() {
  PORTD=0x00;
  TCCR2B=(1<<CS22)|(1<<CS21)|(1<<CS20);
  TCNT2=0;
  while(TCCR2B&(1<<CS20));
}

void send_code(int code_value) {
  if(code_value==0) {
    code=code_LEFT;
  }
  else if(code_value==1) {
     code=code_RIGHT;
  }
  else if(code_value==2) {
     code=code_FOR_LEFT;
  }
  else if(code_value==3) {
     code=code_FOR_RIGHT;
  }
  else if(code_value==4) {
     code=code_SHOOT;
  }
  start_pulse();
    start_delay();
    for(int i=0;i<32;++i) {
      code_pulse();
      if(code&0x80000000) {
        delay_bit(27000);
      }
      else {
        delay_bit(9000);
      }
      code<<=1;
    }
    code_pulse();
    code=0;
}

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

int main() {
  
  int X,Y,Z;
  DDRB=0xFF;
  Serial.begin(9600);
  ADC_Init();   /* Initialize ADC */
  int flag=0;
   
  TCCR0A=(1<<WGM01);
  TCCR1A=(1<<WGM12);
  TIMSK0=(1<<OCIE0A);
  TIMSK1=(1<<OCIE1A);
  TIMSK2=(1<<TOIE2);
  OCR0A=210;
  sei();
  DDRD=0b00000100;
  PORTD=0x00;
  Serial.begin(9600);
  while(1) {
    X= ADC_Read(0)-83;  /* Read X, Y, Z axis ADC value */
    Y= ADC_Read(1)-83;
    Z= ADC_Read(2)-77;
    
    if(X<-8 && Y<5) {
      ++flag;
      if(flag>=1) {
        flag=0;
        send_code(0);
      }
    }
  else if(X>10 && Y<5) {
      ++flag;
      if(flag>=1) {
        flag=0;
        send_code(1);
      }
    }
  else if(X<-8 && Y>8) {
      ++flag;
      if(flag>=1) {
        flag=0;
        send_code(2);
      }
    }
  else if(X>8 && Y>8) {
      ++flag;
      if(flag>=1) {
        flag=0;
        send_code(3);
      }
    }
  else if(Y>10 && X<5) {
        send_code(4);
    }
    delay_time();   
  }
  PORTD=0x00;
  return 0;
}

