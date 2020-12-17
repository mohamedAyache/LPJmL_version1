/***************************************************************************/
/**                                                                       **/
/**                      l  p  j  p  r  i  n  t  .  c                     **/
/**                                                                       **/
/**     print restart file of LPJ                                         **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: $Date:: 2015-07-24 11:22:32 +0200 (ven., 24 juil. #$ **/
/**     By         : $Author:: bloh                            $          **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"
#include "grass.h"
#include "tree.h"
#include "crop.h"
#include "natural.h"
#include "grassland.h"
#include "agriculture.h"
#include "biomass_tree.h"
#include "biomass_grass.h"
#include "agriculture_tree.h"
#include "agriculture_grass.h"

#define PRINTLPJ_VERSION "1.0.016"
#define NTYPES 3
#define NSTANDTYPES 11 /* number of stand types */
#define dflt_conf_filename "lpjml.conf"
#define USAGE "Usage: %s [-h] [-inpath dir] [-restartpath dir]\n"\
              "       [[-Dmacro[=value]] [-Idir] ...] [filename [-check] [start [end]]]\n"


static Bool printgrid(Config *config, /* Pointer to LPJ configuration */
                      Standtype standtype[],
                      int npft,       /* number of natural PFTs */
                      int ncft,       /* number of crop PFTs */
                      Bool isout      /* print output (TRUE/FALSE) */
             )
{
  Cell grid;
  int i,version;
  Bool swap,swap_cow;
  int soilcode;
  Code code;
  Real k_perc;
  String headername;
  FILE *file_restart,*file_countrycode;
  Celldata celldata;

  /* Open coordinate file */
  celldata=opencelldata(config);
  if(celldata==NULL)
    return TRUE;
  if(seekcelldata(celldata,config->startgrid,config->nsoil))
  {
    closecelldata(celldata);
    return TRUE;
  }

  /* Open countrycode file */
  if(config->withlanduse!=NO_LANDUSE)
  {
    file_countrycode=opencountrycode(&config->countrycode_filename,
                                     &swap_cow,headername,&version,TRUE);
    if(file_countrycode==NULL)
      return TRUE;
    if(seekcountrycode(file_countrycode,config->startgrid,headername,version))
    {
      fclose(file_countrycode);
      closecelldata(celldata);
      return TRUE;
    }
  }
  /* If FROM_RESTART open restart file */
  config->count=0;
  file_restart=openrestart(config->write_restart_filename,config,npft+ncft,&swap);
  if(file_restart==NULL)
    return TRUE;

  for(i=0;i<config->ngridcell;i++)
  {
    if(readcelldata(celldata,&grid.coord,&soilcode,&k_perc,i,config))
      break;
    if(config->countrypar!=NULL)
    {
      if(readcountrycode(file_countrycode,&code,swap_cow))
      {
        fprintf(stderr,"WARNING008: Unexpected end of file in '%s', number of gridcells truncated to %d.\n",config->countrycode_filename.name,i);
        config->ngridcell=i;
        break;
      }
      if(code.country<0 || code.country>=config->ncountries ||
         code.region<0 || code.region>=config->nregions)
      {
          if(soilcode>=1 && soilcode<=config->nsoil)
            fprintf(stderr,"WARNING009: Invalid countrycode=%d or regioncode=%d with valid soilcode in cell (not skipped)\n",code.country,code.region);
          grid.ml.manage.laimax=NULL;
          grid.ml.manage.par=NULL;
          grid.ml.manage.regpar=NULL;
      }
      else
        initmanage(&grid.ml.manage,config->countrypar+code.country,
                   config->regionpar+code.region,npft,config->nagtree,ncft,config->isconstlai);
    }
    else
    {
      grid.ml.manage.laimax=NULL;
      grid.ml.manage.par=NULL;
      grid.ml.manage.regpar=NULL;
    }
    /* Init cells */
    grid.ml.cropfrac_rf=grid.ml.cropfrac_ir=0;
    grid.balance.totw=grid.balance.totc=0.0;
    grid.discharge.dmass_lake=0.0;
    grid.discharge.next=0;
    grid.ml.fraction=NULL;
    grid.ml.resdata=NULL;
    grid.ml.dam=FALSE;
    if(config->withlanduse!=NO_LANDUSE)
    {
      grid.ml.landfrac=newvec(Landfrac,2);
      newlandfrac(grid.ml.landfrac,ncft,config->nagtree);
    }
    else
      grid.ml.landfrac=NULL;
    initoutput(&grid.output,config->crop_index,config->crop_irrigation,npft,config->nbiomass,config->nagtree,ncft);
    /*grid.cropdates=init_cropdates(&config.pftpar+npft,ncft,grid.coord.lat); */

    if(freadcell(file_restart,&grid,config->pftpar,npft,config->nagtree,ncft,
                 config->soilpar+soilcode-1,standtype,NSTANDTYPES,config->withlanduse,
                 config->sdate_option,config->sdate_option_restart,config->river_routing,swap))
    {
      fprintf(stderr,"WARNING008: Unexpected end of file in '%s', number of gridcells truncated to %d.\n",config->write_restart_filename,i);
      config->ngridcell=i;
      break;
    }
    if(isout)
      printcell(&grid,1,ncft,config->withlanduse!=NO_LANDUSE,config->river_routing);
    if(grid.ml.landfrac!=NULL)
    {
      freelandfrac(grid.ml.landfrac);
      free(grid.ml.landfrac);
    }
    freecell(&grid,npft,config->river_routing);
  } /* of for(i=0;...) */
  fclose(file_restart);
  closecelldata(celldata);
  if(config->withlanduse!=NO_LANDUSE)
    fclose(file_countrycode);
  return FALSE;
}
int main(int argc,char **argv)
{
  int startgrid,ngridcell;
  Config config;
  Bool rc,isout;
  char *endptr;
  const char *progname;
  Fscanpftparfcn scanfcn[NTYPES]={fscanpft_grass,fscanpft_tree,fscanpft_crop};
  Standtype standtype[NSTANDTYPES];

  standtype[NATURAL]=natural_stand;
  standtype[SETASIDE_RF]=setaside_rf_stand;
  standtype[SETASIDE_IR]=setaside_ir_stand;
  standtype[AGRICULTURE]=agriculture_stand;
  standtype[MANAGEDFOREST]=managedforest_stand;
  standtype[GRASSLAND]=grassland_stand,
  standtype[BIOMASS_TREE]=biomass_tree_stand,
  standtype[BIOMASS_GRASS]=biomass_grass_stand,
  standtype[AGRICULTURE_TREE]=agriculture_tree_stand,
  standtype[AGRICULTURE_GRASS]=agriculture_grass_stand,
  standtype[KILL]=kill_stand;

  progname=strippath(argv[0]);
  if(argc>1 && !strcmp(argv[1],"-h"))
  {
    printf("%s Version %s (" __DATE__ ") - print contents of restart files for LPJmL\n",progname,PRINTLPJ_VERSION);
    printf(USAGE,progname);
    printf("Arguments:\n"
           "-h               print this help text\n"
           "-inpath dir      directory appended to input filenames\n"
           "-restartpath dir directory appended to restart filename\n"
           "-Dmacro[=value]  define macro for preprocessor of configuration file\n"
           "-Idir            directory to search for include files\n"
           "filename         configuration filename. Default is 'lpjml.conf'\n"
           "-check           check only restart file\n"
           "start            index of first grid cell to print\n"
           "end              index of last grid cell to print\n");
    return EXIT_SUCCESS;
  }
  printf("**** %s Version %s (" __DATE__ ") ****\n",progname,PRINTLPJ_VERSION);
  initconfig(&config);
  if(readconfig(&config,dflt_conf_filename,scanfcn,NTYPES,NOUT,&argc,&argv,USAGE))
    fail(READ_CONFIG_ERR,FALSE,"Error opening config");
  printf("Simulation: %s\n",config.sim_name);
  if(config.write_restart_filename==NULL)
  {
    fprintf(stderr,"No restart file written.\n");
    return EXIT_FAILURE;
  }
  isout=TRUE;
  if(argc>0)
    if(!strcmp(argv[0],"-check"))
    {
      isout=FALSE;
      argv++;
      argc--;
    }
  if(argc>0)
  {
    startgrid=strtol(argv[0],&endptr,10);
    if(*endptr!='\0')
    {
      fprintf(stderr,"Invalid number '%s' for start grid.\n",argv[0]);
      return EXIT_FAILURE;
    }
    if(startgrid<0)
    {
      fprintf(stderr,"Invalid number %d for start grid.\n",startgrid);
      return EXIT_FAILURE;
    }
  }
  else
    startgrid=config.startgrid;
  if(argc>1)
  {
    ngridcell=strtol(argv[1],&endptr,10)-startgrid+1;
    if(*endptr!='\0')
    {
      fprintf(stderr,"Invalid number '%s' for end grid.\n",argv[1]);
      return EXIT_FAILURE;
    }
    if(ngridcell<=0)
    {
      fputs("End grid less than start grid.\n",stderr);
      return EXIT_FAILURE;
    }
  }
  else
    ngridcell=config.ngridcell;
  config.ngridcell=min(ngridcell,config.ngridcell-startgrid+config.startgrid);
  if(startgrid>=config.startgrid)
    config.startgrid=startgrid;
  /*config.restart_filename=config.write_restart_filename; */
  rc=printgrid(&config,standtype,config.npft[TREE]+config.npft[GRASS],config.npft[CROP],isout);
  return (rc) ? EXIT_FAILURE : EXIT_SUCCESS;
} /* of 'main' */
