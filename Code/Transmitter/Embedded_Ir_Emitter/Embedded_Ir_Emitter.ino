#include<avr/io.h>
#include<avr/interrupt.h>
/*
 * Binary Values Used
 * Address: 01001101
 * ON: 01001111
 * OFF:01000110
 * INC_BRIGHT:01101010
 * DEC_BRIGHT:01001010
 * 
 * Hexadecimal Values
 * ON: B24DF20D
 * OFF:B24D629D
 * INC_BRIGHT:B24D56A9
 * DEC_BRIGHT:B24D52AD
 */
uint32_t code_ON=0b10110010010011011111001000001101;
uint32_t code_OFF=0b10110010010011010110001010011101;
uint32_t code_INC_BRIGHT=0b10110010010011010101011010101001;
uint32_t code_DEC_BRIGHT=0b10110010010011010101001010101101;
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
  if(code_value==1) {
    code=code_ON;
  }
  else if(code_value==0) {
     code=code_OFF;
  }
  else if(code_value==2) {
     code=code_INC_BRIGHT;
  }
  else if(code_value==3) {
     code=code_DEC_BRIGHT;
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

int main() {
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
    Serial.println("Off");
    send_code(0);
    /*delay_time();
    Serial.println("On");
    send_code(1);
    delay_time();
    send_code(2);
    delay_time();
    send_code(3);*/
    delay_time();   
  }
  PORTD=0x00;
  return 0;
}

