
#include "lpj.h"

#define scanclimatefilename(file,var,path,isfms,what,isout) if(readclimatefilename(file,var,path,isfms,isout)) {if(isout) fprintf(stderr,"ERROR209: Cannot read input filename for '%s'.\n",what); return TRUE;}
#define fscanint2(file,var,name) if(fscanint(file,var,name,isroot(*config))) return TRUE;

/* Reads the filenames that should be used with the nitrogen module.
 *  - text file containing water temperature
 *  - if manure: binary files containing sedac data (N/P, 2d file) and crop/pasture N manure (3d files)
 *  - if wastewater: binary files containing N/P/C inputs (3D files)
 *  - if fertilizers: binary files containing N/P inputs (3D files)
 *
 *  Called in "fscanconfig"
 *
 *  \author Nicolas Barrier
 *  \Mohamed Ayache
 */
Bool nit_fscannitconfig(FILE *file, Config *config)
{
    // barrier.n: scan if use of nitrogen configuration
    fscanint2(file,&config->with_nitrogen,"with_nitrogen");
    if(config->with_nitrogen)
    {
        // barrier.n: scan the boolean for the phyto initialisation
        fscanint2(file,&config->init_phyto, "init_phyto");

        // if we have landuse, then we have the possibility to use
        // fertilizers, manure and/or wastewaters
        if(config->withlanduse==1){
            fscanint2(file,&config->with_wastewater, "with_wastewater");
            fscanint2(file,&config->with_ferti, "with_ferti");
            fscanint2(file,&config->with_manure, "with_manure");
            fscanint2(file,&config->with_nutdrainanddep, "with_nutdrainanddep");
        }

        // if we have no land use, then we don't have the possibility
        // to use anthropogenic inputs
        else{
            config->with_wastewater=0;
            config->with_ferti=0;
            config->with_manure=0;
            config->with_nutdrainanddep=0;

        }


    }
    else  //add this because config->with_wastewater, config->with_ferti and config->with_manure are not initialized to 0
    {
            config->with_wastewater=0;
            config->with_ferti=0;
            config->with_manure=0;
            config->init_phyto =0;
            config->with_nutdrainanddep=0;
    }

    if(config->with_manure){
        // scan the names of the manure related files: N manure sedac and P manure sedac, and N manure for crop and pasture
        scanclimatefilename(file,&config->manure_sedac_n_filename,config->inputdir,FALSE,"nmanure_sedac",isroot(*config));
        scanclimatefilename(file,&config->manure_sedac_p_filename,config->inputdir,FALSE,"pmanure_sedac",isroot(*config));
        scanclimatefilename(file,&config->manure_n_crop_filename,config->inputdir,FALSE,"nmanure_crop",isroot(*config));
        scanclimatefilename(file,&config->manure_n_past_filename,config->inputdir,FALSE,"nmanure_past",isroot(*config));
    }

    if(config->with_wastewater){
        // read the names of the N/P/C wastewater files
        scanclimatefilename(file,&config->wastewater_n_filename,config->inputdir,FALSE,"waste_N",isroot(*config));
        scanclimatefilename(file,&config->wastewater_p_filename,config->inputdir,FALSE,"waste_P",isroot(*config));
        scanclimatefilename(file,&config->wastewater_c_filename,config->inputdir,FALSE,"waste_C",isroot(*config));
    }

    if(config->with_ferti){
        // read the names of the N/P fertilisation files
        scanclimatefilename(file,&config->fert_n_filename,config->inputdir,FALSE,"fert_n",isroot(*config));
        scanclimatefilename(file,&config->fert_p_filename,config->inputdir,FALSE,"fert_p",isroot(*config));
    }

    if(config->with_ferti || config->with_manure)
    {
        // if we use fertilizers or manure, we scan the names of the pulse/no pulse/grass fractions per country per year
        scanclimatefilename(file,&config->pulsefrac_filename, config->inputdir,FALSE,"pulsefrac",isroot(*config));
        scanclimatefilename(file,&config->nopulsefrac_filename, config->inputdir,FALSE,"nopulsefrac",isroot(*config));
        scanclimatefilename(file,&config->grassfrac_filename, config->inputdir,FALSE,"grassfrac",isroot(*config));
        fscanint(file, &config->frac_firstyear, "frac firstyear", isroot(*config));
        fscanint(file, &config->frac_lastyear, "frac lastyear", isroot(*config));
    }

    return FALSE;

}




