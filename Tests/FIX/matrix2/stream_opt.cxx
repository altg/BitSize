// 2x2 Matrix Multiplication using Strassen's Method
// assuming inputs are over [0,1)

// signals to be optimzed: 21
// add/sub: 18
// mult: 7

#include "asc.h"

#define OUTFRACBW R_ARGS(0)

int main( int argc , char* argv[] )
{
  
  
  STREAM_START( argc , argv );
  
  DefaultSign = TWOSCOMPLEMENT;
  ROUNDING_CHOICE = NOROUND;
  STREAM_OPTIMIZE = THROUGHPUT;
  USE_HARDMULT = NO;
  
  HWfix a00( IN , OPT_BW + 3 ,OPT_BW, UNSIGNED); 
  HWfix a01( IN , OPT_BW + 3 ,OPT_BW, UNSIGNED); 
  HWfix a10( IN , OPT_BW + 3 ,OPT_BW, UNSIGNED); 
  HWfix a11( IN , OPT_BW + 3 ,OPT_BW, UNSIGNED); 
  HWfix b00( IN , OPT_BW + 3 ,OPT_BW, UNSIGNED); 
  HWfix b01( IN , OPT_BW + 3 ,OPT_BW, UNSIGNED); 
  HWfix b10( IN , OPT_BW + 3 ,OPT_BW, UNSIGNED); 
  HWfix b11( IN , OPT_BW + 3 ,OPT_BW, UNSIGNED); 
    
  HWfix p0_temp0( TMP , OPT_BW , OPT_BW );
  HWfix p0_temp1( TMP , OPT_BW , OPT_BW );
  HWfix p0( TMP , OPT_BW , OPT_BW );
  HWfix p1_temp0( TMP , OPT_BW , OPT_BW );
  HWfix p1( TMP , OPT_BW , OPT_BW );
  HWfix p2_temp0( TMP , OPT_BW , OPT_BW );
  HWfix p2( TMP , OPT_BW , OPT_BW );
  HWfix p3_temp0( TMP , OPT_BW , OPT_BW );
  HWfix p3( TMP , OPT_BW , OPT_BW );
  HWfix p4_temp0( TMP , OPT_BW , OPT_BW );
  HWfix p4( TMP , OPT_BW , OPT_BW );
  HWfix p5_temp0( TMP , OPT_BW , OPT_BW );
  HWfix p5_temp1( TMP , OPT_BW , OPT_BW );
  HWfix p5( TMP , OPT_BW , OPT_BW );
  HWfix p6_temp0( TMP , OPT_BW , OPT_BW );
  HWfix p6_temp1( TMP , OPT_BW , OPT_BW );
  HWfix p6( TMP , OPT_BW , OPT_BW );
  HWfix y00_temp0( TMP , OPT_BW , OPT_BW );
  HWfix y00_temp1( TMP , OPT_BW , OPT_BW );
  HWfix y11_temp0( TMP , OPT_BW , OPT_BW );
  HWfix y11_temp1( TMP , OPT_BW , OPT_BW );
 
  HWfix y00( OUT , OPT_BW , OUTFRACBW); 
  HWfix y01( OUT , OPT_BW , OUTFRACBW); 
  HWfix y10( OUT , OPT_BW , OUTFRACBW); 
  HWfix y11( OUT , OPT_BW , OUTFRACBW); 

  STREAM_LOOP( 10 )
     
  p0_temp0 = a00 + a11;
  p0_temp1 = b00 + b11;
  p0 = p0_temp0 * p0_temp1;
  
  p1_temp0 = a10 + a11;
  p1 = p1_temp0 * b00;
  
  p2_temp0 = b01 - b11;
  p2 = p2_temp0 * a00;
  
  p3_temp0 = b10 - b00;
  p3 = p3_temp0 * a11;
  
  p4_temp0 = a00 + a01;
  p4 = p4_temp0 * b11;
  
  p5_temp0 = a10 - a00;
  p5_temp1 = b00 + b01;
  p5 = p5_temp0 * p5_temp1;
  
  p6_temp0 = a01 - a11;
  p6_temp1 = b10 + b11;
  p6 = p6_temp0 * p6_temp1;
  
  y00_temp0 = p0 + p3;
  y00_temp1 = p6 - p4;
  y00 = y00_temp0 + y00_temp1;
  
  y01 = p2 + p4;
  
  y10 = p1 + p3;
  
  y11_temp0 = p0 + p2;
  y11_temp1 = p5 - p1;
  y11 = y11_temp0 + y11_temp1;
  
  STREAM_END

  return 0;  
  
}
