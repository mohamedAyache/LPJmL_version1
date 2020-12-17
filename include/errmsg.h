/***************************************************************************/
/**                                                                       **/
/**                 e  r  r  m  s  g  .  h                                **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran version         **/
/**                                                                       **/
/**     Definition of error codes and error messages                      **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: $Date:: 2015-07-22 11:01:20 +0200 (mer. 22 juil. 2#$ **/
/**     By         : $Author:: bloh                            $          **/
/**                                                                       **/
/***************************************************************************/

#ifndef ERRMSG_H /* Already included? */
#define ERRMSG_H

/* Error codes for LPJmL */

#define READ_CONFIG_ERR 1
#define INIT_INPUT_ERR 2
#define INIT_GRID_ERR 3
#define INVALID_CARBON_BALANCE_ERR 4
#define INVALID_WATER_BALANCE_ERR 5
#define NEGATIVE_DISCHARGE_ERR 6
#define INVALID_FIRE_PROB_ERR 7
#define NEGATIVE_SOIL_MOISTURE_ERR 8
#define ALLOC_MEMORY_ERR 9
#define NEGATIVE_STAND_FRAC_ERR 10
#define STAND_FRAC_SUM_ERR 11
#define IS_EMPTY_ERR 12
#define OUT_OF_RANGE_ERR 13
#define INVALID_YEAR_ERR 15
#define CROP_FRACTION_ERR 16
#define NO_NATURAL_STAND_ERR 17
#define WRONG_CULTIVATION_TYPE_ERR 18
#define FPE_ERR 19
#define LIST_NOT_EMPTY_ERR 21
#define NEGATIVE_ESTABLISHMENT_ERR 22
#define SOCKET_ERR 23
#define SEND_IMAGE_ERR 24
#define OPEN_IMAGE_ERR 25
#define NO_SETASIDE_ERR 26
#define FOREST_LEFT_ERR 27
#define OUTFLOW_RESERVOIR_ERR 28
#define PERM_ERR 29
#define GLOBAL_WATER_BALANCE_ERR 30
#define STORE_CLIMATE_ERR 31
#define NO_FMS_ERR 32
#define INITSOILTEMP_ERR 33
#define RW_MANAGE_ERR 34

/* Definition of macros */

#define printfopenerr(filename) fprintf(stderr,"ERROR100: Cannot open file '%s' in %s(): %s.\n",filename,__FUNCTION__,strerror(errno))
#define printallocerr(ptr) fprintf(stderr,"ERROR009: Cannot allocate memory for '%s' in %s().\n",ptr,__FUNCTION__)
#define readstringerr(var) fprintf(stderr,"ERROR103: Cannot read string '%s' in %s().\n",var,__FUNCTION__)

#endif /* of ERRMSG_H */
