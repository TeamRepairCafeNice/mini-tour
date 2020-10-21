#define __MODE_MANUEL__
////////////////////////////
/// Design Simplifié ///////
/// Yves & JP //////////////
////////////////////////////
#ifndef __ECRAN__
#include "ecran.h"
#endif
/////////////////////////////////////////////////
#ifndef __ROTARY_ENCODER_TASK__
#include "RotaryEncoderTask.h"
// Declaration forward des fonctions passées aux constructeurs des Tâches
/// Rotary Encoder
void HandleButtonChanged(EncoderButtonState state);
void HandleRotationChanged(int8_t rotationDelta);
RotaryEncoderTask RotaryTask(HandleRotationChanged,
                             HandleButtonChanged,
                             PIN_CLOCK,
                             PIN_DATA,
                             PIN_BUTTON);
#endif
/////////////////////////////////////////////////
#ifndef __STEPPER_TASK__
#include "StepperTask.h"
#endif
#ifndef __BUTTON_TASK__
#include "ButtonTask.h"
#endif
///////// Debug ///////////////
void logPrompt(char *text)
{ 
  if (Serial) 
  {
    Serial.println(text);
  }
}

void logPromptValue(char *prompt, int value)
{
  if (Serial) 
  {
  Serial.print(prompt); Serial.println(value);
  }
}
///////////////////////////////
uint8_t microsteps = MU_STEPS;
float avance = AVANCE_MM_PAR_MIN; 
float pas = PAS_MM_PAR_TOUR;
int vitesseRotation = int((avance + 0.5) / pas);
int nouvelleVitesse = vitesseRotation; 
void HandleXLimitButtonChanged(ButtonState state);
/// Détecteur fin de course

ButtonTask xLimitDetectionTask(
  HandleXLimitButtonChanged,
  PIN_X_LIMIT
);

// Axe X
StepperTask xStepperTask(
  X, 
  STEPS * MU_STEPS, 
  vitesseRotation, 
  PIN_DIR_X, 
  PIN_STEP_X
); 
////////// Commandes //////////
void commandeStopX()
{
  xStepperTask.ajusteToursParMinute(0);
  logPrompt(">>>>>>>>>>> X stoppé");
}
void commandeStopZ()
{
  logPrompt(">>>>>>>>>>> Z stoppé");
}
///////////////////////////////
bool isHome(int axis)
{
  return etat.homed[axis - 1];
}
bool isActiveWidget(int i)
{
  return etat.ecran[i].wEditable && etat.ecran[i].wFocus;
}

void desactiver(int aDefocusser)
{
  etat.ecran[aDefocusser].wFocus = false;
  etat.ecran[aDefocusser].wModified = true;
}

void activer(int aFocusser)
{
  etat.ecran[aFocusser].wFocus = true;
  etat.ecran[aFocusser].wModified = true;
}
void designerActif(int widget)
{
  etat.wWidgetActifndx = widget;
}
///////////////////////////////
void HandleButtonChanged(EncoderButtonState state)
{
  int wActif = etat.wWidgetActifndx;
  if (EncoderButtonState_Pressed == state)
  {
    switch (wActif)
    {
      case X: 
        commandeStopX();        
        break;
      case Z:
        logPrompt("Z stoppé");
        break;
      case HOME:
      logPrompt("Retour maison");
        // Tant que !etat.homed[0] avancer à la vitesse courante
        break;
    }
    if (isActiveWidget(DEPL_1))
    {
      desactiver(DEPL_1);
      activer(DEPL_2);
      activer(X);
      logPrompt("Déplacement Usinage");
    }
    else if (isActiveWidget(DEPL_2))
    {
      desactiver(DEPL_2);
      activer(DEPL_3);
      activer(X);
      logPrompt("STOP");
    }
    else if (isActiveWidget(DEPL_3))
    {
      desactiver(DEPL_3);
      activer(DEPL_1);
      activer(X);
      logPrompt("Déplacement Outil tangenter");
    }
  }
  else if (state == EncoderButtonState_AutoRepeat)
  {
    etat.nbClicks++;
    if (0 == etat.nbClicks % 10)
    {
      switch (wActif)
      {
      case X:
        desactiver(X);
        activer(Z);
        designerActif(Z);
        break;
      case Z:
        desactiver(Z);
        activer(X);
        activer(HOME);
        designerActif(HOME);
        break;
      case HOME:
        desactiver(HOME);
        activer(TOOL1);
        designerActif(TOOL1);
        break;
      case TOOL1:
        desactiver(TOOL1);
        activer(TOOL2);
        designerActif(TOOL2);
        break;
      case TOOL2:
        desactiver(TOOL2);
        activer(TOOL3);
        designerActif(TOOL3);
        break;
      case TOOL3:
        desactiver(TOOL3);
        activer(TOOL4);
        designerActif(TOOL4);
        break;
      case TOOL4:
        desactiver(TOOL4);
        activer(TOOL1);
        activer(DEPL_1);
        designerActif(DEPL_1);
        break;
      case DEPL_1:
        desactiver(DEPL_1);
        activer(DEPL_2);
        designerActif(DEPL_2);
        break;
      case DEPL_2:
        desactiver(DEPL_2);
        activer(DEPL_3);
        designerActif(DEPL_3);
        break;
      case DEPL_3:
        desactiver(DEPL_3);
        activer(DEPL_1);
        activer(X);
        designerActif(X);
        break;
      }
      logPromptValue("Changement de focus -> ", etat.wWidgetActifndx);
    }
  }
  else if (state == EncoderButtonState_Tracking)
  {
    logPrompt("Tracking");
  }

}

void ajusterValeur(int valeurLue, int *vitesse)
{
  int signe = (valeurLue < 0) ? -1 : (valeurLue > 0);
  int increment = signe * (isActiveWidget(DEPL_1) ? 10 : 1);
  (*vitesse) += increment;
  logPromptValue("Nouvelle vitesse RPM: ", *vitesse);
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
  ajusterValeur(etat.valeurLue, &vitesseRotation);
}

//// ButtonTask ////////////
void HandleXLimitButtonChanged(ButtonState state)
{
  if(ButtonState_Pressed == state)
  {
    etat.homed[0] = true;
  }
}
