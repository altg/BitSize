// $Author: amag97 $
// $Id: hwfix.h,v 1.16 2004/09/17 16:52:13 amag97 Exp $
// $Log: hwfix.h,v $
// Revision 1.16  2004/09/17 16:52:13  amag97
// Added area estimation code
//
// Revision 1.15  2004/07/21 18:22:04  amag97
// Pre-vacation
//
// Revision 1.14  2004/07/06 19:48:11  amag97
// Revised DAG generation
//
// Revision 1.13  2004/06/24 18:45:31  amag97
// Added a seperate class to encapsulate the bitwidth optimisation functions
//
// Revision 1.12  2004/06/21 16:23:20  amag97
// Post FPT04 commit
//
// Revision 1.11  2004/06/10 19:57:23  amag97
// Added leading one detect
//
// Revision 1.10  2004/06/09 21:20:51  amag97
// Added more bitwidth analysis methods for fixed-point
//
// Revision 1.9  2004/06/08 21:07:40  amag97
// More bugfixes to - advar
//
// Revision 1.8  2004/06/07 19:51:08  amag97
// Global HWvector collection
// Bugfixes to copy constructor
//
// Revision 1.7  2004/06/07 11:28:07  amag97
// ASC integration - 2
//
// Revision 1.6  2004/06/05 17:34:18  amag97
// More bug fixes to ASC integration
//
// Revision 1.5  2004/06/03 22:01:30  amag97
// More bug fixes
// Added HWint
//
// Revision 1.4  2004/06/02 19:37:59  amag97
// Added HWvector
//
// Revision 1.3  2004/06/01 19:31:17  amag97
// More fixes to DAG code
//
// Revision 1.2  2004/06/01 11:31:55  amag97
// DAG code
//
// Revision 1.1  2004/05/28 20:14:27  amag97
// ASC Integration
//

// Interface for A Stream Compiler ( ASC ) system for fixed point

#ifndef _HWFIX_H_
#define _HWFIX_H_





#include "bitsize_defs.h"

#include "hwint.h"

#include "hwvector.h"

#include "bwoptfix.h"

//#include "hwvar.h"


// ASC Macro Overides


/////////////////////////////////////





//////////////////////////////////////

// BW options

//#define HWabs( _VAR ) ( _VAR > 0 ) ? _VAR : _VAR * -1.;



// using namespace std;

// //classWvar;
// class  HWfix : public HWvar{

// public:


//  /*HWfix( );	 */
//  //HWfix( char* name = NULL , advarType vtype = TMP );
//   // HWfix( char* name = NULL , advarType vtype = AUTO_TMP , double vman_bw = 23 , double vexp_bw = 8 , double vouterr_spec = 0.0 );
//   //
//  HWfix();

//  HWfix(arch_attr Signal_type, fsize init_size = 16 , fsize fractional = 8,sign_attr sign=TWOSCOMPLEMENT, string name="");

//  HWfix ( const HWfix &a );

  
//  ~HWfix();
 

//  double get_val();
//  HWfix& operator= ( const HWint &a);
//  HWfix& operator= ( const HWfix &r);
//  HWfix& operator= ( double val);

// HWfix& operator= (  HWfix &r);  
// void operator= (HWfix *a);
  
//  HWfix& operator<<= (double val);

//   //operator double();

//   // Members

//   fsize frac_bw;
//   fsize int_bw;

//   BWStateType frac_bw_state;
//   BWStateType int_bw_state;

//   sign_attr c_sign;
 
//  void analyse_bw( int bw , int mode = 1);

//  void print_advar();



//   void read_inputdata( double );

 
  


//   // Static members
//  static void init(); 

//  static void output_results();

//  static void output_results2();

//  static void output_analysis( int mode = 1 );

//  static void output_analysis2( );  

//   // Dot graph generation helper functions

//  string print_dot_bitinfo();

//  HWint operator[]( int index );
  

//   // Global storage for vectors
//  static vector<HWvector<HWfix>* > vec_c;
 
//  static void fix_vecbitwidths();

// };


// //HWfix HWabs( const HWfix& a ); 

// HWfix operator+  (const HWfix &l, const HWfix &r);
// HWfix operator+  (const double dval, const HWfix &r);
// HWfix operator+  (const HWfix &l, const double dval);
// HWfix operator+  (int val,const HWfix &r); 
// HWfix operator+  (const HWfix &l, int val); 

// HWfix operator-  (const HWfix &l, const HWfix &r);  
// HWfix operator-  (const double dval,const HWfix &r); 
// HWfix operator-  (const HWfix &l, const double dval);

// HWfix operator*  (const HWfix &l, const HWfix &r); 
// HWfix operator*  (const double dval,const HWfix &r);  
// HWfix operator*  (const HWfix &l, const double dval); 

// HWfix operator/  (const HWfix &l, const HWfix &r); 
// HWfix operator/  (const double dval,const HWfix &r);  
// HWfix operator/  (const HWfix &l, const double dval); 





// //HWfix operator<< (HWfix &l, double &r );



//   /*HWfix::HWfix (arch_attr tp, HWfix *m, HWfix *e ):

//   }*/

//   /*HWfix::HWfix (HWfix *m, HWfix *e ){

//   }*/

//   // Returns the truncated version of the float
//   /*HWfix & HWfix::truncate() {
//   }*/

  
// //equal operator based on rounding mode
  
// //  HWfix & HWfix::operator= ( HWfix &r ) {
// //  }
// //
// //  HWfix & HWfix::operator= ( HWint &r ) {
// //  }
// //
 
// //
// //  HWfix & HWfix::operator= (int val) { 
// //  }
// //
// //

// //
// //
// //HWfix & operator-  (int val,HWfix &r)  { }
// //HWfix & operator-  (HWfix &l, int val)  { }
// //
// //
// //HWfix & operator*  (int val,HWfix &r)  { }
// //HWfix & operator*  (HWfix &l, int val)  { }
// //
// //HWfix & operator/  (HWfix &l,HWfix &r)  { }
// //HWfix & operator/  (HWfix &l,int val)    { /* error */ }
// //HWfix & operator/  (HWfix &l,double dval)    { /* error */ }
// //HWfix & operator/  (int val,HWfix &r)    { }
// //HWfix & operator/  (double dval,HWfix &r)    { }
// //
// //HWfix & operator%  (HWfix &l,HWfix &r)  { }
// //HWfix & operator%  (HWfix &l,int val) { }
// //HWfix & operator%  (int val,HWfix &r) { }
// //HWfix & operator%  (HWfix &l,double dval) { } 
// //HWfix & operator%  (double dval,HWfix &r) { }
// //
// //HWfix & operator+= (HWfix &l, HWfix &r){ }
// //HWfix & operator-= (HWfix &l, HWfix &r){ }
// //HWfix & operator*= (HWfix &l, HWfix &r){ }
// //HWfix & operator/= (HWfix &l, HWfix &r){ }
// //HWfix & operator%= (HWfix &l, HWfix &r){ }
// //
// //HWfix & operator+= (HWfix &l,int val){ }
// //HWfix & operator-= (HWfix &l,int val){ }
// //HWfix & operator*= (HWfix &l,int val){ }
// //HWfix & operator/= (HWfix &l,int val){ }
// //HWfix & operator%= (HWfix &l,int val){ }
// //
// //HWfix & operator+= (HWfix &l,double val){ }
// //HWfix & operator-= (HWfix &l,double val){ }
// //HWfix & operator*= (HWfix &l,double val){ }
// //HWfix & operator/= (HWfix &l,double val){ }
// //HWfix & operator%= (HWfix &l,double val) { }
// //
// //HWfix & operator-  (HWfix &l){ }
// //
// HWfix & operator>>= (HWfix &l,HWint &r){ }
// HWfix & operator>>= (HWfix &l,int val) { }
// HWfix & operator<<= (HWfix &l,HWint &r){ }
// HWfix & operator<<= (HWfix &l,int val) { }
// //
// HWfix operator>> (const HWfix &l,const HWint &r);
// HWfix operator>> (const HWfix &l,int val); 
// HWfix operator<< (const HWfix &l,const HWint &r);
// HWfix operator<< (const HWfix &l,int val); 
// //
// ////Control 
// //HWint & operator== (HWfix &l,HWfix &r) { }
// //HWint & operator== (double val,HWfix &r) { }
// //HWint & operator== (HWfix &l,double val) { }
// //HWint & operator== (int val,HWfix &r)    { }
// //HWint & operator== (HWfix &l,int val)    { }
// //
// //HWint & operator!= (HWfix &l,HWfix &r) { }
// //HWint & operator!= (double val,HWfix &r) { }
// //HWint & operator!= (HWfix &l,double val) { }
// //HWint & operator!= (int val,HWfix &r)    { }
// //HWint & operator!= (HWfix &l,int val)    { }
// //
// HWint operator>  (const HWfix &l,const HWfix &r);
// HWint operator>  (double val,const HWfix &r);
// HWint operator>  (const HWfix &l,double val);
// HWint operator>  (int val,const HWfix &r);
// HWint operator>  (const HWfix &l,int val);

// //
// //HWint & operator<  (HWfix &l,HWfix &r) { }
// //HWint & operator<  (double val,HWfix &r) { }
// //HWint & operator<  (HWfix &l,double val) { }
// //HWint & operator<  (int val,HWfix &r) { }
// //HWint & operator<  (HWfix &l,int val) { }
// //
// //HWint & operator>=  (HWfix &l,HWfix &r) { }
// //HWint & operator>=  (double val,HWfix &r) { }
// //HWint & operator>=  (HWfix &l,double val) { }
// //HWint & operator>=  (int val,HWfix &r) { }
// //HWint & operator>=  (HWfix &l,int val) { }
// //
// //HWint & operator<=  (HWfix &l,HWfix &r) { }
// //HWint & operator<=  (double val,HWfix &r) { }
// //HWint & operator<=  (HWfix &l,double val) { }
// //HWint & operator<=  (int val,HWfix &r) { }
// //HWint & operator<=  (HWfix &l,int val) { }


// HWint op_lod( const HWfix& var );



#endif

