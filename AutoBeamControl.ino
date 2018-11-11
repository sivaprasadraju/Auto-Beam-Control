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