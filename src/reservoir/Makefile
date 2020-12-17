########################################################################
##                                                                    ##
##               M  a  k  e  f  i  l  e                               ##
##                                                                    ##
##   Makefile for C implementation of reservoir part of LPJ           ##
##   Creates library ../../lib/libreservoir.a                         ##
##                                                                    ##
##   written by Hester Biemans and Werner von Bloh                    ##
##   Potsdam Institute for Climate Impact Research                    ##
##   P.O. Box 60 12 03                                                ##
##   14412 Potsdam/Germany                                            ##
##                                                                    ##
##   Last change: $Date:: 2015-07-14 09:09:50 +0200 (mar., 14 juil.#$ ##
##   By         : $Author:: bloh                            $         ##
##                                                                    ##
########################################################################

include ../../Makefile.inc

OBJS	=  initreservoir.$O allocate_reservoir.$O irrig_amount_reservoir.$O\
           readreservoir.$O landusechange_for_reservoir.$O\
           check_stand_fracs_for_reservoir.$O outflow_reservoir.$O\
           sum_irrig_demand.$O update_reservoir_monthly.$O\
           update_reservoir_annual.$O initresdata.$O\
           fwriteresdata.$O freadresdata.$O fprintresdata.$O

INC	= ../../include
LIBDIR	= ../../lib
LIB	= libreservoir.$A

HDRS	= $(INC)/buffer.h $(INC)/coord.h $(INC)/lpj.h\
          $(INC)/climate.h $(INC)/date.h $(INC)/swap.h\
          $(INC)/pftpar.h $(INC)/types.h $(INC)/header.h\
          $(INC)/errmsg.h $(INC)/numeric.h $(INC)/channel.h\
          $(INC)/conf.h $(INC)/swap.h $(INC)/soilpar.h\
          $(INC)/list.h $(INC)/cell.h $(INC)/units.h\
          $(INC)/config.h $(INC)/queue.h $(INC)/output.h\
          $(INC)/reservoir.h $(INC)/pnet.h $(INC)/intlist.h\
          $(INC)/discharge.h $(INC)/landuse.h $(INC)/soil.h\
          $(INC)/stand.h $(INC)/natural.h $(INC)/cdf.h $(INC)/spitfire.h\
          $(INC)/pftlist.h $(INC)/climbuf.h


$(LIBDIR)/$(LIB): $(OBJS)
	$(AR) $(ARFLAGS)$(LIBDIR)/$(LIB) $(OBJS)

$(OBJS): $(HDRS)

.c.$O: 
	$(CC) $(CFLAGS) -I$(INC) -c $*.c

clean: 
	$(RM) $(RMFLAGS) $(OBJS) 
	(cd $(LIBDIR) && $(RM) $(RMFLAGS) $(LIB))
