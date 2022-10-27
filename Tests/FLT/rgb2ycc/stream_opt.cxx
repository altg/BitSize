// RGB to YCbCr converter for JPEG 2000 

// signals to be optimzed: 21
// add/sub: 7
// mult: 7

#include "asc.h"

#define OUTFRACBW R_ARGS(0)

int main( int argc , char* argv[] )
{
   
   STREAM_START( argc , argv );
 
  const double const_c_00 = 0.299; 
  const double const_c_01 = 0.578; 
  const double const_c_02 = 0.114;
  const double const_c_10 = -0.16875;  
  const double const_c_11 = -0.33126; 
  const double const_c_12 = 0.5; // replaced with a shift
  const double const_c_20 = 0.5; // replaced with a shift
  const double const_c_21 = -0.41869;
  const double const_c_22 = -0.08131;

  const double const_mult_1 = 1.;


  DefaultSign = SIGNMAGNITUDE;
  ROUNDING_CHOICE = NOROUND;
  STREAM_OPTIMIZE = THROUGHPUT;
  USE_HARDMULT = NO;
  
  HWfloat R_in(IN , 3 +OPT_BW , OPT_BW, UNSIGNED); 
  HWfloat G_in(IN , 3 +OPT_BW , OPT_BW, UNSIGNED); 
  HWfloat B_in(IN , 3 +OPT_BW , OPT_BW, UNSIGNED); 

  HWfloat R(TMP , 3 +OPT_BW , OPT_BW, UNSIGNED); 
  HWfloat G(TMP , 3 +OPT_BW , OPT_BW, UNSIGNED); 
  HWfloat B(TMP , 3 +OPT_BW , OPT_BW, UNSIGNED); 
  
  HWfloat Y(OUT , OPT_BW, OUTFRACBW); 
  HWfloat Cb(OUT , OPT_BW, OUTFRACBW);   
  HWfloat Cr(OUT , OPT_BW, OUTFRACBW); 
       
  HWfloat c_00(TMP, OPT_BW, OPT_BW);
  HWfloat c_01(TMP, OPT_BW, OPT_BW);
  HWfloat c_02(TMP, OPT_BW, OPT_BW);
  HWfloat c_10(TMP, OPT_BW, OPT_BW);
  HWfloat c_11(TMP, OPT_BW, OPT_BW);
  HWfloat c_21(TMP, OPT_BW, OPT_BW);
  HWfloat c_22(TMP, OPT_BW, OPT_BW);
  
  HWfloat mult_1(TMP, OPT_BW, OPT_BW);

  HWfloat Y_temp0(TMP, OPT_BW, OPT_BW);
  HWfloat Y_temp1(TMP, OPT_BW, OPT_BW);
  HWfloat Y_temp2(TMP, OPT_BW, OPT_BW);
  HWfloat Y_temp3(TMP, OPT_BW, OPT_BW);
  
  HWfloat Cb_temp0(TMP, OPT_BW, OPT_BW);
  HWfloat Cb_temp1(TMP, OPT_BW, OPT_BW);
  HWfloat Cb_temp2(TMP, 3 + 8, 8);
  HWfloat Cb_temp3(TMP, OPT_BW, OPT_BW);
 
  HWfloat Cr_temp0(TMP, 3 + 8, 8);
  HWfloat Cr_temp1(TMP, OPT_BW, OPT_BW);
  HWfloat Cr_temp2(TMP, OPT_BW, OPT_BW);
  HWfloat Cr_temp3(TMP, OPT_BW, OPT_BW);

  STREAM_LOOP( 10 )
 

    R = R_in;
  G = G_in;
  B = B_in;
  
 
  c_00 = const_c_00;
  c_01 = const_c_01;
  c_02 = const_c_02;
  c_10 = const_c_10;
  c_11 = const_c_11;
  c_21 = const_c_21;
  c_22 = const_c_22;
    
  mult_1 = const_mult_1;


  Y_temp0 = c_00 * R;
  Y_temp1 = c_01 * G;
  Y_temp2 = c_02 * B;
  Y_temp3 = Y_temp0 + Y_temp1;
  Y = Y_temp2 + Y_temp3; 
  
  Cb_temp0 = c_10 * R;
  Cb_temp1 = c_11 * G;
  Cb_temp2 = B * mult_1; 
  Cb_temp3 = Cb_temp0 + Cb_temp1;
  Cb = Cb_temp2 + Cb_temp3; 
  
  Cr_temp0 = R * mult_1;
  Cr_temp1 = c_21 * G;
  Cr_temp2 = c_22 * B;
  Cr_temp3 = Cr_temp0 + Cr_temp1;
  Cr = Cr_temp2 + Cr_temp3; 
  
 
  STREAM_END

  return 0;  
 
}
