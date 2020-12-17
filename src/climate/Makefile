########################################################################
##                                                                    ##
##               M  a  k  e  f  i  l  e                               ##
##                                                                    ##
##   Makefile for C implementation of climate part of LPJmL 3.4.019   ##
##   Creates library ../../lib/libclimate.a                           ##
##                                                                    ##
##   written by Werner von Bloh                                       ##
##   Potsdam Institute for Climate Impact Research                    ##
##   P.O. Box 60 12 03                                                ##
##   14412 Potsdam/Germany                                            ##
##                                                                    ##
##   Last change: 01.03.2013                                          ##
##                                                                    ##
########################################################################

include ../../Makefile.inc

OBJS	= initclimate.$O getclimate.$O freeclimate.$O avgtemp.$O\
          prdaily.$O getco2.$O storeclimate.$O dailyclimate.$O\
          getmtemp.$O initclimate_monthly.$O openclimate.$O\
          getmprec.$O checkvalidclimate.$O readco2.$O\
          closeclimate.$O

INC	= ../../include
LIBDIR	= ../../lib
LIB	= libclimate.$A

HDRS	= $(INC)/buffer.h $(INC)/lpj.h\
          $(INC)/climate.h $(INC)/date.h\
          $(INC)/pftpar.h $(INC)/types.h $(INC)/header.h\
          $(INC)/errmsg.h $(INC)/numeric.h\
          $(INC)/conf.h $(INC)/swap.h $(INC)/soilpar.h\
          $(INC)/list.h $(INC)/cell.h  $(INC)/units.h\
          $(INC)/config.h $(INC)/param.h $(INC)/cdf.h


$(LIBDIR)/$(LIB): $(OBJS)
	$(AR) $(ARFLAGS)$(LIBDIR)/$(LIB) $(OBJS)

$(OBJS): $(HDRS)

.c.$O: 
	$(CC) $(CFLAGS) -I$(INC) -c $*.c

clean: 
	$(RM) $(RMFLAGS) $(OBJS)
	(cd $(LIBDIR) && $(RM) $(RMFLAGS)  $(LIB))
