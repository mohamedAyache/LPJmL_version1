/***************************************************************************/
/**                                                                       **/
/**                  a  g  r  i  c  u  l  t  u  r  e  .  h                **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     Declaration of agriculture stand                                  **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: $Date:: 2015-07-22 11:01:20 +0200 (mer. 22 juil. 2#$ **/
/**     By         : $Author:: bloh                            $          **/
/**                                                                       **/
/***************************************************************************/

#ifndef AGRICULTURE_H
#define AGRICULTURE_H

#define NOIRRIG 0
#define SURF 1
#define SPRINK 2
#define DRIP 3

typedef struct
{
  Bool irrigation;            /* stand irrigated? (TRUE/FALSE) */
  int irrig_event;            /* irrigation water applied to field that day? depends on soil moisture and precipitation, if not irrig_amount is put to irrig_stor */
  int irrig_system;           /* irrigation system type (NOIRRIG=0,SURF=1,SPRINK=2,DRIP=3) */
  Real ec;                    /* conveyance efficiency */
  Real conv_evap;             /* fraction of conveyance losses that is assumed to evaporate */
  Real net_irrig_amount;      /* deficit in upper 50cm soil to fulfill demand (mm) */
  Real dist_irrig_amount;     /* water requirements for uniform field distribution, depending on irrigation system */
  Real irrig_amount;          /* irrigation water (mm) that reaches the field, i.e. without conveyance losses */
  Real irrig_stor;            /* storage buffer (mm), filled if irrig_threshold not reached and if irrig_amount > GIR and with irrig_amount-prec */
  Real no3_stor;              /* no3 quantity in the storage buffer (g)*/
} Irrigation;

extern Standtype agriculture_stand;
extern Standtype managedforest_stand;
extern Standtype setaside_rf_stand;
extern Standtype setaside_ir_stand;
extern Standtype kill_stand;

extern Real daily_agriculture(Stand *,Real,Dailyclimate,int,
                              Real,const Real [],Real,
                              Real,Real,Real,Real,Real,Real,int,int,int,
                              Bool,Bool,const Config *,int);
extern Bool annual_agriculture(Stand *,int,int,Real,int,Bool,const Config *);

extern Bool annual_setaside(Stand *,int,int,Real,int,Bool,const Config *);

extern Real daily_setaside(Stand *,Real,Dailyclimate,int,
                           Real,const Real [],Real,
                           Real,Real,Real,Real,Real,Real,int,int,int,
                           Bool,Bool,const Config *,int);

extern void output_gbw_agriculture(Output *,const Stand *,Real,Real,Real,Real,
                                   const Real[LASTLAYER],const Real[LASTLAYER],Real,Real,int,int,
                                   int,Bool);
extern void new_agriculture(Stand *);
extern void free_agriculture(Stand *);
extern Bool fread_agriculture(FILE *,Stand *,Bool);
extern Bool fwrite_agriculture(FILE *,const Stand *);
extern void fprint_agriculture(FILE *,const Stand *);

#endif
