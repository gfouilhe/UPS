#include <stdio.h>
#include "fondam.h"


/**
 * @brief Transp Transposée d'une matrice
 * @param Mat[in] matrice dont il faut calculer la transposée (n x m)
 * @return Matrice transposée (m x n)
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


/**
 * @brief Mult Multiplication de deux matrices
 * @param MA[in] matrice 1 (n x m)
 * @param MB[in] matrice 2 (m x p)
 * @return Matrice (n x p) résultat de la multiplication
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


/**
 * @brief IndMin Détermination de l'indice de la ligne du plus petit élément de
  la première colonne d'une matrice de type Double
 * @param Matv[in] matrice de type Double
 * @return indice de la ligne du plus petit élément de la première colonne de
   Mat
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


/**
 * @brief CalculA Calcul de la matrice A
 * @param Matpg[in] coordonnées des points de l'image gauche, sous la forme :
       (u1g,v1g)
       (u2g,v2g)
          ...
       (ung,vng)
 * @param Matpd[in] coordonnées des points de l'image droite, sous la forme :
       (u1d,v1d)
       (u2d,v2d)
          ...
       (und,vnd)
 * @return Matrice des coefficients A
 */
Matrix CalculA(Matrix Matpg, Matrix Matpd)
{
  int n = MatNbRow(Matpg);
  
  Matrix Coeff=MatAlloc(Double, n, 9);
  double **A = MatGetDouble(Coeff);
  double **G = MatGetDouble(Matpg);
  double **D = MatGetDouble(Matpd);
  for (int i = 0; i<n;i++) {
    A[i][0] = G[i][0]*D[i][0];
    A[i][1] = G[i][1]*D[i][0];
    A[i][2] = D[i][0];
    A[i][3] = G[i][0]*D[i][1];
    A[i][4] = G[i][1]*D[i][1];
    A[i][5] = D[i][1];
    A[i][6] = G[i][0];
    A[i][7] = G[i][1];
    A[i][8] = 1;
  }
  return Coeff;
}



/**
 * @brief CalculF Estimation de la matrice fondamentale
 * @param Prod[in] transpose(A)*A
 * @return Matrice fondamentale
 */
Matrix CalculF(Matrix Prod)
{
  Matrix MatF=MatAlloc(Double, 3, 3);
  double **F = MatGetDouble(MatF);

  // Calcul des valeurs/vecteurs propres
  Matrix Valpropres;
  Matrix Vecpropres;
  SymEig(Prod, &Valpropres, &Vecpropres);
  double **VecP = MatGetDouble(Vecpropres);

  //Calcul de la plus petite valeur propre et de son vecteur propre associé
  int ind = IndMin(Valpropres);
  Matrix VP = MatAlloc(Double,9,1);
  double **V = MatGetDouble(VP);
  for (int i=0;i<9;i++)
  {
    V[i][0] = VecP[i][ind];
  }

  for (int i=0;i<9;i++)
  {
    F[i / 3][i % 3] = V[i][0];
  }

  return MatF;
}


/**
 * @brief Fondamentale Estimation au sens des moindres carrés totaux de la
 * matrice fondamentale à partir de correspondances 2D <--> 2D
 * @param Matpg[in] coordonnées des points de l'image gauche, sous la forme :
       (u1g,v1g)
       (u2g,v2g)
          ...
       (ung,vng)
 * @param Matpd[in] coordonnées des points de l'image droite, sous la forme :
       (u1d,v1d)
       (u2d,v2d)
          ...
       (und,vnd)
 * @return Matrice fondamentale
 */
Matrix Fondamentale(Matrix Matpg, Matrix Matpd)
{
  Matrix MatF=NULL;
  
  // EXERCICE 1 : ENLEVEZ LES COMMENTAIRES CI-DESSOUS POUR TESTER la fonction CalculA
  printf("====================================\nExercice 1 :\n");
  printf("====================================\n");
  Matrix Coeff=CalculA(Matpg,Matpd);
  // MatWriteAsc(Coeff,"");

  printf("====================================\nTransposee(A) :\n");
  printf("====================================\n");
  Matrix Transposee=Transp(Coeff);
  // MatWriteAsc(Transposee,"");

  printf("====================================\nTransposee(A) x A :\n");
  printf("====================================\n");
  Matrix Prod=Mult(Transposee,Coeff);
  // MatWriteAsc(Prod,"");
  MatFree(&Transposee);
  MatFree(&Coeff);

  // EXERCICE 2 : ENLEVEZ LES COMMENTAIRES CI-DESSOUS POUR TESTER la fonction CalculF
  printf("====================================\nExercice 2 :\n");
  printf("====================================\n");
  MatF=CalculF(Prod);
  MatWriteAsc(MatF,"");
  MatFree(&Prod);

  return MatF;
}
