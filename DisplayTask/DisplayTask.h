#include <Arduino.h>
#include <UTFT.h>
#include "EtatCNC.h"
#include "Couleurs.h"
#include "Arial_round_16x24.h"
#include "Retro8x16.h"

#define DECIMALS 2

// Doit être déclaré au niveau module, pas comme membre privé.
UTFT LCD(ILI9486,38,39,40,41);
// Declare which fonts we will be using

class DisplayTask : public Task 
{
  public:
    DisplayTask(EtatCNC *_etatCNC) :
      Task(TaskTimeToMs(5)),
      pEtatCNC(_etatCNC)
      {
      };
      void afficheValeurs(EtatCNC *pEtatCNC)
      {
        afficherFond(pEtatCNC);
      }
private:
  EtatCNC *pEtatCNC;
  int yOffset = -25;
     
  void printFloat(float f, int x, int y)
  {
    char buffer[8];
    LCD.print(dtostrf(f, 7, DECIMALS, buffer), x, y);    
  }
    
  virtual bool OnStart()
  { 
    LCD.InitLCD(PORTRAIT);
    LCD.clrScr();
    afficherFond(pEtatCNC);
    return true; 
  };
  
  virtual void OnStop() {};

  virtual void OnUpdate(uint32_t deltaTime)
  {
    // UpdateValues(pEtatCNC);
  };

  void Fill_Rect(int x, int y, int w, int h, RGB565 couleur)
  {
    LCD.setColor(couleur);
    LCD.fillRect(x, y + yOffset , x + w, y + yOffset + h);
  }

  void Print_String(char *str, int x, int y)
  {
    LCD.print(str, x, y + yOffset);
  }

  void Print_Number(int value, int x, int y, bool withSign = true)
  {
    char *sign = (value > 0) ? "+ " : (value < 0) ? "- " : "  "; 
    if (withSign) LCD.print(sign, x, y + yOffset);
    int xOffset = 2 * LCD.getFontXsize();
    LCD.printNumI(abs(value), x + xOffset, y + yOffset);
  }

 void Fill_Circle(int x, int y, int radius)
  {
    LCD.fillCircle(x, y + yOffset, radius);  
  }
  
  void afficherFond(EtatCNC* pEtat)
  {
    LCD.setFont(Arial_round_16x24);
    LCD.fillScr(BLACK);
    for (int i = 0; i < 4; i++)
    {
      RGB565 couleur = (i + 1 == pEtat->outil) ? GREEN : MAGENTA;
      LCD.setColor(couleur);
      LCD.setBackColor(couleur);
      Fill_Circle(50 + i * 60, 325, 15);
      LCD.setColor(BLACK);
      LCD.printNumI(i + 1, 42 + 60 * i, 315 + yOffset);
    }
    LCD.setColor((pEtat->aSelected)?GREEN:MAGENTA);
    Fill_Circle(47,67,25);
    LCD.setColor(BLACK);
    
    LCD.setFont((pEtat->aSelected) ? Arial_round_16x24 : Retro8x16);
    LCD.setBackColor((pEtat->aSelected)?GREEN:MAGENTA);
    Print_String("A",40,55);

    Fill_Rect(12,380,75,111,BLUE);//.......Home
    Fill_Rect(95,42,130,51,GREEN);//......X
    Fill_Rect(95,102,130,51,GREEN);//.....Z
    Fill_Rect(95,162,130,51,WHITE);//.....Vit mot
    Fill_Rect(140,410,130,40,GREEN); // Home
    
    Fill_Rect(95,243,130,51,GREEN);//.....Avance
    Fill_Rect(232,42,75,51,YELLOW);//.....X
    Fill_Rect(232,102,75,51,YELLOW);//....Z
    Fill_Rect(232,243,75,51,YELLOW);
    LCD.setColor(WHITE);
    LCD.setBackColor(BLUE);
    LCD.setFont(Arial_round_16x24);
    Print_String("H",41,387); 
    Print_String("O",41,411);
    Print_String("M",41,435);
    Print_String("E",41,459); 
    LCD.setColor(BLACK);
    
    LCD.setBackColor(YELLOW);
    LCD.setFont((pEtat->xSelected) ? Arial_round_16x24 : Retro8x16);
    Print_String("X",265,55);//******************************
    LCD.setFont((pEtat->zSelected) ? Arial_round_16x24 : Retro8x16);
    Print_String("Z",265,116);// ..............     Boutons Jaunes  de remise a ZERO 
    LCD.setFont((pEtat->aSelected) ? Arial_round_16x24 : Retro8x16);
    Print_String("AV",250,255);//****************************

    
    LCD.setBackColor(GREEN);
    LCD.setFont(Arial_round_16x24);
    Print_Number(pEtat->xCoordMM,110,55);//.................Valeur de X
    Print_Number(pEtat->zCoordMM,110,116);//................Valeur de Z
    Print_Number(pEtat->zAvanceMMpm,110,255, false);//................Valeur de l'Avance
    LCD.setBackColor(WHITE);
    Print_Number(pEtat->brocheRPM,110,176, false);//................Valeur Vitesse moteur

    
    LCD.setBackColor((pEtat->mrSelected)?GREEN:MAGENTA);
    Fill_Rect(20,102,51,51,(pEtat->mrSelected)?GREEN:MAGENTA);//......MR
    LCD.setColor(BLACK); 
    LCD.setFont((pEtat->mrSelected) ? Arial_round_16x24 : Retro8x16);  
    Print_String("MR",30,111);//.....................Appui mode deplacement Rapide

    LCD.setBackColor((pEtat->mlSelected)?GREEN:MAGENTA);
    Fill_Rect(20,163,51,51,(pEtat->mlSelected)?GREEN:MAGENTA);//......ML
    LCD.setColor(BLACK);
    LCD.setFont((pEtat->mlSelected) ? Arial_round_16x24 : Retro8x16);  
    Print_String("ML",30,175);//.....................Appui mode deplacement Lent
    LCD.setBackColor(GREEN);
    LCD.setFont(Arial_round_16x24);
    Print_String("START",160,419); // Start

    LCD.setBackColor(BLACK); 
    LCD.setColor(WHITE);
    Print_String("VM",250,176);
    
    LCD.setFont(Retro8x16);
    Print_String("Mm/mn",26,255);
    Print_String("Avance",20,270);
  } 

};
