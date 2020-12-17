#!/bin/sh
# IMPLEMENT (cd $pft && $(MAKE)) and (cd $pft && $(MAKE) clean) in src/Makefile !!!!!

########################################################################
##                                                                    ##
##               n  e  w  p  f  t  .  s  h                            ##
##                                                                    ##
##   Shell script to create new PFT templates for LPJmL 3.5.003       ##
##                                                                    ##
##   written by Werner von Bloh                                       ##
##   Potsdam Institute for Climate Impact Research                    ##
##   P.O. Box 60 12 03                                                ##
##   14412 Potsdam/Germany                                            ##
##                                                                    ##
##   Last change: 14.10.2009                                          ##
##                                                                    ##
########################################################################

function header {

cat >$LPJROOT/src/$1/$2 << EOF
/***************************************************************************/
/**                                                                       **/
EOF
printf "/**            %26s                                 **/\n" $2 >>src/$1/$2
cat >>$LPJROOT/src/$1/$2 << EOF
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran version         **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
EOF
printf "/**     Last change: %s by %-10s                             **/\n" $(date +"%d.%m.%Y") $(whoami) >> $LPJROOT/src/$1/$2
cat >>$LPJROOT/src/$1/$2 << EOF
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"
#include "$1.h"

EOF
}
if [ $# -lt 1 ]
then
  echo >&2 Argument missing!
  echo >&2 Usage: $0 pfttype
  exit 1
fi
if [ "$LPJROOT" = "" ]
then
  echo >&2 Error: environment variable LPJROOT is not set
  echo >&2 "Set by export LPJROOT=<path to lpjml directory>"
  exit 1
fi

pft=$1
if mkdir -p $LPJROOT/src/$pft ;
then
  echo Creating directory $LPJROOT/src/$pft
else
  echo >&2 Error creating directory $LPJROOT/src/$pft
  exit 1
fi
cat >$LPJROOT/src/$pft/Makefile << EOF
########################################################################
##                                                                    ##
##               M  a  k  e  f  i  l  e                               ##
##                                                                    ##
EOF
printf "##   Makefile for C implementation of %-31s ##\n" "$pft part of LPJ" >>$LPJROOT/src/$pft/Makefile
cat >>$LPJROOT/src/$pft/Makefile << EOF
##                                                                    ##
##   written by Werner von Bloh                                       ##
##   Potsdam Institute for Climate Impact Research                    ##
##   P.O. Box 60 12 03                                                ##
##   14412 Potsdam/Germany                                            ##
##                                                                    ##
EOF
printf "##   Last change: %s                                          ##\n" $(date +"%d.%m.%Y") >> $LPJROOT/src/$pft/Makefile
cat >>$LPJROOT/src/$pft/Makefile << EOF
##                                                                    ##
########################################################################

include ../../Makefile.inc

OBJS	= lai_$pft.\$O npp_$pft.\$O phenology_$pft.\$O\\
          init_$pft.\$O alphaa_$pft.\$O annual_$Pft.\$O\\
          allocation_$pft.\$O fprintpar_$pft.\$O\\
          litter_update_$pft.\$O fpc_$pft.\$O annua_$pft.\$O\\
          new_$pft.\$O fwrite_$pft.\$O fscanpft_$pft.\$O  fprint_$pft.\$O \\
          fread_$pft.\$O establishment_$pft.\$O fire_$pft.\$O free_$pft.\$O\\
          vegc_sum_$pft.\$O adjust_$pft.\$O wdf_$pft.\$O livefuel_consum_$pft.\$O

INC     = ../../include
LIBDIR	= ../../lib
LIB	= lib$pft.\$A

HDRS    = \$(INC)/buffer.h \$(INC)/coord.h \$(INC)/lpj.h \$(INC)/pftlist.h\\
          \$(INC)/soil.h \$(INC)/climate.h \$(INC)/date.h \$(INC)/pft.h\\
          \$(INC)/pftpar.h \$(INC)/types.h \$(INC)/$pft.h\\
          \$(INC)/errmsg.h \$(INC)/numeric.h\\
          \$(INC)/conf.h \$(INC)/swap.h \$(INC)/soilpar.h \$(INC)/stand.h\\
          \$(INC)/list.h \$(INC)/cell.h  \$(INC)/units.h \\
          \$(INC)/config.h \$(INC)/landuse.h \$(INC)/carbon.h

\$(LIBDIR)/\$(LIB): \$(OBJS)
	\$(AR) \$(ARFLAGS)\$(LIBDIR)/\$(LIB) \$(OBJS)

\$(OBJS): \$(HDRS)

.c.\$O: 
	\$(CC) \$(CFLAGS) -I\$(INC) -c \$*.c
clean:
	\$(RM) \$(RMFLAGS) \$(OBJS) 
	(cd \$(LIBDIR) && \$(RM) \$(RMFLAGS) \$(LIB))
EOF

cat >$LPJROOT/include/$pft.h << EOF
/***************************************************************************/
/**                                                                       **/
EOF
printf "/**            %16s                                           **/\n" $pft.h >> $LPJROOT/include/$pft.h 
cat >>$LPJROOT/include/$pft.h << EOF
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran version         **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
EOF
printf "/**     Last change: %s                                           **/\n" $(date +"%d.%m.%Y") >> $LPJROOT/include/$pft.h
cat >>$LPJROOT/include/$pft.h << EOF
/**                                                                       **/
/***************************************************************************/

#ifndef ${pft}_H /* Already included? */
#define ${pft}_H

/* Definition of constants */

/* Declaration of datatypes */

typedef struct
{
} Pft$pft;

typedef struct
{
} Pft${pft}par;

/* Declaration of functions */

extern void new_$pft(Pft *);
extern Real npp_$pft(Pft *,Real,Real,Real);
extern Real wdf_$pft(Pft *,Real,Real);
extern Real fpc_$pft(Pft *);
extern void litter_update_$pft(Litter *,Pft *,Real);
extern Bool allocation_$pft(Litter *,Pft *,Real *);
extern Real lai_$pft(const Pft *);
extern Real actual_lai_$pft(const Pft *);
extern Bool annual_$pft(Stand *,Pft *,Real *);
extern void phenology_$pft(Pft *,Real,int);
extern Bool fwrite_$pft(FILE *,const Pft *);
extern void fprint_$pft(FILE *,const Pft *);
extern Bool fread_$pft(FILE *,Pft *,Bool);
extern Bool fscanpft_$pft(FILE *,Pftpar *,const char *,Bool);
extern Real establishment_$pft(Pft *,Real,Real,int);
extern void init_$pft(Pft *);
extern Real fire_$pft(Pft *,Real);
extern Real vegc_sum_$pft(const Pft *);
extern void free_$pft(Pft *);
extern void adjust_$pft(Litter *,Pft *,Real);
extern void reduce_$pft(Litter *,Pft *,Real);
extern Real alphaa_$pft(const Pft *);
extern void fprintpar_$pft(FILE *,const Pftpar *);
extern Real livefuel_consum_$pft(Litter *,Pft *,const Fuel *,Livefuel *,
                                   Bool *,Real,Real);

/* Definitions of macros */

#endif
EOF
header $pft new_$pft.c
cat >>$LPJROOT/src/$pft/new_$pft.c  << EOF
void new_$pft(Pft *pft)
{
  Pft$pft *$pft;
  $pft=new(Pft$pft);
  pft->data=$pft; 
} /* of 'new_$pft' */
EOF
header $pft npp_$pft.c
cat >>$LPJROOT/src/$pft/npp_$pft.c  << EOF
Real npp_$pft(Pft *pft,Real gtemp_air,Real gtemp_soil,
              Real assim)
{
} /* of 'npp_$pft' */
EOF
header $pft fpc_$pft.c
cat >>$LPJROOT/src/$pft/fpc_$pft.c  << EOF
Real fpc_$pft(Pft *pft)
{
} /* of 'fpc_$pft' */
EOF
header $pft livefuel_consum_$pft.c
cat >>$LPJROOT/src/$pft/livefuel_consum_$pft.c  << EOF
Real livefuel_consum_$pft(Litter *litter,
                          Pft *pft,
                          const Fuel *fuel,
                          Livefuel *livefuel,
                          Bool *isdead,
                          Real surface_fi,
                          Real fire_frac)
{
} /* of 'livefuel_consum_$pft' */
EOF
header $pft litter_update_$pft.c
cat >>$LPJROOT/src/$pft/litter_update_$pft.c  << EOF
void litter_update_$pft(Litter *litter,Pft *pft,Real frac)
{
} /* of 'litter_update_$pft' */
EOF
header $pft alphaa_$pft.c
cat >>$LPJROOT/src/$pft/alphaa_$pft.c  << EOF
Real alphaa_$pft(const Pft *pft)
{
  return pft->par->alphaa;
} /* of 'alphaa_$pft' */
EOF
header $pft allometry_$pft.c
cat >>$LPJROOT/src/$pft/allometry_$pft.c  << EOF
void allometry_$pft(Pft *pft)
{
} /* of 'allometry_$pft' */
EOF
header $pft wdf_$pft.c
cat >>$LPJROOT/src/$pft/wdf_$pft.c  << EOF
Real wdf_$pft(Pft *pft,Real demand,Real supply)
{
} /* of 'wdf_$pft' */
EOF
header $pft annual_$pft.c
cat >>$LPJROOT/src/$pft/annual_$pft.c  << EOF
Bool annual$pft(Litter *litter,Pft *pft,Real *fpc_inc)
{
} /* of 'annual_$pft' */
EOF
header $pft allocation_$pft.c
cat >>$LPJROOT/src/$pft/allocation_$pft.c  << EOF
Bool allocation_$pft(Litter *litter,Pft *pft,Real *fpc_inc)
{
} /* of 'allocation_$pft' */
EOF
header $pft lai_$pft.c
cat >>$LPJROOT/src/$pft/lai_$pft.c  << EOF
Real lai_$pft(const Pft *pft)
{
} /* of 'lai_$pft' */

Real actual_lai_$pft(const Pft *pft)
{
} /* of 'actual_lai_$pft' */
EOF
header $pft turnover_$pft.c
cat >>$LPJROOT/src/$pft/turnover_$pft.c  << EOF
Real turnover_$pft(Litter *litter,Pft *pft)
{
} /* of 'turnover_$pft' */
EOF
header $pft phenology_$pft.c
cat >>$LPJROOT/src/$pft/phenology_$pft.c  << EOF
void phenology_$pft(Pft *pft,
                    Real temp,
                    int day
                   )
{
} /* of 'phenology_$pft' */
EOF
header $pft fwrite_$pft.c
cat >>$LPJROOT/src/$pft/fwrite_$pft.c  << EOF
Bool fwrite_$pft(FILE *file,const Pft *pft)
{
  return (fwrite(pft->data,sizeof(Pft$pft),1,file)!=1);
} /* of 'fwrite_$pft' */
EOF
header $pft fprint_$pft.c
cat >>$LPJROOT/src/$pft/fprint_$pft.c  << EOF
void fprint_$pft(FILE *file,const Pft *pft)
{
} /* of 'fprint_$pft' */
EOF
header $pft fread_$pft.c
cat >>$LPJROOT/src/$pft/fread_$pft.c  << EOF
Bool fread_$pft(FILE *file,Pft *pft,Bool swap)
{
 Pft$pft *$pft;
 $pft=new(Pft$pft);
 pft->data=$pft;
} /* of 'fread_$pft' */
EOF
header $pft fscanpft_$pft.c
cat >>$LPJROOT/src/$pft/fscanpft_$pft.c  << EOF
Bool fscanpft_$pft(FILE *file,Pftpar *pft,const char *filename,Bool isout)
{
  Pft${pft}par *pft${pft}par;
  pft->newpft=new_$pft;
  pft->npp=npp_$pft;
  pft->leaf_phenology=phenology_$pft;
  pft->fwrite=fwrite_$pft;
  pft->fprint=fprint_$pft;
  pft->fread=fread_$pft;
  pft->litter_update=litter_update_$pft;
  pft->allocation=allocation_$pft;
  pft->annual=annual_$pft;
  pft->establishment=establishment_$pft;
  pft->init=init_$pft;
  pft->fire=fire_$pft;
  pft->actual_lai=actual_lai_$pft;
  pft->free=free_$pft;
  pft->vegc_sum=vegc_sum_$pft;
  pft->adjust=adjust_$pft;
  pft->reduce=reduce_$pft;
  pft->wdf=wdf_$pft;
  pft->fprintpar=fprintpar_$pft;
  pft->alphaa_manage=alphaa_$pft;
  pft->livefuel_consumption=livefuel_consum_$pft;
  pft${pft}par=new(Pft${pft}par);
  pft->data=pft${pft}par;
  return FALSE;
} /* of 'fscanpft_$pft' */
EOF
header $pft establishment_$pft.c
cat >>$LPJROOT/src/$pft/establishment_$pft.c  << EOF
Real establishment_$pft(Pft *pft,Real fpc_total,Real fpc_tree,int n_est)
{
} /* of 'establishment_$pft' */
EOF
header $pft init_$pft.c
cat >>$LPJROOT/src/$pft/init_$pft.c  << EOF
void init_$pft(Pft *pft)
{
} /* of 'init_$pft' */
EOF
header $pft fire_$pft.c
cat >>$LPJROOT/src/$pft/fire_$pft.c  << EOF
Real fire_$pft(Pft *pft,Real fireprob)
{
} /* of 'fire_$pft' */
EOF
header $pft vegc_sum_$pft.c
cat >>$LPJROOT/src/$pft/vegc_sum_$pft.c  << EOF
Real vegc_sum_$pft(const Pft *pft)
{
} /* of 'vegc_sum_$pft' */
EOF
header $pft free_$pft.c
cat >>$LPJROOT/src/$pft/free_$pft.c  << EOF
void free_$pft(Pft *pft)
{
  free(pft->data);
} /* of 'free_$pft' */
EOF
header $pft fprintpar_$pft.c
cat >>$LPJROOT/src/$pft/fprintpar_$pft.c  << EOF
void fprintpar_$pft(FILE *file,const Pftpar *par)
{
} /* of 'fprintpar_$pft' */
EOF
header $pft reduce_$pft.c
cat >>$LPJROOT/src/$pft/reduce_$pft.c  << EOF
void reduce_$pft(Litter *litter,Pft *pft,Real factor)
{
} /* of 'reduce_$pft' */
EOF
header $pft adjust_$pft.c
cat >>$LPJROOT/src/$pft/adjust_$pft.c  << EOF
void adjust_$pft(Litter *litter,Pft *pft,Real fpc_$pft)
{
} /* of 'adjust_$pft' */
EOF
echo  Implement '(cd' $pft '&& $(MAKE)) and (cd' $pft '&& $(MAKE) clean) in ' $LPJROOT/src/Makefile
