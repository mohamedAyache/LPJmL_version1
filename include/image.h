/***************************************************************************/
/**                                                                       **/
/**                      i  m  a  g  e  .  h                              **/
/**                                                                       **/
/**     Declaration of IMAGE coupler functions and related datatypes      **/
/**                                                                       **/
/**     written by Christoph Mueller                                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: 19.08.2009 by Christoph Mueller & Elke Stehfest      **/
/**     Last change: 09.01.2013 by Werner von Bloh                        **/
/**                                                                       **/
/***************************************************************************/


#ifndef IMAGE_H /* already included? */
#define IMAGE_H

/* Definition of constants for biomass pools used in IMAGE */

#define PRODUCT_FAST 0.1
#define PRODUCT_SLOW 0.01

#define LPJIMAGE "LPJIMAGE"  /* Environment variable for IMAGE coupler */
#define LPJWAITIMAGE "LPJWAITIMAGE"  /* time to wait for IMAGE connection */
#define DEFAULT_IMAGE_HOST "localhost" /* Default host for IMAGE model */
#define DEFAULT_IMAGE_INPORT 2225  /* Default port for ingoing connection */
#define DEFAULT_IMAGE_OUTPORT 2224 /* Default port for outgoing connection */
#define WAIT_IMAGE 12000             /* wait for IMAGE connection (sec) */

/* Definition of datatypes */

typedef struct
{
  float slow,fast;
} Timber;

typedef struct
{
  Real fast,slow;
} Product;

typedef struct
{
  float stems;
  float branches; 
  float leaves; 
  float roots; 
  float litter;
  float humus;
  float charcoal;
  float product_fast;
  float product_slow;
} Biomass;

typedef struct
{
  FILE *file;     /* file pointer for open wateruse file */
  int firstyear;  /* first year in file */
  int nband;      /* number of bands in file */
  int size;       /* number of data items per year */
  int offset;     /* file offset for first grid element */
  Bool swap;      /* data has to be swapped for correct byte order */
  float scalar;   /* scaling factor for input data */
} Productinit;


typedef struct
{
  float pasture;
  float temp_cereals;
  float rice;
  float maize;
  float trop_cereals;
  float pulses;
  float roots_tubers;
  float oil_crops;
  float sugar_cane;
  float maize_feed;
  float woody_biofuels;
  float non_woody_biofuels;
  float irrig_temp_cereals;
  float irrig_rice;
  float irrig_maize;
  float irrig_trop_cereals;
  float irrig_pulses;
  float irrig_roots_tubers;
  float irrig_oil_crops;
} Image_landuse;

typedef struct
{
  Real fburnt;             /* fraction of deforested wood burnt */
  Real timber_frac;        /* fraction of gridcell harvested for timber */
  Real deforest_emissions; /* carbon emissions from deforested wood burnt [gC/m2]*/
  Pool timber_f;           /* fraction that enters fast and slow product pool */
  Pool timber;             /* fast and slow timber pool [gC/m2] */
  Real timber_turnover;    /* annual sum of decayed product pool [gC/m2] */
  Real anpp;               /* annual NPP (gC/m2) */
  Real arh;                /* annual Rh (gC/m2) */
  Real prod_turn_fast;     /* product turnover from fast product pool (gC/m2/a) */
  Real prod_turn_slow;     /* product turnover from slow product pool (gC/m2/a) */
  Real store_bmtree_yield; /* yield of biomass trees storage to distribute over following years (gC/m2/8a) */
  Real biomass_yield_annual; /* biomass yield scaled to annual fractions */
} Image_data;

/* Declaration of functions */

extern Image_data *new_image(const Product *);
extern Bool open_image(Config *);
extern void close_image(const Config *);
extern Real product_turnover(Image_data *);
extern Bool send_image_data(const Config *,const Cell *,const  Climate *,int);
extern Bool receive_image_climate(Climate *,int,const Config *);
extern Bool receive_image_productpools(Cell *,const Config *);
extern Bool receive_image_luc(Cell *,int,int,const Config *);
extern Bool receive_image_data(Cell *,int,int,const Config *);
extern Real receive_image_co2(const Config *);
extern Productinit *initproductinit(const Config *);
extern Bool getproductpools(Productinit *,Product [],int);
extern void setoutput_image(Cell *,int);
extern void monthlyoutput_image(Output *,const Climate *,int,int);

#endif /* IMAGE_H */
