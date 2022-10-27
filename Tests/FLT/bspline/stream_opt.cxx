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
  STREAM_OPTIMIZE = THROUGHPUT;
  USE_HARDMULT = NO;
  
  HWfloat u_in( IN , OPT_BW + 3 , OPT_BW , UNSIGNED); 
  HWfloat u( TMP , OPT_BW + 3 , OPT_BW , UNSIGNED); 
     
  HWfloat u_p2( TMP , OPT_BW , OPT_BW );
  HWfloat u_p3( TMP , OPT_BW , OPT_BW );
  HWfloat u_t2( TMP , OPT_BW , OPT_BW );
  HWfloat u_t3( TMP , OPT_BW , OPT_BW );
  HWfloat u_p2_t2( TMP , OPT_BW , OPT_BW );
  HWfloat u_p2_t3( TMP , OPT_BW , OPT_BW );
  HWfloat u_p2_t6_temp0( TMP , OPT_BW , OPT_BW );
  HWfloat u_p2_t6_temp1( TMP , OPT_BW , OPT_BW );
  HWfloat u_p2_t6( TMP , OPT_BW , OPT_BW );  
  HWfloat u_p3_t3_temp0( TMP , OPT_BW , OPT_BW );
  HWfloat u_p3_t3( TMP , OPT_BW , OPT_BW );
  HWfloat my_1o6( TMP , OPT_BW , OPT_BW );
  HWfloat my_one( TMP , 4 , 3 , UNSIGNED );
  HWfloat my_one_1( TMP , 4 , 3 , UNSIGNED );
  HWfloat my_four( TMP , 4 , 3 , UNSIGNED );
  HWfloat my_zero( TMP , 4 , 3 , UNSIGNED );
  HWfloat B0_temp0( TMP , OPT_BW , OPT_BW );
  HWfloat B0_temp1( TMP , OPT_BW , OPT_BW );
  HWfloat B0_temp2( TMP , OPT_BW , OPT_BW );
  HWfloat B0_temp3( TMP , OPT_BW , OPT_BW );
  HWfloat B0_temp4( TMP , OPT_BW , OPT_BW );
  HWfloat B1_temp0( TMP , OPT_BW , OPT_BW );
  HWfloat B1_temp1( TMP , OPT_BW , OPT_BW );
  HWfloat B2_temp0( TMP , OPT_BW , OPT_BW );
  HWfloat B2_temp1( TMP , OPT_BW , OPT_BW );
  HWfloat B2_temp2( TMP , OPT_BW , OPT_BW );
  HWfloat B3_temp0( TMP , OPT_BW , OPT_BW );

  HWfloat my_two( TMP , 4 , 3 , UNSIGNED );
      
  HWfloat B0_out( OUT , OPT_BW , OUTFRACBW );
  HWfloat B1_out( OUT , OPT_BW , OUTFRACBW );
  HWfloat B2_out( OUT , OPT_BW , OUTFRACBW );
  HWfloat B3_out( OUT , OPT_BW , OUTFRACBW );

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
