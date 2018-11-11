/*
    Name: Siva Prasad Raju Bairaju
    Version: version 1.0
    Case Study Title: Auto Beam Control
    TinkerCad Link: https://www.tinkercad.com/things/cg5mpkx06k6-tremendous-stantia/editel?tenant=circuits?sharecode=-9RhfYRxUiegCPeckrvqv20HFRHTNZ6USNs2srlcYqQ=

*/

#include<stdint.h>
#include<avr/io.h>
#include<avr/interrupt.h>
#include<util/delay.h>
#include<stdbool.h>

#define SET_BIT(PORT,BIT) PORT|= (1<<BIT)
#define CLR_BIT(PORT,BIT) PORT&= ~(1<<BIT)
#define ADC_PIN 0

struct
{
  volatile unsigned int FLAG_ISR_INT1:1;
  volatile unsigned int FLAG_ISR_INT0:1;
}FLAG;

volatile int value;

void idle_state(void)
{
  while(1)
  {
    //Serial.println("Entered into idle state");
    if(FLAG.FLAG_ISR_INT0 == 1)
    {
      //Serial.println("SWITCH1 ON");
      value = adc_read(0);
      Serial.println(value);
        if(value >= 0 && value<= 200 )
          {
                //Serial.println("Entered into Low  state");
                OCR2A = 255;
                OCR2B = 255;
                value = adc_read(0);
                Serial.println(value);
          }
        else if(value > 200  && value <= 400)
          {
                //Serial.println("Entered into medium State");
                OCR2A = 128;
                OCR2B = 255;
                value = adc_read(0);
                Serial.println(value);
          }
        else if(value > 400)
          {
                //Serial.println("Entered into high state");
                OCR2A = 64;
                OCR2B = 255;
               value = adc_read(0);
                Serial.println(value);
          }
        
      }
  }
}

ISR(INT0_vect)
{
  cli();
  if(FLAG.FLAG_ISR_INT0 == 1)
  {
   value = adc_read(0);
  }
  else
  {
    FLAG.FLAG_ISR_INT0 = 1;
  }
  sei();
}

ISR(INT1_vect)
{
  cli();
  FLAG.FLAG_ISR_INT0 = 0;
  sei();
}

ISR(TIMER2_COMPA_vect)
{
    cli();
    CLR_BIT(PORTB,PB1);    
    sei();
}

ISR(TIMER2_COMPB_vect)
{
    cli();
    SET_BIT(PORTB,PB1);
    sei();
}

uint16_t adc_read(uint8_t adc)
{
  ADCSRA |= 1<<ADEN;
  ADMUX |= (1<<REFS0);
  ADMUX |= (adc & 0x0f);
  ADCSRA |= (1<<ADSC);
  while(ADCSRA & (1<<ADSC))
  {
  }
  return ADC;
}

int main()
{
  SET_BIT(DDRB,PB1);
  SET_BIT(DDRD,PD4);
  
  CLR_BIT(DDRB,PB1);
  CLR_BIT(DDRD,PD4);

  CLR_BIT(PORTD,PD3);
  CLR_BIT(PORTD,PD2);
  CLR_BIT(DDRC,PC0);
  
  EICRA|=(1<<ISC00);
  EICRA &= ~(1<<ISC01);                                                                                                                                  
  EIMSK |= (1<<INT0);
  
  EICRA|=(1<<ISC10);
  EICRA &= ~(1<<ISC11);                                                                                                                             
  EIMSK |= (1<<INT1);
  
    
  TCCR2A = 0x00;
  TCNT2 = 0x00;
  TCCR2B|=((1<<CS20)|(1<<CS21)| (1<<CS22));
  TIMSK2 |= (1<<OCIE2A) | (1<<OCIE2B);
  
  Serial.begin(9600);
  
  sei();
  
  while(1)
  {
    //Serial.println("Entered into main loop");
    SET_BIT(PORTD,PD4);
    idle_state();
  }
}