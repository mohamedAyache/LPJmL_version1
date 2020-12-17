/***************************************************************************/
/**                                                                       **/
/**           s  e  t  o  u  t  p  u  t  _  i  m  a  g  e  .  c           **/
/**                                                                       **/
/**     extension of LPJ to couple LPJ online with IMAGE                  **/
/**     Set output for Image land use                                     **/
/**                                                                       **/
/**     written by Christoph Mueller                                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: 31.03.2011 by Werner von Bloh                        **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"

#ifdef IMAGE

void setoutput_image(Cell *cell,int ncft)
{
  int cft;
  for(cft=0;cft<ncft;cft++)
  {
    cell->output.cft_luc_image[cft]=cell->ml.landfrac[0].crop[cft];
    cell->output.cft_luc_image[cft+ncft+NGRASS+NBIOMASSTYPE]=cell->ml.landfrac[1].crop[cft];
  }
  cell->output.cft_luc_image[rothers(ncft)]=cell->ml.landfrac[0].grass[0];
  cell->output.cft_luc_image[rmgrass(ncft)]=cell->ml.landfrac[0].grass[1];
  cell->output.cft_luc_image[rbgrass(ncft)]=cell->ml.landfrac[0].biomass_grass;
  cell->output.cft_luc_image[rbtree(ncft)]=cell->ml.landfrac[0].biomass_tree;
  cell->output.cft_luc_image[rothers(ncft)+ncft+NGRASS+NBIOMASSTYPE]=cell->ml.landfrac[1].grass[0];
  cell->output.cft_luc_image[rmgrass(ncft)+ncft+NGRASS+NBIOMASSTYPE]=cell->ml.landfrac[1].grass[1];
  cell->output.cft_luc_image[rbgrass(ncft)+ncft+NGRASS+NBIOMASSTYPE]=cell->ml.landfrac[1].biomass_grass;
  cell->output.cft_luc_image[rbtree(ncft)+ncft+NGRASS+NBIOMASSTYPE]=cell->ml.landfrac[1].biomass_tree;
} /* of 'setoutput_image' */

#endif
