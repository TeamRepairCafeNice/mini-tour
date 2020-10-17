#define __ETAT_CNC__
#ifndef __WIDGET__
#include "Widget.h"
#endif
#ifndef __COULEURS__
#include "Couleurs.h"
#endif
// Les widget contiennet une copie de la valeurs des paramètres d'état
// C'est seulement quand on valide l'affichage (bouton Start)
// Que les données fraîchement éditées sont recopiées dans l'état.
// Dans la programamtion classique de GUI, c'est le rôle du contrôleur
// de faire le lien entre l'état du "modèle" et la vue affichée.
#define ModelViewLinkID int
// Le trio est abrégé par les 3 lettres MVC Model-View-Controller
struct ModelViewLink
{
  ModelViewLinkID mvlID;
  char *name;
  int modelParam;
  WidgetID viewWidget;
};

struct EtatCNC
{
  RGB565 fond;
  WidgetID wWidgetActifndx;
  int valeurLue;
  int nbClicks;
  int nbWidgets;
  Widget *ecran;
};
