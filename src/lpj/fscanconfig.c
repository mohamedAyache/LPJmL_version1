/***************************************************************************/
/**                                                                       **/
/**                   f  s  c  a  n  c  o  n  f  i  g  .  c               **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     Function reads configuration from an ascii file                   **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: $Date:: 2015-11-06 11:17:06 +0100 (ven., 06 nov. 2#$ **/
/**     By         : $Author:: bloh                            $          **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"

#define fscanint2(file,var,name) if(fscanint(file,var,name,isroot(*config))) return TRUE;
#define fscanreal2(file,var,name) if(fscanreal(file,var)){ if(isroot(*config)) readrealerr(name); return TRUE;}
#define fscanname(file,var,name) if(fscanstring(file,var)){if(isroot(*config)) readstringerr(name); return TRUE;}
#define scanfilename(file,var,path,what,isout) if(readfilename2(file,var,path,isout)) { if(isout) fprintf(stderr,"ERROR209: Cannot read input filename for '%s'.\n",what); return TRUE; }
#define scanclimatefilename(file,var,path,isfms,what,isout) if(readclimatefilename(file,var,path,isfms,isout)) {if(isout) fprintf(stderr,"ERROR209: Cannot read input filename for '%s'.\n",what); return TRUE;}

static Bool readfilename2(FILE *file,Filename *name,const char *path,Bool isout)
{
    if(readfilename(file,name,path,FALSE,isout))
        return TRUE;
    if(name->fmt==CDF)
    {
        if(isout)
            fprintf(stderr,"ERROR197: NetCDF is not supported for input '%s' in this version of LPJmL.\n",name->name);
        return TRUE;
    }
    else if(name->fmt==TXT)
    {
        if(isout)
            fprintf(stderr,"ERROR197: text file is not supported for input '%s' in this version of LPJmL.\n",name->name);
        return TRUE;
    }
    return FALSE;
} /* of 'readfilename2' */

/* barrier.n: removing of static name */
Bool readclimatefilename(FILE *file,Filename *name,const char *path,Bool isfms,Bool isout)
{
    if(readfilename(file,name,path,TRUE,isout))
        return TRUE;
    if(!isfms && name->fmt==FMS)
    {
        if(isout)
            fputs("ERROR197: FMS coupler not allowed for input.\n",stderr);
        return TRUE;
    }
    if(name->fmt==TXT)
    {
        if(isout)
            fprintf(stderr,"ERROR197: text file is not supported for input '%s' in this version of LPJmL.\n",name->name);
        return TRUE;
    }
    return FALSE;
} /* of 'readfilename2' */


static void divide(int *start, /* index of first grid cell */
        int *end,   /* index of last grid cell */
        int rank,   /* my rank id */
        int ntask   /* total number of tasks */
        )
{
    /*
     * Function is used in the parallel MPI version to distribute the cell grid
     * equally on ntask tasks
     * On return start and end are set to the local boundaries of each task
     */
    int i,lo,hi,n;
    n=*end-*start+1;
    lo=*start;
    hi=*start+n/ntask-1;
    if(n % ntask)
        hi++;
    for(i=1;i<=rank;i++)
    {
        lo=hi+1;
        hi=lo+n/ntask-1;
        if(n % ntask>i)
            hi++;
    }
    *start=lo;
    *end=hi;
} /* of 'divide' */

Bool fscanconfig(Config *config,    /* LPJ configuration */
        FILE *file,        /* File pointer to text file */
        Fscanpftparfcn scanfcn[], /* array of PFT-specific scan
                                     functions */
        int ntypes,        /* Number of PFT classes */
        int nout_max       /* maximum number of output files */
        )                   /* returns TRUE on error */
{
    String name;
    int restart,endgrid,israndom,wateruse,grassfix,grassharvest;

    /*=================================================================*/
    /* I. Reading type section                                         */
    /*=================================================================*/

    fscanint2(file,&israndom,"prec");
    if(israndom==RANDOM_PREC)
    {
        fscanint2(file,&config->seed,"random seed");
        if(config->seed==RANDOM_SEED)
            config->seed=time(NULL);
    }
    else
        config->seed=0;
    fscanint2(file,&config->with_radiation,"with_radiation");
#ifdef IMAGE
    if(config->sim_id==LPJML_IMAGE && config->with_radiation)
    {
        if(isroot(*config))
            fputs("ERROR208: Radiation data not supported by IMAGE coupler.\n",
                    stderr);
        return TRUE;
    }
#endif
    fscanint2(file,&config->fire,"fire");
    if(config->sim_id==LPJ)
        config->firewood=NO_FIREWOOD;
    else
    {
        fscanint2(file,&config->firewood,"firewood");
    }
    fscanint2(file,&config->ispopulation,"population");
    fscanint2(file,&config->river_routing,"river routing");
    config->reservoir=FALSE;
    fscanint2(file,&config->permafrost,"permafrost");
    config->sdate_option=NO_FIXED_SDATE;
    config->rw_manage=NO_RWMANAGEMENT;
    if(config->sim_id!=LPJ)
    {
        if(fscanint(file,&config->withlanduse,"withlanduse",isroot(*config)))
            return TRUE;
        if(config->withlanduse!=NO_LANDUSE)
        {
            fscanint2(file,&config->sdate_option,"sowing date option");
            if(config->sdate_option<0 || config->sdate_option>PRESCRIBED_SDATE)
            {
                if(isroot(*config))
                    fprintf(stderr,"ERROR166: Invalid value for sowing date option=%d.\n",
                            config->sdate_option);
                return TRUE;
            }
            fscanint2(file,&config->irrig_scenario,
                    "irrigation scenario");
            if(config->irrig_scenario<0 || config->irrig_scenario>ALL_IRRIGATION)
            {
                if(isroot(*config))
                    fprintf(stderr,"ERROR166: Invalid value for irrigation scenario=%d.\n",
                            config->irrig_scenario);
                return TRUE;
            }
            fscanint2(file,&config->rw_manage,"et management scenario");
            /* TODO remove next two lines when paper on water management is published */
            if(config->rw_manage)
                fail(RW_MANAGE_ERR,TRUE,"Rain-water management module not yet published, please switch off (JJ 06/2015).");
            fscanint2(file,&config->laimax_interpolate,"laimax_interpolate");
            config->isconstlai=(config->laimax_interpolate==CONST_LAI_MAX);
            if(config->river_routing)
                fscanint2(file,&config->reservoir,"reservoir");
            fscanint2(file,&grassfix,"grassland_fixed_pft");
            fscanint2(file,&grassharvest,"grass_harvest_options");
#ifndef NEW_GRASS
            if(isroot(*config) && (grassfix==GRASS_FIXED_PFT || grassharvest==GRASS_HARVEST_OPTIONS))
                fputs("WARNING020: Grassland management options will be ignored when code is not compiled with -DNEW_GRASS.\n",stderr);
#endif
        }
        fscanint2(file,&wateruse,"wateruse");
    }
    else
    {
        config->withlanduse=NO_LANDUSE;
        wateruse=NO_WATERUSE;
    }


    /*=================================================================*/
    /* II. Reading input parameter section                             */
    /*=================================================================*/

    /* ntypes is set to natural vegetation must be considered
     * in light and establishment
     * crops must have last id-number */
    /* Read PFT parameter array */
    if(fscanparam(file,config))
        return TRUE;
    if((config->nsoil=fscansoilpar(file,&config->soilpar,isroot(*config)))==0)
        return TRUE;
    if((config->npft=fscanpftpar(file,&config->pftpar,scanfcn,ntypes,isroot(*config)))==NULL)
        return TRUE;
    config->ntypes=ntypes;
    config->nbiomass=getncultivationtype(config->pftpar,config->npft[GRASS]+config->npft[TREE],BIOMASS);
    config->nagtree=getncultivationtype(config->pftpar,config->npft[GRASS]+config->npft[TREE],ANNUAL_TREE);
    /* Read soil paramater array */
    if(config->withlanduse!=NO_LANDUSE)
    {
        /* landuse enabled */
        if((config->ncountries=fscancountrypar(file,&config->countrypar,config->rw_manage,(config->laimax_interpolate==LAIMAX_CFT) ? config->npft[CROP] : 0,isroot(*config)))==0)
            return TRUE;
        if((config->nregions=fscanregionpar(file,&config->regionpar,isroot(*config)))==0)
            return TRUE;
        if(fscantreedens(file,config->countrypar,config->ncountries,config->nagtree, isroot(*config))==0)
            return TRUE;
    }
    else
    {
        config->countrypar=NULL;
        config->regionpar=NULL;
    }

    fscanint2(file,&config->compress,"compress");
#ifdef USE_NETCDF
    if(config->compress)
        fputs("WARNING403: Compression of NetCDF files is not supported in this version of NetCDF.\n",stderr);
#endif
    if(fscanfloat(file,&config->missing_value,"missing value",isroot(*config)))
        return TRUE;
    config->outnames=fscanoutputvar(file,NOUT,isroot(*config));
    if(config->outnames==NULL)
        return TRUE;
    /*=================================================================*/
    /* III. Reading input data section                                 */
    /*=================================================================*/

    scanclimatefilename(file,&config->soil_filename,config->inputdir,FALSE,"soil",isroot(*config));
    if(config->soil_filename.fmt!=CDF)
    {
        if(fscancoord(file,&config->resolution,isroot(*config)))
        {
            if(isroot(*config))
                fputs("ERROR118: Cannot read resolution in fscanconfig().\n",stderr);
            return TRUE;
        }
        if(config->resolution.lat<=0 || config->resolution.lon<=0)
        {
            if(isroot(*config))
                fprintf(stderr,"ERROR117: Invalid resolution (%g,%g) in fscanconfig().\n",
                        config->resolution.lat,config->resolution.lon);
            return TRUE;
        }
        scanfilename(file,&config->coord_filename,config->inputdir,"coord",isroot(*config));
    }
    scanclimatefilename(file,&config->drainclass_filename,config->inputdir,FALSE,"drainclass",isroot(*config));
    if(config->withlanduse!=NO_LANDUSE)
    {
        scanclimatefilename(file,&config->countrycode_filename,config->inputdir,FALSE,"countrycode",isroot(*config));
        if(config->countrycode_filename.fmt==CDF)
        {
            scanclimatefilename(file,&config->regioncode_filename,config->inputdir,FALSE,"regioncode",isroot(*config));
        }
        scanclimatefilename(file,&config->landuse_filename,config->inputdir,FALSE,"landuse",isroot(*config));
        if(config->sdate_option==PRESCRIBED_SDATE)
        {
            scanclimatefilename(file,&config->sdate_filename,config->inputdir,FALSE,"sdate",isroot(*config));
        }
#ifdef NEW_GRASS
        if(grassfix == GRASS_FIXED_PFT) // SR, grass management options
        {
            scanclimatefilename(file,&config->grassfix_filename,config->inputdir,FALSE,"Grassland fixed pft",isroot(*config));
        }
        else
            config->grassfix_filename.name = NULL;
        if(grassharvest == GRASS_HARVEST_OPTIONS)
        {
            scanclimatefilename(file,&config->grassharvest_filename,config->inputdir,FALSE,"Grassland harvest options",isroot(*config));
        }
        else
            config->grassharvest_filename.name = NULL;
        //SR, grass management options
#endif
    }
#ifdef NEW_GRASS
    else
    {
        config->grassfix_filename.name = NULL;
        config->grassharvest_filename.name = NULL;
    }
#endif
    if(config->river_routing)
    {
        scanclimatefilename(file,&config->lakes_filename,config->inputdir,FALSE,"lakes",isroot(*config));
        scanclimatefilename(file,&config->drainage_filename,config->inputdir,FALSE,"drainage",isroot(*config));
        if(config->drainage_filename.fmt==CDF)
        {
            scanclimatefilename(file,&config->river_filename,config->inputdir,FALSE,"river",isroot(*config));
        }
        if(config->withlanduse!=NO_LANDUSE)
        {
            scanclimatefilename(file,&config->neighb_irrig_filename,config->inputdir,FALSE,"neighb_irrig",isroot(*config));
            if(config->reservoir)
            {
                scanclimatefilename(file,&config->elevation_filename,config->inputdir,FALSE,"elevation",isroot(*config));
                scanfilename(file,&config->reservoir_filename,config->inputdir,"reservoir",isroot(*config));
            }
        }
    }
    scanclimatefilename(file,&config->temp_filename,config->inputdir,config->sim_id==LPJML_FMS,"temp",isroot(*config));
    scanclimatefilename(file,&config->prec_filename,config->inputdir,config->sim_id==LPJML_FMS,"prec",isroot(*config));
    if(config->with_radiation)
    {
        if(config->with_radiation==RADIATION || config->with_radiation==RADIATION_LWDOWN)
            scanclimatefilename(file,&config->lwnet_filename,config->inputdir,config->sim_id==LPJML_FMS,"lwnet",isroot(*config));
        scanclimatefilename(file,&config->swdown_filename,config->inputdir,config->sim_id==LPJML_FMS,"swdown",isroot(*config));
    }
    else
    {
        scanclimatefilename(file,&config->cloud_filename,config->inputdir,config->sim_id==LPJML_FMS,"cloud",isroot(*config));
    }
    if(config->fire==SPITFIRE)
    {
        scanclimatefilename(file,&config->wind_filename,config->inputdir,config->sim_id==LPJML_FMS,"wind",isroot(*config));
        scanclimatefilename(file,&config->tamp_filename,config->inputdir,config->sim_id==LPJML_FMS,"tamp",isroot(*config));
#ifdef TMAX_SPITFIRE
        if(config->tamp_filename.fmt==CDF)
        {
            scanclimatefilename(file,&config->tmax_filename,config->inputdir,config->sim_id==LPJML_FMS,"tmax",isroot(*config));
        }
        else
#endif
            config->tmax_filename.name=NULL;
        scanclimatefilename(file,&config->lightning_filename,config->inputdir,FALSE,"lightning",isroot(*config));
        scanclimatefilename(file,&config->human_ignition_filename,
                config->inputdir,FALSE,"human_ignition",isroot(*config));
    }
    if(config->ispopulation)
    {
        scanclimatefilename(file,&config->popdens_filename,config->inputdir,FALSE,"popdens",isroot(*config));
    }

    if(readfilename(file,&config->co2_filename,config->inputdir,FALSE,isroot(*config)))
    {
        if(isroot(*config))
            fputs("ERROR209: Cannot read input filename for 'co2'.\n",stderr);
        return TRUE;
    }

    if(config->co2_filename.fmt!=TXT &&  (config->sim_id!=LPJML_FMS || config->co2_filename.fmt!=FMS))
    {
        if(isroot(*config))
            fputs("ERROR197: Only text file is supported for CO2 input in this version of LPJmL.\n",stderr);
        return TRUE;
    }

    if(israndom==RANDOM_PREC)
    {
        scanclimatefilename(file,&config->wet_filename,config->inputdir,config->sim_id==LPJML_FMS,"wetdays",isroot(*config));
    }
#ifdef IMAGE
    else if(config->sim_id==LPJML_IMAGE)
    {
        if(isroot(*config))
            fputs("ERROR175: Number of wet days must be supplied for IMAGE coupler.\n",stderr);
        return TRUE;
    }
#endif
    else
        config->wet_filename.name=NULL;
    if(wateruse==WATERUSE)
    {
        scanclimatefilename(file,&config->wateruse_filename,config->inputdir,FALSE,"wateruse",isroot(*config));
    }
    else
        config->wateruse_filename.name=NULL;
#ifdef IMAGE
    if(config->sim_id==LPJML_IMAGE)
    {
        /* reading IMAGE-coupling specific information */
        scanclimatefilename(file,&config->temp_var_filename,config->inputdir,FALSE,"temp_var",isroot(*config));
        scanclimatefilename(file,&config->prec_var_filename,config->inputdir,FALSE,"prec_var",isroot(*config));
        scanclimatefilename(file,&config->prodpool_init_filename,config->inputdir,FALSE,"prodpool_init",isroot(*config));
    }
#endif
    /*=================================================================*/
    /* IV. Reading output data section                                 */
    /*=================================================================*/

    if(fscanoutput(file,config,nout_max))
        return TRUE;

    /*=================================================================*/
    /* V. Reading run settings section                                 */
    /*=================================================================*/

    fscanint2(file,&config->startgrid,"startgrid");
    if(config->startgrid==ALL)
    {
        config->startgrid=0;
        endgrid=getnsoilcode(&config->soil_filename,config->nsoil,isroot(*config));
        if(endgrid==-1)
            return TRUE;
        else if(endgrid==0)
        {
            if(isroot(*config))
                fputs("ERROR135: No soil code found.\n",stderr);
            return TRUE;
        }
        else
            endgrid--;
    }
    else
        fscanint2(file,&endgrid,"endgrid");
    if(endgrid<config->startgrid)
    {
        if(isroot(*config))
            fprintf(stderr,"ERROR136: Endgrid=%d less than startgrid=%d.\n",
                    endgrid,config->startgrid);
        return TRUE;
    }
    config->nall=endgrid-config->startgrid+1;
    config->firstgrid=config->startgrid;
    if(config->nall<config->ntask)
    {
        if(isroot(*config))
            fprintf(stderr,"ERROR198: Number of cells %d less than number of tasks.\n",config->nall);
        return TRUE;
    }
    if(config->ntask>1) /* parallel mode? */
        divide(&config->startgrid,&endgrid,config->rank,
                config->ntask);
    config->ngridcell=endgrid-config->startgrid+1;
    fscanint2(file,&config->nspinup,"nspinup");
    if(config->nspinup)
        fscanint2(file,&config->nspinyear,"nspinyear");
    fscanint2(file,&config->firstyear,"firstyear");
    fscanint2(file,&config->lastyear,"lastyear");
#ifdef IMAGE
    if(config->sim_id==LPJML_IMAGE)
    {
        fscanint2(file,&config->start_imagecoupling,"start_imagecoupling");
    }
    else
        config->start_imagecoupling=INT_MAX;
#endif
    if(config->firstyear-config->nspinup>config->lastyear)
    {
        if(isroot(*config))
            fprintf(stderr,"ERROR105: First simulation year=%d greater than last simulation year=%d.\n",
                    config->firstyear-config->nspinup,config->lastyear);
        return TRUE;
    }
    fscanint2(file,&restart,"restart");
    if(restart==RESTART)
    {
        fscanname(file,name,"restart filename");
        config->restart_filename=addpath(name,config->restartdir);
        config->from_restart=TRUE;
    }
    else
    {
        config->restart_filename=NULL;
        config->from_restart=FALSE;
    }
    fscanint2(file,&restart,"write_restart");
    if(restart==RESTART)
    {
        fscanname(file,name,"write_restart filename");
        config->write_restart_filename=addpath(name,config->restartdir);
        fscanint2(file,&config->restartyear,"restart year");
        if(config->restartyear>config->lastyear || config->restartyear<config->firstyear-config->nspinup)
        {
            if(isroot(*config))
                fprintf(stderr,"WARNING014: Restart year=%d not in simulation years, no restart file written.\n",config->restartyear);
            free(config->write_restart_filename);
            config->write_restart_filename=NULL;
        }
    }
    else
        config->write_restart_filename=NULL;

    /* barrier.n: extracting the names of the input files of the nitrogen configuration */
    if (nit_fscannitconfig(file, config)){
        return TRUE;
    }

    return FALSE;
} /* of 'fscanconfig' */
