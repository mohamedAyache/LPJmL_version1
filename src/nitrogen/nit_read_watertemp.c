
/* Module for the reading of water (river) temperature.
 *
 * \author Nicolas Barrier
 */

#define fscanreal2(isout,file,var,name) \
    if(fscanreal(file,var,name,isout)) \
{ \
    if(isout)\
    fprintf(stderr,"ERROR110: Cannot read '%s' in %s().\n","water_temp",__FUNCTION__); \
    return TRUE; \
}

#define fscanint2(isout,file,var,name) \
    if(fscanint(file,var,name,isout)) \
{ \
    if(isout)\
    fprintf(stderr,"ERROR110: Cannot read '%s' in %s().\n","dayofyear",__FUNCTION__); \
    return TRUE; \
}

#include "lpj.h"

/* Global variable: seasonal cycle for water temperature
 * (366 elements)
 */
//Real *watertemp; mouginot remove whole file

/* Reads the file containing the water temperature.
 *
 * @param Filename filename: name of the water temperature file
 * @param Bool isout: determines whether to print outputs.
 *
 * Called in "main"
 *
 * \author Nicolas Barrier
 *
 * */

int nit_read_watertemp(const Filename *filename, Bool isout)
{

    FILE *file;
    int i;
    int N=366;  // Number of records (366, since it takes into account leap years)
    int temp;


    watertemp = newvec(Real, N);


    file = fopen(filename->name, "r");
printf("filename->name %s \n", filename->name);
    if (file==NULL)
        return 1;


    for (i=0; i<N; i++)
    {
        fscanint2(isout, file, &temp, "dayofyear");
        fscanreal2(isout, file, watertemp + i, "watertemp");
    }

    fclose(file);

    return 0;
}


