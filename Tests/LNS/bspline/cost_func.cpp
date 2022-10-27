#include "myasa.h"


extern "C" { 

int error_function(double *x, long int*in_para_dim, int *in_para_int_real ,USER_DEFINES * USER_OPTIONS )
{
double  E_u_1, E_my_two_29, E_my_1o6_13, E_my_one_14, E_my_one_1_15, E_my_four_16, E_my_zero_17, E_u_p2_2, E_u_t2_4, E_u_p3_3, E_u_t3_5, E_u_p2_t2_6, E_B0_temp0_18, E_u_p2_t3_7, E_u_p2_t6_temp0_8, E_u_p3_t3_temp0_11, E_B0_temp1_19, E_B3_temp0_28, E_u_p2_t6_temp1_9, E_u_p3_t3_12, E_B0_temp2_20, E_B3_out_33, E_u_p2_t6_10, E_B0_temp3_21, E_B2_temp0_25, E_B0_temp4_22, E_B1_temp0_23, E_B2_temp1_26, E_B0_out_30, E_B1_temp1_24, E_B2_temp2_27, E_B1_out_31, E_B2_out_32,;

E_u_1 = pow( 2. , 1 + pow( 2. , -x[ 0 ] - 1 ) ) - pow( 2. , 1 + pow( 2. , -x[ 0 ]  ) );
E_my_two_29 =  0;
E_my_1o6_13 = pow( 2. , -2 + pow( 2. , -x[ 1 ] - 1 ) ) - pow( 2. , -2 + pow( 2. , -x[ 1 ]  ) );
E_my_one_14 =  0;
E_my_one_1_15 =  0;
E_my_four_16 =  0;
E_my_zero_17 =  0;
E_u_p2_2 = pow( 2. , 3 + pow( 2. , -x[ 2 ] - 1 ) ) - pow( 2. , 3 + pow( 2. , -x[ 2 ]  ) );
E_u_t2_4 = pow( 2. , 2 + pow( 2. , -x[ 3 ] - 1 ) ) - pow( 2. , 2 + pow( 2. , -x[ 3 ]  ) );
E_u_p3_3 = pow( 2. , 5 + pow( 2. , -x[ 4 ] - 1 ) ) - pow( 2. , 5 + pow( 2. , -x[ 4 ]  ) );
E_u_t3_5 = pow( 2. , 3 + pow( 2. , -x[ 5 ] - 1 ) ) - pow( 2. , 3 + pow( 2. , -x[ 5 ]  ) );
E_u_p2_t2_6 = pow( 2. , 4 + pow( 2. , -x[ 6 ] - 1 ) ) - pow( 2. , 4 + pow( 2. , -x[ 6 ]  ) );
E_B0_temp0_18 = pow( 2. , 3 + pow( 2. , -x[ 7 ] - 1 ) ) - pow( 2. , 3 + pow( 2. , -x[ 7 ]  ) );
E_u_p2_t3_7 = pow( 2. , 5 + pow( 2. , -x[ 8 ] - 1 ) ) - pow( 2. , 5 + pow( 2. , -x[ 8 ]  ) );
E_u_p2_t6_temp0_8 = pow( 2. , 5 + pow( 2. , -x[ 9 ] - 1 ) ) - pow( 2. , 5 + pow( 2. , -x[ 9 ]  ) );
E_u_p3_t3_temp0_11 = pow( 2. , 6 + pow( 2. , -x[ 10 ] - 1 ) ) - pow( 2. , 6 + pow( 2. , -x[ 10 ]  ) );
E_B0_temp1_19 = pow( 2. , 4 + pow( 2. , -x[ 11 ] - 1 ) ) - pow( 2. , 4 + pow( 2. , -x[ 11 ]  ) );
E_B3_temp0_28 = pow( 2. , 5 + pow( 2. , -x[ 12 ] - 1 ) ) - pow( 2. , 5 + pow( 2. , -x[ 12 ]  ) );
E_u_p2_t6_temp1_9 = pow( 2. , 6 + pow( 2. , -x[ 13 ] - 1 ) ) - pow( 2. , 6 + pow( 2. , -x[ 13 ]  ) );
E_u_p3_t3_12 = pow( 2. , 7 + pow( 2. , -x[ 14 ] - 1 ) ) - pow( 2. , 7 + pow( 2. , -x[ 14 ]  ) );
E_B0_temp2_20 = pow( 2. , 4 + pow( 2. , -x[ 15 ] - 1 ) ) - pow( 2. , 4 + pow( 2. , -x[ 15 ]  ) );
E_B3_out_33 =  0;
E_u_p2_t6_10 = pow( 2. , 6 + pow( 2. , -x[ 16 ] - 1 ) ) - pow( 2. , 6 + pow( 2. , -x[ 16 ]  ) );
E_B0_temp3_21 = pow( 2. , 5 + pow( 2. , -x[ 17 ] - 1 ) ) - pow( 2. , 5 + pow( 2. , -x[ 17 ]  ) );
E_B2_temp0_25 = pow( 2. , 7 + pow( 2. , -x[ 18 ] - 1 ) ) - pow( 2. , 7 + pow( 2. , -x[ 18 ]  ) );
E_B0_temp4_22 = pow( 2. , 6 + pow( 2. , -x[ 19 ] - 1 ) ) - pow( 2. , 6 + pow( 2. , -x[ 19 ]  ) );
E_B1_temp0_23 = pow( 2. , 7 + pow( 2. , -x[ 20 ] - 1 ) ) - pow( 2. , 7 + pow( 2. , -x[ 20 ]  ) );
E_B2_temp1_26 = pow( 2. , 7 + pow( 2. , -x[ 21 ] - 1 ) ) - pow( 2. , 7 + pow( 2. , -x[ 21 ]  ) );
E_B0_out_30 =  0;
E_B1_temp1_24 = pow( 2. , 7 + pow( 2. , -x[ 22 ] - 1 ) ) - pow( 2. , 7 + pow( 2. , -x[ 22 ]  ) );
E_B2_temp2_27 = pow( 2. , 7 + pow( 2. , -x[ 23 ] - 1 ) ) - pow( 2. , 7 + pow( 2. , -x[ 23 ]  ) );
E_B1_out_31 =  0;
E_B2_out_32 =  0;


double Eout[ 4 ];
double Et_B0_out_30[ 13 ];
double Et_B1_out_31[ 14 ];
double Et_B2_out_32[ 15 ];
double Et_B3_out_33[ 6 ];

Et_B0_out_30[ 0 ] = fabs( E_u_1 * 0.00669209 ) ;
Et_B0_out_30[ 1 ] = fabs( E_my_two_29 * 0.880756 ) ;
Et_B0_out_30[ 2 ] = fabs( E_my_1o6_13 * 11.24 ) ;
Et_B0_out_30[ 3 ] = fabs( E_my_one_14 * 0.166667 ) ;
Et_B0_out_30[ 4 ] = fabs( E_u_p2_2 * 0.706673 ) ;
Et_B0_out_30[ 5 ] = fabs( E_u_t2_4 * 0.166667 ) ;
Et_B0_out_30[ 6 ] = fabs( E_u_p3_3 * 0.166667 ) ;
Et_B0_out_30[ 7 ] = fabs( E_u_p2_t2_6 * 0.166667 ) ;
Et_B0_out_30[ 8 ] = fabs( E_B0_temp0_18 * 0.166667 ) ;
Et_B0_out_30[ 9 ] = fabs( E_B0_temp1_19 * 0.166667 ) ;
Et_B0_out_30[ 10 ] = fabs( E_B0_temp2_20 * 0.166667 ) ;
Et_B0_out_30[ 11 ] = fabs( E_B0_temp3_21 * 0.166667 ) ;
Et_B0_out_30[ 12 ] = fabs( E_B0_temp4_22 * 0.166667 ) ;
Eout[ 0 ] = 0.;
for( int ndx = 0 ; ndx < 13 ; ndx++ )
	Eout[ 0 ] += Et_B0_out_30[ ndx ];
Et_B1_out_31[ 0 ] = fabs( E_u_1 * 6.50053 ) ;
Et_B1_out_31[ 1 ] = fabs( E_my_two_29 * 0.00145364 ) ;
Et_B1_out_31[ 2 ] = fabs( E_my_1o6_13 * 24.8121 ) ;
Et_B1_out_31[ 3 ] = fabs( E_my_four_16 * 0.166667 ) ;
Et_B1_out_31[ 4 ] = fabs( E_u_p2_2 * 0.42628 ) ;
Et_B1_out_31[ 5 ] = fabs( E_u_p3_3 * 0.5 ) ;
Et_B1_out_31[ 6 ] = fabs( E_u_p2_t2_6 * 0.333333 ) ;
Et_B1_out_31[ 7 ] = fabs( E_u_p2_t6_temp0_8 * 0.166667 ) ;
Et_B1_out_31[ 8 ] = fabs( E_u_p3_t3_temp0_11 * 0.166667 ) ;
Et_B1_out_31[ 9 ] = fabs( E_u_p2_t6_temp1_9 * 0.166667 ) ;
Et_B1_out_31[ 10 ] = fabs( E_u_p3_t3_12 * 0.166667 ) ;
Et_B1_out_31[ 11 ] = fabs( E_u_p2_t6_10 * 0.166667 ) ;
Et_B1_out_31[ 12 ] = fabs( E_B1_temp0_23 * 0.166667 ) ;
Et_B1_out_31[ 13 ] = fabs( E_B1_temp1_24 * 0.166667 ) ;
Eout[ 1 ] = 0.;
for( int ndx = 0 ; ndx < 14 ; ndx++ )
	Eout[ 1 ] += Et_B1_out_31[ ndx ];
Et_B2_out_32[ 0 ] = fabs( E_u_1 * 0.665539 ) ;
Et_B2_out_32[ 1 ] = fabs( E_my_two_29 * 0.367411 ) ;
Et_B2_out_32[ 2 ] = fabs( E_my_1o6_13 * 7.6134 ) ;
Et_B2_out_32[ 3 ] = fabs( E_my_one_1_15 * 0.166667 ) ;
Et_B2_out_32[ 4 ] = fabs( E_u_p2_2 * 1.12002 ) ;
Et_B2_out_32[ 5 ] = fabs( E_u_t2_4 * 0.166667 ) ;
Et_B2_out_32[ 6 ] = fabs( E_u_p3_3 * 0.5 ) ;
Et_B2_out_32[ 7 ] = fabs( E_u_t3_5 * 0.166667 ) ;
Et_B2_out_32[ 8 ] = fabs( E_u_p2_t2_6 * 0.166667 ) ;
Et_B2_out_32[ 9 ] = fabs( E_u_p2_t3_7 * 0.166667 ) ;
Et_B2_out_32[ 10 ] = fabs( E_u_p3_t3_temp0_11 * 0.166667 ) ;
Et_B2_out_32[ 11 ] = fabs( E_u_p3_t3_12 * 0.166667 ) ;
Et_B2_out_32[ 12 ] = fabs( E_B2_temp0_25 * 0.166667 ) ;
Et_B2_out_32[ 13 ] = fabs( E_B2_temp1_26 * 0.166667 ) ;
Et_B2_out_32[ 14 ] = fabs( E_B2_temp2_27 * 0.166667 ) ;
Eout[ 2 ] = 0.;
for( int ndx = 0 ; ndx < 15 ; ndx++ )
	Eout[ 2 ] += Et_B2_out_32[ ndx ];
Et_B3_out_33[ 0 ] = fabs( E_u_1 * 0.00107772 ) ;
Et_B3_out_33[ 1 ] = fabs( E_my_1o6_13 * 1.90681 ) ;
Et_B3_out_33[ 2 ] = fabs( E_my_zero_17 * 0.166667 ) ;
Et_B3_out_33[ 3 ] = fabs( E_u_p2_2 * 0.206673 ) ;
Et_B3_out_33[ 4 ] = fabs( E_u_p3_3 * 0.166667 ) ;
Et_B3_out_33[ 5 ] = fabs( E_B3_temp0_28 * 0.166667 ) ;
Eout[ 3 ] = 0.;
for( int ndx = 0 ; ndx < 6 ; ndx++ )
	Eout[ 3 ] += Et_B3_out_33[ ndx ];

if ( ( Eout[ 0 ] > USER_OPTIONS->out_err_spec[ 0 ] )
	 && ( Eout[ 1 ] > USER_OPTIONS->out_err_spec[ 1 ] )
	 && ( Eout[ 2 ] > USER_OPTIONS->out_err_spec[ 2 ] )
	 && ( Eout[ 3 ] > USER_OPTIONS->out_err_spec[ 3 ] ))
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
 int bw_u = static_cast<int>( x[0] );
int bw_u_p2 = static_cast<int>( x[1] );
int bw_u_p3 = static_cast<int>( x[2] );
int bw_u_t2 = static_cast<int>( x[3] );
int bw_u_t3 = static_cast<int>( x[4] );
int bw_u_p2_t2 = static_cast<int>( x[5] );
int bw_u_p2_t3 = static_cast<int>( x[6] );
int bw_u_p2_t6_temp0 = static_cast<int>( x[7] );
int bw_u_p2_t6_temp1 = static_cast<int>( x[8] );
int bw_u_p2_t6 = static_cast<int>( x[9] );
int bw_u_p3_t3_temp0 = static_cast<int>( x[10] );
int bw_u_p3_t3 = static_cast<int>( x[11] );
int bw_my_1o6 = static_cast<int>( x[12] );
int bw_my_one = 3;
int bw_my_one_1 = 3;
int bw_my_four = 3;
int bw_my_zero = 3;
int bw_B0_temp0 = static_cast<int>( x[13] );
int bw_B0_temp1 = static_cast<int>( x[14] );
int bw_B0_temp2 = static_cast<int>( x[15] );
int bw_B0_temp3 = static_cast<int>( x[16] );
int bw_B0_temp4 = static_cast<int>( x[17] );
int bw_B1_temp0 = static_cast<int>( x[18] );
int bw_B1_temp1 = static_cast<int>( x[19] );
int bw_B2_temp0 = static_cast<int>( x[20] );
int bw_B2_temp1 = static_cast<int>( x[21] );
int bw_B2_temp2 = static_cast<int>( x[22] );
int bw_B3_temp0 = static_cast<int>( x[23] );
int bw_my_two = 3;
int bw_B0_out = 16;
int bw_B1_out = 16;
int bw_B2_out = 16;
int bw_B3_out = 16;


*cost_flag = error_function( x , parameter_dimension , parameter_int_real , USER_OPTIONS );

double *Cost = new double[ 26 ];
if ( *cost_flag ) {

Cost[ 0 ]  = cost_mul( 3 , bw_u , 3 , bw_u , 3 , bw_u_p2 );
Cost[ 1 ]  = cost_mul( 3 , bw_u_p2 , 3 , bw_u , 4 , bw_u_p3 );
Cost[ 2 ]  = cost_mul( 1 , bw_my_two, 3 , bw_u, 2 , bw_u_t2 );
Cost[ 3 ] = cost_addsub( 2 , bw_u_t2 , 3 , bw_u , 3 , bw_u_t3 );
Cost[ 4 ]  = cost_mul( 1 , bw_my_two, 3 , bw_u_p2, 3 , bw_u_p2_t2 );
Cost[ 5 ] = cost_addsub( 3 , bw_u_p2_t2 , 3 , bw_u_p2 , 4 , bw_u_p2_t3 );
Cost[ 6 ]  = cost_mul( 1 , bw_my_two, 3 , bw_u_p2_t2, 4 , bw_u_p2_t6_temp0 );
Cost[ 7 ] = cost_addsub( 4 , bw_u_p2_t6_temp0 , 3 , bw_u_p2 , 4 , bw_u_p2_t6_temp1 );
Cost[ 8 ] = cost_addsub( 4 , bw_u_p2_t6_temp1 , 3 , bw_u_p2 , 4 , bw_u_p2_t6 );
Cost[ 9 ]  = cost_mul( 1 , bw_my_two, 4 , bw_u_p3, 4 , bw_u_p3_t3_temp0 );
Cost[ 10 ] = cost_addsub( 4 , bw_u_p3_t3_temp0 , 4 , bw_u_p3 , 4 , bw_u_p3_t3 );
Cost[ 11 ] = cost_addsub( 1 , bw_my_one , 2 , bw_u_t2 , 3 , bw_B0_temp0 );
Cost[ 12 ] = cost_addsub( 3 , bw_B0_temp0 , 3 , bw_u_p2 , 3 , bw_B0_temp1 );
Cost[ 13 ] = cost_addsub( 3 , bw_B0_temp1 , 3 , bw_u , 3 , bw_B0_temp2 );
Cost[ 14 ] = cost_addsub( 3 , bw_B0_temp2 , 3 , bw_u_p2_t2 , 4 , bw_B0_temp3 );
Cost[ 15 ] = cost_addsub( 4 , bw_B0_temp3 , 4 , bw_u_p3 , 4 , bw_B0_temp4 );
Cost[ 16 ]  = cost_mul( 3 , bw_my_1o6, 4 , bw_B0_temp4, 3 , bw_B0_out );
Cost[ 17 ] = cost_addsub( 4 , bw_u_p3_t3 , 4 , bw_u_p2_t6 , 4 , bw_B1_temp0 );
Cost[ 18 ] = cost_addsub( 4 , bw_B1_temp0 , 1 , bw_my_four , 4 , bw_B1_temp1 );
Cost[ 19 ]  = cost_mul( 3 , bw_my_1o6, 4 , bw_B1_temp1, 4 , bw_B1_out );
Cost[ 20 ] = cost_addsub( 4 , bw_u_p2_t3 , 4 , bw_u_p3_t3 , 4 , bw_B2_temp0 );
Cost[ 21 ] = cost_addsub( 4 , bw_B2_temp0 , 3 , bw_u_t3 , 4 , bw_B2_temp1 );
Cost[ 22 ] = cost_addsub( 4 , bw_B2_temp1 , 1 , bw_my_one_1 , 4 , bw_B2_temp2 );
Cost[ 23 ]  = cost_mul( 3 , bw_my_1o6, 4 , bw_B2_temp2, 3 , bw_B2_out );
Cost[ 24 ] = cost_addsub( 1 , bw_my_zero , 4 , bw_u_p3 , 4 , bw_B3_temp0 );
Cost[ 25 ]  = cost_mul( 3 , bw_my_1o6, 4 , bw_B3_temp0, 3 , bw_B3_out );

double Total_Cost = 0;
for (int index = 0; index <  26; ++index)
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
*param_dim = 24;
USER_OPTIONS->out_dim = new int;
*(USER_OPTIONS->out_dim) = 4;
USER_OPTIONS->out_err_spec = new double[*(USER_OPTIONS->out_dim)];
USER_OPTIONS->out_err_spec[ 0 ] =  pow( 2. , -16 );
USER_OPTIONS->out_err_spec[ 1 ] =  pow( 2. , -16 );
USER_OPTIONS->out_err_spec[ 2 ] =  pow( 2. , -16 );
USER_OPTIONS->out_err_spec[ 3 ] =  pow( 2. , -16 );
return 0;

}

}
