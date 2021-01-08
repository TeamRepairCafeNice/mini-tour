// AccelStepper library
#include <AccelStepper.h> 
#define PIN_DIR      3
#define PIN_STEP     4
#define PIN_STOP     2
#define DEAD_ZONE    25
#define NB_SAMPLES   50
#define TEMPO_MILLIS 10
#define SPEED_BOOST  100

AccelStepper stepper(AccelStepper::DRIVER, PIN_STEP, PIN_DIR); 


//Pins
const byte Analog_X_pin = A0; //x-axis readings

//Variables
int Analog_X = 0; //x-axis value
int Analog_X_AVG = 0; //x-axis value average
void setup()
{

  //SERIAL
  Serial.begin(9600);
  //----------------------------------------------------------------------------    
  //PINS
  pinMode(Analog_X_pin, INPUT);
  pinMode(PIN_STOP, INPUT_PULLUP);
  //----------------------------------------------------------------------------  
  InitialValues(); //averaging the values of the 3 analog pins (values from potmeters)
  //----------------------------------------------------------------------------  
  //Stepper parameters
  //setting up some default values for maximum speed and maximum acceleration
  stepper.setMaxSpeed(SPEED_BOOST * 1023); //SPEED = Steps / second  
  stepper.setAcceleration(1000); //ACCELERATION = Steps /(second)^2    
  stepper.setSpeed(500);
  delay(500);
}

void loop()
{
  ReadAnalog();  
  if (LOW == digitalRead(PIN_STOP))
  {
     stepper.setSpeed(0); // STOP !
  }
  stepper.runSpeed(); //step the motor (this will step the motor by 1 step at each loop indefinitely)
}

void ReadAnalog()
{
  //Reading the potentiometer in the joystick: x.
  Analog_X = analogRead(Analog_X_pin);  

  //if the value is DEAD_ZONE "value away" from the average (midpoint), we allow the update of the speed
  //This is a sort of a filter for the inaccuracy of the reading
  if(abs(Analog_X-Analog_X_AVG)>DEAD_ZONE) 
  {
  stepper.setSpeed(SPEED_BOOST*(Analog_X-Analog_X_AVG));    
  }
  else
  {
    stepper.setSpeed(0);
  }
}

void InitialValues()
{
  //Set the values to zero before averaging
  float tempX = 0;
  //----------------------------------------------------------------------------  
  //read the analog NB_SAMPLES times, then calculate an average. 
  //they will be the reference values
  for(int i = 0; i<NB_SAMPLES; i++)
  {
     tempX += analogRead(Analog_X_pin);  
     delay(TEMPO_MILLIS); //allowing a little time between two readings
  }
  //----------------------------------------------------------------------------  
  Analog_X_AVG = tempX/NB_SAMPLES; 
  //----------------------------------------------------------------------------  
  Serial.print("AVG_X: ");
  Serial.println(Analog_X_AVG);
  Serial.println("Calibration finished");  
}
