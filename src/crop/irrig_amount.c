/***************************************************************************/
/**                                                                       **/
/**                i  r  r  i  g  _  a  m  o  u  n  t  .  c               **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     written by Stefanie Rost, Werner von Bloh, Sibyll Schaphoff       **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: 22.06.2006 by S.Jachner                              **/
/**     Last change: 26.02.2009 by S. Schaphoff                           **/
/**     Last change: 15.04.2014 by J. Jaegermeyr                          **/
/**     Last change: $Date::                                            $ **/
/**     By         : $Author::                                 $          **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"
#include "agriculture.h"

void irrig_amount(Stand *stand, /* pointer to non-natural stand */
        Bool pft_output_scaled,
        int npft,     /* number of natural PFTs */
        int nagtree,  /* number of agriculture tree PFTs */
        int ncft      /* number of crop PFTs */
        )
{
    /* calc_wateruse(cell); */
    /* irrig(cell); */
    int l,p,m,count;
    Pft *pft;
    Irrigation *data;
    Real conv_loss,irrig_stand,aprec,irrig_threshold;
    Real wr;
    aprec=irrig_threshold=0.0;

    /* determine if today irrigation dependent on threshold */
    data=stand->data;
    data->irrig_event=0;
    data->irrig_amount=0;

    if(data->irrigation)
    {
        for(m=0;m<NMONTH;m++)
            aprec+=max(0,stand->cell->climbuf.mprec20[m]);
        count=0;
        foreachpft(pft,p,&stand->pftlist)
        {
            wr=0;
            for(l=0;l<LASTLAYER;l++)
                wr+=pft->par->rootdist[l]*(stand->soil.w[l]+stand->soil.ice_depth[l]/stand->soil.par->whcs[l]);

            if(pft->par->path==C3)
            {
                if(aprec<param.aprec_lim)
                    irrig_threshold=param.irrig_threshold_c3_dry;
                else
                    irrig_threshold=param.irrig_threshold_c3_humid;
            }
            else
                irrig_threshold=param.irrig_threshold_c4;
            if(pft->par->id==RICE)
                irrig_threshold=param.irrig_threshold_rice;

            count+=(wr>irrig_threshold) ? 0 : 1; /* if one of possibly two (grass) pfts requests irrigation, both get irrigated */
        } /*for each pft*/
        data->irrig_event=(count>0) ? 1 : 0;

        irrig_stand=max(data->net_irrig_amount+data->dist_irrig_amount-data->irrig_stor,0);

        /* conveyance losses */
        conv_loss=irrig_stand*(1/data->ec-1);

        if(data->irrig_event)
        {

            /* net irrigation requirement */
            pft=getpft(&stand->pftlist,0);
            if(stand->type->landusetype==AGRICULTURE)
            {
                if(pft_output_scaled)
                    stand->cell->output.cft_nir[pft->par->id-npft+data->irrigation*(ncft+NGRASS+NBIOMASSTYPE+nagtree)]+=data->net_irrig_amount*stand->frac;
                else
                    stand->cell->output.cft_nir[pft->par->id-npft+data->irrigation*(ncft+NGRASS+NBIOMASSTYPE+nagtree)]+=data->net_irrig_amount;
            }
            else if(stand->type->landusetype==AGRICULTURE_TREE || stand->type->landusetype==AGRICULTURE_GRASS)
            {
                if(pft_output_scaled)
                    stand->cell->output.cft_nir[stand->pft_id-npft+nagtree+agtree(ncft)+data->irrigation*(ncft+NGRASS+NBIOMASSTYPE+nagtree)]+=data->net_irrig_amount*stand->frac;
                else
                    stand->cell->output.cft_nir[stand->pft_id-npft+nagtree+agtree(ncft)+data->irrigation*(ncft+NGRASS+NBIOMASSTYPE+nagtree)]+=data->net_irrig_amount;
            }
            else if (stand->type->landusetype==BIOMASS_GRASS)
            {
                if(pft_output_scaled)
                    stand->cell->output.cft_nir[rbgrass(ncft)+data->irrigation*(ncft+NGRASS+NBIOMASSTYPE+nagtree)]+=data->net_irrig_amount*stand->cell->ml.landfrac[1].biomass_grass;
                else
                    stand->cell->output.cft_nir[rbgrass(ncft)+data->irrigation*(ncft+NGRASS+NBIOMASSTYPE+nagtree)]+=data->net_irrig_amount;
            }
            else if (stand->type->landusetype==GRASSLAND)
            {
                if(pft_output_scaled)
                {
                    stand->cell->output.cft_nir[rothers(ncft)+data->irrigation*(ncft+NGRASS+NBIOMASSTYPE+nagtree)]+=data->net_irrig_amount*stand->cell->ml.landfrac[1].grass[0];
                    stand->cell->output.cft_nir[rmgrass(ncft)+data->irrigation*(ncft+NGRASS+NBIOMASSTYPE+nagtree)]+=data->net_irrig_amount*stand->cell->ml.landfrac[1].grass[1];
                }
                else
                {
                    stand->cell->output.cft_nir[rothers(ncft)+data->irrigation*(ncft+NGRASS+NBIOMASSTYPE+nagtree)]+=data->net_irrig_amount;
                    stand->cell->output.cft_nir[rmgrass(ncft)+data->irrigation*(ncft+NGRASS+NBIOMASSTYPE+nagtree)]+=data->net_irrig_amount;
                }
            }
            else
            {
                if(pft_output_scaled)
                    stand->cell->output.cft_nir[rbtree(ncft)+data->irrigation*(ncft+NGRASS+NBIOMASSTYPE+nagtree)]+=data->net_irrig_amount*stand->cell->ml.landfrac[1].biomass_tree;
                else
                    stand->cell->output.cft_nir[rbtree(ncft)+data->irrigation*(ncft+NGRASS+NBIOMASSTYPE+nagtree)]+=data->net_irrig_amount;
            }

            data->irrig_amount=data->irrig_stor+irrig_stand;
            data->irrig_stor=0.0;

            if(data->irrig_amount>(data->net_irrig_amount+data->dist_irrig_amount))
            {
                data->irrig_stor=data->irrig_amount-(data->net_irrig_amount+data->dist_irrig_amount);
                data->irrig_amount=data->net_irrig_amount+data->dist_irrig_amount;
            }
        }
        else
        {
            data->irrig_stor+=irrig_stand;
            data->irrig_amount=0.0;
        }

        data->net_irrig_amount=data->dist_irrig_amount=0.0;
        stand->cell->output.mconv_loss_drain+=conv_loss*(1-data->conv_evap)*stand->frac;
        stand->cell->output.mconv_loss_evap+=conv_loss*data->conv_evap*stand->frac;

        /* write cft-specific conveyance losses, ATTENTION: full conv losses incl. evaporation and drainage */
        if(stand->type->landusetype==AGRICULTURE)
        {
            pft=getpft(&stand->pftlist,0);
            if(pft_output_scaled)
            {
                stand->cell->output.cft_conv_loss_evap[pft->par->id-npft+data->irrigation*(ncft+NGRASS+NBIOMASSTYPE+nagtree)]+=conv_loss*data->conv_evap*stand->frac;
                stand->cell->output.cft_conv_loss_drain[pft->par->id-npft+data->irrigation*(ncft+NGRASS+NBIOMASSTYPE+nagtree)]+=conv_loss*(1-data->conv_evap)*stand->frac;
            }
            else
            {
                stand->cell->output.cft_conv_loss_evap[pft->par->id-npft+data->irrigation*(ncft+NGRASS+NBIOMASSTYPE+nagtree)]+=conv_loss*data->conv_evap;
                stand->cell->output.cft_conv_loss_drain[pft->par->id-npft+data->irrigation*(ncft+NGRASS+NBIOMASSTYPE+nagtree)]+=conv_loss*(1-data->conv_evap);
            }
        }
        else if(stand->type->landusetype==AGRICULTURE_TREE || stand->type->landusetype==AGRICULTURE_GRASS)
        {
            if(pft_output_scaled)
            {
                stand->cell->output.cft_conv_loss_evap[stand->pft_id-npft+nagtree+agtree(ncft)+data->irrigation*(ncft+NGRASS+NBIOMASSTYPE+nagtree)]+=data->conv_evap*stand->frac;
                stand->cell->output.cft_conv_loss_drain[stand->pft_id-npft+nagtree+agtree(ncft)+data->irrigation*(ncft+NGRASS+NBIOMASSTYPE+nagtree)]+=(1-data->conv_evap)*stand->frac;
            }
            else
            {
                stand->cell->output.cft_conv_loss_evap[stand->pft_id-npft+nagtree+agtree(ncft)+data->irrigation*(ncft+NGRASS+NBIOMASSTYPE+nagtree)]+=data->conv_evap;
                stand->cell->output.cft_conv_loss_drain[stand->pft_id-npft+nagtree+agtree(ncft)+data->irrigation*(ncft+NGRASS+NBIOMASSTYPE+nagtree)]+=(1-data->conv_evap);
            }
        }
        else if (stand->type->landusetype==GRASSLAND)
        {
            if(pft_output_scaled)
            {
                stand->cell->output.cft_conv_loss_evap[rothers(ncft)+data->irrigation*(ncft+NGRASS+NBIOMASSTYPE+nagtree)]+=conv_loss*data->conv_evap*stand->cell->ml.landfrac[1].grass[0];
                stand->cell->output.cft_conv_loss_evap[rmgrass(ncft)+data->irrigation*(ncft+NGRASS+NBIOMASSTYPE+nagtree)]+=conv_loss*data->conv_evap*stand->cell->ml.landfrac[1].grass[1];
                stand->cell->output.cft_conv_loss_drain[rothers(ncft)+data->irrigation*(ncft+NGRASS+NBIOMASSTYPE+nagtree)]+=conv_loss*(1-data->conv_evap)*stand->cell->ml.landfrac[1].grass[0];
                stand->cell->output.cft_conv_loss_drain[rmgrass(ncft)+data->irrigation*(ncft+NGRASS+NBIOMASSTYPE+nagtree)]+=conv_loss*(1-data->conv_evap)*stand->cell->ml.landfrac[1].grass[1];
            }
            else
            {
                stand->cell->output.cft_conv_loss_evap[rothers(ncft)+data->irrigation*(ncft+NGRASS+NBIOMASSTYPE+nagtree)]+=conv_loss*data->conv_evap;
                stand->cell->output.cft_conv_loss_evap[rmgrass(ncft)+data->irrigation*(ncft+NGRASS+NBIOMASSTYPE+nagtree)]+=conv_loss*data->conv_evap;
                stand->cell->output.cft_conv_loss_drain[rothers(ncft)+data->irrigation*(ncft+NGRASS+NBIOMASSTYPE+nagtree)]+=conv_loss*(1-data->conv_evap);
                stand->cell->output.cft_conv_loss_drain[rmgrass(ncft)+data->irrigation*(ncft+NGRASS+NBIOMASSTYPE+nagtree)]+=conv_loss*(1-data->conv_evap);
            }
        }
        else if (stand->type->landusetype==BIOMASS_GRASS)
        {
            if(pft_output_scaled)
            {
                stand->cell->output.cft_conv_loss_evap[rbgrass(ncft)+data->irrigation*(ncft+NGRASS+NBIOMASSTYPE+nagtree)]+=conv_loss*data->conv_evap*stand->cell->ml.landfrac[1].biomass_grass;
                stand->cell->output.cft_conv_loss_drain[rbgrass(ncft)+data->irrigation*(ncft+NGRASS+NBIOMASSTYPE+nagtree)]+=conv_loss*(1-data->conv_evap)*stand->cell->ml.landfrac[1].biomass_grass;
            }
            else
            {
                stand->cell->output.cft_conv_loss_evap[rbgrass(ncft)+data->irrigation*(ncft+NGRASS+NBIOMASSTYPE+nagtree)]+=conv_loss*data->conv_evap;
                stand->cell->output.cft_conv_loss_drain[rbgrass(ncft)+data->irrigation*(ncft+NGRASS+NBIOMASSTYPE+nagtree)]+=conv_loss*(1-data->conv_evap);
            }
        }
        else if(stand->type->landusetype==BIOMASS_TREE)
        {
            if(pft_output_scaled)
            {
                stand->cell->output.cft_conv_loss_evap[rbtree(ncft)+data->irrigation*(ncft+NGRASS+NBIOMASSTYPE+nagtree)]+=conv_loss*data->conv_evap*stand->cell->ml.landfrac[1].biomass_tree;
                stand->cell->output.cft_conv_loss_drain[rbtree(ncft)+data->irrigation*(ncft+NGRASS+NBIOMASSTYPE+nagtree)]+=conv_loss*(1-data->conv_evap)*stand->cell->ml.landfrac[1].biomass_tree;
            }
            else
            {
                stand->cell->output.cft_conv_loss_evap[rbtree(ncft)+data->irrigation*(ncft+NGRASS+NBIOMASSTYPE+nagtree)]+=conv_loss*data->conv_evap;
                stand->cell->output.cft_conv_loss_drain[rbtree(ncft)+data->irrigation*(ncft+NGRASS+NBIOMASSTYPE+nagtree)]+=conv_loss*(1-data->conv_evap);
            }
        }
        else
        {
            if(pft_output_scaled)
            {
                stand->cell->output.cft_conv_loss_evap[agtree(ncft)+stand->pft_id-npft+nagtree+data->irrigation*(ncft+NGRASS+NBIOMASSTYPE+nagtree)]+=conv_loss*data->conv_evap*stand->frac;
                stand->cell->output.cft_conv_loss_drain[agtree(ncft)+stand->pft_id-npft+nagtree+data->irrigation*(ncft+NGRASS+NBIOMASSTYPE+nagtree)]+=conv_loss*(1-data->conv_evap)*stand->frac;
            }
            else
            {
                stand->cell->output.cft_conv_loss_evap[agtree(ncft)+stand->pft_id-npft+nagtree+data->irrigation*(ncft+NGRASS+NBIOMASSTYPE+nagtree)]+=conv_loss*data->conv_evap;
                stand->cell->output.cft_conv_loss_drain[agtree(ncft)+stand->pft_id-npft+nagtree+data->irrigation*(ncft+NGRASS+NBIOMASSTYPE+nagtree)]+=conv_loss*(1-data->conv_evap);
            }
        }
    } /* if data->irrigation */

}   /* of 'irrig_amount' */
