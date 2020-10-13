#include <Task.h>
#include "DisplayTask.h"
#include "RotaryEncoderTask.h"

// Encoder
#define ClockPin  4  // labeled either as CLK or as A
#define DataPin   3  // labeled either as DT or as B
#define ButtonPin 2  // labeled as SW

// foreward declare functions passed to task constructors now required
void HandleButtonChanged(EncoderButtonState state);
void HandleRotationChanged(int8_t rotationDelta);


RotaryEncoderTask RotaryTask(HandleRotationChanged,
    HandleButtonChanged,
    ClockPin,
    DataPin,
    ButtonPin);
    
EtatCNC etat = { 123, 256, 1, 0, 100, 1500, true, false, false, false, false, false, false, true, false, false};
DisplayTask affichage(&etat);
TaskManager taskManager;
uint16_t clics = 0;

void setup() 
{
 Serial.begin(57600);
 traceEtat();
 taskManager.StartTask(&affichage);
 taskManager.StartTask(&RotaryTask);
}

void loop() 
{
 taskManager.Loop();
}

void assignerValeur(int valeurLue)
{
  majSelection(clics++);     
  affichage.afficheValeurs(&etat);
  traceEtat();
}

void afficherValeur(int valeurLue)
{
  if(etat.xSelected) 
  {
    etat.xCoordMM += calculIncrement(valeurLue);
  } 
  else if(etat.zSelected)
  {
    etat.zCoordMM += calculIncrement(valeurLue);
  }
  else if(etat.aSelected)
  {
    etat.zAvanceMMpm += calculIncrement(valeurLue);
  }  
  affichage.afficheValeurs(&etat);
  traceEtat();
}

void majSelection(uint16_t n) {
  etat.xSelected = false;
  etat.zSelected = false;
  etat.aSelected = false;  
  switch(n % 3) {
    case 0: etat.xSelected = true; etat.sauvegarde = etat.xCoordMM; break;
    case 1: etat.zSelected = true; etat.sauvegarde = etat.zCoordMM; break;
    case 2: etat.aSelected = true; etat.sauvegarde = etat.zAvanceMMpm; break;
  }
}
/**
 * Callabcks de l'encodeur rotatif
 */
void HandleButtonChanged(EncoderButtonState state)
{
    if (state == EncoderButtonState_Pressed)
    {
      assignerValeur(etat.valeurLue);
      Serial.println("Pressed - ");
    }
    else if (state == EncoderButtonState_Released)
    {
        Serial.println("Released - ");
    } 
    else
    {
        Serial.println("Auto-repeat - ");
    }
}

void HandleRotationChanged(int8_t rotationDelta)
{
    int valeur = RotaryTask.RotationValue();
    if (rotationDelta > 0)
    {
        etat.valeurLue = -abs(valeur);
    }
    else
    {
        etat.valeurLue = abs(valeur);
    }
    
    afficherValeur(etat.valeurLue);
    Serial.println(etat.valeurLue);
}
int calculIncrement(int valeurLue)
{
  int signe =(valeurLue < 0) ? -1 : (valeurLue > 0);
  if (etat.mlSelected)
  {
    return (etat.xSelected || etat.zSelected) ? signe * 1 : signe * 5;
  }
  else if (etat.mrSelected)
  { 
    return (etat.xSelected || etat.zSelected) ? signe * 10 : signe * 100;    
  }
  return signe;
}
void traceN(char *prompt, int n) 
{
  Serial.print(prompt); Serial.print(n);
}
void traceEtat()
{
  traceN("\t| X:", etat.xCoordMM);
  traceN("\t| Z: ", etat.zCoordMM);
  traceN("\t| A:", etat.zAvanceMMpm);
  Serial.println();
}
