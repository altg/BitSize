//Code PreProcessed by ASC2BitSize
//(c) Altaf Abdul Gaffar

// 2x2 Matrix Multiplication using Strassen's Method

// assuming inputs are over [0,1)



// signals to be optimzed: 21

// add/sub: 18

// mult: 7


#include "bitsize.h"




#define OUTFRACBW R_ARGS(0)



int main( int argc , char* argv[] )

{

  

  

  STREAM_START( argc , argv );

  

//  DefaultSign = SIGNMAGNITUDE;

//  ROUNDING_CHOICE = NOROUND;

//  STREAM_OPTIMIZE = THROUGHPUT;

//  USE_HARDMULT = NO;





	HWfloat a00_in (  IN  , (fsize)(  OPT_BW + 3  ) , (fsize)( OPT_BW ) ,  SIGNMAGNITUDE , "a00_in" );

	HWfloat a01_in (  IN  , (fsize)(  OPT_BW + 3  ) , (fsize)( OPT_BW ) ,  SIGNMAGNITUDE , "a01_in" );

	HWfloat a10_in (  IN  , (fsize)(  OPT_BW + 3  ) , (fsize)( OPT_BW ) ,  SIGNMAGNITUDE , "a10_in" );

	HWfloat a11_in (  IN  , (fsize)(  OPT_BW + 3  ) , (fsize)( OPT_BW ) ,  SIGNMAGNITUDE , "a11_in" );

	HWfloat b00_in (  IN  , (fsize)(  OPT_BW + 3  ) , (fsize)( OPT_BW ) ,  SIGNMAGNITUDE , "b00_in" );

	HWfloat b01_in (  IN  , (fsize)(  OPT_BW + 3  ) , (fsize)( OPT_BW ) ,  SIGNMAGNITUDE , "b01_in" );

	HWfloat b10_in (  IN  , (fsize)(  OPT_BW + 3  ) , (fsize)( OPT_BW ) ,  SIGNMAGNITUDE , "b10_in" );

	HWfloat b11_in (  IN  , (fsize)(  OPT_BW + 3  ) , (fsize)( OPT_BW ) ,  SIGNMAGNITUDE , "b11_in" );



	HWfloat a00 (  TMP  , (fsize)(  OPT_BW + 3  ) , (fsize)( OPT_BW ) ,  SIGNMAGNITUDE , "a00" );

	HWfloat a01 (  TMP  , (fsize)(  OPT_BW + 3  ) , (fsize)( OPT_BW ) ,  SIGNMAGNITUDE , "a01" );

	HWfloat a10 (  TMP  , (fsize)(  OPT_BW + 3  ) , (fsize)( OPT_BW ) ,  SIGNMAGNITUDE , "a10" );

	HWfloat a11 (  TMP  , (fsize)(  OPT_BW + 3  ) , (fsize)( OPT_BW ) ,  SIGNMAGNITUDE , "a11" );

	HWfloat b00 (  TMP  , (fsize)(  OPT_BW + 3  ) , (fsize)( OPT_BW ) ,  SIGNMAGNITUDE , "b00" );

	HWfloat b01 (  TMP  , (fsize)(  OPT_BW + 3  ) , (fsize)( OPT_BW ) ,  SIGNMAGNITUDE , "b01" );

	HWfloat b10 (  TMP  , (fsize)(  OPT_BW + 3  ) , (fsize)( OPT_BW ) ,  SIGNMAGNITUDE , "b10" );

	HWfloat b11 (  TMP  , (fsize)(  OPT_BW + 3  ) , (fsize)( OPT_BW ) ,  SIGNMAGNITUDE , "b11" );

    

	HWfloat p0_temp0( TMP , OPT_BW , OPT_BW , SIGNMAGNITUDE, "p0_temp0" );

	HWfloat p0_temp1( TMP , OPT_BW , OPT_BW , SIGNMAGNITUDE, "p0_temp1" );

	HWfloat p0( TMP , OPT_BW , OPT_BW , SIGNMAGNITUDE, "p0" );

	HWfloat p1_temp0( TMP , OPT_BW , OPT_BW , SIGNMAGNITUDE, "p1_temp0" );

	HWfloat p1( TMP , OPT_BW , OPT_BW , SIGNMAGNITUDE, "p1" );

	HWfloat p2_temp0( TMP , OPT_BW , OPT_BW , SIGNMAGNITUDE, "p2_temp0" );

	HWfloat p2( TMP , OPT_BW , OPT_BW , SIGNMAGNITUDE, "p2" );

	HWfloat p3_temp0( TMP , OPT_BW , OPT_BW , SIGNMAGNITUDE, "p3_temp0" );

	HWfloat p3( TMP , OPT_BW , OPT_BW , SIGNMAGNITUDE, "p3" );

	HWfloat p4_temp0( TMP , OPT_BW , OPT_BW , SIGNMAGNITUDE, "p4_temp0" );

	HWfloat p4( TMP , OPT_BW , OPT_BW , SIGNMAGNITUDE, "p4" );

	HWfloat p5_temp0( TMP , OPT_BW , OPT_BW , SIGNMAGNITUDE, "p5_temp0" );

	HWfloat p5_temp1( TMP , OPT_BW , OPT_BW , SIGNMAGNITUDE, "p5_temp1" );

	HWfloat p5( TMP , OPT_BW , OPT_BW , SIGNMAGNITUDE, "p5" );

	HWfloat p6_temp0( TMP , OPT_BW , OPT_BW , SIGNMAGNITUDE, "p6_temp0" );

	HWfloat p6_temp1( TMP , OPT_BW , OPT_BW , SIGNMAGNITUDE, "p6_temp1" );

	HWfloat p6( TMP , OPT_BW , OPT_BW , SIGNMAGNITUDE, "p6" );

	HWfloat y00_temp0( TMP , OPT_BW , OPT_BW , SIGNMAGNITUDE, "y00_temp0" );

	HWfloat y00_temp1( TMP , OPT_BW , OPT_BW , SIGNMAGNITUDE, "y00_temp1" );

	HWfloat y11_temp0( TMP , OPT_BW , OPT_BW , SIGNMAGNITUDE, "y11_temp0" );

	HWfloat y11_temp1( TMP , OPT_BW , OPT_BW , SIGNMAGNITUDE, "y11_temp1" );

 

	HWfloat y00( OUT , OPT_BW , OUTFRACBW, SIGNMAGNITUDE, "y00" );

	HWfloat y01( OUT , OPT_BW , OUTFRACBW, SIGNMAGNITUDE, "y01" );

	HWfloat y10( OUT , OPT_BW , OUTFRACBW, SIGNMAGNITUDE, "y10" );

	HWfloat y11( OUT , OPT_BW , OUTFRACBW, SIGNMAGNITUDE, "y11" );



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
