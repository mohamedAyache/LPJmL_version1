/***************************************************************************/
/**                                                                       **/
/**                     t  y  p  e  s  .  h                               **/
/**                                                                       **/
/**     Definition of basic datatypes and macros                          **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: 19.08.2009 by Christoph Mueller & Elke Stehfest      **/
/**     Last change: $Date:: 2014-09-29 16:02:37 +0200 (lun. 29 sept. 2#$ **/
/**     By         : $Author:: bloh                            $          **/
/**                                                                       **/
/***************************************************************************/

#ifndef TYPES_H /* Already included? */
#define TYPES_H

#include <time.h>

/* Definition of constants */

#define epsilon 1.0E-6 /* a minimal value -- check if neglegible */
#define NOT_FOUND -1
#ifndef TRUE    /* Check whether TRUE or FALSE are already defined */
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif

#define STRING_LEN 255 /* Maximum length of default strings  */

#ifdef _WIN32
#define isnan(x) (x!=x)
#define snprintf sprintf_s
#define strdup _strdup
#define fileno _fileno
#define unlink _unlink
#define pclose _pclose
#define popen _popen
#define alloca _alloca

#endif
 
#ifdef __INTEL_COMPILER
/* #pragma warning ( disable : 869 )*/ /* disable warning about unused parameter */
#endif

/* Definition of datatypes */

typedef double Real; /* Default floating point type in LPJ */

typedef int Bool; /* type boolean (TRUE/FALSE) */

typedef unsigned char Byte;

typedef char String[STRING_LEN+1];

/* Declaration of functions */

extern void fail(int,Bool,const char *,...);
extern Bool fscanreal(FILE *,Real *,const char *,Bool);
extern Bool fscanstring(FILE *,String);
extern Bool fscantoken(FILE *,String);
extern char *sysname(void);
extern char *getpath(const char *);
extern char *gethost(void);
extern char *getuser(void);
extern char *getdir(void);
extern void banner(const char * const *,int,int);
extern void frepeatch(FILE *,char,int);
extern Bool isabspath(const char *);
extern char *addpath(const char *,const char *);
extern void printflags(const char *);
extern long long getfilesize(const char *name);
extern const char *strippath(const char *);
extern long long diskfree(const char *);
extern void fprintintf(FILE *,int);
extern void getcounts(int [],int [],int,int,int);
extern int getbuildnumber(void);
extern char *getbuilddate(void);
extern time_t getfiledate(const char *);
extern char *stripsuffix(const char *);
extern Bool hassuffix(const char *,const char *);
extern char *mkfilename(const char *);
extern int findstr(const char *,const char *const *,int);
extern Bool checkfmt(const char *,char);
extern int fputstring(FILE *,int,const char *,int);
extern Bool fscanint(FILE *,int *,const char *,Bool);
extern Bool fscanfloat(FILE *,float *,const char *,Bool);
extern int getsvnrev(void);
extern void fprinttime(FILE *,int);
#ifdef WITH_FPE
extern void enablefpe(void);
#endif

/* Definition of data type */

typedef enum {LPJ_BYTE,LPJ_SHORT,LPJ_INT,LPJ_FLOAT,LPJ_DOUBLE} Type;

typedef struct
{
  char *name; /* name of file */
  char *var;  /* name of variable in NetCDF file or NULL */
  int fmt;    /* format (TXT/RAW/CLM/CDF) */
} Filename;

/* Definitions of macros */

#ifdef UNUSED
#elif defined(__GNUC__)
# define UNUSED(x) UNUSED_ ## x __attribute__((unused))
#elif defined(__LCLINT__)
# define UNUSED(x) /*@unused@*/ x
#else
# define UNUSED(x) x
#endif

#define repeatch(c,count) frepeatch(stdout,c,count)
#define newvec(type,size) (type *)malloc(sizeof(type)*(size))
#define newvec2(type,lo,hi) (type *)malloc(sizeof(type)*(hi-(lo)+1))-(lo)
#define freevec(ptr,lo,hi) free(ptr+lo)
#define check(ptr) if((ptr)==NULL) fail(ALLOC_MEMORY_ERR,FALSE,"Cannot allocate memory for '%s' in %s()",#ptr,__FUNCTION__)
#define printintf(i) fprintintf(stdout,i)

#define new(type) (type *)malloc(sizeof(type))
#ifndef min         /* min macro defined? */
#define min(a,b) (((a)<(b)) ?  (a) : (b))
#endif
#ifndef max         /* max macro defined? */
#define max(a,b) (((a)>(b)) ?  (a) : (b))
#endif
#define fwriten(var,size,n,file) if(fwrite(var,size,n,file)!=n) return TRUE
#define fwrite1(var,size,file) fwriten(var,size,1,file)
#define bool2str(b) ((b) ? "true" : "false")
#define printtime(s) fprinttime(stdout,s)

#endif /* of TYPES_H */
