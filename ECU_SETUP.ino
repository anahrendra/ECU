/*================================SETUP=========================================*/
void setup() {
  time=millis();
  pinMode(hallTDC, INPUT);
  pinMode(hallPreTDC, INPUT);
  pinMode(adcMAT, INPUT);
  pinMode(adcCHT, INPUT);
  pinMode(adcMAP, INPUT);
  pinMode(adcLambda, INPUT);
  pinMode(adcTPS, INPUT);
  pinMode(adcPot, INPUT);

  pinMode(injectorPin, OUTPUT);
  pinMode(engineIndicator, OUTPUT);
  pinMode(starterIndicator, OUTPUT);
  pinMode(fuelPumpRelay, OUTPUT);
  pinMode(safetyPin, OUTPUT);
  digitalWrite(adcLambda,HIGH);
  attachInterrupt(digitalPinToInterrupt(hallTDC), checkTDC, FALLING);
  attachInterrupt(digitalPinToInterrupt(hallPreTDC), checkPreTDC, FALLING);

  temp1 = 0;
  temp2 = 0;

  throttleServo.attach(9);
  Serial.begin(115200);

  //Initialize Neural Network
  k = 0;

  w1 = 1;
  w2 = 0;
  w3 = 0;
  KP = w1;
  KI = w2;
  KD = w3;

  w11 = 1;
  w12 = 1;
  w13 = 1;

  w21 = -1;
  w22 = -1;
  w23 = -1;

  dj_dw1 = 0;
  dj_dw2 = 0;
  dj_dw3 = 0;

  dj_dw11 = 0;
  dj_dw12 = 0;
  dj_dw13 = 0;
  dj_dw21 = 0;
  dj_dw22 = 0;
  dj_dw23 = 0;

  targetEqRat = 0;
  eqRat = 0;
  errorEqRat = 0;
  correction = 0;

  int a,b;
  for(b = 0; b <= 4; b++){
    for(a = 0; a <= samp; a++){
      ryev_del[a][b] = 0;
    }
  }
  digitalWrite(fuelPumpRelay,LOW);
  //Serial.print(xd1(k));Serial.print("\t");Serial.print(KI);Serial.print("\t");Serial.println(KD);
  digitalWrite(injectorPin, HIGH);
  delay(300);
  digitalWrite(injectorPin, LOW);
  totalTime = 500;

  TCCR1A = (0 << COM1A1) | (0 << COM1A0) | (0 << COM1B1) | (0 << COM1B0) | (0 << COM1C1) | (0 << COM1C0) | (0 << WGM11) | (0 << WGM10);
  TCCR1B = (0 << ICNC1) | (0 << ICES1) | (0 << WGM13) | (1 << WGM12) | (0 << CS12) | (0 << CS11) | (0 << CS10);

  // Timer 4 Initialization for Engine State
  // Interrupt every 500 ms
  TCCR4A = (0 << COM4A1) | (0 << COM4A0) | (0 << COM4B1) | (0 << COM4B0) | (0 << COM4C1) | (0 << COM4C0) | (0 << WGM41) | (0 << WGM40);
  TCCR4B = (0 << ICNC4) | (0 << ICES4) | (0 << WGM43) | (1 << WGM42) | (1 << CS42) | (0 << CS41) | (1 << CS40);
  TCNT4H = 0x00;
  TCNT4L = 0x00;
  OCR4AH = 0x1E;
  OCR4AL = 0x84;

  // Timer/Counter 4 Interrupt(s) initialization
  TIMSK4 = (0 << ICIE4) | (0 << OCIE4C) | (0 << OCIE4B) | (1 << OCIE4A) | (0 << TOIE4);
  
  Serial.println("Engine starting in");
  Serial.println("3");
  //delay(1000);
  Serial.println("2");
  //delay(1000);
  Serial.println("1");
  //delay(1000);
  Serial.println("Go");

  Serial.print("Time"); Serial.print("\t");
  Serial.print("MAT"); Serial.print("\t");
  Serial.print("CHT"); Serial.print("\t");
  Serial.print("MAP"); Serial.print("\t");
  Serial.print("AFR"); Serial.print("\t");
  Serial.print("TPS"); Serial.print("\t");
  Serial.print("RPM"); Serial.print("\t");
  Serial.print("interval"); Serial.print("\t");
  Serial.print("correction"); Serial.print("\t");
  Serial.println("printEngine");
}
