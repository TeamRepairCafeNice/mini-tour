#include <Arduino.h>
#include <UTFT.h>
#include "EtatCNC.h"
#include "Couleurs.h"
#include "Arial_round_16x24.h"
#include "Retro8x16.h"
#

#define DECIMALS 2

// Doit être déclaré au niveau module, pas comme membre privé.
UTFT LCD(ILI9486,38,39,40,41);

class DisplayTask : public Task
{
  public:
    DisplayTask(EtatCNC *_etatCNC) :
      Task(TaskTimeToMs(5)),
      pEtatCNC(_etatCNC)
      {
      }
      void afficherEcran()
      {
        dessinerEcran(pEtatCNC);
      }
private:
  EtatCNC *pEtatCNC;

  virtual bool OnStart()
  {
    LCD.InitLCD(PORTRAIT);
    LCD.fillScr(pEtatCNC->fond);
    dessinerEcran(pEtatCNC);
    return true;
  }

  virtual void OnStop() {};

  virtual void OnUpdate(uint32_t deltaTime)
  {
  }
    
  void dessinerEcran(EtatCNC *pEtatCNC, int xOffset = 0, int yOffset = 0)
  {
    for (int i = 0; i < pEtatCNC->nbWidgets; i++)
    {
      Widget widget =  pEtatCNC->ecran[i];
      if (widget.wModified) {
        LCD.setColor((!widget.wFocus) ? widget.wHiliteBackColor : widget.wBackColor);
        LCD.setBackColor((!widget.wFocus) ? widget.wHiliteBackColor : widget.wBackColor);
        switch(widget.wType)
        {
          case wRECT:
            LCD.fillRect(widget.wXPos + xOffset, widget.wYPos + yOffset, widget.wXPos + widget.wWidth, widget.wYPos + widget.wHeight);
            break;
          case wCRCL:
            int radius = (widget.wWidth + widget.wHeight)/4;
            LCD.fillCircle(widget.wXPos  + xOffset + radius, widget.wYPos + yOffset + radius, radius);
            break;
          case wRRCT: // Pas bon bug dans la lib
            LCD.fillRoundRect(widget.wXPos  + xOffset, widget.wYPos + yOffset, widget.wXPos + widget.wWidth, widget.wYPos + widget.wHeight);
            break;
        }
        // Texte de la case/du bouton
        LCD.setColor((!widget.wFocus) ? widget.wHiliteColor : widget.wColor);
        DisplayValue dv = widget.wDisplayValue;
        int labelWidth;
        int labelHeight;
        int xCenter = widget.wXPos + widget.wWidth / 2;
        int yCenter = widget.wYPos + widget.wHeight / 2;
         
        if (IntType == dv.dvType)
        {
            FontCode font = ((!widget.wFocus) ? widget.wHiliteFont : widget.wFont);
            switch(font)
            {
              case wFont_Arial_round_16x24:
                LCD.setFont(Arial_round_16x24);
                break;
              case wFont_Retro8x16:
                LCD.setFont(Retro8x16);
                break;
            }
            int wFont = LCD.getFontXsize();
            int hFont = LCD.getFontYsize();
            int value = dv.wValue.intValue;
            char *signe = (!widget.wWithSign) ? "" : (value < 0) ? "- " : (value > 0) ? "+ " : "  ";
            labelWidth = (strlen(signe) + 1 + log10(1.0 * abs(value))) * wFont; // log10(10) = 1 + 1 => 10 = 2 chiffres, +2 pour le signe => +3 en tout
            labelHeight = hFont;
            LCD.print(signe, xOffset + xCenter - labelWidth / 2, yOffset + yCenter - labelHeight / 2);
            LCD.printNumI(abs(value), xOffset + strlen(signe) * wFont + xCenter - labelWidth / 2, yOffset + yCenter - labelHeight / 2);
        }
        if (StringType == dv.dvType)
        {
          FontCode font = ((widget.wFocus) ? widget.wHiliteFont : widget.wFont);
          switch(font)
          {
            case wFont_Arial_round_16x24:
              LCD.setFont(Arial_round_16x24);
              break;
            case wFont_Retro8x16:
              LCD.setFont(Retro8x16);
              break;
          }
          int wFont = LCD.getFontXsize();
          int hFont = LCD.getFontYsize();
          char *value = dv.wValue.strValue;
          String strValue(value);
          int longueur = strlen(value);
          if (Paysage == widget.wOrientation)
          {
            int iNL = strValue.indexOf('|');
            int nbLignes = 1 + (iNL != -1); // si NL 2 lignes, sinon 1
            labelWidth = wFont * max(iNL, longueur - iNL - 1);
            labelHeight = hFont * nbLignes;
            LCD.print(strValue.substring(0, iNL).c_str(), xOffset + xCenter - labelWidth / 2, yOffset + yCenter - labelHeight / 2);
            if (nbLignes > 1)
            {
              LCD.print(strValue.substring(iNL + 1, longueur).c_str(), xOffset + xCenter - labelWidth / 2, hFont + 1 + yOffset + yCenter - labelHeight / 2);
            }
          }
          else // Portrait
          {
            labelWidth = wFont;
            labelHeight = (hFont + 1) * longueur;
            for (int i = 0; i < longueur; i++)
            {
              LCD.print(strValue.substring(i, i+1).c_str(), xOffset + xCenter - labelWidth / 2, (hFont + 1) * i + yOffset + yCenter - labelHeight / 2);
            }
          }
        }
        pEtatCNC->ecran[i].wModified = false;
      } // if modified
    } // for
  }
};
