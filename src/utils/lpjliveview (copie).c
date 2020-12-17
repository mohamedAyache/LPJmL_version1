/***************************************************************************/
/**                                                                       **/
/**                l  p  j  l  i  v  e  v  i  e  w  .  c                  **/
/**                                                                       **/
/**     Live view for LPjmL Version 3.5.003 runs using socket connection  **/
/**                                                                       **/
/**     written by Werner von Bloh                                        **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: $Date:: 2015-12-02 12:16:26 +0100 (mer., 02 déc. #$ **/
/**     By         : $Author:: bloh                            $          **/
/**                                                                       **/
/***************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <X11/Xos.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/IntrinsicP.h>
#define Bool BOOL /* Bool already defined in X11 */
#define String STRING /* same for String */
#include "lpj.h"
#include "tree.h"
#include "crop.h"
#include "grass.h"

#define NTYPES 3
#define xsize 720
#define ysize 360
#define LPJDISPLAY_VERSION "0.9.005"
#define USAGE "Usage: %s [-param] [-output socket=:port] [-outpath dir] [-inpath dir] [[-Dmacro[=value]] [-Idir] ...] [filename [-display disp] [-step -n] [-scale min max] [variable]]\n"
#define dflt_filename "lpjml.conf"

typedef struct
{
  unsigned char r,g,b;
} RGBpixel;

typedef RGBpixel Image[ysize*xsize];

typedef enum {GR_RGBMAP=1,GR_RBMAP,GR_RMAP,GR_BMAP,GR_GMAP,GR_HSVMAP,
              GR_GRAYMAP,GR_GRAY2MAP} GR_MAPTYPE;

#define low 0.0

static float colstep=0;

static void gr_val2rgb(float *r,float *g,float *b,float value,GR_MAPTYPE type)
{
  value=1-value;
  if(value<0)
   value=0;
  if(value>1)
   value=1;
  if(colstep)
    value=floor(value*colstep)/colstep;
  switch(type)
  {
    case GR_RGBMAP:
      if(value<1.0/3.)
      {
        *r=1;
        *g=low+value*3.0*(1-low);
        *b=low;
      }
      else if(value<2.0/3.0)
      {
        *r=1-(value*3-1)*(1-low);
        *g=1;
        *b=low+(1-low)*(value*3-1);
      }
      else
      {
        *r=low;
        *g=1-(value*3-2)*(1-low);
        *b=1;
      }
      break;
    case GR_RBMAP:
      if(value<0.5)
      {
        *r=1-(0.5-value)/0.5*0.5;
        *g=*b=value*2;
      }
      else
      {
        *b=1-(value-0.5)/0.5*0.5;
        *r=*g=1-(value-0.5)*2;
      }
      break;
    case GR_RMAP:
      *r=1-(1-value)*0.5;
      *g=*b=value;
      break;
    case GR_BMAP:
      *b=1-(1-value)*0.5;
      *r=*g=value;
      break;
    case GR_GMAP:
      *g=1-(1-value)*0.5;
      *r=*b=value;
      break;
    case GR_GRAYMAP:
      *r=*g=*b=value;
      break;
    case GR_GRAY2MAP:
      *r=*g=*b=1-value;
      break;
  }
} /* of 'gr_val2rgb' */

static void gr_colstep(int n)
{
  colstep=n;
} /* of 'gr_colstep' */

static int getmax(const float *const *array,int x,int y)
{
  float max;
  int i,index;
  max=array[0][x];
  index=0;
  for(i=1;i<y;i++)
    if(max<array[i][x])
    {
      max=array[i][x];
      index=i;
    }
  return index;
} /* of 'getmax' */

static int get_byte_order(void) 
{
  union 
  {
    char c[sizeof(short)];
    short s;
  } order;

  order.s = 1;
  if ((1 == order.c[0])) 
    return LSBFirst;
  else 
    return MSBFirst;
} /* of 'get_byte_order' */

static void outimage(Display *dis, Window mainW,int screen, GC theGC,
                     RGBpixel *image,int width, int height) 
{
  int depth;
  XImage *img = NULL;
  Visual *vis;
  double rRatio;
  double gRatio;
  double bRatio;
  unsigned int r, g, b;
  u_int32_t *newBuf32;
  int i;
  int numBufBytes = width * height;
  size_t numNewBufBytes;
  u_int16_t *newBuf16;

  depth = DefaultDepth (dis, screen);
  vis = DefaultVisual (dis, screen);
  
  rRatio = vis->red_mask / 255.0;
  gRatio = vis->green_mask / 255.0;
  bRatio = vis->blue_mask / 255.0;

  if (depth >= 24) 
  {
    numNewBufBytes = (4 * (width * height));
    newBuf32 = malloc (numNewBufBytes);
  
    for (i = 0; i < numBufBytes; ++i) 
    {
      r = (unsigned int)(image[i].r * rRatio);
      g = (unsigned int)(image[i].g * gRatio);
      b = (unsigned int)(image[i].b * bRatio);

      r &= vis->red_mask;
      g &= vis->green_mask;
      b &= vis->blue_mask;

      newBuf32[i] = r | g | b;
    }

    img = XCreateImage (dis, 
                        CopyFromParent, depth, 
		        ZPixmap, 0, 
                        (char *) newBuf32,
                        width, height,
                        32, 0);

  } 
  else if (depth >= 15) 
  {
    numNewBufBytes = (2 * (width * height));
    newBuf16 = malloc (numNewBufBytes);

    for (i = 0; i < numBufBytes; ++i)
    {

      r = (unsigned int)(image[i].r * rRatio);
      g = (unsigned int)(image[i].g * gRatio);
      b = (unsigned int)(image[i].b * bRatio);

      r &= vis->red_mask;
      g &= vis->green_mask;
      b &= vis->blue_mask;

      newBuf16[i] = r | g | b;
    }

    img = XCreateImage (dis,
                        CopyFromParent, depth,
			ZPixmap, 0,
			(char *) newBuf16,
			width, height,
			16, 0);
  } 
  else 
  {
    fprintf (stderr, "This program does not support displays with a depth less than 15.");
    return;				
  }

  XInitImage (img);
  /*Set the client's byte order, so that XPutImage knows what to do with the data.*/
  /*The default in a new X image is the server's format, which may not be what we want.*/
  if ((LSBFirst == get_byte_order ())) 
    img->byte_order = LSBFirst;
  else 
    img->byte_order = MSBFirst;
	
  /*The bitmap_bit_order doesn't matter with ZPixmap images.*/
  img->bitmap_bit_order = MSBFirst;

  XPutImage(dis,mainW,theGC,img,0,0,0,0,width,height);
  XDestroyImage(img);
} /* of 'outimage' */

static void draw(Display *disp, Window mainW,int screen,GC theGC,
                 Image image,int year)
{
  char line[256];
  sprintf(line,"Year: %d",year);
  outimage(disp,mainW,screen,theGC,image,xsize, ysize); 
  XDrawImageString(disp,mainW,theGC,10,360+24,line,strlen(line));
  XFlush(disp);
} /* of 'draw' */

static void drawmonth(Display *disp, Window mainW,int screen,GC theGC,
                 Image image,int year,int month)
{
  char line[256];
  sprintf(line,"%s %d",months[month],year);
  outimage(disp,mainW,screen,theGC,image,xsize, ysize); 
  XDrawImageString(disp,mainW,theGC,10,360+24,line,strlen(line));
  XFlush(disp);
} /* of 'drawmonth' */

static void drawday(Display *disp, Window mainW,int screen,GC theGC,
                 Image image,int year,int day)
{
  char line[256];
  int dayofmonth,month;
  cvrtdaymonth(&dayofmonth,&month,day+1);
  sprintf(line,"%2d %s %d",dayofmonth,months[month-1],year);
  outimage(disp,mainW,screen,theGC,image,xsize, ysize);
  XDrawImageString(disp,mainW,theGC,10,360+24,line,strlen(line));
  XFlush(disp);
} /* of 'drawmonth' */

static void eventloop(Display *disp,Window mainW,int screen,GC theGC,
                      Image *images,const Config *config)
{
  XEvent event;
  int year;
  XExposeEvent *exp_event;
  while(TRUE)
  {
     XNextEvent(disp,&event);
     switch(event.type)
     {
       case KeyPress:
         if(event.xkey.window==mainW)
           return;
         break;
       case Expose:
         exp_event = (XExposeEvent *) &event;
         if (exp_event->count>0 && exp_event->window!=mainW )
           break;
         drawday(disp,mainW,screen,theGC,
                 images[0],config->lastyear,NDAYYEAR-1);
         break;
     }
  }
} /* of 'loopyear' */

static void loopyear(Display *disp,Window mainW,int screen,GC theGC,
                      Image *images,const Config *config)
{
  XEvent event;
  int year;
  XExposeEvent *exp_event;
  while(TRUE)
    for(year=config->firstyear;year<=config->lastyear;year++)
    {
      if(XPending(disp))
      {
        XNextEvent(disp,&event);
        switch(event.type)
        {
          case KeyPress:
            if(event.xkey.window==mainW)
              return;
            break;
          case Expose:
             exp_event = (XExposeEvent *) &event;
             if (exp_event->count>0 && exp_event->window!=mainW )
               break;
             draw(disp,mainW,screen,theGC,images[year-config->firstyear],year); 
             break;
         }
       }
       draw(disp,mainW,screen,theGC,images[year-config->firstyear],year); 
    }
} /* of 'loopyear' */

static void loopmonth(Display *disp,Window mainW,int screen,GC theGC,
                      Image *images,const Config *config)
{
  XEvent event;
  int year,month;
  XExposeEvent *exp_event;
  while(TRUE)
    for(year=config->firstyear;year<=config->lastyear;year++)
    for(month=0;month<12;month++)
    {
      if(XPending(disp))
      {
        XNextEvent(disp,&event);
        switch(event.type)
        {
          case KeyPress:
            if(event.xkey.window==mainW)
              return;
            break;
          case Expose:
             exp_event = (XExposeEvent *) &event;
             if (exp_event->count>0 && exp_event->window!=mainW )
               break;
             drawmonth(disp,mainW,screen,theGC,images[(year-config->firstyear)*12+month],year,month);
             break;
         }
       }
       drawmonth(disp,mainW,screen,theGC,images[(year-config->firstyear)*12+month],year,month);
    }
} /* of 'loopmonth' */

static int findvar(const char *s,const Variable variable[],int size)
{
  int i;
  if(s!=NULL && variable!=NULL)
  {
    for(i=0;i<size;i++)
      if(!strcmp(s,variable[i].name))
        return i;
  }
  return NOT_FOUND;
} /* of 'findvar' */

int main(int argc,char **argv)
{
  Socket *socket;
  float **data;
  struct
  {
    FILE *file;
    int count;
  } files[NOUT];
  int total,year,index,i,j,k,x,y,count_max,*coord_index,out_index,sub,cstep;
  char id[3];
  Intcoord *coords;
  Bool isgrid,iscountry,isregion,isscale;
  short *svec,**sdata;
  char *display=NULL;
  XSizeHints hints;
  Config config;
  Display *theDisp;
  Window mainW,rootW;
  Font font;
  GC theGC;
  float r,g,b,scale_min,scale_max;
  int theScreen,nmonth,month,nday,day,dayofmonth;
  Image *images;
  RGBpixel back={200,200,255},white={255,255,255};
  String line;
  const char *progname,*client;
  char *title[3];
  time_t start,end;
  Flux flux;
  FILE *file;
  Header header;
  Bool swap;
  int size,version;
  Fscanpftparfcn scanfcn[NTYPES]={fscanpft_grass,fscanpft_tree,fscanpft_crop};
  out_index=VEGC;
  progname=strippath(argv[0]);
  if(argc>1 && !strcmp(argv[1],"-h"))
  {
    printf(USAGE,progname);
    return EXIT_SUCCESS;
  }
  initconfig(&config);
  cstep=0;
  isscale=FALSE;
  snprintf(line,STRING_LEN,                                               
           "%s Version %s (" __DATE__ ")",progname,LPJDISPLAY_VERSION);
  title[0]=line;
  title[1]="Displaying output data for LPJmL Version " LPJ_VERSION;
  title[2]="(c)2008, 2009 PIK Potsdam";
  banner(title,3,78);
  /* Get the configuration */
  if(readconfig(&config,dflt_filename,scanfcn,NTYPES,NOUT,&argc,&argv,USAGE))
    fail(READ_CONFIG_ERR,FALSE,"Error opening config",0);
  for(i=0;i<argc;i++)
    if(argv[i][0]=='-')
    {
      if(!strcmp(argv[i],"-display"))
      {
        if(i==argc-1)
        {
          fprintf(stderr,"Argument missing for option '-display'\n");
          return EXIT_FAILURE;
        }
        display=argv[++i];
      }
      else if(!strcmp(argv[i],"-scale"))
      {
        if(i>=argc-2)
        {
          fprintf(stderr,"Number missing after '-scale' option.\n");
          return EXIT_FAILURE;
        }
        isscale=TRUE;
        scale_min=atof(argv[++i]);
        scale_max=atof(argv[++i]);
      }
      else if(!strcmp(argv[i],"-step"))
      {
        if(i==argc-1)
        {
          fprintf(stderr,"Argument missing for option '-step'\n");
          return EXIT_FAILURE;
        }
        cstep=atoi(argv[++i]);
      }
      else
      {
        fprintf(stderr,"Invalid option '%s'.\n",argv[i]);
        return EXIT_FAILURE;
      }
    }
    else
      break;
  if(i<argc)
  {
    out_index=findvar(argv[i],config.outnames,NOUT);
    if(out_index==NOT_FOUND)
    {
      fprintf(stderr,"Invalid output variable '%s'.\n",argv[i]);
      return EXIT_FAILURE;
    }
  }
  printf("Listening for connections...\n");
  socket=open_socket(config.port,0);
  if(socket==NULL)
  {
    fprintf(stderr,"Error opening communication channel at port %d: %s\n",
            config.port,strerror(errno));
    return EXIT_FAILURE;
  }
  read_socket(socket,id,3);
  if(strncmp(id,"LPJ",3))
  {
    fprintf(stderr,"Invalid id opening communication channel\n");
    return EXIT_FAILURE;
  }
  client=getclientname(socket);
  printf("Connected to host %s\n",(client==NULL) ? "unknown" : client);
  /* Establish the connection */
  if(ismonthlyoutput(out_index))
    images=newvec(Image,(config.lastyear-config.firstyear+1)*12);
  else if(isdailyoutput(out_index))
    /*images=newvec(Image,(config.lastyear-config.firstyear+1)*NDAYYEAR);*/
    images=newvec(Image,1);

  else
    images=newvec(Image,config.lastyear-config.firstyear+1);
  printconfig(&config,config.npft[GRASS]+config.npft[TREE],
              config.npft[CROP]);
  readint_socket(socket,&total,1);
  if(total<1 || total>config.ngridcell)
  {
    fprintf(stderr,"Invalid number of cells=%d\n",total);
    return EXIT_FAILURE;
  }
  isgrid=iscountry=isregion=FALSE;
  for(i=0;i<NOUT;i++)
    files[i].file=NULL;
  count_max=0;
  sub=nmonth=nday=0;
  for(i=0;i<config.n_out;i++)
  {
    switch(config.outputvars[i].id)
    {
      case GRID:
        isgrid=TRUE;
        sub++;
        break;
      case REGION:
        isregion=TRUE;
        sub++;
        break;
      case COUNTRY:
        iscountry=TRUE;
        sub++;
        break;
      default:
        if(ismonthlyoutput(config.outputvars[i].id))
        {
          nmonth++;
          sub++;
        }
        else if(isdailyoutput(config.outputvars[i].id))
        {
          nday++;
          sub++;
        }
        break;
    }
    files[config.outputvars[i].id].file=fopen(config.outputvars[i].filename.name,"wb");
    if(files[config.outputvars[i].id].file==NULL)
    {
      fprintf(stderr,"Error creating file '%s' :%s\n",
              config.outputvars[i].filename.name,
              strerror(errno));
    }
    files[config.outputvars[i].id].count=outputsize(config.outputvars[i].id,config.npft[TREE]+config.npft[GRASS],config.nbiomass,config.nagtree,config.npft[CROP]);
    if(files[config.outputvars[i].id].count>count_max)
      count_max=files[config.outputvars[i].id].count;
  }
  if(out_index==ADISCHARGE)
  {
    file=fopen(config.coord_filename.name,"rb");
    if(file==NULL) 
    {
      fprintf(stderr,"Error opening '%s': %s\n",config.coord_filename.name,
              strerror(errno));
      return EXIT_FAILURE;
    }
    version=(config.coord_filename.fmt==CLM) ? READ_VERSION : 2;
    if(freadheader(file,&header,&swap,LPJGRID_HEADER,&version))
    {
      fprintf(stderr,"Invalid header in '%s'.\n",config.coord_filename.name);
      return EXIT_FAILURE;
    }
    coords=newvec(Intcoord,config.ngridcell);
    for(i=0;i<config.ngridcell;i++)
      readintcoord(file,coords+i,swap);
    fclose(file);
    size=config.ngridcell;
  }
  else
  {
    if(!isgrid)
    {
      fprintf(stderr,"Grid output is missing.\n");
      return EXIT_FAILURE;
    }
    coords=newvec(Intcoord,total);
    coord_index=newvec(int,total);
    readshort_socket(socket,(short *)coords,2*total);
    if(files[GRID].file!=NULL)
      fwrite(coords,sizeof(Intcoord),total,files[GRID].file);
    size=total;
  }
  for(i=0;i<size;i++)
  {
    x=(int)((180+coords[i].lon*0.01)*2);
    y=ysize-1-(int)((90+(float)coords[i].lat*0.01)*2);
    if(x<0 || x>=xsize)
    {
      printf("x=%d\n",x);
      x=0;
    }
    if(y<0 || y>=ysize)
    {
      printf("y=%d\n",y);
      y=0;
    }
    coord_index[i]=x+y*xsize;
  }
  free(coords);
  if(iscountry)
  {
    svec=newvec(short,total);
    readshort_socket(socket,svec,total);
    if(files[COUNTRY].file!=NULL)
      fwrite(svec,sizeof(short),total,files[COUNTRY].file);
    free(svec);
  }  
  if(isregion)
  {
    svec=newvec(short,total);
    readshort_socket(socket,svec,total);
    if(files[REGION].file!=NULL)
      fwrite(svec,sizeof(short),total,files[REGION].file);
    free(svec);
  }  
  if(files[SDATE].file!=NULL)
  {
    sdata=newvec(short *,files[SDATE].count);
    for(i=0;i<files[SDATE].count;i++)
      sdata[i]=newvec(short,total);
  }
  data=newvec(float *,count_max);
  for(i=0;i<count_max;i++)
    data[i]=newvec(float,config.ngridcell);
  if((theDisp=XOpenDisplay(display))==NULL)
  {
    fprintf(stderr,"Error opening display '%s'.\n",
            (display==NULL) ? ":0.0" : display);
    return 1;
  }
  theScreen=DefaultScreen(theDisp);
  theGC=DefaultGC(theDisp,theScreen);
  rootW=RootWindow(theDisp,theScreen);
  hints.flags=0;
  mainW=XCreateSimpleWindow(theDisp,rootW,100,100,720,360+24,0,
                            BlackPixel(theDisp,theScreen),
                            WhitePixel(theDisp,theScreen));
  XSetStandardProperties(theDisp,mainW,config.sim_name,config.sim_name,
                         (Pixmap)NULL,NULL,0,&hints);
  XSelectInput(theDisp, mainW,ButtonPressMask | ButtonReleaseMask |ButtonMotionMask |PointerMotionHintMask | ExposureMask |KeyPressMask);
  XMapWindow(theDisp,mainW);
  //font=XLoadFont(theDisp,"fixed");
  //XSetFont(theDisp,theGC,font);
  time(&start);
  gr_colstep(cstep);
  for(year=config.firstyear;year<=config.lastyear;year++)
  {
    day=0;
    for(month=0;month<12;month++)
    {
      for(dayofmonth=0;dayofmonth<ndaymonth[month];dayofmonth++)
      {
        for(j=0;j<nday;j++)
        {
          if(readint_socket(socket,&index,1))
          {
            fprintf(stderr,"Error reading index of output\n");
            return EXIT_FAILURE;
          }
          if(index<0 || index>=NOUT)
          {
            fprintf(stderr,"Invalid index of output\n");
            return EXIT_FAILURE;
          }
          readfloat_socket(socket,data[0],total);
          if(files[index].file!=NULL)
            fwrite(data[0],sizeof(float),total,files[index].file);
          if(!isscale)
          {
            scale_min=0;
            scale_max=0.25e3;
          }
          if(index==out_index)
          {
            for(k=0;k<ysize*xsize;k++)
              images[(year-config.firstyear)*NDAYYEAR+day][k]=back;
            for(k=0;k<total;k++)
            {
              gr_val2rgb(&r,&g,&b,(data[0][k]-scale_min)/(scale_max-scale_min),GR_GMAP);
              images[0][coord_index[k]].r=(unsigned char)(r*255);
              images[0][coord_index[k]].g=(unsigned char)(g*255);
              images[0][coord_index[k]].b=(unsigned char)(b*255);
            }
            drawday(theDisp,mainW,theScreen,theGC,
                      images[0],year,day);
          }
          day++;
        }
      }
      for(j=0;j<nmonth;j++)
      {
        if(readint_socket(socket,&index,1))
        {
          fprintf(stderr,"Error reading index of output\n");
          return EXIT_FAILURE;
        }
        if(index<0 || index>=NOUT)
        {
          fprintf(stderr,"Invalid index of output\n");
          return EXIT_FAILURE;
        }
        readfloat_socket(socket,data[0],total);
        if(files[index].file!=NULL)
          fwrite(data[0],sizeof(float),total,files[index].file);
        if(index==out_index)
        {
          for(k=0;k<ysize*xsize;k++)
            images[(year-config.firstyear)*12+month][k]=back;
          if(!isscale)
          {
            scale_min=0;
            scale_max=0.25e3;
          }
          for(k=0;k<total;k++)
          {
            gr_val2rgb(&r,&g,&b,(data[0][k]-scale_min)/(scale_max-scale_min),GR_GMAP);
            images[(year-config.firstyear)*12+month][coord_index[k]].r=(unsigned char)(r*255);
            images[(year-config.firstyear)*12+month][coord_index[k]].g=(unsigned char)(g*255);
            images[(year-config.firstyear)*12+month][coord_index[k]].b=(unsigned char)(b*255);
          }
          drawmonth(theDisp,mainW,theScreen,theGC,
                    images[(year-config.firstyear)*12+month],year,month); 
        }
      }
    }
    for(i=0;i<config.n_out-sub;i++)
    {
      if(readint_socket(socket,&index,1))
      {
        fprintf(stderr,"Error reading index of output\n");
        return EXIT_FAILURE;
      }
      if(index<0 || index>=NOUT)
      {
        fprintf(stderr,"Invalid index of output\n");
        return EXIT_FAILURE;
      }
      if(index==SDATE)
        for(j=0;j<files[index].count;j++)
        {
          readshort_socket(socket,sdata[j],total);
          if(files[index].file!=NULL)
            fwrite(sdata[j],sizeof(short),total,files[index].file);
        }
      else
        for(j=0;j<files[index].count;j++)
        {
          readfloat_socket(socket,data[j],(index==ADISCHARGE) ? config.ngridcell : total);
          if(files[index].file!=NULL)
            fwrite(data[j],sizeof(float),(index==ADISCHARGE) ? config.ngridcell : total,files[index].file);
        }
      if(index==out_index)
      {
        for(j=0;j<ysize*xsize;j++)
          images[year-config.firstyear][j]=back;
        switch(index)
        {
          case VEGC:
            for(j=0;j<total;j++)
            {
              gr_val2rgb(&r,&g,&b,data[0][j]/25e3,GR_GMAP);
              images[year-config.firstyear][coord_index[j]].r=(unsigned char)(r*255);
              images[year-config.firstyear][coord_index[j]].g=(unsigned char)(g*255);
              images[year-config.firstyear][coord_index[j]].b=(unsigned char)(b*255);
            }
            break;
          case FPC:
            for(j=0;j<total;j++)
            {
              index=getmax(data+1,j,config.npft[GRASS]+config.npft[TREE]-config.nbiomass);
              if(data[index][j]<0.001)
                images[year-config.firstyear][coord_index[j]]=white;
              else
              {
                gr_val2rgb(&r,&g,&b,
                           (float)index/(config.npft[GRASS]+config.npft[TREE]),
                            GR_RGBMAP);
                images[year-config.firstyear][coord_index[j]].r=(unsigned char)(r*255);
                images[year-config.firstyear][coord_index[j]].g=(unsigned char)(g*255);
                images[year-config.firstyear][coord_index[j]].b=(unsigned char)(b*255);
              }
            }
            break;
         default:
            for(j=0;j<total;j++)
            {
              gr_val2rgb(&r,&g,&b,(data[0][j]-scale_min)/(scale_max-scale_min),GR_GMAP);
              images[year-config.firstyear][coord_index[j]].r=(unsigned char)(r*255);
              images[year-config.firstyear][coord_index[j]].g=(unsigned char)(g*255);
              images[year-config.firstyear][coord_index[j]].b=(unsigned char)(b*255);
            }
        }
        draw(theDisp,mainW,theScreen,theGC,images[year-config.firstyear],year); 
      }
    }
    readdouble_socket(socket,(Real *)&flux,sizeof(Flux)/sizeof(Real));
    if((year-config.firstyear) % 20==0)
      printf("\nYear NEP     fire    estab   harvest total\n"
               "---- ------- ------- ------- ------- -------\n");
    printf("%4d %7.3f %7.3f %7.3f %7.3f %7.3f\n",
           year,flux.nep*1e-15,flux.fire*1e-15,flux.estab*1e-15,
           flux.harvest*1e-15,(flux.nep-flux.fire-flux.harvest+flux.estab)*1e-15);
  }
  time(&end);
  printf("Frame rate: %.1f (1/sec)\n",
         (ismonthlyoutput(out_index)) ?
           (config.lastyear-config.firstyear+1)*12.0/(float)(end-start):
         (config.lastyear-config.firstyear+1)/(float)(end-start));
  for(i=0;i<NOUT;i++)
   if(files[i].file!=NULL)
     fclose(files[i].file);
  if(ismonthlyoutput(out_index))
    loopmonth(theDisp,mainW,theScreen,theGC,images,&config);
  else if(isdailyoutput(out_index))
    eventloop(theDisp,mainW,theScreen,theGC,images,&config);
  else
    loopyear(theDisp,mainW,theScreen,theGC,images,&config);
  close_socket(socket);
  XDestroyWindow(theDisp,mainW);
  XCloseDisplay(theDisp);
  return EXIT_SUCCESS;
} /* of 'main' */
