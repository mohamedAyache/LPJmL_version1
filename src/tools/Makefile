########################################################################
##                                                                    ##
##               M  a  k  e  f  i  l  e                               ##
##                                                                    ##
##   Makefile for C implementation of tools part of LPJ               ##
##   Creates library ../../lib/libtools.a                             ##
##                                                                    ##
##   written by Werner von Bloh                                       ##
##   Potsdam Institute for Climate Impact Research                    ##
##   P.O. Box 60 12 03                                                ##
##   14412 Potsdam/Germany                                            ##
##                                                                    ##
##   Last change: $Date:: 2013-10-15 10:24:05 +0200 (mar., 15 oct. #$ ##
##   By         : $Author:: bloh                            $         ##
##                                                                    ##
########################################################################

include ../../Makefile.inc

OBJS	= swap.$O freadheader.$O coord.$O openinputfile.$O\
          fscanreal.$O fscanstring.$O fail.$O list.$O headersize.$O\
          bigendian.$O fprintheader.$O sysname.$O queue.$O\
          openrestart.$O banner.$O getdir.$O freadanyheader.$O\
          getpath.$O getuser.$O gethost.$O failonerror.$O\
          addpath.$O frepeatch.$O isabspath.$O mpi_write.$O\
          printflags.$O getfilesize.$O enablefpe.$O\
          strippath.$O diskfree.$O fprintintf.$O freadrestartheader.$O\
          fwriteheader.$O getcounts.$O getfiledate.$O fscanint.$O\
          iserror.$O mpi_write_txt.$O mkfilename.$O stripsuffix.$O\
          hassuffix.$O checkfmt.$O findstr.$O fputstring.$O fscanfloat.$O\
          fscantoken.$O fprinttime.$O

INC	= ../../include
LIBDIR	= ../../lib
LIB	= libtools.$A

HDRS	= $(INC)/buffer.h $(INC)/coord.h $(INC)/lpj.h\
          $(INC)/climate.h $(INC)/date.h $(INC)/swap.h\
          $(INC)/pftpar.h $(INC)/types.h $(INC)/header.h\
          $(INC)/errmsg.h $(INC)/numeric.h $(INC)/channel.h\
          $(INC)/conf.h $(INC)/swap.h $(INC)/soilpar.h\
          $(INC)/list.h $(INC)/cell.h $(INC)/units.h\
          $(INC)/config.h $(INC)/queue.h $(INC)/output.h


$(LIBDIR)/$(LIB): $(OBJS)
	$(AR) $(ARFLAGS)$(LIBDIR)/$(LIB) $(OBJS)

$(OBJS): $(HDRS)

printflags.$O: printflags.c
	$(CC) $(CFLAGS) -DCFLAGS='"$(LPJFLAGS)"' -I$(INC) -c $*.c

.c.$O: 
	$(CC) $(CFLAGS) -I$(INC) -c $*.c

clean: 
	$(RM) $(RMFLAGS) $(OBJS) 
	(cd $(LIBDIR) && $(RM) $(RMFLAGS) $(LIB))
