//Code PreProcessed by ASC2BitSize
//(c) Altaf Abdul Gaffar

// 4th order polynomial approximation to log(1+x) where x=[0,1)



// signals to be optimzed: 12

// add/sub: 4

// mult: 4


#include "bitsize.h"




#define OUTFRACBW R_ARGS(0)



int main( int argc , char* argv[] )

{



 

   

  STREAM_START( argc , argv );

  

//  DefaultSign = SIGNMAGNITUDE;

//  ROUNDING_CHOICE = NOROUND;

//  STREAM_OPTIMIZE = THROUGHPUT;

//  USE_HARDMULT = NO;





	HWlns a_in ( IN , (fsize)(  0 + OPT_BW  ) , (fsize)(  OPT_BW  ) ,  SIGNMAGNITUDE , "a_in" );

	HWlns b_in ( IN , (fsize)(  0 + OPT_BW  ) , (fsize)(  OPT_BW  ) ,  SIGNMAGNITUDE , "b_in" );

  

	HWlns a ( TMP , (fsize)(  0 + OPT_BW  ) , (fsize)(  OPT_BW  ) ,  SIGNMAGNITUDE , "a" );

	HWlns b ( TMP , (fsize)(  0 + OPT_BW  ) , (fsize)(  OPT_BW  ) ,  SIGNMAGNITUDE , "b" );

  

	HWlns y ( OUT , (fsize)(  OPT_BW ) , (fsize)(  OUTFRACBW ) ,  SIGNMAGNITUDE , "y" );



  STREAM_LOOP( 10 )

   

  a = a_in;



  b = b_in;

  

  y = a + b;

  



  STREAM_END





  return 0;  



}
