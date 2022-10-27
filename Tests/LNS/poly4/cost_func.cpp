#include "myasa.h"


extern "C" { 

int error_function(double *x, long int*in_para_dim, int *in_para_int_real ,USER_DEFINES * USER_OPTIONS )
{
double E_x_in, E_x, E_c4, E_c3, E_c2, E_c1, E_c0, E_dp0, E_dp1, E_dp2, E_dp3, E_dp4, E_dp5, E_dp6, ;
double E_y;
double Ey[ 14 ];
double Eout[ 1 ];


E_x_in = 0;
E_x = pow( 2. , 0 - pow( 2. , -x[ 0 ] - 1 ) ) - pow( 2. , 0 - pow( 2. , -x[ 0 ]  ) );
E_c4 = pow( 2. , -4 + pow( 2. , -x[ 1 ] - 1 ) ) - pow( 2. , -4 + pow( 2. , -x[ 1 ]  ) );
E_c3 = pow( 2. , -2 + pow( 2. , -x[ 2 ] - 1 ) ) - pow( 2. , -2 + pow( 2. , -x[ 2 ]  ) );
E_c2 = pow( 2. , -1 + pow( 2. , -x[ 3 ] - 1 ) ) - pow( 2. , -1 + pow( 2. , -x[ 3 ]  ) );
E_c1 = pow( 2. , -0 - pow( 2. , -x[ 4 ] - 1 ) ) - pow( 2. , -0 - pow( 2. , -x[ 4 ]  ) );
E_c0 = pow( 2. , -14 + pow( 2. , -x[ 5 ] - 1 ) ) - pow( 2. , -14 + pow( 2. , -x[ 5 ]  ) );
E_dp0 = pow( 2. , -3 + pow( 2. , -x[ 6 ] - 1 ) ) - pow( 2. , -3 + pow( 2. , -x[ 6 ]  ) );
E_dp1 = pow( 2. , -1 + pow( 2. , -x[ 7 ] - 1 ) ) - pow( 2. , -1 + pow( 2. , -x[ 7 ]  ) );
E_dp2 = pow( 2. , -1 + pow( 2. , -x[ 8 ] - 1 ) ) - pow( 2. , -1 + pow( 2. , -x[ 8 ]  ) );
E_dp3 = pow( 2. , -0 - pow( 2. , -x[ 9 ] - 1 ) ) - pow( 2. , -0 - pow( 2. , -x[ 9 ]  ) );
E_dp4 = pow( 2. , 0 - pow( 2. , -x[ 10 ] - 1 ) ) - pow( 2. , 0 - pow( 2. , -x[ 10 ]  ) );
E_dp5 = pow( 2. , 1 - pow( 2. , -x[ 11 ] - 1 ) ) - pow( 2. , 1 - pow( 2. , -x[ 11 ]  ) );
E_dp6 = pow( 2. , 1 - pow( 2. , -x[ 12 ] - 1 ) ) - pow( 2. , 1 - pow( 2. , -x[ 12 ]  ) );

E_y = 0;

Ey[ 0 ] = fabs( ( 2.26001 * E_x + 1.41421 * ( 0.893408 * E_x + 1.41421 * ( 0.300921 * E_x + 1.41421 * 0.0565797 * E_x ) ) ) );
Ey[ 1 ] = fabs( 1.41421 * 1.41421 * 1.41421 * 1.41421 * E_c4 );
Ey[ 2 ] = fabs( 1.41421 * 1.41421 * 1.41421 * E_c3 );
Ey[ 3 ] = fabs( 1.41421 * 1.41421 * E_c2 );
Ey[ 4 ] = fabs( 1.41421 * E_c1 );
Ey[ 5 ] = fabs( E_c0 );
Ey[ 6 ] = fabs( 1.41421 * 1.41421 * 1.41421 * E_dp0 );
Ey[ 7 ] = fabs( 1.41421 * 1.41421 * 1.41421 * E_dp1 );
Ey[ 8 ] = fabs( 1.41421 * 1.41421 * E_dp2 );
Ey[ 9 ] = fabs( 1.41421 * 1.41421 * E_dp3 );
Ey[ 10 ] = fabs( 1.41421 * E_dp4 );
Ey[ 11 ] = fabs( 1.41421 * E_dp5 );
Ey[ 12 ] = fabs( E_dp6 );
Ey[ 13 ] = fabs( E_y );
Eout[ 0 ] = 0;
for( int index = 0 ; index < 14; index++ ){ 
Eout[ 0 ] += Ey [  index  ];
}

if ( ( Eout[ 0 ] > USER_OPTIONS->out_err_spec[ 0 ] ))
	{return 0;}
 else
	{return 1;}


}
 double cost_mul( int range_bw_A , int prec_bw_A , int range_bw_B , int prec_bw_B , int range_bw_OUT , int prec_bw_OUT ){ 
 double tot_A , tot_B, tot_OUT; 
 tot_A = range_bw_A + prec_bw_A; 
 tot_B = range_bw_B + prec_bw_B; 
 tot_OUT = range_bw_OUT + prec_bw_OUT;
 return max( tot_A , tot_B); 
 }
 double cost_addsub( int int_bw_A , int frac_bw_A , int int_bw_B , int frac_bw_B , int int_bw_OUT , int frac_bw_OUT ){ 
 double C[] = { 2.1767377326188989e+02, 1.1405056880924322e+02, -2.4861884792968281e+03};
 double frac_bw , int_bw;
 frac_bw = max( frac_bw_A , frac_bw_B );
 int_bw = max( int_bw_A , int_bw_B );
 return ( C[0]*int_bw + C[1]*frac_bw + C[2] );
 }



double cost_function(double *x,
		double *parameter_lower_bound,
		double *parameter_upper_bound,
		double *cost_tangents,
		double *cost_curvature,
		long int * parameter_dimension,
		int *parameter_int_real,
		int *cost_flag, int *exit_code, USER_DEFINES * USER_OPTIONS)
{
 int bw_x = static_cast<int>( x[0] );
int bw_c4 = static_cast<int>( x[1] );
int bw_c3 = static_cast<int>( x[2] );
int bw_c2 = static_cast<int>( x[3] );
int bw_c1 = static_cast<int>( x[4] );
int bw_c0 = static_cast<int>( x[5] );
int bw_dp0 = static_cast<int>( x[6] );
int bw_dp1 = static_cast<int>( x[7] );
int bw_dp2 = static_cast<int>( x[8] );
int bw_dp3 = static_cast<int>( x[9] );
int bw_dp4 = static_cast<int>( x[10] );
int bw_dp5 = static_cast<int>( x[11] );
int bw_dp6 = static_cast<int>( x[12] );
int bw_y = 16;


*cost_flag = error_function( x , parameter_dimension , parameter_int_real , USER_OPTIONS );

double *Cost = new double[ 8 ];
if ( *cost_flag ) {

Cost[ 0 ]  = cost_mul( 4 , bw_c4, 0 , bw_x, 3 , bw_dp0 );
Cost[ 1 ] = cost_addsub( 3 , bw_dp0 , 3 , bw_c3 , 2 , bw_dp1 );
Cost[ 2 ]  = cost_mul( 0 , bw_x , 2 , bw_dp1 , 2 , bw_dp2 );
Cost[ 3 ] = cost_addsub( 2 , bw_dp2 , 2 , bw_c2 , 1 , bw_dp3 );
Cost[ 4 ]  = cost_mul( 0 , bw_x , 1 , bw_dp3 , 0 , bw_dp4 );
Cost[ 5 ] = cost_addsub( 0 , bw_dp4 , 1 , bw_c1 , 1 , bw_dp5 );
Cost[ 6 ]  = cost_mul( 0 , bw_x , 1 , bw_dp5 , 1 , bw_dp6 );
Cost[ 7 ] = cost_addsub( 1 , bw_dp6 , 5 , bw_c0 , 1 , bw_y );

double Total_Cost = 0;
for (int index = 0; index <  8; ++index)
	Total_Cost += Cost[index];
return Total_Cost;
}
else
{
return 0;
}

}
int setup_parameters( long int *param_dim , USER_DEFINES * USER_OPTIONS )
{
*param_dim = 13;
USER_OPTIONS->out_dim = new int;
*(USER_OPTIONS->out_dim) = 1;
USER_OPTIONS->out_err_spec = new double[*(USER_OPTIONS->out_dim)];
USER_OPTIONS->out_err_spec[ 0 ] =  pow( 2. , -16 );
return 0;

}

}
