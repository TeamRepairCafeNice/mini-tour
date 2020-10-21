#include <Task.h>
#include "DisplayTask.h"

#define SERIAL_BAUDS        (57600)
#define X                   (1)
#define STEPS               (800)
#define MU_STEPS            (32) 
#define AVANCE_MM_PAR_MIN   (80)
#define PAS_MM_PAR_TOUR     (2.0)

// PINS
#define PIN_BUTTON          (2) // labeled as SW
#define PIN_DATA            (3) // labeled either as DT or as B
#define PIN_CLOCK           (4) // labeled either as CLK or as A
#define PIN_DIR_X           (5)
#define PIN_STEP_X          (6)
#define PIN_X_LIMIT         (7)
#define INC_SPEED           (10)

// Encoder
#define ClockPin 4  // labeled either as CLK or as A
#define DataPin 3   // labeled either as DT or as B
#define ButtonPin 2 // labeled as SW


// Choix de la mise en page
#include "modeClassique.h"
DisplayTask affichage(&etat); // Création de la tâche


///////// The Boss /////////////////////
TaskManager taskManager;
////////////////////////////////////////
void setup()
{
  Serial.begin(SERIAL_BAUDS);
  taskManager.StartTask(&affichage); // Affiche l'ecran dasn onStart
  taskManager.StartTask(&RotaryTask);
#ifdef __MODE_MANUEL__
  taskManager.StartTask(&xStepperTask);
  taskManager.StartTask(&xLimitDetectionTask);
#endif
}

void loop()
{
  affichage.afficherEcran();
  taskManager.Loop();
}
//////////// Utilitaire usage général /////////
/**
 * Signe d'un entier, retourne
 * -1 si négatif
 *  0 si nul
 *  1 si positif
 */
 
int sgn(int n)
{
  return (n < 0) ? -1 : (n > 0);
}
//////////////////////////////////////// UTILITAIRES DEBUG ///////////////////////////////
