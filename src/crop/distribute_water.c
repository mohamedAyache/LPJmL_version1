/***************************************************************************/
/**                                                                       **/
/**          d  i  s  t  r  i  b  u  t  e  _  w  a  t  e  r  .  c         **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     Mathematical description of the river routing algorithm can be    **/
/**     found in:                                                         **/
/**     Rost, S., Gerten, D., Bondeau, A., Lucht, W., Rohwer, J.,         **/
/**     Schaphoff, S.: Agricultural green and blue water consumption and  **/
/**     its influence on the global water system. Water Resources         **/
/**     Research (in press).                                              **/
/**                                                                       **/
/**     Parallel version using the Pnet library                           **/
/**     Pnet is described in:                                             **/
/**     W. von Bloh, 2008. Sequential and Parallel Implementation of      **/
/**     Networks. In P. beim Graben, C. Zhou, M. Thiel, and J. Kurths     **/
/**     (eds.), Lectures in Supercomputational Neuroscience, Dynamics in  **/
/**     Complex Brain Networks, Springer, New York, 279-318.              **/
/**                                                                       **/
/**     written by Jens Heinke, Stefanie Rost, Werner von Bloh,           **/
/**                Sibyll Schaphoff                                       **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: 04.06.2010 by Jens Heinke                            **/
/**     Last change: 15.04.2014 by J. Jaegermeyr                          **/
/**     Last change: $Date:: 2015-07-31 11:06:28 +0200 (ven., 31 juil. #$ **/
/**     By         : $Author:: bloh                            $          **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"
#include "agriculture.h"

void distribute_water(Cell *cell,            /* pointer to cell */
        int irrig_scenario,
        int pft_output_scaled,
        int npft,              /* number of natural PFTs */
        int nagtree,           /* number of agricultural tree PFTs */
        int ncft               /* number of crop PFTs */
        )
{
    int s,p,l,m,count;
    Real wr;
    Real conv_loss,irrig_stand;
    Real no3_stand;
    Real frac_irrig_amount,frac_unsustainable,aprec,irrig_threshold;
    Stand *stand;
    Pft *pft;
    Irrigation *data;
    aprec=irrig_threshold=0.0;
    conv_loss=0.0;

    /* actual irrigation requirement */
    if(irrig_scenario==LIM_IRRIGATION)
    {
        frac_irrig_amount=cell->discharge.gir>0 ? cell->discharge.withdrawal/cell->discharge.gir : 0.0;
    }
    else
    {
        /* potential irrigation requirement */
        frac_irrig_amount=cell->discharge.gir>0 ? 1.0 : 0.0;
        frac_unsustainable=cell->discharge.gir>0 ? 1 - cell->discharge.withdrawal/cell->discharge.gir : 0.0;
        frac_unsustainable=frac_unsustainable>0 ? frac_unsustainable : 0.0;
        cell->output.awd_unsustainable+=frac_unsustainable*cell->discharge.gir;
        cell->output.mwd_unsustainable+=frac_unsustainable*cell->discharge.gir;
    }

    cell->discharge.withdrawal=0.0;

    foreachstand(stand,s,cell->standlist)
        if(stand->type->landusetype==AGRICULTURE || stand->type->landusetype==GRASSLAND || stand->type->landusetype==BIOMASS_GRASS || stand->type->landusetype==BIOMASS_TREE || stand->type->landusetype==AGRICULTURE_TREE || stand->type->landusetype==AGRICULTURE_GRASS)
        {
            data=stand->data;
            data->irrig_event=0;
            data->irrig_amount=0;

            if(data->irrigation)
            {
                /* determine if irrigation today */
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

                    count+=(wr>irrig_threshold) ? 0 : 1; /* if single grass pft needs irrigation both grass pft are irrigated */
                } /*for each pft*/
                data->irrig_event=(count>0) ? 1 : 0;

                irrig_stand=max(data->net_irrig_amount+data->dist_irrig_amount-data->irrig_stor,0)*frac_irrig_amount;

                //no3store is missing
                if (cell->discharge.gir != 0) {
                    no3_stand = cell->discharge.NO3withdraw*max(data->net_irrig_amount+data->dist_irrig_amount-data->irrig_stor,0)/cell->discharge.gir ;

                }
                else{
                    no3_stand =0;
                }
                /* conveyance losses *///ignore conv_loss for N and P atm
                conv_loss=irrig_stand*(1/data->ec-1);

                if(data->irrig_event) /* if irrigation today */
                {
                    switch(stand->type->landusetype)
                    {
                        case AGRICULTURE:
                            /* write net irrigation requirement */
                            pft=getpft(&stand->pftlist,0);
                            if(pft_output_scaled)
                                stand->cell->output.cft_nir[pft->par->id-npft+data->irrigation*(ncft+NGRASS+NBIOMASSTYPE+nagtree)]+=data->net_irrig_amount*stand->frac;
                            else
                                stand->cell->output.cft_nir[pft->par->id-npft+data->irrigation*(ncft+NGRASS+NBIOMASSTYPE+nagtree)]+=data->net_irrig_amount;
                            break;
                        case GRASSLAND:
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
                            break;
                        case BIOMASS_GRASS:
                            if(pft_output_scaled)
                                stand->cell->output.cft_nir[rbgrass(ncft)+data->irrigation*(ncft+NGRASS+NBIOMASSTYPE+nagtree)]+=data->net_irrig_amount*stand->cell->ml.landfrac[1].biomass_grass;
                            else
                                stand->cell->output.cft_nir[rbgrass(ncft)+data->irrigation*(ncft+NGRASS+NBIOMASSTYPE+nagtree)]+=data->net_irrig_amount;
                            break;
                        case BIOMASS_TREE:
                            if(pft_output_scaled)
                                stand->cell->output.cft_nir[rbtree(ncft)+data->irrigation*(ncft+NGRASS+NBIOMASSTYPE+nagtree)]+=data->net_irrig_amount*stand->cell->ml.landfrac[1].biomass_tree;
                            else
                                stand->cell->output.cft_nir[rbtree(ncft)+data->irrigation*(ncft+NGRASS+NBIOMASSTYPE+nagtree)]+=data->net_irrig_amount;
                            break;
                        default:
                            if(pft_output_scaled)
                                stand->cell->output.cft_nir[agtree(ncft)+stand->pft_id-npft+nagtree+data->irrigation*(ncft+NGRASS+NBIOMASSTYPE+nagtree)]+=data->net_irrig_amount*stand->frac;
                            else
                                stand->cell->output.cft_nir[agtree(ncft)+stand->pft_id-npft+nagtree+data->irrigation*(ncft+NGRASS+NBIOMASSTYPE+nagtree)]+=data->net_irrig_amount;
                    } /* of switch */

                    data->irrig_amount=data->irrig_stor+irrig_stand;

                    stand->soil.NO3[0]+= (no3_stand+ data->no3_stor)/(stand->frac *stand->cell->coord.area);

                    data->irrig_stor=0.0;
                    data->no3_stor =0.0;
                    /* excess water goes back to irrig_stor *///TODO manage that for no3
                    if(data->irrig_amount>(data->net_irrig_amount+data->dist_irrig_amount))
                    {
                        data->irrig_stor=data->irrig_amount-(data->net_irrig_amount+data->dist_irrig_amount);
                        data->irrig_amount=data->net_irrig_amount+data->dist_irrig_amount;
                    }
                }
                else
                {
                    data->irrig_stor+=irrig_stand;
                    data->no3_stor+=no3_stand;
                    data->irrig_amount=0.0;
                }

                data->net_irrig_amount=data->dist_irrig_amount=0.0;

                cell->discharge.dmass_lake+=conv_loss*cell->coord.area*(1-data->conv_evap)*stand->frac; /* Note: conveyance losses are not included in return flow */
                cell->discharge.mfin+=conv_loss*cell->coord.area*(1-data->conv_evap)*stand->frac;
                cell->output.mconv_loss_drain+=conv_loss*(1-data->conv_evap)*stand->frac;
                cell->output.mconv_loss_evap+=conv_loss*data->conv_evap*stand->frac;

                /* write cft-specific conveyance losses */
                /* not written within irrig_event loop, because irrig_stor consists of water already transported to the field */
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
                        stand->cell->output.cft_conv_loss_drain[rbtree(ncft)+stand->pft_id-npft+nagtree+data->irrigation*(ncft+NGRASS+NBIOMASSTYPE+nagtree)]+=conv_loss*(1-data->conv_evap)*stand->frac;
                    }
                    else
                    {
                        stand->cell->output.cft_conv_loss_evap[agtree(ncft)+stand->pft_id-npft+nagtree+data->irrigation*(ncft+NGRASS+NBIOMASSTYPE+nagtree)]+=conv_loss*data->conv_evap;
                        stand->cell->output.cft_conv_loss_drain[rbtree(ncft)+stand->pft_id-npft+nagtree+data->irrigation*(ncft+NGRASS+NBIOMASSTYPE+nagtree)]+=conv_loss*(1-data->conv_evap);
                    }
                }
            } /* if data->irrigation = TRUE */
        }/* end of for each stand loop */
} /* of 'distribute_water' */
