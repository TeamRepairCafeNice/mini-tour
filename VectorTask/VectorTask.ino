/**
 * Prototype de banc de test du tour Jeulin 700 CE
 * Jean-Pierre de SOZA - 09/2020
 * V2.0.1 - Septembre 2020 - Adoption des conventions CNC tour avec seulement X et Z
 */
// Constantes pour désigner les axes
#define NOP (0)
#define X   (1)
#define Y   (2)
#define Z   (3)
// Même structure pour points et vecteurs
typedef struct {
  int x;
  int y;
  int z;
} Point3D ;

// Synonyme
#define VECTOR3D Point3D
VECTOR3D deltaC1[] = {
  {0, 0, 1}, 
  {1, 0, 1},
  {1, 0, 0}, 
  {1, 0, -1}, 
  {0, 0, -1}, 
  {-1, 0, -1},
  {-1, 0, 0},
  {-1, 0, 1}
};

Point3D p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11;

// Copie de point ou de vecteur
void copie(Point3D orig, Point3D* duplicate) {
  duplicate->x = orig.x;
  duplicate->y = orig.y;
  duplicate->z = orig.z;
}
// Translation de point par vecteur
void translate(Point3D u, VECTOR3D v, Point3D* sum) {
  sum->x = u.x + v.x;
  sum->y = u.y + v.y;
  sum->z = u.z + v.z;
}
// Produit vectoriel
void prod(VECTOR3D u, VECTOR3D v, VECTOR3D* prod) {
  prod->x = u.y*v.z - u.z*v.y;
  prod->y = u.z*v.x - u.x*v.z;
  prod->z = u.x*v.y - u.y*v.x;
}
// Vecteur à partir d'un bi-point
void makeVector(Point3D p1, Point3D p2, VECTOR3D *vector) {
  vector->x = p2.x - p1.x;
  vector->y = p2.y - p1.y; 
  vector->z = p2.z - p1.z;
}
// Commande du moteur d'axe
void doStep(int axis) {
  // Serial.print("doStep("); Serial.print(axis);Serial.println(")");
  switch (axis) {
    case -Z: Serial.print("-Z"); break;
    case -Y: Serial.print("-Y"); break;
    case -X: Serial.print("-X"); break;
    case NOP: break;
    case  X: Serial.print("+X"); break;
    case  Y: Serial.print("+Y"); break;
    case  Z: Serial.print("+Z"); break;
    default: Serial.print("Erreur");
  }
  Serial.flush();
}

void logPos(char *prompt, Point3D pos) {
  Serial.print(prompt); Serial.print("\t| x = "); Serial.print(pos.x);Serial.print("\t| y = "); Serial.print(pos.y);Serial.print("\t| z = "); Serial.print(pos.z); Serial.println("\t|") ;
  Serial.flush();
}

void analysePente(Point3D delta, int *octant, float *pente) {
  
  // Cas général angle quelconque  
  // Orientation X pointe vers le haut
  //             Z pointe vers la droite
  
  *pente = abs(1.0 * delta.x / delta.z);

  // Détermination de l'octant de direction du vecteur
  if (delta.z > 0) { // demi-plan droit
    if (delta.x > 0) { //quadrant du haut-droit
      if (*pente < 1.0) { // octant du haut-droit-bas
        *octant = 0;
      } else {
        *octant = 1;
      }
    } else { // quadrant du bas-gauche
      if (*pente < 1.0) { // octant du bas-droit-haut
        *octant = 7;
      } else {
        *octant = 6;
      }
    }
  } else { // demi-plan gauche
    if (delta.x > 0) { //quadrant du haut-gauche
      if (*pente < 1.0) { // octant du haut-gauche-bas
        *octant = 3;
      } else {
        *octant = 2;
      }
    } else { // quadrant du bas-gauche
      if (*pente < 1.0) { // octant du bas-droit-haut
        *octant = 4;
      } else {
        *octant = 5;
      }
    }
  }
  Serial.print("Pente = "); Serial.println(*pente);
  Serial.print("Octant = ");Serial.println(*octant);
}
// Interpolation des pas pour un chemin droit d'outil
void vectorTask(Point3D start, Point3D finish ) {
  Point3D actuel;
  VECTOR3D delta;
  makeVector(start, finish, &delta);

  Serial.println("################################################");
  logPos("Start", start);
  logPos("Finish", finish);
  logPos("Delta", delta);

  copie(start, &actuel);
  // Cas particuliers
  // Parallèle à Z
  if (delta.x == 0) {
    for(int i = 0; i < abs(delta.z); i++) {
      logPos("", actuel);
      doStep(delta.z > 0 ? Z : -Z);
      translate(actuel, {0, 0, delta.z > 0 ? 1 : -1}, &actuel);
    }
    logPos("", actuel);
    Serial.println();
    return;
  } 
  // Parallèle à X
  if (delta.z == 0) {
    for(int i = 0; i < abs(delta.x); i++) {
      logPos("", actuel);
      doStep(delta.x > 0 ? X : -X); 
      translate(actuel, { delta.x > 0 ? 1 : -1, 0, 0 }, &actuel);
    }
    logPos("", actuel);
    Serial.println();
    return;
  } 
  // Pente == 1.0
  if (abs(delta.x) == abs(delta.z)) {
    for (int i = 0; i < abs(delta.x); i++) {
      logPos("", actuel);
      doStep(delta.x > 0 ? X : -X); 
      translate(actuel, { delta.x > 0 ? 1 : -1, 0, 0 }, &actuel);
      logPos("", actuel);
      doStep(delta.z > 0 ? Z : -Z); 
      translate(actuel, { 0, 0, delta.z > 0 ? 1 : -1 }, &actuel);
    }
    logPos("", actuel);
    Serial.println();
    return;
  } 

  // Cas général, angle quelconque
  float pente;
  int octant;
  analysePente(delta, &octant, &pente);
  int octant_suivant = (8 == octant + 1) ? 0 : octant + 1;
  Serial.print("Octant++ = ");Serial.println(octant_suivant);
  
  Point3D c1, c2;
  VECTOR3D p0c1, p0c2, p0p1, prod1, prod2;

  makeVector(start, finish, &p0p1);

  for (int i = 0; i < max(abs(delta.x), abs(delta.z)); i++) {
    logPos("", actuel);
    // Calcul des candidats
    translate(actuel, deltaC1[octant], &c1);
    translate(actuel, deltaC1[octant_suivant], &c2);
    // Calcul des vecteurs et de leur produit
    makeVector(start, c1, &p0c1);
    makeVector(start, c2, &p0c2);
    prod(p0p1, p0c1, &prod1);
    prod(p0p1, p0c2, &prod2);
    // Détermination du meilleur candidat, 
    // Pour le rapport entre produit vectoriel et 
    // aire du parallélogramme formé par les deux vecteurs
    // https://www.geogebra.org/m/CMyDAgrV
    // Test simplifié cas où prod.x == 0.0 et prod.z == 0.0;
    if (abs(prod1.y) < abs(prod2.y)) { // C1
      if (pente < 1.0) {
        if (deltaC1[octant].x != 0) doStep(deltaC1[octant].x * X);        
        if (deltaC1[octant].z != 0) doStep(deltaC1[octant].z * Z);  
      } else {
        if (deltaC1[octant].z != 0) doStep(deltaC1[octant].z * Z);  
        if (deltaC1[octant].x != 0) doStep(deltaC1[octant].x * X);        
      }
      // on continue avec C1    
      copie(c1, &actuel);    
    } else { // C2   
      if (pente < 1.0) {
        if (deltaC1[octant_suivant].x != 0) doStep(deltaC1[octant_suivant].x * X);
        if (deltaC1[octant_suivant].z != 0) doStep(deltaC1[octant_suivant].z * Z);      
      } else {
        if (deltaC1[octant_suivant].z != 0) doStep(deltaC1[octant_suivant].z * Z);      
        if (deltaC1[octant_suivant].x != 0) doStep(deltaC1[octant_suivant].x * X);
      }
      // On continue avec C2
      copie(c2, &actuel);
    }
  }
  // Dernier print en sortie de boucle
  logPos("", actuel);
} 
void setup() {
  Serial.begin(57600);
  p0 = { -1, 0,  3 };
  p1 = {  2, 0, -6 };
  // Permutation X et Y = symétrie selon la diagonale
  p2 = {  3, 0, -1 };
  p3 = { -6, 0,  2 };
  // Parallèle à Z (delta.x === 0)
  p4 = {5, 0,  4};
  p5 = {5, 0, -6};
  // Diagonale ascendante
  p6 = {-5, 0, -5};
  p7 = { 5, 0,  5};
  // Parallèle à X (delta.z == 0)
  p8 = {-8, 0, -3};
  p9 = { 7, 0, -3};
  // Diagonale descendante
  p10 = {-12, 0,  12};
  p11 = { 12, 0, -12};
  Serial.println("Démo de génération de vecteurs par interpolation.");
  run_tests();
}

void run_tests() {
  Serial.print("Aller ");
  vectorTask(p0, p1);
  Serial.print("Retour ");
  vectorTask(p1, p0);
  Serial.print("Symétrie aller");
  vectorTask(p2, p3);
  Serial.print("Symétrie retour ");
  vectorTask(p3, p2);
  Serial.print("Parallèle à axe Z aller ");
  vectorTask(p4, p5);
  Serial.print("Parallèle à axe Z retour ");
  vectorTask(p5, p4);
  Serial.print("Diagonale ascendante aller ");
  vectorTask(p6, p7);
  Serial.print("Diagonale ascendante retour ");
  vectorTask(p7, p6);
  Serial.print("Parallèle à axe X aller ");
  vectorTask(p8, p9);
  Serial.print("Parallèle à axe X retour ");
  vectorTask(p9, p8);
  Serial.print("Diagonale descendante aller ");
  vectorTask(p10, p11);
  Serial.print("Diagonale descendante retour ");
  vectorTask(p11, p10);
}

void loop() {
  delay(65535);
} 
