########################################################################
##                                                                    ##
##               M  a  k  e  f  i  l  e                               ##
##                                                                    ##
##   Makefile for C implementation of numeric part of LPJ             ##
##   Creates library ../../lib/libnum.a                               ##
##                                                                    ##
##   written by Werner von Bloh                                       ##
##   Potsdam Institute for Climate Impact Research                    ##
##   P.O. Box 60 12 03                                                ##
##   14412 Potsdam/Germany                                            ##
##                                                                    ##
##   Last change: $Date:: 2015-11-06 11:17:06 +0100 (ven., 06 nov. #$ ##
##   By         : $Author:: bloh                            $         ##
##                                                                    ##
########################################################################

include ../../Makefile.inc

OBJS	= leftmostzero.$O bisect.$O linreg.$O date.$O interpolate.$O\
          buffer.$O rand.$O petpar.$O zbrent.$O bilinear.$O linear.$O\
          vec_sum.$O ivec_sum.$O int2date.$O petpar2.$O gamma.$O\
          petpar3.$O

INC	= ../../include
LIBDIR	= ../../lib
LIB	= libnum.$A

HDRS	= $(INC)/types.h $(INC)/numeric.h $(INC)/date.h\
          $(INC)/list.h $(INC)/buffer.h $(INC)/errmsg.h 

$(LIBDIR)/$(LIB): $(OBJS)
	$(AR) $(ARFLAGS)$(LIBDIR)/$(LIB) $(OBJS)

$(OBJS): $(HDRS)

.c.$O: 
	$(CC) $(CFLAGS) -I$(INC) -c $*.c

clean: 
	$(RM) $(RMFLAGS) $(OBJS)
	(cd $(LIBDIR) && $(RM) $(RMFLAGS) $(LIB))
