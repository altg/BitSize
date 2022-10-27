#include "myasa.h"


extern "C" { 

int error_function(double *x, long int*in_para_dim, int *in_para_int_real ,USER_DEFINES * USER_OPTIONS )
{
double E_a00_in, E_a01_in, E_a10_in, E_a11_in, E_b00_in, E_b01_in, E_b10_in, E_b11_in, E_a00, E_a01, E_a10, E_a11, E_b00, E_b01, E_b10, E_b11, E_p0_temp0, E_p0_temp1, E_p0, E_p1_temp0, E_p1, E_p2_temp0, E_p2, E_p3_temp0, E_p3, E_p4_temp0, E_p4, E_p5_temp0, E_p5_temp1, E_p5, E_p6_temp0, E_p6_temp1, E_p6, E_y00_temp0, E_y00_temp1, E_y11_temp0, E_y11_temp1, ;
double E_y00;
double E_y01;
double E_y10;
double E_y11;
double Ey00[ 19 ];
double Ey01[ 9 ];
double Ey10[ 9 ];
double Ey11[ 19 ];
double Eout[ 4 ];


E_a00_in = 0;
E_a01_in = 0;
E_a10_in = 0;
E_a11_in = 0;
E_b00_in = 0;
E_b01_in = 0;
E_b10_in = 0;
E_b11_in = 0;
E_a00 = pow( 2. , 2 - pow( 2. , -x[ 0 ] - 1 ) ) - pow( 2. , 2 - pow( 2. , -x[ 0 ]  ) );
E_a01 = pow( 2. , 2 - pow( 2. , -x[ 1 ] - 1 ) ) - pow( 2. , 2 - pow( 2. , -x[ 1 ]  ) );
E_a10 = pow( 2. , 2 - pow( 2. , -x[ 2 ] - 1 ) ) - pow( 2. , 2 - pow( 2. , -x[ 2 ]  ) );
E_a11 = pow( 2. , 2 - pow( 2. , -x[ 3 ] - 1 ) ) - pow( 2. , 2 - pow( 2. , -x[ 3 ]  ) );
E_b00 = pow( 2. , 2 - pow( 2. , -x[ 4 ] - 1 ) ) - pow( 2. , 2 - pow( 2. , -x[ 4 ]  ) );
E_b01 = pow( 2. , 2 - pow( 2. , -x[ 5 ] - 1 ) ) - pow( 2. , 2 - pow( 2. , -x[ 5 ]  ) );
E_b10 = pow( 2. , 2 - pow( 2. , -x[ 6 ] - 1 ) ) - pow( 2. , 2 - pow( 2. , -x[ 6 ]  ) );
E_b11 = pow( 2. , 2 - pow( 2. , -x[ 7 ] - 1 ) ) - pow( 2. , 2 - pow( 2. , -x[ 7 ]  ) );
E_p0_temp0 = pow( 2. , 3 - pow( 2. , -x[ 8 ] - 1 ) ) - pow( 2. , 3 - pow( 2. , -x[ 8 ]  ) );
E_p0_temp1 = pow( 2. , 3 - pow( 2. , -x[ 9 ] - 1 ) ) - pow( 2. , 3 - pow( 2. , -x[ 9 ]  ) );
E_p0 = pow( 2. , 6 - pow( 2. , -x[ 10 ] - 1 ) ) - pow( 2. , 6 - pow( 2. , -x[ 10 ]  ) );
E_p1_temp0 = pow( 2. , 3 - pow( 2. , -x[ 11 ] - 1 ) ) - pow( 2. , 3 - pow( 2. , -x[ 11 ]  ) );
E_p1 = pow( 2. , 5 - pow( 2. , -x[ 12 ] - 1 ) ) - pow( 2. , 5 - pow( 2. , -x[ 12 ]  ) );
E_p2_temp0 = pow( 2. , 3 - pow( 2. , -x[ 13 ] - 1 ) ) - pow( 2. , 3 - pow( 2. , -x[ 13 ]  ) );
E_p2 = pow( 2. , 5 - pow( 2. , -x[ 14 ] - 1 ) ) - pow( 2. , 5 - pow( 2. , -x[ 14 ]  ) );
E_p3_temp0 = pow( 2. , 3 - pow( 2. , -x[ 15 ] - 1 ) ) - pow( 2. , 3 - pow( 2. , -x[ 15 ]  ) );
E_p3 = pow( 2. , 5 - pow( 2. , -x[ 16 ] - 1 ) ) - pow( 2. , 5 - pow( 2. , -x[ 16 ]  ) );
E_p4_temp0 = pow( 2. , 3 - pow( 2. , -x[ 17 ] - 1 ) ) - pow( 2. , 3 - pow( 2. , -x[ 17 ]  ) );
E_p4 = pow( 2. , 5 - pow( 2. , -x[ 18 ] - 1 ) ) - pow( 2. , 5 - pow( 2. , -x[ 18 ]  ) );
E_p5_temp0 = pow( 2. , 3 - pow( 2. , -x[ 19 ] - 1 ) ) - pow( 2. , 3 - pow( 2. , -x[ 19 ]  ) );
E_p5_temp1 = pow( 2. , 3 - pow( 2. , -x[ 20 ] - 1 ) ) - pow( 2. , 3 - pow( 2. , -x[ 20 ]  ) );
E_p5 = pow( 2. , 6 - pow( 2. , -x[ 21 ] - 1 ) ) - pow( 2. , 6 - pow( 2. , -x[ 21 ]  ) );
E_p6_temp0 = pow( 2. , 3 - pow( 2. , -x[ 22 ] - 1 ) ) - pow( 2. , 3 - pow( 2. , -x[ 22 ]  ) );
E_p6_temp1 = pow( 2. , 3 - pow( 2. , -x[ 23 ] - 1 ) ) - pow( 2. , 3 - pow( 2. , -x[ 23 ]  ) );
E_p6 = pow( 2. , 6 - pow( 2. , -x[ 24 ] - 1 ) ) - pow( 2. , 6 - pow( 2. , -x[ 24 ]  ) );
E_y00_temp0 = pow( 2. , 6 - pow( 2. , -x[ 25 ] - 1 ) ) - pow( 2. , 6 - pow( 2. , -x[ 25 ]  ) );
E_y00_temp1 = pow( 2. , 6 - pow( 2. , -x[ 26 ] - 1 ) ) - pow( 2. , 6 - pow( 2. , -x[ 26 ]  ) );
E_y11_temp0 = pow( 2. , 6 - pow( 2. , -x[ 27 ] - 1 ) ) - pow( 2. , 6 - pow( 2. , -x[ 27 ]  ) );
E_y11_temp1 = pow( 2. , 6 - pow( 2. , -x[ 28 ] - 1 ) ) - pow( 2. , 6 - pow( 2. , -x[ 28 ]  ) );

E_y00 = 0;
E_y01 = 0;
E_y10 = 0;
E_y11 = 0;

Ey00[ 0 ] = fabs( (8 * E_a00 + 4 * E_a00) );
Ey00[ 1 ] = fabs( (8 * E_a01 + 4 * E_a01) );
Ey00[ 2 ] = fabs( ((8 * E_a11 + 8 * E_a11) + 8 * E_a11) );
Ey00[ 3 ] = fabs( (8 * E_b00 + 4 * E_b00) );
Ey00[ 4 ] = fabs( (4 * E_b10 + 8 * E_b10) );
Ey00[ 5 ] = fabs( (8 * E_b11 + (8 * E_b11 + 8 * E_b11)) );
Ey00[ 6 ] = fabs( 8 * E_p0_temp0 );
Ey00[ 7 ] = fabs( 8 * E_p0_temp1 );
Ey00[ 8 ] = fabs( 4 * E_p3_temp0 );
Ey00[ 9 ] = fabs( 4 * E_p4_temp0 );
Ey00[ 10 ] = fabs( 8 * E_p6_temp0 );
Ey00[ 11 ] = fabs( 8 * E_p6_temp1 );
Ey00[ 12 ] = fabs( E_p0 );
Ey00[ 13 ] = fabs( E_p3 );
Ey00[ 14 ] = fabs( E_p4 );
Ey00[ 15 ] = fabs( E_p6 );
Ey00[ 16 ] = fabs( E_y00_temp0 );
Ey00[ 17 ] = fabs( E_y00_temp1 );
Ey00[ 18 ] = fabs( E_y00 );
Eout[ 0 ] = 0;
for( int index = 0 ; index < 19; index++ ){ 
Eout[ 0 ] += Ey00 [  index  ];
}

Ey01[ 0 ] = fabs( (8 * E_a00 + 4 * E_a00) );
Ey01[ 1 ] = fabs( 4 * E_a01 );
Ey01[ 2 ] = fabs( 4 * E_b01 );
Ey01[ 3 ] = fabs( (4 * E_b11 + 8 * E_b11) );
Ey01[ 4 ] = fabs( 4 * E_p2_temp0 );
Ey01[ 5 ] = fabs( 4 * E_p4_temp0 );
Ey01[ 6 ] = fabs( E_p2 );
Ey01[ 7 ] = fabs( E_p4 );
Ey01[ 8 ] = fabs( E_y01 );
Eout[ 1 ] = 0;
for( int index = 0 ; index < 9; index++ ){ 
Eout[ 1 ] += Ey01 [  index  ];
}

Ey10[ 0 ] = fabs( 4 * E_a10 );
Ey10[ 1 ] = fabs( (4 * E_a11 + 8 * E_a11) );
Ey10[ 2 ] = fabs( (8 * E_b00 + 4 * E_b00) );
Ey10[ 3 ] = fabs( 4 * E_b10 );
Ey10[ 4 ] = fabs( 4 * E_p1_temp0 );
Ey10[ 5 ] = fabs( 4 * E_p3_temp0 );
Ey10[ 6 ] = fabs( E_p1 );
Ey10[ 7 ] = fabs( E_p3 );
Ey10[ 8 ] = fabs( E_y10 );
Eout[ 2 ] = 0;
for( int index = 0 ; index < 9; index++ ){ 
Eout[ 2 ] += Ey10 [  index  ];
}

Ey11[ 0 ] = fabs( ((8 * E_a00 + 8 * E_a00) + 8 * E_a00) );
Ey11[ 1 ] = fabs( (8 * E_a10 + 4 * E_a10) );
Ey11[ 2 ] = fabs( (8 * E_a11 + 4 * E_a11) );
Ey11[ 3 ] = fabs( (8 * E_b00 + (8 * E_b00 + 8 * E_b00)) );
Ey11[ 4 ] = fabs( (4 * E_b01 + 8 * E_b01) );
Ey11[ 5 ] = fabs( (8 * E_b11 + 4 * E_b11) );
Ey11[ 6 ] = fabs( 8 * E_p0_temp0 );
Ey11[ 7 ] = fabs( 8 * E_p0_temp1 );
Ey11[ 8 ] = fabs( 4 * E_p1_temp0 );
Ey11[ 9 ] = fabs( 4 * E_p2_temp0 );
Ey11[ 10 ] = fabs( 8 * E_p5_temp0 );
Ey11[ 11 ] = fabs( 8 * E_p5_temp1 );
Ey11[ 12 ] = fabs( E_p0 );
Ey11[ 13 ] = fabs( E_p1 );
Ey11[ 14 ] = fabs( E_p2 );
Ey11[ 15 ] = fabs( E_p5 );
Ey11[ 16 ] = fabs( E_y11_temp0 );
Ey11[ 17 ] = fabs( E_y11_temp1 );
Ey11[ 18 ] = fabs( E_y11 );
Eout[ 3 ] = 0;
for( int index = 0 ; index < 19; index++ ){ 
Eout[ 3 ] += Ey11 [  index  ];
}

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
 int bw_a00 = static_cast<int>( x[0] );
int bw_a01 = static_cast<int>( x[1] );
int bw_a10 = static_cast<int>( x[2] );
int bw_a11 = static_cast<int>( x[3] );
int bw_b00 = static_cast<int>( x[4] );
int bw_b01 = static_cast<int>( x[5] );
int bw_b10 = static_cast<int>( x[6] );
int bw_b11 = static_cast<int>( x[7] );
int bw_p0_temp0 = static_cast<int>( x[8] );
int bw_p0_temp1 = static_cast<int>( x[9] );
int bw_p0 = static_cast<int>( x[10] );
int bw_p1_temp0 = static_cast<int>( x[11] );
int bw_p1 = static_cast<int>( x[12] );
int bw_p2_temp0 = static_cast<int>( x[13] );
int bw_p2 = static_cast<int>( x[14] );
int bw_p3_temp0 = static_cast<int>( x[15] );
int bw_p3 = static_cast<int>( x[16] );
int bw_p4_temp0 = static_cast<int>( x[17] );
int bw_p4 = static_cast<int>( x[18] );
int bw_p5_temp0 = static_cast<int>( x[19] );
int bw_p5_temp1 = static_cast<int>( x[20] );
int bw_p5 = static_cast<int>( x[21] );
int bw_p6_temp0 = static_cast<int>( x[22] );
int bw_p6_temp1 = static_cast<int>( x[23] );
int bw_p6 = static_cast<int>( x[24] );
int bw_y00_temp0 = static_cast<int>( x[25] );
int bw_y00_temp1 = static_cast<int>( x[26] );
int bw_y11_temp0 = static_cast<int>( x[27] );
int bw_y11_temp1 = static_cast<int>( x[28] );
int bw_y00 = 16;
int bw_y01 = 16;
int bw_y10 = 16;
int bw_y11 = 16;


*cost_flag = error_function( x , parameter_dimension , parameter_int_real , USER_OPTIONS );

double *Cost = new double[ 25 ];
if ( *cost_flag ) {

Cost[ 0 ] = cost_addsub( 2 , bw_a00 , 2 , bw_a11 , 3 , bw_p0_temp0 );
Cost[ 1 ] = cost_addsub( 2 , bw_b00 , 2 , bw_b11 , 3 , bw_p0_temp1 );
Cost[ 2 ]  = cost_mul( 3 , bw_p0_temp0 , 3 , bw_p0_temp1 , 4 , bw_p0 );
Cost[ 3 ] = cost_addsub( 2 , bw_a10 , 2 , bw_a11 , 3 , bw_p1_temp0 );
Cost[ 4 ]  = cost_mul( 3 , bw_p1_temp0 , 2 , bw_b00 , 4 , bw_p1 );
Cost[ 5 ] = cost_addsub( 2 , bw_b01 , 2 , bw_b11 , 3 , bw_p2_temp0 );
Cost[ 6 ]  = cost_mul( 3 , bw_p2_temp0 , 2 , bw_a00 , 4 , bw_p2 );
Cost[ 7 ] = cost_addsub( 2 , bw_b10 , 2 , bw_b00 , 3 , bw_p3_temp0 );
Cost[ 8 ]  = cost_mul( 3 , bw_p3_temp0 , 2 , bw_a11 , 4 , bw_p3 );
Cost[ 9 ] = cost_addsub( 2 , bw_a00 , 2 , bw_a01 , 3 , bw_p4_temp0 );
Cost[ 10 ]  = cost_mul( 3 , bw_p4_temp0 , 2 , bw_b11 , 4 , bw_p4 );
Cost[ 11 ] = cost_addsub( 2 , bw_a10 , 2 , bw_a00 , 3 , bw_p5_temp0 );
Cost[ 12 ] = cost_addsub( 2 , bw_b00 , 2 , bw_b01 , 3 , bw_p5_temp1 );
Cost[ 13 ]  = cost_mul( 3 , bw_p5_temp0 , 3 , bw_p5_temp1 , 4 , bw_p5 );
Cost[ 14 ] = cost_addsub( 2 , bw_a01 , 2 , bw_a11 , 3 , bw_p6_temp0 );
Cost[ 15 ] = cost_addsub( 2 , bw_b10 , 2 , bw_b11 , 3 , bw_p6_temp1 );
Cost[ 16 ]  = cost_mul( 3 , bw_p6_temp0 , 3 , bw_p6_temp1 , 4 , bw_p6 );
Cost[ 17 ] = cost_addsub( 4 , bw_p0 , 4 , bw_p3 , 4 , bw_y00_temp0 );
Cost[ 18 ] = cost_addsub( 4 , bw_p6 , 4 , bw_p4 , 4 , bw_y00_temp1 );
Cost[ 19 ] = cost_addsub( 4 , bw_y00_temp0 , 4 , bw_y00_temp1 , 4 , bw_y00 );
Cost[ 20 ] = cost_addsub( 4 , bw_p2 , 4 , bw_p4 , 4 , bw_y01 );
Cost[ 21 ] = cost_addsub( 4 , bw_p1 , 4 , bw_p3 , 4 , bw_y10 );
Cost[ 22 ] = cost_addsub( 4 , bw_p0 , 4 , bw_p2 , 4 , bw_y11_temp0 );
Cost[ 23 ] = cost_addsub( 4 , bw_p5 , 4 , bw_p1 , 4 , bw_y11_temp1 );
Cost[ 24 ] = cost_addsub( 4 , bw_y11_temp0 , 4 , bw_y11_temp1 , 4 , bw_y11 );

double Total_Cost = 0;
for (int index = 0; index <  25; ++index)
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
*param_dim = 29;
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
