
float value=0;
float rev=0;
int rpm;
int oldtime=0;        
int time;
#define PIN_SENSOR 2

void increment()          //interrupt service routine
{ 
  rev++;
}

void setup()
{
  Serial.begin(57600);
  pinMode(PIN_SENSOR ,INPUT_PULLUP);// Instead of using a pull up resistor
  attachInterrupt(digitalPinToInterrupt(PIN_SENSOR),increment,FALLING);  //attaching the interrupt
}
void loop()
{
  int junk = digitalRead(2);
  delay(2000);// 2 second delay
  detachInterrupt(0);           //detaches the interrupt while calculating
  time=millis()-oldtime;        //finds the time 
  rpm=(rev/time)*60000;         //calculates rpm
  oldtime=millis();             //saves the current time
  rev=0;
  Serial.print("\r"); Serial.println(rpm); 
  attachInterrupt(digitalPinToInterrupt(PIN_SENSOR),increment,FALLING);
}
