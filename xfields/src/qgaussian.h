#ifndef QGAUSSIAN 
#define QGAUSSIAN 

#include <math.h> //only_for_context none


/*gpukern*/
void q_gaussian_profile(
		    const int     n,
      /*gpuglmem*/  const double* z,
		    const double  z0,
		    const double  z_min,
		    const double  z_max,
		    const double  beta, 
		    const double  q, 
		    const double  q_tol,
		    const double  factor,
      /*gpuglmem*/        double* res){

    if (fabs(q-1.) < q_tol){
    	#pragma omp parallel for //only_for_context cpu_openmp 
    	for(int ii; ii<n; ii++){ //vectorize_over ii n 
    	    double zi = z[ii];
	    if (zi<z_max && zi>z_min){
		double zi_m_z0 = zi - z0;
	    	res[ii] = factor*exp(-beta*zi_m_z0*zi_m_z0 );
	    }
	    else{
	    	res[ii] = 0; 
	    }
    	}//end_vectorize
    }
    else{
    	double exponent = 1./(1.-q);
    	#pragma omp parallel for //only_for_context cpu_openmp 
    	for(int ii; ii<n; ii++){ //vectorize_over ii n
    	    double zi = z[ii];
	    if (zi<z_max && zi>z_min){
		double zi_m_z0 = zi - z0;
    	    	double q_exp_arg =  -(beta*zi_m_z0*zi_m_z0 );
    	    	double q_exp_res = pow(
			 (1.+(1.-q)*q_exp_arg), exponent );
    	    	res[ii] = factor*q_exp_res;
	    }
	    else{
	    	res[ii] = 0; 
	    }
    	}//end_vectorize
    }
}	

#endif