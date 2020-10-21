#define __STEPPER_TASK__
#include <Stepper.h>
      
#define motorInterfaceType 1
class StepperTask : public Task
{
public:
  // Constructor : nbPass any custom arguments you need
  StepperTask(int id, int uSteps, int rpm, int pinDir, int pinStep) :
    Task(MsToTaskTime(5)), 
    axe(id),
    nbMicroPasParTour(uSteps), // uSteps : si 200 nbPas/tr par quart de nbPas uSteps = 800 
    toursParMinute(rpm),
    dirPin(pinDir),
    stpPin(pinStep),
    moteurPaP(uSteps, pinDir, pinStep)
    { 
    };

    void marcheArriere() {
      digitalWrite(dirPin, LOW);
    }

    void marcheAvant() {
      digitalWrite(dirPin, HIGH);
    }

    void ajusteToursParMinute(int rpm)
    {
      toursParMinute = rpm;
      if (rpm < 0) {
        marcheArriere();
      } 
      else
      {
        marcheAvant();
      }
      moteurPaP.setSpeed(abs(rpm));
    }
    
    int lireToursParMinute()
    {
      return toursParMinute;
    }

    void retourneMaison(int _speedReversed, int _muReversed) 
    {
      sauveToursParMinute = toursParMinute;
      muReversed = _muReversed;
      ajusteToursParMinute(-_speedReversed);
    }

    
private:
  // put member variables here that are scoped to this object
  int axe;
  int nbMicroPasParTour;
  int toursParMinute;
  int sauveToursParMinute = toursParMinute;
  int dirPin;
  int stpPin;
  int muReversed = 0;
  Stepper moteurPaP;
  
  virtual bool OnStart() // optional
  {
    // put code here that will be run when the task starts 
    ajusteToursParMinute(toursParMinute);   
    return true;
  }

  virtual void OnStop() // optional
  {
    // put code here that will be run when the task stops
    // moteurPaP.setSpeed(0);
  }

  virtual void OnUpdate(uint32_t deltaTime)
  {
    uint32_t deltaTimeMs = TaskTimeToMs(deltaTime);
    /*
    logPromptValue("deltaTimeMs : ", deltaTimeMs);
    logPromptValue("toursParMinute: ", toursParMinute);
    logPromptValue("nbMicroPasParTour : ", nbMicroPasParTour);
    */
    int steps = sgn(toursParMinute) * max(1, abs(int((1.0 * toursParMinute * nbMicroPasParTour * deltaTimeMs) / 60000.0)));
    if (muReversed > 0)
    {
       muReversed += steps;
    } 
    else if (muReversed < 0)
    {
      steps = muReversed;
      muReversed = 0;
      ajusteToursParMinute(sauveToursParMinute);   
    }
    
    // logPromptValue("Steps: ", steps);
    moteurPaP.step(steps);
  }

  int sgn(int n) 
  {
    return (n < 0) ? -1 : (n > 0);
  }

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
};
