// $Author: amag97 $
// $Id: hwfloat.h,v 1.6 2004/11/28 23:29:02 amag97 Exp $
// $Log: hwfloat.h,v $
// Revision 1.6  2004/11/28 23:29:02  amag97
// Added back support for HWfloat
// Uniform bit-width optimisation for HWfloat
//
// Revision 1.5  2004/11/26 18:20:41  amag97
// Data flow graph based automatic differentiation
//
// Revision 1.4  2004/09/30 20:32:51  amag97
// Added Area model
// Lots of fixes to get it to work
//
// Revision 1.3  2004/06/01 19:31:17  amag97
// More fixes to DAG code
//
// Revision 1.2  2004/05/28 20:14:27  amag97
// ASC Integration
//
// Revision 1.1  2004/05/14 17:33:20  amag97
// Intgeration with ASC system
//

// Interface for A Stream Compiler ( ASC ) system for floating point

#ifndef _HWFLOAT_H_
#define _HWFLOAT_H_

#include "hwvar.h"
#include "bitsize_defs.h"


// ASC Macro Overides

// #define STREAM_START HWfloat::init();

// #define STREAM_END HWfloat::output_analysis();}\
//                        HWfloat::output_results();

// #define STREAM_END_ARG(_XARG ) HWfloat::output_analysis( _XARG );}\
//                        HWfloat::output_results();


// Depreciated 
#define DATA_STREAM_END HWfloat::output_analysis();}
#define DATA_STREAM_END_ARG( _XARG ) HWfloat::output_analysis( _XARG );}

///////////////////////////////////////



//////////////////////////////////////

// BW options

// #define OPT_BW 9999

using namespace std;

// #define _DLLDEC_ASC




  /*HWfloat::HWfloat (arch_attr tp, HWfix *m, HWfix *e ):

  }*/

  /*HWfloat::HWfloat (HWfix *m, HWfix *e ){

  }*/

  // Returns the truncated version of the float
  /*HWfloat & HWfloat::truncate() {
  }*/

  
// equal operator based on rounding mode
  
//  HWfloat & HWfloat::operator= ( HWfix &r ) {
//  }
//
//  HWfloat & HWfloat::operator= ( HWint &r ) {
//  }
//
//  
//
//  HWfloat & HWfloat::operator= (int val) { 
//  }
//
//
//HWfloat & operator+  (int val,HWfloat &r)  {  }
//HWfloat & operator+  (HWfloat &l, int val)  {  }
//
//
//HWfloat & operator-  (int val,HWfloat &r)  { }
//HWfloat & operator-  (HWfloat &l, int val)  { }
//
//
//HWfloat & operator*  (int val,HWfloat &r)  { }
//HWfloat & operator*  (HWfloat &l, int val)  { }
//
//HWfloat & operator/  (HWfloat &l,HWfloat &r)  { }
//HWfloat & operator/  (HWfloat &l,int val)    { /* error */ }
//HWfloat & operator/  (HWfloat &l,double dval)    { /* error */ }
//HWfloat & operator/  (int val,HWfloat &r)    { }
//HWfloat & operator/  (double dval,HWfloat &r)    { }
//
//HWfloat & operator%  (HWfloat &l,HWfloat &r)  { }
//HWfloat & operator%  (HWfloat &l,int val) { }
//HWfloat & operator%  (int val,HWfloat &r) { }
//HWfloat & operator%  (HWfloat &l,double dval) { } 
//HWfloat & operator%  (double dval,HWfloat &r) { }
//
//HWfloat & operator+= (HWfloat &l, HWfloat &r){ }
//HWfloat & operator-= (HWfloat &l, HWfloat &r){ }
//HWfloat & operator*= (HWfloat &l, HWfloat &r){ }
//HWfloat & operator/= (HWfloat &l, HWfloat &r){ }
//HWfloat & operator%= (HWfloat &l, HWfloat &r){ }
//
//HWfloat & operator+= (HWfloat &l,int val){ }
//HWfloat & operator-= (HWfloat &l,int val){ }
//HWfloat & operator*= (HWfloat &l,int val){ }
//HWfloat & operator/= (HWfloat &l,int val){ }
//HWfloat & operator%= (HWfloat &l,int val){ }
//
//HWfloat & operator+= (HWfloat &l,double val){ }
//HWfloat & operator-= (HWfloat &l,double val){ }
//HWfloat & operator*= (HWfloat &l,double val){ }
//HWfloat & operator/= (HWfloat &l,double val){ }
//HWfloat & operator%= (HWfloat &l,double val) { }
//
//HWfloat & operator-  (HWfloat &l){ }
//
//HWfloat & operator>>= (HWfloat &l,HWint &r){ }
//HWfloat & operator>>= (HWfloat &l,int val) { }
//HWfloat & operator<<= (HWfloat &l,HWint &r){ }
//HWfloat & operator<<= (HWfloat &l,int val) { }
//
//HWfloat & operator>> (HWfloat &l,HWint &r) { }
//HWfloat & operator>> (HWfloat &l,int val)  { }
//HWfloat & operator<< (HWfloat &l,HWint &r) { }
//HWfloat & operator<< (HWfloat &l,int val)  { }
//
////Control 
//HWint & operator== (HWfloat &l,HWfloat &r) { }
//HWint & operator== (double val,HWfloat &r) { }
//HWint & operator== (HWfloat &l,double val) { }
//HWint & operator== (int val,HWfloat &r)    { }
//HWint & operator== (HWfloat &l,int val)    { }
//
//HWint & operator!= (HWfloat &l,HWfloat &r) { }
//HWint & operator!= (double val,HWfloat &r) { }
//HWint & operator!= (HWfloat &l,double val) { }
//HWint & operator!= (int val,HWfloat &r)    { }
//HWint & operator!= (HWfloat &l,int val)    { }
//
//HWint & operator>  (HWfloat &l,HWfloat &r) { }
//HWint & operator>  (double val,HWfloat &r) { }
//HWint & operator>  (HWfloat &l,double val) { }
//HWint & operator>  (int val,HWfloat &r) { }
//HWint & operator>  (HWfloat &l,int val) { }
//
//HWint & operator<  (HWfloat &l,HWfloat &r) { }
//HWint & operator<  (double val,HWfloat &r) { }
//HWint & operator<  (HWfloat &l,double val) { }
//HWint & operator<  (int val,HWfloat &r) { }
//HWint & operator<  (HWfloat &l,int val) { }
//
//HWint & operator>=  (HWfloat &l,HWfloat &r) { }
//HWint & operator>=  (double val,HWfloat &r) { }
//HWint & operator>=  (HWfloat &l,double val) { }
//HWint & operator>=  (int val,HWfloat &r) { }
//HWint & operator>=  (HWfloat &l,int val) { }
//
//HWint & operator<=  (HWfloat &l,HWfloat &r) { }
//HWint & operator<=  (double val,HWfloat &r) { }
//HWint & operator<=  (HWfloat &l,double val) { }
//HWint & operator<=  (int val,HWfloat &r) { }
//HWint & operator<=  (HWfloat &l,int val) { }

#endif

