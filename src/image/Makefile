########################################################################
##                                                                    ##
##               M  a  k  e  f  i  l  e                               ##
##                                                                    ##
##   Makefile coupling LPJmL to IMAGE.                                ##
##   Creates library ../../lib/libimage.a                             ##
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

OBJS	= receive_image_climate.$O  receive_image_data.$O\
          receive_image_luc.$O receive_image_productpools.$O\
          send_image_data.$O biome_classification.$O\
          receive_image_co2.$O open_image.$O setoutput_image.$O\
          product_turnover.$O close_image.$O monthlyoutput_image.$O\
          initproductinit.$O getproductpools.$O new_image.$O

INC	= ../../include
LIBDIR	= ../../lib
LIB	= libimage.$A

HDRS    = $(INC)/buffer.h $(INC)/coord.h $(INC)/lpj.h $(INC)/pftlist.h\
          $(INC)/soil.h $(INC)/manage.h $(INC)/climate.h $(INC)/date.h\
          $(INC)/pft.h $(INC)/climbuf.h $(INC)/image.h $(INC)/biomes.h\
          $(INC)/pftpar.h $(INC)/types.h $(INC)/crop.h $(INC)/errmsg.h\
          $(INC)/numeric.h $(INC)/header.h $(INC)/landuse.h\
          $(INC)/conf.h $(INC)/swap.h $(INC)/soilpar.h $(INC)/managepar.h\
          $(INC)/stand.h $(INC)/discharge.h $(INC)/queue.h $(INC)/intlist.h\
          $(INC)/list.h $(INC)/cell.h  $(INC)/units.h $(INC)/output.h\
          $(INC)/config.h $(INC)/pnet.h $(INC)/channel.h $(INC)/reservoir.h\
          $(INC)/spitfire.h

$(LIBDIR)/$(LIB): $(OBJS)
	$(AR) $(ARFLAGS)$(LIBDIR)/$(LIB) $(OBJS)

$(OBJS): $(HDRS)

.c.$O: 
	$(CC) $(CFLAGS) -I$(INC) -c $*.c

clean: 
	$(RM) $(RMFLAGS) $(OBJS)
	(cd $(LIBDIR) && $(RM) $(RMFLAGS) $(LIB))
