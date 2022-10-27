#if OPTTYPE == 0

#if OPTMODE == 0

#include "stream_af_uni.cxh"

#elif OPTMODE == 1

#include "stream_af_nonuni.cxh"

#endif

#elif OPTTYPE == 1

#if OPTMODE == 0

#include "stream_ad_uni.cxh"

#elif OPTMODE == 1

#include "stream_ad_nonuni.cxh"

#endif

#else

#error "OPTTYPE not supported"

#endif

