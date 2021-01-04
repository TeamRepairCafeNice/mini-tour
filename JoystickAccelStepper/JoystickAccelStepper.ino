// AccelStepper library
#include <AccelStepper.h> 
#define X            1
#define PIN_DIR      3
#define PIN_STEP     4
#define DEAD_ZONE    25
#define NB_SAMPLES   50
#define SPEED_BOOST  5

AccelStepper stepper(X, PIN_STEP, PIN_DIR); 


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
  //----------------------------------------------------------------------------  
  InitialValues(); //averaging the values of the 3 analog pins (values from potmeters)
  //----------------------------------------------------------------------------  
  //Stepper parameters
  //setting up some default values for maximum speed and maximum acceleration
  stepper.setMaxSpeed(5000); //SPEED = Steps / second  
  stepper.setAcceleration(1000); //ACCELERATION = Steps /(second)^2    
  stepper.setSpeed(500);
  delay(500);
}

void loop()
{
  ReadAnalog();  
  stepper.runSpeed(); //step the motor (this will step the motor by 1 step at each loop indefinitely)
}

void ReadAnalog()
{
  //Reading the 3 potentiometers in the joystick: x, y and r.
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
     delay(10); //allowing a little time between two readings
  }
  //----------------------------------------------------------------------------  
  Analog_X_AVG = tempX/NB_SAMPLES; 
  //----------------------------------------------------------------------------  
  Serial.print("AVG_X: ");
  Serial.println(Analog_X_AVG);
  Serial.println("Calibration finished");  
}
