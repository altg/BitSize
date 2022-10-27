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


  DefaultSign = TWOSCOMPLEMENT;
  ROUNDING_CHOICE = NOROUND;
  STREAM_OPTIMIZE = THROUGHPUT;
  USE_HARDMULT = NO;
  
  HWfix R(IN , 4 + OPT_BW , OPT_BW, UNSIGNED); 
  HWfix G(IN , 4 + OPT_BW , OPT_BW, UNSIGNED); 
  HWfix B(IN , 4 + OPT_BW , OPT_BW, UNSIGNED); 
  
  HWfix Y(OUT , OPT_BW, OUTFRACBW); 
  HWfix Cb(OUT , OPT_BW, OUTFRACBW);   
  HWfix Cr(OUT , OPT_BW, OUTFRACBW); 
       
  HWfix c_00(TMP, OPT_BW, OPT_BW);
  HWfix c_01(TMP, OPT_BW, OPT_BW);
  HWfix c_02(TMP, OPT_BW, OPT_BW);
  HWfix c_10(TMP, OPT_BW, OPT_BW);
  HWfix c_11(TMP, OPT_BW, OPT_BW);
  HWfix c_21(TMP, OPT_BW, OPT_BW);
  HWfix c_22(TMP, OPT_BW, OPT_BW);
  
  HWfix mult_1(TMP, OPT_BW, OPT_BW);

  HWfix Y_temp0(TMP, OPT_BW, OPT_BW);
  HWfix Y_temp1(TMP, OPT_BW, OPT_BW);
  HWfix Y_temp2(TMP, OPT_BW, OPT_BW);
  HWfix Y_temp3(TMP, OPT_BW, OPT_BW);
  
  HWfix Cb_temp0(TMP, OPT_BW, OPT_BW);
  HWfix Cb_temp1(TMP, OPT_BW, OPT_BW);
  HWfix Cb_temp2(TMP, 0 + 8, 8);
  HWfix Cb_temp3(TMP, OPT_BW, OPT_BW);
 
  HWfix Cr_temp0(TMP, 0 + 8, 8);
  HWfix Cr_temp1(TMP, OPT_BW, OPT_BW);
  HWfix Cr_temp2(TMP, OPT_BW, OPT_BW);
  HWfix Cr_temp3(TMP, OPT_BW, OPT_BW);

  STREAM_LOOP( 10 )
  
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
