/***************************************************************************/
/**                                                                       **/
/**                p  r  i  n  t  r  e  s  e  r  v  o  i  r  .  c         **/
/**                                                                       **/
/**     Print contents of reservoir data files                            **/
/**                                                                       **/
/**     written by Werner von Bloh                                        **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: 23.01.2013                                           **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"

int main(int argc,char **argv)
{
  FILE *file;
  Header header;
  Reservoir reservoir;
  Bool swap;
  String headername;
  int i,j,version;
  version=READ_VERSION;
  if(argc>1 && !strcmp(argv[1],"-longheader"))
  {
    version=2;
    argc--;
    argv++;
  }
  if(argc==1)
  {
    fprintf(stderr,"Argument missing.\nUsage: printreservoir [-longheader] filename\n");
    return EXIT_FAILURE;
  }
  file=fopen(argv[1],"rb");
  if(file==NULL)
  {
    fprintf(stderr,"Error opening '%s': %s\n",argv[1],strerror(errno));
    return EXIT_FAILURE;
  }
  if(freadanyheader(file,&header,&swap,headername,&version))
  {
    fprintf(stderr,"Error reading header.\n");
    return EXIT_FAILURE;
  }
  for(i=0;i<header.ncell;i++)
  {
    if(readreservoir(&reservoir,swap,file))
    {
      fprintf(stderr,"Error reading reservoir data at cell %d\n",i);
      break;
    }
    if(reservoir.year>0)
    {
      printf("Cell:\t\t%d\n"
             "Year:\t\t%d\n"
             "Capacity:\t%g\n"
             "Area:\t\t%g\n"
             "Inst cap.:\t%d\n"
             "Height:\t\t%d\n"
             "Purpose:\t",
             i,reservoir.year,reservoir.capacity,reservoir.area,reservoir.inst_cap,
             reservoir.height);
      for(j=0;j<NPURPOSE;j++)
        printf("%d ",reservoir.purpose[j]);
      printf("\n");
    }
  }
  fclose(file);
  return EXIT_SUCCESS;
} /* of 'main' */
