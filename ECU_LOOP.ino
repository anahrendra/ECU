/*================================MAIN LOOP=========================================*/
void loop() {
  runServo();
  displayEngineState();
  sensorSamplingTask();
  printToHMI();

  idleCondition();
  //digitalWrite(safetyPin, LOW);
  if (rpm > 4000) {
    count += 1;
    if (count == 20) {
      digitalWrite(safetyPin, HIGH);
      delay(5000);
      digitalWrite(safetyPin, LOW);
      count = 0;
    }
  } else {
    count = 0;
  }


}

void idleCondition() {
  /* Crank at TDC */
  if (isTDC == 1) {
    /* Measure Input Parameter */
    sensorSamplingTask();

    /* Calculate RPM */
    if (temp1 == temp2) {
      rpm = 0;
    }
    else {
      rpm = (60.0) * 1000000.0 / (temp1 - temp2);
    }

        if(rpm >10000){
      rpm = tempRPM;
    }
    RPMFilter.Filter(rpm);
    rpm = RPMFilter.Current();

    
    tempRPM = rpm;

    /* Look for Injector Duration */
    idxRPM = indexSearch(rpm, axisRPM, 16);
    idxTPS = indexSearch(f_TPS, axisTPS, 16);

    targetAFR = pgm_read_word(&(AFR[idxTPS][idxRPM]));
    targetEqRat = 1;
    eqRat = 14.7 / f_Lambda;
    //eqRat = random(50,150);
    //eqRat /= 100.0;
    errorEqRat = targetEqRat - eqRat;

    k += 1;
    varStorage();

    find_dj_dwi();
    find_dj_dwij();

    if (k == samp) {
      updateWeight();
      if (isEngineIdle != 1.0) {
        correction = 0;
      }
      if (isEngineIdle == 1) {
        correction = constrain(200.0 * (w1 * xd1(k) + w2 * xd2(k) + w3 * xd3(k)), -100, 100);
      }
      //correction = constrain(100.0 * (w1 * xd1(k) + w2 * xd2(k) + w3 * xd3(k)),-100,100);
      //ryev_del[k][3] = correction;
      // Serial.print(tes1);Serial.print("\t");Serial.print(tes2);Serial.print("\t");
      // Serial.print(tes3);Serial.print("\t");Serial.print(tes4);Serial.print("\t");
      // Serial.print(eqRat);Serial.print("\t");Serial.print(ryev_del[k][1]);Serial.print("\t");Serial.println(correction);


      //printHere2();
      ryev_del[0][1] = ryev_del[k][1];
      ryev_del[0][3] = ryev_del[k][3];
      ryev_del[k][3] = correction;
      k = 0;
    }

    controlSignal =  interval + correction;

    totalTime += controlSignal / 1000;
    ccm = (rpm * controlSignal / 1000000.0) * (268.0 / 60.0);

    totalCons = (268.0 / 60.0) * totalTime / 1000;
    isTDC = 0;
    //printToHMI();

  }

  if (isPreTDC == 1) {
    isPreTDC = 0;
  }


}
