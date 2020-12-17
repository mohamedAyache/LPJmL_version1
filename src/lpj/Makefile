########################################################################
##                                                                    ##
##               M  a  k  e  f  i  l  e                               ##
##                                                                    ##
##   Makefile for C implementation of LPJ                             ##
##   creates library ../../lib/liblpj.a                               ##
##                                                                    ##
##   written by Werner von Bloh                                       ##
##   Potsdam Institute for Climate Impact Research                    ##
##   P.O. Box 60 12 03                                                ##
##   14412 Potsdam/Germany                                            ##
##                                                                    ##
##   Last change: $Date:: 2015-07-22 11:01:20 +0200 (mer., 22 juil.#$ ##
##   By         : $Author:: bloh                            $         ##
##                                                                    ##
########################################################################

include ../../Makefile.inc

OBJS	= update_daily.$O update_annual.$O init_annual.$O albedo.$O\
          update_monthly.$O newgrid.$O iterate.$O initoutput_monthly.$O\
          iterateyear.$O nep_sum.$O iterateyear_river.$O\
          fwriteoutput_monthly2.$O initoutput.$O fopenoutput.$O\
          fcloseoutput.$O fwriteoutput_annual.$O fwriteoutput_pft.$O\
          fwriteoutput_monthly.$O newoutputmonthly.$O update_outputmonthly.$O\
          freeoutputmonthly.$O cflux_sum.$O flux_sum.$O initoutput_annual.$O\
          freeoutput.$O freegrid.$O freecell.$O fscanoutput.$O\
          fscanpftpar.$O firepft.$O climbuf.$O outputsize.$O\
          pftlist.$O newpft.$O establish.$O fprintstand.$O temp_stress.$O\
          updategdd.$O fwritecell.$O fwriterestart.$O freadcell.$O\
          fscanconfig.$O noinit.$O nofire.$O noturnover_monthly.$O\
          noestablishment.$O new_natural.$O free_natural.$O createpftnames.$O\
          freepft.$O photosynthesis.$O survive.$O outputnames.$O\
          light.$O gp.$O gp_sum.$O water_stressed.$O interception.$O\
          fpc_sum.$O establishmentpft.$O noadjust.$O fscanoutputvar.$O\
          freadpft.$O fwritepft.$O fwritestand.$O fprintpft.$O\
          fprintcell.$O writecoords.$O freadstand.$O equilsom.$O equilveg.$O\
          initgdd.$O standlist.$O nomix_veg.$O check_fluxes.$O\
          fprintconfig.$O freepftpar.$O drain.$O initdrain.$O\
          openconfig.$O freeconfig.$O readconfig.$O initconfig.$O\
          check_stand_fracs.$O waterusefcns.$O getoutputtype.$O\
          fscanlimit.$O filesexist.$O readfilename.$O freeoutputvar.$O\
          fprintflux.$O initoutput_daily.$O fwriteoutput_daily.$O\
          newoutputdaily.$O update_outputdaily.$O freeoutputdaily.$O\
          fwriteoutput_daily2.$O outputbuffersize.$O nowdf.$O\
          ismonthlyoutput.$O initmpiconfig.$O fscanparam.$O fprintparam.$O\
          fprintpftpar.$O help.$O copyright.$O initinput.$O freeinput.$O\
          natural.$O daily_natural.$O fread_natural.$O fprint_natural.$O\
          annual_natural.$O standcarbon.$O fwrite_natural.$O celldata.$O\
          fscanemissionfactor.$O check_balance.$O transfer_function.$O\
          fprintfiles.$O roughnesslength.$O closeoutput_yearly.$O\
          getncultivationtype.$O getroute.$O


INC	= ../../include
LIBDIR	= ../../lib
LIB	= liblpj.$A

HDRS	= $(INC)/buffer.h $(INC)/coord.h $(INC)/lpj.h $(INC)/pftlist.h\
          $(INC)/soil.h $(INC)/manage.h $(INC)/climate.h $(INC)/date.h\
          $(INC)/pft.h $(INC)/climbuf.h $(INC)/image.h $(INC)/biomes.h\
          $(INC)/pftpar.h $(INC)/types.h $(INC)/crop.h $(INC)/errmsg.h\
          $(INC)/numeric.h $(INC)/header.h $(INC)/landuse.h $(INC)/input.h\
          $(INC)/conf.h $(INC)/swap.h $(INC)/soilpar.h $(INC)/managepar.h\
          $(INC)/stand.h $(INC)/discharge.h $(INC)/queue.h $(INC)/intlist.h\
          $(INC)/list.h $(INC)/cell.h  $(INC)/units.h $(INC)/output.h\
          $(INC)/config.h $(INC)/pnet.h $(INC)/channel.h $(INC)/param.h\
          $(INC)/natural.h $(INC)/reservoir.h $(INC)/spitfire.h $(INC)/grass.h\
          $(INC)/cropdates.h $(INC)/tree.h $(INC)/outfile.h $(INC)/cdf.h


$(LIBDIR)/$(LIB): $(OBJS)
	$(AR) $(ARFLAGS)$(LIBDIR)/$(LIB) $(OBJS)

$(OBJS): $(HDRS)

.c.$O: 
	$(CC) $(CFLAGS) -I$(INC) -c $*.c

clean: 
	$(RM) $(RMFLAGS) $(OBJS)
	(cd $(LIBDIR) && $(RM) $(RMFLAGS) $(LIB))
