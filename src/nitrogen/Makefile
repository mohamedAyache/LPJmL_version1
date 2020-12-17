########################################################################
##                                                                    ##
##               M  a  k  e  f  i  l  e                               ##
##                                                                    ##
##   Makefile for C implementation of soil part of LPJ                ##
##                                                                    ##
##   written by Werner von Bloh                                       ##
##   Potsdam Institute for Climate Impact Research                    ##
##   P.O. Box 60 12 03                                                ##
##   14412 Potsdam/Germany                                            ##
##                                                                    ##
##   Last change: $Date:: 2015-07-15 12:26:32 +0200 (mer., 15 juil.#$ ##
##   By         : $Author:: bloh                            $         ##
##                                                                    ##
########################################################################

include ../../Makefile.inc

# replace .c by .$O
OBJS	= nit_openfiles.$O\
	  nit_readfiles.$O\
	  nit_crop_grass_surf_percount.$O\
	  nit_tempdep.$O\
	  nit_denitrification.$O\
	  nit_phyto.$O\
	  nit_wastewater.$O\
	  nit_fscannitconfig.$O\
	  nit_manure.$O\
	  nit_fertilizers.$O\
	  nit_nitrification.$O\
	  nit_infil.$O\
	  nit_write_restarts.$O\
	  nit_read_restarts.$O\
	  nit_update_water.$O\
	  nit_uptake_crop.$O\
	  nit_uptake_grass.$O\
	  nit_uptake_tree.$O\
	  nit_remin_tree.$O\
	  nit_remin_grass.$O\
	  nit_remin_crop.$O\
	  nit_update_preccheck.$O\
	  nit_immo_soil.$O\
	  nit_init_uptake_residue.$O\
	  nit_uptake_allpft.$O\
	  nit_remin_allpft.$O\
	  nit_update_outputs.$O\
	  nit_adsorption.$O\
	  nit_immo_water.$O\
	  nit_init_nutrients.$O\
	  nit_deforest_updatesoil.$O\
	  nit_mix_setaside.$O\
	  nit_init_application.$O\
	  nit_printlog.$O\


INC     = ../../include
LIBDIR	= ../../lib
LIB	= libnitrogen.$A

HDRS    = $(INC)/buffer.h $(INC)/coord.h $(INC)/lpj.h $(INC)/pftlist.h\
          $(INC)/soil.h $(INC)/climate.h $(INC)/date.h $(INC)/pft.h\
          $(INC)/pftpar.h $(INC)/types.h $(INC)/header.h $(INC)/landuse.h\
          $(INC)/crop.h $(INC)/errmsg.h $(INC)/numeric.h $(INC)/spitfire.h\
          $(INC)/conf.h $(INC)/swap.h $(INC)/soilpar.h $(INC)/stand.h\
          $(INC)/list.h $(INC)/cell.h  $(INC)/units.h $(INC)/output.h\
          $(INC)/config.h $(INC)/param.h $(INC)/cdf.h $(INC)/discharge.h\
          $(INC)/climbuf.h $(INC)/reservoir.h $(INC)/agriculture.h\
	  $(INC)/nitrogen.h

$(LIBDIR)/$(LIB): $(OBJS)
	$(AR) $(ARFLAGS)$(LIBDIR)/$(LIB) $(OBJS)

$(OBJS): $(HDRS)

.c.$O: 
	$(CC) $(CFLAGS) -I$(INC) -c $*.c

clean: 
	$(RM) $(RMFLAGS) $(OBJS)
	(cd $(LIBDIR) && $(RM) $(RMFLAGS) $(LIB))
