// $Author: amag97 $
// $Id: hwvar.h,v 1.18 2005/12/13 14:25:40 amag97 Exp $
// $Log: hwvar.h,v $
// Revision 1.18  2005/12/13 14:25:40  amag97
// New compiler version fixes
//
// Revision 1.17  2004/12/29 13:51:39  amag97
// Added DIV support - not tested
//
// Revision 1.16  2004/12/20 21:28:08  amag97
// Added floating-point verification matlab code generation
//
// Revision 1.15  2004/11/28 23:29:02  amag97
// Added back support for HWfloat
// Uniform bit-width optimisation for HWfloat
//
// Revision 1.14  2004/11/12 19:05:24  amag97
// More fixes to affine erromodel
//
// Revision 1.13  2004/10/26 18:32:59  amag97
// Fixed most bugs with fixed-point frac. opt 6
//
// Revision 1.12  2004/10/23 10:37:37  amag97
// Addition of new fraction bw evaluation method
// (work in progress)
//
// Revision 1.11  2004/09/30 20:32:52  amag97
// Added Area model
// Lots of fixes to get it to work
//
// Revision 1.10  2004/09/17 16:52:13  amag97
// Added area estimation code
//
// Revision 1.9  2004/07/12 20:08:11  amag97
// Update
//
// Revision 1.8  2004/07/06 19:48:11  amag97
// Revised DAG generation
//
// Revision 1.7  2004/06/08 21:07:40  amag97
// More bugfixes to - advar
//
// Revision 1.6  2004/06/08 01:35:46  amag97
// Fixed HWvector bitwidth calculation
//
// Revision 1.5  2004/06/07 19:51:08  amag97
// Global HWvector collection
// Bugfixes to copy constructor
//
// Revision 1.4  2004/06/03 22:01:31  amag97
// More bug fixes
// Added HWint
//
// Revision 1.3  2004/06/02 19:37:59  amag97
// Added HWvector
//
// Revision 1.2  2004/06/01 19:31:17  amag97
// More fixes to DAG code
//
// Revision 1.1  2004/05/28 20:14:27  amag97
// ASC Integration
//

#ifndef _HWVAR_H
#define _HWVAR_H



#include <vector>
#include <string>
#include <cstdio>
#include <iostream>
#include <fstream>

using namespace std;

//#include "bitsize.h"
#include "bitsize_defs.h"

#include "advar.h"
#include "bsclass.h"
#include "varcollections.h"
#include "varnames.h"

//#include "hwvector.h"

//#include "node.h"
//#include "edge.h"
#include "dag.h"



class DAG;



//////////////////////////////////////

// ASC Macro Overides

#define HWabs( _VAR ) ( _VAR > 0 ) ? _VAR : _VAR * -1.;

//#include "hwint.h"

class Edge;

class Node;


class range{

public:
  range( double _min , double _max ):min(_min),max(_max){}
  
  double min;

  double max;


};

class R_ERR{
public:
  R_ERR( double _rerr ):rel_err( _rerr ){}

  double rel_err;


};

//*******************************************************************
//
// HWvar base class

class HWvar{

public:
  
  enum VAR_DTYPE { DECVAR , TMPVAR  , VECVAR};
  

  HWvar();

  virtual ~HWvar();

  HWvar(const HWvar &a );

  HWvar& operator= ( const HWvar &a );

 
  

  operator double();


  

  // Class Members 
  string var_name;
  //dag_index var_number;

  VAR_DTYPE var_dectype;
  advar ad_val;

  arch_attr var_type;
  //mutable vector<dag_index> src_nodes;
  //mutable vector<dag_index> dst_nodes;

  static varnames vnames;
  static varnames tmpvnames;

  //static DAG mydag;

  static dag_index var_count;

  Edge* myedge;

  static int read_inputs( istream &os );

  //static varcollections<HWvar *> varC;

  void add_varcollection( arch_attr var_type );
  
  
  //dag_index get_src_node();
  //dag_index get_dst_node();
  
  virtual string print_dot_bitinfo(){};


  static void reset_vars();


  double round_val( double val , int bits );


 
   
};


//**********************************************************************
// HWfix - Class for fixed point derived from HWvar

class  HWfix : public HWvar{

public:


 /*HWfix( );	 */
 //HWfix( char* name = NULL , advarType vtype = TMP );
  // HWfix( char* name = NULL , advarType vtype = AUTO_TMP , double vman_bw = 23 , double vexp_bw = 8 , double vouterr_spec = 0.0 );
  //
 HWfix();

 HWfix(arch_attr Signal_type, range var_range , fsize fractional = 8,sign_attr sign=TWOSCOMPLEMENT, string name=""); 

 HWfix(arch_attr Signal_type, fsize init_size = 16 , fsize fractional = 8,sign_attr sign=TWOSCOMPLEMENT, string name="");

 void init(arch_attr Signal_type, fsize init_size = 16 , fsize fractional = 8,sign_attr sign=TWOSCOMPLEMENT, string name=""); 


 HWfix ( const HWfix &a );

  
 ~HWfix();
 

  double get_val();
  HWfix& operator= ( const HWint &a);
  HWfix& operator= ( const HWfix &r);
  HWfix& operator= ( double val);

  //HWfix& operator= (  HWfix &r);
  void operator= (HWfix *a);
  HWfix& operator<<= (double val);

  //operator double();

  // Members

  fsize frac_bw;
  fsize int_bw;

  BWStateType frac_bw_state;
  BWStateType int_bw_state;

  sign_attr c_sign;
 
 void analyse_bw( int bw , int mode = 1);

 void print_advar();



  void read_inputdata( double );

 
  


  // Static members
 static void init(); 

 static void output_results();

 static void output_results2();

 static void output_analysis( int mode = 1 );

 static void output_analysis2( );  

  // Dot graph generation helper functions

 string print_dot_bitinfo();

 HWint operator[]( int index );
  

  // Global storage for vectors
  static vector<HWvector<HWfix>* > vec_c;
 
 static void fix_vecbitwidths();


 static void  setup_max_grads();

};


//HWfix HWabs( const HWfix& a ); 

HWfix operator+  (const HWfix &l, const HWfix &r);
HWfix operator+  (const double dval, const HWfix &r);
HWfix operator+  (const HWfix &l, const double dval);
HWfix operator+  (int val,const HWfix &r); 
HWfix operator+  (const HWfix &l, int val); 

HWfix operator-  (const HWfix &l, const HWfix &r);  
HWfix operator-  (const double dval,const HWfix &r); 
HWfix operator-  (const HWfix &l, const double dval);

HWfix operator*  (const HWfix &l, const HWfix &r); 
HWfix operator*  (const double dval,const HWfix &r);  
HWfix operator*  (const HWfix &l, const double dval); 

HWfix operator/  (const HWfix &l, const HWfix &r); 
HWfix operator/  (const double dval,const HWfix &r);  
HWfix operator/  (const HWfix &l, const double dval); 





//HWfix operator<< (HWfix &l, double &r );



  /*HWfix::HWfix (arch_attr tp, HWfix *m, HWfix *e ):

  }*/

  /*HWfix::HWfix (HWfix *m, HWfix *e ){

  }*/

  // Returns the truncated version of the float
  /*HWfix & HWfix::truncate() {
  }*/

  
//equal operator based on rounding mode
  
// HWfix & HWfix::operator= ( HWfix &r ) {
// }
//
// HWfix & HWfix::operator= ( HWint &r ) {
// }
//
 
//
// HWfix & HWfix::operator= (int val) { 
// }
//
//

//
//
//HWfix & operator-  (int val,HWfix &r)  { }
//HWfix & operator-  (HWfix &l, int val)  { }
//
//
//HWfix & operator*  (int val,HWfix &r)  { }
//HWfix & operator*  (HWfix &l, int val)  { }
//
//HWfix & operator/  (HWfix &l,HWfix &r)  { }
//HWfix & operator/  (HWfix &l,int val)    { /* error */ }
//HWfix & operator/  (HWfix &l,double dval)    { /* error */ }
//HWfix & operator/  (int val,HWfix &r)    { }
//HWfix & operator/  (double dval,HWfix &r)    { }
//
//HWfix & operator%  (HWfix &l,HWfix &r)  { }
//HWfix & operator%  (HWfix &l,int val) { }
//HWfix & operator%  (int val,HWfix &r) { }
//HWfix & operator%  (HWfix &l,double dval) { } 
//HWfix & operator%  (double dval,HWfix &r) { }
//
//HWfix & operator+= (HWfix &l, HWfix &r){ }
//HWfix & operator-= (HWfix &l, HWfix &r){ }
//HWfix & operator*= (HWfix &l, HWfix &r){ }
//HWfix & operator/= (HWfix &l, HWfix &r){ }
//HWfix & operator%= (HWfix &l, HWfix &r){ }
//
//HWfix & operator+= (HWfix &l,int val){ }
//HWfix & operator-= (HWfix &l,int val){ }
//HWfix & operator*= (HWfix &l,int val){ }
//HWfix & operator/= (HWfix &l,int val){ }
//HWfix & operator%= (HWfix &l,int val){ }
//
//HWfix & operator+= (HWfix &l,double val){ }
//HWfix & operator-= (HWfix &l,double val){ }
//HWfix & operator*= (HWfix &l,double val){ }
//HWfix & operator/= (HWfix &l,double val){ }
//HWfix & operator%= (HWfix &l,double val) { }
//
//HWfix & operator-  (HWfix &l){ }
//
// HWfix & operator>>= (HWfix &l,HWint &r){ }
// HWfix & operator>>= (HWfix &l,int val) { }
// HWfix & operator<<= (HWfix &l,HWint &r){ }
// HWfix & operator<<= (HWfix &l,int val) { }
//
HWfix operator>> (const HWfix &l,const HWint &r);
HWfix operator>> (const HWfix &l,int val); 
HWfix operator<< (const HWfix &l,const HWint &r);
HWfix operator<< (const HWfix &l,int val); 
//
////Control 
//HWint & operator== (HWfix &l,HWfix &r) { }
//HWint & operator== (double val,HWfix &r) { }
//HWint & operator== (HWfix &l,double val) { }
//HWint & operator== (int val,HWfix &r)    { }
//HWint & operator== (HWfix &l,int val)    { }
//
//HWint & operator!= (HWfix &l,HWfix &r) { }
//HWint & operator!= (double val,HWfix &r) { }
//HWint & operator!= (HWfix &l,double val) { }
//HWint & operator!= (int val,HWfix &r)    { }
//HWint & operator!= (HWfix &l,int val)    { }
//
HWint operator>  (const HWfix &l,const HWfix &r);
HWint operator>  (double val,const HWfix &r);
HWint operator>  (const HWfix &l,double val);
HWint operator>  (int val,const HWfix &r);
HWint operator>  (const HWfix &l,int val);

//
//HWint & operator<  (HWfix &l,HWfix &r) { }
//HWint & operator<  (double val,HWfix &r) { }
//HWint & operator<  (HWfix &l,double val) { }
//HWint & operator<  (int val,HWfix &r) { }
//HWint & operator<  (HWfix &l,int val) { }
//
//HWint & operator>=  (HWfix &l,HWfix &r) { }
//HWint & operator>=  (double val,HWfix &r) { }
//HWint & operator>=  (HWfix &l,double val) { }
//HWint & operator>=  (int val,HWfix &r) { }
//HWint & operator>=  (HWfix &l,int val) { }
//
//HWint & operator<=  (HWfix &l,HWfix &r) { }
//HWint & operator<=  (double val,HWfix &r) { }
//HWint & operator<=  (HWfix &l,double val) { }
//HWint & operator<=  (int val,HWfix &r) { }
//HWint & operator<=  (HWfix &l,int val) { }


HWint op_lod( const HWfix& var );

//*************************************************************************

class  HWfloat : public HWvar{

public:


 /*HWfloat( );	 */
 //HWfloat( char* name = NULL , advarType vtype = TMP );
  // HWfloat( char* name = NULL , advarType vtype = AUTO_TMP , double vman_bw = 23 , double vexp_bw = 8 , double vouterr_spec = 0.0 );
  //
 HWfloat();

 HWfloat(arch_attr Signal_type, fsize fullsize, fsize msize, sign_attr sign=SIGNMAGNITUDE, string name="");

 HWfloat(arch_attr Signal_type, fsize tsize, R_ERR _r_err, sign_attr sign=SIGNMAGNITUDE, string name="");


  void init(arch_attr Signal_type, fsize tsize, fsize msize, sign_attr sign, string name);
  

 HWfloat ( const HWfloat &a );	
 ~HWfloat();
 

 double get_val();

 HWfloat& operator= ( const HWfloat &r);
 HWfloat& operator= ( double val);

 HWfloat& operator<<= (double val);

 

  //advar ad_val;
 
 void analyse_bw( int bw , int mode = 1);

 void print_advar();

 

 void read_inputdata( double );

  // Static members
 
 static void init(); 

 static void output_results();

 static void output_analysis( int mode = 1 );

 
  string print_dot_bitinfo(){};

  //static int read_inputs( istream &os );


  fsize man_bw;
  fsize exp_bw;

  BWStateType man_bw_state;
  BWStateType exp_bw_state;

  sign_attr c_sign;


  double rel_err;


};

 
HWfloat operator+  (const HWfloat &l, const HWfloat &r);
HWfloat operator+  (const double dval,const HWfloat &r);
HWfloat operator+  (const HWfloat &l, const double dval);

HWfloat operator-  (const HWfloat &l, const HWfloat &r);  
HWfloat operator-  (const double dval,const HWfloat &r); 
HWfloat operator-  (const HWfloat &l, const double dval);

HWfloat operator*  (const HWfloat &l, const HWfloat &r); 
HWfloat operator*  (const double dval,const HWfloat &r);  
HWfloat operator*  (const HWfloat &l, const double dval); 

HWfloat operator/  (const HWfloat &l, const HWfloat &r); 
HWfloat operator/  (const double dval,const HWfloat &r);  
HWfloat operator/  (const HWfloat &l, const double dval); 



HWfloat operator<< (HWfloat &l, double &r );


class  HWlns : public HWvar{

public:


 /*HWlns( );	 */
 //HWlns( char* name = NULL , advarType vtype = TMP );
  // HWlns( char* name = NULL , advarType vtype = AUTO_TMP , double vman_bw = 23 , double vexp_bw = 8 , double vouterr_spec = 0.0 );
  //
 HWlns();

 HWlns(arch_attr Signal_type, fsize fullsize, fsize msize, sign_attr sign=SIGNMAGNITUDE, string name="");

 HWlns(arch_attr Signal_type, fsize tsize, R_ERR _r_err, sign_attr sign=SIGNMAGNITUDE, string name="");


  void init(arch_attr Signal_type, fsize tsize, fsize msize, sign_attr sign, string name);
  

 HWlns ( const HWlns &a );	
 ~HWlns();
 

 double get_val();

 HWlns& operator= ( const HWlns &r);
 HWlns& operator= ( double val);

 HWlns& operator<<= (double val);

 

  //advar ad_val;
 
 void analyse_bw( int bw , int mode = 1);

 void print_advar();

 

 void read_inputdata( double );

  // Static members
 
 static void init(); 

 static void output_results();

 static void output_analysis( int mode = 1 );

 
  string print_dot_bitinfo(){};

  //static int read_inputs( istream &os );


  int int_bw;
  int frac_bw;

  BWStateType int_bw_state;
  BWStateType frac_bw_state;

  sign_attr c_sign;


  double rel_err;


};

 
HWlns operator+  (const HWlns &l, const HWlns &r);
HWlns operator+  (const double dval,const HWlns &r);
HWlns operator+  (const HWlns &l, const double dval);

HWlns operator-  (const HWlns &l, const HWlns &r);  
HWlns operator-  (const double dval,const HWlns &r); 
HWlns operator-  (const HWlns &l, const double dval);

HWlns operator*  (const HWlns &l, const HWlns &r); 
HWlns operator*  (const double dval,const HWlns &r);  
HWlns operator*  (const HWlns &l, const double dval); 

HWlns operator/  (const HWlns &l, const HWlns &r); 
HWlns operator/  (const double dval,const HWlns &r);  
HWlns operator/  (const HWlns &l, const double dval); 



HWlns operator<< (HWlns &l, double &r );



#endif
