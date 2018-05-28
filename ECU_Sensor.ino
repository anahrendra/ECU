void sensorSamplingTask()
{
  time = millis();
  /* ADC sampling */
  valMAT = analogRead(adcMAT);
  valCHT = analogRead(adcCHT);
  valMAP = analogRead(adcMAP);
  valLambda = analogRead(adcLambda);
  valTPS = analogRead(adcTPS);

  /* Konversi */
  f_MAT = (0.4956 * valMAT) - 308.81;
  f_CHT = (0.4108 * valCHT) - 255.04;

  voltPress = valMAP * 0.004887586;  
  f_MAP = 0.0;
  if (voltPress < 4.6) {
    f_MAP = voltPress * 152.173913;   // it raises 700KPA over 4.6 volt      faster =>   pressure =  voltage * 152.173913;
  }
  else {
    f_MAP = (333.33 * voltPress) - 833.33;   //   last piece raises  100KPa over 0.3 Volt.   // can be optimized to   p = c1 + v * c2
  }
  f_MAP /= 100.0;
  f_MAP -= 0.13;

  f_Lambda = 10.0 + ((20.0 - 10.0) * valLambda) / 1023.0;

  // Read from potentiometer
  f_TPS = constrain(map(valPot,143,0,0,100),0,100);
  
  //Filtering data
  MATFilter.Filter(f_MAT);
  CHTFilter.Filter(f_CHT);
  MAPFilter.Filter(f_MAP);
  AFRFilter.Filter(f_Lambda);

  f_MAT = MATFilter.Current();
  f_CHT = CHTFilter.Current();
  f_MAP = MAPFilter.Current();
  f_Lambda = AFRFilter.Current();
  

  idxTemp = indexSearch(f_CHT, axisTemp,17);
  interval = baseInt[idxTemp];


}
