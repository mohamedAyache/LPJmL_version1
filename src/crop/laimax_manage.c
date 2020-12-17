/***************************************************************************/
/**                                                                       **/
/**            l  a  i  m  a  x  _  m  a  n  a  g  e  .  c                **/
/**                                                                       **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: 14.05.2007, Marlies Gumpenberger                     **/
/**     Last change: 23.10.2008, J. Heinke                                **/
/**     Last change: 12.06.2009, Werner von Bloh                          **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"

void laimax_manage(Manage *manage, 
        const Pftpar pftpar[], /* crop PFT parameter array */
        int npft, /* number of natural PFTs */
        int ncft, /* number of crop PFTs */
        int year  /* year (AD) */
        )
{
    int cft;
    const Pftcroppar *croppar;
    if(manage->par->laimax_cft==NULL)
        for(cft=0;cft<ncft;cft++)
        {
            /* Section for interpolation of laimax between 1950 and 2000   */


            croppar=pftpar[cft].data; 

            if(pftpar[cft].id==TROPICAL_CEREALS)
                manage->laimax[npft+cft]=croppar->laimax;
            else
            {
                if(year<=1949)
                    manage->laimax[npft+cft]=croppar->laimin;
                else if(year>2003)
                    switch(pftpar[cft].id)
                    {
                        case TEMPERATE_CEREALS:
                            manage->laimax[npft+cft]=manage->par->laimax_tempcer;
                            break;
                        case MAIZE:
                            manage->laimax[npft+cft]=manage->par->laimax_maize;
                            break;
                        default:
                            manage->laimax[npft+cft]=croppar->laimax;
                    }
                else
                    switch(pftpar[cft].id)
                    {
                        case TEMPERATE_CEREALS:
                            /*calculate linear trend of country-specific laimax from 1950 onwards*/
                            /*function y=ax+b   (x,y): (1950,laimin)....(2000,laimax)*/
                            manage->laimax[npft+cft]=(manage->par->laimax_tempcer-croppar->laimin)/50*year+40*croppar->laimin-39*manage->par->laimax_tempcer;
                            break;
                        case MAIZE:
                            manage->laimax[npft+cft]=(manage->par->laimax_maize-croppar->laimin)/50*year+40*croppar->laimin-39*manage->par->laimax_maize;
                            break;
                        default:
                            manage->laimax[npft+cft]=(croppar->laimax-croppar->laimin)/50*year+40*croppar->laimin-39*croppar->laimax;
                    }
            }
        }
} /* of 'laimax_manage' */

/* note: the wintercrop can be still at the field when a new laimax is calculated at the beginning of the year */
