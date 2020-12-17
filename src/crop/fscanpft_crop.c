/***************************************************************************/
/**                                                                       **/
/**             f  s  c  a  n  p  f  t  _  c  r  o  p  .  c               **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran version         **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: 03.05.2007   by S.Jachner, M.Gumpenberger            **/
/**     Last change: 28.02.2013   by W. von Bloh                          **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"
#include "crop.h"
#include "grass.h"

#define fscanpftreal(isout,file,var,pft,name) \
    if(fscanreal(file,var,name,isout)) \
{ \
    if(isout)\
    fprintf(stderr,"ERROR110: Cannot read CFT '%s' in %s().\n",pft,__FUNCTION__); \
    return TRUE; \
}
#define fscanpftint(isout,file,var,pft,name) \
    if(fscanint(file,var,name,isout)) \
{ \
    if(isout)\
    fprintf(stderr,"ERROR110: Cannot read CFT '%s' in %s().\n",pft,__FUNCTION__); \
    return TRUE; \
}
#define fscanpftlimit(isout,file,var,pft,name) \
    if(fscanlimit(file,var,isout)) \
{ \
    if(isout)\
    fprintf(stderr,"ERROR112: Cannot read limit '%s' of CFT '%s' in %s().\n",name,pft,__FUNCTION__); \
    return TRUE; \
}

#define fscancropdate2(isout,file,var,pft,name)\
    if(fscancropdate(file,var,isout))\
{\
    if(isout)\
    fprintf(stderr,"ERROR112: Cannot read '%s' of CFT '%s' in %s().\n",name,pft,__FUNCTION__); \
    return TRUE; \
}

static Bool fscancropdate(FILE *file,Initdate *initdate,Bool isout)
{
    if(fscanint(file,&initdate->sdatenh,"date northern hemisphere",isout))
        return TRUE;
    if(fscanint(file,&initdate->sdatesh,"date southern hemisphere",isout))
        return TRUE;
    return FALSE;
} /* of 'fscancropdate' */

#define fscancropphys2(isout,file,var,pft,name)\
    if(fscancropphys(file,var,isout))\
{\
    if(isout)\
    fprintf(stderr,"ERROR114: Cannot read cropphys '%s' of CFT '%s' in %s().\n",name,pft,__FUNCTION__); \
    return TRUE; \
}

static Bool fscancropphys(FILE *file,Cropphys *phys,Bool isout)
{
    if(fscanreal(file,&phys->root,"root",isout))
        return TRUE;
    if(fscanreal(file,&phys->so,"so",isout))
        return TRUE;
    if(fscanreal(file,&phys->pool,"pool",isout))
        return TRUE;
    if(phys->root<=0 || phys->so<=0 || phys->pool<=0)
        return TRUE;
    return FALSE;
} /* of 'fscancropphys' */

Bool fscanpft_crop(FILE *file,           /* file pointer */
        Pftpar *pft,          /* Pointer to Pftpar array */
        Bool isout            /* error output is generated */
        )                      /* returns FALSE for success */
{
    Pftcroppar *crop;
    pft->newpft=new_crop;
    pft->npp=NULL;
    pft->leaf_phenology=NULL;
    pft->fwrite=fwrite_crop;
    pft->fprint=fprint_crop;
    pft->fread=fread_crop;
    pft->litter_update=litter_update_crop;
    pft->actual_lai=actual_lai_crop;
    pft->fpar=fpar_crop;
    pft->alphaa_manage=alphaa_crop;
    pft->free=free_crop;
    pft->vegc_sum=vegc_sum_crop;
    pft->wdf=wdf_crop;
    pft->fprintpar=fprintpar_crop;
    pft->livefuel_consumption=NULL;
    pft->annual=NULL;
    pft->albedo=albedo_grass;
    crop=new(Pftcroppar);
    check(crop);
    pft->data=crop;
    fscanpftint(isout,file,&crop->calcmethod_sdate,pft->name,
            "calcmethod_sdate");
    fscancropdate2(isout,file,&crop->initdate,pft->name,"init date");
    fscanpftint(isout,file,&crop->hlimit,pft->name,
            "max length of crop cycle");
    fscanpftint(isout,file,&crop->fallow_days,pft->name,
            "fallow period after harvest");
    fscanpftreal(isout,file,&crop->temp_fall,pft->name,
            "temperature threshold for fall");
    fscanpftreal(isout,file,&crop->temp_spring,pft->name,
            "temperature threshold for spring");
    fscanpftreal(isout,file,&crop->temp_vern,pft->name,
            "temperature threshold for vernalization");
    fscanpftlimit(isout,file,&crop->trg,pft->name,"vernalization temperature");
    fscanpftreal(isout,file,&crop->pvd,pft->name,"number of vernalising days");
    fscanpftreal(isout,file,&crop->psens,pft->name,
            "photoperiod sensitivity");
    fscanpftreal(isout,file,&crop->pb,pft->name,"photoperiod basal");
    fscanpftreal(isout,file,&crop->ps,pft->name,"photoperiod saturated");
    fscanpftlimit(isout,file,&crop->phuw,pft->name,
            "potential heat units winter");
    fscanpftlimit(isout,file,&crop->phus,pft->name,
            "potential heat units summer");
    fscanpftreal(isout,file,&crop->phu_par,pft->name,
            "parameter for determining the variability of phu");
    fscanpftlimit(isout,file,&crop->basetemp,pft->name,"base temperature");
    fscanpftreal(isout,file,&crop->fphuc,pft->name,
            "growing season to the first point");
    fscanpftreal(isout,file,&crop->flaimaxc,pft->name,
            "plant maximal LAI to the first point");
    fscanpftreal(isout,file,&crop->fphuk,pft->name,
            "growing season to the second point");
    fscanpftreal(isout,file,&crop->flaimaxk,pft->name,
            "plant maximal LAI to the second point");
    fscanpftreal(isout,file,&crop->fphusen,pft->name,"growing period");
    fscanpftreal(isout,file,&crop->flaimaxharvest,pft->name,
            "maximal LAI at harvest");
    fscanpftreal(isout,file,&crop->laimax,pft->name,"maximal LAI");
    fscanpftreal(isout,file,&crop->laimin,pft->name,"minimal LAI");
    fscanpftreal(isout,file,&crop->hiopt,pft->name,"optimum harvest index");
    fscanpftreal(isout,file,&crop->himin,pft->name,"minimum harvest index");
    fscanpftreal(isout,file,&crop->shapesenescencenorm,pft->name,
            "shapesenescencenorm");
    fscancropphys2(isout,file,&crop->cn_ratio,pft->name,"cn_ratio");
    crop->cn_ratio.root=pft->respcoeff*param.k/crop->cn_ratio.root;
    crop->cn_ratio.so=pft->respcoeff*param.k/crop->cn_ratio.so;
    crop->cn_ratio.pool=pft->respcoeff*param.k/crop->cn_ratio.pool;
    return FALSE;
} /* of 'fscanpft_crop' */

/*
   - called in fscanpftpar()
   - scans crop specific parameters from pft.par
   -> struct Pftcroppar in crop.h
   */
