#include "lpj.h"

void nit_printlog(const Config *config)

{
    fputs("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n", stdout);
    if(config->with_nitrogen)
        fputs("Nitrogen is activated\n", stdout);
    else
        fputs("Nitrogen is deactivated\n", stdout);

    fputs("\n", stdout);

    if(config->init_phyto)
    {
        fputs("Phytoplankton initialisation is activated\n", stdout);
        fputs("Phytoplankton in water is initialised in lakes and river with a value of 1g/m3 of water\n", stdout);
    }

    if(config->with_manure)
    {
        fputs("Manure inputs are activated\n", stdout);
        fputs("N manure sedac file: ", stdout);
        fputs(config->manure_sedac_n_filename.name, stdout);
        fputs("\n", stdout);
        fputs("P manure sedac file: ", stdout);
        fputs(config->manure_sedac_p_filename.name, stdout);
        fputs("\n", stdout);
        fputs("N manure for crop file: ", stdout);
        fputs(config->manure_n_crop_filename.name, stdout);
        fputs("\n", stdout);
        fputs("N manure for pasture file: ", stdout);
        fputs(config->manure_n_past_filename.name, stdout);
        fputs("\n", stdout);
    }
    else
    {
        fputs("Manure inputs are deactivated\n", stdout);
    }

    if(config->with_wastewater)
    {
        fputs("Wastewater inputs are activated\n", stdout);
        fputs("N wastewater file: ", stdout);
        fputs(config->wastewater_n_filename.name, stdout);
        fputs("\n", stdout);
        fputs("P wastewater file: ", stdout);
        fputs(config->wastewater_p_filename.name, stdout);
        fputs("\n", stdout);
        fputs("C wastewater file: ", stdout);
        fputs(config->wastewater_c_filename.name, stdout);
        fputs("\n", stdout);
    }
    else
    {
        fputs("Wastewater inputs are deactivated\n", stdout);
    }

    if(config->with_ferti)
    {
        fputs("Fertilizers inputs are activated\n", stdout);
        fputs("N fertilizer file: ", stdout);
        fputs(config->fert_n_filename.name, stdout);
        fputs("\n", stdout);
        fputs("P fertilizer file: ", stdout);
        fputs(config->fert_p_filename.name, stdout);
        fputs("\n", stdout);
    }
    else
    {
        fputs("Fertilizers inputs are deactivated\n", stdout);
    }

    if(config->with_ferti || config->with_manure)
    {
        fputs("When manure and/or fertilizers, crop/grass fractions per country per year are read\n", stdout);
        fputs("Pulse crop fraction file: ", stdout);
        fputs(config->pulsefrac_filename.name, stdout);
        fputs("\n", stdout);
        fputs("No pulse crop fraction file: ", stdout);
        fputs(config->nopulsefrac_filename.name, stdout);
        fputs("\n", stdout);
        fputs("Grass fraction file: ", stdout);
        fputs(config->grassfrac_filename.name, stdout);
        fputs("\n", stdout);
        fprintf(stdout, "First year in the file: %d\n", config->frac_firstyear);
        fprintf(stdout, "Last year in the file: %d\n", config->frac_lastyear);
   }

    fputs("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n", stdout);

}
