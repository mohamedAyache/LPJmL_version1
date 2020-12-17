/***************************************************************************/
/**                                                                       **/
/**                c  r  e  a  t  e  p  f  t  n  a  m  e  s  .  c         **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     Function creates string vector for desctiption of PFT-specific    **/
/**     output in NetCDF files.                                           **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: $Date:: 2015-07-22 11:01:20 +0200 (mer., 22 juil. #$ **/
/**     By         : $Author:: bloh                            $          **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"

char **createpftnames(int index,           /* output index */
                      int npft,            /* number of natural PFTs */
                      int nbiomass,        /* number of biomass PFTs */
                      int nagtree,         /* number of agriculture tree PFTs */
                      int ncft,            /* number of crop PFTs */
                      const Pftpar *pftpar /* PFT parameter vector */
                     )                     /* returns string vector */
{
  int i,j,incr,size;
  char **pftnames;
  size=outputsize(index,npft,nbiomass,nagtree,ncft);
  pftnames=newvec(char *,size);
  if(pftnames==NULL)
    return NULL;
  switch(index)
  {
    case SDATE: case HDATE:
        for(i=0;i<ncft;i++)
          pftnames[i]=strdup(pftpar[npft+i].name);
        for(i=0;i<ncft;i++)
        {
          pftnames[i+ncft]=malloc(strlen(pftpar[npft+i].name)+strlen("irrigated ")+1);
          strcpy(pftnames[i+ncft],"irrigated ");
          strcat(pftnames[i+ncft],pftpar[npft+i].name);
        }
        break;
    case PFT_NPP: case PFT_GCGP:
      for(i=0;i<npft-nbiomass-nagtree;i++)
        pftnames[i]=strdup(pftpar[i].name);
      incr=npft-nbiomass-nagtree;
      for(i=0;i<2;i++)
      {
        for(j=0;j<ncft;j++)
          if(i)
          {
            pftnames[j+incr]=malloc(strlen(pftpar[npft+j].name)+strlen("irrigated ")+1);
            strcpy(pftnames[j+incr],"irrigated ");
            strcat(pftnames[j+incr],pftpar[npft+j].name);
          }
          else
            pftnames[j+incr]=strdup(pftpar[npft+j].name);
        incr+=ncft;
        pftnames[incr]=strdup((i) ? "irrigated others":  "others");
        pftnames[incr+1]=strdup((i) ? "irrigated pasture": "pasture");
        incr+=2;
        pftnames[incr]=strdup((i) ? "irrigated biomass grass": "biomass grass");
        pftnames[incr+1]=strdup((i) ? "irrigated biomass tree":  "biomass tree");
        incr+=2;
        for(j=0;j<nagtree;j++)
          if(i)
          {
            pftnames[j+incr]=malloc(strlen(pftpar[npft-nagtree+j].name)+strlen("irrigated ")+1);
            strcpy(pftnames[j+incr],"irrigated ");
            strcat(pftnames[j+incr],pftpar[npft+j-nagtree].name);
          }
          else
            pftnames[j+incr]=strdup(pftpar[npft+j-nagtree].name);
         incr+=nagtree;
      } 
      break;
    case PFT_HARVEST: case PFT_RHARVEST: case CFT_CONSUMP_WATER_G: case CFT_EVAP: case CFT_EVAP_B:
    case CFT_CONSUMP_WATER_B: case CFTFRAC: case CFT_AIRRIG: case CFT_FPAR: case CFT_IRRIG_EVENTS:
    case CFT_TRANSP: case CFT_TRANSP_B: case CFT_INTERC: case CFT_INTERC_B: case CFT_RETURN_FLOW_B:
    case CFT_CONV_LOSS_EVAP: case CFT_CONV_LOSS_DRAIN: case CFT_NIR:
      incr=0;
      for(i=0;i<2;i++)
      {
        for(j=0;j<ncft;j++)
          if(i)
          {
            pftnames[j+incr]=malloc(strlen(pftpar[npft+j].name)+strlen("irrigated ")+1);
            strcpy(pftnames[j+incr],"irrigated ");
            strcat(pftnames[j+incr],pftpar[npft+j].name);
          }
          else
            pftnames[j+incr]=strdup(pftpar[npft+j].name);
        incr+=ncft;
        pftnames[incr]=strdup((i) ? "irrigated others":  "others");
        pftnames[incr+1]=strdup((i) ? "irrigated pasture": "pasture");
        incr+=2;
        pftnames[incr]=strdup((i) ? "irrigated biomass grass": "biomass grass");
        pftnames[incr+1]=strdup((i) ? "irrigated biomass tree":  "biomass tree");
        incr+=2;
        for(j=0;j<nagtree;j++)
          if(i)
          {
            pftnames[j+incr]=malloc(strlen(pftpar[npft-nagtree+j].name)+strlen("irrigated ")+1);
            strcpy(pftnames[j+incr],"irrigated ");
            strcat(pftnames[j+incr],pftpar[npft+j-nagtree].name);
          }
          else
            pftnames[j+incr]=strdup(pftpar[npft+j-nagtree].name);
         incr+=nagtree;
      }
      break;
    case CFT_PET:case GROWING_PERIOD:case CFT_TEMP:case CFT_PREC:
    case CFT_SRAD: case CFT_ABOVEGBM:
      incr=0;
      for(i=0;i<2;i++)
      {
        for(j=0;j<ncft;j++)
          if(i)
          {
            pftnames[j+incr]=malloc(strlen(pftpar[npft+j].name)+strlen("irrigated ")+1);
            strcpy(pftnames[j+incr],"irrigated ");
            strcat(pftnames[j+incr],pftpar[npft+j].name);
          }
          else
            pftnames[j+incr]=strdup(pftpar[npft+j].name);
        incr+=ncft;
        pftnames[incr]=strdup((i) ? "irrigated others":  "others");
        pftnames[incr+1]=strdup((i) ? "irrigated pasture": "pasture");
        incr+=2;
      }
      break;
    case FPC:
      pftnames[0]=strdup("natural stand fraction");
      for(i=0;i<npft-nbiomass-nagtree;i++)
        pftnames[i+1]=strdup(pftpar[i].name);
      break;
  }
  return pftnames;
} /* of 'createpftnames' */

void freepftnames(char **pftnames,int index,int npft,int nagtree,int nbiomass,int ncft)
{
  int i,size;
  if(pftnames!=NULL)
  {
    size=outputsize(index,npft,nbiomass,nagtree,ncft);
    for(i=0;i<size;i++)
      free(pftnames[i]);
    free(pftnames);
  }
} /* of 'freepftnames' */
