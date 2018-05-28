// Nama file     : ECU_GLOBAL.ino
// Editor        : TA1718.01.009
// Last update   : 7 Maret 2017
// Edit history  : (4-2-17)   Modifikasi pada metode pemilihan interval injeksi [Line 84-89]
//                 (5-2-17)   Penambahan fungsi pengaturan interval injeksi dengan konsep PID sederhana [Line 83-84]
//                 (8-2-17)   Ubah program utama, injectionTask, checkTDC, interval menjadi int agar hemat memori dan lebih cepat
//                 (17-2-17)  Ubah tipe data hall number menjadi double
//                 (18-2-17)  Penambahan fungsi starter
//                 (19-2-17)  Penambahan fungsi starter dengan metode interrupt
//                 (24-2-17)  Penambahan fungsi pembacaan bukaan throttle (TPS), edit LUT
//                 (26-2-17)  Penambahan LUT untuk AFR dan fungsi feedback menggunakan AFR untuk faktor koreksi durasi injeksi
//                 (27-2-17)  Update LUT, penambahan filter
//                 (6-3-17)   Update LUT
//                 (7-3-17)   Update LUT
//                 (26-3-17)  Update untuk mesin 2-stroke
//                 (29-3-17)  Update durasi injeksi menggunakan internal timer Arduino Mega 2560
//                 (04-3-17)  Update Neural Network
// Board type    : Arduino Mega 2560

/*=============================================Look Up Table====================================================*/

// Ambil checkPreTDC, ISR_COMPA_vect, injectionTask, sama di idleCondition yang (isPreTDC == 1)
#include<MegunoLink.h>
#include<Filter.h>
#include<Servo.h>
#include<math.h>

const float baseInt[] = {
  //Temperature      20    25    30    35    40    45    50    55    60    65    70    80    90    100   110   120  130
  /*Base Interval*/ 3600, 3600, 3500, 3500, 3400, 3400, 3300, 3300, 3200, 3200, 3200, 3000, 2900, 2800, 2700,  2600, 2500
};


const PROGMEM  float AFR[][16] = {
  /* RPM         0    400     800     1200    1600    2000    2400    2800    3200    3600    4000    4400    4800    6000    7000    8000 */
  /* TPS (in bit)                                                                                                                                                  */
  /*  5 */  { 13  , 13  , 13  , 13  , 14.7  , 14.7  , 14.7  , 14.7  , 14.7  , 14.7  , 14.7  , 14.7  , 14.7  , 14.7  , 14.7  , 14.7  },
  /* 10 */  { 13  , 13  , 14  , 14  , 15.7  , 15.7  , 14.7  , 14.7  , 14.7  , 14.7  , 14.7  , 14.7  , 14.7  , 14.7  , 14.7  , 14.7  },
  /* 15 */  { 13  , 13  , 15  , 15  , 16.7  , 16.7  , 14.7  , 14.7  , 14.7  , 14.7  , 14.7  , 14.7  , 14.7  , 14.7  , 14.7  , 14.7  },
  /* 20 */  { 13  , 13  , 16  , 16  , 17.7  , 17.7  , 14.7  , 14.7  , 14.7  , 14.7  , 14.7  , 14.7  , 14.7  , 14.7  , 14.7  , 14.7  },
  /* 25 */  { 13  , 13  , 17  , 17  , 18.7  , 18.7  , 14.7  , 14.7  , 14.7  , 14.7  , 14.7  , 14.7  , 14.7  , 14.7  , 14.7  , 14.7  },
  /* 30 */  { 13  , 13  , 18  , 18  , 19.7  , 19.7  , 14.7  , 14.7  , 14.7  , 14.7  , 14.7  , 14.7  , 14.7  , 14.7  , 14.7  , 14.7  },
  /* 35 */  { 13  , 13  , 19  , 19  , 20.7  , 20.7  , 14.7  , 14.7  , 14.7  , 14.7  , 14.7  , 14.7  , 14.7  , 14.7  , 14.7  , 14.7  },
  /* 40 */  { 13  , 13  , 20  , 20  , 21.7  , 21.7  , 14.7  , 14.7  , 14.7  , 14.7  , 14.7  , 14.7  , 14.7  , 14.7  , 14.7  , 14.7  },
  /* 45 */  { 13  , 13  , 21  , 21  , 22.7  , 22.7  , 14.7  , 14.7  , 14.7  , 14.7  , 14.7  , 14.7  , 14.7  , 14.7  , 14.7  , 14.7  },
  /* 50 */  { 13  , 13  , 22  , 22  , 23.7  , 23.7  , 14.7  , 14.7  , 14.7  , 14.7  , 14.7  , 14.7  , 14.7  , 14.7  , 14.7  , 14.7  },
  /* 55 */  { 13  , 13  , 23  , 23  , 24.7  , 24.7  , 14.7  , 14.7  , 14.7  , 14.7  , 14.7  , 14.7  , 14.7  , 14.7  , 14.7  , 14.7  },
  /* 60 */  { 13  , 13  , 24  , 24  , 25.7  , 25.7  , 14.7  , 14.7  , 14.7  , 14.7  , 14.7  , 14.7  , 14.7  , 14.7  , 14.7  , 14.7  },
  /* 70 */  { 13  , 13  , 25  , 25  , 26.7  , 26.7  , 14.7  , 14.7  , 14.7  , 14.7  , 14.7  , 14.7  , 14.7  , 14.7  , 14.7  , 14.7  },
  /* 80 */  { 13  , 13  , 26  , 26  , 27.7  , 27.7  , 14.7  , 14.7  , 14.7  , 14.7  , 14.7  , 14.7  , 14.7  , 14.7  , 14.7  , 14.7  },
  /* 90 */  { 13  , 13  , 27  , 27  , 28.7  , 28.7  , 14.7  , 14.7  , 14.7  , 14.7  , 14.7  , 14.7  , 14.7  , 14.7  , 14.7  , 14.7  },
  /* 100 */ { 13  , 13  , 28  , 28  , 29.7  , 29.7  , 14.7  , 14.7  , 14.7  , 14.7  , 14.7  , 14.7  , 14.7  , 14.7  , 14.7  , 14.7  }
};

// Axis disimpan di RAM karena ukuran kecil dan lebih cepat untuk diakses
const int axisRPM[] = {0, 400, 800, 1200, 1600, 2000, 2400, 2800, 3200, 3600, 4000, 4400, 4800, 6000, 7000, 8000};
const int axisTPS[] = {5 ,10, 15, 20, 25, 30, 35, 40, 45, 50, 55, 60, 70, 80, 90, 100};
const int axisTemp[] = {0, 5, 10, 15, 20, 25, 30, 35, 40, 45, 50, 60, 70, 80, 90, 100, 110};
// const int axisState[] = {1};


int idxRPM;
int idxTPS;
int idxTemp;
// int idxState;

// ADC Sampling data filter with simple linear recursive exponential filter algorithm
ExponentialFilter<float> MATFilter(5,0);
ExponentialFilter<float> CHTFilter(5,0);
ExponentialFilter<float> MAPFilter(5,0);
ExponentialFilter<float> AFRFilter(5,0);
ExponentialFilter<float> TPSFilter(5,0);
ExponentialFilter<float> RPMFilter(5,0);

/*============================================Sensor-Actuator====================================================*/
// Actuator pin assignment
const int injectorPin = 4;
const int fuelPumpRelay = 5;
const int engineIndicator = 7;
const int starterIndicator = 8;;
const int throtPotPin = 9; //Absolute
const int safetyPin = 38;

// Sensor pin assignment
const int hallTDC = 2; //INT1
const int hallPreTDC = 3; //INT0
const int adcMAT = A0;
const int adcCHT = A1;
const int adcMAP = A2;
const int adcLambda = A3;
const int adcTPS = A4;
const int adcPot = A5;


// ADC variables
int valMAT = 0;
int valCHT = 0;
int valMAP = 0;
int valLambda = 0;
int valTPS = 0;
int valPot = 0;

// Real value variables
float f_MAT = 0;
float f_CHT = 0;
float f_MAP = 0;
float f_Lambda = 0;
float f_TPS = 0;
float f_Pot = 0;


// Temporary variables for sensor measurement
volatile double time, temp1, temp2;
double rpm;
float voltPress;

/* RPM Check variables */
volatile int isPreTDC = 0;
volatile int isTDC = 0;

/* Injector variables */
int injectorState = LOW;
float interval;
int durationRegister = 0;
float ccm;

int count=0;;

/* AFR feedback variables*/
float targetAFR;
float errorAFR;
float correction;
float eqRat;
float errorEqRat;
float controlSignal;
float targetEqRat;

/* Neural Network Variables */
float KP,KI,KD;
int samp = 1;
int n = 2;
float ryev_del[2][5]; //Reference AFR, actual AFR, 
int m, i, j, k, l, del;

float w11, w12, w13, w21, w22, w23, w1, w2, w3;

float dj_dwi, dj_dw1, dj_dw2, dj_dw3;
float dj_dw11, dj_dw12, dj_dw13, dj_dw21, dj_dw22, dj_dw23;
float learningRate = 0.001;
float iDelay = 0;
float ud3Delay = 0;
float lim = 1;



/* Servo Assignment */
Servo throttleServo;

/* Engine status variables */
volatile double lastTemp;
volatile int startingCount = 0;
volatile int isEngineIdle = 0;
volatile int isStarting = 0;
int printEngine = 0;

float totalCons;
float totalTime;
float tempRPM;

/*================================= Functions and Voids Declarations ================================*/
int indexSearch(int value, int arr[], int arrLength) ;
float ud1(int h);
float ud2(int h);
float ud3(int h);
float xd1(int h);
float xd2(int h);
float xd3(int h);
float satlin (float a);
float findDif (float a, float b);
void varStorage();
void find_dj_dwi();
void find_dj_dwij();
void updateWeight();

void idleCondition();
void sensorSamplingTask();
void injectionTask();
void checkTDC();
void checkPreTDC();
void runServo();
void printHere();
void printHere2();
void printToHMI();

int tes1,tes2,tes3,tes4;
