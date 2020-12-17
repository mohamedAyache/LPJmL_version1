########################################################################
##                                                                    ##
##               M  a  k  e  f  i  l  e                               ##
##                                                                    ##
##   Makefile for database program                                    ##
##                                                                    ##
##   written by Werner von Bloh                                       ##
##   Potsdam Institute for Climate Impact Research                    ##
##   P.O. Box 60 12 03                                                ##
##   14412 Potsdam/Germany                                            ##
##                                                                    ##
##   Last change: 15.12.2008                                          ##
##                                                                    ##
########################################################################

include ../../Makefile.inc

.SUFFIXES: .pc .c

OBJS	= database.$O db2clm.$O

INC	= ../../include
LIBDIR	= ../../lib
BIN	= ../../bin

$(BIN)/db2clm$E: $(OBJS) $(LIBDIR)/libtools.$A $(LIBDIR)/libnum.$A
	$(LINK) $(LNOPTS)$(BIN)/db2clm$E $(OBJS) $(LIBDIR)/libtools.$A\
                $(LIBDIR)/libnum.$A $(ORACLE_HOME)/lib/libclntsh.so

database.c: database.pc

.pc.c:
	proc $*.pc

.c.$O:
	$(CC) $(CFLAGS) -I$(INC) -I$(ORACLE_HOME)/precomp/public -c $*.c

$(OBJS): database.h $(INC)/coord.h $(INC)/types.h $(INC)/header.h\
         $(INC)/lpj.h $(INC)/date.h $(INC)/coord.h

clean:
	(cd $(BIN) && $(RM) $(RMFLAGS) db2clm$E)
	$(RM) $(RMFLAGS) $(OBJS) database.c
