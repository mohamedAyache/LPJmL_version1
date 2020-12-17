###############################################################################
##                                                                           ##
##                i  n  s  t  _  p  i  k  d  _ a  i  x  .  s  h              ##
##                                                                           ##
##    sh script to set environment variables for Oracle database on the      ##
##    IdataPlex cluster at PIK                                               ##
##    script has to be put in ~/.profile in the following way:               ##
##                                                                           ##
##    . $LPJROOT/bin/inst_pikd_aix.sh                                        ##
##                                                                           ##
##    LPJROOT has to be set to your root directory of LPJmL                  ##
##                                                                           ##
##    Written by Achim Glauer, PIK Potsdam                                   ##
##                                                                           ##
##    Created: 04.05.2010                                                    ##
##                                                                           ##
###############################################################################

ORACLE_BASE=/usr/local/oracle8/oracle10/10.2.0
ORACLE_SID=pikd
ORACLE_HOME=$ORACLE_BASE
ORACLE_TERM=hft
TWO_TASK=pikd
LIBPATH=$LIBPATH:$ORACLE_HOME/lib
PATH=$ORACLE_HOME/bin:$PATH
export PATH ORACLE_SID ORACLE_HOME ORACLE_TERM ORACLE_BASE
export LIBPATH TWO_TASK
