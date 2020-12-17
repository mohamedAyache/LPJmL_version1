/***************************************************************************/
/**                                                                       **/
/**              f  r  e  a  d  l  i  t  t  e  r  .  c                    **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     Function reads litter data from file                              **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: $Date:: 2014-06-23 08:39:51 +0200 (lun., 23 juin 2#$ **/
/**     By         : $Author:: bloh                            $          **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"

Bool freadlitter(FILE *file, /* File pointer to binary file */
                 Litter *litter, /* Litter pool to be read */
                 const Pftpar pftpar[], /* PFT parameter array */
                 int ntotpft, /* total number of PFTs */
                 Bool swap /* Byte order has to be changed (TRUE/FALSE) */
              )            /* returns TRUE on error */
{
    Byte b;
    int i;
    freadreal(litter->avg_fbd,NFUELCLASS+1,swap,file);
    if(fread(&b,sizeof(b),1,file)!=1)
        return TRUE;
    litter->n=b;
    if(litter->n)
    {
        litter->ag=newvec(Litteritem,litter->n);
        litter->bg=newvec(Real,litter->n);
        if(litter->ag==NULL || litter->bg==NULL)
            return TRUE;
        for(i=0;i<litter->n;i++)
        {
            if(fread(&b,sizeof(b),1,file)!=1)
            {
                free(litter->ag);
                free(litter->bg);
                litter->n=0;
                litter->ag=NULL;
                litter->bg=NULL;
                return TRUE;
            }
            litter->ag[i].pft=pftpar+b;
            if(b>=ntotpft)
            {
                fprintf(stderr,"ERROR195: Invalid value %d for PFT index litter.\n",(int)b);
                free(litter->ag);
                free(litter->bg);
                litter->n=0;
                litter->ag=NULL;
                litter->bg=NULL;
                return TRUE;
            }
            litter->ag[i].pft=pftpar+b;
            //if(freadreal((Real *)&litter->ag[i].trait,sizeof(Trait)/sizeof(Real),
            //            swap,file)!=sizeof(Trait)/sizeof(Real))
            // barrier.n
            if(fread(&litter->ag[i].trait, sizeof(Trait), 1, file) != 1)
                return TRUE;
            freadreal1(&litter->bg[i],swap,file);
        }
    }
    else
    {
        litter->ag=NULL;
        litter->bg=NULL;
    }
    return FALSE;
} /* of 'freadlitter' */
