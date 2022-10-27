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
  
  DefaultSign = SIGNMAGNITUDE;
  ROUNDING_CHOICE = NOROUND;
  STREAM_OPTIMIZE = THROUGHPUT;
  USE_HARDMULT = NO;


  HWlns a00_in( IN , OPT_BW + 2 ,OPT_BW, UNSIGNED); 
  HWlns a01_in( IN , OPT_BW + 2 ,OPT_BW, UNSIGNED); 
  HWlns a10_in( IN , OPT_BW + 2 ,OPT_BW, UNSIGNED); 
  HWlns a11_in( IN , OPT_BW + 2 ,OPT_BW, UNSIGNED); 
  HWlns b00_in( IN , OPT_BW + 2 ,OPT_BW, UNSIGNED); 
  HWlns b01_in( IN , OPT_BW + 2 ,OPT_BW, UNSIGNED); 
  HWlns b10_in( IN , OPT_BW + 2 ,OPT_BW, UNSIGNED); 
  HWlns b11_in( IN , OPT_BW + 2 ,OPT_BW, UNSIGNED); 

  HWlns a00( TMP , OPT_BW + 2 ,OPT_BW, UNSIGNED); 
  HWlns a01( TMP , OPT_BW + 2 ,OPT_BW, UNSIGNED); 
  HWlns a10( TMP , OPT_BW + 2 ,OPT_BW, UNSIGNED); 
  HWlns a11( TMP , OPT_BW + 2 ,OPT_BW, UNSIGNED); 
  HWlns b00( TMP , OPT_BW + 2 ,OPT_BW, UNSIGNED); 
  HWlns b01( TMP , OPT_BW + 2 ,OPT_BW, UNSIGNED); 
  HWlns b10( TMP , OPT_BW + 2 ,OPT_BW, UNSIGNED); 
  HWlns b11( TMP , OPT_BW + 2 ,OPT_BW, UNSIGNED); 
    
  HWlns p0_temp0( TMP , OPT_BW , OPT_BW );
  HWlns p0_temp1( TMP , OPT_BW , OPT_BW );
  HWlns p0( TMP , OPT_BW , OPT_BW );
  HWlns p1_temp0( TMP , OPT_BW , OPT_BW );
  HWlns p1( TMP , OPT_BW , OPT_BW );
  HWlns p2_temp0( TMP , OPT_BW , OPT_BW );
  HWlns p2( TMP , OPT_BW , OPT_BW );
  HWlns p3_temp0( TMP , OPT_BW , OPT_BW );
  HWlns p3( TMP , OPT_BW , OPT_BW );
  HWlns p4_temp0( TMP , OPT_BW , OPT_BW );
  HWlns p4( TMP , OPT_BW , OPT_BW );
  HWlns p5_temp0( TMP , OPT_BW , OPT_BW );
  HWlns p5_temp1( TMP , OPT_BW , OPT_BW );
  HWlns p5( TMP , OPT_BW , OPT_BW );
  HWlns p6_temp0( TMP , OPT_BW , OPT_BW );
  HWlns p6_temp1( TMP , OPT_BW , OPT_BW );
  HWlns p6( TMP , OPT_BW , OPT_BW );
  HWlns y00_temp0( TMP , OPT_BW , OPT_BW );
  HWlns y00_temp1( TMP , OPT_BW , OPT_BW );
  HWlns y11_temp0( TMP , OPT_BW , OPT_BW );
  HWlns y11_temp1( TMP , OPT_BW , OPT_BW );
 
  HWlns y00( OUT , OPT_BW , OUTFRACBW); 
  HWlns y01( OUT , OPT_BW , OUTFRACBW); 
  HWlns y10( OUT , OPT_BW , OUTFRACBW); 
  HWlns y11( OUT , OPT_BW , OUTFRACBW); 

  STREAM_LOOP( 10 )
  
a00 = a00_in;
a01 = a01_in;
a10 = a10_in;
a11 = a11_in;
b00 = b00_in;
b01 = b01_in;
b10 = b10_in;
b11 = b11_in;

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
