/***************************************************************************/
/**                                                                       **/
/**                   o  p  e  n  c  o  n  f  i  g  .  c                  **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     Function opens LPJ configuration file                             **/
/**     Input is prepocessed by cpp or by program defined in the          **/
/**     LPJPREP environment variable                                      **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: 27.02.2013                                           **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"

#ifdef _WIN32              /* are we on a Windows machine? */
#define cpp_cmd "cl /EP /nologo"  /* C preprocessor command for Windows */
#else
#define cpp_cmd "cpp -P"  /* C preprocessor command for Unix */
#endif

FILE *openconfig(Config *config,      /* configuration struct */
                 const char *dflt_filename, /* default name of configuration file */
                 int *argc,           /* pointer to the number of arguments */
                 char ***argv,        /* pointer to the argument vector  */
                 const char *usage    /* usage information string or NULL */
                )                     /* returns file pointer of open file or NULL */

{
  char *cmd,*lpjpath,*lpjinc,*filter,*env_options,*pos;
  char **options;
  char *endptr;
  Bool iscpp;
  FILE *file;
  int i,len,dcount;
  filter=getenv(LPJPREP);
  if(filter==NULL)
  {
    filter=cpp_cmd;
    iscpp=TRUE;
  }
  else
    iscpp=FALSE;
  config->outputdir=getenv(LPJOUTPUT);
  config->inputdir=getenv(LPJINPUT);
  config->restartdir=getenv(LPJRESTART);
  env_options=getenv(LPJOUTPUTMETHOD);
  config->port=DEFAULT_PORT;
  config->param_out=FALSE;
#ifdef IMAGE
  config->image_inport=DEFAULT_IMAGE_INPORT;
  config->image_outport=DEFAULT_IMAGE_OUTPORT;
  config->image_host=getenv(LPJIMAGE);
  pos=getenv(LPJWAITIMAGE);
  if(pos!=NULL)
  {
    config->wait_image=strtol(pos,&endptr,10);
    if(*endptr!='\0')
    {
      if(isroot(*config))
        fprintf(stderr,"ERROR193: Invalid number for IMAGE wait in environment variable.\n");
      return NULL;
    }
  }
  else
    config->wait_image=WAIT_IMAGE;
  if(config->image_host==NULL)
    config->image_host=DEFAULT_IMAGE_HOST;
  else
  {
    pos=strstr(config->image_host,":");
    if(pos!=NULL)
    {
      *pos='\0';
       config->image_inport=strtol(pos+1,&endptr,10);
       if(pos+1==endptr || config->image_inport<1 || config->image_inport>65535)
       {
         if(isroot(*config))
           fprintf(stderr,"ERROR193: Invalid number for IMAGE input port.\n");
         return NULL;
       }
       if(*endptr==',')
       {
         pos=endptr;
         config->image_outport=strtol(endptr+1,&endptr,10);
         if(pos+1==endptr || *endptr!='\0' || config->image_outport<1
                          || config->image_outport>65535)
         {
           if(isroot(*config))
             fprintf(stderr,
                     "ERROR194: Invalid number for IMAGE output port.\n");
           return NULL;
         }
       }
    }
  }
#endif
  if(env_options==NULL)
#ifdef USE_MPI
    config->outputmethod=LPJ_GATHER;
#else
    config->outputmethod=LPJ_FILES;
#endif
#ifdef USE_MPI
  else if(!strcmp(env_options,"mpi2"))
    config->outputmethod=LPJ_MPI2;
  else if(!strcmp(env_options,"gather"))
    config->outputmethod=LPJ_GATHER;
#else
  else if(!strcmp(env_options,"write"))
    config->outputmethod=LPJ_FILES;
#endif
  else if(!strncmp(env_options,"socket",6))
  {
    config->outputmethod=LPJ_SOCKET;
    if(env_options[6]=='=')
    {
      config->hostname=env_options+7;
      pos=strstr(config->hostname,":");
      if(pos!=NULL)
      {
        *pos='\0';
        config->port=strtol(pos+1,&endptr,10);
        if(endptr==pos+1 || *endptr!='\0' || config->port<1  || config->port>65535)
        {
          if(isroot(*config))
            fprintf(stderr,
                    "ERROR169: Invalid port number for output method socket.\n");
          return NULL;
        }
      }
    }
    else
    {
      if(isroot(*config))
      {
        fprintf(stderr,
                "ERROR168: Hostname missing for output method socket.\n");
        if(usage!=NULL)
          fprintf(stderr,usage,(*argv)[0]);
      }
      return NULL;
    }
  }
  else
  {
    if(isroot(*config))
    {
      fprintf(stderr,"ERROR163: Invalid output method '%s'.\n",env_options);
      if(usage!=NULL)
        fprintf(stderr,usage,(*argv)[0]);
    }
    return NULL;
  }
  env_options=getenv(LPJOPTIONS);
  options=newvec(char *,(env_options==NULL) ? *argc : *argc+1);
  check(options);
  dcount=0;
  len=1;
  /* parse command line arguments */
  for(i=1;i<*argc;i++)
  {
    if((*argv)[i][0]=='-') /* check whether option is given */
    {
      if((*argv)[i][1]=='D' || (*argv)[i][1]=='I')
      {
        /* only '-D' (Define) or '-I' (Include) options are allowed */
        options[dcount++]=(*argv)[i];
        len+=strlen((*argv)[i])+1;
      }
#ifdef IMAGE
      else if(!strcmp((*argv)[i],"-wait"))
      {
        if(i==*argc-1)
        {
          if(isroot(*config))
          {
            fprintf(stderr,"ERROR164: Argument missing for '-wait' option.\n");
            if(usage!=NULL)
              fprintf(stderr,usage,(*argv)[0]);
          }
          free(options);
          return NULL;
        }
        else
        {
          config->wait_image=strtol((*argv)[++i],&endptr,10);
          if(*endptr!='\0')
          {
            if(isroot(*config))
              fprintf(stderr,"ERROR193: Invalid number '%s' for IMAGE wait in option.\n",(*argv)[i]);
            free(options);
            return NULL;
          }
        }
      }
      else if(!strcmp((*argv)[i],"-pp"))
      {
        if(i==*argc-1)
        {
          if(isroot(*config))
          {
            fprintf(stderr,"ERROR164: Argument missing for '-pp' option.\n");
            if(usage!=NULL)
              fprintf(stderr,usage,(*argv)[0]);
          }
          free(options);
          return NULL;
        }
        filter=(*argv)[++i];
        iscpp=FALSE;
      }
      else if(!strcmp((*argv)[i],"-image"))
      {
        if(i==*argc-1)
        {
          if(isroot(*config))
          {
            fprintf(stderr,"ERROR164: Argument missing for '-image' option.\n");
            if(usage!=NULL)
              fprintf(stderr,usage,(*argv)[0]);
          }
          free(options);
          return NULL;
        }
        else
        {
           config->image_host=(*argv)[++i];
           pos=strstr(config->image_host,":");
           if(pos!=NULL)
           {
             *pos='\0';
             config->image_inport=strtol(pos+1,&endptr,10);
             if(pos+1==endptr || config->image_inport<1
                              || config->image_inport>65535)
             {
               if(isroot(*config))
                 fprintf(stderr,
                         "ERROR193: Invalid number for IMAGE input port.\n");
               free(options);
               return NULL;
             }
             if(*endptr==',')
             {
               pos=endptr;
               config->image_outport=strtol(endptr+1,&endptr,10);
               if(endptr==pos+1 || *endptr!='\0' || config->image_outport<1
                                || config->image_outport>65535)
               {
                 if(isroot(*config))
                   fprintf(stderr,"ERROR194: Invalid number for IMAGE output port.\n");
                 free(options);
                 return NULL;
               }
             }
           }
        }
      }
#endif
      else if(!strcmp((*argv)[i],"-param"))
        config->param_out=TRUE;
      else if(!strcmp((*argv)[i],"-inpath"))
      {
        if(i==*argc-1)
        {
          if(isroot(*config))
          {
            fprintf(stderr,
                    "ERROR164: Argument missing for '-inpath' option.\n");
            if(usage!=NULL)
              fprintf(stderr,usage,(*argv)[0]);
          }
          free(options);
          return NULL;
        }
        else
           config->inputdir=(*argv)[++i];
      }
      else if(!strcmp((*argv)[i],"-outpath"))
      {
        if(i==*argc-1)
        {
          if(isroot(*config))
          {
            fprintf(stderr,
                    "ERROR164: Argument missing for '-outpath' option.\n");
            if(usage!=NULL)
              fprintf(stderr,usage,(*argv)[0]);
          }
          free(options);
          return NULL;
        }
        else
           config->outputdir=(*argv)[++i];
      }
      else if(!strcmp((*argv)[i],"-restartpath"))
      {
        if(i==*argc-1)
        {
          if(isroot(*config))
          {
            fprintf(stderr,"ERROR164: Argument missing for '-restartpath' option.\n");
            if(usage!=NULL)
              fprintf(stderr,usage,(*argv)[0]);
          }
          free(options);
          return NULL;
        }
        else
           config->restartdir=(*argv)[++i];
      }
#ifdef WITH_FPE
      else if(!strcmp((*argv)[i],"-fpe"))
        /* enable floating point exceptions, core file will be generated */
        enablefpe();
#endif
      else if(!strcmp((*argv)[i],"-output"))
      {
        if(i==*argc-1)
        {
          if(isroot(*config))
          {
            fprintf(stderr,
                    "ERROR164: Argument missing for '-output' option.\n");
            if(usage!=NULL)
              fprintf(stderr,usage,(*argv)[0]);
          }
          free(options);
          return NULL;
        }
#ifdef USE_MPI
        else if(!strcmp((*argv)[i+1],"mpi2"))
          config->outputmethod=LPJ_MPI2;
        else if(!strcmp((*argv)[i+1],"gather"))
          config->outputmethod=LPJ_GATHER;
#else
        else if(!strcmp((*argv)[i+1],"write"))
          config->outputmethod=LPJ_FILES;
#endif
        else if(!strncmp((*argv)[i+1],"socket",6))
        {
          config->outputmethod=LPJ_SOCKET;
          if((*argv)[i+1][6]=='=')
          {
            config->hostname=&((*argv)[i+1][7]);
            pos=strstr(config->hostname,":");
            if(pos!=NULL)
            {
              *pos='\0';
              config->port=strtol(pos+1,&endptr,10);
              if(endptr==pos+1 || *endptr!='\0' || config->port<1
                               || config->port>65535)
              {
                if(isroot(*config))
                  fprintf(stderr,"ERROR169: Invalid port number for output method socket.\n");
                free(options);
                return NULL;
              }
            }
          }
          else
          {
            if(isroot(*config))
            {
              fprintf(stderr,"ERROR168: Hostname missing for output method socket.\n");
              if(usage!=NULL)
                fprintf(stderr,usage,(*argv)[0]);
            }
            free(options);
            return NULL;
          }
        }
        else
        {
          if(isroot(*config))
          {
            fprintf(stderr,"ERROR163: Invalid output method '%s'.\n",
                    (*argv)[i+1]);
            if(usage!=NULL)
              fprintf(stderr,usage,(*argv)[0]);
          }
          free(options);
          return NULL;
        }
        i++;
      }
      else
      {
        if(isroot(*config))
        {
          fprintf(stderr,"ERROR162: Invalid option '%s'.\n",(*argv)[i]);
          if(usage!=NULL)
            fprintf(stderr,usage,(*argv)[0]);
        }
        free(options);
        return NULL;
      }
    }
    else
      break;
  }
  config->filename=(i==*argc)  ? dflt_filename : (*argv)[i++];
  /* adjust argc and argv */
  *argv+=i;
  *argc-=i;
  lpjpath=getenv(LPJROOT);
  if(lpjpath==NULL || !iscpp) /* Is LPJROOT environment variable defined? */
  { /* no */
    if(isroot(*config))
      fprintf(stderr,"WARNING019: Environment variable '%s' not set.\n",
              LPJROOT);
    lpjpath=NULL;
  }
  else
  { /* yes, include LPJROOT directory in search path for includes */
    lpjinc=malloc(strlen(lpjpath)+3);
    options[dcount++]=strcat(strcpy(lpjinc,"-I"),lpjpath);
    len+=strlen(lpjinc)+1;
  }
  if(env_options!=NULL)
  {
    options[dcount++]=env_options;
    len+=strlen(env_options)+1;
  }
  len+=strlen(filter);
  if(config->rank!=0)
#ifdef _WIN32
    len+=strlen(" 2>nul:");
#else
    len+=strlen(" 2>/dev/null");
#endif
  cmd=malloc(strlen(config->filename)+len+1);
  strcat(strcpy(cmd,filter)," ");
  /* concatenate options for cpp command */
  for(i=0;i<dcount;i++)
    strcat(strcat(cmd,options[i])," ");
  strcat(cmd,config->filename);
  if(config->rank!=0)
#ifdef _WIN32
    strcat(cmd," 2>nul:"); /* only task 0 sends error messages */
#else
    strcat(cmd," 2>/dev/null"); /* only task 0 sends error messages */
#endif
  if(lpjpath!=NULL)
    free(lpjinc);
  file=popen(cmd,"r"); /* open pipe, output of cpp goes to file */
  free(cmd);
  if(file==NULL)
  {
    if(isroot(*config))
      printfopenerr(config->filename);
    free(options);
    return NULL;
  }
  if(isroot(*config))
  {
    len=printf("Reading configuration from '%s'",config->filename);
    if(dcount>1 || (dcount && lpjpath==NULL))
    {
      len+=printf(" with options ");
      if(lpjpath!=NULL)
        dcount--;
      for(i=0;i<dcount;i++)
      {
        if(i)
          len+=printf(",");
        if(strlen(options[i])+len+2>78)
        {
          len=0;
          printf("\n");
        }
        len+=printf("'%s'",options[i]);
      }
    }
    printf(".\n");
  }
  free(options);
  return file;
} /* of 'openconfig' */
