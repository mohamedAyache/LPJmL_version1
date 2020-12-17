/***************************************************************************/
/**                                                                       **/
/**                     d  a  t  e  .  c                                  **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change:  13.12.2012                                          **/
/**                                                                       **/
/***************************************************************************/

#include <stdio.h>
#include "types.h"
#include "date.h"

Real dayseconds1=1.0/24/60/60;

int ndaymonth[NMONTH]=
{
  31,28,31,30,31,30,31,31,30,31,30,31
};

MReal ndaymonth1=
{
  1/31.0,1/28.0,1/31.0,1/30.0,1/31.0,1/30.0,1/31.0,1/31.0,1/30.0,1/31.0,
  1/30.0,1/31.0
};

int midday[NMONTH+1]=
{
  15,43,74,104,135,165,196,227,257,288,318,349,380 
};

char *months[NMONTH]={"Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep",
                      "Oct","Nov","Dec"};

MReal diffday=
{
  1/28.0,1/31.0,1/30.0,1/31.0,1/30.0,1/31.0,1/31.0,1/30.0,1/31.0,1/30.0,
  1/31.0,1/31.0
};

void cvrtdaymonth(int *dayofmonth, /* on return day of month (1..31) */
                  int *month,      /* on return month (1..12) */
                  int day          /* day of year (1..365) */
                 )
{
  int i,sum;
  sum=0;
  for(i=0;i<NMONTH;i++)
    if(day<=sum+ndaymonth[i])
    {
      *month=i+1;
      *dayofmonth=day-sum;
      break;
    }
    else
      sum+=ndaymonth[i];
} /* of 'cvrtdaymonth' */

Bool isleapyear(int year)
{
  /* Function determines whether year is leap year */
  if(year % 100==0)
    return (year % 400==0);
  else
    return (year % 4==0);
} /* of 'isleapyear' */

int nleapyears(int startyear,
               int lastyear
              )              /* returns number of leap years in interval */
{
  int year,count;
  count=0;
  for(year=startyear;year<lastyear;year++)
    if(isleapyear(year))
      count++;
  return count;
} /* of 'nleapyears' */

