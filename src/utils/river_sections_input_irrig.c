/* returns irrigation input-data */
/* search for water in one of the 8 neighboring pixel as follows: 
   choose neighbor pixel with the most water input from other pixels */

#include "lpj.h"

#define NCOLS 720
#define NROWS 360
#define WIDTH 50
#define XCORNER -18000
#define YCORNER 9000
#define SWAP TRUE

static void sort(int feld0[],int feld1[],int feld2[],int len){
  int i,j,min,tmp;

  for(i=0;i<len-1;i++){
    min=i;
    for(j=i+1;j<len;j++)
      if(feld0[j]<feld0[min]) 
        min=j;
    
    tmp=feld0[min];
    feld0[min]=feld0[i];
    feld0[i]=tmp;

    tmp=feld1[min];
    feld1[min]=feld1[i];
    feld1[i]=tmp;

    tmp=feld2[min];
    feld2[min]=feld2[i];
    feld2[i]=tmp;    
  }
}

int main(int argc,char **argv)
{  
  FILE *ifp, *ifp_nr;
  FILE *mfp;
  FILE *ofp;
  Header header,header_grid;
  int ilat,ilon,ilat_n[8],ilon_n[8];
  int i,j,k,ncell,cell;
  int swap_grid;
  int neighb[8];
  short rbuf;
  short river[NROWS][NCOLS];
  int cell_n;
  int *lon,*lat,lat_n,lon_n,count,grid_version;
  grid_version=READ_VERSION; 
  if(argc>1 && !strcmp(argv[1],"-longheader"))
  {
    grid_version=2;
    argc--;
    argv++;
  }
  header.nyear=0;
  header.firstyear=0;
  header.order=0;
  header.firstcell=0;
  header.nbands=1;
  header.cellsize=0.5;
  header.scalar=1; 
/* Parse command line */
/* z.B. "river_sections_input_irrig re-ordered_grid_file numbered_grid_file vec_drain_river.bip outfile" */
  if(argc!=5){
    fprintf(stdout,"USAGE: river_sections_input_irrig [-longheader] re-ordered_grid numbered_grid_file vec_drain_river.bip outfile\n");
    exit(99);
  }

  /* Open in- & output file */
  if((ifp=fopen(argv[1],"rb")) == NULL){
    fprintf(stderr,"Warning: File open failed on re-ordered grid file \n");
    exit(1);
  }

  if((ifp_nr=fopen(argv[2],"r")) == NULL){
    fprintf(stderr,"Warning: File open failed on numbered grid file \n");
    exit(1);
  }

  if((mfp=fopen(argv[3],"rb")) == NULL){
    fprintf(stderr,"Warning: File open failed on mask-file \n");
    exit(1);
  }   

  if((ofp=fopen(argv[4],"wb")) == NULL){
    fprintf(stderr,"Warning: File open failed on output file \n");
    exit(1);
  }

  for(i=0;i<NROWS;i++){
    for(j=0;j<NCOLS;j++){
      fread(&rbuf,sizeof(short),1,mfp);
      if(SWAP) rbuf=swapshort(rbuf);
      river[i][j]=rbuf;
    }
    /* printf("i:%d %d\n",i,river[i][180]); */
  }
  fclose(mfp);

  /* HEADER */
  if(freadheader(ifp,&header_grid,&swap_grid,LPJGRID_HEADER,&grid_version)){
    fclose(ifp);
    fail(23,FALSE,"Invalid header in re-ordered grid file.");
  }

  header.ncell=header_grid.ncell;

  /* write header */
  fwriteheader(ofp,header,LPJNEIGHB_IRRIG_HEADER,LPJNEIGHB_IRRIG_VERSION);

  count=0;
  while(fscanf(ifp_nr,"%d",&ncell)==1){
    lat=newvec(int,ncell);
    lon=newvec(int,ncell);

    for(cell=0;cell<ncell;cell++){
      if(fread(&rbuf,sizeof(short),1,ifp)==1){
	if(swap_grid) rbuf=swapshort(rbuf);
	lon[cell]=rbuf;
	fread(&rbuf,sizeof(short),1,ifp);
	if(swap_grid) rbuf=swapshort(rbuf);
	lat[cell]=rbuf;
	/* printf("c:%d %d %d\n",cell,lon[cell],lat[cell]); */
      }
      else{
	fprintf(stderr,"Error reading grid.\n");
	exit(1);
      }
    }

    for(cell=0;cell<ncell;cell++){
      /* determine ilatitude and ilongitude of current position */
      ilat=(YCORNER+25-lat[cell])/WIDTH-1;
      ilon=(lon[cell]-XCORNER+25)/WIDTH-1;
      /* printf("A c:%d lon:%d %d lat:%d %d\n",cell,ilon,lon[cell],ilat,lat[cell]); */

      for(j=0;j<8;j++){ 
	neighb[j]=0;
	ilat_n[j]=ilat;
	ilon_n[j]=ilon;
      }

      if((ilat-1)>=0 && (ilon-1)>=0 && river[ilat-1][ilon-1]>0){
	neighb[0]=river[ilat-1][ilon-1];
	ilat_n[0]=ilat-1;
	ilon_n[0]=ilon-1;
      }

      if((ilat-1)>=0 && river[ilat-1][ilon]>0){
	neighb[1]=river[ilat-1][ilon];
	ilat_n[1]=ilat-1;
	ilon_n[1]=ilon;
      }

      if((ilat-1)>=0 && (ilon+1)<NCOLS && river[ilat-1][ilon+1]>0){
	neighb[2]=river[ilat-1][ilon+1];
	ilat_n[2]=ilat-1;
	ilon_n[2]=ilon+1;
      }
      
      if((ilon-1)>=0 && river[ilat][ilon-1]>0){
	neighb[3]=river[ilat][ilon-1];
	ilat_n[3]=ilat;
	ilon_n[3]=ilon-1;
      }

      if((ilon+1)<NCOLS && river[ilat][ilon+1]>0){
	neighb[4]=river[ilat][ilon+1];
	ilat_n[4]=ilat;
	ilon_n[4]=ilon+1;
      }
      
      if((ilat+1)<NROWS && (ilon-1)>=0 && river[ilat+1][ilon-1]>0){
	neighb[5]=river[ilat+1][ilon-1];
	ilat_n[5]=ilat+1;
	ilon_n[5]=ilon-1;
      }

      if((ilat+1)<NROWS && river[ilat+1][ilon]>0){
	neighb[6]=river[ilat+1][ilon];
	ilat_n[6]=ilat+1;
	ilon_n[6]=ilon;
      }

      if((ilat+1)<NROWS && (ilon+1)<NCOLS && river[ilat+1][ilon+1]>0){
	neighb[7]=river[ilat+1][ilon+1];
	ilat_n[7]=ilat+1;
	ilon_n[7]=ilon+1;
      }
      
      sort(neighb,ilat_n,ilon_n,8);

      cell_n=-9;
      for(j=7;j>=0;j--){
	lat_n=YCORNER+25-WIDTH*(ilat_n[j]+1);
	lon_n=XCORNER-25+WIDTH*(ilon_n[j]+1);
	/* printf("C c:%d lon:%d %d lat:%d %d\n",cell,ilon_n[j],lon_n,ilat_n[j],lat_n); */
	for(k=0;k<ncell;k++)
	  if(lat_n==lat[k] && lon_n==lon[k]){ 
	    cell_n=k;
	    break;
	  }
	if(cell_n>=0) break;
      }

      /* printf("c:%d c_n:%d\n",cell,cell_n); */
      if(cell_n==-9) cell_n=cell;
      cell_n+=count;

      fwrite(&cell_n,sizeof(int),1,ofp); 
    } /* of cell loop */
    free(lat);
    free(lon);
    count+=ncell;
  } /* end of while*/
  printf("processed %d cells\n",count);
  fflush(stdout);
  fclose(ifp);
  fclose(ifp_nr);
  fclose(ofp);

  exit(0);
}
