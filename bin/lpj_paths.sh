###############################################################################
##                                                                           ##
##          l  p  j  _  p  a  t  h  s  .  s  h                               ##
##                                                                           ##
##    sh script to set environment variables for LPJmL. A call to this       ##
##    script has to be put in ~/.profile in the following way:               ##
##                                                                           ##
##    . $LPJROOT/bin/lpj_paths.sh                                            ##
##                                                                           ##
##    LPJROOT has to be set to your root directory of LPJmL                  ##
##                                                                           ##
##    Written by Werner von Bloh, PIK Potsdam                                ##
##                                                                           ##
##    Created: 30.10.2018                                                    ##
##                                                                           ##
###############################################################################

export LPJROOT=/home/alberte/SVN/SVN_LaserMed_5mn/LPJml_AgTree # change here to your directory

# set search path for LPJmL commands

export PATH=$PATH:$LPJROOT/bin

# set path for input files

export LPJINPATH=

# include manpages of LPJmL

export MANPATH=$MANPATH:$LPJROOT/man

# define alias

alias printheader="printclm -data"
