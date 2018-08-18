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

void usart_init(void)
  {
    UCSR0A=0x00;
    UCSR0B=(1<< RXEN0)|(1<< TXEN0);
    UCSR0C=(1<<UCSZ01)|(1<<UCSZ00);
    UBRR0L=103;
  }

/*
 TO PRINT DATA TO SERIAL MONITOR
 */
void USARTWriteChar(char data)
 {
   while(!(UCSR0A & (1<<UDRE0)))
     {
    
     }
   UDR0=data;
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
  usart_init();
  setup_external_interrupt();

  init_timer0();
  timer0_start();
  sei();
 
  while (1) {
    if(new_key) {
       if( new_key == 0xB24DF20D ) {
          USARTWriteChar('0');
          USARTWriteChar('\n');
       }
       else if( new_key == 0xB24D629D ) {
          USARTWriteChar('1');
          USARTWriteChar('\n');
       }
       else if( new_key == 0xB24D56A9 ) {
          USARTWriteChar('2');
          USARTWriteChar('\n');
       }
       else if( new_key == 0xB24D52AD ) {
          USARTWriteChar('3');
          USARTWriteChar('\n');
       }
       else if( new_key == 0xB24D6699 ) {
          USARTWriteChar('4');
          USARTWriteChar('\n');
       }
       else if( new_key == 0xB24DFF00 ) {
          USARTWriteChar('5');
          USARTWriteChar('\n');
       }
       else if( new_key == 0xB24DF0F0 ) {
          USARTWriteChar('6');
          USARTWriteChar('\n');
       }
       else if( new_key == 0xB24DA694  ) {
          USARTWriteChar('7');
          USARTWriteChar('\n');
       }
       new_key=0;
    }
    
  }
  return 0;
}


