// 4th order polynomial approximation to log(1+x) where x=[0,1)

// signals to be optimzed: 12
// add/sub: 4
// mult: 4

#include "asc.h"

#define OUTFRACBW R_ARGS(0)

int main( int argc , char* argv[] )
{

 
   
  STREAM_START( argc , argv );
  
  DefaultSign = SIGNMAGNITUDE;
  ROUNDING_CHOICE = NOROUND;
  STREAM_OPTIMIZE = THROUGHPUT;
  USE_HARDMULT = NO;


  HWlns a_in(IN, 0 + OPT_BW , OPT_BW , UNSIGNED);
  HWlns b_in(IN, 0 + OPT_BW , OPT_BW , UNSIGNED);
  
  HWlns a(TMP, 0 + OPT_BW , OPT_BW , UNSIGNED);
  HWlns b(TMP, 0 + OPT_BW , OPT_BW , UNSIGNED);
  
  HWlns y(OUT, OPT_BW, OUTFRACBW, UNSIGNED);

  STREAM_LOOP( 10 )
   
  a = a_in;

  b = b_in;
  
  y = a + b;
  

  STREAM_END


  return 0;  

}
