#ifndef __fondam_h_
#define __fondam_h_

#include <stdio.h>
#include <string.h>
#include <math.h>
#include "limace.h"
#include "salade.h"
#include "erreurs.h"

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
extern Matrix Fondamentale(Matrix Matpg, Matrix Matpd);


#endif /* !__fondam_h_ */
 
