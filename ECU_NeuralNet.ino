/*===============================================Neural Network=====================================================*/
void varStorage(){
  
  ryev_del[k][0] = targetEqRat;
  tes1=100.0*eqRat;
  tes2=100.0*ryev_del[k-1][1];
  if(tes1==tes2){
    ryev_del[k][1] = eqRat + 0.01;
  }
  else{
    ryev_del[k][1] = eqRat;
  }
  ryev_del[k][2] = errorEqRat;
  ryev_del[k][3] = correction; //Control signal, NN output
  tes3=100.0*correction;
  tes4=100.0*ryev_del[k-1][3];
  if(tes3==tes4){
    ryev_del[k][3]=correction+0.01;
  }
}

void find_dj_dwi(){
  l = k;
  //     del     = 2 * (reference[k]   -  output[k])     * (  (0.01+output[k]    - output[k-1])   ) / (0.01+control[k]    - control[k-1])  
  ryev_del[l][4] = 2 * (ryev_del[l][0] - ryev_del[l][1]) * ( findDif(ryev_del[l][1],ryev_del[l-1][1]) / findDif(ryev_del[l][3],ryev_del[l-1][3]) );
  dj_dw1 += ryev_del[l][4] * xd1(l);
  dj_dw2 += ryev_del[l][4] * xd2(l); 
  dj_dw3 += ryev_del[l][4] * xd3(l);
}


void find_dj_dwij(){
  l = k;
  //             del[k]                                                                     ref[k]
  dj_dw11 += ryev_del[l][4] * ( findDif(xd1(l),xd1(l-1)) / findDif(ud1(l),ud1(l-1)) ) * w1 * ryev_del[0][l];
  dj_dw12 += ryev_del[l][4] * ( findDif(xd2(l),xd2(l-1)) / findDif(ud2(l),ud2(l-1)) ) * w2 * ryev_del[0][l]; 
  dj_dw13 += ryev_del[l][4] * ( findDif(xd2(l),xd2(l-1)) / findDif(ud3(l),ud3(l-1)) ) * w3 * ryev_del[0][l];

  //              del[k]                                                                    output[k-1]
  dj_dw21 += ryev_del[l][4] * ( findDif(xd1(l),xd1(l-1)) / findDif(ud1(l),ud1(l-1)) ) * w1 * ryev_del[1][l-1];
  dj_dw22 += ryev_del[l][4] * ( findDif(xd2(l),xd2(l-1)) / findDif(ud2(l),ud2(l-1)) ) * w2 * ryev_del[1][l-1]; iDelay = xd2(l);
  dj_dw23 += ryev_del[l][4] * ( findDif(xd3(l),xd3(l-1)) / findDif(ud3(l),ud3(l-1)) ) * w3 * ryev_del[1][l-1]; ud3Delay = ud3(l);
}

void updateWeight(){
  dj_dw1 = -dj_dw1/samp;
  dj_dw2 = -dj_dw2/samp;
  dj_dw3 = -dj_dw3/samp;

  dj_dw11 = -dj_dw11/samp;
  dj_dw12 = -dj_dw12/samp;
  dj_dw13 = -dj_dw13/samp;
  dj_dw21 = -dj_dw21/samp;
  dj_dw22 = -dj_dw22/samp;
  dj_dw23 = -dj_dw23/samp;

  w1 -= learningRate * dj_dw1;
  w2 -= learningRate * dj_dw2;
  w3 -= learningRate * dj_dw3;

  w11 -= learningRate * dj_dw11;
  w12 -= learningRate * dj_dw12;
  w13 -= learningRate * dj_dw13;
  w21 -= learningRate * dj_dw21;
  w22 -= learningRate * dj_dw22;
  w23 -= learningRate * dj_dw23;

  // w1 = constrain(w1,0.001,10);
  // w2 = constrain(w2,0.001,10);
  // w3 = constrain(w3,0.001,10);

  // w11 = constrain(w11,0.001,1);
  // w12 = constrain(w12,0.001,1);
  // w13 = constrain(w13,0.001,1);
  // w21 = constrain(w21,-1,-0.001);
  // w22 = constrain(w22,-1,-0.001);
  // w23 = constrain(w23,-1,-0.001);


  dj_dw1 = 0;
  dj_dw2 = 0;
  dj_dw3 = 0;

  dj_dw11 = 0;
  dj_dw12 = 0;
  dj_dw13 = 0;
  dj_dw21 = 0;
  dj_dw22 = 0;
  dj_dw23 = 0;

}

