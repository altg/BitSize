#include "myasa.h"


extern "C" { 

int error_function(double *x, long int*in_para_dim, int *in_para_int_real ,USER_DEFINES * USER_OPTIONS )
{
double  E_R_0, E_G_1, E_B_2, E_c_00_6, E_c_01_7, E_c_02_8, E_c_10_9, E_c_11_10, E_c_21_11, E_c_22_12, E_mult_1_13, E_Y_temp0_14, E_Y_temp1_15, E_Y_temp2_16, E_Cb_temp0_18, E_Cb_temp1_19, E_Cb_temp2_20, E_Cr_temp0_22, E_Cr_temp1_23, E_Cr_temp2_24, E_Y_temp3_17, E_Cb_temp3_21, E_Cr_temp3_25, E_Y_3, E_Cb_4, E_Cr_5,;

E_R_0 = pow( 2. , -x[ 0 ] );
E_G_1 = pow( 2. , -x[ 1 ] );
E_B_2 = pow( 2. , -x[ 2 ] );
E_c_00_6 = pow( 2. , -x[ 3 ] );
E_c_01_7 = pow( 2. , -x[ 4 ] );
E_c_02_8 = pow( 2. , -x[ 5 ] );
E_c_10_9 = pow( 2. , -x[ 6 ] );
E_c_11_10 = pow( 2. , -x[ 7 ] );
E_c_21_11 = pow( 2. , -x[ 8 ] );
E_c_22_12 = pow( 2. , -x[ 9 ] );
E_mult_1_13 = pow( 2. , -x[ 10 ] );
E_Y_temp0_14 = pow( 2. , -x[ 11 ] );
E_Y_temp1_15 = pow( 2. , -x[ 12 ] );
E_Y_temp2_16 = pow( 2. , -x[ 13 ] );
E_Cb_temp0_18 = pow( 2. , -x[ 14 ] );
E_Cb_temp1_19 = pow( 2. , -x[ 15 ] );
E_Cb_temp2_20 =  0;
E_Cr_temp0_22 =  0;
E_Cr_temp1_23 = pow( 2. , -x[ 16 ] );
E_Cr_temp2_24 = pow( 2. , -x[ 17 ] );
E_Y_temp3_17 = pow( 2. , -x[ 18 ] );
E_Cb_temp3_21 = pow( 2. , -x[ 19 ] );
E_Cr_temp3_25 = pow( 2. , -x[ 20 ] );
E_Y_3 =  0;
E_Cb_4 =  0;
E_Cr_5 =  0;


double Eout[ 3 ];
double Et_Y_3[ 23 ];
double Et_Cb_4[ 24 ];
double Et_Cr_5[ 25 ];

Et_Y_3[ 0 ] = fabs( E_R_0 * 0.299 ) ;
Et_Y_3[ 1 ] = fabs( E_G_1 * 0.578 ) ;
Et_Y_3[ 2 ] = fabs( E_B_2 * 0.114 ) ;
Et_Y_3[ 3 ] = fabs( E_c_00_6 * 3.99815 ) ;
Et_Y_3[ 4 ] = fabs( E_c_01_7 * 3.9606 ) ;
Et_Y_3[ 5 ] = fabs( E_c_02_8 * 5.1657 ) ;
Et_Y_3[ 6 ] = fabs( E_c_10_9 * 0 ) ;
Et_Y_3[ 7 ] = fabs( E_c_11_10 * 0 ) ;
Et_Y_3[ 8 ] = fabs( E_c_21_11 * 0 ) ;
Et_Y_3[ 9 ] = fabs( E_c_22_12 * 0 ) ;
Et_Y_3[ 10 ] = fabs( E_mult_1_13 * 0 ) ;
Et_Y_3[ 11 ] = fabs( E_Y_temp0_14 * 1 ) ;
Et_Y_3[ 12 ] = fabs( E_Y_temp1_15 * 1 ) ;
Et_Y_3[ 13 ] = fabs( E_Y_temp2_16 * 1 ) ;
Et_Y_3[ 14 ] = fabs( E_Cb_temp0_18 * 0 ) ;
Et_Y_3[ 15 ] = fabs( E_Cb_temp1_19 * 0 ) ;
Et_Y_3[ 16 ] = fabs( E_Cb_temp2_20 * 0 ) ;
Et_Y_3[ 17 ] = fabs( E_Cr_temp0_22 * 0 ) ;
Et_Y_3[ 18 ] = fabs( E_Cr_temp1_23 * 0 ) ;
Et_Y_3[ 19 ] = fabs( E_Cr_temp2_24 * 0 ) ;
Et_Y_3[ 20 ] = fabs( E_Y_temp3_17 * 1 ) ;
Et_Y_3[ 21 ] = fabs( E_Cb_temp3_21 * 0 ) ;
Et_Y_3[ 22 ] = fabs( E_Cr_temp3_25 * 0 ) ;
Eout[ 0 ] = 0.;
for( int ndx = 0 ; ndx < 23 ; ndx++ )
	Eout[ 0 ] += Et_Y_3[ ndx ];
Et_Cb_4[ 0 ] = fabs( E_R_0 * -0.16875 ) ;
Et_Cb_4[ 1 ] = fabs( E_G_1 * -0.33126 ) ;
Et_Cb_4[ 2 ] = fabs( E_B_2 * 1 ) ;
Et_Cb_4[ 3 ] = fabs( E_c_00_6 * 0 ) ;
Et_Cb_4[ 4 ] = fabs( E_c_01_7 * 0 ) ;
Et_Cb_4[ 5 ] = fabs( E_c_02_8 * 0 ) ;
Et_Cb_4[ 6 ] = fabs( E_c_10_9 * 3.99815 ) ;
Et_Cb_4[ 7 ] = fabs( E_c_11_10 * 3.9606 ) ;
Et_Cb_4[ 8 ] = fabs( E_c_21_11 * 0 ) ;
Et_Cb_4[ 9 ] = fabs( E_c_22_12 * 0 ) ;
Et_Cb_4[ 10 ] = fabs( E_mult_1_13 * 5.1657 ) ;
Et_Cb_4[ 11 ] = fabs( E_Y_temp0_14 * 0 ) ;
Et_Cb_4[ 12 ] = fabs( E_Y_temp1_15 * 0 ) ;
Et_Cb_4[ 13 ] = fabs( E_Y_temp2_16 * 0 ) ;
Et_Cb_4[ 14 ] = fabs( E_Cb_temp0_18 * 1 ) ;
Et_Cb_4[ 15 ] = fabs( E_Cb_temp1_19 * 1 ) ;
Et_Cb_4[ 16 ] = fabs( E_Cb_temp2_20 * 1 ) ;
Et_Cb_4[ 17 ] = fabs( E_Cr_temp0_22 * 0 ) ;
Et_Cb_4[ 18 ] = fabs( E_Cr_temp1_23 * 0 ) ;
Et_Cb_4[ 19 ] = fabs( E_Cr_temp2_24 * 0 ) ;
Et_Cb_4[ 20 ] = fabs( E_Y_temp3_17 * 0 ) ;
Et_Cb_4[ 21 ] = fabs( E_Cb_temp3_21 * 1 ) ;
Et_Cb_4[ 22 ] = fabs( E_Cr_temp3_25 * 0 ) ;
Et_Cb_4[ 23 ] = fabs( E_Y_3 * 0 ) ;
Eout[ 1 ] = 0.;
for( int ndx = 0 ; ndx < 24 ; ndx++ )
	Eout[ 1 ] += Et_Cb_4[ ndx ];
Et_Cr_5[ 0 ] = fabs( E_R_0 * 1 ) ;
Et_Cr_5[ 1 ] = fabs( E_G_1 * -0.41869 ) ;
Et_Cr_5[ 2 ] = fabs( E_B_2 * -0.08131 ) ;
Et_Cr_5[ 3 ] = fabs( E_c_00_6 * 0 ) ;
Et_Cr_5[ 4 ] = fabs( E_c_01_7 * 0 ) ;
Et_Cr_5[ 5 ] = fabs( E_c_02_8 * 0 ) ;
Et_Cr_5[ 6 ] = fabs( E_c_10_9 * 0 ) ;
Et_Cr_5[ 7 ] = fabs( E_c_11_10 * 0 ) ;
Et_Cr_5[ 8 ] = fabs( E_c_21_11 * 3.9606 ) ;
Et_Cr_5[ 9 ] = fabs( E_c_22_12 * 5.1657 ) ;
Et_Cr_5[ 10 ] = fabs( E_mult_1_13 * 3.99815 ) ;
Et_Cr_5[ 11 ] = fabs( E_Y_temp0_14 * 0 ) ;
Et_Cr_5[ 12 ] = fabs( E_Y_temp1_15 * 0 ) ;
Et_Cr_5[ 13 ] = fabs( E_Y_temp2_16 * 0 ) ;
Et_Cr_5[ 14 ] = fabs( E_Cb_temp0_18 * 0 ) ;
Et_Cr_5[ 15 ] = fabs( E_Cb_temp1_19 * 0 ) ;
Et_Cr_5[ 16 ] = fabs( E_Cb_temp2_20 * 0 ) ;
Et_Cr_5[ 17 ] = fabs( E_Cr_temp0_22 * 1 ) ;
Et_Cr_5[ 18 ] = fabs( E_Cr_temp1_23 * 1 ) ;
Et_Cr_5[ 19 ] = fabs( E_Cr_temp2_24 * 1 ) ;
Et_Cr_5[ 20 ] = fabs( E_Y_temp3_17 * 0 ) ;
Et_Cr_5[ 21 ] = fabs( E_Cb_temp3_21 * 0 ) ;
Et_Cr_5[ 22 ] = fabs( E_Cr_temp3_25 * 1 ) ;
Et_Cr_5[ 23 ] = fabs( E_Y_3 * 0 ) ;
Et_Cr_5[ 24 ] = fabs( E_Cb_4 * 0 ) ;
Eout[ 2 ] = 0.;
for( int ndx = 0 ; ndx < 25 ; ndx++ )
	Eout[ 2 ] += Et_Cr_5[ ndx ];

if ( ( Eout[ 0 ] > USER_OPTIONS->out_err_spec[ 0 ] )
	 && ( Eout[ 1 ] > USER_OPTIONS->out_err_spec[ 1 ] )
	 && ( Eout[ 2 ] > USER_OPTIONS->out_err_spec[ 2 ] ))
	{return 0;}
 else
	{return 1;}

}
 double cost_mul( int range_bw_A , int prec_bw_A , int range_bw_B , int prec_bw_B , int range_bw_OUT , int prec_bw_OUT ){ 
 return ( range_bw_A + prec_bw_A ) * (  range_bw_B +  prec_bw_B );
 }

 double cost_addsub( int range_bw_A , int prec_bw_A , int range_bw_B , int prec_bw_B , int range_bw_OUT , int prec_bw_OUT ){ 
 double tot_A , tot_B, tot_OUT; 
 tot_A = range_bw_A + prec_bw_A; 
 tot_B = range_bw_B + prec_bw_B; 
 tot_OUT = range_bw_OUT + prec_bw_OUT;
 return max( tot_A , tot_B); 
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
 int bw_R = static_cast<int>( x[0] );
int bw_G = static_cast<int>( x[1] );
int bw_B = static_cast<int>( x[2] );
int bw_c_00 = static_cast<int>( x[3] );
int bw_c_01 = static_cast<int>( x[4] );
int bw_c_02 = static_cast<int>( x[5] );
int bw_c_10 = static_cast<int>( x[6] );
int bw_c_11 = static_cast<int>( x[7] );
int bw_c_21 = static_cast<int>( x[8] );
int bw_c_22 = static_cast<int>( x[9] );
int bw_mult_1 = static_cast<int>( x[10] );
int bw_Y_temp0 = static_cast<int>( x[11] );
int bw_Y_temp1 = static_cast<int>( x[12] );
int bw_Y_temp2 = static_cast<int>( x[13] );
int bw_Y_temp3 = static_cast<int>( x[14] );
int bw_Cb_temp0 = static_cast<int>( x[15] );
int bw_Cb_temp1 = static_cast<int>( x[16] );
int bw_Cb_temp2 = 8;
int bw_Cb_temp3 = static_cast<int>( x[17] );
int bw_Cr_temp0 = 8;
int bw_Cr_temp1 = static_cast<int>( x[18] );
int bw_Cr_temp2 = static_cast<int>( x[19] );
int bw_Cr_temp3 = static_cast<int>( x[20] );
int bw_Y = 16;
int bw_Cb = 16;
int bw_Cr = 16;


*cost_flag = error_function( x , parameter_dimension , parameter_int_real , USER_OPTIONS );

double *Cost = new double[ 15 ];
if ( *cost_flag ) {

Cost[ 0 ]   = cost_mul( 2 , bw_c_00, 4 , bw_R, 2 , bw_Y_temp0 );
Cost[ 1 ]   = cost_mul( 2 , bw_c_01, 4 , bw_G, 3 , bw_Y_temp1 );
Cost[ 2 ]   = cost_mul( 2 , bw_c_02, 4 , bw_B, 2 , bw_Y_temp2 );
Cost[ 3 ] = cost_addsub( 2 , bw_Y_temp0 , 3 , bw_Y_temp1 , 3 , bw_Y_temp3 );
Cost[ 4 ] = cost_addsub( 2 , bw_Y_temp2 , 3 , bw_Y_temp3 , 3 , bw_Y );
Cost[ 5 ]   = cost_mul( 1 , bw_c_10, 4 , bw_R, 2 , bw_Cb_temp0 );
Cost[ 6 ]   = cost_mul( 1 , bw_c_11, 4 , bw_G, 2 , bw_Cb_temp1 );
Cost[ 7 ]  = cost_mul( 1 , bw_mult_1, 4 , bw_B, 0 , bw_Cb_temp2 );
Cost[ 8 ] = cost_addsub( 2 , bw_Cb_temp0 , 2 , bw_Cb_temp1 , 2 , bw_Cb_temp3 );
Cost[ 9 ] = cost_addsub( 0 , bw_Cb_temp2 , 2 , bw_Cb_temp3 , 4 , bw_Cb );
Cost[ 10 ]  = cost_mul( 1 , bw_mult_1, 4 , bw_R, 0 , bw_Cr_temp0 );
Cost[ 11 ]   = cost_mul( 1 , bw_c_21, 4 , bw_G, 2 , bw_Cr_temp1 );
Cost[ 12 ]   = cost_mul( 1 , bw_c_22, 4 , bw_B, 2 , bw_Cr_temp2 );
Cost[ 13 ] = cost_addsub( 0 , bw_Cr_temp0 , 2 , bw_Cr_temp1 , 4 , bw_Cr_temp3 );
Cost[ 14 ] = cost_addsub( 2 , bw_Cr_temp2 , 4 , bw_Cr_temp3 , 4 , bw_Cr );

double Total_Cost = 0;
for (int index = 0; index <  15; ++index)
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
*param_dim = 21;
USER_OPTIONS->out_dim = new int;
*(USER_OPTIONS->out_dim) = 3;
USER_OPTIONS->out_err_spec = new double[*(USER_OPTIONS->out_dim)];
USER_OPTIONS->out_err_spec[ 0 ] =  pow( 2. , -16 );
USER_OPTIONS->out_err_spec[ 1 ] =  pow( 2. , -16 );
USER_OPTIONS->out_err_spec[ 2 ] =  pow( 2. , -16 );
return 0;

}

}
