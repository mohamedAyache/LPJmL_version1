########################################################################
##                                                                    ##
##               M  a  k  e  f  i  l  e                               ##
##                                                                    ##
##   Makefile for C implementation of distributed networks            ##
##   Creates library ../../lib/libpnet.a                              ##
##                                                                    ##
##   written by Werner von Bloh                                       ##
##   Potsdam Institute for Climate Impact Research                    ##
##   P.O. Box 60 12 03                                                ##
##   14412 Potsdam/Germany                                            ##
##                                                                    ##
##   Last change: 16.09.2009                                          ##
##                                                                    ##
########################################################################

include ../../Makefile.inc

OBJS	= pnet_init.$O  pnet_setup.$O pnet_addconnect.$O intlist.$O\
          pnet_reverse.$O pnet_free.$O pnet_strerror.$O pnet_dup.$O

INC	= ../../include
LIBDIR	= ../../lib
LIB	= libpnet.$A

HDRS	= $(INC)/types.h $(INC)/pnet.h $(INC)/list.h $(INC)/intlist.h\
          $(INC)/errmsg.h


$(LIBDIR)/$(LIB): $(OBJS)
	$(AR) $(ARFLAGS)$(LIBDIR)/$(LIB) $(OBJS)

$(OBJS): $(HDRS)

.c.$O: 
	$(CC) $(CFLAGS) -I$(INC) -c $*.c

clean: 
	$(RM) $(RMFLAGS) $(OBJS)
	(cd $(LIBDIR) && $(RM) $(RMFLAGS) $(LIB))
