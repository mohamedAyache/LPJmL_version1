/***************************************************************************/
/**                                                                       **/
/**              u  p  d  a  t  e  _  m  o  n  t  h  l  y  .  c           **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     Function performs necessary updates after iteration over one      **/
/**     month                                                             **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     See file AUTHORS for list of authors contributing to this code    **/
/**                                                                       **/
/**     Last change: 07.03.2012                                           **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"

void update_monthly(Cell *cell,  /* Pointer to cell */
        Real mtemp,  /* monthly average temperature (deg C) */
        Real mprec,  /* monthly average precipitation (mm) */
        int month    /* month (0..11) */
        )
{
    int s,l,p,i;
    Stand *stand;
    Pft *pft;


    monthly_climbuf(&cell->climbuf,mtemp,mprec,cell->output.mpet,month);
    if(cell->ml.dam) /* to store the monthly inflow and demand */
        update_reservoir_monthly(cell,month);

    foreachstand(stand,s,cell->standlist)
    {
        // barrier.n: initialisation of residues and uptakes
        // to 0 for all pfts

        nit_init_residue_daily(stand);
        nit_init_uptake_daily(stand);

        getlag(&stand->soil,month);
        foreachpft(pft,p,&stand->pftlist)
            turnover_monthly(&stand->soil.litter,pft);

        // barrier.n: uptake from soil and adding to litter

        nit_uptake_allpft(stand,month*30);

        nit_remin_allpft(stand);



        for (i =0 ; i<LASTLAYER; i++) {


        if ((stand->annualUptakeP[i] - stand->annualResP[i]) >0)
        {
            stand->soil.RESP[i]+=(stand->annualUptakeP[i] - stand->annualResP[i]);

        }
        if ((stand->annualUptakeN[i] - stand->annualResN[i]) >0)
        {   stand->soil.RESN[i]+=(stand->annualUptakeN[i] - stand->annualResN[i]);


        stand->soil.RESC[i]+=(stand->annualUptakeN[i] - stand->annualResN[i])*RATIO_CN_ROOT*10;
        }
        // C should never be limiting for the decomposers so the quantity of C we have doesn't really matter as long as it is big enough

        // if some residue remains consider them as part of the next year (to avoid creation of matter)
        if ((stand->annualUptakeP[i] - stand->annualResP[i]) >0) {stand->annualResP[i] =0;}
        else {stand->annualResP[i] = stand->annualResP[i] - stand->annualUptakeP[i]; }
        if ((stand->annualUptakeN[i] - stand->annualResN[i]) >0) {stand->annualResN[i] =0;}
        else {stand->annualResN[i] = stand->annualResN[i] - stand->annualUptakeN[i]; }


        stand->annualUptakeN[i] =0;
        stand->annualUptakeP[i] =0;

        }

    } /* of foreachstand */


    for(l=0;l<NSOILLAYER;l++)
        cell->output.mswc[l]*=ndaymonth1[month];
    cell->output.mrootmoist*=ndaymonth1[month];
    cell->output.mfpar*=ndaymonth1[month];
    cell->output.mfiredi*=ndaymonth1[month];
    cell->output.atransp+=cell->output.mtransp;
    cell->output.aevap+=cell->output.mevap;
    cell->output.ainterc+=cell->output.minterc;
    cell->output.airrig+=cell->output.mirrig;
    cell->output.aevap_lake+=cell->output.mevap_lake;
    cell->output.aevap_res+=cell->output.mevap_res;
    cell->output.mswe*=ndaymonth1[month];
    cell->output.aconv_loss_evap+=cell->output.mconv_loss_evap;
    cell->output.aconv_loss_drain+=cell->output.mconv_loss_drain;
#ifdef IMAGE
    cell->ml.image_data->anpp+=cell->output.mnpp;
    cell->ml.image_data->arh+=cell->output.mrh;
#endif
    /* for carbon balance check  */
    cell->balance.nep+=cell->output.mnpp-cell->output.mrh;

    /* for water balance check */
    cell->balance.awater_flux+=cell->output.mrunoff+cell->output.mtransp+
        cell->output.mevap+cell->output.minterc+cell->output.mevap_lake+
        ((cell->discharge.mfout-cell->discharge.mfin)/cell->coord.area)-cell->output.mirrig;
    if(cell->ml.dam)
        cell->balance.awater_flux+=cell->output.mevap_res;


} /* of 'monthly_update' */
