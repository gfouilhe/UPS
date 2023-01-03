#include <stdio.h>
#include <string.h>
#include <math.h>
#include "limace.h"
#include "erreurs.h"
#include "fondam.h"

/* Codes de retour */
#define OK 0
#define HELP 1
#define ERR_NB_PARAM 2
#define ERR_MAT_G 3
#define ERR_MAT_G_TYPE 4
#define ERR_MAT_D 5
#define ERR_MAT_D_TYPE 6
#define ERR_MAT_OUT 7



void Syntaxe(void)
{
    Usage("MatricePointsGauche MatricePointsDroite MatriceFondamentale\n"
          "-h\n");
}

void Description(void)
{
    Mesg("ROLE\n");
    Mesg("\tEstimation de la matrice fondamentale à partir de correspondances de points\n");
    Mesg("ARGUMENTS\n");
    Mesg("\tMatricePointsGauche : Matrice nx2 (format Matrix) des coordonnées des points de l'image gauche\n");
    Mesg("\tMatricePointsDroite : Matrice nx2 (format Matrix) des coordonnées des points de l'image droite\n");
    Mesg("\tMatriceFondamentale : nom du fichier destination qui va contenir la matrice fondamentale 3x3 (format Matrix)\n");
    Mesg("OPTION\n");
    Mesg("\t-h : affichage de l'aide\n");
    Mesg("DIAGNOSTIC (codes de retour)\n");
    Mesg("\t0 : opération réalisée sans problème\n");
    Mesg("\t1 : aide demandée\n");
    Mesg("\t2 : mauvais nombre de paramètres\n");
    Mesg("\t3 : problème d'ouverture du fichier des coordonnées dans l'image gauche\n");
    Mesg("\t4 : type de matrice des coordonnées dans l'image gauche incorrect (Double attendu)\n");
    Mesg("\t5 : problème d'ouverture du fichier des coordonnées dans l'image droite\n");
    Mesg("\t6 : type de matrice des coordonnées dans l'image droite incorrect (Double attendu)\n");
    Mesg("\t7 : problème lors du calcul de la matrice fondamentale\n");
}

int main(int argc, char *argv[])
{
    /* Déclaration des variables de la fonction main */
    Matrix Matpg=NULL,Matpd=NULL,MatFond=NULL;

    /* Initialisation du mécanisme d'affichage des messages */
    InitMesg(argv);
    /* Vérification du nombre de paramètres */
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
        else /* un seul paramètre différent de -h */
        {
            Syntaxe();
            return ERR_NB_PARAM;
        }
    }

    /* Lecture de la matrice des points de l'image gauche */
    Matpg=MatReadAsc(argv[1]);
    if (Matpg==NULL)
    {
        Erreur("Problème lors de la lecture de la matrice des coordonnées de l'image gauche");
        return ERR_MAT_G;
    }
    /* Vérification du type de la matrice */
    if (MatType(Matpg)!=Double)
    {
        Erreur("La matrice des coordonnées de l'image gauche doit être Double");
        MatFree(&Matpg);
        return ERR_MAT_G_TYPE;
    }
    /* Lecture de la matrice des points de l'image droite */
    Matpd=MatReadAsc(argv[2]);
    if (Matpd==NULL)
    {
        Erreur("Problème lors de la lecture de la matrice des coordonnées de l'image droite");
        MatFree(&Matpg);
        return ERR_MAT_D;
    }
    /* Vérification du type de la matrice */
    if (MatType(Matpd)!=Double)
    {
        Erreur("La matrice des coordonnées de l'image droite doit être Double");
        MatFree(&Matpg);
        MatFree(&Matpd);
        return ERR_MAT_D_TYPE;
    }

    /* Estimation de la matrice fondamentale */
    MatFond=Fondamentale(Matpg,Matpd);
    if (MatFond==NULL)
    {
        Erreur("Problème lors de l'estimation de la matrice fondamentale");
        MatFree(&Matpg);
        MatFree(&Matpd);
        return ERR_MAT_OUT;
    }
    /* Écriture de la matrice résultat */
    MatWriteAsc(MatFond,argv[3]);
    /* Libération des matrices */
    MatFree(&Matpg);
    MatFree(&Matpd);
    MatFree(&MatFond);

    return OK;
}
