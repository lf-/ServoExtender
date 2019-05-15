#include <Arduino.h>

void setup()
{
  // f_pwm = f_tck1 / (OCR1C + 1)
  // where f_tck1 = f_pck / 16 = 4MHz since pck is 64MHz
  // this is intended to get us about 25KHz pwm frequency
  OCR1C = 159;

  OCR1B = 48; // approximately 30%
  // see intel spec at https://www.glkinst.com/cables/cable_pics/4_Wire_PWM_Spec.pdf

  TCCR1 = 0;         // clear the entire register because we don't know
                     // what bootloader does
  TCCR1 |= (0b0101); // set clock mode f_tck1 = PCK/16 for timer 1

  // since PLL is the system clock, we don't need to enable it or
  // wait for it to stabilize
  // Wait for PLOCK then turn on 64MHz peripheral clock source
  while ((PLLCSR & (1 << PLOCK)) == 0)
  {
  }
  PLLCSR |= (1 << PCKE);

  // turn on PWM on OC1B
  // also set the output mode per 12.2.2 table 12-1
  GTCCR |= (1 << PWM1B) | (1 << COM1B1);

  while (true)
  {
    auto inp = pulseIn(0, HIGH);
    auto nMicros = map(inp, 900, 2000, 500, 2500);
    if (nMicros == 0)
      continue;
    digitalWrite(2, HIGH);
    delayMicroseconds(nMicros);
    digitalWrite(2, LOW);
  }
}

void loop()
{
  // put your main code here, to run repeatedly:
}