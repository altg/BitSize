// 4th order polynomial approximation to log(1+x) where x=[0,1)

// signals to be optimzed: 12
// add/sub: 4
// mult: 4

#include "asc.h"

#define OUTFRACBW R_ARGS(0)

int main( int argc , char* argv[] )
{

 
  const double c4_const = 5.657966225442204e-002;
  const double c3_const = 2.209050738836212e-001;
  const double c2_const = 4.678418107994854e-001;
  const double c1_const = 9.965418695736998e-001;
  const double c0_const = 6.068767183533240e-005;
  
  STREAM_START( argc , argv );
  
  DefaultSign = SIGNMAGNITUDE;
  ROUNDING_CHOICE = NOROUND;
  STREAM_OPTIMIZE = THROUGHPUT;
  USE_HARDMULT = NO;


  HWlns x_in(IN, 0 + OPT_BW , OPT_BW , UNSIGNED);
  
  HWlns x(TMP, 0 + OPT_BW , OPT_BW , UNSIGNED);
  HWlns c4(TMP, OPT_BW, OPT_BW);
  HWlns c3(TMP, OPT_BW, OPT_BW);
  HWlns c2(TMP, OPT_BW, OPT_BW);
  HWlns c1(TMP, OPT_BW, OPT_BW);
  HWlns c0(TMP, OPT_BW, OPT_BW);
  HWlns dp0(TMP, OPT_BW, OPT_BW);
  HWlns dp1(TMP, OPT_BW, OPT_BW);
  HWlns dp2(TMP, OPT_BW, OPT_BW);
  HWlns dp3(TMP, OPT_BW, OPT_BW);
  HWlns dp4(TMP, OPT_BW, OPT_BW);
  HWlns dp5(TMP, OPT_BW, OPT_BW);
  HWlns dp6(TMP, OPT_BW, OPT_BW);
  
  HWlns y(OUT, OPT_BW, OUTFRACBW, UNSIGNED);

  STREAM_LOOP( 10 )
  
  c4 = c4_const;
  c3 = c3_const;
  c2 = c2_const;
  c1 = c1_const;
  c0 = c0_const;
  
  x = x_in;

  dp0 = x * c4;
  
  dp1 = dp0 + c3;
  dp2 = x * dp1;
  dp3 = dp2 + c2;
  dp4 = x * dp3;
  dp5 = dp4 + c1;
  dp6 = x * dp5;
  
  y = dp6 + c0;
  
  

  STREAM_END


  return 0;  

}
