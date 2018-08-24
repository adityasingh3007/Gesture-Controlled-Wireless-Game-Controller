#include<avr/io.h>
#include<avr/interrupt.h>
/*
 * Binary Values Used
 * Address: 01001101
 * LEFT: 01001111
 * RIGHT:01000110
 * ACC_LEFT:01101010
 * ACC_RIGHT:01001010
 * ACC: 01100110 
 * STABLE:11111111
 * BRAKE:11110000
 * 
 * Hexadecimal Values
 * LEFT: B24DF20D
 * RIGHT: B24D629D
 * ACC_LEFT: B24D56A9
 * ACC_RIGHT: B24D52AD
 * ACC: B24D6699
 * STABLE: B24DFF00
 * BRAKE: B24DF0F0
 * BOOST: B24DA694
 */
uint32_t code_LEFT=0b10110010010011011111001000001101;
uint32_t code_RIGHT=0b10110010010011010110001010011101;
uint32_t code_FOR_LEFT=0b10110010010011010101011010101001;
uint32_t code_FOR_RIGHT=0b10110010010011010101001010101101;
uint32_t code_SHOOT=0b10110010010011010110011010011001;
uint32_t code_REST=0b10110010010011011111111100000000;
uint32_t code_BRAKE=0b10110010010011011111000011110000;
uint32_t code_BOOST=0b10110010010011011010011010010100;
uint32_t code=0;
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
    TCCR2B=0x00;
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
  else if(code_value==5) {
     code=code_REST;
  }
  else if(code_value==6) {
     code=code_BRAKE;
  }
  else if(code_value==7) {
     code=code_BOOST;
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
    
    if(X<-4&& Y<2) {
      send_code(0);
      Serial.print(0);
    }
  else if(X>6 && Y<2) {
      send_code(1);
      Serial.print(1);
    }
  else if(X<-4 && Y>=2) {
      send_code(2);
      Serial.print(2);
    }
  else if(X>6 && Y>=2) {
      send_code(3);
      Serial.print(3);
    }
  else if(Y>2 & Y<=10) {
      send_code(4);
      Serial.print(4);
  }
  else if(X<=6 & Y<= 2 & X>=-4 & Y>=-3) {
        send_code(5);
        Serial.print(5);
     }
  else if(Y<-3) {
      send_code(6);
      Serial.print(6);
  }
  else if(Y>10) {
      send_code(7);
      Serial.print(7);
  }
  else {
    Serial.print(6);Serial.print('\t');
    Serial.print(X);Serial.print('\t');Serial.println(Y);
  }
    delay_time();   
}
  PORTD=0x00;
  return 0;
}

