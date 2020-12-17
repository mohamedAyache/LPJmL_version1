/***************************************************************************/
/**                                                                       **/
/**            i  n  i  t  i  g  n  i  t  i  o  n  .  c                   **/
/**                                                                       **/
/**     C implementation of LPJ                                           **/
/**                                                                       **/
/**     Function reads human ignitions from file                          **/ 
/**                                                                       **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: 30.04.2008, Ursula Heyder                            **/
/**     Last change: 13.10.2011, Werner von Bloh                          **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"

Bool initignition(Cell grid[],         /* LPJ grid */
                  const Config *config /* LPJ configuration */
                 )                     /* returns TRUE on error */
{
  FILE *file;
  Header header;
  String headername;
  int version;
  Bool swap;
  int cell;
  short human_ignition;
  Input_netcdf input_netcdf;
  if(config->human_ignition_filename.fmt==CDF)
  {
    input_netcdf=openinput_netcdf(config->human_ignition_filename.name,
                                  NULL,NULL,0,config);
    if(input_netcdf==NULL)
      return TRUE;
    for(cell=0;cell<config->ngridcell;cell++)
    {
      if(readinput_netcdf(input_netcdf,&grid[cell].ignition.human,
                          &grid[cell].coord,&config->resolution))
      {
        closeinput_netcdf(input_netcdf);
        return TRUE;
      }
      grid[cell].ignition.nesterov_accum=0;
    }
    closeinput_netcdf(input_netcdf);
  }
  else
  {
    file=openinputfile(&header,&swap,&config->human_ignition_filename,
                       headername,&version,config);
    if(file==NULL)
    {
     printfopenerr(config->human_ignition_filename.name);
     return TRUE;
    }
    if(version==1)
      header.scalar=0.001;
    if(fseek(file,config->startgrid*sizeof(human_ignition),SEEK_CUR))
    {
      fputs("ERROR186: Cannot seek file for human ignition.\n",stderr);
      fclose(file);
      return TRUE;
    }
    for(cell=0;cell<config->ngridcell;cell++)
    {
      if(!freadshort1(&human_ignition,swap,file))
      {
        fputs("ERROR186: Cannot read human ignition.\n",stderr);
        fclose(file);
        return TRUE;
      }
      grid[cell].ignition.human=(human_ignition<0) ? 0 : (Real)human_ignition*header.scalar;
      grid[cell].ignition.nesterov_accum=0;
    }
    fclose(file);
  }
  return FALSE;
} /* of 'initignition' */
