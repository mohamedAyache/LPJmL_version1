/***************************************************************************/
/**                                                                       **/
/**               f  p  r  i  n  t  _  c  r  o  p  .  c                   **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran version         **/
/**                                                                       **/
/**     Function writes crop-specific data in ASCII format to file        **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: 02.03.2009                                           **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"
#include "crop.h"

void fprint_crop(FILE *file,const Pft *pft)
{
    const Pftcrop *crop;
    crop=pft->data;
    fprintf(file,"Wtyp:\t\t%s crop\n",(crop->wtype) ? "Winter" : "Summer");
    fprintf(file,"Growing days:\t%d\n",crop->growingdays);
    fprintf(file,"Pvd:\t\t%g\n",crop->pvd);
    fprintf(file,"Phu:\t\t%g\n",crop->phu);
    fprintf(file,"Basetemp:\t%g\n",crop->basetemp);
    fprintf(file,"Senescence:\t%d\n",crop->senescence);
    fprintf(file,"Senescence0:\t%d\n",crop->senescence0);
    fprintf(file,"Husum:\t\t%g\n",crop->husum);
    fprintf(file,"Vdsum:\t\t%g\n",crop->vdsum);
    fprintf(file,"Fphu:\t\t%g\n",crop->fphu);
    fprintf(file,"Ind:\t\t");
    fprintcropphys2(file,crop->ind,pft->nind);
    fprintf(file,"\nLAI:\t\t%g\n",crop->lai);
    fprintf(file,"LAI000:\t\t%g\n",crop->lai000);
    fprintf(file,"LAImax_adjusted:\t%g\n",crop->laimax_adjusted);
    fprintf(file,"Demandsum:\t%g\n",crop->demandsum);
    fprintf(file,"Supplysum:\t%g\n",crop->supplysum);
} /* of 'fprint_crop' */
