#include <Stepper.h>
#define STEPS        200
#define MICRO_STEPS  32
#define PIN_STOP     2
#define PIN_DIR      3
#define PIN_STEP     4
#define PIN_JOY      A0
#define INITIAL_RPM  700
#define MAX_RPM      4000
#define DEAD_ZONE    20   // Amplititude de la zone neutre du joystick
#define NB_SAMPLES   1000  // lectures de calibrage
#define TEMPO_MILLIS 100   
#define BAUD_RATE    57600  

Stepper moteurPaP(STEPS * MICRO_STEPS, PIN_DIR, PIN_STEP); 

//Variables
int avg_centre = 0; // valeur moyenne du zéro central
int rpm = 0;        // Révolutions par minute
unsigned long last_time = millis();

void setup() {
 //SERIAL
  Serial.begin(BAUD_RATE);
  //----------------------------------------------------------------------------    
  //PINS
  pinMode(PIN_JOY, INPUT);
  pinMode(PIN_STOP, INPUT_PULLUP);
  pinMode(PIN_STEP, OUTPUT);
  pinMode(PIN_DIR, OUTPUT);

  //----------------------------------------------------------------------------  
  InitialValues(); //averaging the values of the 3 analog pins (values from potmeters)
  //----------------------------------------------------------------------------  
  //Stepper parameters
  //setting up some default values for maximum speed and maximum acceleration
  moteurPaP.setSpeed(INITIAL_RPM);
  moteurPaP.step(10 * MICRO_STEPS * STEPS);
  delay(1000);
}

// Lecture anti-rebond
int digitalButtonRead(int pinNum)
{
  int state0 = LOW, state1 = HIGH;
  while(state0 != state1) {
    state0= digitalRead(pinNum);
    delay(TEMPO_MILLIS);
    state1 = digitalRead(pinNum);
  }
  return state0;
}

void loop() {
  ReadAnalog();
  if (LOW == digitalButtonRead(PIN_STOP))
  {
    moteurPaP.setSpeed(0);
    Serial.println("----- Stopped ------");
  }
}

void ReadAnalog()
{
  // Lecture du potentiomètre du Joystick.
  int value = analogRead(PIN_JOY);
  unsigned long time_now = millis();
  Serial.print("\t| Lecture ");Serial.println(value - avg_centre);

//--------- lecture hors de la "dead-zone" du joystick ------------------------------  
  if(abs(value - avg_centre) > DEAD_ZONE) 
  {
    rpm = map(value, 0, 1023, -MAX_RPM, MAX_RPM); // 0 => -MAX_SPEED 1023 => MAX_SPEED
    Serial.print("\t| Tours par minute ");Serial.println( rpm );
    moteurPaP.setSpeed(abs(rpm));
    long pulses = (long) ((1.0 * rpm * MICRO_STEPS * STEPS)  * (time_now - last_time) / 60000.0) ; //steps for delta_time at rpm
    Serial.print("\t| Pulses ");Serial.println(pulses);
    delay(TEMPO_MILLIS);
    moteurPaP.step(pulses); 
    last_time = time_now;
  }
}

void InitialValues()
{
  // Calcul de la moyenne de plusieurs lectures de la position neutre
  float sum = 0.0;
  //----------------------------------------------------------------------------  
  for(int i = 0; i < NB_SAMPLES; i++)
  {
     sum += analogRead(PIN_JOY);  
     delay(TEMPO_MILLIS); //allowing a little time between two readings
  }
  //----------------------------------------------------------------------------  
  avg_centre = floor(sum/NB_SAMPLES); 
  //----------------------------------------------------------------------------  
  Serial.print("Milieu moyen: "); Serial.println(avg_centre);
  Serial.println("Calibrage fait");  
}
