/***************************************************************************/
/**                                                                       **/
/**      f  p  r  i  n  t  _  a  g  r  i  c  u  l  t  u  r  e  .  c       **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     Function prints irrigation data of stand                          **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: 08.10.2009                                           **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"
#include "agriculture.h"

void fprint_agriculture(FILE *file,        /* pointer to text file */
                        const Stand *stand /* pointer to stand */
                       )
{
  Irrigation *irrigation;
  irrigation=stand->data;
  fprintf(file,"Irrigation:\t%s\n"
          "Irrigation event today:\t%d\n"
          "Irrigation system:\t%d\n"
          "conveyance efficiency:\t%g\n"
          "conveyance evaporation:\t%g\n"
          "net irrig amount:\t%g\n"
          "distribution irrig amount:\t%g\n"
          "irrig amount:\t%g\n"
          "irrig stor:\t%g\n",
          (irrigation->irrigation) ? "true" : "false",irrigation->irrig_event,irrigation->irrig_system,
          irrigation->ec,irrigation->conv_evap,irrigation->net_irrig_amount,irrigation->dist_irrig_amount,irrigation->irrig_amount,irrigation->irrig_stor);
} /* of 'fprint_agriculture' */
