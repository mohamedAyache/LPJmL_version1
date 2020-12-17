/***************************************************************************/
/**                                                                       **/
/**                  u  p  d  a  t  e  b  u  i  l  d  .  c                **/
/**                                                                       **/
/**     Program updates build number in header file                       **/
/**                                                                       **/
/**     Usage: updatebuild headerfile                                     **/
/**                                                                       **/
/**     written by Werner von Bloh                                        **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: 06.10.2011                                           **/
/**                                                                       **/
/***************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#ifdef _WIN32
#define unlink _unlink
#else
#include <unistd.h>
#endif

#define LEN 256        /* maximum line length+1 */
#define SUFFIX ".bak"

int main(int argc,char **argv)
{
  FILE *file,*new;
  char *filename;
  char line[LEN];
  int num;
  if(argc==1)
  {
    fprintf(stderr,"Argument missing.\n"
            "Usage: %s headerfile\n",argv[0]);
    return EXIT_FAILURE;
  }
  filename=malloc(strlen(argv[1])+strlen(SUFFIX)+1);
  if(filename==NULL)
  {
    fputs("Error allocating memory.\n",stderr);
    return EXIT_FAILURE;
  }
  strcat(strcpy(filename,argv[1]),SUFFIX);
  /* Delete old header file */
  unlink(filename);
  /* rename header file */
  if(rename(argv[1],filename))
  {
    fprintf(stderr,"Error renaming '%s': %s.\n",argv[1],strerror(errno));
    return EXIT_FAILURE;
  }
  /* open renamed header file */
  file=fopen(filename,"r");
  if(file==NULL)
  {
    fprintf(stderr,"Error opening '%s': %s.\n",filename,strerror(errno));
    return EXIT_FAILURE;
  }
  free(filename);
  /* create new header file */
  new=fopen(argv[1],"w"); 
  if(new==NULL)
  {
    fprintf(stderr,"Error creating '%s': %s.\n",argv[1],strerror(errno));
    fclose(file);
    return EXIT_FAILURE;
  }
  while(fgets(line,LEN,file)!=NULL)
    if(sscanf(line,"#define BUILDNUMBER %d",&num)==1)
    {
      /* build number read, write incremented build number in new headerfile */
      fprintf(new,"#define BUILDNUMBER %d\n",num+1);
      printf("Build number %d\n",num+1);
      fclose(new);
      fclose(file);
      return EXIT_SUCCESS;
    }
    else
      fputs(line,new); /* copy line to new headerfile */
  /* no build number found, exit with error */
  fputs("Error: Cannot read build number.\n",stderr);
  fclose(file);
  fclose(new);
  return EXIT_FAILURE;
} /* of 'main' */
