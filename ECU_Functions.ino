/*===============================================Functions=====================================================*/

int indexSearch(int value, int arr[], int arrLength) {
  int first, last, middle;
  first = 0;
  last = arrLength - 1;
  middle = (first + last) / 2;

  if (value <= arr[first])
    return 0;
  if (value >= arr[last])
    return last;

  while (first <= last) {
    if ( arr[middle] < value ) {
      first = middle + 1;
    }
    else if ( arr[middle] == value ) {
      return middle;
    }
    else {
      last = middle - 1;
    }
    middle = (first + last) / 2;
  }

  if ((value - arr[middle]) < (arr[middle + 1] - value))
    return middle;
  else
    return middle + 1;
}


/*===============================================NN Functions=====================================================*/
float ud1(int h){
  return w11*ryev_del[h][0] + w21*ryev_del[h][1];
}

float ud2(int h){
  return w12*ryev_del[h][0] + w22*ryev_del[h][1];
}

float ud3(int h){
  return w13*ryev_del[h][0] + w23*ryev_del[h][1];
}

float xd1(int h){
  return satlin(ud1(h));
}

float xd2(int h){
  if (h < 1){
    return 0;
  }
  else {}
    return satlin(iDelay + ud2(h));

}

float xd3(int h){
  return satlin(ud3(h) - ud3Delay);
}

float satlin (float a){
  if(a > lim){
    return lim;
  }
  else if(a < -lim){
    return -lim;
  }
  else{
    return a;
  }

}

float findDif(float a, float b){
  if (a==b){
    return 0.01;
  }else{
    return a-b;
  }
}

