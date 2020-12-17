########################################################################
##                                                                    ##
##               M  a  k  e  f  i  l  e                               ##
##                                                                    ##
##   Makefile for utility programs                                    ##
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

include ../../Makefile.inc

OBJS	= cru2clm.$O cvrtsoil.$O grid2clm.$O cfts26_lu2clm.$O drainage.$O\
          river_sections_input_grid.$O river_sections_input_soil.$O\
          river_sections_input_climate.$O river_sections_input_landuse.$O\
          river_sections_input_countrycode.$O river_sections_input_irrig.$O\
          lpjshuffle.$O txt2clm.$O water_use_input.$O catclm.$O asc2clm.$O\
          lpjprint.$O lpjcat.$O cat2bsq.$O grd2bsq.$O cft2clm.$O txt2grid.$O\
          printdrain.$O lpjcheck.$O lpjliveview.$O printclm.$O\
          printharvest.$O printreservoir.$O setclm.$O getcountry.$O\
          regridclm.$O regridsoil.$O cdf2coord.$O cdf2soil.$O lpjfiles.$O\
          joingrid.$O clm2cdf.$O soil2cdf.$O cdf2clm.$O bin2cdf.$O adddrain.$O\
          getcellindex.$O

SRC	= cru2clm.c cvrtsoil.c grid2clm.c cfts26_lu2clm.c drainage.c\
          river_sections_input_grid.c river_sections_input_soil.c\
          river_sections_input_climate.c river_sections_input_landuse.c\
          river_sections_input_countrycode.c river_sections_input_irrig.c\
          lpjshuffle.c txt2clm.c water_use_input.c catclm.c cdf2coord.c\
          lpjprint.c lpjcat.c cat2bsq.c grd2bsq.c cft2clm.c regridsoil.c\
          printdrain.c lpjcheck.c lpjliveview.c printclm.c printharvest.c\
          printreservoir.c lpjserver.c setclm.c getcountry.c regridclm.c\
          cdf2soil.c lpjfiles.c asc2clm.c joingrid.c clm2cdf.c soil2cdf.c\
          cdf2clm.c bin2cdf.c adddrain.c txt2grid.c getcellindex.c

INC     = ../../include

HDRS    = $(INC)/buffer.h $(INC)/coord.h $(INC)/lpj.h $(INC)/pftlist.h\
          $(INC)/soil.h $(INC)/manage.h $(INC)/climate.h $(INC)/date.h\
          $(INC)/pft.h $(INC)/pftpar.h $(INC)/types.h $(INC)/tree.h\
          $(INC)/header.h $(INC)/landuse.h $(INC)/crop.h $(INC)/grass.h\
          $(INC)/errmsg.h $(INC)/numeric.h $(INC)/conf.h $(INC)/swap.h\
          $(INC)/soilpar.h $(INC)/managepar.h $(INC)/stand.h $(INC)/types.h\
          $(INC)/list.h $(INC)/cell.h  $(INC)/units.h $(INC)/output.h\
          $(INC)/config.h $(INC)/intlist.h $(INC)/queue.h $(INC)/pnet.h\
          $(INC)/discharge.h $(INC)/channel.h $(INC)/reservoir.h $(INC)/soil.h\
          $(INC)/tree.h $(INC)/cdf.h $(INC)/spitfire.h $(INC)/climbuf.h

LIBDIR	= ../../lib
BIN	= ../../bin

LPJLIBS	= $(LIBDIR)/liblpj.$A $(LIBDIR)/libtree.$A $(LIBDIR)/libgrass.$A\
          $(LIBDIR)/libspitfire.$A $(LIBDIR)/libsoil.$A\
          $(LIBDIR)/libgrass.$A  $(LIBDIR)/liblanduse.$A\
          $(LIBDIR)/libreservoir.$A $(LIBDIR)/libclimate.$A\
          $(LIBDIR)/libnum.$A $(LIBDIR)/libtools.$A\
          $(LIBDIR)/libcrop.$A $(LIBDIR)/libpnet.$A $(LIBDIR)/libsoil.$A\
          $(LIBDIR)/libtree.$A $(LIBDIR)/liblpj.$A $(LIBDIR)/libcdf.$A\
          $(LIBDIR)/libtools.$A $(LIBDIR)/libnum.$A $(LIBDIR)/libnitrogen.$A

all: $(BIN)/cru2clm$E $(BIN)/grid2clm$E $(BIN)/cfts26_lu2clm$E\
     $(BIN)/river_sections_input_grid$E $(BIN)/river_sections_input_soil$E\
     $(BIN)/river_sections_input_climate$E $(BIN)/cdf2coord$E\
     $(BIN)/river_sections_input_landuse$E $(BIN)/cdf2soil$E\
     $(BIN)/river_sections_input_countrycode$E $(BIN)/txt2clm$E\
     $(BIN)/lpjshuffle$E $(BIN)/setclm$E $(BIN)/lpjfiles$E\
     $(BIN)/lpjcat$E $(BIN)/cat2bsq$E $(BIN)/grd2bsq$E $(BIN)/drainage$E\
     $(BIN)/cvrtsoil$E $(BIN)/river_sections_input_irrig$E $(BIN)/soil2cdf$E\
     $(BIN)/water_use_input$E $(BIN)/getcountry$E $(BIN)/clm2cdf$E\
     $(BIN)/cft2clm$E $(BIN)/printdrain$E $(BIN)/lpjcheck$E\
     $(BIN)/printreservoir$E $(BIN)/regridclm$E $(BIN)/asc2clm$E\
     $(BIN)/$(VIEWPRG)$E $(BIN)/catclm$E $(BIN)/printclm$E $(BIN)/joingrid$E\
     $(BIN)/printharvest$E $(BIN)/lpjprint$E $(BIN)/regridsoil$E\
     $(BIN)/cdf2clm$E $(BIN)/bin2cdf$E $(BIN)/adddrain$E $(BIN)/txt2grid$E\
     $(BIN)/getcellindex$E

clean:
	$(RM) $(RMFLAGS) $(OBJS)
	(cd $(BIN) && $(RM) $(RMFLAGS) cru2clm$E grid2clm$E cfts26_lu2clm$E\
            drainage$E river_sections_input_grid$E river_sections_input_soil$E\
            river_sections_input_climate$E river_sections_input_landuse$E\
            river_sections_input_countrycode$E txt2clm$E catclm$E cdf2coord$E\
            lpjcat$E cat2bsq$E grd2bsq$E lpjprint$E lpjshuffle$E printclm$E\
            cvrtsoil$E river_sections_input_irrig$E water_use_input$E\
            cft2clm$E printdrain$E lpjcheck$E $(VIEWPRG)$E printharvest$E\
            printreservoir$E setclm$E getcountry$E regridclm$E regridsoil$E\
            cdf2soil$E lpjfiles$E asc2clm$E joingrid$E clm2cdf$E soil2cdf$E\
            cff2clm$E bin2cdf$E adddrain$E txt2grid$E getcellindex$E)

$(OBJS): $(HDRS)

.c.$O: 
	$(CC) $(CFLAGS) -I$(INC) -c $*.c

bin2cdf.$O: bin2cdf.c
	$(CC) $(CFLAGS) -I$(INC) $(NETCDF_INC) -c bin2cdf.c

$(BIN)/bin2cdf$E: bin2cdf.$O $(LPJLIBS)
	$(LINK) $(LNOPTS)$(BIN)/bin2cdf$E bin2cdf.$O $(LPJLIBS) $(LIBS)

cdf2clm.$O: cdf2clm.c
	$(CC) $(CFLAGS) -I$(INC) $(NETCDF_INC) -c cdf2clm.c

$(BIN)/cdf2clm$E: cdf2clm.$O $(LPJLIBS)
	$(LINK) $(LNOPTS)$(BIN)/cdf2clm$E cdf2clm.$O $(LPJLIBS) $(LIBS)

soil2cdf.$O: soil2cdf.c
	$(CC) $(CFLAGS) -I$(INC) $(NETCDF_INC) -c soil2cdf.c

$(BIN)/soil2cdf$E: soil2cdf.$O $(LPJLIBS)
	$(LINK) $(LNOPTS)$(BIN)/soil2cdf$E soil2cdf.$O $(LPJLIBS) $(LIBS)

clm2cdf.$O: clm2cdf.c
	$(CC) $(CFLAGS) -I$(INC) $(NETCDF_INC) -c clm2cdf.c

$(BIN)/clm2cdf$E: clm2cdf.$O $(LPJLIBS)
	$(LINK) $(LNOPTS)$(BIN)/clm2cdf$E clm2cdf.$O $(LPJLIBS) $(LIBS)

cdf2soil.$O: cdf2soil.c
	$(CC) $(CFLAGS) -I$(INC) $(NETCDF_INC) -c cdf2soil.c

$(BIN)/cdf2soil$E: cdf2soil.$O $(LPJLIBS)
	$(LINK) $(LNOPTS)$(BIN)/cdf2soil$E cdf2soil.$O $(LPJLIBS) $(LIBS)

cdf2coord.$O: cdf2coord.c
	$(CC) $(CFLAGS) -I$(INC) $(NETCDF_INC) -c cdf2coord.c

$(BIN)/cdf2coord$E: cdf2coord.$O $(LPJLIBS)
	$(LINK) $(LNOPTS)$(BIN)/cdf2coord$E cdf2coord.$O $(LPJLIBS) $(LIBS)

$(BIN)/joingrid$E: joingrid.$O $(LPJLIBS)
	$(LINK) $(LNOPTS)$(BIN)/joingrid$E joingrid.$O $(LPJLIBS) $(LIBS)

$(BIN)/asc2clm$E: asc2clm.$O $(LPJLIBS)
	$(LINK) $(LNOPTS)$(BIN)/asc2clm$E asc2clm.$O $(LPJLIBS) $(LIBS)

$(BIN)/lpjcheck$E: lpjcheck.$O $(LPJLIBS)
	$(LINK) $(LNOPTS)$(BIN)/lpjcheck$E lpjcheck.$O $(LPJLIBS) $(LIBS)

$(BIN)/printharvest$E: printharvest.$O $(LPJLIBS)
	$(LINK) $(LNOPTS)$(BIN)/printharvest$E printharvest.$O $(LPJLIBS) $(LIBS)

$(BIN)/regridsoil$E: regridsoil.$O $(LIBDIR)/libtools.$A
	$(LINK) $(LNOPTS)$(BIN)/regridsoil$E regridsoil.$O $(LIBDIR)/libtools.$A $(LIBS)

$(BIN)/setclm$E: setclm.$O $(LIBDIR)/libtools.$A
	$(LINK) $(LNOPTS)$(BIN)/setclm$E setclm.$O $(LIBDIR)/libtools.$A

$(BIN)/catclm$E: catclm.$O $(LIBDIR)/libtools.$A
	$(LINK) $(LNOPTS)$(BIN)/catclm$E catclm.$O $(LIBDIR)/libtools.$A

$(BIN)/lpjliveview$E: lpjliveview.$O $(LIBDIR)/libsocket.$A $(LPJLIBS)
	$(LINK) $(LNOPTS)$(BIN)/lpjliveview$E lpjliveview.$O $(LPJLIBS)\
                $(LIBDIR)/libsocket.$A  $(LIBS)

$(BIN)/lpjserver$E: lpjserver.$O $(LIBDIR)/libsocket.$A $(LPJLIBS)
	$(LINK) $(LNOPTS)$(BIN)/lpjserver$E lpjserver.$O $(LPJLIBS)\
                $(LIBDIR)/libsocket.$A $(LIBS) 

$(BIN)/adddrain$E: adddrain.$O $(LPJLIBS)
	$(LINK) $(LNOPTS)$(BIN)/adddrain$E adddrain.$O $(LPJLIBS) $(LIBS)

$(BIN)/regridclm$E: regridclm.$O $(LPJLIBS)
	$(LINK) $(LNOPTS)$(BIN)/regridclm$E regridclm.$O $(LPJLIBS) $(LIBS)

$(BIN)/lpjfiles$E: lpjfiles.$O $(LPJLIBS)
	$(LINK) $(LNOPTS)$(BIN)/lpjfiles$E lpjfiles.$O $(LPJLIBS) $(LIBS)

$(BIN)/getcountry$E: getcountry.$O $(LPJLIBS)
	$(LINK) $(LNOPTS)$(BIN)/getcountry$E getcountry.$O $(LPJLIBS) $(LIBS)

$(BIN)/lpjprint$E: lpjprint.$O $(LPJLIBS)
	$(LINK) $(LNOPTS)$(BIN)/lpjprint$E lpjprint.$O $(LPJLIBS) $(LIBS)

$(BIN)/lpjshuffle$E: lpjshuffle.$O $(LPJLIBS)
	$(LINK) $(LNOPTS)$(BIN)/lpjshuffle$E lpjshuffle.$O $(LPJLIBS) $(LIBS)

$(BIN)/cru2clm$E: cru2clm.$O $(LIBDIR)/libnum.$A $(LIBDIR)/libtools.$A
	$(LINK) $(LNOPTS)$(BIN)/cru2clm$E cru2clm.$O\
                $(LIBDIR)/libnum.$A $(LIBDIR)/libtools.$A $(LIBS)

$(BIN)/txt2clm$E: txt2clm.$O $(LIBDIR)/libnum.$A $(LIBDIR)/libtools.$A
	$(LINK) $(LNOPTS)$(BIN)/txt2clm$E txt2clm.$O\
                $(LIBDIR)/libnum.$A $(LIBDIR)/libtools.$A $(LIBS)

$(BIN)/grid2clm$E: grid2clm.$O $(LIBDIR)/libnum.$A $(LIBDIR)/libtools.$A
	$(LINK) $(LNOPTS)$(BIN)/grid2clm$E grid2clm.$O\
                $(LIBDIR)/libnum.$A $(LIBDIR)/libtools.$A $(LIBS)

$(BIN)/cfts26_lu2clm$E: cfts26_lu2clm.$O $(LIBDIR)/libnum.$A $(LIBDIR)/libtools.$A
	$(LINK) $(LNOPTS)$(BIN)/cfts26_lu2clm$E cfts26_lu2clm.$O\
                $(LIBDIR)/libnum.$A $(LIBDIR)/libtools.$A $(LIBS)

$(BIN)/drainage$E: drainage.$O $(LIBDIR)/libnum.$A $(LIBDIR)/libtools.$A
	$(LINK) $(LM) $(LNOPTS)$(BIN)/drainage$E drainage.$O\
                $(LIBDIR)/libnum.$A $(LIBDIR)/libtools.$A $(LIBS)

$(BIN)/river_sections_input_grid$E: river_sections_input_grid.$O\
                                    $(LIBDIR)/libnum.$A $(LIBDIR)/libtools.$A
	$(LINK) $(LNOPTS)$(BIN)/river_sections_input_grid$E\
                river_sections_input_grid.$O\
                $(LIBDIR)/libnum.$A $(LIBDIR)/libtools.$A $(LIBS)

$(BIN)/river_sections_input_soil$E: river_sections_input_soil.$O\
                                    $(LIBDIR)/libnum.$A $(LIBDIR)/libtools.$A
	$(LINK) $(LNOPTS)$(BIN)/river_sections_input_soil$E\
                river_sections_input_soil.$O\
                $(LIBDIR)/libnum.$A $(LIBDIR)/libtools.$A $(LIBS)

$(BIN)/river_sections_input_climate$E: river_sections_input_climate.$O\
                                       $(LIBDIR)/libnum.$A $(LIBDIR)/libtools.$A
	$(LINK) $(LNOPTS)$(BIN)/river_sections_input_climate$E\
                river_sections_input_climate.$O\
                $(LIBDIR)/libnum.$A $(LIBDIR)/libtools.$A $(LIBS)

$(BIN)/river_sections_input_landuse$E: river_sections_input_landuse.$O\
                                       $(LIBDIR)/libnum.$A $(LIBDIR)/libtools.$A
	$(LINK) $(LNOPTS)$(BIN)/river_sections_input_landuse$E\
                river_sections_input_landuse.$O\
                $(LIBDIR)/libnum.$A $(LIBDIR)/libtools.$A $(LIBS)

$(BIN)/river_sections_input_countrycode$E: river_sections_input_countrycode.$O\
                                           $(LIBDIR)/libnum.$A $(LIBDIR)/libtools.$A
	$(LINK) $(LNOPTS)$(BIN)/river_sections_input_countrycode$E\
                river_sections_input_countrycode.$O\
                $(LIBDIR)/libnum.$A $(LIBDIR)/libtools.$A $(LIBS)

$(BIN)/river_sections_input_irrig$E: river_sections_input_irrig.$O\
                                     $(LIBDIR)/libnum.$A $(LIBDIR)/libtools.$A
	$(LINK) $(LNOPTS)$(BIN)/river_sections_input_irrig$E\
                river_sections_input_irrig.$O\
                $(LIBDIR)/libnum.$A $(LIBDIR)/libtools.$A $(LIBS)

$(BIN)/water_use_input$E: water_use_input.$O $(LIBDIR)/libnum.$A $(LIBDIR)/libtools.$A
	$(LINK) $(LNOPTS)$(BIN)/water_use_input$E water_use_input.$O\
                $(LIBDIR)/libnum.$A $(LIBDIR)/libtools.$A $(LIBS)

$(BIN)/cft2clm$E: cft2clm.$O $(LIBDIR)/libnum.$A $(LIBDIR)/libtools.$A
	$(LINK) $(LNOPTS)$(BIN)/cft2clm$E cft2clm.$O\
                $(LIBDIR)/libnum.$A $(LIBDIR)/libtools.$A  $(LIBS)

$(BIN)/cvrtsoil$E: cvrtsoil.$O $(LIBDIR)/libtools.$A $(LIBDIR)/libnum.$A  
	$(LINK) $(LNOPTS)$(BIN)/cvrtsoil$E cvrtsoil.$(O)\
                $(LIBDIR)/libnum.$A $(LIBDIR)/libtools.$A $(LIBS)

$(BIN)/lpjcat$E: lpjcat.$O $(LIBDIR)/libtools.$A $(LIBDIR)/libnum.$A  
	$(LINK) $(LNOPTS)$(BIN)/lpjcat$E lpjcat.$(O)\
                $(LIBDIR)/libnum.$A $(LIBDIR)/libtools.$A $(LIBS)

$(BIN)/printreservoir$E: printreservoir.$O $(LIBDIR)/libtools.$A $(LIBDIR)/libnum.$A $(LIBDIR)/libreservoir.$A 
	$(LINK) $(LNOPTS)$(BIN)/printreservoir$E printreservoir.$(O)\
                $(LIBDIR)/libtools.$A $(LIBDIR)/libnum.$A\
                $(LIBDIR)/libreservoir.$A $(LIBS)

$(BIN)/printclm$E: printclm.$O $(LIBDIR)/libtools.$A $(LIBDIR)/libnum.$A $(LIBDIR)/libreservoir.$A
	$(LINK) $(LNOPTS)$(BIN)/printclm$E printclm.$(O)\
                $(LIBDIR)/libnum.$A $(LIBDIR)/libtools.$A\
                $(LIBDIR)/libreservoir.$A $(LIBS)

$(BIN)/printdrain$E: printdrain.$O $(LPJLIBS)  
	$(LINK) $(LNOPTS)$(BIN)/printdrain$E printdrain.$(O) $(LPJLIBS) $(LIBS)

$(BIN)/cat2bsq$E: cat2bsq.$O $(LPJLIBS)
	$(LINK) $(LNOPTS)$(BIN)/cat2bsq$E cat2bsq.$O $(LPJLIBS) $(LIBS)

$(BIN)/grd2bsq$E: grd2bsq.$O $(LPJLIBS)
	$(LINK) $(LNOPTS)$(BIN)/grd2bsq$E grd2bsq.$O $(LPJLIBS) $(LIBS)

$(BIN)/getcellindex$E: getcellindex.$O $(LPJLIBS)
	$(LINK) $(LNOPTS)$(BIN)/getcellindex$E getcellindex.$O $(LPJLIBS) $(LIBS)
$(BIN)/txt2grid$E: txt2grid.$O $(LPJLIBS)
	$(LINK) $(LNOPTS)$(BIN)/txt2grid$E txt2grid.$O $(LPJLIBS) $(LIBS)
