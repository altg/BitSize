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


  HWfloat a00_in( IN , OPT_BW + 3 ,OPT_BW, UNSIGNED); 
  HWfloat a01_in( IN , OPT_BW + 3 ,OPT_BW, UNSIGNED); 
  HWfloat a10_in( IN , OPT_BW + 3 ,OPT_BW, UNSIGNED); 
  HWfloat a11_in( IN , OPT_BW + 3 ,OPT_BW, UNSIGNED); 
  HWfloat b00_in( IN , OPT_BW + 3 ,OPT_BW, UNSIGNED); 
  HWfloat b01_in( IN , OPT_BW + 3 ,OPT_BW, UNSIGNED); 
  HWfloat b10_in( IN , OPT_BW + 3 ,OPT_BW, UNSIGNED); 
  HWfloat b11_in( IN , OPT_BW + 3 ,OPT_BW, UNSIGNED); 

  HWfloat a00( TMP , OPT_BW + 3 ,OPT_BW, UNSIGNED); 
  HWfloat a01( TMP , OPT_BW + 3 ,OPT_BW, UNSIGNED); 
  HWfloat a10( TMP , OPT_BW + 3 ,OPT_BW, UNSIGNED); 
  HWfloat a11( TMP , OPT_BW + 3 ,OPT_BW, UNSIGNED); 
  HWfloat b00( TMP , OPT_BW + 3 ,OPT_BW, UNSIGNED); 
  HWfloat b01( TMP , OPT_BW + 3 ,OPT_BW, UNSIGNED); 
  HWfloat b10( TMP , OPT_BW + 3 ,OPT_BW, UNSIGNED); 
  HWfloat b11( TMP , OPT_BW + 3 ,OPT_BW, UNSIGNED); 
    
  HWfloat p0_temp0( TMP , OPT_BW , OPT_BW );
  HWfloat p0_temp1( TMP , OPT_BW , OPT_BW );
  HWfloat p0( TMP , OPT_BW , OPT_BW );
  HWfloat p1_temp0( TMP , OPT_BW , OPT_BW );
  HWfloat p1( TMP , OPT_BW , OPT_BW );
  HWfloat p2_temp0( TMP , OPT_BW , OPT_BW );
  HWfloat p2( TMP , OPT_BW , OPT_BW );
  HWfloat p3_temp0( TMP , OPT_BW , OPT_BW );
  HWfloat p3( TMP , OPT_BW , OPT_BW );
  HWfloat p4_temp0( TMP , OPT_BW , OPT_BW );
  HWfloat p4( TMP , OPT_BW , OPT_BW );
  HWfloat p5_temp0( TMP , OPT_BW , OPT_BW );
  HWfloat p5_temp1( TMP , OPT_BW , OPT_BW );
  HWfloat p5( TMP , OPT_BW , OPT_BW );
  HWfloat p6_temp0( TMP , OPT_BW , OPT_BW );
  HWfloat p6_temp1( TMP , OPT_BW , OPT_BW );
  HWfloat p6( TMP , OPT_BW , OPT_BW );
  HWfloat y00_temp0( TMP , OPT_BW , OPT_BW );
  HWfloat y00_temp1( TMP , OPT_BW , OPT_BW );
  HWfloat y11_temp0( TMP , OPT_BW , OPT_BW );
  HWfloat y11_temp1( TMP , OPT_BW , OPT_BW );
 
  HWfloat y00( OUT , OPT_BW , OUTFRACBW); 
  HWfloat y01( OUT , OPT_BW , OUTFRACBW); 
  HWfloat y10( OUT , OPT_BW , OUTFRACBW); 
  HWfloat y11( OUT , OPT_BW , OUTFRACBW); 

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
