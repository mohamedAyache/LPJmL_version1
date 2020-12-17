/***************************************************************************/
/**                                                                       **/
/**                     f  r  e  a  d  h  e  a  d  e  r  .  c             **/
/**                                                                       **/
/**     Reading file header for LPJ related files. Detects                **/
/**     whether byte order has to be changed                              **/
/**                                                                       **/
/**     written by Werner von Bloh                                        **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change:  22.01.2013                                          **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"

Bool freadheader(FILE *file, /* file pointer of binary file */
                 Header *header, /* file header to be read */
                 Bool *swap,     /* set to TRUE if data is in different order */
                 const char *headername,
                 int *version    /* returns version of CLM file, has to be set
                                    to READ_VERSION or prescibed value  */
                ) /* returns TRUE on error */
{
  char *buffer;
  int file_version,*ptr;
  buffer=newvec(char,strlen(headername)+1);
  if(buffer==NULL)
    return TRUE;
  if(fread(buffer,strlen(headername),1,file)!=1)
  {
    free(buffer);
    return TRUE;
  }
  /* set ending '\0' in string */
  buffer[strlen(headername)]='\0';
  if(strcmp(buffer,headername))
  {
    free(buffer);
    return TRUE;
  }
  free(buffer);
  if(fread(&file_version,sizeof(file_version),1,file)!=1)
    return TRUE;
  if((file_version & 0xff)==0)
  {
    /* byte order has to be changed in file */
    *swap=TRUE;
    file_version=swapint(file_version);
  }
  else
    *swap=FALSE;
  if(*version==READ_VERSION)
    *version=file_version;
  if(*version==1) /* old header? */
  {
    if(fread(header,sizeof(Header_old),1,file)!=1)
      return TRUE;
    /* set default values for cell size and scalar */
    header->cellsize=0.5;
    header->scalar=1;
  }
  else if(fread(header,sizeof(Header),1,file)!=1)
    return TRUE;
  if(*swap)  /* is data in different byte order? */
  {
    /* yes, swap bytes */
    header->order=swapint(header->order);
    header->firstyear=swapint(header->firstyear);
    header->nyear=swapint(header->nyear);
    header->firstcell=swapint(header->firstcell);
    header->ncell=swapint(header->ncell);
    header->nbands=swapint(header->nbands);
    if(*version>1)
    {
      ptr=(int *)&header->cellsize;
      header->cellsize=swapfloat(*ptr);
      ptr=(int *)&header->scalar;
      header->scalar=swapfloat(*ptr);
    }
 }
 return FALSE;
} /* of 'freadheader' */
