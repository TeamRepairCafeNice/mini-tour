#define __WIDGET__
// RGB565 : Entier 16-bit
// R 5 bits
// G 6 bits
// B 5 bits
#ifndef RGB565
#include "Couleurs.h"
#endif
// Code arbitraire pour les formes
enum WidgetType
{
  wRECT = 1,
  wROND = 2,
  wRRCT = 3
};
// Trop compliqué de passer une adresse pour la fonte
// donc on utilise un code à nous.
enum FontCode 
{
  wFont_Arial_round_16x24 = 1,
  wFont_Retro8x16 = 2
};
struct Widget
{
    WidgetType wType;
    bool selected;
    bool modified;
    bool zeroed;
    int wXPos;
    int wYPos;
    int wWidth;
    int wHeight;
    RGB565 wBackColor; 
    RGB565 wColor;
    FontCode wFont;
};
