
#define PIN_CW        2
#define PIN_CCW       3
#define PIN_STEP      23
#define PIN_DIR       22
#define PIN_STOP      53
#define PIN_ENABLE    7

// It seems that TB6600 requires to double the original steps number
// So 200 steps by 1/4 microsteps should require 800 pulses, 
// but works accurately only if defined as 1600

#define STEPS         1600
#define RPM           500
#define GEAR_RATIO    2.5
#define QUARTER_TURN  quarterTurn
#define PULSE_MICROS  50

#define BAUDS         57600

float stepCalc = 0.0;
int quarterTurn = 0;

void setup()
{
  pinMode(PIN_STOP, INPUT_PULLUP);
  pinMode(PIN_CW, INPUT_PULLUP);
  pinMode(PIN_CCW, INPUT_PULLUP);
  pinMode(PIN_ENABLE, OUTPUT);
  pinMode(PIN_STEP, OUTPUT);
  pinMode(PIN_DIR, OUTPUT);
  Serial.begin(BAUDS);
  stepCalc = STEPS * GEAR_RATIO;
  quarterTurn = floor(stepCalc / 4.0);
  Serial.print(QUARTER_TURN);
  Serial.println(" steps.");
  //steps(STEPS);
  steps(-STEPS);
}
void enableMotor(bool on) 
{
}
void turnQuarterRight()
{
  Serial.println("1/4 horaire");
  enableMotor(true);
  steps(QUARTER_TURN);
}
void turnQuarterLeft()
{
  Serial.println("1/4 anti-horaire");
  enableMotor(true);
  steps(-QUARTER_TURN);
}

void steps(int n)
{
  digitalWrite(PIN_DIR,(n > 0) ? LOW : HIGH);
  for (int i = 0; i < abs(n); i++)
  {
    digitalWrite(PIN_STEP,HIGH);
    delayMicroseconds(PULSE_MICROS);
    digitalWrite(PIN_STEP,LOW);
    delayMicroseconds(PULSE_MICROS);
    Serial.println(i + 1);
  }
}

int digitalButtonRead(int pinNum)
{
  int state0 = LOW, state1 = HIGH;
  while(state0 != state1) {
    state0= digitalRead(pinNum);
    delay(50);
    state1 = digitalRead(pinNum);
  }
  return state0;
}
void loop() 
{
  if(LOW == digitalButtonRead(PIN_CW))
  {
     turnQuarterRight();
  }

  if(LOW == digitalButtonRead(PIN_CCW))
  {
     turnQuarterLeft();
  }

  if(LOW == digitalButtonRead(PIN_STOP)) 
  {
    Serial.println("Stopped"); Serial.flush();
    enableMotor(false);
  }
}
