#include <Arduino.h>
#include <LCDWIKI_GUI.h> //Core graphics library
#include <LCDWIKI_KBV.h> //Hardware-specific library
#include "EtatCNC.h"
#include "Couleurs.h"
#include "Arial_round_16x24.h"
#include "Retro8x16.h"
#

#define DECIMALS 2

// Doit être déclaré au niveau module, pas comme membre privé.
LCDWIKI_KBV LCD(ILI9486,A3,A2,A1,A0,A4);

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
    LCD.Init_LCD();
    LCD.Fill_Screen(pEtatCNC->fond);
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
        LCD.Set_Draw_color((!widget.wFocus) ? widget.wHiliteBackColor : widget.wBackColor);
        LCD.Set_Text_Back_colour((!widget.wFocus) ? widget.wHiliteBackColor : widget.wBackColor);
        switch(widget.wType)
        {
          case wRECT:
            LCD.Fill_Rectangle(widget.wXPos + xOffset, widget.wYPos + yOffset, widget.wXPos + widget.wWidth, widget.wYPos + widget.wHeight);
            break;
          case wCRCL:
            int radius = (widget.wWidth + widget.wHeight)/4;
            LCD.Fill_Circle(widget.wXPos  + xOffset + radius, widget.wYPos + yOffset + radius, radius);
            break;
        }
        // Texte de la case/du bouton
        LCD.Set_Text_colour((!widget.wFocus) ? widget.wHiliteColor : widget.wColor);
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
                // LCD.setFont(Arial_round_16x24);
                LCD.Set_Text_Size(2);
                break;
              case wFont_Retro8x16:
                // LCD.setFont(Retro8x16);
                LCD.Set_Text_Size(1);
                break;
            }
            int wFont = (LCD.Get_Text_Size() == 2) ? 16 : 8;
            int hFont = (LCD.Get_Text_Size() == 2) ? 24 : 16;
            int value = dv.wValue.intValue;
            char *signe = (!widget.wWithSign) ? "" : (value < 0) ? "- " : (value > 0) ? "+ " : "  ";
            labelWidth = (strlen(signe) + 1 + log10(1.0 * abs(value))) * wFont; // log10(10) = 1 + 1 => 10 = 2 chiffres, +2 pour le signe => +3 en tout
            labelHeight = hFont;
            LCD.Print_String(signe, xOffset + xCenter - labelWidth / 2, yOffset + yCenter - labelHeight / 2);
            LCD.Print_Number_Int((long)abs(value), (int16_t)(xOffset + strlen(signe) * wFont + xCenter - labelWidth / 2), (int16_t)(yOffset + yCenter - labelHeight / 2), 3, ' ', 10);
        }
        if (StringType == dv.dvType)
        {
          FontCode font = ((widget.wFocus) ? widget.wHiliteFont : widget.wFont);
          switch(font)
          {
            case wFont_Arial_round_16x24:
              LCD.Set_Text_Size(2);
              break;
            case wFont_Retro8x16:
              LCD.Set_Text_Size(1);
              break;
          }
          int wFont = (LCD.Get_Text_Size() == 2) ? 16 : 8;
          int hFont = (LCD.Get_Text_Size() == 2) ? 24 : 16;
          char *value = dv.wValue.strValue;
          String strValue(value);
          int longueur = strlen(value);
          if (Paysage == widget.wOrientation)
          {
            int iNL = strValue.indexOf('|');
            int nbLignes = 1 + (iNL != -1); // si NL 2 lignes, sinon 1
            labelWidth = wFont * max(iNL, longueur - iNL - 1);
            labelHeight = hFont * nbLignes;
            LCD.Print_String(strValue.substring(0, iNL).c_str(), xOffset + xCenter - labelWidth / 2, yOffset + yCenter - labelHeight / 2);
            if (nbLignes > 1)
            {
              LCD.Print_String(strValue.substring(iNL + 1, longueur).c_str(), xOffset + xCenter - labelWidth / 2, hFont + 1 + yOffset + yCenter - labelHeight / 2);
            }
          }
          else // Portrait
          {
            labelWidth = wFont;
            labelHeight = (hFont + 1) * longueur;
            for (int i = 0; i < longueur; i++)
            {
              LCD.Print_String(strValue.substring(i, i+1).c_str(), xOffset + xCenter - labelWidth / 2, (hFont + 1) * i + yOffset + yCenter - labelHeight / 2);
            }
          }
        }
        pEtatCNC->ecran[i].wModified = false;
      } // if modified
    } // for
  }
};
