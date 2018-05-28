//Timer1 output compare A interrupt service routine
ISR( TIMER1_COMPA_vect )
{
  digitalWrite(injectorPin, LOW);

  // Disable Interrupt
  TIMSK1 = (0 << ICIE1) | (0 << OCIE1C) | (0 << OCIE1B) | (1 << OCIE1A) | (0 << TOIE1);

  // Stop Timer 1
  TCCR1B = (0 << ICNC1) | (0 << ICES1) | (0 << WGM13) | (1 << WGM12) | (0 << CS12) | (0 << CS11) | (0 << CS10);
  
}

void injectionTask() {
  // Set OCR1A value for injection duration
  durationRegister = (controlSignal * 2) - 1;
  OCR1AH = (durationRegister & 0xFF00U) >> 8U;
  OCR1AL = (durationRegister & 0x00FFU);

  // Reset Timer 1
  TCNT1H = 0x00;
  TCNT1L = 0x00;

  digitalWrite(injectorPin, HIGH);

  // Timer/Counter 1 Interrupt(s) initialization
  TIMSK1 = (0 << ICIE1) | (0 << OCIE1C) | (0 << OCIE1B) | (1 << OCIE1A) | (0 << TOIE1);

  // Clear Timer 1 Interrupt flags
  TIFR1 = (0 << ICF1) | (0 << OCF1C) | (0 << OCF1B) | (0 << OCF1A) | (0 << TOV1);
  
  // Start Timer 1
  TCCR1B = (0 << ICNC1) | (0 << ICES1) | (0 << WGM13) | (1 << WGM12) | (0 << CS12) | (1 << CS11) | (0 << CS10);
  
}
