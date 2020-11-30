#define STEPS        200
#define MICRO_STEPS  32
#define PULSE_MICROS 50
#define PIN_STOP     2
#define PIN_DIR      3
#define PIN_STEP     4
#define PIN_JOY      A0
#define DEAD_ZONE    15   // Amplititude de la zone neutre du joystick
#define NB_SAMPLES   100  // lectures de calibrage
#define TEMPO_MILLIS 10   
#define BAUD_RATE    9600  

//Variables
int avg_centre = 0; // valeur moyenne du zéro central
unsigned long last_time = millis();

void calibrage()
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

void setup() {
 //SERIAL
  Serial.begin(BAUD_RATE);
  while (!Serial) 
  {
    yield();
  }
  Serial.println("Contrôle au joystick");
  //----------------------------------------------------------------------------    
  //PINS
  pinMode(PIN_JOY, INPUT);
  pinMode(PIN_STOP, INPUT_PULLUP);
  pinMode(PIN_STEP, OUTPUT);
  pinMode(PIN_DIR, OUTPUT);

  calibrage(); 
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

// Execution de n pas ou micro-pas
// PULSE_MICROS minimum 2µs, ici 50 µs
void steps(int n)
{
  digitalWrite(PIN_DIR,(n > 0) ? LOW : HIGH);
  for (int i = 0; i < abs(n); i++)
  {
    digitalWrite(PIN_STEP,HIGH);
    delayMicroseconds(PULSE_MICROS);
    digitalWrite(PIN_STEP,LOW);
    delayMicroseconds(PULSE_MICROS);
  }
 delay(PULSE_MICROS);
}

void loop() {
  ReadAnalog();
  if (LOW == digitalButtonRead(PIN_STOP))
  {
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
    int rpm = map(value, 0, 1023, -MAX_RPM, MAX_RPM); // 0 => -MAX_SPEED 1023 => MAX_SPEED
    Serial.print("\t| Tours par minute ");Serial.println( rpm );
    long pulses = (long) ((1.0 * rpm * MICRO_STEPS * STEPS)  * (time_now - last_time) / 60000.0) ; //steps for delta_time at rpm
    Serial.print("\t| Pulses ");Serial.println(pulses);
    delay(TEMPO_MILLIS);
    steps(pulses); 
    last_time = time_now;
  }
}


