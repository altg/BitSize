//Code PreProcessed by ASC2BitSize
//(c) Altaf Abdul Gaffar
#include "bitsize.h"


//#include "lns.h"





#define UFB 27

#define UIB 8



#define HELL 0



#define OUTFRACBW R_ARGS(0)



int main(int argc, char **argv){



 double val_six = 2.584962500721156181453739;

 double val_three = 1.584962500721156181453739;



 STREAM_START;



 //HWfix tinput(IN, UFB+UIB, UFB, SIGNMAGNITUDE);

 

	HWlns u_in(IN, OPT_BW+2, OPT_BW,TWOSCOMPLEMENT, "u_in" );



	HWlns u(TMP, OPT_BW+2, OPT_BW,TWOSCOMPLEMENT, "u" );



	HWlns const_six(TMP, OPT_BW, OPT_BW,TWOSCOMPLEMENT, "const_six" );



	HWlns const_three(TMP, OPT_BW, OPT_BW,TWOSCOMPLEMENT, "const_three" );



	HWlns const_one(TMP, OPT_BW, OPT_BW,TWOSCOMPLEMENT, "const_one" );

	HWlns const_four(TMP, OPT_BW, OPT_BW,TWOSCOMPLEMENT, "const_four" );





 //temp variables



	HWlns u2p(TMP, OPT_BW, OPT_BW,TWOSCOMPLEMENT, "u2p" );

	HWlns u3p(TMP, OPT_BW, OPT_BW,TWOSCOMPLEMENT, "u3p" );



	HWlns u3t(TMP, OPT_BW, OPT_BW,TWOSCOMPLEMENT, "u3t" );

	HWlns u2p3t(TMP, OPT_BW, OPT_BW,TWOSCOMPLEMENT, "u2p3t" );

	HWlns u2p6t(TMP, OPT_BW, OPT_BW,TWOSCOMPLEMENT, "u2p6t" );

	HWlns u3p3t(TMP, OPT_BW, OPT_BW,TWOSCOMPLEMENT, "u3p3t" );



	HWlns b01(TMP, OPT_BW, OPT_BW,TWOSCOMPLEMENT, "b01" );

	HWlns b02(TMP, OPT_BW, OPT_BW,TWOSCOMPLEMENT, "b02" );

	HWlns b03(TMP, OPT_BW, OPT_BW,TWOSCOMPLEMENT, "b03" );

 



	HWlns b11(TMP, OPT_BW, OPT_BW,TWOSCOMPLEMENT, "b11" );

	HWlns b12(TMP, OPT_BW, OPT_BW,TWOSCOMPLEMENT, "b12" );





	HWlns b21(TMP, OPT_BW, OPT_BW,TWOSCOMPLEMENT, "b21" );

	HWlns b22(TMP, OPT_BW, OPT_BW,TWOSCOMPLEMENT, "b22" );

	HWlns b23(TMP, OPT_BW, OPT_BW,TWOSCOMPLEMENT, "b23" );









	HWlns b0(OUT, OPT_BW, OUTFRACBW,TWOSCOMPLEMENT, "b0" );

	HWlns b1(OUT, OPT_BW, OUTFRACBW,TWOSCOMPLEMENT, "b1" );

	HWlns b2(OUT, OPT_BW, OUTFRACBW,TWOSCOMPLEMENT, "b2" );

	HWlns b3(OUT, OPT_BW, OUTFRACBW,TWOSCOMPLEMENT, "b3" );





//  HWfix out0(OUT, 32, OPT_BW, SIGNMAGNITUDE);

//  HWfix out1(OUT, 32, OPT_BW, SIGNMAGNITUDE);

//  HWfix out2(OUT, 32, OPT_BW, SIGNMAGNITUDE);

//  HWfix out3(OUT, 32, OPT_BW, SIGNMAGNITUDE);



 STREAM_LOOP(1);



 //init_coeff();



 const_three = val_three;

 const_six = val_six;

 const_one = 0;

 const_four = 2;



 u2p = u * u;

 u3p = u2p * u;



 u3t = u * const_three;

 u2p3t = u2p * const_three;

 u2p6t = u2p * const_six;

 u3p3t = u3p * const_three;



 b01 = u2p3t - u3p;

 b02 = b01 - u3t;

 b03 = b02 + const_one;

 //b0 = b03 / const_six;



 b0 = b03 * const_six;



 b11 = u3p3t - u2p6t;

 b12 = b11 + const_four;

 //b1 = b12 / const_six;



 b1 = b12 * const_six;



 b21 = u2p3t - u3p3t;

 b22 = b21 + u3t;

 b23 = b22 + const_one;

 b2 = b23 * const_six;



 // b2 = b23 / const_six;



 // b3 = - u3p / const_six;



 b3 = - u3p * const_six;



//  out0 = *b0.ptr_val;

//  out1 = *b0.ptr_val;

//  out2 = *b0.ptr_val;

//  out3 = *b0.ptr_val;



 STREAM_END;



} 
