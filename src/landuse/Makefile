########################################################################
##                                                                    ##
##               M  a  k  e  f  i  l  e                               ##
##                                                                    ##
##   Makefile for C implementation of landuse part of LPJ             ##
##   creates library ../../lib/liblanduse.a                           ##
##                                                                    ##
##   written by Werner von Bloh                                       ##
##   Potsdam Institute for Climate Impact Research                    ##
##   P.O. Box 60 12 03                                                ##
##   14412 Potsdam/Germany                                            ##
##                                                                    ##
##   Last change: $Date:: 2015-07-24 15:06:48 +0200 (ven., 24 juil.#$ ##
##   By         : $Author:: bloh                            $         ##
##                                                                    ##
########################################################################

include ../../Makefile.inc

OBJS	= cultivate.$O landuse.$O landusechange.$O\
          sowing_season.$O sowing_prescribe.$O\
          check_lu.$O agriculture.$O reclaim_land.$O allocation_today.$O\
          initmanage.$O fscancountrypar.$O fscanregionpar.$O fscantreedens.$O\
          findlandusetype.$O findstand.$O setaside.$O harvest_stand.$O\
          opencountrycode.$O readcountrycode.$O grassland.$O\
          biomass_tree.$O biomass_grass.$O daily_setaside.$O\
          createcountrycode.$O freecountrypar.$O freeregionpar.$O\
          writecountrycode.$O writeregioncode.$O freemanage.$O\
          timber_burn.$O timber_harvest.$O annual_grassland.$O\
          annual_agriculture.$O annual_biomass_grass.$O annual_setaside.$O\
          daily_grassland.$O daily_biomass_grass.$O annual_biomass_tree.$O\
          daily_agriculture.$O daily_biomass_tree.$O output_gbw_grassland.$O\
          output_gbw_biomass_grass.$O output_gbw_biomass_tree.$O\
          output_gbw_agriculture.$O irrigation_stand.$O killstand.$O\
          landfrac.$O cutpfts.$O crop_sum_frac.$O woodconsum.$O\
          new_agriculture.$O free_agriculture.$O fwrite_agriculture.$O\
          fread_agriculture.$O fprint_agriculture.$O fprintregionpar.$O\
          fprintcountrypar.$O agriculture_tree.$O daily_agriculture_tree.$O\
          annual_agriculture_tree.$O findstandpft.$O new_irrigsystem.$O\
          output_gbw_agriculture_tree.$O agriculture_grass.$O\
          annual_agriculture_grass.$O daily_agriculture_grass.$O\
          output_gbw_agriculture_grass.$O rw_irrigation.$O\
          free_irrigsystem.$O

INC	= ../../include
LIBDIR	= ../../lib
LIB	= liblanduse.$A

HDRS	= $(INC)/buffer.h $(INC)/coord.h $(INC)/lpj.h $(INC)/biomes.h\
          $(INC)/landuse.h $(INC)/date.h $(INC)/image.h $(INC)/crop.h\
          $(INC)/tree.h $(INC)/agriculture_grass.h\
          $(INC)/pftpar.h $(INC)/types.h $(INC)/header.h $(INC)/natural.h\
          $(INC)/errmsg.h $(INC)/numeric.h $(INC)/manage.h $(INC)/grassland.h\
          $(INC)/biomass_tree.h $(INC)/biomass_grass.h $(INC)/discharge.h\
          $(INC)/conf.h $(INC)/swap.h $(INC)/soilpar.h $(INC)/agriculture.h\
          $(INC)/list.h $(INC)/cell.h  $(INC)/units.h $(INC)/output.h\
          $(INC)/config.h $(INC)/stand.h $(INC)/pft.h $(INC)/pftlist.h\
          $(INC)/reservoir.h $(INC)/soil.h $(INC)/cropdates.h $(INC)/grass.h\
          $(INC)/tree.h $(INC)/outfile.h $(INC)/cdf.h $(INC)/param.h\
          $(INC)/spitfire.h $(INC)/climbuf.h $(INC)/agriculture_tree.h

$(LIBDIR)/$(LIB): $(OBJS)
	$(AR) $(ARFLAGS)$(LIBDIR)/$(LIB) $(OBJS)

$(OBJS): $(HDRS)

.c.$O: 
	$(CC) $(CFLAGS) -I$(INC) -c $*.c

clean: 
	$(RM) $(RMFLAGS) $(OBJS)
	(cd $(LIBDIR) && $(RM) $(RMFLAGS) $(LIB))
