#include "myasa.h"


extern "C" { 

int error_function(double *x, long int*in_para_dim, int *in_para_int_real ,USER_DEFINES * USER_OPTIONS )
{
double E_u_in, E_u, E_const_six, E_const_three, E_const_one, E_const_four, E_u2p, E_u3p, E_u3t, E_u2p3t, E_u2p6t, E_u3p3t, E_b01, E_b02, E_b03, E_b11, E_b12, E_b21, E_b22, E_b23, ;
double E_b0;
double E_b1;
double E_b2;
double E_b3;
double Eb0[ 11 ];
double Eb1[ 10 ];
double Eb2[ 12 ];
double Eb3[ 2 ];
double Eout[ 4 ];


E_u_in = 0;
E_u = pow( 2. , 0 - pow( 2. , -x[ 0 ] - 1 ) ) - pow( 2. , 0 - pow( 2. , -x[ 0 ]  ) );
E_const_six = pow( 2. , 1 - pow( 2. , -x[ 1 ] - 1 ) ) - pow( 2. , 1 - pow( 2. , -x[ 1 ]  ) );
E_const_three = pow( 2. , 0 - pow( 2. , -x[ 2 ] - 1 ) ) - pow( 2. , 0 - pow( 2. , -x[ 2 ]  ) );
E_const_one = pow( 2. , 0 - pow( 2. , -x[ 3 ] - 1 ) ) - pow( 2. , 0 - pow( 2. , -x[ 3 ]  ) );
E_const_four = pow( 2. , 1 - pow( 2. , -x[ 4 ] - 1 ) ) - pow( 2. , 1 - pow( 2. , -x[ 4 ]  ) );
E_u2p = pow( 2. , 0 - pow( 2. , -x[ 5 ] - 1 ) ) - pow( 2. , 0 - pow( 2. , -x[ 5 ]  ) );
E_u3p = pow( 2. , 0 - pow( 2. , -x[ 6 ] - 1 ) ) - pow( 2. , 0 - pow( 2. , -x[ 6 ]  ) );
E_u3t = pow( 2. , 0 - pow( 2. , -x[ 7 ] - 1 ) ) - pow( 2. , 0 - pow( 2. , -x[ 7 ]  ) );
E_u2p3t = pow( 2. , 0 - pow( 2. , -x[ 8 ] - 1 ) ) - pow( 2. , 0 - pow( 2. , -x[ 8 ]  ) );
E_u2p6t = pow( 2. , 0 - pow( 2. , -x[ 9 ] - 1 ) ) - pow( 2. , 0 - pow( 2. , -x[ 9 ]  ) );
E_u3p3t = pow( 2. , 0 - pow( 2. , -x[ 10 ] - 1 ) ) - pow( 2. , 0 - pow( 2. , -x[ 10 ]  ) );
E_b01 = pow( 2. , 0 - pow( 2. , -x[ 11 ] - 1 ) ) - pow( 2. , 0 - pow( 2. , -x[ 11 ]  ) );
E_b02 = pow( 2. , 0 - pow( 2. , -x[ 12 ] - 1 ) ) - pow( 2. , 0 - pow( 2. , -x[ 12 ]  ) );
E_b03 = pow( 2. , 0 - pow( 2. , -x[ 13 ] - 1 ) ) - pow( 2. , 0 - pow( 2. , -x[ 13 ]  ) );
E_b11 = pow( 2. , 0 - pow( 2. , -x[ 14 ] - 1 ) ) - pow( 2. , 0 - pow( 2. , -x[ 14 ]  ) );
E_b12 = pow( 2. , 1 - pow( 2. , -x[ 15 ] - 1 ) ) - pow( 2. , 1 - pow( 2. , -x[ 15 ]  ) );
E_b21 = pow( 2. , 0 - pow( 2. , -x[ 16 ] - 1 ) ) - pow( 2. , 0 - pow( 2. , -x[ 16 ]  ) );
E_b22 = pow( 2. , 0 - pow( 2. , -x[ 17 ] - 1 ) ) - pow( 2. , 0 - pow( 2. , -x[ 17 ]  ) );
E_b23 = pow( 2. , 0 - pow( 2. , -x[ 18 ] - 1 ) ) - pow( 2. , 0 - pow( 2. , -x[ 18 ]  ) );

E_b0 = 0;
E_b1 = 0;
E_b2 = 0;
E_b3 = 0;

Eb0[ 0 ] = fabs( 2.58496 * (0 * E_const_three + 0 * E_const_three) );
Eb0[ 1 ] = fabs( 0 * E_const_six );
Eb0[ 2 ] = fabs( 2.58496 * E_const_one );
Eb0[ 3 ] = fabs( 2.58496 * (1.58496 * E_u2p + 0 * E_u2p) );
Eb0[ 4 ] = fabs( 2.58496 * E_u3t );
Eb0[ 5 ] = fabs( 2.58496 * E_u3p );
Eb0[ 6 ] = fabs( 2.58496 * E_u2p3t );
Eb0[ 7 ] = fabs( 2.58496 * E_b01 );
Eb0[ 8 ] = fabs( 2.58496 * E_b02 );
Eb0[ 9 ] = fabs( 2.58496 * E_b03 );
Eb0[ 10 ] = fabs( E_b0 );
Eout[ 0 ] = 0;
for( int index = 0 ; index < 11; index++ ){ 
Eout[ 0 ] += Eb0 [  index  ];
}

Eb1[ 0 ] = fabs( 2.58496 * 0 * E_const_three );
Eb1[ 1 ] = fabs( ( 2.58496 * 0 * E_const_six + 2 * E_const_six ) );
Eb1[ 2 ] = fabs( 2.58496 * E_const_four );
Eb1[ 3 ] = fabs( 2.58496 * (1.58496 * 0 * E_u2p + 2.58496 * E_u2p) );
Eb1[ 4 ] = fabs( 2.58496 * 1.58496 * E_u3p );
Eb1[ 5 ] = fabs( 2.58496 * E_u2p6t );
Eb1[ 6 ] = fabs( 2.58496 * E_u3p3t );
Eb1[ 7 ] = fabs( 2.58496 * E_b11 );
Eb1[ 8 ] = fabs( 2.58496 * E_b12 );
Eb1[ 9 ] = fabs( E_b1 );
Eout[ 1 ] = 0;
for( int index = 0 ; index < 10; index++ ){ 
Eout[ 1 ] += Eb1 [  index  ];
}

Eb2[ 0 ] = fabs( 2.58496 * ((0 * E_const_three + 0 * E_const_three) + 0 * E_const_three) );
Eb2[ 1 ] = fabs( 0 * E_const_six );
Eb2[ 2 ] = fabs( 2.58496 * E_const_one );
Eb2[ 3 ] = fabs( 2.58496 * (1.58496 * E_u2p + 1.58496 * 0 * E_u2p) );
Eb2[ 4 ] = fabs( 2.58496 * E_u3t );
Eb2[ 5 ] = fabs( 2.58496 * 1.58496 * E_u3p );
Eb2[ 6 ] = fabs( 2.58496 * E_u2p3t );
Eb2[ 7 ] = fabs( 2.58496 * E_u3p3t );
Eb2[ 8 ] = fabs( 2.58496 * E_b21 );
Eb2[ 9 ] = fabs( 2.58496 * E_b22 );
Eb2[ 10 ] = fabs( 2.58496 * E_b23 );
Eb2[ 11 ] = fabs( E_b2 );
Eout[ 2 ] = 0;
for( int index = 0 ; index < 12; index++ ){ 
Eout[ 2 ] += Eb2 [  index  ];
}

Eb3[ 0 ] = fabs( 1.79769e+308 * E_const_six );
Eb3[ 1 ] = fabs( E_b3 );
Eout[ 3 ] = 0;
for( int index = 0 ; index < 2; index++ ){ 
Eout[ 3 ] += Eb3 [  index  ];
}

if ( ( Eout[ 0 ] > USER_OPTIONS->out_err_spec[ 0 ] )
	 && ( Eout[ 1 ] > USER_OPTIONS->out_err_spec[ 1 ] )
	 && ( Eout[ 2 ] > USER_OPTIONS->out_err_spec[ 2 ] )
	 && ( Eout[ 3 ] > USER_OPTIONS->out_err_spec[ 3 ] ))
	{return 0;}
 else
	{return 1;}


}
