
/* Module for the immobilization in soils (08, section 2).
 *
 * This should eventually replace the littersom routine.
 *
 * \author Nicolas Barrier
 */

#include "lpj.h"

/* Immobilisation in soils (08, section 2). Loop over all the soil layers.
 *
 * Called in "update_daily".
 *
 * @param Soil soil: pointer to the soil variable
 *
 * \author Nicolas Barrier
 *
 */
void nit_immo_soil(Soil *soil, int landusetype ,char* name)
{
    int l;

    Real gdec;  //daily consumption of carbon residues by soil decomposers using only the residue(g/m2)
    Real gdecmax; // maximum daily consumption of carbon residues by soil decomposers using only the residue(g/m2)
	Real maxCfromNsoil;//maximum daily C residue consumption by using N in soil and N remaining in residue(g/m2)
	Real maxCfromPsoil;//maximum daily C residue consumption by using P in soil and P remaining in residue(g/m2)
	Real bonusGdec;// daily consumption of carbon residues made possible by using N and P from soil(g/m2)
	Real NsoilConsumption;// (g/m2)
	Real PsoilConsumption;// (g/m2)
	Real diff; //(g/m2)

    forrootsoillayer(l){

    gdec =0;
	bonusGdec =0;
	maxCfromNsoil =0;
	maxCfromPsoil =0;
	NsoilConsumption =0;
	PsoilConsumption =0;

	//maximum daily C residues consumption by the soil decomposers
	gdecmax = BETA_RES_SOIL * nit_tempdep(soil->temp[l], 24.0, 14.0) * soil->RESC[l];

	//When decomposers consume C they consume N and P to keep their C/N and C/P ratio constant
	//so the quantity of C consumed can be limited (and is in practice) by N and P quantities in the residues.
	gdec = min(gdecmax, soil->RESN[l]*RATIO_CN_DEC);
	gdec = min(gdec, soil->RESP[l]*RATIO_CP_DEC);

	//however, even when there is no more N or P in the residues, the decomposers can keep
	//consuming C in the residues by taking the N or P missing in the soil up to a point.
	if (gdec < gdecmax)
	{
		diff = gdecmax -gdec;
		//maximum daily C residue consumption by using N in soil and N remaining in residue
		maxCfromNsoil =IMMO_N*(soil->NO3[l]+soil->NH4[l])* RATIO_CN_DEC + (soil->RESN[l] -gdec/RATIO_CN_DEC)* RATIO_CN_DEC;

		//maximum daily C residue consumption by using P in soil and P remaining in residue
		maxCfromPsoil =IMMO_P*(soil->PO4[l])* RATIO_CP_DEC + (soil->RESP[l] -gdec/RATIO_CP_DEC)* RATIO_CP_DEC;

		bonusGdec= min(diff,maxCfromNsoil );
		bonusGdec= min(bonusGdec, maxCfromPsoil);

		//We use in priority the N in the residue so N taken from the soil for the growth is total N consumed - RESN
		NsoilConsumption = max(0,(gdec +bonusGdec)/RATIO_CN_DEC - soil->RESN[l]);
		PsoilConsumption = max(0,(gdec +bonusGdec)/RATIO_CP_DEC - soil->RESP[l]);
	}

	//decomposers growth in C biomass
    soil->DECC[l] = soil->DECC[l]*(1 - MDEC) + (gdec + bonusGdec);

	//residue update
	soil->RESC[l] -= (gdec + bonusGdec);
	soil->RESN[l] -= ((gdec + bonusGdec)/RATIO_CN_DEC - NsoilConsumption);
	soil->RESP[l] -= ((gdec + bonusGdec)/RATIO_CP_DEC - PsoilConsumption);
	//soil update
	soil->NO3[l] += -NsoilConsumption/2;
	soil->NH4[l] += MDEC * soil->DECC[l]/RATIO_CN_DEC - NsoilConsumption/2;


	if (soil->NH4[l] < 0) // 
	{
		soil->NO3[l] += soil->NH4[l];
		soil->NH4[l] =0;
	}
	if (soil->NO3[l] <0)
    {
        soil->NH4[l] += soil->NO3[l];
        soil->NO3[l] =0;
    }
	soil->PO4[l] += MDEC * soil->DECC[l]/RATIO_CP_DEC -PsoilConsumption;



    } // end of depth loop
} // end of function
