#include<avr/io.h>
#include<avr/interrupt.h>
#include<util/delay.h>

volatile int count=0;
volatile int timer_value=0;
volatile int pulse_count=-1;
int sof=0;
volatile uint32_t msg_bit=0;
volatile uint32_t new_key=0;

ISR(TIMER0_COMPA_vect) {
  if(count<50) {
    ++count;
  }
}

ISR(INT0_vect) {
  timer_value=count;
  count=0;
  TCNT0=0;
  if(sof) {
    ++pulse_count;
  }

  if(timer_value>=13&&timer_value<=15) {
    msg_bit=0;
    sof=1;
    pulse_count=-1;
  }
  else if((pulse_count>=0) && (pulse_count<=31)) {
    if(timer_value>=2) {
      msg_bit|=(uint32_t)1<<(31-pulse_count);
    }
    else {
    }
    if(pulse_count==31) {
      EICRA=(1<<ISC00);
    }
  }
  else if(pulse_count>=32)                  
    {
        new_key = msg_bit; 
        pulse_count = -1;
        sof=0;
        EICRA=(1<<ISC01);
    }
}

void setup_external_interrupt() {
  EICRA=(1<<ISC01);
  EIMSK=(1<<INT0);
}

void init_timer0() {
  TCCR0A=(1<<WGM01);
  TIMSK0=(1<<OCIE0A);
  OCR0A=250;
}

void timer0_start() {
  TCCR0B=(1<<CS01)|(1<<CS00);
  TCNT0=0;
}

void timer0_stop() {
  TCCR0B=0x00;
  
}

int main() {
  
  DDRD=0x00;
  
  setup_external_interrupt();

  init_timer0();
  timer0_start();
  sei();
  
  Serial.begin(9600);
  while (1) {
    if(new_key) {
      Serial.println(new_key,HEX);
       new_key=0;
    }
    //_delay_ms(1000);
    
  }
  return 0;
}


