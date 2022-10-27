#if OPTTYPE == 0


#if OUTFRACBW=12

#include "stream_af_12_nonuni.cxh"

#elif OUTFRACBW=14

#include "stream_af_14_nonuni.cxh"

#elif OUTFRACBW=16

#include "stream_af_16_nonuni.cxh"

#elif OUTFRACBW=18

#include "stream_af_18_nonuni.cxh"

#elif OUTFRACBW=20

#include "stream_af_20_nonuni.cxh"

#else

#error "OPTTYPE not supported"

#endif




#elif OPTTYPE == 1



#if OUTFRACBW=12

#include "stream_ad_12_nonuni.cxh"

#elif OUTFRACBW=14

#include "stream_ad_14_nonuni.cxh"

#elif OUTFRACBW=16

#include "stream_ad_16_nonuni.cxh"

#elif OUTFRACBW=18

#include "stream_ad_18_nonuni.cxh"

#elif OUTFRACBW=20

#include "stream_ad_20_nonuni.cxh"

#else

#error "OPTTYPE not supported"

#endif




#ednif
