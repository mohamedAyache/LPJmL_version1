########################################################################
##                                                                    ##
##               M  a  k  e  f  i  l  e                               ##
##                                                                    ##
##   Makefile for C implementation of crop part of LPJ                ##
##   creates library ../../lib/libcrop.a                              ##
##                                                                    ##
##   written by Werner von Bloh                                       ##
##   Potsdam Institute for Climate Impact Research                    ##
##   P.O. Box 60 12 03                                                ##
##   14412 Potsdam/Germany                                            ##
##                                                                    ##
##   Last change: $Date:: 2015-07-14 09:09:50 +0200 (mar., 14 juil.#$ ##
##   By         : $Author:: bloh                            $         ##
##                                                                    ##
########################################################################

include ../../Makefile.inc

OBJS	= lai_crop.$O npp_crop.$O phenology_crop.$O fwritecropdates.$O\
          freadcropdates.$O litter_update_crop.$O calc_cropdates.$O\
          fpc_crop.$O new_crop.$O fwrite_crop.$O fscanpft_crop.$O\
          fprint_crop.$O fread_crop.$O free_crop.$O vegc_sum_crop.$O\
          phen_variety.$O fpar_crop.$O freecropdates.$O\
          irrig_amount.$O laimax_manage.$O init_cropdates.$O\
          fprintcropdates.$O update_cropdates.$O\
          allocation_daily_crop.$O wateruse.$O\
          wdf_crop.$O harvest_crop.$O fprintpar_crop.$O\
          alphaa_crop.$O output_daily_crop.$O withdrawal_demand.$O\
          distribute_water.$O calc_seasonality.$O update_fallowdays.$O

INC     = ../../include
LIBDIR	= ../../lib
LIB	= libcrop.$A

HDRS    = $(INC)/buffer.h $(INC)/coord.h $(INC)/lpj.h $(INC)/pftlist.h\
          $(INC)/soil.h $(INC)/climate.h $(INC)/date.h $(INC)/pft.h\
          $(INC)/pftpar.h $(INC)/types.h $(INC)/cropdates.h $(INC)/crop.h\
          $(INC)/errmsg.h $(INC)/numeric.h $(INC)/output.h $(INC)/queue.h\
          $(INC)/conf.h $(INC)/swap.h $(INC)/soilpar.h $(INC)/stand.h\
          $(INC)/list.h $(INC)/cell.h  $(INC)/units.h $(INC)/intlist.h\
          $(INC)/config.h $(INC)/landuse.h $(INC)/manage.h $(INC)/pnet.h\
          $(INC)/param.h $(INC)/agriculture.h $(INC)/discharge.h\
          $(INC)/grass.h $(INC)/spitfire.h $(INC)/climbuf.h $(INC)/reservoir.h

$(LIBDIR)/$(LIB): $(OBJS)
	$(AR) $(ARFLAGS)$(LIBDIR)/$(LIB) $(OBJS)

$(OBJS): $(HDRS)

.c.$O: 
	$(CC) $(CFLAGS) -I$(INC) -c $*.c
clean:
	$(RM) $(RMFLAGS) $(OBJS) 
	(cd $(LIBDIR) && $(RM) $(RMFLAGS) $(LIB))
