
#include "lpj.h"

/* Module for the determination of total crop (pulse and no pulse) and grass fractions as read from an input file */

/* Global variable containing the 2D arrays of crop (pulse and no pulse) and grass fractions
 * used for manure and fertilisation calculations 
 */
CountryFrac *countryfrac;

/* Returns the pulse+nopulse+grass area for a given country at a given year. 
 * Input file is in ha, converted into m2 here (1.0e4 factor)
 *
 * Used for P fertilizers (nit_fert).
 * 
 * @param int countcode: Country code as read from the binary file
 * @param int idyear: year index within the input file (if year==firstyear, idyear==0)
 * 
 * \author Nicolas Barrier
 * \Update Mohamed Ayache
 */
Real nit_allfrac(int countcode, int idyear) {

    Real output;
    output = 1.0e4 * (getfrac(countryfrac->countrygrassfrac, countcode, idyear) + getfrac(countryfrac->countrypulsefrac, countcode, idyear) + getfrac(countryfrac->countrynopulsefrac, countcode, idyear));
    return output;
}

/* Returns the nopulse+grass area for a given country at a given year. 
 * Input file is in ha, converted into m2 here (1.0e4 factor)
 *
 * Used for N fertilizers (nit_fert)
 * 
 * @param int countcode: Country code as read from the binary file
 * @param int idyear: year index within the input file (if year==firstyear, idyear==0)
 * 
 */
Real nit_allfrac_nopulse(int countcode, int idyear) {

    Real output;
    output = 1.0e4 * (getfrac(countryfrac->countrygrassfrac, countcode, idyear) + getfrac(countryfrac->countrynopulsefrac, countcode, idyear));
    return output;
}

/* Returns the nopulse+pulse (crop) area for a given country at a given year. 
 * Input file is in ha, converted into m2 here (1.0e4 factor)
 *
 * Used for manure (nit_manure)
 * 
 * @param int countcode: Country code as read from the binary file
 * @param int idyear: year index within the input file (if year==firstyear, idyear==0)
 * 
 */
Real nit_cropfrac(int countcode, int idyear) {

    Real output;
    output = 1.0e4 * (getfrac(countryfrac->countrynopulsefrac, countcode, idyear) + getfrac(countryfrac->countrypulsefrac, countcode, idyear));
    return output;
}

/* Returns the grass area for a given country at a given year. 
 * Input file is in ha, converted into m2 here (1.0e4 factor)
 *
 * Used for manure (nit_manure)
 * 
 * @param int countcode: Country code as read from the binary file
 * @param int idyear: year index within the input file (if year==firstyear, idyear==0)
 * 
 */
Real nit_grassfrac(int countcode, int idyear) {

    Real output;
    output = 1.0e4 * (getfrac(countryfrac->countrygrassfrac, countcode, idyear)) ;
    return output;
}


/* Reads the file that contains the crop or grass fraction per country (yearly time step).
 *
 * Called in "nit_init_countfrac"
 *
 * WARNING: it assumes that the countries in the input file are ordered as in the managepar.h file.
 *
 * @param Config *config: constant pointer to the current config (used to determine whether to print outputs)
 * @param int countryfrac: pointer to a 2D (country vs. year) fraction array, in which the file data will be stored
 * @param int firstyear: first year in the file
 * @param int lastyear: last year in the input file
 * @param char filename: name of the file to read
 */
void readcropfile(const Config *config, int ***countryfrac, int firstyear, int lastyear, char *filename)
{

    FILE *file;
    int tempint;  // trash int for storing unused int variables
    String s;
    int tempyear;  // year index
    int tempcount;  // country index
    int N;

    // number of yearly records in thge file
    N = lastyear - firstyear + 1;

    //  open the file and scan the 1951 1952 ... 2005 strings
    file = fopen(filename, "r");

    for(tempyear=firstyear; tempyear<=lastyear; tempyear++)
        fscanint(file, &tempint, "year", isroot(*config));   //scan the year
    // TO DO: check that within the file, the value of the firstyear and lastyer are consistent
    // with the parameters (or segmentation fault will occurr)

    // allocate the array (ncountry size) of crop fractions
    // note: here, we use *countryfrac because we allocate the //value// of the pointer argument
    *countryfrac = malloc(sizeof(int *) * config->ncountries)  ;  

    // loop over all the countries
    for(tempcount=0; tempcount<config->ncountries; tempcount++)
    {
        // read the country name
        fscanstring(file, s);

        // alloc the Nyear array associated with the current country
        // note: here we use *countryfrac because it is the //value// of the pointer argument, i.e.
        // it is the pointer to the country array
        *(*countryfrac + tempcount) = malloc(sizeof(int) * N);

        // loop over all the years and read the array value
        for(tempyear=firstyear; tempyear<=lastyear; tempyear++){
            // note: here, it is a bit complex: "*countryfrac + tempcount" refers to te country column of the array
            // *(*countryfrac + tempcount) refers to the first yearly element associated with the country column
            fscanint(file, *(*countryfrac + tempcount) + tempyear-firstyear, "frac", isroot(*config));
        }
    }

    fclose(file);

}

/* Initialises the coutryfrac structure, which is the structure that contains the annual surface of crop (pulse and no pulse) and grass per country.
 * - Allocation of the structure pointer
 * - Set the firstyear and lastyear attributes (first and last file record)
 * - Read the crop (pulse and no pulse) and the grass fraction file using the "readcropfile" routine. Argument is the pointer to the pointer to pointer array 
 *   (in order to allow for modification through the function call)
 *
 *   Called in "main"
 *
 *   @param Config config: constant pointer to the configuration object (to get file names)
 *
 *   \author Nicolas Barrier
 */
void nit_init_countfrac(const Config *config)
{

    // allocation of the global variable
    countryfrac = malloc(sizeof(CountryFrac));

    // set the firtyear and lastyear arguments as equal to those of the
    // config objects (i.e. from the .conf file)
    countryfrac->firstyear = config->frac_firstyear;
    countryfrac->lastyear = config->frac_lastyear;

    // read the 2D crop pulse fraction
    readcropfile(config, &countryfrac->countrypulsefrac, countryfrac->firstyear, countryfrac->lastyear, config->pulsefrac_filename.name);
    // read the 2D crop no pulse fraction
    readcropfile(config, &countryfrac->countrynopulsefrac, countryfrac->firstyear, countryfrac->lastyear, config->nopulsefrac_filename.name);
    // read the 2D grass fraction
    readcropfile(config, &countryfrac->countrygrassfrac, countryfrac->firstyear, countryfrac->lastyear, config->grassfrac_filename.name);

}
