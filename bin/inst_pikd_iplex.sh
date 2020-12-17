###############################################################################
##                                                                           ##
##          i  n  s  t  _  p  i  k  d  _ i  p  l  e  x  .  s  h              ##
##                                                                           ##
##    sh script to set environment variables for Oracle database on the      ##
##    IdataPlex cluster at PIK                                               ##
##    script has to be put in ~/.profile in the following way:               ##
##                                                                           ##
##    . $LPJROOT/bin/inst_pikd_iplex.sh                                      ##
##                                                                           ##
##    LPJROOT has to be set to your root directory of LPJmL                  ##
##                                                                           ##
##    Written by Achim Glauer, PIK Potsdam                                   ##
##                                                                           ##
##    Created: 24.06.2009                                                    ##
##                                                                           ##
###############################################################################

ORACLE_BASE=/iplex/01/sys/applications/oracle/oracle_10
ORACLE_SID=pikd
ORACLE_HOME=$ORACLE_BASE/client
ORACLE_TERM=hft
TWO_TASK=pikd
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$ORACLE_HOME/lib
PATH=$ORACLE_HOME/bin:$PATH
export PATH ORACLE_SID ORACLE_HOME ORACLE_TERM ORACLE_BASE
export LD_LIBRARY_PATH TWO_TASK
