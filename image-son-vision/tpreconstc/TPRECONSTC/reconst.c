#include <stdio.h>
#include <string.h>
#include <math.h>
#include "limace.h"
#include "salade.h"
#include "erreurs.h"

/* Codes de retour */
#define OK                 0
#define HELP               1
#define ERR_NB_PARAM       2
#define ERR_MAT_P_G        3
#define ERR_MAT_P_G_TYPE   4
#define ERR_MAT_P_G_SIZE   5
#define ERR_MAT_P_D        6
#define ERR_MAT_P_D_TYPE   7
#define ERR_MAT_P_D_SIZE   8
#define ERR_MAT_2D_G       9
#define ERR_MAT_2D_G_TYPE 10
#define ERR_MAT_2D_G_SIZE 11
#define ERR_MAT_2D_D      12
#define ERR_MAT_2D_D_TYPE 13
#define ERR_MAT_2D_D_SIZE 14
#define ERR_MAT_2D_SIZE   15
#define ERR_MAT_OUT       16

/* Affichage de la syntaxe d'appel du programme */
void Syntaxe(void)
{
  Usage("Pg.mx Pd.mx p2g.mx p2d.mx p3.mx\n"
        "-h\n");
}

/* Affichage de la description du programme */
void Description(void)
{
  Mesg("ROLE\n");
  Mesg("\tReconstruction 3D par triangulation\n");
  Mesg("ARGUMENTS\n");
  Mesg("\tPg.mx : Matrice 3 x 4 (format Matrix) de projection perspective gauche\n");
  Mesg("\tPd.mx : Matrice 3 x 4 (format Matrix) de projection perspective droite\n");
  Mesg("\tp2g.mx : Matrice n x 2 (format Matrix) des coordonnées des points à gauche\n");
  Mesg("\tp2d.mx : Matrice n x 2 (format Matrix) des coordonnées des correspondants à droite\n");
  Mesg("\tp3.mx : nom du fichier destination qui va contenir la matrice n x 3 (format Matrix) des coordonnées des points reconstruits\n");
  Mesg("OPTION\n");
  Mesg("\t-h : affichage de l'aide\n");
  Mesg("DIAGNOSTIC (codes de retour)\n");
  Mesg("\t 0 : opération réalisée sans problème\n");
  Mesg("\t 1 : aide demandée\n");
  Mesg("\t 2 : mauvais nombre de paramètres\n");
  Mesg("\t 3 : problème d'ouverture du fichier de la matrice de projection perspective gauche\n");
  Mesg("\t 4 : type de la matrice de projection perspective gauche incorrect (Double attendu)\n");
  Mesg("\t 5 : taille de la matrice de projection perspective gauche incorrecte (3 x 4 attendue)\n");
  Mesg("\t 6 : problème d'ouverture du fichier de la matrice de projection perspective droite\n");
  Mesg("\t 7 : type de la matrice de projection perspective droite incorrect (Double attendu)\n");
  Mesg("\t 8 : taille de la matrice de projection perspective droite incorrecte (3 x 4 attendue)\n");
  Mesg("\t 9 : problème d'ouverture du fichier des coordonnées 2D gauche \n");
  Mesg("\t10 : type de la matrice 2D gauche incorrect (Double attendu)\n");
  Mesg("\t11 : taille de la matrice 2D gauche incorrecte (2 colonnes attendues)\n");
  Mesg("\t12 : problème d'ouverture du fichier des coordonnées 2D droite \n");
  Mesg("\t13 : type de matrice 2D droite incorrect (Double attendu)\n");
  Mesg("\t14 : taille de la matrice 2D droite incorrecte (2 colonnes attendues)\n");
  Mesg("\t15 : les deux matrices 2D n'ont pas le même nombre de lignes\n");
  Mesg("\t16 : problème lors du calcul de la matrice des coordonnées des points 3D reconstruits\n");
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

/* Stéréovision binoculaire : reconstruction 3D par triangulation : 
 * estimation au sens des moindres carrés totaux des coordonnées 3D des 
 * points reconstruits à partir de correspondances 2D <--> 2D et des matrices 
 * de projection perspective des deux caméras
 * Entrées :
 *    - MatPg : matrice de projection perspective de la caméra gauche 
 *              de type Double de taille 3 x 4 
 *    - MatPd : matrice de projection perspective de la caméra droite 
 *              de type Double de taille 3 x 4 
 *    - Matp2g : matrice de type Double de taille n x 2 des coordonnées des
 *               projections sur l'image gauche
 *    - Matp2d : matrice de type Double de taille n x 2 des coordonnées des
 *               projections correspondantes sur l'image droite
 * Valeur de retour : matrice de type Double de taille n x 3 des coordonnées 
 *                    3D des points reconstruits
 */
Matrix Reconstruction(Matrix MatPg, Matrix MatPd, Matrix Matp2g, Matrix Matp2d)
{
  double **pg = MatGetDouble(MatPg);
  double **pd = MatGetDouble(MatPd);
  double **p2g = MatGetDouble(Matp2g);
  double **p2d = MatGetDouble(Matp2d);

  int n = MatNbRow(Matp2g);

  for (int i=0;i<n;i++)
  {
    Matrix MatA = MatAlloc(Double,4,3);
    double **A = MatGetDouble(MatA); 

    A[0][0] = pg[0][0] - p2g[i][0] * pg[2][0];
    A[0][1] = pg[0][1] - p2g[i][0] * pg[2][1];
    A[0][2] = pg[0][2] - p2g[i][0] * pg[2][2];

    A[1][0] = pg[1][0] - p2g[i][1] * pg[2][0];
    A[1][1] = pg[1][1] - p2g[i][1] * pg[2][1];
    A[1][2] = pg[1][2] - p2g[i][1] * pg[2][2];

    A[2][0] = pd[0][0] - p2d[i][0] * pd[2][0];
    A[2][1] = pd[0][1] - p2d[i][0] * pd[2][1];
    A[2][2] = pd[0][2] - p2d[i][0] * pd[2][2];

    A[3][0] = pd[1][0] - p2d[i][1] * pd[2][0];
    A[3][1] = pd[1][0] - p2d[i][1] * pd[2][1];
    A[3][2] = pg[1][0] - p2d[i][1] * pd[2][2];

    MatWriteAsc(MatA,"");

  }
  
 
  
  // À FAIRE
  printf("---> Reconstruction à écrire.\n"); return NULL; // LIGNE À SUPPRIMER

}


/* Fonction principale */
int main(int argc, char *argv[])
{
  // Initialisation du mécanisme d'affichage des messages
  InitMesg(argv);
  // Vérification du nombre de paramètres
  if (argc!=2 && argc!=6)
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
  // Lecture des matrices
  Matrix MatPg=MatReadAsc(argv[1]);
  if (MatPg==NULL)
  {
    Erreur("Problème lors de la lecture de la matrice de projection perspective gauche");
	return ERR_MAT_P_G;
  }
  if (MatType(MatPg)!=Double)
  {
    Erreur("La matrice de projection perspective gauche doit être Double");
	MatFree(&MatPg);
	return ERR_MAT_P_G_TYPE;
  }
  if (MatNbRow(MatPg)!=3 || MatNbCol(MatPg)!=4)
  {
    Erreur("La matrice de projection perspective gauche doit être de taille 3 x 4");
	MatFree(&MatPg);
	return ERR_MAT_P_G_SIZE;
  }
  Matrix MatPd=MatReadAsc(argv[2]);
  if (MatPd==NULL)
  {
    Erreur("Problème lors de la lecture de la matrice de projection perspective droite");
	MatFree(&MatPg);
	return ERR_MAT_P_D;
  }
  if (MatType(MatPd)!=Double)
  {
    Erreur("La matrice de projection perspective droite doit être Double");
	MatFree(&MatPg);
	MatFree(&MatPd);
	return ERR_MAT_P_D_TYPE;
  }
  if (MatNbRow(MatPd)!=3 || MatNbCol(MatPd)!=4)
  {
    Erreur("La matrice de projection perspective droite doit être de taille 3 x 4");
	MatFree(&MatPg);
	MatFree(&MatPd);
	return ERR_MAT_P_D_SIZE;
  }
  Matrix Matp2g=MatReadAsc(argv[3]);
  if (Matp2g==NULL)
  {
    Erreur("Problème lors de la lecture de la matrice des coordonnées 2D gauche");
	MatFree(&MatPg);
	MatFree(&MatPd);
	return ERR_MAT_2D_G;
  }
  if (MatType(Matp2g)!=Double)
  {
    Erreur("La matrice des coordonnées 2D gauche doit être Double");
	MatFree(&MatPg);
	MatFree(&MatPd);
	MatFree(&Matp2g);
	return ERR_MAT_2D_G_TYPE;
  }
  if (MatNbCol(Matp2g)!=2)
  {
    Erreur("La matrice des coordonnées 2D gauche doit avoir 2 colonnes");
	MatFree(&MatPg);
	MatFree(&MatPd);
	MatFree(&Matp2g);
	return ERR_MAT_2D_G_SIZE;
  }
  Matrix Matp2d=MatReadAsc(argv[4]);
  if (Matp2d==NULL)
  {
    Erreur("Problème lors de la lecture de la matrice des coordonnées 2D droite");
	MatFree(&MatPg);
	MatFree(&MatPd);
	MatFree(&Matp2g);
	return ERR_MAT_2D_D;
  }
  if (MatType(Matp2d)!=Double)
  {
    Erreur("La matrice des coordonnées 2D droite doit être Double");
	MatFree(&MatPg);
	MatFree(&MatPd);
	MatFree(&Matp2g);
	MatFree(&Matp2d);
	return ERR_MAT_2D_D_TYPE;
  }
  if (MatNbCol(Matp2d)!=2)
  {
    Erreur("La matrice des coordonnées 2D droite doit avoir 2 colonnes");
	MatFree(&MatPg);
	MatFree(&MatPd);
	MatFree(&Matp2g);
	MatFree(&Matp2d);
	return ERR_MAT_2D_D_SIZE;
  }
  if (MatNbRow(Matp2g)!=MatNbRow(Matp2d))
  {
    Erreur("Les deux matrices de coordonnées 2D doivent avoir le même nombre de lignes");
	MatFree(&MatPg);
	MatFree(&MatPd);
	MatFree(&Matp2g);
	MatFree(&Matp2d);
	return ERR_MAT_2D_SIZE;
  }
  // Reconstruction 3D par triangulation
  Matrix Matp3d=Reconstruction(MatPg,MatPd,Matp2g,Matp2d);
  MatFree(&MatPg);
  MatFree(&MatPd);
  MatFree(&Matp2g);
  MatFree(&Matp2d);
  if (Matp3d==NULL)
  {
    Erreur("Problème lors de la triangulation des points");
	return ERR_MAT_OUT;
  }
  // Écriture de la matrice résultat
  MatWriteAsc(Matp3d,argv[5]);
  // Libération de la matrice résultat 
  MatFree(&Matp3d);

  return OK;
}
