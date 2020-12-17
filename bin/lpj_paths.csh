###############################################################################
##                                                                           ##
##          l  p  j  _  p  a  t  h  s  .  c  s  h                            ##
##                                                                           ##
##    csh script to set environment variables for LPJmL. A call to this      ##
##    script has to be put in ~/.cshrc in the following way:                 ##
##                                                                           ##
##    source $LPJROOT/bin/lpj_paths.csh                                     ##
##                                                                           ##
##    LPJROOT has to be set to your root directory of LPJmL                  ##
##                                                                           ##
##    Written by Werner von Bloh, PIK Potsdam                                ##
##                                                                           ##
##    Created: 30.10.2018                                                    ##
##                                                                           ##
###############################################################################

setenv LPJROOT /home/alberte/SVN/SVN_LaserMed_5mn/LPJml_AgTree # change here to your directory

# set search path for LPJmL commands

setenv PATH $PATH:$LPJROOT/bin

# set path for input files

setenv LPJINPATH 

# include manpages of LPJmL

setenv MANPATH $MANPATH:$LPJROOT/man
