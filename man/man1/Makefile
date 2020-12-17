########################################################################
##                                                                    ##
##               M  a  k  e  f  i  l  e                               ##
##                                                                    ##
##   Makefile to create HTML files from man pages                     ##
##                                                                    ##
##   written by Werner von Bloh                                       ##
##   Potsdam Institute for Climate Impact Research                    ##
##   P.O. Box 60 12 03                                                ##
##   14412 Potsdam/Germany                                            ##
##                                                                    ##
##   Last change: $Date:: 2013-09-30 15:43:25 +0200 (lun., 30 sept.#$ ##
##   By         : $Author:: bloh                            $         ##
##                                                                    ##
########################################################################


SRC	= cat2bsq.1 lpj.1 lpjml.1 lpjshuffle.1 plotlpj.1 txt2grid.1\
          cft2clm.1 lpjcat.1 lpj_paths.csh.1 lpjsubmit.1 printheader.1\
          configure.sh.1 lpjcheck.1 lpj_paths.sh.1 newpft.sh.1 txt2clm.1\
          cru2clm.1 lpjdistribute.1 lpjprint.1 output_bsq.1 asc2clm.1\
          grid2clm.1 lpjdistribute_river.1 lpjrun.1 plotclimate.1\
          backtrace.1 lpjliveview.1 db2clm.1 catclm.1 printclm.1 adddrain.1\
          printreservoir.1 printharvest.1 setclm.1 lpjfiles.1 lpjsave.1\
          regridlpj.1 cdf2clm.1 clm2cdf.1 soil2cdf.1 cdf2soil.1 bin2cdf.1

HTMLDIR	= ../../html
HTML	= $(SRC:%.1=$(HTMLDIR)/%.html)

html: $(HTML)

$(HTMLDIR)/%.html: %.1
	nroff -man $*.1 |man2html -cgiurl '$$title.html' -title $*.1 >$@
clean:
	rm -f $(HTML)
