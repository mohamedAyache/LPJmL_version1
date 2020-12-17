/***************************************************************************/
/**                                                                       **/
/**               r  e  a  d  c  o  2  .  c                               **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     Function initclimate read atmopsheric CO2 concentration from text **/
/**     file.                                                             **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: 01.03.2013                                           **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"

static Bool fscanint2(FILE *file,int *value,const char *name,Bool *iseof,Bool isout)
{
  String line,token;
  char *ptr;
  Bool rc;
  rc=fscantoken(file,token);
  if(rc)
  {
    *iseof=strlen(token)==0;
    return TRUE;
  }
  *iseof=FALSE;
  if(!rc)
  {
    *value=(int)strtol(token,&ptr,10);
    rc=*ptr!='\0';
  }
  if(rc && isout)
  {
    fprintf(stderr,"ERROR101: Cannot read int '%s', line read:\n",name);
    if(fgets(line,STRING_LEN,file)!=NULL)
      line[strlen(line)-1]='\0';
    else
      line[0]='\0';
    fprintf(stderr,"          '%s%s'\n           ",token,line);
    frepeatch(stderr,'^',strlen(token));
    fputc('\n',stderr);
  }
  return rc;
} /* of 'fscanint2' */

Bool readco2(Co2data *co2,const Filename *filename,Bool isout)
{
  FILE *file;
  int yr,yr_old;
  Bool iseof;
  if(filename->fmt==FMS)
  {
    co2->data=NULL;
    co2->nyear=0;
    co2->firstyear=0;
  }
  else if(filename->fmt==TXT)
  {
    if((file=fopen(filename->name,"r"))==NULL)
    {
      if(isout)
        printfopenerr(filename->name);
      return TRUE;
    }
    co2->data=newvec(Real,1);
    if(co2->data==NULL)
    {
      printallocerr("co2");
      fclose(file);
      return TRUE;
    }
    /* find start year in co2-file */
    if(fscanint(file,&yr,"year",isout) || fscanreal(file,co2->data,"co2",isout))
    {
      if(isout)
        fprintf(stderr,"ERROR129: Cannot read CO2 data in first line of '%s'.\n",
                filename->name);
      free(co2->data);
      fclose(file);
      return TRUE;
    }
    co2->firstyear=yr;
    co2->nyear=1;
    yr_old=yr;
    while(!feof(file))
    {
      co2->data=(Real *)realloc(co2->data,sizeof(Real)*(co2->nyear+1));
      if(co2->data==NULL)
      {
        printallocerr("co2");
        fclose(file);
        return TRUE;
      }
      if(fscanint2(file,&yr,"year",&iseof,isout) || fscanreal(file,co2->data+co2->nyear,"co2",isout))
      {
        if(iseof)
          break;
        if(isout)
          fprintf(stderr,"ERROR129: Cannot read CO2 data in line %d of '%s'.\n",
                  co2->nyear+1,filename->name);
        free(co2->data);
        fclose(file);
        return TRUE;
      }
      if(yr!=yr_old+1)
      {
        if(isout)
          fprintf(stderr,"ERROR157: Invalid year=%d in line %d of '%s'.\n",
                  yr,co2->nyear+1,filename->name);
        free(co2->data);
        fclose(file);
        return TRUE;
      }
      co2->nyear++;
      yr_old=yr;
    }
    fclose(file);
  }
  else
    return TRUE;
  return FALSE;
} /* of 'readco2' */
