#include <Task.h>
#include "DisplayTask.h"
#include "RotaryEncoderTask.h"

// Encoder
#define ClockPin  4  // labeled either as CLK or as A
#define DataPin   3  // labeled either as DT or as B
#define ButtonPin 2  // labeled as SW


// 

// Declaration forward des fonctions passées aux constructeurs des Tâches

void HandleButtonChanged(EncoderButtonState state);
void HandleRotationChanged(int8_t rotationDelta);

RotaryEncoderTask RotaryTask(HandleRotationChanged,
    HandleButtonChanged,
    ClockPin,
    DataPin,
    ButtonPin);

#include "ecran.h"

DisplayTask affichage(&etat); // Création de la tâche

TaskManager taskManager;

void setup()
{
  Serial.begin(57600);
  taskManager.StartTask(&affichage); // Affiche l'ecran dasn onStart
  taskManager.StartTask(&RotaryTask);
}

void loop()
{
  taskManager.Loop();
}
//////// Les commandes click sur un champ /////
void commandeLireValeur(int widgetEditable) 
{
  assignerValeur(widgetEditable);
}

void commandeHome()
{
  Serial.println("Exécution de Home");
}

void commandeStart()
{
  Serial.println("Exécution de Start");
}

void commandeChangerOutil(int outil)
{
  traceN("Exécution de Changement d'outil, sélection outil n° ", outil);
}

void assignerValeur(int widget)
{
  
  
}
///////////////////////////////////////////////
bool isActiveWidget(int i)
{
  return etat.ecran[i].wEditable && etat.ecran[i].wFocus;
}

void desactiver(int aDefocusser) {
  etat.ecran[aDefocusser].wFocus = false;
  etat.ecran[aDefocusser].wModified = true;  
}

void activer(int aFocusser) {
  etat.ecran[aFocusser].wFocus = true;
  etat.ecran[aFocusser].wModified = true;  
}

void ajusterValeur(int valeurLue, int widget)
{
  int signe =(valeurLue < 0) ? -1 : (valeurLue > 0);
  int increment = 0;
  switch(widget)
  {
    case X :
      // prise en compte MR et ML
      increment = signe * (isActiveWidget(MR) ? 10 : 1);
      break;
    case Z : 
      // prise en compte MR et ML
      increment = signe * (isActiveWidget(MR) ? 10 : 1);
      break;
    case AV : 
      // increment constant
      increment = signe * 100;
  }
  etat.ecran[widget].wDisplayValue.wValue.intValue += increment;
  etat.ecran[widget].wModified = true;  
}

/**
 * Callabcks de l'encodeur rotatif
 */
void HandleButtonChanged(EncoderButtonState state)
{
    int wActif = etat.wWidgetActifndx;
    if (state == EncoderButtonState_Pressed)
    {
      switch(wActif)
      {
        case X:
          commandeLireValeur(X);
          break;
        case Z:
          commandeLireValeur(Z);
          break;
        case AV:
          commandeLireValeur(AV);
          break;
        case TOOL1: 
          commandeChangerOutil(TOOL1);
          break;
        case TOOL2:
          commandeChangerOutil(TOOL2);
          break;
        case TOOL3:
          commandeChangerOutil(TOOL3);
          break;
        case TOOL4:
          commandeChangerOutil(TOOL4);
          break;
        case HOME:
          commandeHome();
          break;
        case START:
          commandeStart();
          break;
      }
      // Toujours en plus des autres commandes
      if (isActiveWidget(MR))
      {        
          desactiver(MR);
          activer(ML);
      } 
      else if (isActiveWidget(ML))
      {
          desactiver(ML);
          activer(MR);
      }
    }
    else if (state == EncoderButtonState_AutoRepeat)
    {
      etat.nbClicks++;
      if (0 == etat.nbClicks % 10) 
      {
        switch(wActif)
        {
          case X:
            desactiver(X);
            desactiver(X_LBL);
            activer(Z);
            activer(Z_LBL);
            etat.wWidgetActifndx = Z;
            break;
          case Z :       
            desactiver(Z);
            desactiver(Z_LBL);
            activer(AV);
            activer(AV_LBL);
            etat.wWidgetActifndx = AV;
            break;
          case AV:
            desactiver(AV);
            desactiver(AV_LBL);
            activer(TOOL1);
            etat.wWidgetActifndx = TOOL1;
            break;
          case TOOL1:
            desactiver(TOOL1);
            activer(TOOL2);
            etat.wWidgetActifndx = TOOL2;            
            break;
          case TOOL2:
            desactiver(TOOL2);
            activer(TOOL3);
            etat.wWidgetActifndx = TOOL3;            
            break;
          case TOOL3:
            desactiver(TOOL3);
            activer(TOOL4);
            etat.wWidgetActifndx = TOOL4;            
            break;
          case TOOL4:
            desactiver(TOOL4);
            activer(TOOL1);
            activer(HOME);
            etat.wWidgetActifndx = HOME;
            break;
          case HOME:
            desactiver(HOME);
            activer(START);
            etat.wWidgetActifndx = START;
            break;
          case START:
            desactiver(START);
            activer(X);
            activer(X_LBL);
            etat.wWidgetActifndx = X;
            break;
        }
       traceN("Changement de focus -> ", etat.wWidgetActifndx);
      }
    }
    affichage.afficherEcran();
}

void HandleRotationChanged(int8_t rotationDelta)
{
    int valeur = RotaryTask.RotationValue();
    int wActif = etat.wWidgetActifndx;
    if (rotationDelta > 0)
    {
      etat.valeurLue = -abs(valeur);
    }
    else
    {
      etat.valeurLue = abs(valeur);
    }
    ajusterValeur(etat.valeurLue, wActif);
    affichage.afficherEcran();
}

//////////////////////////////////////// UTILITAIRES DEBUG ///////////////////////////////
void traceN(char *prompt, int n)
{
  if (Serial) 
  {
    Serial.print(prompt); Serial.print(n);
    Serial.println();
  }
}
  
void traceEtat()
{
  
}

void traceWidgetValue(Widget w)
{
  if (Serial)
  {
    Serial.print("wID :");Serial.print(w.wID);
    Serial.print("\t| Value: ");Serial.print((w.wDisplayValue.dvType == IntType) ? String(w.wDisplayValue.wValue.intValue).c_str() : w.wDisplayValue.wValue.strValue);
    Serial.print("\t| Focus: ");Serial.print((w.wFocus == IntType) ? "OUI" : "non"); 
    Serial.print("\t| Modifié: ");Serial.print((w.wModified == IntType) ? "OUI" : "non"); 
    Serial.println();
  }
}
void traceDonneesEcran()
{
  for (int i = 0; i < etat.nbWidgets; i++)
  {
    traceWidgetValue(ecran[i]);  
  }
}
  
