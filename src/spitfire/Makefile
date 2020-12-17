########################################################################
##                                                                    ##
##               M  a  k  e  f  i  l  e                               ##
##                                                                    ##
##   Makefile for C implementation of SPITFIRE part of LPJ            ##
##   creates library ../../lib/libspitfire.a                          ##
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

OBJS	= area_burnt.$O firedangerindex.$O fire_emissions.$O\
          popdens.$O humanignition.$O\
	  fraction_of_consumption.$O fuel_consumption_1hr.$O\
          initfuel.$O deadfuel_consumption.$O\
          initignition.$O\
          litter_update_fire.$O surface_fire_intensity.$O\
          wildfire_ignitions.$O fuelload.$O rateofspread.$O\
          windspeed_fpc.$O update_fbd_tree.$O fuel_consum_total.$O\
          update_fbd_grass.$O dailyfire.$O
		 
INC     = ../../include
LIBDIR	= ../../lib
LIB	= libspitfire.$A

HDRS    = $(INC)/buffer.h $(INC)/coord.h $(INC)/lpj.h $(INC)/pftlist.h\
          $(INC)/soil.h $(INC)/manage.h $(INC)/spitfire.h\
          $(INC)/climate.h $(INC)/date.h $(INC)/pft.h\
          $(INC)/pftpar.h $(INC)/types.h $(INC)/cropdates.h $(INC)/crop.h\
          $(INC)/errmsg.h $(INC)/numeric.h $(INC)/grass.h\
          $(INC)/conf.h $(INC)/swap.h $(INC)/soilpar.h $(INC)/managepar.h\
          $(INC)/spitfire.h $(INC)/stand.h $(INC)/cdf.h $(INC)/discharge.h\
          $(INC)/list.h $(INC)/cell.h  $(INC)/units.h\
          $(INC)/config.h $(INC)/climbuf.h
          
$(LIBDIR)/$(LIB): $(OBJS)
	$(AR) $(ARFLAGS)$(LIBDIR)/$(LIB) $(OBJS)

$(OBJS): $(HDRS)

.c.$O: 
	$(CC) $(CFLAGS) -I$(INC) -c $*.c
clean:
	$(RM) $(RMFLAGS) $(OBJS)
	(cd $(LIBDIR) && $(RM) $(RMFLAGS) $(LIB))
