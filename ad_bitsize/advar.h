// $Id: advar.h,v 1.38 2006/02/17 18:14:54 amag97 Exp $
// $Author: amag97 $
// $Log: advar.h,v $
// Revision 1.38  2006/02/17 18:14:54  amag97
// Added VHDL generation pass
//
// Revision 1.37  2005/05/06 13:56:21  amag97
// Pre-Power
//
// Revision 1.36  2004/10/26 18:32:59  amag97
// Fixed most bugs with fixed-point frac. opt 6
//
// Revision 1.35  2004/06/24 18:45:31  amag97
// Added a seperate class to encapsulate the bitwidth optimisation functions
//
// Revision 1.34  2004/06/09 21:20:51  amag97
// Added more bitwidth analysis methods for fixed-point
//
// Revision 1.33  2004/06/08 21:07:40  amag97
// More bugfixes to - advar
//
// Revision 1.32  2004/06/08 01:35:46  amag97
// Fixed HWvector bitwidth calculation
//
// Revision 1.31  2004/06/05 17:34:18  amag97
// More bug fixes to ASC integration
//
// Revision 1.30  2004/06/01 11:31:55  amag97
// DAG code
//
// Revision 1.29  2004/05/28 20:14:27  amag97
// ASC Integration
//
// Revision 1.28  2004/05/14 17:33:20  amag97
// Intgeration with ASC system
//
// Revision 1.27  2004/04/22 13:24:57  amag97
// Pre-ASC
//
// Revision 1.26  2004/04/06 17:42:06  amag97
// Changes to Background
//
// Revision 1.25  2004/02/27 18:27:02  amag97
// Version without MpFloat
//
// Revision 1.24  2004/02/27 11:39:23  amag97
// Final version before MpFloat removal
//
// Revision 1.23  2004/02/24 11:58:36  amag97
// ad_bitsize: Imrovements to bit-width determination functions
// others: many changes and updates
//
// Revision 1.22  2003/12/23 18:30:23  amag97
// More changes to bitwidth methods
//
// Revision 1.21  2003/12/11 20:39:44  amag97
// Added bit width analysis method 4
//
// Revision 1.20  2003/12/11 11:16:24  amag97
// Added new bitwidth analysis methods
//
// Revision 1.19  2003/11/28 15:28:15  amag97
// Latest changes
//
// Revision 1.18  2003/09/13 21:38:26  amag97
// Added bitwidth info access functions
//
// Revision 1.17  2003/09/09 19:16:45  amag97
// More functions for bitwidth analysis and output
//
// Revision 1.16  2003/08/22 17:10:24  amag97
// Improvements to bitwidth calculation
//
// Revision 1.15  2003/08/06 16:32:36  amag97
// Changes to var numbering and automated bit config m4 file in advar
// Added m4 macro file 'bitinfo.m4'
// Changes to simulation file 'test_ann3.cpp'
//
// Revision 1.14  2003/07/28 09:33:01  amag97
// Improvements to bit width analysis
//
// Revision 1.13  2003/07/25 20:03:51  amag97
// Added fixed point bitwidth calculation
//
// Revision 1.12  2003/07/24 21:07:23  amag97
// Improvements to test functions
//
// Revision 1.11  2003/07/23 23:51:47  amag97
// Changes to bitwidth calculation
// New testfiles
//
// Revision 1.10  2003/07/23 11:04:22  amag97
// More bug fixes to bitwidth calculation section
//
// Revision 1.9  2003/07/18 00:34:30  amag97
// Added bitwidth analysis functions
// More debugging
//
// Revision 1.8  2003/07/15 18:48:52  amag97
// Added bitwidth analysis methods
//
// Revision 1.7  2003/07/10 20:28:42  amag97
// Use MpFloat for calculations
//
// Revision 1.6  2003/07/09 11:32:55  amag97
// Added '/' function
//
// Revision 1.5  2003/07/03 01:01:27  amag97
// Added sine, cosine , log functions
//
// Revision 1.4  2003/07/02 11:30:55  amag97
// More debugging and bug fixes
//
// Revision 1.3  2003/07/01 01:22:00  amag97
// Added global vector for advar
//
// Revision 1.2  2003/06/26 23:32:00  amag97
// Added sub , mult
//
// Revision 1.1  2003/06/25 19:26:59  amag97
// Initial Commit
//

#ifndef _AD_VAR_H
#define _AD_VAR_H
#pragma warning(disable:4251)

#include <iostream>

#include <iomanip>
//#include <algorithm>
#include <list>
#include <vector>
#include <cfloat>
#include <cmath>

#include <fstream>

#include <string>
using namespace std;

//#include "bitsize_defs.h"
#include "bitsize_dbg.h"

#define BRK_PT  __asm int 3


#ifdef _MSC_VER

#ifdef AD_BITSIZE_EXPORTS
//#define _DLLDEC_AD __declspec(dllexport) 
#else
//#define _DLLDEC_AD __declspec(dllimport)
//#define _DLLDEC_AD
//#error here
#endif

#endif 



#ifdef __GNUC__
#define _DLLDEC_AD
#endif


#define AD_DEFAULT 0x00
#define AD_AUTONUM 0x01

// Notes:
// Calculations can be done are done using IEEE Doubles




#define DTYPE double

#define MAX( A , B ) ( abs(A) > abs(B) ) ? A : B
#define MIN( A , B ) ( abs(A) < abs(B) ) ? A : B

#define MAX_VAL( _V )\
                ref_maxval = MAX( _V , ref_maxval )
#define MIN_VAL( _V )\
                ref_minval = MIN( _V , ref_minval )
#define ABS_AD( _V ) fabs( _V )




//enum BWStateType{ SET , NOTSET };



class BSClass;

//BSClass BS;

class _DLLDEC_AD advar{

 public:
  
  enum advarType{ IN , OUT , TMP , AUTO_TMP };

  typedef vector<DTYPE>::iterator iter_t;
  mutable vector<DTYPE> grads;
  mutable vector<DTYPE> max_grads;

  DTYPE ref_value;
  DTYPE ref_minval;
  DTYPE ref_maxval;

  bool indep_var;
  bool depend_var;
  size_t var_number;
  size_t advar_vec_offset;
  
  
  advarType varType;

  // Global static variables
  static size_t var_count;
  static bool auto_numbering;
  
  
  void initvar( size_t var_num = var_count++);

  
  // Bitwidth calculation data members
  DTYPE output_perr;
  DTYPE m_bitwidth;
  DTYPE e_bitwidth;

  BWStateType BWState;	

  DTYPE fp_frac_bw;
  DTYPE fp_int_bw;
  BWStateType fix_frac_state;
  BWStateType fix_int_state;

  // Variable naming

  string var_name;


  // Bit-width analysis functions
  DTYPE get_output_err_flt(list<int>::iterator , list<int>::iterator);
  DTYPE get_output_err_fx(list<int>::iterator  , list<int>::iterator);
 
  static void analyse_flt_e_bw();
  static void analyse_fx_int_bw();

 //public:
  

  advar( advarType vtype = AUTO_TMP );

  advar( string vname , advarType vtype = TMP  );

  advar( const advar& );

  void  copy( const advar& );

  advar( const DTYPE fltValue , size_t var_num = var_count++);
  

  advar( string vname , size_t m_size , size_t e_size , size_t var_num = var_count++);
  
  ~advar(); 


  // Naming the variables
  void set_var_name( string name );

   //advar( std::string& varName_in );

 

  advar& operator = ( const advar& param_val );
  advar& operator = ( const DTYPE refValue_in );

  // Operator to set independent variable

  advar& operator <<= ( const advar& param_val );
  advar& operator <<= ( const DTYPE param_val );
  void set_indep();

  // Mark depenedent variable 
  advar& operator >>= ( const advar& param_val ){ return *this;}
  void set_depend();
  void set_error( double err);

  // Analysis functions

  void find_bitwidths();
  
  

  // Fixed point analysis functions
  void analyse_bw_fx( double perr );
  void analyse_bw_ulp_fx( double ulp_err );
  
  void analyse_bw_fx_fracbw( int method );
  void analyse_bw_fx_1( double bit_err );
  void analyse_bw_fx_2( double bit_err );
  void analyse_bw_fx_3( double bit_err );
  void analyse_bw_fx_4( double bit_err );


  // Floating point analysis functions

  void analyse_bw_flt_mbw( int method );
  void analyse_bw_flt_mbw( int m_bw , int method );

  void analyse_bw_flt( double perr );
  void analyse_bw_ulp_flt( double ulp_err );


  // Analysis method2

  void analyse2_bw_flt( double );
  void analyse2_bw_ulp_flt( double , int min_bw = 5);	
  void bw_method2( double  , int = 5);

  void analyse3_bw_flt( double );
  void analyse3_bw_ulp_flt( double );
  void bw_method3( double );

  void analyse4_bw_flt( double );
  void analyse4_bw_ulp_flt( double );
  void bw_method4( double );
  
  
  // Bitwidth access functions

  // Floating point
  DTYPE get_m_bw();
  DTYPE get_e_bw();

  // Fixed point
  DTYPE get_i_bw();
  DTYPE get_f_bw();
  

  string get_var_name();

  DTYPE operator[]( size_t wrt_no );
  DTYPE operator[]( const advar& wrt_ad );

  operator DTYPE();

  advar& operator += ( const advar& pr );
   friend const advar operator + ( const advar& advar_lhs , const DTYPE fltValue_rhs );
   friend const advar operator + ( const DTYPE fltValue_lhs , const advar& advar_rhs);
   friend const advar operator + ( const advar& advar_lhs , const advar& advar_rhs);

  advar& operator -= ( const advar& pr );
   friend const advar operator - ( const advar& advar_lhs , const DTYPE fltValue_rhs );
   friend const advar operator - ( const DTYPE fltValue_lhs , const advar& advar_rhs );
   friend const advar operator - ( const advar& advar_lhs , const advar& advar_rhs);

  advar& operator *= ( const advar& pr );
   friend const advar operator * ( const advar& advar_lhs , const DTYPE fltValue_rhs );
   friend const advar operator * ( const DTYPE fltValue_lhs , const advar& fltValue_rhs );
   friend const advar operator * ( const advar& advar_lhs , const advar& advar_rhs);

  advar& operator /= ( const advar& pr );
   friend const advar operator / ( const advar& advar_lhs , const DTYPE fltValue_rhs );
   friend const advar operator / ( const DTYPE fltValue_lhs , const advar& fltValue_rhs );
   friend const advar operator / ( const advar& advar_lhs , const advar& advar_rhs);


   friend const advar pow( const advar& advar_val , signed long n);
   friend const advar exp( const advar& advar_val );
   friend const advar log( const advar& advar_val );
   friend const advar sin( const advar& advar_val );
   friend const advar cos( const advar& advar_val );
/*   _DLLDEC_AD friend const advar tan( const advar& advar_val ){}; */


  _DLLDEC_AD friend ostream& operator << ( ostream& os , advar& pr );
  
  _DLLDEC_AD friend istream& operator >> ( istream& is , advar& pr );

  //advar( PrecType in_m_bits , PrecType in_exp_bits );

  //inline T get_rvalue() const { return refValue; } 

  //	inline double get_cusvalue() const { return cusValue.f_value(); }

  //inline T get_error(){ return refValue - (T)(cusValue); }

  static void advar_setup( unsigned int control_W , size_t Default_Prec = 23);

  

  void print_table_flt();
  void print_mbw( ostream &os);

  void print_table_fx();
  

  void output_bitspec( ostream &os );

  void output_bitspec2( const string &fname );


  // VHDL print outs

  static void print_VHDL_flt_bw();

  // Matlab - fixed point 
  static void print_fx_mat( ostream &os );
  void print_flt_mat( ostream &os );

  /*typedef vector<advar *> advar_list;
  static advar_list advar_vec;*/


  // Named vars

 static list<advar *> named_vars;


  static BSClass BS;	

  // Output information for data flow graph 

  void print_flt_dfg( ostream &os );
  void print_fx_dfg( ostream &os );

  // Utility functions
#ifdef _MSC_VER
  static double log2( double );
#endif

  double get_exp_value( double );
	
  void print_flt_make_defs( ostream &os );


  //#ifdef _BITSIZE_DEBUG_
  void print_grads_vec();
  //#endif

  

  /* Output error spec */

  double output_errspec;



  void reset_grad_vec();


};



#endif 
