#define __ETAT_CNC__

struct EtatCNC 
{
  int xCoordMM;
  int zCoordMM;
  int outil;
  int xAvanceMMpm;
  int zAvanceMMpm;
  int brocheRPM;
  bool xSelected;
  bool xZero;
  bool zSelected;
  bool zZero;
  bool aSelected;
  bool aZero;
  bool autoSelected;
  bool mrSelected;
  bool mlSelected;
  bool started;
  int sauvegarde;
  int valeurLue; // peut-être abandonnée
};
