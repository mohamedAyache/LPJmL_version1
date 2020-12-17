/***************************************************************************/
/**                                                                       **/
/**                  l  p  j  s  h  u  f  f  l  e  .  c                   **/
/**                                                                       **/
/**     Program shuffles input data files to balance load in parallel     **/
/**     code.                                                             **/
/**                                                                       **/
/**     written by Werner von Bloh                                        **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: $Date:: 2015-07-22 11:01:20 +0200 (mer., 22 juil. #$ **/
/**     By         : $Author:: bloh                            $          **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"
#include "tree.h"
#include "grass.h"
#include "crop.h"
#include <sys/stat.h>

#define NTYPES 3 /* number of PFT classes */
#define dflt_filename "lpjml.conf" /* default LPJ configuration file */
#define SUFFIX "_shuffle"
#define SHUFFLE_VERSION "1.0.007"
#define USAGE "%s [-h] [-inpath dir] [[-Dmacro[=value]] [-Idir] ...] [filename]\n"

static char *insert(char *dest,const char *src,const char *ins)
{
  int pos;
  /* search for leftmost '.' */
  for(pos=strlen(src)-1;pos>=0 && src[pos]!='.';pos--);
  strncpy(dest,src,pos);
  dest[pos]='\0';
  strcat(dest,ins);
  strcat(dest,src+pos);
  return dest;
} /* of 'insert' */

static int *permute(int n)
{
  /* Function creates a random permutation of n numbers */
  int *perm,index,i,swp;
  perm=newvec(int,n);
  for(i=0;i<n;i++)
    perm[i]=i;
  for(i=0;i<n-1;i++)
  {
    index=i+(int)(drand48()*(n-i));
    swp=perm[i];
    perm[i]=perm[index];
    perm[index]=swp;
  }
  return perm;
} /* of 'permute' */

static void writedata(Filename filename,const int perm[],int n)
{
  Header header;
  String headername;
  FILE *file,*out;
  Bool swap;
  short *data;
  String line;
  int i,j,len;
  int version;
  file=fopen(filename.name,"rb");
  if(file==NULL)
  {
    fprintf(stderr,"Error opening '%s': %s\n",filename.name,strerror(errno));
    return;
  }
  version=(filename.fmt==CLM) ? READ_VERSION :2 ;
  freadanyheader(file,&header,&swap,headername,&version);
  out=fopen(insert(line,filename.name,SUFFIX),"wb");
  if(out==NULL)
  {
    fprintf(stderr,"Error creating '%s': %s\n",line,strerror(errno));
    return;
  }
  len=header.ncell;
  data=newvec(short,header.ncell*header.nbands);
  header.ncell=n;
  if(header.nyear<=0)
    header.nyear=1;
  fwriteheader(file,header,headername,version);
  for(i=0;i<header.nyear;i++)
  {
    freadshort(data,len*header.nbands,swap,file);
    for(j=0;j<header.ncell;j++)
      fwrite(data+perm[j]*header.nbands,sizeof(short),header.nbands,out);
  }
  free(data);
  fclose(out);
  fclose(file);
}

static int skipsoil(char soilcode[],int n,int nsoil,int perm[])
{
  /* Function deletes grid cells with invalid soilcode */
  int *new,count,i;
  count=0;
  new=newvec(int,n);
  for(i=0;i<n;i++)
    if(soilcode[perm[i]]>0  && soilcode[perm[i]]<=nsoil)
      new[count++]=perm[i];
  for(i=0;i<count;i++)
    perm[i]=new[i];
  free(new);
  return count;
} /* of 'skipsoil' */

int main(int argc,char **argv)
{
  String line;
  FILE *file,*out;
  Coordfile coord_file;
  Config config;
  int n,*wateruse,index,version;
  int *perm;
  Coord *coords;
  Header header;
  String headername;
  char *soil,*lakefrac;
  Bool swap;
  Fscanpftparfcn scanfcn[NTYPES]={fscanpft_grass,fscanpft_tree,fscanpft_crop};
  int i,j;
  Routing *r;
  initconfig(&config);
  if(argc>1 && !strcmp(argv[1],"-h"))
  {
    printf(USAGE,argv[0]);
    return EXIT_SUCCESS;
  }
  printf("**** %s Version %s (" __DATE__ ") ****\n",argv[0],SHUFFLE_VERSION);
  if(readconfig(&config,dflt_filename,scanfcn,NTYPES,NOUT,&argc,&argv,USAGE))
    fail(READ_CONFIG_ERR,FALSE,FALSE,"Error reading config");
  perm=permute(config.ngridcell);
  soil=newvec(char,config.ngridcell);
  file=fopen(config.soil_filename.name,"rb");
  if(file==NULL)
  {
    fprintf(stderr,"Error opening soil file '%s': %s\n",
            config.soil_filename.name,strerror(errno));
    return EXIT_FAILURE;
  }
  fseek(file,config.firstgrid,SEEK_SET);
  fread(soil,config.ngridcell,1,file);
  fclose(file);
  if(config.river_routing)
  {
    lakefrac=newvec(char,config.ngridcell);
    file=fopen(config.lakes_filename.name,"rb");
    if(file==NULL)
    {
      fprintf(stderr,"Error opening lake fraction file '%s': %s\n",
              config.lakes_filename.name,strerror(errno));
      return EXIT_FAILURE;
    }
    fseek(file,config.firstgrid,SEEK_SET);
    fread(lakefrac,config.ngridcell,1,file);
    fclose(file);
    file=fopen(insert(line,config.lakes_filename.name,SUFFIX),"wb");
    n=config.ngridcell;
    if(file==NULL)
    {
      fprintf(stderr,"Error creating lake fraction file '%s': %s\n",
              line,strerror(errno));
      return EXIT_FAILURE;
    }
    for(i=0;i<n;i++)
      fwrite(lakefrac+perm[i],1,1,file);
    fclose(file);
    free(lakefrac);
  }
  else
    n=skipsoil(soil,config.ngridcell,config.nsoil,perm);
  file=fopen(insert(line,config.soil_filename.name,SUFFIX),"wb");
  if(file==NULL)
  {
    fprintf(stderr,"Error creating soil file '%s': %s\n",
            line,strerror(errno));
    return EXIT_FAILURE;
  }
  for(i=0;i<n;i++)
    fwrite(soil+perm[i],1,1,file);
  fclose(file);
  free(soil);
  coords=newvec(Coord,config.ngridcell);
  coord_file=opencoord(&config.coord_filename);
  if(coord_file==NULL)
  {
    fprintf(stderr,"Error opening grid file '%s'.\n",
            config.coord_filename.name);
    return EXIT_FAILURE;
  }
  seekcoord(coord_file,config.firstgrid);
  for(i=0;i<config.ngridcell;i++)
    readcoord(coord_file,coords+i,config.resolution);
  closecoord(coord_file);
  header.nyear=0;
  header.firstyear=0;
  header.order=0;
  header.firstcell=0;
  header.ncell=n;
  header.nbands=2;
  header.cellsize=0.5;
  header.scalar=0.01;
  file=fopen(insert(line,config.coord_filename.name,SUFFIX),"wb");
  if(file==NULL)
  {
    fprintf(stderr,"Error creating grid file '%s': %s\n",
            line,strerror(errno));
    return EXIT_FAILURE;
  }
  fwriteheader(file,header,LPJGRID_HEADER,LPJGRID_VERSION);
  for(i=0;i<n;i++)
    writecoord(file,coords[perm[i]]);
  free(coords);
  fclose(file);
  if(config.wateruse_filename.name!=NULL)
  {
    file=fopen(config.wateruse_filename.name,"rb");
    if(file==NULL)
    {
      fprintf(stderr,"Error opening wateruse file '%s': %s\n",
              config.wateruse_filename.name,strerror(errno));
      return EXIT_FAILURE;
    }
    wateruse=newvec(int,config.ngridcell);
    version=(config.wateruse_filename.fmt==CLM) ? READ_VERSION :2 ;
    freadanyheader(file,&header,&swap,headername,&version);
    out=fopen(insert(line,config.wateruse_filename.name,SUFFIX),"wb");
    fseek(file,sizeof(int)*config.firstgrid,SEEK_CUR);
    if(file==NULL)
    {
      fprintf(stderr,"Error creating wateruse file '%s': %s\n",
              line,strerror(errno));
      return EXIT_FAILURE;
    }
    fwriteheader(out,header,LPJWATERUSE_HEADER,version);
    for(i=0;i<header.nyear;i++)
    {
      fread(wateruse,sizeof(int),config.ngridcell,file);
      if(swap)
        for(j=0;j<config.ngridcell;j++)
          wateruse[j]=swapint(wateruse[j]);
      for(j=0;j<config.ngridcell;j++)
        fwrite(wateruse+perm[j],sizeof(int),1,out);
    }
    free(wateruse);
    fclose(file);
    fclose(out);
  }
  if(config.river_routing)
  {
    if(config.withlanduse)
    {
      file=fopen(config.neighb_irrig_filename.name,"rb");
      if(file==NULL)
      {
        fprintf(stderr,"Error opening neighbour irrigation file '%s': %s\n",
                config.neighb_irrig_filename.name,strerror(errno));
        return EXIT_FAILURE;
      }
      version=(config.neighb_irrig_filename.fmt==CLM) ? READ_VERSION :2 ;
      freadanyheader(file,&header,&swap,headername,&version);
      fseek(file,sizeof(int)*config.firstgrid,SEEK_CUR);
      out=fopen(insert(line,config.neighb_irrig_filename.name,SUFFIX),"wb");
      if(file==NULL)
      {
        fprintf(stderr,"Error creating neighbour irrigation file '%s': %s\n",
                line,strerror(errno));
        return EXIT_FAILURE;
      }
      fwriteheader(out,header,headername,version);
      wateruse=newvec(int,config.ngridcell);
      fread(wateruse,sizeof(int),config.ngridcell,file);
      if(swap)
        for(i=0;i<config.ngridcell;i++)
          wateruse[i]=swapint(wateruse[i])-config.firstgrid;
      else
        for(i=0;i<config.ngridcell;i++)
          wateruse[i]-=config.firstgrid;
      for(i=0;i<config.ngridcell;i++)
      {
        if(wateruse[i]<0 || wateruse[i]>=config.ngridcell)
        {
          fprintf(stderr,"Invalid index in river routing: %d\n",wateruse[i]);
          return EXIT_FAILURE;
        }
        index=wateruse[perm[i]];
        for(j=0;j<n;j++)
          if(perm[j]==index)
            break;
        fwrite(&j,sizeof(int),1,out);
      }
      free(wateruse);
      fclose(file);
      fclose(out);
    }
    file=fopen(config.drainage_filename.name,"rb");
    if(file==NULL)
    {
      fprintf(stderr,"Error opening drainage file '%s': %s\n",
              config.drainage_filename.name,strerror(errno));
      return EXIT_FAILURE;
    }
    version=(config.drainage_filename.fmt==CLM) ? READ_VERSION :2 ;
    freadanyheader(file,&header,&swap,headername,&version);
    fseek(file,sizeof(Routing)*config.firstgrid,SEEK_CUR);
    out=fopen(insert(line,config.drainage_filename.name,SUFFIX),"wb");
    if(file==NULL)
    {
      fprintf(stderr,"Error creating drainage file '%s': %s\n",
              line,strerror(errno));
      return EXIT_FAILURE;
    }
    fwriteheader(out,header,LPJDRAIN_HEADER,version);
    r=newvec(Routing,config.ngridcell);
    for(i=0;i<config.ngridcell;i++)
    {
      getroute(file,r+i,swap);
      if(r[i].index>=0)
        r[i].index-=config.firstgrid;
      if(r[i].index<0 || r[i].index>=config.ngridcell)
      {
         fprintf(stderr,"Invalid index in river routing: %d\n",r[i].index);
         return EXIT_FAILURE;
      }
    }
    for(i=0;i<config.ngridcell;i++)
    {
      if(r[i].index>=0)
      {
        for(j=0;j<n;j++)
          if(perm[j]==r[i].index)
            break;
        r[i].index=j;
      }
    }
    for(i=0;i<config.ngridcell;i++)
      fwrite(r+perm[i],sizeof(Routing),1,out);
    free(r);
    fclose(file);
    fclose(out);
  }
  writedata(config.temp_filename,perm,n);
  writedata(config.prec_filename,perm,n);
  if(config.with_radiation)
  {
    writedata(config.lwnet_filename,perm,n);
    writedata(config.swdown_filename,perm,n);
  }
  else
    writedata(config.cloud_filename,perm,n);
  if(config.wet_filename.name!=NULL)
    writedata(config.wet_filename,perm,n);
  if(config.fire==SPITFIRE)
  {
    writedata(config.wind_filename,perm,n);
    writedata(config.tamp_filename,perm,n);
    writedata(config.popdens_filename,perm,n);
    writedata(config.lightning_filename,perm,n);
    writedata(config.human_ignition_filename,perm,n);
  }
  if(config.withlanduse)
  {
    writedata(config.landuse_filename,perm,n);
    writedata(config.countrycode_filename,perm,n);
  }
  return EXIT_SUCCESS;
} /* of 'main' */
