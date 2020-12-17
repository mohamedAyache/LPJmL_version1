/***************************************************************************/
/**                                                                       **/
/**                       b  u  f  f  e  r  .  h                          **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran version         **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: 05.05.2011                                           **/
/**                                                                       **/
/***************************************************************************/

#ifndef BUFFER_H
#define BUFFER_H

/* Definitions of datatypes */

typedef struct buffer *Buffer;

/* Declaration of functions */

extern Buffer newbuffer(int);
extern void updatebuffer(Buffer,Real); 
extern Bool fwritebuffer(FILE *,const Buffer);
extern Buffer freadbuffer(FILE *,Bool);
extern void freebuffer(Buffer);
extern Real getbufferavg(const Buffer);
extern Bool isemptybuffer(const Buffer);

#endif
