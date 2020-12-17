/***************************************************************************/
/**                                                                       **/
/**                       d  a  t  a  b  a  s  e  .  h                    **/
/**                                                                       **/
/**     Header file for functions accessing the PIK Oracle database.      **/
/**                                                                       **/
/**     written by Werner von Bloh                                        **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change:  09.12.2008                                          **/
/**                                                                       **/
/***************************************************************************/

#ifndef DATABASE_H
#define DATABASE_H

extern void db_connect(const char *,const char *);
extern void db_close(void);
extern void db_getclimate(float [],float [],float [],int,int,int);
extern int db_getncoords(void);
extern int db_getnscenarios(void);
extern void db_printscenarios(void);
extern void db_getscenarios(int [],int);
extern void db_getdescr(char *,int);
extern void db_getcoords(float *,float *,int);
extern void db_getyears(int *,int *,int);

#endif
