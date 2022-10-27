#include "myasa.h"


extern "C" { 

int error_function(double *x, long int*in_para_dim, int *in_para_int_real ,USER_DEFINES * USER_OPTIONS )
{
double E_a_in, E_b_in, E_a, E_b, ;
double E_y;
double Ey[ 3 ];
double Eout[ 1 ];


E_a_in = 0;
E_b_in = 0;
E_a = pow( 2. , 0 - pow( 2. , -x[ 0 ] - 1 ) ) - pow( 2. , 0 - pow( 2. , -x[ 0 ]  ) );
E_b = pow( 2. , 0 - pow( 2. , -x[ 1 ] - 1 ) ) - pow( 2. , 0 - pow( 2. , -x[ 1 ]  ) );

E_y = 0;

Ey[ 0 ] = fabs( E_a );
Ey[ 1 ] = fabs( E_b );
Ey[ 2 ] = fabs( E_y );
Eout[ 0 ] = 0;
for( int index = 0 ; index < 3; index++ ){ 
Eout[ 0 ] += Ey [  index  ];
}

cout << Eout[ 0 ] << " :: " << USER_OPTIONS->out_err_spec[ 0 ] << endl;
if ( ( Eout[ 0 ] > USER_OPTIONS->out_err_spec[ 0 ] ))
	{return 0;}
 else
	{return 1;}


}
 double cost_mul( int int_bw_A , int frac_bw_A , int int_bw_B , int frac_bw_B , int int_bw_OUT , int frac_bw_OUT ){ 
 double C[] = {1.9772 , 4.7262 , 99.0829 };
 double X[2];
 X[0] = frac_bw_A * frac_bw_B;
 X[1]= max( int_bw_A , int_bw_B );
 return ( C[0]*X[0] + C[1]*X[1] + C[2]);
 }

 double cost_addsub( int int_bw_A , int frac_bw_A , int int_bw_B , int frac_bw_B , int int_bw_OUT , int frac_bw_OUT ){ 
 double C[] = {0.2895, 18.2743 , 147.8651 };
 double X[1];
 double frac_bw , int_bw;
 frac_bw = max( frac_bw_A , frac_bw_B );
 int_bw = max( int_bw_A , int_bw_B );
 X[0] = frac_bw * int_bw; 
 return ( C[0]*X[0] + C[1]*frac_bw + C[2] );
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
 int bw_a = static_cast<int>( x[0] );
int bw_b = static_cast<int>( x[1] );
int bw_y = 5;


*cost_flag = error_function( x , parameter_dimension , parameter_int_real , USER_OPTIONS );

double *Cost = new double[ 1 ];
if ( *cost_flag ) {

Cost[ 0 ] = cost_addsub( 0 , bw_a , 0 , bw_b , 9994 , bw_y );

double Total_Cost = 0;
for (int index = 0; index <  1; ++index)
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
*param_dim = 2;
USER_OPTIONS->out_dim = new int;
*(USER_OPTIONS->out_dim) = 1;
USER_OPTIONS->out_err_spec = new double[*(USER_OPTIONS->out_dim)];
USER_OPTIONS->out_err_spec[ 0 ] =  pow( 2. , -5 );
return 0;

}

}
