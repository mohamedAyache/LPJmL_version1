/***************************************************************************/
/**                                                                       **/
/**               f i r e _ e m i s s i o n s . c                         **/
/**                                                                       **/
/**     C implementation of LPJ, derived from the Fortran version         **/
/**                                                                       **/
/**     written by Kirsten Thonicke                                       **/
/**     Potsdam Institute for Climate Impact Research                     **/
/**     PO Box 60 12 03                                                   **/
/**     14412 Potsdam/Germany                                             **/
/**                                                                       **/
/**     Last change: 06.05.2008                                           **/
/**                                                                       **/
/***************************************************************************/

#include "lpj.h"

Real fire_emissions(Real total_firec)
{

/*continue coding*/
/* add emiss-factor to pftpar*/
/*	     Calculate monthly trace gas emission resulting from biomass burning (gSpecies/m2)


	       do m=1,12 !months
	        do x=1,6 !trace_species

	         do pft=1,npft
	         if (present(pft)) then

	            if (mcflux_fire_pft(m,pft).gt.0.0) then !A 

	            mcflux_trace_pft(m,x,pft)=(mcflux_fire_pft(m,pft)*
	     *                                    ef_trace(pft,x)/0.45)
	          mcflux_trace(m,x)=mcflux_trace(m,x)+mcflux_trace_pft(m,x,pft)
	           else
	               mcflux_trace(m,x)=0.0
	           endif

	         endif !if present

	        enddo
	           acflux_trace(x)=acflux_trace(x)+mcflux_trace(m,x)
	       enddo
	      enddo
*/
	
	return 0;   
}
