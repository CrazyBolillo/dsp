#include <Arduino.h>
#include <Adafruit_MCP4725.h>

/**
 * Ammount of bits to be removed. For example if 4 bits are removed
 * then only the 6 most significant bits will remain.
*/
uint8_t remove_opts[] = {4, 5, 6 , 7};
uint8_t remove_amnt = remove_opts[3];
uint32_t adc_read;
Adafruit_MCP4725 dac;

void setup() {
  dac.begin(0x60);

  /**
   * Timer 0 setup
   * Toggle D6 (PD6) on compare match. Operate in CTC (as counter, clearing it on compare).
   * Prescaler of 64 used. 
   * Running at a frequency of 5kHz.
  */
  pinMode(PD6, OUTPUT);
  TCCR0A = 0x42; 
  TCCR0B = 0x03;
  OCR0A = 50;
  TIMSK0 = 0x02;

  // ADC Prescaler of 16
  ADCSRA &= 0xF8;
  ADCSRA |= 0x04;
}

void loop() {
  dac.setVoltage(adc_read, false);
}

ISR(TIMER0_COMPA_vect) {
  adc_read = analogRead(A0);
  adc_read = (adc_read >> remove_amnt);
  adc_read = (adc_read << remove_amnt);
  adc_read *= 4;
}