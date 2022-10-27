//Code Bitwidth Optimised by BitSize
//(c) Altaf Abdul Gaffar
#ifdef ASCSIM_TESTS

#define OPT_EXPBW_B0_out atoi( argv[1] )

#define OPT_EXPBW_B0_temp0 atoi( argv[2] )
#define OPT_MANBW_B0_temp0 atoi( argv[3] )

#define OPT_EXPBW_B0_temp1 atoi( argv[4] )
#define OPT_MANBW_B0_temp1 atoi( argv[5] )

#define OPT_EXPBW_B0_temp2 atoi( argv[6] )
#define OPT_MANBW_B0_temp2 atoi( argv[7] )

#define OPT_EXPBW_B0_temp3 atoi( argv[8] )
#define OPT_MANBW_B0_temp3 atoi( argv[9] )

#define OPT_EXPBW_B0_temp4 atoi( argv[10] )
#define OPT_MANBW_B0_temp4 atoi( argv[11] )

#define OPT_EXPBW_B1_out atoi( argv[12] )

#define OPT_EXPBW_B1_temp0 atoi( argv[13] )
#define OPT_MANBW_B1_temp0 atoi( argv[14] )

#define OPT_EXPBW_B1_temp1 atoi( argv[15] )
#define OPT_MANBW_B1_temp1 atoi( argv[16] )

#define OPT_EXPBW_B2_out atoi( argv[17] )

#define OPT_EXPBW_B2_temp0 atoi( argv[18] )
#define OPT_MANBW_B2_temp0 atoi( argv[19] )

#define OPT_EXPBW_B2_temp1 atoi( argv[20] )
#define OPT_MANBW_B2_temp1 atoi( argv[21] )

#define OPT_EXPBW_B2_temp2 atoi( argv[22] )
#define OPT_MANBW_B2_temp2 atoi( argv[23] )

#define OPT_EXPBW_B3_out atoi( argv[24] )

#define OPT_EXPBW_B3_temp0 atoi( argv[25] )
#define OPT_MANBW_B3_temp0 atoi( argv[26] )

#define OPT_EXPBW_my_1o6 atoi( argv[27] )
#define OPT_MANBW_my_1o6 atoi( argv[28] )






#define OPT_MANBW_u atoi( argv[29] )

#define OPT_MANBW_u_in atoi( argv[30] )

#define OPT_EXPBW_u_p2 atoi( argv[31] )
#define OPT_MANBW_u_p2 atoi( argv[32] )

#define OPT_EXPBW_u_p2_t2 atoi( argv[33] )
#define OPT_MANBW_u_p2_t2 atoi( argv[34] )

#define OPT_EXPBW_u_p2_t3 atoi( argv[35] )
#define OPT_MANBW_u_p2_t3 atoi( argv[36] )

#define OPT_EXPBW_u_p2_t6 atoi( argv[37] )
#define OPT_MANBW_u_p2_t6 atoi( argv[38] )

#define OPT_EXPBW_u_p2_t6_temp0 atoi( argv[39] )
#define OPT_MANBW_u_p2_t6_temp0 atoi( argv[40] )

#define OPT_EXPBW_u_p2_t6_temp1 atoi( argv[41] )
#define OPT_MANBW_u_p2_t6_temp1 atoi( argv[42] )

#define OPT_EXPBW_u_p3 atoi( argv[43] )
#define OPT_MANBW_u_p3 atoi( argv[44] )

#define OPT_EXPBW_u_p3_t3 atoi( argv[45] )
#define OPT_MANBW_u_p3_t3 atoi( argv[46] )

#define OPT_EXPBW_u_p3_t3_temp0 atoi( argv[47] )
#define OPT_MANBW_u_p3_t3_temp0 atoi( argv[48] )

#define OPT_EXPBW_u_t2 atoi( argv[49] )
#define OPT_MANBW_u_t2 atoi( argv[50] )

#define OPT_EXPBW_u_t3 atoi( argv[51] )
#define OPT_MANBW_u_t3 atoi( argv[52] )

#else

#define OPT_EXPBW_B0_out 4

#define OPT_EXPBW_B0_temp0 4
#define OPT_MANBW_B0_temp0 16

#define OPT_EXPBW_B0_temp1 4
#define OPT_MANBW_B0_temp1 15

#define OPT_EXPBW_B0_temp2 4
#define OPT_MANBW_B0_temp2 15

#define OPT_EXPBW_B0_temp3 4
#define OPT_MANBW_B0_temp3 15

#define OPT_EXPBW_B0_temp4 4
#define OPT_MANBW_B0_temp4 15

#define OPT_EXPBW_B1_out 4

#define OPT_EXPBW_B1_temp0 5
#define OPT_MANBW_B1_temp0 20

#define OPT_EXPBW_B1_temp1 5
#define OPT_MANBW_B1_temp1 15

#define OPT_EXPBW_B2_out 4

#define OPT_EXPBW_B2_temp0 5
#define OPT_MANBW_B2_temp0 15

#define OPT_EXPBW_B2_temp1 5
#define OPT_MANBW_B2_temp1 15

#define OPT_EXPBW_B2_temp2 5
#define OPT_MANBW_B2_temp2 15

#define OPT_EXPBW_B3_out 4

#define OPT_EXPBW_B3_temp0 4
#define OPT_MANBW_B3_temp0 20

#define OPT_EXPBW_my_1o6 4
#define OPT_MANBW_my_1o6 16






#define OPT_MANBW_u 15

#define OPT_MANBW_u_in 15

#define OPT_EXPBW_u_p2 4
#define OPT_MANBW_u_p2 19

#define OPT_EXPBW_u_p2_t2 4
#define OPT_MANBW_u_p2_t2 15

#define OPT_EXPBW_u_p2_t3 4
#define OPT_MANBW_u_p2_t3 16

#define OPT_EXPBW_u_p2_t6 4
#define OPT_MANBW_u_p2_t6 15

#define OPT_EXPBW_u_p2_t6_temp0 4
#define OPT_MANBW_u_p2_t6_temp0 15

#define OPT_EXPBW_u_p2_t6_temp1 4
#define OPT_MANBW_u_p2_t6_temp1 16

#define OPT_EXPBW_u_p3 4
#define OPT_MANBW_u_p3 21

#define OPT_EXPBW_u_p3_t3 5
#define OPT_MANBW_u_p3_t3 15

#define OPT_EXPBW_u_p3_t3_temp0 4
#define OPT_MANBW_u_p3_t3_temp0 18

#define OPT_EXPBW_u_t2 3
#define OPT_MANBW_u_t2 15

#define OPT_EXPBW_u_t3 4
#define OPT_MANBW_u_t3 15

#endif

// Cubic B-Splines

// signals to be optimzed: 23
// add/sub: 16
// mult: 6

#include "asc.h"

#define OUTFRACBW R_ARGS(0)

int main( int argc , char* argv[] )
{
  
  const double const_1o6 = 0.166666666666666666666667;     // 1/6
  
  STREAM_START( argc , argv );
  
  DefaultSign = SIGNMAGNITUDE;
ROUNDING_CHOICE = NOROUND;

STREAM_OPTIMIZE = LATENCY;

MULTIPLY_TYPE = XC4000_MULT;

glod = LOD;
galg = STANDARD;
gsh = BARREL;

  STREAM_OPTIMIZE = THROUGHPUT;
  USE_HARDMULT = NO;
  
  HWlns u_in( IN , OPT_BW + 3 , OPT_BW , UNSIGNED); 
  HWlns u( TMP , OPT_BW + 3 , OPT_BW , UNSIGNED); 
     
  HWlns u_p2( TMP , OPT_BW , OPT_BW );
  HWlns u_p3( TMP , OPT_BW , OPT_BW );
  HWlns u_t2( TMP , OPT_BW , OPT_BW );
  HWlns u_t3( TMP , OPT_BW , OPT_BW );
  HWlns u_p2_t2( TMP , OPT_BW , OPT_BW );
  HWlns u_p2_t3( TMP , OPT_BW , OPT_BW );
  HWlns u_p2_t6_temp0( TMP , OPT_BW , OPT_BW );
  HWlns u_p2_t6_temp1( TMP , OPT_BW , OPT_BW );
  HWlns u_p2_t6( TMP , OPT_BW , OPT_BW );  
  HWlns u_p3_t3_temp0( TMP , OPT_BW , OPT_BW );
  HWlns u_p3_t3( TMP , OPT_BW , OPT_BW );
  HWlns my_1o6( TMP , OPT_BW , OPT_BW );
  HWlns my_one( TMP , 4 , 3 , UNSIGNED );
  HWlns my_one_1( TMP , 4 , 3 , UNSIGNED );
  HWlns my_four( TMP , 4 , 3 , UNSIGNED );
  HWlns my_zero( TMP , 4 , 3 , UNSIGNED );
  HWlns B0_temp0( TMP , OPT_BW , OPT_BW );
  HWlns B0_temp1( TMP , OPT_BW , OPT_BW );
  HWlns B0_temp2( TMP , OPT_BW , OPT_BW );
  HWlns B0_temp3( TMP , OPT_BW , OPT_BW );
  HWlns B0_temp4( TMP , OPT_BW , OPT_BW );
  HWlns B1_temp0( TMP , OPT_BW , OPT_BW );
  HWlns B1_temp1( TMP , OPT_BW , OPT_BW );
  HWlns B2_temp0( TMP , OPT_BW , OPT_BW );
  HWlns B2_temp1( TMP , OPT_BW , OPT_BW );
  HWlns B2_temp2( TMP , OPT_BW , OPT_BW );
  HWlns B3_temp0( TMP , OPT_BW , OPT_BW );

  HWlns my_two( TMP , 4 , 3 , UNSIGNED );
      
  HWlns B0_out( OUT , OPT_BW , OUTFRACBW );
  HWlns B1_out( OUT , OPT_BW , OUTFRACBW );
  HWlns B2_out( OUT , OPT_BW , OUTFRACBW );
  HWlns B3_out( OUT , OPT_BW , OUTFRACBW );

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
