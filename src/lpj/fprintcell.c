/***************************************************************************/
/**                                                                       **/
/**               f  p  r  i  n  t  c  e  l  l  .  c                      **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     Function prints cell variables in text file (used by lpjprint     **/
/**     to print contents of restart file)                                **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: $Date:: 2015-07-14 09:09:50 +0200 (mar., 14 juil. #$ **/
/**     By         : $Author:: bloh                            $          **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"

void fprintcell(FILE *file,            /* file pointer to text file */
                const Cell grid[],     /* cell array */
                int ncell,             /* number of cells */
                int ncft,              /* number of crop PFTs */
                Bool landuse,          /* land use enabled? (TRUE/FALSE) */
                Bool river_routing     /* river routing (TRUE/FALSE) */
               )
{
  int cell,cft;
  for(cell=0;cell<ncell;cell++)
  {
    fputs("Coord:\t\t",file);
    fprintcoord(file,grid[cell].coord);
    fputc('\n',file);
    if(river_routing)
    {
       fprintf(file,"dmass_lake:\t%g (dm3)\n"
                    "dfout:\t\t%g (dm3/d)\n"
                    "dmass_river:\t%g (dm3)\n"
                    "dmass_sum:\t%g (dm3)\n",
               grid[cell].discharge.dmass_lake,grid[cell].discharge.dfout,
               grid[cell].discharge.dmass_river,grid[cell].discharge.dmass_sum);
       fputs("Queue:\t\t",file);
       fprintqueue(file,grid[cell].discharge.queue);
       fputc('\n',file);
       if(grid[cell].ml.dam)
         fprintresdata(file,grid+cell);
    }
    if(grid[cell].skip)
      fputs("Invalid soil\n",file);
    else
    {
      if(landuse)
      {
        if(grid[cell].ml.sdate_fixed!=NULL)
        {
          fputs("CFT     ",file);
          for(cft=0;cft<ncft;cft++)
            fprintf(file,"%4d",cft);
          fputs("\n--------",file);
          for(cft=0;cft<ncft;cft++)
            fputs(" ---",file);
          fputs("\nfsdate  ",file);
          for(cft=0;cft<ncft;cft++)
            fprintf(file,"%4d",grid[cell].ml.sdate_fixed[cft]);
          fputs("\nirrsdate",file);
          for(cft=ncft;cft<2*ncft;cft++)
            fprintf(file,"%4d",grid[cell].ml.sdate_fixed[cft]);
          fputc('\n',file);
        }
        fprintf(file,"Cropfrac (rf/ir):\t%g\t%g\n",grid[cell].ml.cropfrac_rf,grid[cell].ml.cropfrac_ir);
        fprintcropdates(file,grid[cell].ml.cropdates,ncft);
      }
      fprintstandlist(file,grid[cell].standlist);
    }
  } /* of 'for(cell=...)' */
} /* of 'fprintcell' */
