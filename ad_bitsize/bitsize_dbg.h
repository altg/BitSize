// $Author: amag97 $
// $Id: bitsize_dbg.h,v 1.5 2005/05/06 13:56:21 amag97 Exp $

#ifndef BITSIZE_DBG_H
#define BITSIZE_DBG_H

#ifdef _BITSIZE_DEBUG_

#define _DBOUT( _EXP ) cerr <<__FILE__ << ":" << __LINE__ << " " << _EXP << endl;
#define _SHOW_VAR( _EXP ) _DBOUT( ##_EXP << _EXP )


#else

#define _DBOUT( _EXP )

#endif

enum BWStateType{ SET , NOTSET };

#include<iostream>

// Code courtsey of FADBAD++
inline void ReportError(const char* errmsg)
{
  std::cout<<errmsg<<std::endl;
}

#define USER_ASSERT(check,msg)\
	if (!(check))\
	{\
		std::ostringstream ost;\
		ost<<"User assertion failed: \""<<msg<<"\", at line "<<__LINE__<<", file "<<__FILE__<<std::endl;\
		ReportError(ost.str().c_str());\
	}
#define INTERNAL_ASSERT(check,msg)\
	if (!(check))\
	{\
		std::ostringstream ost;\
		ost<<"Internal error: \""<<msg<<"\", at line "<<__LINE__<<", file "<<__FILE__<<std::endl;\
		ReportError(ost.str().c_str());\
	}
#define ASSERT(check)\
	if (!(check))\
	{\
		std::ostringstream ost;\
		ost<<"Internal error at line "<<__LINE__<<", file "<<__FILE__<<std::endl;\
		ReportError(ost.str().c_str());\
	}

#define INTERNAL_ASSERT_EXIT(check,msg)\
	if (!(check))\
	{\
		std::ostringstream ost;\
		ost<<"Internal error: \""<<msg<<"\", at line "<<__LINE__<<", file "<<__FILE__<<std::endl;\
		ReportError(ost.str().c_str());\
		exit(-1);\
	}

////////////////


#endif
