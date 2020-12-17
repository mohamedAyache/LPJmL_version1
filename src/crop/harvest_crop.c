/***************************************************************************/
/**                                                                       **/
/**         h  a  r  v  e  s  t  _  c  r  o  p  .  c                      **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran version         **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: $Date::                                            $ **/
/**     By         : $Author::                                 $          **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"
#include "agriculture.h"
#include "crop.h"

/*
 *  Called in function daily_agriculture when crop
 *  is harvested
 *
 */
void harvest_crop(Output *output,        /* Output data */
        Stand *stand,          /* pointer to crop stand */
        Pft *pft,              /* PFT variables */
        int npft,              /* number of natural PFTs */
        int nagtree,           /* number of agriculture tree PFTs */
        int ncft,              /* number of crop PFTs */
        Bool pft_output_scaled /* pft-specific output scaled with
                                  stand->frac (TRUE/FALSE) */
        )
{
    Pftcroppar *croppar;
    Harvest harvest;
    Pftcrop *crop;
    Irrigation *data;
    Real fuelratio,bifratio,factor;
    data=stand->data;
    crop=pft->data;
    stand->soil.litter.ag[pft->litter].trait.leaf+=(crop->ind.leaf+crop->ind.pool)*RESIDUES_IN_SOIL;
    // barrier.n
    crop->residue.leaf += (crop->ind.leaf+crop->ind.pool)*RESIDUES_IN_SOIL;
    if(!RESIDUES_FIRE)
    {
        harvest.residuals_burnt=harvest.residuals_burntinfield=0;
        factor=(1-RESIDUES_IN_SOIL);
    }
    else
    {
        fuelratio=stand->cell->ml.manage.regpar->fuelratio; /* burn outside of field */
        bifratio=stand->cell->ml.manage.regpar->bifratio; /* burn in field */
        if(bifratio+fuelratio>(1-RESIDUES_IN_SOIL))
        {
            bifratio*=(1-RESIDUES_IN_SOIL);
            fuelratio*=(1-RESIDUES_IN_SOIL);
        }
        factor=1-RESIDUES_IN_SOIL-fuelratio-bifratio;
        harvest.residuals_burnt=(crop->ind.leaf+crop->ind.pool)*fuelratio;
        harvest.residuals_burntinfield=(crop->ind.leaf+crop->ind.pool)*bifratio;
    }
    if(param.remove_residuals)
        harvest.residual=(crop->ind.leaf+crop->ind.pool)*factor;
    else
    {
        stand->soil.litter.ag[pft->litter].trait.leaf+=(crop->ind.leaf+crop->ind.pool)*factor;
        // barrier.n
        crop->residue.leaf += (crop->ind.leaf+crop->ind.pool)*factor;
        harvest.residual=0;
    }
    harvest.harvest=crop->ind.so;
    //printf("harvest(%s)=%g gC/m2\n",pft->par->name,harvest.harvest);
    stand->soil.litter.bg[pft->litter]+=crop->ind.root;
    // barrier.n
    crop->residue.root += crop->ind.root;

    if(pft_output_scaled)
    {
        output->pft_harvest[pft->par->id-npft+data->irrigation*(ncft+NGRASS+NBIOMASSTYPE+nagtree)].harvest+=harvest.harvest*stand->frac;
        output->pft_harvest[pft->par->id-npft+data->irrigation*(ncft+NGRASS+NBIOMASSTYPE+nagtree)].residual+=(harvest.residual+harvest.residuals_burnt+harvest.residuals_burntinfield)*stand->frac;
    }
    else
    {
        output->pft_harvest[pft->par->id-npft+data->irrigation*(ncft+NGRASS+NBIOMASSTYPE+nagtree)].harvest+=harvest.harvest;
        output->pft_harvest[pft->par->id-npft+data->irrigation*(ncft+NGRASS+NBIOMASSTYPE+nagtree)].residual+=(harvest.residual+harvest.residuals_burnt+harvest.residuals_burntinfield);
    }
    /* harvested area */

    output->cftfrac[pft->par->id-npft+data->irrigation*(ncft+NGRASS+NBIOMASSTYPE+nagtree)]+=stand->frac;

    output->flux_harvest+=(harvest.harvest+harvest.residual+harvest.residuals_burnt+harvest.residuals_burntinfield)*stand->frac;
    output->dcflux+=(harvest.harvest+harvest.residual+harvest.residuals_burnt+harvest.residuals_burntinfield)*stand->frac;
    output->flux_rharvest_burnt+=harvest.residuals_burnt*stand->frac;
    output->flux_rharvest_burnt_in_field+=harvest.residuals_burntinfield*stand->frac;
    croppar=pft->par->data;
    if(data->irrigation)
        stand->cell->ml.cropdates[pft->par->id-npft].fallow_irrig=croppar->fallow_days;
    else
        stand->cell->ml.cropdates[pft->par->id-npft].fallow=croppar->fallow_days;
} /* of 'harvest_crop' */
