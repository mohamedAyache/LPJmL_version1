/***************************************************************************/
/**                                                                       **/
/**                     s  t  a  n  d  l  i  s  t  .  c                   **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     Implementation of list of stands                                  **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff, Stefanie Jachner    **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: 25.06.2010                                           **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"
#include "natural.h"

int fwritestandlist(FILE *file,                /*  pointer to binary file */
        const Standlist standlist, /* stand list */
        int ntotpft                /* total number of PFTs */
        ) /* returns number of stands written */
{
    const Stand *stand;
    int s;
    fwrite(&standlist->n,sizeof(int),1,file);
    foreachstand(stand,s,standlist){
        if(fwritestand(file,stand,ntotpft))
            return s;
    }
    return s;
} /* of 'fwritestandlist' */

void fprintstandlist(FILE *file,               /* Pointer to ASCII file */
        const Standlist standlist /* Stand list */
        )
{
    const Stand *stand;
    int s;
    fprintf(file,"Number of stands:\t%d\n",standlist->n);
    foreachstand(stand,s,standlist)
    {
        fprintf(file,"Stand:\t\t%d\n",s);
        fprintstand(file,stand);
    }
} /* of 'fprintstandlist' */

Standlist freadstandlist(FILE *file,/* File pointer to binary file */
        Cell *cell, /* Cell pointer */
        const Pftpar pftpar[], /* pft parameter array */
        int ntotpft, /* total number of PFTs */
        const Soilpar *soilpar,/* soil parameter */
        const Standtype standtype[],
        int nstand, /* number of stand types */
        Bool swap /* Byte order has to be changed */
        ) /* returns allocated stand list or NULL */
{
    /* Function reads stand list from file */
    int s;
    Standlist standlist;
    /* Read number of stands */
    standlist=new(List);
    if(standlist==NULL)
        return NULL;
    if(freadint1(&standlist->n,swap,file)!=1)
    {
        free(standlist);
        return NULL;
    }
    standlist->data=newvec(void *,standlist->n);
    if(standlist->data==NULL)
    {
        free(standlist);
        return NULL;
    }
    /* Read all stand data */
    for(s=0;s<standlist->n;s++){
        if((standlist->data[s]=freadstand(file,cell,pftpar,ntotpft,soilpar,
                        standtype,nstand,swap))==NULL)
            return NULL;
    }
    return standlist;
} /* of 'freadstandlist' */

int addstand(const Standtype *type, /* stand type */
        Cell *cell             /* cell pointer */
        )           /* returns new number of stands */
{
    /* Function adds stand to list */
    Stand *stand;
    stand=new(Stand);
    check(stand);
    addlistitem(cell->standlist,stand);
    stand->cell=cell;
    newpftlist(&stand->pftlist);
    newsoil(&stand->soil);

    stand->type=type;
    /* call stand-specific allocation function */
    stand->type->newstand(stand);
    initstand(stand);
    return getlistlen(cell->standlist);
} /* of 'addstand' */

void initstand(Stand *stand /* Pointer to stand */
        )
{
    /* Function initializes stand */
    stand->fire_sum=0.0;
    stand->growing_time=stand->growing_days=stand->age=0;

    stand->lastdate_fert=-1;
    stand->lastdate_man=-1;

    int i;
    for (i=0; i<LASTLAYER;i++){
    stand->annualUptakeP[i]=0;
    stand->annualResP[i]=0;
    stand->annualUptakeN[i]=0;
    stand->annualResN[i]=0;
    }

} /* of 'initstand' */

void freestand(Stand *stand)
{
    freepftlist(&stand->pftlist); /* free list of established PFTs */
    freesoil(&stand->soil);
    /* call stand-specific free function */
    stand->type->freestand(stand);
    free(stand);
} /* of 'freestand'  */

int delstand(Standlist list, /* stand list */
        int index       /* index of stand to be deleted */
        )                /* return new number of stands */
{
    /* Function deletes stand from list */
    Stand *stand;
    stand=getstand(list,index);
    freestand(stand);
    return dellistitem(list,index);
} /* of 'delstand' */

void freestandlist(Standlist standlist)
{
    int s;
    Stand *stand;
    foreachstand(stand,s,standlist)
        freestand(stand);
    freelist(standlist);
} /* of 'freestandlist' */
