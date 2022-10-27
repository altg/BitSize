// $Author: amag97 $
// $Id: hwint.h,v 1.4 2004/07/12 20:08:11 amag97 Exp $

#ifndef _HWINT_H
#define _HWINT_H


#include "hwvar.h"
#include "bitsize_defs.h"



class HWvar;

class HWint {

public:
  
  HWint(arch_attr Signal_type = TMP, fsize init_size = 32 , sign_attr sign=TWOSCOMPLEMENT, char *name=NULL);

  HWint& operator= ( int int_val );

  HWint& operator= ( const HWint& );

  HWint& operator= ( const HWfix& );

  int var_val;
  
  string var_name;
  
  arch_attr var_signaltype;

  fsize var_size;

  sign_attr var_sign;

  operator int();

  HWint& operator+= (const HWint &r);
  HWint& operator-= (const HWint &r);
  HWint& operator*= (const HWint &r);
  HWint& operator/= (const HWint &r);
  HWint& operator%= (const HWint &r);
  HWint& operator|= (const HWint &r);
  HWint& operator&= (const HWint &r);
  HWint& operator^= (const HWint &r);

  HWint& operator+= (int val);
  HWint& operator-= (int val);
  HWint& operator*= (int val);
  HWint& operator/= (int val);
  HWint& operator%= (int val);
  HWint& operator|= (int val);
  HWint& operator&= (int val);
  HWint& operator^= (int val);

  HWint& operator++ ();
  //HWint& operator++ (HWint &l,int);
  HWint& operator-- ();
  //HWint& operator-- (HWint &l,int);

  HWint operator[]( int index );
  HWint operator[]( const HWint& index );

};

 
  
 //  HWint & operator>>= (HWint &l,HWint &r);
//   HWint & operator>>= (HWint &l,int val);
//   HWint & operator<<= (HWint &l,HWint &r);
//   HWint & operator<<= (HWint &l,int val);

 

//   HWint & operator,  (HWint &l,HWint &r);
//   HWint & operator-  (HWint &l);
//   HWint & operator!  (HWint &l);
  
 //   HWint & operator== (HWint &l,HWint &r);
//    HWint & operator== (int val,HWint &r);
//    HWint & operator== (HWint &l,int val);

//    HWint & operator!= (HWint &l,HWint &r);
//    HWint & operator!= (int val,HWint &r);
//    HWint & operator!= (HWint &l,int val);

//    HWint & operator<= (HWint &l,HWint &r);
//    HWint & operator<= (int val,HWint &r);
//    HWint & operator<= (HWint &l,int val);

//    HWint & operator>= (HWint &l,HWint &r);
//    HWint & operator>= (int val,HWint &r);
//    HWint & operator>= (HWint &l,int val);

//    HWint & operator||  (HWint &l,HWint &r);
//    HWint & operator||  (int val, HWint &r);
//    HWint & operator||  (HWint &l, int val);

//    HWint & operator&&  (HWint &l,HWint &r);
//    HWint & operator&&  (int val,HWint &r);
//    HWint & operator&&  (HWint &l,int val);

   HWint operator>  (HWint &l,HWint &r);
   HWint operator>  (int val,HWint &r);
   HWint operator>  (HWint &l,int val);

   HWint operator<  (HWint &l,HWint &r);
   HWint operator<  (int val,HWint &r);
   HWint operator<  (HWint &l,int val);
   
//    HWint & operator|  (HWint &l,HWint &r);
//    HWint & operator|  (int val, HWint &r);
//    HWint & operator|  (HWint &l, int val);

//    HWint & operator^  (HWint &l,HWint &r);
//    HWint & operator^  (int val,HWint &r);
//    HWint & operator^  (HWint &l,int val);
   
//    HWint & operator&  (HWint &l,HWint &r);
//    HWint & operator&  (int val,HWint &r);
//    HWint & operator&  (HWint &l,int val);
   
   HWint operator>> (const HWint &l,const HWint &r);
   HWint operator>> (const HWint &l,int val);
   HWint operator>> (int val,const HWint &r);

   HWint operator<< (const HWint &l,const HWint &r);
   HWint operator<< (const HWint &l,int val);
   HWint operator<< (int val,const HWint &r);
   

   HWint operator+  (const HWint &l,const HWint &r);
   HWint operator+  (const HWint &l,int val);
   HWint operator+  (int val,const HWint &r);

   HWint operator-  (const HWint &l,const HWint &r);
   HWint operator-  (const HWint &l,int val);
   HWint operator-  (int val,HWint &r);

   HWint operator*  (const HWint &l,const HWint &r);
   HWint operator*  (const HWint &l,int val);
   HWint operator*  (int val,HWint &r);

   HWint operator/  (const HWint &l,const HWint &r);
   HWint operator/  (const HWint &l,int val);
   HWint operator/  (int val,const HWint &r);

   HWint operator%  (const HWint &l,const HWint &r);
   HWint operator%  (const HWint &l,int val);
   HWint operator%  (int val,const HWint &r);


   HWint op_encoder( const HWint& var );
#endif



