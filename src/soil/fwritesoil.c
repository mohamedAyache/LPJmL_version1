/***************************************************************************/
/**                                                                       **/
/**               f  w  r  i  t  e  s  o  i  l  .  c                      **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     Function write soil variables into binary file                    **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: 14.10.2009                                           **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"

Bool fwritesoil(FILE *file,const Soil *soil,int ntotpft)
{
    int l;
    forrootsoillayer(l)
    {
        fwrite1(&soil->cpool[l],sizeof(Pool),file);
        fwriten(soil->c_shift_fast[l],sizeof(Real),ntotpft,file);
        fwriten(soil->c_shift_slow[l],sizeof(Real),ntotpft,file);
    }
    if(fwritelitter(file,&soil->litter))
        return TRUE;
    fwriten(soil->w,sizeof(Real),NSOILLAYER,file);
    fwrite1(&soil->w_evap,sizeof(Real),file);
    fwriten(soil->w_fw,sizeof(Real),NSOILLAYER,file);
    fwrite1(&soil->snowpack,sizeof(Real),file);
    fwriten(soil->temp,sizeof(Real),NSOILLAYER+1,file);
    fwriten(soil->ice_depth,sizeof(Real),NSOILLAYER,file);
    fwriten(soil->ice_fw,sizeof(Real),NSOILLAYER,file);
    fwriten(soil->freeze_depth,sizeof(Real),NSOILLAYER,file);
    fwriten(soil->ice_pwp,sizeof(Real),NSOILLAYER,file);
    fwriten(soil->perc_energy,sizeof(Real),NSOILLAYER,file);
    fwriten(soil->state,sizeof(short),NSOILLAYER,file);
    fwrite1(&soil->mean_maxthaw,sizeof(Real),file);
    fwrite1(&soil->alag,sizeof(Real),file);
    fwrite1(&soil->amp,sizeof(Real),file);
    fwrite1(&soil->meanw1,sizeof(Real),file);

    nit_write_restarts_soil(file, soil, ntotpft);

    return FALSE;
} /* of 'fwritesoil' */
