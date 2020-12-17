########################################################################
##                                                                    ##
##          M  a  k  e  f  i  l  e  .  m  p  i  c  h                  ##
##                                                                    ##
##   Make include file for compiler and os specific settings          ##
##   Compile and link options for Gnu compiler gcc                    ##
##   Parallel MPI is enabled using MPICH                              ##
##                                                                    ##
##   written by Werner von Bloh                                       ##
##   Potsdam Institute for Climate Impact Research                    ##
##   P.O. Box 60 12 03                                                ##
##   14412 Potsdam/Germany                                            ##
##                                                                    ##
##   Last change: $Date:: 2015-10-28 10:14:19 +0100 (mer., 28 oct. #$ ##
##   By         : $Author:: bloh                            $         ##
##                                                                    ##
########################################################################

CC	= mpicc
DEBUGFLAGS  = -g
OPTFLAGS= -O3
WFLAG	= -Wall -m64
LPJFLAGS  = -DUSE_RAND48 -DUSE_MPI -DSAFE -DWITH_FPE -DUSE_TGAMMA
O	= o
A	= a
E	=
AR	= ar
ARFLAGS	= r 
RM	= rm 
RMFLAGS	= -f
LIBS	= -lm 
LINKMAIN= mpicc
LINK	= cc -m64
MKDIR	= mkdir -p
SLASH	= /
X11LIB	= -L/usr/X11R6/lib -lX11
VIEWPRG	= lpjliveview
CFLAGS	= $(WFLAG) $(LPJFLAGS) $(OPTFLAGS)
LNOPTS	= $(WFLAG) $(OPTFLAGS) -o 
LPJROOT = /home/alberte/SVN/SVN_LaserMed_5mn/LPJml_AgTree