#define __ETAT_CNC__
#ifndef __WIDGET__
#include "Widget.h"
#endif
#ifndef __COULEURS__
#include "Couleurs.h"
#endif

struct EtatCNC
{
  RGB565 fond;
  WidgetID wWidgetActifndx;
  int valeurLue;
  int nbClicks;
  bool *homed;
  int nbWidgets;
  Widget *ecran;
};
