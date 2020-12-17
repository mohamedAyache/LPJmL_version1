
/* Module dedicated to Wastewater.
 *
 * \author Nicolas Barrier
 * \update Mohamed Ayache
 */

#include "lpj.h"

/* Global variables associated with wastewaters */
CountryWasteData *countrywastedata;  /* pointer to a structure containing sewp, sewn and sewp */

/* input files associated with wastewater */
Climatefile *fwaste_n_file;
Climatefile *fwaste_p_file;
Climatefile *fwaste_c_file;

/* Initialises the file variables associated with wastewater.
 * - Open the binary files containing wastewater values (fwaste_N variables)
 * - Allocates the sewn, sewp and sewc arrays associated with the data structure
 *
 * Called in "main" (lpjml.c) when "landuse" and "wastewater" are both activated.
 *
 * @param Config config: constant pointer to the config. object
 *
 * \author Nicolas Barrier
 */
int nit_init_wastewater_country_par(const Config *config)
{

    countrywastedata = malloc(sizeof(CountryWasteData));
    fwaste_n_file = malloc(sizeof(Climatefile));
    fwaste_p_file = malloc(sizeof(Climatefile));
    fwaste_c_file = malloc(sizeof(Climatefile));

    if(nit_openfiles(fwaste_n_file, &config->wastewater_n_filename, "%", sizeof(int), config))
    {
        free(fwaste_n_file);
        return TRUE;
    }

    if(nit_openfiles(fwaste_p_file, &config->wastewater_p_filename, "%", sizeof(int), config))
    {
        free(fwaste_p_file);
        return TRUE;
    }

    if(nit_openfiles(fwaste_c_file, &config->wastewater_c_filename, "%", sizeof(int), config))
    {
        free(fwaste_c_file);
        return TRUE;
    }

    /* Initialisation of the data arrays (one element per grid cell) */
    countrywastedata->sewn = newvec(Real, fwaste_n_file->n);
    countrywastedata->sewp = newvec(Real, fwaste_p_file->n);
    countrywastedata->sewc = newvec(Real, fwaste_c_file->n);

    return FALSE;
}

/* Reads the wastewater files (SEWN, SEWP, SEWDOC) for the current year.
 *
 * Data are in g
 *
 * Called once a year in "iterate".
 *
 * @param Config config: constant pointer to the configuration object.
 * @param Cell grid: array of Cell objects
 * @param int year: Current year
 *
 * \author Nicolas Barrier
 */
void nit_update_waste_file(const Config *config, const Cell grid[], int year)
{

    nit_readfiles(fwaste_n_file, countrywastedata->sewn, 0.0, fwaste_n_file->scalar, grid, year, config);
    nit_readfiles(fwaste_p_file, countrywastedata->sewp, 0.0, fwaste_p_file->scalar, grid, year, config);
    nit_readfiles(fwaste_c_file, countrywastedata->sewc, 0.0, fwaste_c_file->scalar, grid, year, config);

}

/* Update the value of the NUTwas variables (which are the daily wastewater inputs
 * inferred from the input files).
 *
 * For nitrogen: equations 5
 * For phospherus: equation 18
 * For carbon: equation 24
 *
 * Called at the beginning of "iterateyear_river".
 *
 * @param Cell grid: array of Cell objects
 * @param int cell: cell index
 * @param Config config: constant pointer to the config object
 *
 * \author Nicolas Barrier
 *
 */
void nit_update_daily_wastewater(Cell grid[], int cell, const Config *config )
{
    //* update for nitrogen
    grid[cell].discharge.NO3was =  FRAC_SEW_IN * FRAC_SEW_DIN_NO3 * *(countrywastedata->sewn + cell);
    grid[cell].discharge.NH4was =  FRAC_SEW_IN * (1 - FRAC_SEW_DIN_NO3) * *(countrywastedata->sewn + cell);
    grid[cell].discharge.DONwas =  (1 - FRAC_SEW_IN) * *(countrywastedata->sewn + cell);
    grid[cell].discharge.PO4was =  FRAC_SEW_IN * *(countrywastedata->sewp + cell);
    grid[cell].discharge.DOPwas =  (1 - FRAC_SEW_IN) * *(countrywastedata->sewp + cell);

    //* update for DOC
    grid[cell].discharge.DOCwas =  *(countrywastedata->sewc + cell);

}
