void checkTDC() {
  temp2 = temp1;
  temp1 = micros();
  isTDC = 1;
}

void checkPreTDC() {
  isPreTDC = 1;
  injectionTask();
}

void runServo(){
  valPot = analogRead(adcPot);
  valPot = map(valPot, 0, 1023, 125, 143);
  TPSFilter.Filter(valPot);
  valPot = TPSFilter.Current();
  valPot = constrain(valPot,125,143);
  throttleServo.write(valPot);
}

void displayEngineState(){
  if(isEngineIdle == 1){
   printEngine = 1.0;

  }

 if(isStarting == 1){
   printEngine = 2.0;
   interval *= 1.25;
 } 
  
 if (isEngineIdle == 0 && isStarting == 0){
   printEngine = 0.0;
   rpm = 0;
 }
  
  
}

void printHere(){
  Serial.print(millis());Serial.print("\t");
  Serial.print(temp1);Serial.print("\t");
  Serial.print(f_TPS);Serial.print("\t");
  Serial.print(f_Lambda);Serial.println("\t");
}

void printHere2(){
  //Serial.print(KP);Serial.print("\t");Serial.print(KI);Serial.print("\t");Serial.println(KD);
  // Serial.print("k");Serial.print("\t");Serial.print("ref");Serial.print("\t");Serial.print("y");Serial.print("\t");
  // Serial.print("e");Serial.print("\t");Serial.print("sig");Serial.print("\t");Serial.print("del");Serial.print("\t");
  // Serial.print("w11");Serial.print("\t");Serial.print("w12");Serial.print("\t");Serial.print("w13");Serial.print("\t");
  // Serial.print("w21");Serial.print("\t");Serial.print("w22");Serial.print("\t");Serial.print("w23");Serial.println("\t");
  // Serial.print(k);Serial.print("\t");
  // Serial.print(ryev_del[k][0]);Serial.print("\t");
  // Serial.print(ryev_del[k][1]);Serial.print("\t");
  // Serial.print(ryev_del[k][2]);Serial.print("\t");
  // Serial.print(ryev_del[k][3]);Serial.print("\t");
  // Serial.print(ryev_del[k][4]);Serial.println("\t");

  Serial.print(w11);Serial.print("\t");
  Serial.print(w12);Serial.print("\t");
  Serial.print(w13);Serial.print("\t");
  Serial.print(w21);Serial.print("\t");
  Serial.print(w22);Serial.print("\t");
  Serial.print(w23);Serial.print("\t");

  Serial.print(w1);Serial.print("\t");
  Serial.print(w2);Serial.print("\t");
  Serial.print(w3);Serial.print("\t");

  Serial.print(dj_dw11);Serial.print("\t");
  Serial.print(dj_dw12);Serial.print("\t");
  Serial.print(dj_dw13);Serial.print("\t");
  Serial.print(dj_dw21);Serial.print("\t");
  Serial.print(dj_dw22);Serial.print("\t");
  Serial.print(dj_dw23);Serial.println("\t");
  // Serial.print(f_Lambda);Serial.print("\t");
  // Serial.print(correction);Serial.print("\t");
  // Serial.println(f_TPS);
}

void printToHMI(){
  Serial.print(time); Serial.print("\t");
  Serial.print(f_MAT); Serial.print("\t");
  Serial.print(f_CHT); Serial.print("\t");
  Serial.print(f_MAP); Serial.print("\t");
  Serial.print(f_Lambda); Serial.print("\t");
  Serial.print(f_TPS); Serial.print("\t");
  Serial.print(rpm); Serial.print("\t");
  Serial.print(ccm); Serial.print("\t");
  Serial.print(totalCons); Serial.print("\t");
  Serial.println(printEngine);

}
