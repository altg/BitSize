// $Author: amag97 $
// $Id: bitsize_defs.h,v 1.10 2006/02/17 18:14:55 amag97 Exp $
// $Log: bitsize_defs.h,v $
// Revision 1.10  2006/02/17 18:14:55  amag97
// Added VHDL generation pass
//
// Revision 1.9  2005/05/06 13:56:21  amag97
// Pre-Power
//
// Revision 1.8  2004/10/23 10:37:33  amag97
// Addition of new fraction bw evaluation method
// (work in progress)
//
// Revision 1.7  2004/09/30 20:32:51  amag97vh
// Added Area model
// Lots of fixes to get it to work
//
// Revision 1.6  2004/07/12 20:08:11  amag97
// Update
//
// Revision 1.5  2004/07/06 19:48:11  amag97
// Revised DAG generation
//
// Revision 1.4  2004/06/24 18:45:31  amag97
// Added a seperate class to encapsulate the bitwidth optimisation functions
//
// Revision 1.3  2004/06/03 22:01:30  amag97
// More bug fixes
// Added HWint
//
// Revision 1.2  2004/06/02 19:37:59  amag97
// Added HWvector
//
// Revision 1.1  2004/05/28 20:14:27  amag97
// ASC Integration
//

#ifndef _BITSIZE_DEFS_H
#define _BITSIZE_DEFS_H

//////////////////////////////////////

#include "bitsize_dbg.h"

#include <vector>
#include <string>
#include <iostream>
#include <cstdlib>
#include <cmath>

using namespace std;
 
enum arch_attr {IN , OUT , TMP , OP_IN, REGISTER, MAPPED_REGISTER, 
                INTMEM, EXTMEM0, EXTMEM1, EXTMEM2, EXTMEM3, 
                EXTMEM4, EXTMEM5, EXTMEM6, EXTMEM7, ECONSTANT};

enum sign_attr {UNSIGNED, TWOSCOMPLEMENT, SIGNMAGNITUDE, ONESCOMPLEMENT, BIASED};

//enum BWStateType{ SET , NOTSET };

typedef unsigned int fsize;

typedef arch_attr Edge_TYPE; 
//enum Edge_TYPE {IN , OUT , TMP , ECONSTANT };

typedef long long int64;

////////////////////////////////////////




///////////////////////////////////////
class HWvar;
class HWint;
class HWfix;
class HWfloat;


class DAG;
class node;

class bitsize;

#include "bitsize_class.h"

//extern __declspec(dllimport) bitsize* bs;
extern bitsize* bs;

#define IF( _COND , _EXP1 , _EXP2 ) ( _COND ) ? ( _EXP1 ) : ( _EXP2 )

enum YesNo {NO, YES};

// lib defines while user just declares extern
#ifdef BITSIZE_DEFS

arch_attr DefaultArch=TMP;
sign_attr DefaultSign=TWOSCOMPLEMENT;
fsize DefaultSize=16;
fsize DefaultFract=0;
YesNo USE_HARDMULT=N0;

#else
#define OPT_BW 9999
extern arch_attr DefaultArch;
extern sign_attr DefaultSign;
extern fsize DefaultSize;
extern fsize DefaultFract;
extern YesNo USE_HARDMULT;


#endif

#define ERROR(_x) { cerr << _x << endl; exit(1); }

#endif



#ifdef _BITSIZE_DEBUG_LOG

#define DBLOG( _STR ) cout << _STR << endl;

#else

#define DBLOG( _STR )

#endif
