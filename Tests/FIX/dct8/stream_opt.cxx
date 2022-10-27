// 8x8 DCT for 8-bit pixels

// follows the compuations in: Madisetti and Wilson, "A 100 MHz 2-D 8 x 8 DCT/IDCT Processor for HDTV Applications" 

// signals to be optimzed: 55
// add/sub: 32
// mult: 32

#include "asc.h"

#define OUTFRACBW R_ARGS(0)

int main( int argc , char* argv[] )
{
  
  const double const_a = 3.535533905932738e-001; // cos(pi/4)/2
  const double const_b = 4.903926402016152e-001; // cos(pi/16)/2
  const double const_c = 4.619397662556434e-001; // cos(pi/8)/2
  const double const_d = 4.157348061512726e-001; // cos((3*pi)/16)/2
  const double const_e = 2.777851165098011e-001; // cos((5*pi)/16)/2
  const double const_f = 1.913417161825449e-001; // cos((3*pi)/8)/2
  const double const_g = 9.754516100806417e-002; // cos((7*pi)/16)/2
  
  STREAM_START( argc , argv );
  
  DefaultSign = TWOSCOMPLEMENT;
  ROUNDING_CHOICE = NOROUND;
  STREAM_OPTIMIZE = THROUGHPUT;
  USE_HARDMULT = NO;
  
  HWfix x0(IN , 3 + OPT_BW, OPT_BW, UNSIGNED); 
  HWfix x1(IN , 3 + OPT_BW, OPT_BW, UNSIGNED);
  HWfix x2(IN , 3 + OPT_BW, OPT_BW, UNSIGNED);
  HWfix x3(IN , 3 + OPT_BW, OPT_BW, UNSIGNED);
  HWfix x4(IN , 3 + OPT_BW, OPT_BW, UNSIGNED);
  HWfix x5(IN , 3 + OPT_BW, OPT_BW, UNSIGNED);
  HWfix x6(IN , 3 + OPT_BW, OPT_BW, UNSIGNED);
  HWfix x7(IN , 3 + OPT_BW, OPT_BW, UNSIGNED);
  
  HWfix a(TMP, OPT_BW, OPT_BW);
  HWfix b(TMP, OPT_BW, OPT_BW);
  HWfix c(TMP, OPT_BW, OPT_BW);
  HWfix d(TMP, OPT_BW, OPT_BW);
  HWfix e(TMP, OPT_BW, OPT_BW);
  HWfix f(TMP, OPT_BW, OPT_BW);
  HWfix g(TMP, OPT_BW, OPT_BW); 
   
  HWfix x0_p_x7(TMP, OPT_BW, OPT_BW );
  HWfix x1_p_x6(TMP, OPT_BW, OPT_BW );
  HWfix x2_p_x5(TMP, OPT_BW, OPT_BW );
  HWfix x3_p_x4(TMP, OPT_BW, OPT_BW ); 
  HWfix x0_m_x7(TMP, OPT_BW, OPT_BW );
  HWfix x1_m_x6(TMP, OPT_BW, OPT_BW );
  HWfix x2_m_x5(TMP, OPT_BW, OPT_BW );
  HWfix x3_m_x4(TMP, OPT_BW, OPT_BW );
  
  HWfix y0_temp0(TMP, OPT_BW, OPT_BW);
  HWfix y0_temp1(TMP, OPT_BW, OPT_BW);
  HWfix y0_temp2(TMP, OPT_BW, OPT_BW);
  HWfix y0_temp3(TMP, OPT_BW, OPT_BW);
  HWfix y0_temp4(TMP, OPT_BW, OPT_BW);
  HWfix y0_temp5(TMP, OPT_BW, OPT_BW);
  HWfix y1_temp0(TMP, OPT_BW, OPT_BW);
  HWfix y1_temp1(TMP, OPT_BW, OPT_BW);
  HWfix y1_temp2(TMP, OPT_BW, OPT_BW);
  HWfix y1_temp3(TMP, OPT_BW, OPT_BW);
  HWfix y1_temp4(TMP, OPT_BW, OPT_BW);
  HWfix y1_temp5(TMP, OPT_BW, OPT_BW);
  HWfix y2_temp0(TMP, OPT_BW, OPT_BW);
  HWfix y2_temp1(TMP, OPT_BW, OPT_BW);
  HWfix y2_temp2(TMP, OPT_BW, OPT_BW);
  HWfix y2_temp3(TMP, OPT_BW, OPT_BW);
  HWfix y2_temp4(TMP, OPT_BW, OPT_BW);
  HWfix y2_temp5(TMP, OPT_BW, OPT_BW);
  HWfix y3_temp0(TMP, OPT_BW, OPT_BW);
  HWfix y3_temp1(TMP, OPT_BW, OPT_BW);
  HWfix y3_temp2(TMP, OPT_BW, OPT_BW);
  HWfix y3_temp3(TMP, OPT_BW, OPT_BW);
  HWfix y3_temp4(TMP, OPT_BW, OPT_BW);
  HWfix y3_temp5(TMP, OPT_BW, OPT_BW);
  HWfix y4_temp0(TMP, OPT_BW, OPT_BW);
  HWfix y4_temp1(TMP, OPT_BW, OPT_BW);
  HWfix y4_temp2(TMP, OPT_BW, OPT_BW);
  HWfix y4_temp3(TMP, OPT_BW, OPT_BW);
  HWfix y4_temp4(TMP, OPT_BW, OPT_BW);
  HWfix y4_temp5(TMP, OPT_BW, OPT_BW);
  HWfix y5_temp0(TMP, OPT_BW, OPT_BW);
  HWfix y5_temp1(TMP, OPT_BW, OPT_BW);
  HWfix y5_temp2(TMP, OPT_BW, OPT_BW);
  HWfix y5_temp3(TMP, OPT_BW, OPT_BW);
  HWfix y5_temp4(TMP, OPT_BW, OPT_BW);
  HWfix y5_temp5(TMP, OPT_BW, OPT_BW);
  HWfix y6_temp0(TMP, OPT_BW, OPT_BW);
  HWfix y6_temp1(TMP, OPT_BW, OPT_BW);
  HWfix y6_temp2(TMP, OPT_BW, OPT_BW);
  HWfix y6_temp3(TMP, OPT_BW, OPT_BW);
  HWfix y6_temp4(TMP, OPT_BW, OPT_BW);
  HWfix y6_temp5(TMP, OPT_BW, OPT_BW);
  HWfix y7_temp0(TMP, OPT_BW, OPT_BW);
  HWfix y7_temp1(TMP, OPT_BW, OPT_BW);
  HWfix y7_temp2(TMP, OPT_BW, OPT_BW);
  HWfix y7_temp3(TMP, OPT_BW, OPT_BW);
  HWfix y7_temp4(TMP, OPT_BW, OPT_BW);
  HWfix y7_temp5(TMP, OPT_BW, OPT_BW);
  
  HWfix y0(OUT, OPT_BW, OUTFRACBW); 
  HWfix y1(OUT, OPT_BW, OUTFRACBW);
  HWfix y2(OUT, OPT_BW, OUTFRACBW);
  HWfix y3(OUT, OPT_BW, OUTFRACBW);
  HWfix y4(OUT, OPT_BW, OUTFRACBW);
  HWfix y5(OUT, OPT_BW, OUTFRACBW);
  HWfix y6(OUT, OPT_BW, OUTFRACBW);
  HWfix y7(OUT, OPT_BW, OUTFRACBW);

  STREAM_LOOP( 10 )
    
  a = const_a;  
  b = const_b;
  c = const_c;
  d = const_d;
  e = const_e;
  f = const_f;
  g = const_g;
    
  x0_p_x7 = x0 + x7; 
  x1_p_x6 = x1 + x6;
  x2_p_x5 = x2 + x5;
  x3_p_x4 = x3 + x4;
  x0_m_x7 = x0 - x7; 
  x1_m_x6 = x1 - x6;
  x2_m_x5 = x2 - x5;
  x3_m_x4 = x3 - x4;
  
  y0_temp0 = a * x0_p_x7;
  y0_temp1 = a * x1_p_x6;
  y0_temp2 = a * x2_p_x5;
  y0_temp3 = a * x3_p_x4;
  y0_temp4 = y0_temp0 + y0_temp1;
  y0_temp5 = y0_temp2 + y0_temp3;
  y0 = y0_temp4 + y0_temp5;
  
  y1_temp0 = b * x0_m_x7;
  y1_temp1 = d * x1_m_x6;
  y1_temp2 = e * x2_m_x5;
  y1_temp3 = g * x3_m_x4;
  y1_temp4 = y1_temp0 + y1_temp1;
  y1_temp5 = y1_temp2 + y1_temp3;
  y1 = y1_temp4 + y1_temp5;
  
  y2_temp0 = c * x0_p_x7;
  y2_temp1 = f * x1_p_x6;
  y2_temp2 = f * x2_p_x5;
  y2_temp3 = c * x3_p_x4;
  y2_temp4 = y2_temp0 + y2_temp1;
  y2_temp5 = y2_temp3 - y2_temp2;
  y2 = y2_temp4 + y2_temp5;
  
  y3_temp0 = d * x0_m_x7;
  y3_temp1 = g * x1_m_x6;
  y3_temp2 = b * x2_m_x5;
  y3_temp3 = e * x3_m_x4;
  y3_temp4 = y3_temp0 - y3_temp1;
  y3_temp5 = y3_temp2 + y3_temp3;
  y3 = y3_temp4 - y3_temp5;
  
  y4_temp0 = a * x0_p_x7;
  y4_temp1 = a * x1_p_x6;
  y4_temp2 = a * x2_p_x5;
  y4_temp3 = a * x3_p_x4;
  y4_temp4 = y4_temp0 - y4_temp1;
  y4_temp5 = y4_temp3 - y4_temp2;
  y4 = y4_temp4 + y4_temp5;
  
  y5_temp0 = e * x0_m_x7;
  y5_temp1 = b * x1_m_x6;
  y5_temp2 = g * x2_m_x5;
  y5_temp3 = a * x3_m_x4;
  y5_temp4 = y5_temp0 - y5_temp1;
  y5_temp5 = y5_temp2 + y5_temp3;
  y5 = y5_temp4 + y5_temp5;
  
  y6_temp0 = f * x0_p_x7;
  y6_temp1 = c * x1_p_x6;
  y6_temp2 = c * x2_p_x5;
  y6_temp3 = f * x3_p_x4;
  y6_temp4 = y6_temp0 - y6_temp1;
  y6_temp5 = y6_temp2 - y6_temp3;
  y6 = y6_temp4 + y6_temp5;
  
  y7_temp0 = g * x0_m_x7;
  y7_temp1 = e * x1_m_x6;
  y7_temp2 = d * x2_m_x5;
  y7_temp3 = b * x3_m_x4;
  y7_temp4 = y7_temp0 - y7_temp1;
  y7_temp5 = y7_temp2 - y7_temp3;
  y7 = y7_temp4 + y7_temp5;
  
  STREAM_END

  return 0;  
  
}
