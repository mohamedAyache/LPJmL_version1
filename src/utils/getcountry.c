/***************************************************************************/
/**                                                                       **/
/**                  g  e  t  c  o  u  n  t  r  y  .  c                   **/
/**                                                                       **/
/**     Extract country from grid file                                    **/
/**                                                                       **/
/**     written by Werner von Bloh                                        **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: $Date::                                            $ **/
/**     By         : $Author::                                 $          **/
/**                                                                       **/
/***************************************************************************/

#undef USE_MPI
#include "lpj.h"

#define USAGE "Usage: %s [-list] [-longheader] countryfile gridfile outfile country ...\n"

#define NCOUNTRY 197 /* number of countries defined in LPJmL */

typedef struct
{
  int code;         /* LPJmL country code */
  const char *name; /* country name */
  char abbrev[4];   /* ISO 3166-1 alpha-3 abbreviation for country */
} Countryname;

static Countryname countrynames[NCOUNTRY]=
{
  {Afghanistan,"Afghanistan","AFG"},
  {Albania,"Albania","ALB"},
  {Algeria,"Algeria","DZA"},
  {American_Samoa,"American Samoa","ASM"},
  {Angola,"Angola","AGO"},
  {Argentina,"Argentina","ARG"},
  {Armenia,"Armenia","ARM"},
  {Austria,"Austria","AUT"},
  {Azerbaijan,"Azerbaijan","AZE"},
  {Bahamas_The,"Bahamas, The","BHS"},
  {Bangladesh,"Bangladesh","BGD"},
  {Belgium,"Belgium","BEL"},
  {Belize,"Belize","BLZ"},
  {Benin,"Benin","BEN"},
  {Bhutan,"Bhutan","BTN"},
  {Bolivia,"Bolivia","BOL"},
  {Bosnia_and_Herzegovina,"Bosnia and Herzegovina","BIH"},
  {Botswana,"Botswana","BWA"},
  {Brunei,"Brunei","BRN"},
  {Bulgaria,"Bulgaria","BGR"},
  {Burkina_Faso,"Burkina Faso","BFA"},
  {Burundi,"Burundi","BDI"},
  {Byelarus,"Byelarus","BLR"},
  {Cambodia,"Cambodia","KHM"},
  {Cameroon,"Cameroon","CMR"},
  {Cape_Verde,"Cape Verde","CPV"},
  {Cayman_Islands,"Cayman Islands","CYM"},
  {Central_African_Republic,"Central African Republic","CAF"},
  {Chad,"Chad","TCD"},
  {Chile,"Chile","CHL"},
  {Colombia,"Colombia","COL"},
  {Comoros,"Comoros","COM"},
  {Congo_Brazzaville,"Congo-Brazzaville","COG"},
  {Cook_Islands,"Cook Islands","COK"},
  {Costa_Rica,"Costa Rica","CRI"},
  {Croatia,"Croatia","HRV"},
  {Cuba,"Cuba","CUB"},
  {Cyprus,"Cyprus","CYP"},
  {Czech_Republic,"Czech Republic","CZE"},
  {Denmark,"Denmark","DNK"},
  {Djibouti,"Djibouti","DJI"},
  {Dominican_Republic,"Dominican Republic","DOM"},
  {Ecuador,"Ecuador","ECU"},
  {Egypt,"Egypt","EGY"},
  {El_Salvador,"El Salvador","SLV"},
  {Equatorial_Guinea,"Equatorial Guinea","GNQ"},
  {Eritrea,"Eritrea","ERI"},
  {Estonia,"Estonia","EST"},
  {Ethiopia,"Ethiopia","ETH"},
  {Falkland_Islands_or_Islas_Malvinas,"Falkland Islands or Islas Malvinas","FLK"},
  {Faroe_Islands,"Faroe Islands","FRO"},
  {Federated_States_of_Micronesia,"Federated States of Micronesia","FSM"},
  {Fiji,"Fiji","FJI"},
  {Finland,"Finland","FIN"},
  {France,"France","FRA"},
  {French_Guiana,"French Guiana","GUF"},
  {French_Polynesia,"French Polynesia","PYF"},
  {French_Southern_and_Antarctica_Lands,"French Southern and Antarctica Lands","NOC"},
  {Gabon,"Gabon","GAB"},
  {Gambia_The,"Gambia, The","GMB"},
  {Georgia,"Georgia","GEO"},
  {Germany,"Germany","DEU"},
  {Ghana,"Ghana","GHA"},
  {Greece,"Greece","GRC"},
  {Greenland,"Greenland","GRL"},
  {Guadeloupe,"Guadeloupe","GLP"},
  {Guatemala,"Guatemala","GTM"},
  {Guinea,"Guinea","GIN"},
  {Guinea_Bissau,"Guinea-Bissau","GNB"},
  {Guyana,"Guyana","GUY"},
  {Haiti,"Haiti","HTI"},
  {Honduras,"Honduras","HND"},
  {Hungary,"Hungary","HUN"},
  {Iceland,"Iceland","ISL"},
  {Indonesia,"Indonesia","IDN"},
  {Iran,"Iran","IRN"},
  {Iraq,"Iraq","IRQ"},
  {Ireland,"Ireland","IRL"},
  {Israel,"Israel","ISR"},
  {Italy,"Italy","ITA"},
  {Ivory_Coast,"Ivory Coast","CIV"},
  {Jamaica,"Jamaica","JAM"},
  {Japan,"Japan","JPN"},
  {Jordan,"Jordan","JOR"},
  {Kazakhstan,"Kazakhstan","KAZ"},
  {Kenya,"Kenya","KEN"},
  {Kiribati,"Kiribati","KIR"},
  {Kuwait,"Kuwait","KWT"},
  {Kyrgyzstan,"Kyrgyzstan","KGZ"},
  {Laos,"Laos","LAO"},
  {Latvia,"Latvia","LVA"},
  {Lebanon,"Lebanon","LBN"},
  {Lesotho,"Lesotho","LSO"},
  {Liberia,"Liberia","LBR"},
  {Libya,"Libya","LBY"},
  {Lithuania,"Lithuania","LTU"},
  {Luxembourg,"Luxembourg","LUX"},
  {Macedonia,"Macedonia","MKD"},
  {Madagascar,"Madagascar","MDG"},
  {Malawi,"Malawi","MWI"},
  {Malaysia,"Malaysia","MYS"},
  {Maldives,"Maldives","MDV"},
  {Mali,"Mali","MLI"},
  {Marshall_Islands,"Marshall Islands","MHL"},
  {Mauritania,"Mauritania","MRT"},
  {Mauritius,"Mauritius","MUS"},
  {Mexico,"Mexico","MEX"},
  {Moldova,"Moldova","MDA"},
  {Mongolia,"Mongolia","MNG"},
  {Montenegro,"Montenegro","MNE"},
  {Morocco,"Morocco","MAR"},
  {Mozambique,"Mozambique","MOZ"},
  {Myanmar_or_Burma,"Myanmar or Burma","MMR"},
  {Namibia,"Namibia","NAM"},
  {Nepal,"Nepal","NPL"},
  {Netherlands,"Netherlands","NLD"},
  {New_Caledonia,"New Caledonia","NCL"},
  {New_Zealand,"New Zealand","NZL"},
  {Nicaragua,"Nicaragua","NIC"},
  {Niger,"Niger","NER"},
  {Nigeria,"Nigeria","NGA"},
  {North_Korea,"North Korea","PRK"},
  {Northern_Mariana_Islands,"Northern Mariana Islands","MNP"},
  {Norway,"Norway","NOR"},
  {Oman,"Oman","OMN"},
  {Pakistan,"Pakistan","PAK"},
  {Panama,"Panama","PAN"},
  {Papua_New_Guinea,"Papua New Guinea","PNG"},
  {Paraguay,"Paraguay","PRY"},
  {Peru,"Peru","PER"},
  {Philippines,"Philippines","PHL"},
  {Pitcairn_Islands,"Pitcairn Islands","PCN"},
  {Poland,"Poland","POL"},
  {Portugal,"Portugal","PRT"},
  {Puerto_Rico,"Puerto Rico","PRI"},
  {Qatar,"Qatar","QAT"},
  {Reunion,"Reunion","REU"},
  {Romania,"Romania","ROU"},
  {Rwanda,"Rwanda","RWA"},
  {Sao_Tome_and_Principe,"Sao Tome and Principe","STP"},
  {Saudi_Arabia,"Saudi Arabia","SAU"},
  {Senegal,"Senegal","SEN"},
  {Serbia,"Serbia","SRB"},
  {Sierra_Leone,"Sierra Leone","SLE"},
  {Slovakia,"Slovakia","SVK"},
  {Slovenia,"Slovenia","SVN"},
  {Solomon_Islands,"Solomon Islands","SLB"},
  {Somalia,"Somalia","SOM"},
  {South_Africa,"South Africa","ZAF"},
  {South_Georgia_and_the_South_Sandwich_Islands,"South Georgia and the South Sandwich Islands","SGS"},
  {South_Korea,"South Korea","KOR"},
  {Spain,"Spain","ESP"},
  {Sri_Lanka,"Sri Lanka","LKA"},
  {St_Vincent_and_the_Grenadines,"St. Vincent and the Grenadines","VCT"},
  {Sudan,"Sudan","SDN"},
  {Suriname,"Suriname","SUR"},
  {Svalbard,"Svalbard","SJM"},
  {Swaziland,"Swaziland","SWZ"},
  {Sweden,"Sweden","SWE"},
  {Switzerland,"Switzerland","CHE"},
  {Syria,"Syria","SYR"},
  {Taiwan,"Taiwan","TWN"},
  {Tajikistan,"Tajikistan","TJK"},
  {Tanzania_United_Republic_of,"Tanzania, United Republic of","TZA"},
  {Thailand,"Thailand","THA"},
  {Togo,"Togo","TGO"},
  {Tokelau,"Tokelau","TKL"},
  {Trinidad_and_Tobago,"Trinidad and Tobago","TTO"},
  {Tunisia,"Tunisia","TUN"},
  {Turkey,"Turkey","TUR"},
  {Turkmenistan,"Turkmenistan","TKM"},
  {Tuvalu,"Tuvalu","TUV"},
  {Uganda,"Uganda","UGA"},
  {Ukraine,"Ukraine","UKR"},
  {United_Arab_Emirates,"United Arab Emirates","ARE"},
  {United_Kingdom,"United Kingdom","GBR"},
  {Uruguay,"Uruguay","URY"},
  {Uzbekistan,"Uzbekistan","UZB"},
  {Vanuatu,"Vanuatu","VUT"},
  {Venezuela,"Venezuela","VEN"},
  {Vietnam,"Vietnam","VNM"},
  {Virgin_Islands,"Virgin Islands","VGB"},
  {Wake_Islands,"Wake Islands","UMI"},
  {West_Bank,"West Bank","PSE"},
  {Western_Sahara,"Western Sahara","ESH"},
  {Western_Samoa,"Western Samoa","WSM"},
  {Yemen,"Yemen","YEM"},
  {Zaire_DR_Congo,"Zaire, DR Congo","COD"},
  {Zambia,"Zambia","ZMB"},
  {Zimbabwe,"Zimbabwe","ZWE"},
  {Australia,"Australia","AUS"},
  {Brazil,"Brazil","BRA"},
  {Canada,"Canada","CAN"},
  {China,"China","CHN"},
  {India,"India","IND"},
  {Russia,"Russia","RUS"},
  {United_States,"United States of America","USA"}
};

static int findcountryname(const char *name,
                           const Countryname countryname[],
                           int ncountries)
{
  int i;
  for(i=0;i<ncountries;i++)
    if(!strcmp(name,countryname[i].abbrev))
      return countryname[i].code;
  return NOT_FOUND;
} /* 'findcountryname' */

static int compare(const Countryname *a,const Countryname *b)
{
  return strcmp(a->name,b->name);
} /* of 'compare' */

static Bool findcountry(const int country[],int n,int c)
{
  int i;
  for(i=0;i<n;i++)
    if(country[i]==c)
      return TRUE;
  return FALSE;
} /* of 'findcountry' */

int main(int argc,char **argv)
{
  FILE *file,*grid,*out;
  int i,*country,n,version,country_version;
  char *endptr;
  Intcoord coord;
  Header header,gridheader,outheader;
  String headername;
  Code code;
  Bool swap_country,swap_grid;
  outheader.nyear=1;
  outheader.firstcell=0;
  outheader.order=0;
  outheader.ncell=0;
  outheader.nbands=2;
  version=country_version=READ_VERSION;
  if(argc>1 && !strcmp(argv[1],"-longheader"))
  {
    version=country_version=2;
    argc--;
    argv++;
  }
  if(argc>1 && !strcmp(argv[1],"-list"))
  {
    puts("List of country codes:\nCode Num Name");
    qsort(countrynames,NCOUNTRY,sizeof(Countryname),
          (int (*)(const void *,const void *))compare);
    for(i=0;i<NCOUNTRY;i++)
      printf("%s  %3d %s\n",countrynames[i].abbrev,countrynames[i].code,
             countrynames[i].name);
    return EXIT_SUCCESS;
  }
  if(argc<5)
  {
    fprintf(stderr,"Argument(s) missing.\n"
            USAGE,argv[0]);
    return EXIT_FAILURE;
  }
  file=fopen(argv[1],"rb");
  if(file==NULL)
  {
    fprintf(stderr,"Error opening '%s': %s.\n",argv[1],strerror(errno));
    return EXIT_FAILURE;
  }
  if(freadanyheader(file,&header,&swap_country,headername,&country_version))
  {
    fprintf(stderr,"Error reading header of '%s'.\n",argv[1]);
    return EXIT_FAILURE;
  }
  grid=fopen(argv[2],"rb");
  if(grid==NULL)
  {
    fprintf(stderr,"Error opening '%s': %s.\n",argv[2],strerror(errno));
    return EXIT_FAILURE;
  }
  if(freadheader(grid,&gridheader,&swap_grid,LPJGRID_HEADER,&version))
  {
    fprintf(stderr,"Error reading header of '%s'.\n",argv[2]);
    return EXIT_FAILURE;
  }
  outheader.firstyear=gridheader.firstyear;
  if(version==2)
  {
    outheader.cellsize=gridheader.cellsize;
    outheader.scalar=gridheader.scalar;
  }
  n=argc-4;
  country=newvec(int,n);
  check(country);
  for(i=0;i<n;i++)
  {
    country[i]=strtol(argv[4+i],&endptr,10);
    if(*endptr!='\0')
    {
      /* argument is not a number */
      country[i]=findcountryname(argv[4+i],countrynames,NCOUNTRY);
      if(country[i]==NOT_FOUND)
      {
        fprintf(stderr,"Invalid number/name '%s' for country.\n",argv[4+i]);
        return EXIT_FAILURE;
      }
    }
    else if(country[i]<0 || country[i]>=NCOUNTRY)
    {
      fprintf(stderr,"Invalid number %d for country.\n",country[i]);
      return EXIT_FAILURE;
    }
  }
  out=fopen(argv[3],"wb");
  if(out==NULL)
  {
    fprintf(stderr,"Error creating '%s': %s.\n",argv[3],strerror(errno));
    return EXIT_FAILURE;
  }
  fwriteheader(out,outheader,LPJGRID_HEADER,version);
  for(i=0;i<header.ncell;i++)
  {
    if(readcountrycode(file,&code,swap_country))
    {
      fprintf(stderr,"Error reading country code at %d.\n",i+1);
      return EXIT_FAILURE;
    }
    if(readintcoord(grid,&coord,swap_grid))
    {
      fprintf(stderr,"Error reading coordinate at %d.\n",i+1);
      return EXIT_FAILURE;
    }
    if(findcountry(country,n,code.country))
    {
      if(fwrite(&coord,sizeof(coord),1,out)!=1)
      {
        fprintf(stderr,"Error writing coordinate at %d.\n",i+1);
        return EXIT_FAILURE;
      }
      outheader.ncell++;
    }
  }
  fclose(file);
  fclose(grid);
  /* write actual number of cells into header */
  rewind(out);
  fwriteheader(out,outheader,LPJGRID_HEADER,version);
  fclose(out);
  if(header.ncell)
    printf("Number of cells: %d\n",outheader.ncell);
  else
    fputs("Warning: no cells found.\n",stderr);
  return EXIT_SUCCESS;
} /* of 'main' */
