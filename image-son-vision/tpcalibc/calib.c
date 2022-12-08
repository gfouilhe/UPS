#include <stdio.h>
#include <string.h>
#include <math.h>
#include "limace.h"
#include "salade.h"
#include "erreurs.h"

/* Codes de retour */
#define OK              0
#define HELP            1
#define ERR_NB_PARAM    2
#define ERR_MAT_3D      3
#define ERR_MAT_3D_TYPE 4
#define ERR_MAT_3D_SIZE 5
#define ERR_MAT_2D      6
#define ERR_MAT_2D_TYPE 7
#define ERR_MAT_2D_SIZE 8
#define ERR_MAT_SIZE    9
#define ERR_MAT_OUT    10

/* Affichage de la syntaxe d'appel du programme */
void Syntaxe(void)
{
  Usage("p3d.mx p2d.mx mpp.mx\n"
        "-h\n");
}

/* Affichage de la description du programme */
void Description(void)
{
  Mesg("ROLE\n");
  Mesg("\tCalibrage 3D d'une caméra\n");
  Mesg("ARGUMENTS\n");
  Mesg("\tp3d.mx : Matrice n x 3 (format Matrix) des coordonnées des points de la mire\n");
  Mesg("\tp2d.mx : Matrice n x 2 (format Matrix) des coordonnées des projections dans l'image des points de la mire\n");
  Mesg("\tmpp.mx : nom du fichier destination qui va contenir la matrice 3 x 4 (format Matrix) de projection perspective\n");
  Mesg("OPTION\n");
  Mesg("\t-h : affichage de l'aide\n");
  Mesg("DIAGNOSTIC (codes de retour)\n");
  Mesg("\t 0 : opération réalisée sans problème\n");
  Mesg("\t 1 : aide demandée\n");
  Mesg("\t 2 : mauvais nombre de paramètres\n");
  Mesg("\t 3 : problème d'ouverture du fichier des coordonnées 3D\n");
  Mesg("\t 4 : type de matrice 3D incorrect (Double attendu)\n");
  Mesg("\t 5 : taille de la matrice 3D incorrecte (3 colonnes attendues)\n");
  Mesg("\t 6 : problème d'ouverture du fichier des coordonnées 2D\n");
  Mesg("\t 7 : type de matrice 2D incorrect (Double attendu)\n");
  Mesg("\t 8 : taille de la matrice 2D incorrecte (2 colonnes attendues)\n");
  Mesg("\t 9 : les deux matrices doivent avoir le même nombre de lignes\n");
  Mesg("\t10 : problème lors du calcul de la matrice de projection perspective\n");
}

/*
 * Produit de deux matrices de type Double
 * Entrées :
 *    - MA : première matrice
 *    - MB : seconde matrice
 * Valeur de retour : nouvelle matrice résultat du produit de MA par MB 
 *                    ou NULL en cas de problème
 */
Matrix Mult(Matrix MA, Matrix MB)
{
  int la=MatNbRow(MA);
  int ca=MatNbCol(MA);
  int lb=MatNbRow(MB);
  int cb=MatNbCol(MB);
  if (ca!=lb)
  {
    Erreur("Mult : tailles des matrices incompatibles");
    return NULL;
  }
  Matrix MC=MatAlloc(Double,la,cb);
  if (MC==NULL)
  {
    Erreur("Mult : problème d'allocation mémoire");
    return NULL;
  }
  double **A=MatGetDouble(MA);
  double **B=MatGetDouble(MB);
  double **C=MatGetDouble(MC);

  for (int i=0;i<la;i++)
    for (int j=0;j<cb;j++)
    {
      C[i][j]=0.0;
      for (int k=0;k<ca;k++)
        C[i][j]+=A[i][k]*B[k][j];
    }

  return MC;
}

/* Transposition d'une matrice de type Double
 * Entrée :
 *    - Mat : matrice de type Double à transposer
 * Valeur de retour : nouvelle matrice transposée de Mat ou NULL en cas
 *                    de problème
 */
Matrix Transp(Matrix Mat)
{
  int l=MatNbRow(Mat);
  int c=MatNbCol(Mat);
  Matrix Res=MatAlloc(Double,c,l);
  if (Res==NULL)
  {
    Erreur("Transp : problème d'allocation memoire");
    return NULL;
  }
  double **M=MatGetDouble(Mat);
  double **R=MatGetDouble(Res);
  for (int i=0;i<c;i++)
    for (int j=0;j<l;j++)
      R[i][j]=M[j][i];
  return Res;
}

/* Indice de la ligne du plus petit élément de la première colonne d'une matrice
 * de type Double
 * Entrée :
 *    - Mat : matrice de type Double
 * Valeur de retour : indice de la ligne du plus petit élément de la première
 *                    colonne de Mat
 */
int IndMin(Matrix Mat)
{
  int n=MatNbRow(Mat);
  double **M=MatGetDouble(Mat);
  int IndM=0;
  double Min=M[0][0];
  for (int i=0; i<n; i++)
    if (M[i][0]<Min)
    {
      Min=M[i][0];
      IndM=i;
    }
  return IndM;
}

/* Calibrage 3D : estimation au sens des moindres carrés totaux de la matrice
 * de projection perspective à partir de correspondances 3D <--> 2D
 * Entrées :
 *    - MatP3 : matrice de type Double de taille n x 3 des coordonnées des 
 *              n points de référence de la mire
 *    - Matp2 : matrice de type Double de taille n x 2 des coordonnées des
 *              projections sur l'image des n points de référence de la mire
 * Valeur de retour : matrice de projection perspective de la caméra de type
 *                    Double et de taille 3 x 4
 */
Matrix Calibrage(Matrix MatP3, Matrix Matp2)
{
  // Calcul de la matrice A
  Matrix A;
  int n;
  n = MatNbRow(MatP3);
  A =  MatAlloc(Double, 2 * n, 12);
  double **Aval = MatGetDouble(A);
  double **MP3 = MatGetDouble(MatP3);
  double **MP2 = MatGetDouble(Matp2);
  for (int i = 0; i <n; i++)
    {
    for (int j = 0; j<3; j++)
      {
      Aval[2*i][j] = MP3[i][j];
      Aval[2*i+1][j] = 0;
      Aval[2*i+1][j+4] = MP3[i][j];
      Aval[2*i][j+4] = 0;
      Aval[2*i][j+8] = - MP2[i][0] * MP3[i][j];
      Aval[2*i+1][j+8] = - MP2[i][1] * MP3[i][j];
      }

    Aval[2*i][3] = 1;
    Aval[2*i+1][3] = 0;
    Aval[2*i+1][7] = 1;
    Aval[2*i][7] = 0;
    Aval[2*i][11] = - MP2[i][0];
    Aval[2*i+1][11] = - MP2[i][1];
    }
  
  // Calcul de AtA

  Matrix At = MatAlloc(Double, 12, 2*n);
  At = Transp(A); 
  Matrix AtA = MatAlloc(Double, 12, 12);
  AtA = Mult(At,A);

  // MatWriteAsc(AtA,""); // uncomment to print AtA

  
  // Calcul des valeurs/vecteurs propres
  Matrix Valpropres;
  Matrix Vecpropres;
  SymEig(AtA, &Valpropres, &Vecpropres);
  double **VecP = MatGetDouble(Vecpropres);

  //Calcul de la plus petite valeur propre et de son vecteur propre associé
  int ind = IndMin(Valpropres);
  Matrix VP = MatAlloc(Double,12,1);
  double **V = MatGetDouble(VP);
  for (int i=0;i<12;i++)
  {
    V[i][0] = VecP[i][ind];
  }

  //Calcul et normalisation de P^ la matrice de projection
  Matrix P3 = MatAlloc(Double,3,1);
  double **P3val = MatGetDouble(P3);
  P3val[0][0] = V[8][0];
  P3val[1][0] = V[9][0];
  P3val[2][0] = V[10][0];

  double p34 = V[11][0];
  int sign = sgn(p34);
  double norm = 0.0;
  for (int i=0;i<3;i++)
  {
    norm = norm + (P3val[i][0]) ^ 2
  }
  norm = sqrt(norm);

  

  


  printf("---> Calibrage à écrire.\n"); return NULL; // LIGNE À SUPPRIMER
}


/* Fonction principale */
int main(int argc, char *argv[])
{
  // Initialisation du mécanisme d'affichage des messages
  InitMesg(argv);
  // Vérification du nombre de paramètres
  if (argc!=2 && argc!=4)
  {
	Syntaxe();
	return ERR_NB_PARAM;
  }
  if (argc==2)
  {
    if (!strcmp(argv[1],"-h"))
	{
	  Syntaxe();
	  Description();
	  return HELP;
	}
	else // un seul paramètre différent de -h
	{
	  Syntaxe();
	  return ERR_NB_PARAM;
	}
  }
  // Lecture de la matrice des points 3D
  Matrix MatP3=MatReadAsc(argv[1]);
  if (MatP3==NULL)
  {
    Erreur("Problème lors de la lecture de la matrice des coordonnées 3D");
	return ERR_MAT_3D;
  }
  // Vérification du type de la matrice
  if (MatType(MatP3)!=Double)
  {
    Erreur("La matrice des coordonnées 3D doit être Double");
	MatFree(&MatP3);
	return ERR_MAT_3D_TYPE;
  }
  // Vérification de la taille de la matrice
  if (MatNbCol(MatP3)!=3)
  {
    Erreur("La matrice des coordonnées 3D doit avoir 3 colonnes");
	MatFree(&MatP3);
	return ERR_MAT_3D_SIZE;
  }
  // Lecture de la matrice des points 2D
  Matrix Matp2=MatReadAsc(argv[2]);
  if (Matp2==NULL)
  {
    Erreur("Problème lors de la lecture de la matrice des coordonnées 2D");
	MatFree(&MatP3);
	return ERR_MAT_2D;
  }
  // Vérification du type de la matrice
  if (MatType(Matp2)!=Double)
  {
    Erreur("La matrice des coordonnées 2D doit être Double");
	MatFree(&MatP3);
	MatFree(&Matp2);
	return ERR_MAT_2D_TYPE;
  }
  // Vérification de la taille de la matrice
  if (MatNbCol(Matp2)!=2)
  {
    Erreur("La matrice des coordonnées 2D doit avoir 2 colonnes");
	MatFree(&MatP3);
	MatFree(&Matp2);
	return ERR_MAT_2D_SIZE;
  }
  // Vérification des nombres de points
  if (MatNbRow(MatP3)!=MatNbRow(Matp2))
  {
    Erreur("Les deux matrices doivent avoir le même nombre de lignes");
	MatFree(&MatP3);
	MatFree(&Matp2);
	return ERR_MAT_SIZE;
  }
  // Estimation de la matrice de projection perspective
  Matrix MatProjPers=Calibrage(MatP3,Matp2);
  // Libération des deux matrices de points
  MatFree(&MatP3);
  MatFree(&Matp2);
  if (MatProjPers==NULL)
  {
    Erreur("Problème lors de l'estimation de la matrice de projection perspective");
	return ERR_MAT_OUT;
  }
  // Écriture de la matrice résultat
  MatWriteAsc(MatProjPers,argv[3]);
  // Libération de la matrice résultat 
  MatFree(&MatProjPers);

  return OK;
}
