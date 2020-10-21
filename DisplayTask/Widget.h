#define __WIDGET__
#ifndef __COULEURS__
#include "Couleurs.h"
#endif

enum WidgetType
{
  wRECT = 1,
  wCRCL = 2,
  wRRCT = 3,
  wJAGG = 4, // Jauge zéro à gauche
  wJAGC = 5  // Jauge à zéro central
};

enum FontCode
{
  wFont_Arial_round_16x24 = 1,
  wFont_Retro8x16 = 2
};

enum Orientation
{
  Paysage = 0,
  Portrait = 1
};
// Ne contient qu'une seule valeur, mais de type variable
union WidgetValue
{
  int    intValue;
  double dblValue;
  char *strValue;
};
enum ValueType
{
  IntType = 1,
  FloatType = 2,
  StringType = 3
};
struct DisplayValue
{
  ValueType dvType;
  WidgetValue wValue;
};

#define WidgetID int
struct Widget
{
  WidgetID      wID;
  WidgetType    wType;
  bool          wEditable;
  int           wXPos;
  int           wYPos;
  int           wWidth;
  int           wHeight;
  RGB565        wBackColor;
  RGB565        wHiliteBackColor;
  RGB565        wColor;
  RGB565        wHiliteColor;
  FontCode      wFont;          // non sélectionné
  FontCode      wHiliteFont;    // Séléctionné
  DisplayValue  wDisplayValue;
  Orientation   wOrientation;
  bool          wWithSign;      // Signe séparé ou pas de signe
  bool          wFocus;         // Sélectionné
  bool          wModified;      // A besoin d'un rafraichissement écran
};
