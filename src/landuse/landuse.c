/***************************************************************************/
/**                                                                       **/
/**                           l  a  n  d  u  s  e  .  c                   **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     Function initializes landuse datatype                             **/
/**     - opens the landuse input file (see also cfts26_lu2clm.c)         **/
/**     - sets the landuse variables (see also landuse.h)                 **/
/**     - order of landuse input data:                                    **/
/**        0-11   CFT                                                     **/
/**       12-14   AGRICULTURE TREES                                       **/
/**          15   OTHERS                                                  **/
/**          16   PASTURES                                                **/
/**          17   BIOMASS_GRASS                                           **/
/**          18   BIOMASS_TREE                                            **/
/**       19-30   CFT_irr                                                 **/
/**       31-33   AGRICULTURE TREES IRR                                   **/
/**          34   others_irr                                              **/
/**          35   PASTURE_irr                                             **/
/**          36   BIOMASS_GRASS IRR                                       **/
/**          37   BIOMASS_TREE IRR                                        **/
/**     - called in iterate()                                             **/
/**     - reads every year the fractions of the bands for all cells from  **/
/**       the input file                                                  **/
/**     - checks if sum of fraction is not greater 1                      **/
/**       -> if sum of fraction is greater 1: subtraction from fraction   **/
/**          of managed grass if possible                                 **/
/**       -> else fail incorrect input file                               **/
/**                                                                       **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for Landuse Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: $Date:: 2015-07-24 15:27:51 +0200 (ven., 24 juil. #$ **/
/**     By         : $Author:: bloh                            $          **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"
#include "natural.h"
#include "agriculture.h"
#include "grassland.h"

struct landuse
{
    Bool intercrop;      /* intercropping possible (TRUE/FALSE) */
    Bool allcrops;       /* all crops establish (TRUE/FALSE) */
    int nbands;          /* number of data items per cell */
    int nbands_sdate;    /* number of data items per cell for sowing dates */
    Bool isbiomass;
    Bool isagtree;
    Bool isirrigsystem;
    Climatefile landuse; /* file pointer */
    Climatefile sdate;   /* file pointer to prescribed sdates */
};                     /* definition of opaque datatype Landuse */

Landuse initlanduse(int ncft,            /* number of crop PFTs */
        const Config *config /* LPJ configuration */
        )                     /* returns allocated landuse or NULL */
{
    Header header;
    String headername;
    int version;
    Landuse landuse;
    landuse=new(struct landuse);
    if(landuse==NULL)
        return NULL;
    landuse->allcrops=(config->withlanduse==ALL_CROPS);
    landuse->landuse.fmt=config->landuse_filename.fmt;
    if(landuse->allcrops)
        landuse->landuse.file=NULL;
    else
    {
        if(config->landuse_filename.fmt==CDF)
        {
            if(opendata_netcdf(&landuse->landuse,config->landuse_filename.name,config->landuse_filename.var,NULL,config))
            {
                free(landuse);
                return NULL;
            }
            if(landuse->landuse.var_len==2*(ncft+config->nagtree+NGRASS+NBIOMASSTYPE))
            {
                if(isroot(*config))
                    fputs("WARNING024: Land-use input does not include irrigation systems, suboptimal country values are used.\n",stderr);
                landuse->isbiomass=landuse->isagtree=TRUE;
                landuse->isirrigsystem=FALSE;
            }
            else if(landuse->landuse.var_len==2*(ncft+NGRASS+NBIOMASSTYPE))
            {
                if(isroot(*config))
                {
                    fputs("WARNING024: Land-use input does not include irrigation systems, suboptimal country values are used.\n",stderr);
                    fputs("WARNING022: No landuse for agriculture trees defined.\n",stderr);
                }
                landuse->isbiomass=TRUE;
                landuse->isirrigsystem=landuse->isagtree=FALSE;
            }
            else if(landuse->landuse.var_len==2*(ncft+NGRASS))
            {
                if(isroot(*config))
                {
                    fputs("WARNING024: Land-use input does not include irrigation systems, suboptimal country values are used.\n",stderr);
                    fputs("WARNING022: No landuse for agriculture trees and biomass defined.\n",stderr);
                }
                landuse->isirrigsystem=landuse->isagtree=landuse->isbiomass=FALSE;
            }
            else if(landuse->landuse.var_len==4*(ncft+NGRASS+NBIOMASSTYPE))
            {
                if(isroot(*config))
                    fputs("WARNING022: No landuse for agriculture trees defined.\n",stderr);
                landuse->isbiomass=TRUE;
                landuse->isirrigsystem=TRUE;
                landuse->isagtree=FALSE;
            }
            else if(landuse->landuse.var_len!=4*(ncft+NGRASS+NBIOMASSTYPE+config->nagtree))
            {
                closeclimate_netcdf(&landuse->landuse,isroot(*config));
                if(isroot(*config))
                    fprintf(stderr,
                            "ERROR147: Invalid number of bands=%d in landuse data file.\n",
                            (int)landuse->landuse.var_len);
                free(landuse);
                return NULL;
            }
            else
                landuse->isirrigsystem=landuse->isagtree=landuse->isbiomass=TRUE;
        }
        else
        {
            if((landuse->landuse.file=openinputfile(&header,&landuse->landuse.swap,
                            &config->landuse_filename,
                            headername,
                            &version,config))==NULL)
            {
                free(landuse);
                return NULL;
            }
            if(config->landuse_filename.fmt==RAW)
            {
                header.nbands=4*(ncft+config->nagtree+NGRASS+NBIOMASSTYPE);
                landuse->landuse.offset=config->startgrid*header.nbands*sizeof(short);
                landuse->isirrigsystem=landuse->isagtree=landuse->isbiomass=TRUE;
            }
            else
            {
                if(header.nbands==2*(ncft+config->nagtree+NGRASS+NBIOMASSTYPE))
                {
                    if(isroot(*config))
                        fputs("WARNING024: Land-use input does not include irrigation systems, suboptimal country values are used.\n",stderr);
                    landuse->isbiomass=landuse->isagtree=TRUE;
                    landuse->isirrigsystem=FALSE;
                }
                else if(header.nbands==2*(ncft+NGRASS+NBIOMASSTYPE))
                {
                    if(isroot(*config))
                    {
                        fputs("WARNING024: Land-use input does not include irrigation systems, suboptimal country values are used.\n",stderr);
                        fputs("WARNING022: No landuse for agriculture trees defined.\n",stderr);
                    }
                    landuse->isbiomass=TRUE;
                    landuse->isirrigsystem=landuse->isagtree=FALSE;
                }
                else if(header.nbands==2*(ncft+NGRASS))
                {
                    if(isroot(*config))
                    {
                        fputs("WARNING024: Land-use input does not include irrigation systems, suboptimal country values are used.\n",stderr);
                        fputs("WARNING022: No landuse for agriculture trees and biomass defined.\n",stderr);
                    }
                    landuse->isirrigsystem=landuse->isagtree=landuse->isbiomass=FALSE;
                }
                else if(header.nbands==4*(ncft+NGRASS+NBIOMASSTYPE))
                {
                    if(isroot(*config))
                        fputs("WARNING022: No landuse for agriculture trees defined.\n",stderr);
                    landuse->isbiomass=TRUE;
                    landuse->isirrigsystem=TRUE;
                    landuse->isagtree=FALSE;
                }
                else if(header.nbands!=4*(ncft+NGRASS+NBIOMASSTYPE+config->nagtree))
                {
                    fclose(landuse->landuse.file);
                    if(isroot(*config))
                        fprintf(stderr,
                                "ERROR147: Invalid number of bands=%d in landuse data file.\n",
                                header.nbands);
                    free(landuse);
                    return NULL;
                }
                else
                    landuse->isirrigsystem=landuse->isagtree=landuse->isbiomass=TRUE;
                landuse->landuse.offset=(config->startgrid-header.firstcell)*header.nbands*sizeof(short)+headersize(headername,version);
            }
            landuse->landuse.firstyear=header.firstyear;
            landuse->landuse.nyear=header.nyear;
            landuse->landuse.size=header.ncell*header.nbands*sizeof(short);
            landuse->landuse.n=config->ngridcell*header.nbands;
            landuse->nbands=header.nbands;
            landuse->landuse.scalar=(version==1) ? 0.001 : header.scalar;
        }
    }

    if(config->sdate_option==PRESCRIBED_SDATE)
    {
        landuse->sdate.fmt=config->sdate_filename.fmt;
        if(config->sdate_filename.fmt==CDF)
        {
            if(opendata_netcdf(&landuse->sdate,config->sdate_filename.name,config->sdate_filename.var,NULL,config))
            {
                if(landuse->landuse.fmt==CDF)
                    closeclimate_netcdf(&landuse->landuse,isroot(*config));
                else
                    fclose(landuse->landuse.file);
                free(landuse);
                return NULL;
            }
            if(landuse->sdate.var_len!=2*ncft)
            {
                closeclimate_netcdf(&landuse->sdate,isroot(*config));
                if(isroot(*config))
                    fprintf(stderr,
                            "ERROR147: Invalid number of bands=%d in sowing date data file.\n",
                            (int)landuse->sdate.var_len);
                if(landuse->landuse.fmt==CDF)
                    closeclimate_netcdf(&landuse->landuse,isroot(*config));
                else
                    fclose(landuse->landuse.file);
                free(landuse);
                return NULL;
            }
        }
        else
        {
            if((landuse->sdate.file=openinputfile(&header,&landuse->sdate.swap,
                            &config->sdate_filename,headername,
                            &version,config))==NULL)
            {
                fclose(landuse->landuse.file);
                free(landuse);
                return NULL;
            }
            if(config->sdate_filename.fmt==RAW)
            {
                header.nbands=2*ncft;
                landuse->sdate.offset=config->startgrid*header.nbands*sizeof(short);
            }
            else
            {
                if(header.nbands!=2*ncft)
                {
                    if(landuse->landuse.fmt==CDF)
                        closeclimate_netcdf(&landuse->landuse,isroot(*config));
                    else
                        fclose(landuse->landuse.file);
                    fclose(landuse->sdate.file);
                    if(isroot(*config))
                        fprintf(stderr,
                                "ERROR147: Invalid number of bands=%d in sowing date file.\n",
                                header.nbands);
                    free(landuse);
                    return(NULL);
                }
                landuse->sdate.offset=(config->startgrid-header.firstcell)*header.nbands*sizeof(short)+headersize(headername,version);
            }
            landuse->sdate.size=header.ncell*header.nbands*sizeof(short);
            landuse->sdate.n=config->ngridcell*header.nbands;
            landuse->nbands_sdate=header.nbands;
            landuse->sdate.scalar=header.scalar;
        }
    }
    else
        landuse->sdate.file=NULL;
    landuse->intercrop=param.intercrop;
    return landuse;
} /* of 'initlanduse' */


/***************************************************************************/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran/C++ version     **/
/**                                                                       **/
/**     Function reads landuse data for a specific year                   **/
/**                                                                       **/
/**     - order of landuse input data:                                    **/
/**        0-10   CFT                                                     **/
/**          11   OTHERS                                                  **/
/**          12   PASTURES                                                **/
/**          13   BIOMASS_GRASS                                           **/
/**          14   BIOMASS_TREE                                            **/
/**       15-25   CFT_irr                                                 **/
/**          26   others_irr                                              **/
/**          27   PASTURE_irr                                             **/
/**          28   BIOMASS_GRASS IRR                                       **/
/**          29   BIOMASS_TREE IRR                                        **/
/**     - called in iterate()                                             **/
/**     - reads every year the fractions of the bands for all cells from  **/
/**       the input file                                                  **/
/**     - checks if sum of fraction is not greater 1                      **/
/**       -> if sum of fraction is greater 1: subtraction from fraction   **/
/**          of managed grass if possible                                 **/
/**       -> else fail incorrect input file                               **/
/**                                                                       **/
/**     written by Werner von Bloh, Sibyll Schaphoff                      **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: 15.10.2009                                           **/
/**                                                                       **/
/***************************************************************************/

static Real reducelanduse(Cell *cell,Real sum,int ncft,int nagtree)
{
    int i,j;
    if(cell->ml.landfrac[0].grass[1] > sum)
    {
        cell->ml.landfrac[0].grass[1]-=sum;
        return 0.0;
    }
    if(cell->ml.landfrac[1].grass[1] > sum)
    {
        cell->ml.landfrac[1].grass[1]-=sum;
        return 0.0;
    }
    for(j=0;j<2;j++)
    {
        for(i=0;i<ncft;i++)
            if(cell->ml.landfrac[j].crop[i] > sum)
            {
                cell->ml.landfrac[j].crop[i]-=sum;
                return 0;
            }
        for(i=0;i<nagtree;i++)
            if(cell->ml.landfrac[j].ag_tree[i] > sum)
            {
                cell->ml.landfrac[j].ag_tree[i]-=sum;
                return 0;
            }
        for(i=0;i<NGRASS;i++)
            if(cell->ml.landfrac[j].grass[i] > sum)
            {
                cell->ml.landfrac[j].grass[i]-=sum;
                return 0;
            }
        if(cell->ml.landfrac[j].biomass_tree > sum)
        {
            cell->ml.landfrac[j].biomass_tree-=sum;
            return 0;
        }
        if(cell->ml.landfrac[j].biomass_grass > sum)
        {
            cell->ml.landfrac[j].biomass_grass-=sum;
            return 0;
        }
    }
    return sum;
} /* of 'reducelanduse' */

Bool getlanduse(Landuse landuse,     /* Pointer to landuse data */
        Cell grid[],         /* LPJ cell array */
        int year,            /* year (AD) */
        int ncft,            /* number of crop PFTs */
        const Config *config /* LPJ configuration */
        )                     /* returns TRUE on error */
{
    short *vec;
    int i,j,p,count,cell;
    Real sum,*data;
    int *dates;

    /* so far, read prescribed sdates only once at the beginning of each simulation */
    if(config->sdate_option==PRESCRIBED_SDATE)
    {
        if(landuse->sdate.fmt==CDF)
        {
            dates=newvec(int,config->ngridcell*landuse->sdate.var_len);
            if(dates==NULL)
            {
                printallocerr("dates");
                return TRUE;
            }
            if(readintdata_netcdf(&landuse->sdate,dates,grid,0,config))
            {
                fprintf(stderr,
                        "ERROR149: Cannot read sowing dates of year %d in getlanduse().\n",
                        year+landuse->sdate.firstyear);
                fflush(stderr);
                return TRUE;
            }
            count=0;
            for(cell=0;cell<config->ngridcell;cell++)
                if(!grid[cell].skip)
                    for(j=0;j<2*ncft;j++)
                        grid[cell].ml.sdate_fixed[j]=dates[count++];
                else
                    count+=2*ncft;
            free(dates);
        }
        else
        {
            if(fseek(landuse->sdate.file,landuse->sdate.offset,SEEK_SET))
            {
                fprintf(stderr,
                        "ERROR148: Cannot seek sowing dates to year %d in getlanduse().\n",
                        year);
                return TRUE;
            }
            vec=newvec(short,landuse->sdate.n);
            if(vec==NULL)
            {
                printallocerr("vec2");
                return TRUE;
            }
            if(fread(vec,sizeof(short),landuse->sdate.n,landuse->sdate.file)!=landuse->sdate.n)
            {
                fprintf(stderr,
                        "ERROR149: Cannot read sowing dates of year %d in getlanduse().\n",
                        year);
                free(vec);
                return TRUE;
            }
            if(landuse->sdate.swap)
                for(i=0;i<landuse->sdate.n;i++)
                    vec[i]=swapshort(vec[i]);
            count=0;
            for(cell=0;cell<config->ngridcell;cell++)
                if(!grid[cell].skip)
                    for(j=0;j<2*ncft;j++)
                        grid[cell].ml.sdate_fixed[j]=vec[count++]*landuse->sdate.scalar;
                else
                    count+=2*ncft;
            free(vec);
        }
    }

    if(!landuse->allcrops)
    {
        year-=landuse->landuse.firstyear;
        if(year>=landuse->landuse.nyear)
            year=landuse->landuse.nyear-1;
        else if(year<0)
            year=0;
        if(landuse->landuse.fmt==CDF)
        {
            data=newvec(Real,config->ngridcell*landuse->landuse.var_len);
            if(data==NULL)
            {
                printallocerr("data");
                return TRUE;
            }
            if(readdata_netcdf(&landuse->landuse,data,grid,year,config))
            {
                fprintf(stderr,
                        "ERROR149: Cannot read landuse of year %d in getlanduse().\n",
                        year+landuse->landuse.firstyear);
                fflush(stderr);
                return TRUE;
            }
        }
        else
        {
            if(fseek(landuse->landuse.file,(long long)year*landuse->landuse.size+landuse->landuse.offset,SEEK_SET))
            {
                fprintf(stderr,
                        "ERROR148: Cannot seek landuse to year %d in getlanduse().\n",
                        year+landuse->landuse.firstyear);
                fflush(stderr);
                return TRUE;
            }
            vec=newvec(short,landuse->landuse.n);
            if(vec==NULL)
            {
                printallocerr("vec");
                return TRUE;
            }
            if(fread(vec,sizeof(short),landuse->landuse.n,landuse->landuse.file)!=landuse->landuse.n)
            {
                fprintf(stderr,
                        "ERROR149: Cannot read landuse of year %d in getlanduse().\n",
                        year+landuse->landuse.firstyear);
                fflush(stderr);
                free(vec);
                return TRUE;
            }
            if(landuse->landuse.swap)
                for(i=0;i<landuse->landuse.n;i++)
                    vec[i]=swapshort(vec[i]);
        }
        count=0;
    }

    for(cell=0;cell<config->ngridcell;cell++)
    {
        if(landuse->allcrops)
        {
            sum=0;
            for(j=0;j<ncft;j++)
            {
                grid[cell].ml.landfrac[1].crop[j]=0.01;
                grid[cell].ml.landfrac[0].crop[j]=0.01;
                sum += 0.02;
                grid[cell].ml.irrig_system->crop[j]=grid[cell].ml.manage.par->default_irrig_system; /*default national irrigation system (Rohwer & Gerten 2007)*/
            }
            for(j=0;j<config->nagtree;j++)
            {
                grid[cell].ml.landfrac[1].ag_tree[j]=0.01;
                grid[cell].ml.landfrac[0].ag_tree[j]=0.01;
                grid[cell].ml.irrig_system->ag_tree[j]=grid[cell].ml.manage.par->default_irrig_system; /*default national irrigation system (Rohwer & Gerten 2007)*/
                sum += 0.02;
            }
            for(j=0;j<NGRASS;j++)
            {
                grid[cell].ml.landfrac[0].grass[j]=0.01;
                grid[cell].ml.landfrac[1].grass[j]=0.01;
                sum += 0.02;
                grid[cell].ml.irrig_system->grass[j]=grid[cell].ml.manage.par->default_irrig_system;
            }
            grid[cell].ml.landfrac[1].biomass_tree=0.01;
            grid[cell].ml.landfrac[0].biomass_tree=0.01;
            grid[cell].ml.landfrac[1].biomass_grass=0.01;
            grid[cell].ml.landfrac[0].biomass_grass=0.01;
            sum += 0.04;
            grid[cell].ml.landfrac[0].grass[NGRASS-1]=1.0-sum+0.01; /* I set landfrac[0], trunk has landfrac[1], which is irrigated -> much more irrigated grass in allcrops run */

            grid[cell].ml.irrig_system->biomass_tree=grid[cell].ml.manage.par->default_irrig_system;
            grid[cell].ml.irrig_system->biomass_grass=grid[cell].ml.manage.par->default_irrig_system;
        }
        else
        {
            if(landuse->landuse.fmt==CDF)
            {
                for(i=0;i<WIRRIG;i++)
                {
                    if(!landuse->isirrigsystem || i<1)
                    {
                        for(j=0;j<ncft;j++)
                        {
                            grid[cell].ml.landfrac[i].crop[j]=data[count++];
                            if(i>0)
                                grid[cell].ml.irrig_system->crop[j]=grid[cell].ml.manage.par->default_irrig_system; /*default national irrigation system (Rohwer & Gerten 2007)*/
                        }
                        if(landuse->isagtree)
                            for(j=0;j<config->nagtree;j++)
                            {
                                grid[cell].ml.landfrac[i].ag_tree[j]=data[count++];
                                if(i>0)
                                    grid[cell].ml.irrig_system->ag_tree[j]=grid[cell].ml.manage.par->default_irrig_system; /*default national irrigation system (Rohwer & Gerten 2007)*/
                            }
                        else
                            grid[cell].ml.landfrac[i].ag_tree[j]=0;
                        for(j=0;j<NGRASS;j++)
                        {
                            grid[cell].ml.landfrac[i].grass[j]=data[count++];
                            if(i>0)
                                grid[cell].ml.irrig_system->grass[j]=grid[cell].ml.manage.par->default_irrig_system;
                        }
                        if(landuse->isbiomass)
                        {
                            grid[cell].ml.landfrac[i].biomass_grass=data[count++];
                            if(i>0)
                                grid[cell].ml.irrig_system->biomass_grass=grid[cell].ml.manage.par->default_irrig_system;
                            grid[cell].ml.landfrac[i].biomass_tree=data[count++];
                            if(i>0)
                                grid[cell].ml.irrig_system->biomass_tree=grid[cell].ml.manage.par->default_irrig_system;
                        }
                        else
                            grid[cell].ml.landfrac[i].biomass_grass=grid[cell].ml.landfrac[i].biomass_tree=0;
                    }
                    else /* read irrigated cropfrac and irrigation systems from 64 bands input */
                    {
                        for(j=0;j<ncft;j++) /* initialization needed */
                            grid[cell].ml.landfrac[i].crop[j]=0;
                        for(j=0;j<config->nagtree;j++)
                            grid[cell].ml.landfrac[i].ag_tree[j]=0;
                        for(j=0;j<NGRASS;j++)
                            grid[cell].ml.landfrac[i].grass[j]=0;
                        grid[cell].ml.landfrac[i].biomass_grass=0;
                        grid[cell].ml.landfrac[i].biomass_tree=0;

                        for(p=1;p<4;p++) /* irrigation system loop; 1: surface, 2: sprinkler, 3: drip */
                        {
                            for(j=0;j<ncft;j++)
                            {
                                if(data[count]>0)
                                {
                                    grid[cell].ml.landfrac[i].crop[j]=data[count++];
                                    grid[cell].ml.irrig_system->crop[j]=p;
                                }
                                else
                                    count++;
                            }
                            if(landuse->isagtree)
                                for(j=0;j<config->nagtree;j++)
                                {
                                    if(data[count]>0)
                                    {
                                        grid[cell].ml.landfrac[i].ag_tree[j]=data[count++];
                                        grid[cell].ml.irrig_system->ag_tree[j]=p;
                                    }
                                    else
                                        count++;
                                }
                            else
                                for(j=0;j<config->nagtree;j++)
                                    grid[cell].ml.landfrac[i].ag_tree[j]=0;
                            for(j=0;j<NGRASS;j++)
                            {
                                if(data[count]>0)
                                {
                                    grid[cell].ml.landfrac[i].grass[j]=data[count++];
                                    grid[cell].ml.irrig_system->grass[j]=p;
                                }
                                else
                                    count++;
                            }
                            if(landuse->isbiomass)
                            {
                                if(data[count]>0)
                                {
                                    grid[cell].ml.landfrac[i].biomass_grass=data[count++];
                                    grid[cell].ml.irrig_system->biomass_grass=p;
                                }
                                else
                                    count++;
                                if(data[count]>0)
                                {
                                    grid[cell].ml.landfrac[i].biomass_tree=data[count++];
                                    grid[cell].ml.irrig_system->biomass_tree=p;
                                }
                                else
                                    count++;
                            }
                            else
                                grid[cell].ml.landfrac[i].biomass_grass=grid[cell].ml.landfrac[i].biomass_tree=0;
                        }

                    }
                }
            }
            else
                //printf("barrier.n: ncft=%d, nagtree=%d, nagtree=%d, biomass=%d\n", ncft, config->nagtree, NGRASS, landuse->isbiomass);
                for(i=0;i<WIRRIG;i++)
                {
                    /* read cropfrac from 32 bands or rain-fed cropfrac from 64 bands input */
                    if(!landuse->isirrigsystem || i<1)
                    {
                        for(j=0;j<ncft;j++)
                        {
                            grid[cell].ml.landfrac[i].crop[j]=vec[count++]*landuse->landuse.scalar;
                            if(i>0)
                                grid[cell].ml.irrig_system->crop[j]=grid[cell].ml.manage.par->default_irrig_system; /*default national irrigation system (Rohwer & Gerten 2007)*/
                        }
                        if(landuse->isagtree)
                            for(j=0;j<config->nagtree;j++)
                            {
                                grid[cell].ml.landfrac[i].ag_tree[j]=vec[count++]*landuse->landuse.scalar;
                                if(i>0)
                                    grid[cell].ml.irrig_system->ag_tree[j]=grid[cell].ml.manage.par->default_irrig_system; /*default national irrigation system (Rohwer & Gerten 2007)*/
                            }
                        else
                            for(j=0;j<config->nagtree;j++)
                                grid[cell].ml.landfrac[i].ag_tree[j]=0;
                        for(j=0;j<NGRASS;j++)
                        {
                            grid[cell].ml.landfrac[i].grass[j]=vec[count++]*landuse->landuse.scalar;
                            if(i>0)
                                grid[cell].ml.irrig_system->grass[j]=grid[cell].ml.manage.par->default_irrig_system;
                        }
                        if(landuse->isbiomass)
                        {
                            grid[cell].ml.landfrac[i].biomass_grass=vec[count++]*landuse->landuse.scalar;
                            if(i>0)
                                grid[cell].ml.irrig_system->biomass_grass=grid[cell].ml.manage.par->default_irrig_system;
                            grid[cell].ml.landfrac[i].biomass_tree=vec[count++]*landuse->landuse.scalar;
                            if(i>0)
                                grid[cell].ml.irrig_system->biomass_tree=grid[cell].ml.manage.par->default_irrig_system;
                        }
                        else
                            grid[cell].ml.landfrac[i].biomass_grass=grid[cell].ml.landfrac[i].biomass_tree=0;
                    }
                    else /* read irrigated cropfrac and irrigation systems from 64 bands input */
                    {
                        for(j=0;j<ncft;j++) /* initialization needed */
                            grid[cell].ml.landfrac[i].crop[j]=0;
                        for(j=0;j<config->nagtree;j++)
                            grid[cell].ml.landfrac[i].ag_tree[j]=0;
                        for(j=0;j<NGRASS;j++)
                            grid[cell].ml.landfrac[i].grass[j]=0;
                        grid[cell].ml.landfrac[i].biomass_grass=0;
                        grid[cell].ml.landfrac[i].biomass_tree=0;

                        for(p=1;p<4;p++) /* irrigation system loop; 1: surface, 2: sprinkler, 3: drip */
                        {
                            for(j=0;j<ncft;j++)
                            {
                                if(vec[count]>0)
                                {
                                    grid[cell].ml.landfrac[i].crop[j]=vec[count++]*landuse->landuse.scalar;
                                    grid[cell].ml.irrig_system->crop[j]=p;
                                }
                                else
                                    count++;
                            }
                            if(landuse->isagtree)
                                for(j=0;j<config->nagtree;j++)
                                {
                                    if(vec[count]>0)
                                    {
                                        grid[cell].ml.landfrac[i].ag_tree[j]=vec[count++]*landuse->landuse.scalar;
                                        grid[cell].ml.irrig_system->ag_tree[j]=p;
                                    }
                                    else
                                        count++;
                                }
                            else
                                for(j=0;j<config->nagtree;j++)
                                    grid[cell].ml.landfrac[i].ag_tree[j]=0;
                            for(j=0;j<NGRASS;j++)
                            {
                                if(vec[count]>0)
                                {
                                    grid[cell].ml.landfrac[i].grass[j]=vec[count++]*landuse->landuse.scalar;
                                    grid[cell].ml.irrig_system->grass[j]=p;
                                }
                                else
                                    count++;
                            }
                            if(landuse->isbiomass)
                            {
                                if(vec[count]>0)
                                {
                                    grid[cell].ml.landfrac[i].biomass_grass=vec[count++]*landuse->landuse.scalar;
                                    grid[cell].ml.irrig_system->biomass_grass=p;
                                }
                                else
                                    count++;
                                if(vec[count]>0)
                                {
                                    grid[cell].ml.landfrac[i].biomass_tree=vec[count++]*landuse->landuse.scalar;
                                    grid[cell].ml.irrig_system->biomass_tree=p;
                                }
                                else
                                    count++;
                            }
                            else
                                grid[cell].ml.landfrac[i].biomass_grass=grid[cell].ml.landfrac[i].biomass_tree=0;
                        }
                    }
                }
            switch(config->irrig_scenario)
            {
                case NO_IRRIGATION:
                    for(j=0;j<ncft;j++)
                    {
                        grid[cell].ml.landfrac[0].crop[j]+=grid[cell].ml.landfrac[1].crop[j];
                        grid[cell].ml.landfrac[1].crop[j]=0;
                        grid[cell].ml.irrig_system->crop[j]=NOIRRIG;
                    }
                    for(j=0;j<config->nagtree;j++)
                    {
                        grid[cell].ml.landfrac[0].ag_tree[j]+=grid[cell].ml.landfrac[1].ag_tree[j];
                        grid[cell].ml.landfrac[1].ag_tree[j]=0;
                        grid[cell].ml.irrig_system->ag_tree[j]=NOIRRIG;
                    }
                    for(j=0;j<NGRASS;j++)
                    {
                        grid[cell].ml.landfrac[0].grass[j]+=grid[cell].ml.landfrac[1].grass[j];
                        grid[cell].ml.landfrac[1].grass[j]=0;
                        grid[cell].ml.irrig_system->grass[j]=NOIRRIG;
                    }
                    grid[cell].ml.landfrac[0].biomass_grass+=grid[cell].ml.landfrac[1].biomass_grass;
                    grid[cell].ml.landfrac[1].biomass_grass=0;
                    grid[cell].ml.irrig_system->biomass_grass=NOIRRIG;
                    grid[cell].ml.landfrac[0].biomass_tree+=grid[cell].ml.landfrac[1].biomass_tree;
                    grid[cell].ml.landfrac[1].biomass_tree=0;
                    grid[cell].ml.irrig_system->biomass_tree=NOIRRIG;
                    break;
                case ALL_IRRIGATION:
                    for(j=0;j<ncft;j++)
                    {
                        grid[cell].ml.landfrac[1].crop[j]+=grid[cell].ml.landfrac[0].crop[j];
                        grid[cell].ml.landfrac[0].crop[j]=0;
                        grid[cell].ml.irrig_system->crop[j]=grid[cell].ml.manage.par->default_irrig_system; /*default national irrigation system (Rohwer & Gerten 2007)*/
                    }
                    for(j=0;j<config->nagtree;j++)
                    {
                        grid[cell].ml.landfrac[1].ag_tree[j]+=grid[cell].ml.landfrac[0].ag_tree[j];
                        grid[cell].ml.landfrac[0].ag_tree[j]=0;
                        grid[cell].ml.irrig_system->ag_tree[j]=grid[cell].ml.manage.par->default_irrig_system; /*default national irrigation system (Rohwer & Gerten 2007)*/
                    }
                    for(j=0;j<NGRASS;j++)
                    {
                        grid[cell].ml.landfrac[1].grass[j]+=grid[cell].ml.landfrac[0].grass[j];
                        grid[cell].ml.landfrac[0].grass[j]=0;
                        grid[cell].ml.irrig_system->grass[j]=grid[cell].ml.manage.par->default_irrig_system;
                    }
                    grid[cell].ml.landfrac[1].biomass_grass+=grid[cell].ml.landfrac[0].biomass_grass;
                    grid[cell].ml.landfrac[0].biomass_grass=0;
                    grid[cell].ml.irrig_system->biomass_grass=grid[cell].ml.manage.par->default_irrig_system;
                    grid[cell].ml.landfrac[1].biomass_tree+=grid[cell].ml.landfrac[0].biomass_tree;
                    grid[cell].ml.landfrac[0].biomass_tree=0;
                    grid[cell].ml.irrig_system->biomass_tree=grid[cell].ml.manage.par->default_irrig_system;
                    break;
            } /* of switch(...) */

            /* DEBUG: here you can set land-use fractions manually, it overwrites the land-use input, in all cells */
            /* the irrigation system is set to the default country value, but you can set a number from 1-3 manually */
            /* 1: surface, 2: sprinkler, 3: drip irrigation */

            /*      for(j=0;j<ncft;j++)
                    {
                    grid[cell].ml.landfrac[1].crop[j]=0.0;
                    grid[cell].ml.landfrac[0].crop[j]=0.0;
                    grid[cell].ml.irrig_system->crop[j]=grid[cell].ml.manage.par->default_irrig_system;
                    }
                    for(j=0;j<NGRASS;j++)
                    {
                    grid[cell].ml.landfrac[1].grass[j]=0.25;
                    grid[cell].ml.landfrac[0].grass[j]=0.0;
                    grid[cell].ml.irrig_system->grass[j]=grid[cell].ml.manage.par->default_irrig_system;
                    }

                    grid[cell].ml.landfrac[1].biomass_grass=0.25;
                    grid[cell].ml.landfrac[0].biomass_grass=0;
                    grid[cell].ml.irrig_system->biomass_grass=grid[cell].ml.manage.par->default_irrig_system;
                    grid[cell].ml.landfrac[1].biomass_tree=0.25;
                    grid[cell].ml.landfrac[0].biomass_tree=0;
                    grid[cell].ml.irrig_system->biomass_tree=grid[cell].ml.manage.par->default_irrig_system;
                    */
            /* END DEBUG */

            sum=landfrac_sum(grid[cell].ml.landfrac,ncft,config->nagtree,FALSE)+landfrac_sum(grid[cell].ml.landfrac,ncft,config->nagtree,TRUE);
        }

        if(sum>1.00001)
        {
            if(year>0)
            {
                /*fprintf(stderr,"WARNING013: in cell %d at year %d: sum of cropfraction greater 1: %f\n",
                        cell+config->startgrid,year+landuse->landuse.firstyear,sum);*/
                fflush(stderr);
            }
            sum=reducelanduse(grid+cell,sum-1,ncft,config->nagtree);
            if(sum > 0.00001)
                fail(CROP_FRACTION_ERR,FALSE,
                        "cropfraction greater 1: %f cell: %d, managed grass is 0",
                        sum+1,cell+config->startgrid);
        }
    } /* for(cell=0;...) */

    if(!landuse->allcrops)
    {
        if(landuse->landuse.fmt==CDF)
            free(data);
        else
            free(vec);
    }
    return FALSE;
} /* of 'getlanduse' */

static void reduce_frac(Real *standfrac, Real *remainfrac)
{
    if(*standfrac>=*remainfrac)
    {
        *standfrac-=*remainfrac;
        *remainfrac=0;
    }
    else
    {
        *remainfrac-=*standfrac;
        *standfrac=0;
    }
} /* of 'reduce_frac' */

Bool newlanduse(Cell grid[],           /* cell grid array */
        int ngridcell,         /* number of grid cells */
        const Pftpar *pftpar,  /* PFT parameter array */
        int npft,              /* number of natural PFTs */
        int ncft,              /* number of crop PFTs */
        int nagtree,
        const Landuse landuse, /* new landuse data */
        Bool istimber          /* IMAGE coupling (TRUE/FALSE) */
        )                       /* returns TRUE on error */
{

    Stand *stand,*standg;
    int s,sg,pos,i,cell;
    Real grassfrac[2],grassfrac_total;
    Real cropfrac[2],frac,remain_lakefrac;
    Real naturalfrac=0;
    int separate=2;
    Bool exist[3];
    Irrigation *data;
    exist[0]=exist[1]=exist[2]=FALSE;
    printf("###################### barrier inside newlanduse\n");
    for(cell=0;cell<ngridcell;cell++)
        if(!grid[cell].skip)
        {
            foreachstand(stand,s,grid[cell].standlist)
                switch(stand->type->landusetype)
                {
                    case AGRICULTURE:
                        /* setaside all existing croplands */
                        cutpfts(stand);
                        data=stand->data;
                        if(setaside(grid+cell,getstand(grid[cell].standlist,s),pftpar,landuse->intercrop,npft,data->irrigation))
                        {
                            delstand(grid[cell].standlist,s);
                            s--; /* adjust stand index */
                        }
                        break;
                    case GRASSLAND:
                        /* mix all existing GRASSLAND stands */
                        cutpfts(stand);
                        foreachstand(standg,sg,grid[cell].standlist)
                            if(standg->type->landusetype==GRASSLAND && s!= sg)
                            {
                                cutpfts(standg);
                                mixsoil(stand,standg);
                                stand->frac+=standg->frac;
                                delstand(grid[cell].standlist,sg);
                                break;
                            }
                        break;
                } /* of 'switch' */

            /* PASTURE + OTHERS rainfed */
            grassfrac[0]=grid[cell].ml.landfrac[0].grass[0]+grid[cell].ml.landfrac[0].grass[1];
            /* PASTURE + OTHERS irrigated */
            grassfrac[1]=grid[cell].ml.landfrac[1].grass[0]+grid[cell].ml.landfrac[1].grass[1];
            cropfrac[0]=landfrac_sum(grid[cell].ml.landfrac,ncft,nagtree,FALSE)-grassfrac[0];
            cropfrac[1]=landfrac_sum(grid[cell].ml.landfrac,ncft,nagtree,TRUE)-grassfrac[1];
            naturalfrac=1.0-(cropfrac[0]+cropfrac[1])-(grassfrac[0]+grassfrac[1]);

            frac=1.0-grid[cell].lakefrac-grid[cell].ml.reservoirfrac;

            /* if a lake fraction exist, reduce fractions of the stands in the order
               natural vegetation, setaside, managed grassland */
            remain_lakefrac=grid[cell].lakefrac+grid[cell].ml.reservoirfrac;
            if(remain_lakefrac>0)
            {
                reduce_frac(&naturalfrac,&remain_lakefrac);
                if(remain_lakefrac>0)
                {
                    reduce_frac(&cropfrac[0],&remain_lakefrac);
                    if(remain_lakefrac>0)
                    {
                        reduce_frac(&cropfrac[1],&remain_lakefrac);
                        if(remain_lakefrac>0)
                        {
                            reduce_frac(&grassfrac[0],&remain_lakefrac);
                            if(remain_lakefrac>0)
                                reduce_frac(&grassfrac[1],&remain_lakefrac);
                        }
                    }
                }
            }

            grassfrac_total=grassfrac[0]+grassfrac[1];
            grid[cell].ml.cropfrac_rf=cropfrac[0]+grassfrac[0];
            grid[cell].ml.cropfrac_ir=cropfrac[1]+grassfrac[1];
            foreachstand(stand,s,grid[cell].standlist)
                switch(stand->type->landusetype)
                {
                    case GRASSLAND:
                        exist[0]=TRUE;
                        break;
                    case SETASIDE_RF:
                        exist[1]=TRUE;
                        break;
                    case SETASIDE_IR:
                        exist[2]=TRUE;
                        break;
                }
            foreachstand(stand,s,grid[cell].standlist)
                switch(stand->type->landusetype)
                {
                    case GRASSLAND:
                        if (grassfrac[0]>epsilon)
                        {
                            stand->frac=grassfrac[0];
                            data=stand->data;
                            data->irrigation=FALSE;
                            separate=1;
                        }
                        else if (grassfrac[1]>epsilon)
                        {
                            stand->frac=grassfrac[1];
                            data=stand->data;
                            data->irrigation=TRUE;
                            separate=2;
                        }
                        else
                        {
                            delstand(grid[cell].standlist,s);
                            s--;
                        }
                        break;
                    case SETASIDE_RF:
                        stand->frac=cropfrac[0];
                        if (stand->frac<epsilon)
                        {
                            delstand(grid[cell].standlist,s);
                            s--;
                        }
                        break;
                    case SETASIDE_IR:
                        stand->frac=cropfrac[1];
                        if (stand->frac<epsilon)
                        {
                            delstand(grid[cell].standlist,s);
                            s--;
                        }
                        break;
                    default:
                        if(!exist[0] && !exist[1] && !exist[2])
                        {
                            stand->frac=frac;
                            grid[cell].ml.cropfrac_rf=grid[cell].ml.cropfrac_ir=0;
                        }
                        else if(!exist[0])
                        {
                            if(!exist[1])
                            {
                                stand->frac=frac-cropfrac[1];
                                grid[cell].ml.cropfrac_ir=cropfrac[1];

                            }
                            else if(!exist[2])
                            {
                                stand->frac=frac-cropfrac[0];
                                grid[cell].ml.cropfrac_rf=cropfrac[0];
                            }
                            else
                            {
                                stand->frac=frac-cropfrac[0]-cropfrac[1];
                                grid[cell].ml.cropfrac_rf=cropfrac[0];
                                grid[cell].ml.cropfrac_ir=cropfrac[1];
                            }
                        }
                        else if(!exist[1] && !exist[2])
                        {
                            stand->frac=frac-grassfrac_total;
                            grid[cell].ml.cropfrac_rf=grassfrac[0];
                            grid[cell].ml.cropfrac_ir=grassfrac[1];
                        }
                        else if(!exist[1])
                        {
                            stand->frac=frac-grid[cell].ml.cropfrac_ir;
                        }
                        else
                            stand->frac=frac-grid[cell].ml.cropfrac_rf;
                        naturalfrac=stand->frac;
                        initstand(stand);
                        if (stand->frac<epsilon)
                        {
                            delstand(grid[cell].standlist,s);
                            s--;
                        }
                } /* of 'switch' */

            if (frac-grid[cell].ml.cropfrac_rf-grid[cell].ml.cropfrac_ir-naturalfrac>epsilon)
            { /* no NATURAL stand in restart */
                pos=findlandusetype(grid[cell].standlist,SETASIDE_RF);
                if(pos==NOT_FOUND)
                {
                    pos=findlandusetype(grid[cell].standlist,SETASIDE_IR);
                    if(pos==NOT_FOUND)
                        pos=findlandusetype(grid[cell].standlist,GRASSLAND);
                }
                if(pos!=NOT_FOUND)
                {
                    standg=getstand(grid[cell].standlist,pos);
                    s=addstand(&natural_stand,grid+cell);
                    stand=getstand(grid[cell].standlist,s-1);
                    reclaim_land(standg,stand,grid+cell,istimber,npft+ncft);
                    stand->frac=1-frac;
                }
                else
                {
                    fprintf(stderr,"ERROR158: Fraction error in newlanduse(), cropfrac (rf/ir) %.2f %.2f natural_frac= %.5f.\n",
                            grid[cell].ml.cropfrac_rf,grid[cell].ml.cropfrac_ir,naturalfrac);
                    return TRUE;
                }

            }

            if (separate<2)
            {
                s=findlandusetype(grid[cell].standlist,GRASSLAND);
                if(s!=NOT_FOUND)
                {
                    for (i=separate;i<2;i++)
                        if (grassfrac[i]>0)
                        {
                            pos=addstand(&grassland_stand,grid+cell)-1;
                            standg=getstand(grid[cell].standlist,pos);
                            stand=getstand(grid[cell].standlist,s);
                            reclaim_land(stand,standg,grid+cell,istimber,npft+ncft);
                            initstand(standg);
                            if (i==1)
                            {
                                data=standg->data;
                                data->irrigation=TRUE;
                            }
                            standg->frac=grassfrac[i];
                        }
                }
                else
                {
                    fputs("ERROR159: No grassland found in restart.\n",stderr);
                    return TRUE;
                }

            }
        }
    return FALSE;
} /* of 'newlanduse' */

Bool getintercrop(const Landuse landuse)
{
    return (landuse==NULL) ? FALSE : landuse->intercrop;
} /* of 'getintercrop' */

void freelanduse(Landuse landuse,Bool isroot)
{
    if(landuse!=NULL)
    {
        if(!landuse->allcrops)
        {
            if(landuse->landuse.fmt==CDF)
                closeclimate_netcdf(&landuse->landuse,isroot);
            else
                fclose(landuse->landuse.file);
        }
        if(landuse->sdate.file!=NULL)
        {
            if(landuse->sdate.fmt==CDF)
                closeclimate_netcdf(&landuse->sdate,isroot);
            else
                fclose(landuse->sdate.file);
        }
        free(landuse);
    }
} /* of 'freelanduse' */
