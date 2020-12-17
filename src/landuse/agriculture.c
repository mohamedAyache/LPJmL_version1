/***************************************************************************/
/**                                                                       **/
/**                    a  g  r  i  c  u  l  t  u  r  e  .  c              **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     Definition of agricultural, setaside and kill stand               **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: 15.09.2019                                           **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"
#include "natural.h"
#include "agriculture.h"

Standtype setaside_rf_stand={SETASIDE_RF,"setaside_rf",new_agriculture,
                             free_agriculture,fwrite_agriculture,
                             fread_agriculture,fprint_agriculture,
#ifdef DAILY_ESTABLISHMENT
                             daily_setaside,
#else
                             daily_natural,
#endif
                             annual_setaside,NULL};

Standtype setaside_ir_stand={SETASIDE_IR,"setaside_ir",new_agriculture,
                             free_agriculture,fwrite_agriculture,
                             fread_agriculture,fprint_agriculture,
#ifdef DAILY_ESTABLISHMENT
                             daily_setaside,
#else
                             daily_natural,
#endif
                             annual_setaside,NULL};

Standtype managedforest_stand={MANAGEDFOREST,"managed forest",NULL,NULL,NULL,
                               NULL,NULL,NULL,NULL,NULL};

Standtype kill_stand={KILL,"kill",NULL,free_agriculture,NULL,NULL,NULL,NULL,NULL,
                      NULL};

Standtype agriculture_stand={AGRICULTURE,"agriculture",new_agriculture,
                             free_agriculture,fwrite_agriculture,
                             fread_agriculture,fprint_agriculture,
                             daily_agriculture,annual_agriculture,NULL};
