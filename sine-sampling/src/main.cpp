#include <Arduino.h>
#include <Adafruit_MCP4725.h>

/**
 * Values used to obtain different frequencies from Timer 0.
 * 1kHz, 1.5kHz, 2kHz and 5kHz respectively.
*/
uint8_t timer_count[] = {250, 167, 125, 50};
uint32_t adc_read;
Adafruit_MCP4725 dac;

void setup() {
  dac.begin(0x60);
  Serial.begin(9600);

  /**
   * Timer 0 setup
   * Toggle D6 (PD6) on compare match. Operate in CTC (as counter, clearing it on compare).
   * Prescaler of 64 used. 
   * Start with frequency of 1kHz
  */
  pinMode(PD6, OUTPUT);
  TCCR0A = 0x42; 
  TCCR0B = 0x03;
  OCR0A = timer_count[3];
  TIMSK0 = 0x02;

  // ADC Prescaler of 16
  ADCSRA &= 0xF8;
  ADCSRA |= 0x04;
}

void loop() {
  dac.setVoltage(adc_read, false);
}

ISR(TIMER0_COMPA_vect) {
  adc_read = 4 * analogRead(A0);
}