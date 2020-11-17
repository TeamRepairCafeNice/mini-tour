// AccelStepper library
#include <AccelStepper.h> 
#define MOTX_DIR      22
#define MOTX_PUL      23
#define MOTX_STOP     53
#define MOTZ_PUL      24
#define MOTZ_DIR      25
#define MOTX_HOME     52
#define MAX_SPEED     5000
#define MAX_READ      20000
#define MAX_AVG       100
#define X             1
#define Z             3
#define NB_SAMPLES    50
#define EPSILON       10
AccelStepper xStepper(X, MOTX_PUL, MOTX_DIR); // direction Digital 23 (CCW), pulses Digital 22 (CLK)
AccelStepper zStepper(Z, MOTZ_PUL, MOTZ_DIR); // direction Digital 25 (CCW), pulses Digital 24 (CLK)

//Pins
const byte Analog_X_pin = A8;   //x-axis readings
const byte Analog_Z_pin = A9;   //z-axis readings
const byte Analog_R_pin = A10;  //r-axis readings

//Variables
int Analog_X = 0;               //x-axis value
int Analog_Z = 0;               //z-axis value
int Analog_R = 0;               //r-axis value

int Analog_X_AVG = 0; //x-axis value average
int Analog_Z_AVG = 0; //y-axis value average
int Analog_R_AVG = 0; //r-axis value average

int Analog_R_Value = 0; //this is used for the speed value

int count = 0;
int xSpeeds[MAX_AVG];
int zSpeeds[MAX_AVG];
bool xHomed = false;
bool xStopped = false;

void setup() {
 //SERIAL
  Serial.begin(56700);
  //----------------------------------------------------------------------------    
  //PINS
  pinMode(Analog_X_pin, INPUT);
  pinMode(Analog_Z_pin, INPUT);  
  pinMode(Analog_R_pin, INPUT); 
  pinMode(MOTX_HOME, INPUT_PULLUP);
  pinMode(MOTX_STOP, INPUT_PULLUP);
  pinMode(MOTX_PUL, OUTPUT);
  pinMode(MOTX_DIR, OUTPUT);
  pinMode(MOTZ_PUL, OUTPUT);
  pinMode(MOTZ_DIR, OUTPUT);

  for (int i = 0; i < MAX_AVG; i++) 
  {
    xSpeeds[i] = 0;
    zSpeeds[i] = 0;
  }

  //----------------------------------------------------------------------------  
  InitialValues(); //averaging the values of the 3 analog pins (values from potmeters)
  //----------------------------------------------------------------------------  
  //Stepper parameters
  //setting up some default values for maximum speed and maximum acceleration
  xStepper.setMaxSpeed(MAX_SPEED); //SPEED = Steps / second  
  xStepper.setAcceleration(1000); //ACCELERATION = Steps /(second)^2    
  xStepper.setSpeed(500);
  zStepper.setMaxSpeed(MAX_SPEED); //SPEED = Steps / second  
  zStepper.setAcceleration(1000); //ACCELERATION = Steps /(second)^2    
  zStepper.setSpeed(500);
  delay(500);
}

void loop() {
  ReadAnalog();  
  ReadDigital();
  xStepper.runSpeed(); //step the motor (this will step the motor by 1 step at each loop indefinitely)
  zStepper.runSpeed(); //step the motor (this will step the motor by 1 step at each loop indefinitely)
}
void ReadDigital()
{
  xHomed = (LOW == digitalRead(MOTX_HOME));
  xStopped = (LOW == digitalRead(MOTX_STOP));
// ---------------------------------------------------------------------------
  if (xStopped)
  {
    xStepper.setSpeed(0);
  }
// ---------------------------------------------------------------------------
  if (xHomed)
  {
    xStepper.setSpeed(2000);
  }
}

void ReadAnalog()
{
  //Reading the 3 potentiometers in the joystick: x, y and r.
  Analog_X = analogRead(Analog_X_pin);  
  Analog_Z = analogRead(Analog_Z_pin);  
  Analog_R = analogRead(Analog_R_pin);      
  
  Serial.print("\t| Delta R ");Serial.print(Analog_R - Analog_R_AVG);

//----------------------------------------------------------------------------  
  if(abs(Analog_R - Analog_R_AVG) > EPSILON) 
  {
    Analog_R_Value = map(Analog_R, -100, 900, 0, 1000); 
  }
//----------------------------------------------------------------------------  
  if(abs(Analog_X - Analog_X_AVG) > EPSILON) 
  {
    int xspeed = map(Analog_R_Value * (Analog_X - Analog_X_AVG), -MAX_READ, MAX_READ, -MAX_SPEED, MAX_SPEED);
    xStepper.setSpeed(xspeed) ; 
    Serial.print("\t| Delta X ");Serial.print(Analog_X - Analog_X_AVG); Serial.print("\t| X speed ");Serial.println(xspeed);
    calcAvgSpeed(xspeed, xSpeeds, &Analog_X_AVG);
  }
  else
  {
    xStepper.setSpeed(0);
    Serial.print("\t| Delta X ");Serial.print(Analog_X - Analog_X_AVG); Serial.print("\t| X speed ");Serial.println(0);
  }
//----------------------------------------------------------------------------  
  if(abs(Analog_Z - Analog_Z_AVG) > EPSILON) 
  {
    int zspeed = map(Analog_R_Value * (Analog_Z - Analog_Z_AVG),  -MAX_READ, MAX_READ, -MAX_SPEED, MAX_SPEED);
    zStepper.setSpeed(zspeed); 
    Serial.print("\t| Delta Z ");Serial.print(Analog_Z - Analog_Z_AVG); Serial.print("\t| Z speed ");Serial.println(zspeed);
    calcAvgSpeed(zspeed, zSpeeds, &Analog_Z_AVG);
  }
  else
  {
    zStepper.setSpeed(0);
    Serial.print("\t| Delta Z ");Serial.print(Analog_Z - Analog_Z_AVG); Serial.print("\t| Z speed ");Serial.println(0);
  }
}

void InitialValues()
{
  //Set the values to zero before averaging
  float sumX = 0;
  float sumZ = 0;
  float sumR = 0;
  //----------------------------------------------------------------------------  
  //read the analog 50x, then calculate an average. 
  //they will be the reference values
  for(int i = 0; i < NB_SAMPLES; i++)
  {
     sumX += analogRead(Analog_X_pin);  
     delay(10); //allowing a little time between two readings
     sumZ += analogRead(Analog_Z_pin);        
     delay(10);
     sumR += analogRead(Analog_R_pin);
     delay(10);
  }
  //----------------------------------------------------------------------------  
  Analog_X_AVG = sumX/NB_SAMPLES; 
  Analog_Z_AVG = sumZ/NB_SAMPLES; 
  Analog_R_AVG = sumR/NB_SAMPLES; 
  //----------------------------------------------------------------------------  
  Serial.print("AVG_X: "); Serial.println(Analog_X_AVG);
  Serial.print("AVG_Z: "); Serial.println(Analog_Z_AVG);
  Serial.print("AVG_R: "); Serial.println(Analog_R_AVG);
  Serial.println("Calibration finished");  
}

void calcAvgSpeed(int spd, int speeds[], int *pAvg)
{
  int sum = 0;
  speeds[count % MAX_AVG] = spd;
  for (int i = 0, j = 0; i < MAX_AVG; i++)
  {
    if (speeds[i] != 0) {
      j++;
      sum += speeds[i];
      *pAvg = sum / j;
    }
  }
  count++;
}
