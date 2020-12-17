/***************************************************************************/
/**                                                                       **/
/**     r  e  c  e  i  v  e  _  i  m  a  g  e  _  c  o  2  .  c           **/
/**                                                                       **/
/**     extension of LPJ to couple LPJ online with IMAGE                  **/
/**                                                                       **/
/**     written by Christoph Mueller                                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: 12.11.2009 by Werner von Bloh                        **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"

#ifdef IMAGE

Real receive_image_co2(const Config *config /* LPJ configuration */
                      )                  /* returns atmospheric CO2 (ppm) */
{
  double co2;
#ifdef USE_MPI
  if(config->rank==0)
  {
#endif
#ifdef DEBUG_IMAGE
  printf("reading CO2 from IMAGE\n");
  fflush(stdout);
#endif
  readdouble_socket(config->in,&co2,1);
#ifdef DEBUG_IMAGE
  printf("got CO2 (%g) from IMAGE\n",co2);
  fflush(stdout);
#endif
#ifdef USE_MPI
  }
  MPI_Bcast(&co2,1,MPI_DOUBLE,0,config->comm);
#endif
  return (Real)co2;
} /* of 'receive_image_co2' */

#endif
