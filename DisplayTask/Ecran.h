#define __ECRAN__
#ifndef __WIDGET__
#include "Widget.h"
#endif
/**
enum FontCode
{
  wFont_Arial_round_16x24 = 1,
  wFont_Retro8x16 = 2
};
enum WNdxgetType
{
  wRECT = 1,
  wCRCL = 2,
  wRRCT = 3
};
struct WNdxget
{
  int           wNdx;
  WNdxgetType    wType;
  bool          wEditable;
  int           wXPos;
  int           wYPos;
  int           wWNdxth;
  int           wHeight;
  RGB565        wBackColor;
  RGB565        wHiliteBackColor;
  RGB565        wColor;
  RGB565        wHiliteColor;
  FontCode      wFont;          // non sélectionné
  FontCode      wHiliteFont;    // Séléctionné
  DisplayValue  wValue;
  Orientation   wOrientation;
  bool          wWithSign;      // Signe séparé ou pas de signe
  bool          wFocus;         // Sélectionné
  bool          wModified;      // A besoin d'un rafraichissement écran
};
 */
Widget ecran[] =
{
  { 0, wRECT, true, 20,20,51,51, GREEN, DRKGRN, BLACK, BLACK,  wFont_Retro8x16, wFont_Arial_round_16x24, {StringType, "/1"}, Paysage, false, true, true }, // Par 1
  
  { 1, wRECT, true, 95,20,130,51, GREEN, DRKGRN, BLACK, BLACK,  wFont_Arial_round_16x24, wFont_Arial_round_16x24, {IntType, 123}, Paysage, true, true, true }, // X value
  { 2, wRECT, true, 232,20,75,51, YELLOW, DRKYEL, BLACK, BLACK,  wFont_Retro8x16, wFont_Arial_round_16x24, {StringType, "X"}, Paysage, true, true, true }, // X
  
  { 3, wRECT, true, 20,80,51,51, GREEN, DRKGRN, BLACK, BLACK,  wFont_Retro8x16, wFont_Arial_round_16x24, {StringType, "/10"}, Paysage, false, false, true }, // Par 10
  
  { 4, wRECT, true, 95,80,130,51, GREEN, DRKGRN, BLACK, BLACK,  wFont_Arial_round_16x24, wFont_Arial_round_16x24, {IntType, 256}, Paysage, true, false, true }, // Z value
  { 5, wRECT, true, 232,80,75,51, YELLOW, DRKYEL, BLACK, BLACK,  wFont_Retro8x16, wFont_Arial_round_16x24, {StringType, "Z"}, Paysage, false, false, true }, // Z
  
  { 6, wRECT, false, 95,140,130,51, WHITE, WHITE, BLACK, BLACK,  wFont_Arial_round_16x24, wFont_Arial_round_16x24, {IntType,1500}, Paysage, false, true, true }, // RPM broche
  { 7, wRECT, false, 232,140,75,51, BLACK, BLACK, WHITE, WHITE,  wFont_Arial_round_16x24, wFont_Arial_round_16x24, {StringType,"VM"}, Paysage, false, false, true }, // VM

  { 8, wRECT, false, 20,210,70,51, BLACK, BLACK, WHITE, WHITE,  wFont_Retro8x16, wFont_Retro8x16, {StringType,"Mm/mn|Manuel"}, Paysage, false, true, true }, // Manuel variable
  { 9, wRECT, true, 95,210,130,51, GREEN, DRKGRN, BLUE, BLUE,  wFont_Arial_round_16x24, wFont_Arial_round_16x24, {IntType,0}, Paysage, true, false, true }, // Vitesse Manuelle
  {10, wRECT, true, 232,210,75,51, YELLOW, DRKYEL, BLACK, BLACK,  wFont_Retro8x16, wFont_Arial_round_16x24, {StringType, "MAN"}, Paysage, false, false, true }, // MAN 
  
  {11, wRECT, false, 20,270,70,51, BLACK, BLACK, WHITE, WHITE,  wFont_Retro8x16, wFont_Retro8x16, {StringType,"Mm/mn|Avance"}, Paysage, false, false, true }, // Mm/mn/Avance
  {12, wRECT, true, 95,270,130,51, GREEN, DRKGRN, BLACK, BLACK,  wFont_Arial_round_16x24, wFont_Arial_round_16x24, {IntType, 200}, Paysage, false, false, true }, // Avance value
  {13, wRECT, true, 232,270,75,51, YELLOW, DRKYEL, BLACK, BLACK,  wFont_Retro8x16, wFont_Arial_round_16x24, {StringType,"AUTO"}, Paysage, true, false, true }, // AV
  
  {14, wRECT, false, 95,330,60,30, BLACK, BLACK, WHITE, WHITE,  wFont_Retro8x16, wFont_Retro8x16, {StringType,"Outil"}, Paysage, false, true, true }, // Outil
  {15, wCRCL, true, 155,330,30,30, GREEN, DRKGRN, BLACK, BLACK,  wFont_Arial_round_16x24, wFont_Retro8x16, {IntType,1}, Paysage, false, true, true }, // 1
  {16, wCRCL, true, 195,330,30,30, GREEN, DRKGRN, BLACK, BLACK,  wFont_Arial_round_16x24, wFont_Retro8x16, {IntType,2}, Paysage, false, false, true }, // 2
  {17, wCRCL, true, 235,330,30,30, GREEN, DRKGRN, BLACK, BLACK,  wFont_Arial_round_16x24, wFont_Retro8x16, {IntType,3}, Paysage, false, false, true }, // 3
  {18, wCRCL, true, 275,330,30,30, GREEN, DRKGRN, BLACK, BLACK,  wFont_Arial_round_16x24, wFont_Retro8x16, {IntType,4}, Paysage, false, false, true }, // 4

  {19, wRECT, true, 20,360,75,111, BLUE, DULBLU, WHITE, WHITE,  wFont_Arial_round_16x24, wFont_Arial_round_16x24, {StringType,"HOME"}, Portrait, false, false, true }, // HOME
  {20, wRECT, true, 140,390,130,40, GREEN, DRKGRN, WHITE, WHITE,  wFont_Arial_round_16x24, wFont_Arial_round_16x24, {StringType,"START"}, Paysage, false, false, true }, // START
};

#define PAR_1   0
#define X       1
#define X_LBL   2
#define PAR_10  3
#define Z       4
#define Z_LBL   5

#define MAN     9
#define MAN_LBL 10

#define AV      12
#define AV_LBL  13

#define TOOL1   15
#define TOOL2   16
#define TOOL3   17
#define TOOL4   18
#define HOME    19
#define START   20

/*
struct EtatCNC
{
  RGB565 fond;
  WNdxgetNdx wWNdxgetActifNdx;
  int valeurLue;
  int nbClicks;
  int nbWidgets;
  Widget *ecran;
}; */
EtatCNC etat = { BLACK, 1, 0, 0, sizeof(ecran)/sizeof(Widget), ecran };
