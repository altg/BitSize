//Code PreProcessed by ASC2BitSize
//(c) Altaf Abdul Gaffar

// Cubic B-Splines



// signals to be optimzed: 23

// add/sub: 16

// mult: 6


#include "bitsize.h"




#define OUTFRACBW R_ARGS(0)



int main( int argc , char* argv[] )

{

  

  const double const_1o6 = 0.166666666666666666666667;     // 1/6

  

  STREAM_START( argc , argv );

  

//  DefaultSign = SIGNMAGNITUDE;

//  ROUNDING_CHOICE = NOROUND;

//  STREAM_OPTIMIZE = THROUGHPUT;

//  USE_HARDMULT = NO;

  

	HWlns u_in (  IN  , (fsize)(  OPT_BW + 3  ) , (fsize)(  OPT_BW  ) ,  SIGNMAGNITUDE , "u_in" );

	HWlns u (  TMP  , (fsize)(  OPT_BW + 3  ) , (fsize)(  OPT_BW  ) ,  SIGNMAGNITUDE , "u" );

     

	HWlns u_p2( TMP , OPT_BW , OPT_BW , SIGNMAGNITUDE, "u_p2" );

	HWlns u_p3( TMP , OPT_BW , OPT_BW , SIGNMAGNITUDE, "u_p3" );

	HWlns u_t2( TMP , OPT_BW , OPT_BW , SIGNMAGNITUDE, "u_t2" );

	HWlns u_t3( TMP , OPT_BW , OPT_BW , SIGNMAGNITUDE, "u_t3" );

	HWlns u_p2_t2( TMP , OPT_BW , OPT_BW , SIGNMAGNITUDE, "u_p2_t2" );

	HWlns u_p2_t3( TMP , OPT_BW , OPT_BW , SIGNMAGNITUDE, "u_p2_t3" );

	HWlns u_p2_t6_temp0( TMP , OPT_BW , OPT_BW , SIGNMAGNITUDE, "u_p2_t6_temp0" );

	HWlns u_p2_t6_temp1( TMP , OPT_BW , OPT_BW , SIGNMAGNITUDE, "u_p2_t6_temp1" );

	HWlns u_p2_t6( TMP , OPT_BW , OPT_BW , SIGNMAGNITUDE, "u_p2_t6" );

	HWlns u_p3_t3_temp0( TMP , OPT_BW , OPT_BW , SIGNMAGNITUDE, "u_p3_t3_temp0" );

	HWlns u_p3_t3( TMP , OPT_BW , OPT_BW , SIGNMAGNITUDE, "u_p3_t3" );

	HWlns my_1o6( TMP , OPT_BW , OPT_BW , SIGNMAGNITUDE, "my_1o6" );

	HWlns my_one (  TMP  , (fsize)(  4  ) , (fsize)(  3  ) ,  SIGNMAGNITUDE , "my_one" );

	HWlns my_one_1 (  TMP  , (fsize)(  4  ) , (fsize)(  3  ) ,  SIGNMAGNITUDE , "my_one_1" );

	HWlns my_four (  TMP  , (fsize)(  4  ) , (fsize)(  3  ) ,  SIGNMAGNITUDE , "my_four" );

	HWlns my_zero (  TMP  , (fsize)(  4  ) , (fsize)(  3  ) ,  SIGNMAGNITUDE , "my_zero" );

	HWlns B0_temp0( TMP , OPT_BW , OPT_BW , SIGNMAGNITUDE, "B0_temp0" );

	HWlns B0_temp1( TMP , OPT_BW , OPT_BW , SIGNMAGNITUDE, "B0_temp1" );

	HWlns B0_temp2( TMP , OPT_BW , OPT_BW , SIGNMAGNITUDE, "B0_temp2" );

	HWlns B0_temp3( TMP , OPT_BW , OPT_BW , SIGNMAGNITUDE, "B0_temp3" );

	HWlns B0_temp4( TMP , OPT_BW , OPT_BW , SIGNMAGNITUDE, "B0_temp4" );

	HWlns B1_temp0( TMP , OPT_BW , OPT_BW , SIGNMAGNITUDE, "B1_temp0" );

	HWlns B1_temp1( TMP , OPT_BW , OPT_BW , SIGNMAGNITUDE, "B1_temp1" );

	HWlns B2_temp0( TMP , OPT_BW , OPT_BW , SIGNMAGNITUDE, "B2_temp0" );

	HWlns B2_temp1( TMP , OPT_BW , OPT_BW , SIGNMAGNITUDE, "B2_temp1" );

	HWlns B2_temp2( TMP , OPT_BW , OPT_BW , SIGNMAGNITUDE, "B2_temp2" );

	HWlns B3_temp0( TMP , OPT_BW , OPT_BW , SIGNMAGNITUDE, "B3_temp0" );



	HWlns my_two (  TMP  , (fsize)(  4  ) , (fsize)(  3  ) ,  SIGNMAGNITUDE , "my_two" );

      

	HWlns B0_out( OUT , OPT_BW , OUTFRACBW , SIGNMAGNITUDE, "B0_out" );

	HWlns B1_out( OUT , OPT_BW , OUTFRACBW , SIGNMAGNITUDE, "B1_out" );

	HWlns B2_out( OUT , OPT_BW , OUTFRACBW , SIGNMAGNITUDE, "B2_out" );

	HWlns B3_out( OUT , OPT_BW , OUTFRACBW , SIGNMAGNITUDE, "B3_out" );



  STREAM_LOOP( 10 )

     

    u = u_in;



    my_two = 2.;



  u_p2 = u * u;

  u_p3 = u_p2 * u;

  u_t2 = u * my_two;

  u_t3 = u_t2 + u;

  u_p2_t2 = u_p2 * my_two;

  u_p2_t3 = u_p2_t2 + u_p2;

  

  u_p2_t6_temp0 = u_p2_t2 * my_two;

  u_p2_t6_temp1 = u_p2_t6_temp0 + u_p2;

  u_p2_t6 = u_p2_t6_temp1 + u_p2;

  

  u_p3_t3_temp0 = u_p3 * my_two;

  u_p3_t3 = u_p3_t3_temp0 + u_p3; 

  

  my_1o6 = const_1o6;

  my_one = 1.;

  my_one_1 = 1.;

  my_four = 4.;

  my_zero = 0.;



  

  

  B0_temp0 = my_one - u_t2;

  B0_temp1 = B0_temp0 + u_p2;

  B0_temp2 = B0_temp1 - u;

  B0_temp3 = B0_temp2 + u_p2_t2;

  B0_temp4 = B0_temp3 - u_p3;

  B0_out = B0_temp4 * my_1o6;

  

  B1_temp0 = u_p3_t3 - u_p2_t6;

  B1_temp1 = B1_temp0 + my_four;

  B1_out = B1_temp1 * my_1o6;

  

  B2_temp0 = u_p2_t3 - u_p3_t3;

  B2_temp1 = B2_temp0 + u_t3;

  B2_temp2 = B2_temp1 + my_one_1;

  B2_out = B2_temp2 * my_1o6 ;

  

  B3_temp0 = my_zero - u_p3;

  B3_out = B3_temp0 * my_1o6; 

  

  STREAM_END



  return 0;  

  

}
