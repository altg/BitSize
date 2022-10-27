// ADVAR library implementation

// $Id: advar.cpp,v 1.44 2004/10/26 18:32:59 amag97 Exp $
// $Author: amag97 $
// $Log: advar.cpp,v $
// Revision 1.44  2004/10/26 18:32:59  amag97
// Fixed most bugs with fixed-point frac. opt 6
//
// Revision 1.43  2004/10/05 19:23:53  amag97
// Fixes to AD code - bug when multiple inputs are used
// Updated Graph info
//
// Revision 1.42  2004/06/24 18:45:30  amag97
// Added a seperate class to encapsulate the bitwidth optimisation functions
//
// Revision 1.41  2004/06/10 19:57:23  amag97
// Added leading one detect
//
// Revision 1.40  2004/06/09 21:20:51  amag97
// Added more bitwidth analysis methods for fixed-point
//
// Revision 1.39  2004/06/08 21:07:40  amag97
// More bugfixes to - advar
//
// Revision 1.38  2004/06/05 17:34:18  amag97
// More bug fixes to ASC integration
//
// Revision 1.37  2004/06/03 22:01:30  amag97
// More bug fixes
// Added HWint
//
// Revision 1.36  2004/06/02 19:37:59  amag97
// Added HWvector
//
// Revision 1.35  2004/06/01 11:31:55  amag97
// DAG code
//
// Revision 1.34  2004/05/28 20:14:27  amag97
// ASC Integration
//
// Revision 1.33  2004/05/14 17:33:20  amag97
// Intgeration with ASC system
//
// Revision 1.32  2004/04/22 13:24:57  amag97
// Pre-ASC
//
// Revision 1.31  2004/04/06 17:42:06  amag97
// Changes to Background
//
// Revision 1.30  2004/02/27 18:27:02  amag97
// Version without MpFloat
//
// Revision 1.29  2004/02/27 11:39:23  amag97
// Final version before MpFloat removal
//
// Revision 1.28  2004/02/25 18:00:23  amag97
// Fixes to bit-width calculation code
//
// Revision 1.27  2004/02/24 11:58:36  amag97
// ad_bitsize: Imrovements to bit-width determination functions
// others: many changes and updates
//
// Revision 1.26  2003/12/23 18:30:23  amag97
// More changes to bitwidth methods
//
// Revision 1.25  2003/12/11 20:39:44  amag97
// Added bit width analysis method 4
//
// Revision 1.24  2003/12/11 11:16:24  amag97
// Added new bitwidth analysis methods
//
// Revision 1.23  2003/11/28 15:28:15  amag97
// Latest changes
//
// Revision 1.22  2003/09/13 21:38:26  amag97
// Added bitwidth info access functions
//
// Revision 1.21  2003/09/09 19:16:45  amag97
// More functions for bitwidth analysis and output
//
// Revision 1.20  2003/08/22 17:10:24  amag97
// Improvements to bitwidth calculation
//

#include "advar.h"

#include "bsclass.h"

#ifdef _DEBUG
#define _DEBUGOUT
#endif


#ifdef _DEBUGOUT
#define SETDB double DBmp_dbl; 
#define MPDB( _VAL ) DBmp_dbl = static_cast<double>( _VAL );
#else
#define SETDB
#define MPDB( _VAL )
#endif


#ifdef __GNUC__
#define _logb ilogb
#endif

//#define _logb ilogb

//unsigned int advar::var_count = 0;

size_t advar::var_count = 0;

bool advar::auto_numbering = true;

//BSClass::advar_veclist advar::advar_vec;

// Named vars
list<advar *> advar::named_vars;

BSClass advar::BS;

// Utility functions


#ifdef _MSC_VER
double advar::log2( double in_val)
{

	return ( in_val != 0 ?  log( fabs( in_val ) ) / log( 2. ) : 0. ); 

	
}

#endif

double advar::get_exp_value( double dbval )
{
  return ( dbval != 0. ? _logb( fabs(dbval) ) : 0. );
       
}



void advar::advar_setup( unsigned int control_W , size_t Default_Prec ) 
{

  auto_numbering = ( control_W & AD_AUTONUM ) ? true : false;


}


void advar::initvar( size_t var_num)
{

  if ( auto_numbering )
    var_number = var_num;
  else
    var_number = 9999; 

  //ref_maxval = -DBL_MAX;
  //ref_minval = DBL_MAX;


  ref_maxval = FLT_MIN;
  ref_minval = FLT_MAX;

  m_bitwidth = 0;
  fp_frac_bw = 0;
  fp_int_bw = 0;
  
  indep_var = false;

  depend_var = false;

  var_name = "NO_NAME";

  // advar_vec.push_back( this );

  // advar_vec_offset = advar_vec.size() - 1;

  BWState = NOTSET;

}


advar::advar( advarType vtype )
{
 
  ref_value = 0.0;

  initvar();
  
  BS.advar_vec.push_back( this );


  
  
  advar_vec_offset = BS.advar_vec.size() - 1;

   varType = vtype;

  BS.add_to_list( this );
}



advar::advar( string vname , advarType vtype  )
{


  ref_value = 0.0;

  initvar();

  
  named_vars.push_back( this );

  varType = vtype;

  var_name = vname;

  
}

advar::advar( const DTYPE fltValue , size_t var_num){
  
  ref_value = fltValue;

 
  initvar( var_num );

 
  MAX_VAL( fltValue );
  MIN_VAL( fltValue );
  

}


advar::advar( string vname , size_t m_size , size_t e_size , size_t var_num )
{

  //  ref_value.SetPrec( m_size );
  ref_value = 0.0;

  initvar( var_num );

  m_bitwidth = m_size;
  e_bitwidth = e_size;

//  BS.fixed_varlist.push_back( this );

}


// Copy constructor
advar::advar( const advar& a )
{

  initvar( a.var_number );

  //initvar( var_count++ );

  ref_value = a.ref_value;

  MAX_VAL( a.ref_maxval );
  MIN_VAL( a.ref_minval );

  grads.assign( a.grads.begin() , a.grads.end() );
  

}


void advar::copy( const advar& a )
{

  //initvar();

  ref_value = a.ref_value;

  MAX_VAL( a.ref_maxval );
  MIN_VAL( a.ref_minval );

  grads.assign( a.grads.begin() , a.grads.end() );


}

advar::~advar()
{
  //advar_vec.erase( advar_vec.begin() + advar_vec_offset );
  
}

void advar::set_var_name( string name ){
  var_name = name;
  named_vars.push_back( this );
}


 
advar& advar::operator = ( const DTYPE refValue_in )
{

  ref_value = refValue_in;


  MAX_VAL( refValue_in );
  MIN_VAL( refValue_in );

  
  if ( fix_int_state == NOTSET )
    fp_int_bw = ceil(log2( abs( refValue_in ) )  );  
  //fix_int_state = SET;

  //_frac_state = NOTSET;

  return *this;

}



 
advar& advar::operator = ( const advar& a )
{

  if (this == &a) return *this;  // Taking care of self assignment

  ref_value = a.ref_value;

  MAX_VAL( a.ref_value );
  MIN_VAL( a.ref_value );

  vector<DTYPE>::iterator iter_a, iter;

  //if ( a.grads.size() == 0 ) return *this; // a has an empty grad vector
  //if ( ( varType == AUTO_TMP ) ||  ( grads.size() < a.grads.size() ))
//  if (  grads.size() < a.grads.size() )
//     {
//       grads.assign( a.grads.begin() , a.grads.end() );

//     }
//   else
//     {

//       for( iter_a = a.grads.begin(), iter = grads.begin(); iter_a != a.grads.end() ; iter_a++ , iter++ )
// 	{ 
// 	  (*iter) = (*iter_a);

// 	}

//      }


   for( iter_a = a.grads.begin(), iter = grads.begin(); ( iter != grads.end() &&  iter_a != a.grads.end()); iter_a++ , iter++ )
    {


      (*iter) = (*iter_a);

    }



  if ( var_number >= grads.size() )
    {
      grads.push_back( 1. );
    }
  else
    {
      grads[var_number] = ( grads[var_number] == 0 ) ? 1. : grads[var_number];
    }
  
  //fix_frac_state = a.fix_frac_state;
  // fix_int_state = a.fix_int_state;


  return *this;


}

advar::operator DTYPE()
{

  return (DTYPE)ref_value;

}


// Operator for marking independent variables

void advar::set_indep()
{
  if ( var_number != 9999 )
    {
      	  grads.resize( var_number + 1, 0. );
	  grads.push_back( 1. );
     
    }

  indep_var = true;
}



advar& advar::operator <<= ( const DTYPE param_val )
{

  ref_value = param_val;

  MAX_VAL( param_val );
  MIN_VAL( param_val );


  if ( var_number != 9999 )
    {
      	  grads.resize( var_number , 0. );
	  grads.push_back( 1. );
      
    }

   indep_var = true;

  return *this;

}

// Operator for marking independent variables plus assignment

advar& advar::operator <<= ( const advar& a )
{

  ref_value = a.ref_value;

  MAX_VAL( a.ref_value );
  MIN_VAL( a.ref_value );

  // Resize assign grads etc.

  grads.assign( a.grads.begin() , a.grads.end() );

  if ( a.grads.size() < var_number+1  ) // param_val has empty array
    {
       grads.resize( var_number , 0. );
       grads.push_back( 1. );
       return *this;
    }

  //grads.assign( a.grads.begin() , a.grads.end() );

  grads[ var_number ] = 1.;

  indep_var = true;
  

  return *this;
}


void advar::reset_grad_vec()
{
  iter_t itr = grads.begin();

  while( itr != grads.end() )
    (*itr++) = 0.;

  grads[ var_number ] = 1.;


}


void advar::set_depend()
{

  depend_var = true;
  
  /*if ( max_grads.size() == 0 )
    max_grads.resize( grads.size() ,  -DBL_MAX );*/

  if ( max_grads.size() == 0 )
    max_grads.resize( grads.size() ,  -FLT_MAX );

  // Store the maximum values for the gardients
  iter_t mg_iter, g_iter;

  for ( mg_iter = max_grads.begin() , g_iter = grads.begin() ; mg_iter != max_grads.end(); 
	  mg_iter++ , g_iter++ )
    {

      *mg_iter = max( *mg_iter , *g_iter );
    }             
	  

}

DTYPE advar::operator[]( size_t wrt_no )
{

  if ( wrt_no > grads.size() )
    return 0.;
  else 
    return grads[ wrt_no ];
}



DTYPE advar::operator[]( const advar& wrt_ad )
{
  
  DTYPE temp = this->operator[]( wrt_ad.var_number );

  return ( temp );

}


// '+' Operator

advar& advar::operator += ( const advar& pr )
{

  ref_value += pr.ref_value;


  MAX_VAL( ref_value );
  MIN_VAL( ref_value );


  return *this;

}

const advar operator + ( const advar& adflt_lhs , const DTYPE fltValue_rhs )
{
  
  advar temp( fltValue_rhs );

  return ( adflt_lhs + temp );

};

const advar operator + ( const DTYPE fltValue_lhs , const advar& adflt_rhs)
{

  advar temp( fltValue_lhs );


  return ( temp + adflt_rhs );

};

const advar operator + ( const advar& adflt_lhs , const advar& adflt_rhs)
{

  advar temp( adflt_lhs );
  
  

  temp += adflt_rhs;



   if ( temp.grads.size() < adflt_rhs.grads.size() )
    {
      temp.grads.resize( adflt_rhs.grads.size() , 0. );
    }


   for ( advar::iter_t itr = temp.grads.begin() , rhs_itr =  adflt_rhs.grads.begin() ;
	       ( itr != temp.grads.end()) && ( rhs_itr != adflt_rhs.grads.end() ); itr++ , rhs_itr++ )
     {
       *itr += *rhs_itr;
     }

  return temp;

};


// '-' Operator

advar& advar::operator -= ( const advar& pr )
{
 
  ref_value -= pr.ref_value;

  MAX_VAL( ref_value );
  MIN_VAL( ref_value );

  return *this;


}

const advar operator - ( const advar& adflt_lhs , const DTYPE fltValue_rhs )
{
  advar temp( fltValue_rhs );

  return ( adflt_lhs - temp); 

};




const advar operator - ( const DTYPE fltValue_lhs , const advar& adflt_rhs )
{
  advar temp( fltValue_lhs );

  return ( temp - adflt_rhs );

};

const advar operator - ( const advar& adflt_lhs , const advar& adflt_rhs)
{

  advar temp( adflt_lhs );

  temp -= adflt_rhs;


 if ( temp.grads.size() < adflt_rhs.grads.size() )
    {
      temp.grads.resize( adflt_rhs.grads.size() , 0. );
    }


   for ( advar::iter_t itr = temp.grads.begin() , rhs_itr =  adflt_rhs.grads.begin() ;
	 ( itr != temp.grads.end() ) && ( rhs_itr != adflt_rhs.grads.end()); itr++ , rhs_itr++ )
     {
       *itr -= *rhs_itr;
     }

  return temp;

};


// '*' Operator

advar& advar::operator *= ( const advar& pr )
{
 
  ref_value *= pr.ref_value;
  
  MAX_VAL( ref_value );
  MIN_VAL( ref_value );
  
  return *this;


}

	
const advar operator * ( const advar& adflt_lhs , const DTYPE fltValue_rhs )
{

  advar temp( fltValue_rhs , 0 );


  return ( adflt_lhs * temp );


};

const advar operator * ( const DTYPE fltValue_lhs , const advar& adflt_rhs )
{
  advar temp( fltValue_lhs , 0 );

  return  (temp * adflt_rhs);


};


const advar operator * ( const  advar& adflt_lhs ,const advar& adflt_rhs)
{

  advar temp( adflt_lhs );

  temp *= adflt_rhs;



  if ( temp.grads.size() < adflt_rhs.grads.size() )
    {
      temp.grads.resize( adflt_rhs.grads.size() , 0. );
    }
  
  double rhs_grad;
  
  for ( advar::iter_t itr = temp.grads.begin() , rhs_itr = adflt_rhs.grads.begin() ; 
	      itr != temp.grads.end(); itr++ )
    {

      rhs_grad = ( rhs_itr != adflt_rhs.grads.end() ) ? *(rhs_itr++) : 0.;

      *itr = (*itr) * adflt_rhs.ref_value + (rhs_grad) * adflt_lhs.ref_value;
    }


 //  adflt_lhs.ref_maxval = temp.ref_maxval;
//   adflt_lhs.ref_minval = temp.ref_minval;

//   adflt_rhs.ref_maxval = temp.ref_maxval;
//   adflt_rhs.ref_minval = temp.ref_minval;


  return temp;

}


// '/' Operator 

advar& advar::operator /= ( const advar& pr )
{
 
  ref_value /= pr.ref_value;
  
  MAX_VAL( ref_value );
  MIN_VAL( ref_value );
  
  return *this;


}

	
const advar operator / ( const advar& adflt_lhs , const DTYPE fltValue_rhs )
{

  advar temp( fltValue_rhs );


  return ( adflt_lhs / temp );


};

const advar operator / ( const DTYPE fltValue_lhs , const advar& adflt_rhs )
{
  advar temp( fltValue_lhs );


  return  (temp / adflt_rhs);


};


const advar operator / ( const advar& adflt_lhs , const advar& adflt_rhs)
{

  advar temp( adflt_lhs );

  temp /= adflt_rhs;



  if ( temp.grads.size() < adflt_rhs.grads.size() )
    {
      temp.grads.resize( adflt_rhs.grads.size() , 0. );
    }
  
  double rhs_grad;
  
  for ( advar::iter_t itr = temp.grads.begin() , rhs_itr = adflt_rhs.grads.begin() ; 
	      itr != temp.grads.end(); itr++ )
    {

      rhs_grad = ( rhs_itr != adflt_rhs.grads.end() ) ? *(rhs_itr++) : 0.;

      //*itr = (*itr) * adflt_rhs.ref_value + (rhs_grad) * adflt_lhs.ref_value;

      *itr = (( *itr ) - temp.ref_value * rhs_grad ) / adflt_rhs.ref_value;

    }


 

  return temp;

}


const advar pow( const advar& advar_val , signed long n )
{


  advar temp( (DTYPE)pow( advar_val.ref_value , static_cast<DTYPE>(n) ));
  double dtemp = n * pow ( advar_val.ref_value ,static_cast<DTYPE> (n-1) );


  //temp.ref_value = pow( temp.ref_value , n );

  for ( advar::iter_t itr = advar_val.grads.begin() ; itr != advar_val.grads.end() ; itr ++ )
    {
      //*itr *= dtemp;
      temp.grads.push_back( *itr * dtemp );

    }

  return temp;

}

const advar exp( const advar& advar_val )
{

  advar temp( (DTYPE)exp( advar_val.ref_value ) );

  for ( advar::iter_t itr = advar_val.grads.begin() ; itr != advar_val.grads.end() ; itr ++ )
    {
      temp.grads.push_back( *itr * temp.ref_value );
    }

  return temp;

}

// Natural logarithm
const advar log( const advar& advar_val )
{

  advar temp( (DTYPE)log( advar_val.ref_value ) );

  for ( advar::iter_t itr = advar_val.grads.begin() ; itr != advar_val.grads.end() ; itr ++ )
    {

      temp.grads.push_back( *itr / temp.ref_value );
    }

  return temp;

}

// 'sin'
const advar sin( const advar& advar_val )
{

  advar temp( (DTYPE)sin( advar_val.ref_value ) );

  double tempd = cos( advar_val.ref_value );

  //MpFloat tempd = cos( advar_val.ref_value );

  for ( advar::iter_t itr = advar_val.grads.begin() ; itr != advar_val.grads.end() ; itr ++ )
    {

      temp.grads.push_back( *itr * tempd );
    }

  return temp;

}

// 'cos'
const advar cos( const advar& advar_val )
{

  advar temp( (DTYPE)cos( advar_val.ref_value ) );

  double tempd = -sin( advar_val.ref_value );
  //MpFloat tempd = -1. * sin( advar_val.ref_value );

  for ( advar::iter_t itr = advar_val.grads.begin() ; itr != advar_val.grads.end() ; itr ++ )
    {

      temp.grads.push_back( *itr * tempd );
    }

  return temp;


}


// Bitwidth analysis functions

string advar::get_var_name()
{
  return var_name;
}


DTYPE advar::get_m_bw()
{
  return m_bitwidth;
}

DTYPE advar::get_e_bw()
{
  return e_bitwidth;
}


DTYPE advar::get_i_bw()
{
  return fp_int_bw;
}

DTYPE advar::get_f_bw()
{
  return fp_frac_bw;
}

void advar::analyse_flt_e_bw()
{
  DTYPE diff , log2_val , tt;
  
  DTYPE shift_bits;

  
   BSClass::advar_veclist::iterator u_iter;

   for ( u_iter = BS.advar_vec.begin() ; u_iter != BS.advar_vec.end() ; u_iter++ )
    {

               
	  log2_val = ((*u_iter)->ref_maxval != 0. ? _logb( (*u_iter)->ref_maxval ) : 0. ) 
			- (((*u_iter)->ref_minval ) != 0. ? _logb( (*u_iter)->ref_minval ) : 0. );

          	  

	  if ( log2_val != 0 )
	    shift_bits = log2_val < 0 ? log2( ABS_AD( log2_val ) + 1 ) + 1 : log2( log2_val ) + 1;
	  else
	    shift_bits = 5.;
	  
	  
	  
          tt = ceil(shift_bits); 
    
          (*u_iter)->e_bitwidth = static_cast<double>( tt );
      
    }


}




// Output advar information to stream
ostream& operator << ( ostream& os , advar& pr )
{


  os << "Var Number : " << (unsigned int)pr.var_number << "\t Var Type : "  << advar::BS.advarType_name[ pr.varType ] <<endl;
  os << setw( 10 ) << "Value :" << setw( 10 ) << "MAX :" << setw(10) << "MIN " << endl;
  os.width(10);
  os <<  setw( 20 ) << (double)pr.ref_value << setw( 20 ) << (double) pr.ref_maxval << setw(20) << (double) pr.ref_minval << endl;
#ifdef DB_INFO
 os << "Grad Vector Size :" << pr.grads.size() << endl;
#endif

 advar::iter_t g_iter, g_end;

//  if ( pr.depend_var )
//    {
//      g_iter = pr.max_grads.begin();
//      g_end = pr.max_grads.end();
//     }
//  else
//    {
     g_iter = pr.grads.begin();
     g_end = pr.grads.end();
//    }


 for (size_t i = 0; g_iter != g_end ; g_iter++ , i++)
   { 
     os << (unsigned int)i << " : " << *g_iter << endl;
   }


 //  for ( size_t i = 0 ; i < pr.grads.size() ; i ++ )
//      {

//        //os << pr.grads[i].node_no << " : " << pr.grads[i].grad_val << endl;
//        os << i << " : " << pr.grads[i] << endl;
//      }

//   // copy( pr.grads.begin() , pr.grads.end() , os );

  os << endl;
  return os;


}

istream& operator >> ( istream& is , advar& pr )
{

  double temp;

  is >> temp;

  pr = temp;

  return is;

}




void advar::print_mbw(ostream &os)
{
  BSClass::advar_veclist::iterator u_iter;
  iter_t v_iter;
  int ctr = 0;
 
  //DTYPE log2_val , log2_val2 ,  tt , diff1;
  
  size_t no_vars = grads.size();
 
  analyse_flt_e_bw();
  
  //cout << "Floating point results" << endl;

  //cout << "Var  :" << ": M :" << ": E :" << endl;

  

  for ( ctr = 0 , u_iter = BS.advar_vec.begin() ; u_iter != BS.advar_vec.end(); u_iter++ ) 
    {

      //cout <<  (*u_iter)->ref_value << " : ";

      //cout << *(v_iter) << endl;
      //cout << endl << "u_m[" << ctr++ << "] = " << (*u_iter)->m_bitwidth << " : ";

      os << endl << "const int " << (*u_iter)->var_name << "_mbw = " << (*u_iter)->m_bitwidth << ";" << endl; 

      //cout <<  (*u_iter)->e_bitwidth << endl;
    }

}

void advar::print_table_flt()
{

  //advar *dep_var = advar_vec[ dep ];

  //cout << "u : " << " v" << endl;

  BSClass::advar_veclist::iterator u_iter;
  iter_t v_iter;
  int ctr = 0;
 
  //DTYPE log2_val , log2_val2 ,  tt , diff1;
  
  size_t no_vars = grads.size();
 
  //analyse_flt_e_bw();
  
  cout << "Floating point results" << endl;

  cout << "Var  :" << ": M :" << ": E :" << endl;

  for ( ctr = 0 , u_iter = BS.advar_vec.begin() ; u_iter != BS.advar_vec.end(); u_iter++ ) 
    {

      //cout <<  (*u_iter)->ref_value << " : ";

      //cout << *(v_iter) << endl;
      //cout << endl << "u_m[" << ctr++ << "] = " << (*u_iter)->m_bitwidth << " : ";

      cout << endl << (*u_iter)->var_name << " " << (*u_iter)->m_bitwidth << " : "; 

      cout <<  (*u_iter)->e_bitwidth << endl;
    }



}


void advar::print_VHDL_flt_bw()
{

  BSClass::advar_veclist::iterator u_iter;
   for ( u_iter = BS.advar_vec.begin() ; u_iter != BS.advar_vec.end(); u_iter++ ) 
    {

      cout << endl << "constant " << (*u_iter)->var_name << "_m_bw : integer :=" << (*u_iter)->m_bitwidth << ";" << endl;
      cout << "constant " << (*u_iter)->var_name << "_e_bw : integer :=" << (*u_iter)->e_bitwidth << ";" << endl;
      
      // cout <<  (*u_iter)->e_bitwidth << endl;
    }


}

void advar::output_bitspec( ostream &os )
{
  BSClass::advar_veclist::iterator u_iter;
  iter_t v_iter;
  int ctr = 0;
 
 

  
  os << "define( `_bitdata', _bitspec(" << endl;
  for ( u_iter = BS.advar_vec.begin() ; u_iter != BS.advar_vec.end() - 1; u_iter++ ) 
    {
      
      if ( (*u_iter)->indep_var == true ){
       os << endl << "u_m[" << ctr++ << "]= " << (*u_iter)->m_bitwidth << ";" << endl;
      }
     
    }
  os << "))" << endl;

  os << "define( `nvars'," << ctr << ")" << endl;

}


void advar::output_bitspec2( const string &fname )
{
  BSClass::advar_veclist::iterator u_iter;
  iter_t v_iter;
  int ctr = 0;

  fstream os( fname.c_str() , ios::out );

  os << "/// bitdata.h "  << endl;
  os << "/// u_m , u_m_size" << endl;

  int num =  (int)BS.advar_vec.size();
  os << "const int u_m_size = " << num << ";" << endl;
  
  os << "const size_t u_m[ " << num << " ] = { ";
  for ( u_iter = BS.advar_vec.begin() ; u_iter != BS.advar_vec.end() - 1; u_iter++ ) 
    {
      os << (*u_iter)->m_bitwidth << "," << endl;
    }
  
  os << "};" << endl;

  os.close();

}


void advar::print_table_fx( )
{



  BSClass::advar_veclist::iterator u_iter;
  iter_t v_iter;
  size_t ctr = 0;
 
  

  size_t no_vars = grads.size();
  
 

  cout << "Fixed point results" << endl;

  cout << "Var  :" << ": Int :" << ": Frac :" << ": Tot :" << endl;	

  for ( u_iter = BS.advar_vec.begin() ; u_iter != BS.advar_vec.end(); u_iter++ ) 
    {
      //        cout << endl << "u_m[" << ctr++ << "]= " << (*u_iter)->fp_int_bw +  (*u_iter)->fp_frac_bw << ";" << endl;
      //cout << (*u_iter)->fp_int_bw << " : " <<   (*u_iter)->fp_frac_bw << " : " << (*u_iter)->fp_int_bw +  (*u_iter)->fp_frac_bw << endl;
      cout << endl << (*u_iter)->var_name << " = " << (*u_iter)->fp_int_bw << " : ";
      cout << (*u_iter)->fp_frac_bw << " : " << (*u_iter)->fp_int_bw +  (*u_iter)->fp_frac_bw << endl;
    }


}


// Bitwidth Analysis methods

void advar::analyse_bw_flt( double perr )
{
#define DTYPE_2 DTYPE

  DTYPE_2 output_abs_error;

  output_abs_error = (DTYPE_2)( perr * ref_value );

  size_t no_of_vars = grads.size();
  
  iter_t g_itr = grads.begin();

  DTYPE_2 delta_x , x_exp , log2_val;

  
  output_abs_error = ( output_abs_error == 0.0 ? perr * DBL_EPSILON : output_abs_error );

  for ( size_t ctr = 0 ;  g_itr != grads.end() ; g_itr++ , ctr++ )
    {
      
      delta_x = ( output_abs_error / ( static_cast<DTYPE_2>(no_of_vars) * (*g_itr) ));
     
	  cout << (*g_itr) << " : ";

	  x_exp = ( BS.advar_vec[ ctr ]->ref_maxval != 0. ? _logb( BS.advar_vec[ctr]->ref_maxval ) : 0. );

	 
      //log2_val = ( delta_x != 0. ? log( ABS_AD( delta_x ) / log( 2. ) ) : 0. );	
      log2_val = log2( delta_x );


      BS.advar_vec[ctr]->m_bitwidth  = MAX( BS.advar_vec[ctr]->m_bitwidth  , ceil(x_exp - log2_val + 1.));
      
    }

	cout << endl;
  analyse_flt_e_bw();
}


void advar::analyse_bw_ulp_flt( double ulp_err )
{


  DTYPE output_abs_error;

  output_abs_error = ulp_err; 

  size_t no_of_vars = 0;
  
  iter_t g_itr = grads.begin();

  DTYPE delta_x , x_exp , log2_val;

   
  while(  g_itr != grads.end() )
    if( *g_itr++ != 0 ) no_of_vars++;
 
  g_itr = grads.begin();

  //cerr << var_name << " vars: " <<  no_of_vars << endl;

  for ( size_t ctr = 0 ;  g_itr != grads.end() ; g_itr++ , ctr++ )
    {
      //      cerr << BS.advar_vec[ctr]->var_name << " : ";

      if ( ( BS.advar_vec[ctr]->BWState != SET ) && ( *g_itr != 0. ) ) 
	{ 
	    
	       delta_x =   ( output_abs_error / ( static_cast<double>(no_of_vars) * (*g_itr)) );
     
	       x_exp = get_exp_value( BS.advar_vec[ ctr ]->ref_value );	

               log2_val = log2( abs(delta_x) );
	  
	       //     cerr << BS.advar_vec[ctr]->var_name << " : " << delta_x << " : " << (*g_itr) << " : " << x_exp << " : " << log2_val << endl;

               BS.advar_vec[ctr]->m_bitwidth  = MAX( BS.advar_vec[ctr]->m_bitwidth  , abs(ceil(x_exp - log2_val + 1.)));
	}
      else
	{
	  //	  if ( BS.advar_vec[ctr]->BWState != SET ) cerr << ( *g_itr ) << endl;
	}
    }

	analyse_flt_e_bw();
}

void advar::analyse_bw_flt_mbw( int method )
{

  analyse_bw_flt_mbw( static_cast<int>( m_bitwidth ) , method );

}


void advar::analyse_bw_flt_mbw( int m_bw , int method )
{

	double m_err = pow( 2.  , -m_bw );

	double exp_value = get_exp_value( ref_value );

        double out_err = m_err * pow( 2. , exp_value );

	m_bitwidth = m_bw;

	cout << "Output Error :" << out_err << " : " << out_err/ref_value << endl;

	switch( method )
	{
		case 1: analyse_bw_ulp_flt( out_err ); break;

		case 2: analyse2_bw_ulp_flt( out_err , m_bw ); break;
	
		case 3: analyse3_bw_ulp_flt( out_err ); break;

		case 4: analyse4_bw_ulp_flt( out_err ); break;	

		default:
			cerr << "Analyse method :" << method << " not defined" << endl;
	}

    


}

void advar::analyse_bw_ulp_fx( double ulp_err )
{


  DTYPE output_abs_error;

  output_abs_error = ulp_err;

  size_t no_of_vars = grads.size();
  
  iter_t g_itr = grads.begin();

  DTYPE delta_x1 , log2_val;

  DTYPE log2_MSB;
  
  //MpFloat Temp;
   
  //Temp.SetPrec(53);

  for ( size_t ctr = 0 ;  g_itr != grads.end() ; g_itr++ , ctr++ )
    {
      
      
      
      delta_x1 = ( ( static_cast<double>(no_of_vars) *  (*g_itr) ) / output_abs_error );

	  //delta_x1 = output_abs_error / ( ( static_cast<double>(no_of_vars) *  (*g_itr) ) );
      
      // Temp = BS.advar_vec[ ctr ]->ref_value; 
     
      // x_exp = Temp.get_exp();

      log2_val = log( ABS_AD(delta_x1) ) / log( 2. );
      
      //BS.advar_vec[ctr]->m_bitwidth  = ceil( MAX( BS.advar_vec[ctr]->m_bitwidth , static_cast<double>( log2_val)));
      BS.advar_vec[ctr]->fp_frac_bw  = MAX( BS.advar_vec[ctr]->fp_frac_bw , ceil( static_cast<double>( log2_val) ));


      log2_MSB = log( ABS_AD( BS.advar_vec[ ctr ]->ref_value ) ) / log( 2. );

      BS.advar_vec[ctr]->fp_int_bw = MAX( BS.advar_vec[ctr]->fp_int_bw , ceil( log2_MSB ) );

    }

  
}

void advar::analyse_bw_fx_fracbw( int method )
{
  
        double out_err = pow( 2.  , -fp_frac_bw - 1  );

  	//cout << "Output Error :" << out_err << " : " << out_err/ref_value << endl;

	switch( method )
	{
	case 1: analyse_bw_fx_1( out_err ); break;

	case 2: analyse_bw_fx_2( out_err ); break;

	case 3: analyse_bw_fx_3( out_err ); break;

	case 4: analyse_bw_fx_4( out_err ); break;

	default:
   	        cerr << "Analyse method :" << method << " not defined" << endl;
	}



}


void advar::analyse_bw_fx_1( double bit_err )
{

  size_t no_of_vars = 0;
  
  iter_t g_itr = grads.begin();

  DTYPE delta_x1 , log2_val , log2_MSB;
  

  size_t ctr = 0;

  while(  g_itr != grads.end()  )
    if(( *g_itr++ != 0 )  && ( BS.advar_vec[ctr++]->fix_frac_state == NOTSET ) ) no_of_vars++;


  g_itr = grads.begin();

  for ( ctr = 0 ;  g_itr != grads.end() ; g_itr++ , ctr++ )
    {
       if ( ( BS.advar_vec[ctr]->fix_frac_state == NOTSET ) && ( *g_itr != 0. ) ) 
	 { 
	   delta_x1 = ( bit_err  / ( (static_cast<double>(no_of_vars) *  (*g_itr)) ) );
     
	   //delta_x1 = ( ( static_cast<double>(no_of_vars) *  (*g_itr) ) / bit_err );       
   
	   log2_val = abs( log2( abs( delta_x1 ) ) );
      
	   //cerr << no_of_vars << " : " << BS.advar_vec[ctr]->var_name << " : " << delta_x1 << " : " << log2_val << endl;

	   BS.advar_vec[ctr]->fp_frac_bw  = MAX( BS.advar_vec[ctr]->fp_frac_bw , ceil( static_cast<double>( log2_val) ));
	 }
    }


  analyse_fx_int_bw();

}


void advar::analyse_bw_fx_2( double bit_err )
{
  
	iter_t g_itr;
	iter_t g_end = grads.end();

	DTYPE max_err = 0.;

	bool done = false;

	list< int > bw_table;
	
	

        BSClass::advar_veclist::iterator var_itr;
	BSClass::advar_veclist::iterator var_end = BS.advar_vec.end();

	for( size_t i = 0 ; i < BS.advar_vec.size() ; i++ ){

	  if ( BS.advar_vec[ i ]->fix_frac_state == NOTSET )
	    bw_table.push_back( 53 );
	  else
	    bw_table.push_back(  (int)BS.advar_vec[ i ]->fp_frac_bw );

	}

	list<int>::iterator bwt_itr;
	list<int>::iterator bwt_end = bw_table.end();
	
	

	int ctr;

	while( !done )
	{

		max_err = get_output_err_fx( bw_table.begin() , bw_table.end() );

		if ( max_err > bit_err )
			done = true;
		else
		{
		  for( bwt_itr = bw_table.begin(), ctr = 0 ; bwt_itr != bwt_end ; bwt_itr++ , ctr++ )
			{
			

			  if ( BS.advar_vec[ ctr ]->fix_frac_state == NOTSET ){
			        if( (*bwt_itr) < 5)
				{
					done = true;
					break;
				}
				else
				{
				(*bwt_itr) -= 1;
				}
			  }
			}
			
	  	}


	}

	size_t ctr2;
	for( ctr2 = 0, bwt_itr = bw_table.begin() ; ctr < BS.advar_vec.size() ; ctr2++ , bwt_itr++)
	{

		BS.advar_vec[ctr2]->fp_frac_bw  = MAX( BS.advar_vec[ctr2]->fp_frac_bw  , (*bwt_itr) );
      
	}





 analyse_fx_int_bw();
}

 //Mantissa bitwidth reduction by
 //1. Reduce the bitwidth of one node while 
 //   keeping all other nodes constant
 //2. Repeat this for all nodes
 //3. Set node fraction to bitwidth found
 //4. Increase the bitwidth of each node by one bit 
 //   until error requirement is met

 //Global used to exchange data between functions
void advar::analyse_bw_fx_3( double bit_err )
{
  	iter_t g_itr;
	iter_t g_end = grads.end();

	
	DTYPE max_err = 0.;

	bool done = false;

	list< int > bw_table;
	list< int > min_bw_table;

	
	BSClass::advar_veclist::iterator var_itr;
	BSClass::advar_veclist::iterator var_end = BS.advar_vec.end();

	for( size_t i = 0 ; i < BS.advar_vec.size() ; i++ )
	{
	  if ( BS.advar_vec[ i ]->fix_frac_state == NOTSET ){
		bw_table.push_back( 5 );
		min_bw_table.push_back( 5 );
	  }
	  else
	    {
	     //  bw_table.push_back( (int)BS.advar_vec[ i ]->fp_frac_bw  );
// 	      min_bw_table.push_back(  (int)BS.advar_vec[ i ]->fp_frac_bw  );
	      	bw_table.push_back( 5 );
		min_bw_table.push_back( 5 );
	      
	    }
	}

	list<int>::iterator bwt_itr;
	list<int>::iterator bwt_end = bw_table.end();
	
	list<int>::iterator min_bwt_itr = min_bw_table.begin();
	//list<int>::iterator min_bwt_end = min_bw_table.end();


	double c_err = 0., p_err = 0.;

	
	int no_of_vars = static_cast<int>( BS.advar_vec.size() );

	// Reduce bitwidth of a node while keeping all other nodes constant
	
	bwt_itr = bw_table.begin();

	

	//p_err = c_err;

	for( int i = 0 ; i < no_of_vars ; i++ )
	{
		c_err = get_output_err_fx( min_bw_table.begin() , min_bw_table.end() );
		
		//cout << c_err << " : " << bit_err << " : " << c_err - bit_err << endl;
		while(( c_err < bit_err ) && ( (*min_bwt_itr) > 1 ) )
		{

			/*if ( c_err > p_err )
				break;*/
			/*if ( (*min_bwt_itr) < 15 )
				break;
			else
			{
				p_err = c_err;
				(*min_bwt_itr) -= 1;
			}*/

		        // cout << "Here " <<BS.advar_vec[ i ]->var_number << endl;
			(*min_bwt_itr) -= 1;

			c_err = get_output_err_fx( min_bw_table.begin() , min_bw_table.end() );

       		
			

		}

		(*bwt_itr++) = (*min_bwt_itr);

		
		(*min_bwt_itr++) = 5;

		

	}

	//cout << "done 1" << endl;

	// Increase the bitwidths until the error spec is met
        // Uniform increase

	p_err = c_err = 0.;

	done = false;	

	while( !done ) 
	{

        c_err = get_output_err_fx( bw_table.begin() , bw_table.end() );
	
		
		if ( c_err < bit_err )
		{
			done = true;
			break;
		}
		else
		{
			for( bwt_itr = bw_table.begin(); bwt_itr != bw_table.end() ;bwt_itr++ )
			{
				(*bwt_itr) += 1;
			}
		}
	
	}



	// Update bitwidth info
	int ctr;
	for( ctr = 0, bwt_itr = bw_table.begin() ; ctr < no_of_vars ; ctr++ , bwt_itr++)
	{

	  if( BS.advar_vec[ctr]->fix_frac_state == NOTSET )
	    BS.advar_vec[ctr]->fp_frac_bw  = MAX( BS.advar_vec[ctr]->fp_frac_bw  , (*bwt_itr) );
      
	}





  analyse_fx_int_bw();
}


/* 1. Start with maximum mantissa bitwidth for all nodes
    2. Remove 1 bit from all the nodes while all the other 
       nodes remain fixed
    3. The node for which the bitwidth is minimised is allowed
       to keep the new bitwidth
    4. Repeat for until the error spec. is met.*/

void advar::analyse_bw_fx_4( double bit_err )
{

  double maximum_err;
int no_of_vars = static_cast<int>( BS.advar_vec.size() );

list< int > bw_table_1, min_bw_table_1;
list< int > bw_table, min_bw_table;


for( int i = 0 ; i < no_of_vars ; i++ )
	{
	  bw_table_1.push_back( ( BS.advar_vec[i]->fp_frac_bw != 0. ?  (int)BS.advar_vec[i]->fp_frac_bw :0)  );
	  min_bw_table_1.push_back( ( BS.advar_vec[i]->fp_frac_bw != 0. ?  (int)BS.advar_vec[i]->fp_frac_bw :0 ) );
	  // bw_table_1.push_back( ( BS.advar_vec[i]->fp_frac_bw)  );
	  //min_bw_table_1.push_back( ( BS.advar_vec[i]->fp_frac_bw ) );
	 
	}

double bias = get_output_err_fx( bw_table_1.begin() , bw_table_1.end() );


 cout << "B : " << bias << endl;

for( int i = 0 ; i < no_of_vars ; i++ )
	{
	  
	  bw_table.push_back( 30  );
	  min_bw_table.push_back( 30 );

	}

list<int>::iterator bwt_itr, min_bwt_itr; 
list<int>::iterator bwt_end = bw_table.end();
list<int>::iterator min_bwt_end = min_bw_table.end();

maximum_err = get_output_err_fx( bw_table.begin() , bw_table.end() );

double c_err = maximum_err;

int ctr;
while( maximum_err < bit_err )
{
	bwt_itr = bw_table.begin();
	min_bwt_itr = min_bw_table.begin();

	maximum_err = 0.;
	for( ctr = 0;  ctr < no_of_vars ; ctr++ , bwt_itr++ , min_bwt_itr++)
	{
		(*bwt_itr) -= 1;
		c_err = get_output_err_fx( bw_table.begin() , bw_table.end() );
		
		maximum_err = max( c_err , maximum_err );

		if (( c_err < bit_err )  )
		{
		  	(*min_bwt_itr) -= 1;
		}
		(*bwt_itr) += 1;

	}

	bw_table.assign( min_bw_table.begin() , min_bw_table.end() );
	//cout << maximum_err << " : " << output_abs_error << endl;
	
}



//int ctr;
for( ctr = 0, min_bwt_itr = min_bw_table.begin() ; ctr < no_of_vars ; ctr++ , min_bwt_itr++)
{
  if( BS.advar_vec[ctr]->fix_frac_state == NOTSET )
	BS.advar_vec[ctr]->fp_frac_bw  = MAX( BS.advar_vec[ctr]->fp_frac_bw  , (*min_bwt_itr) );
      
}



   analyse_fx_int_bw();
}

// Calculate the output error at the output, using the gradients
// for Fixed-Point
// Equivalent to runsim in ( modelsim bitsim )
// Input : List of bitwidths
// Output : Output error estimate
DTYPE advar::get_output_err_fx(list<int>::iterator bwt_itr , list<int>::iterator bwt_end)
{

	double c_err = 0.;

	//list<int>::iterator bwt_itr;

	iter_t g_itr, g_end;

	g_end = grads.end();

	int ctr = 0;

	double exp;	
	for( g_itr = grads.begin(); g_itr != g_end ; bwt_itr++ , g_itr++ , ctr++ )
	{

		//exp = _logb( advar_vec[ctr++]->ref_value );
	  if( BS.advar_vec[ctr]->fix_frac_state == NOTSET )
		c_err += (*g_itr) * pow( 2. , -(*bwt_itr) ); 
	}

	return ABS_AD(c_err);

}



void advar::analyse_bw_fx( double perr )
{


  DTYPE output_abs_error;

  //output_abs_error = ulp_err;

  output_abs_error = ( perr * ref_value );

  size_t no_of_vars = grads.size();
  
  iter_t g_itr = grads.begin();

  DTYPE delta_x1 , log2_val;

//  DTYPE log2_MSB;
  
 // MpFloat Temp;
   
//  Temp.SetPrec(53);

  output_abs_error = ( output_abs_error == 0.0 ? perr * DBL_EPSILON : output_abs_error );

  for ( size_t ctr = 0 ;  g_itr != grads.end() ; g_itr++ , ctr++ )
    {
      
	  //cout << output_abs_error << endl;
      
      delta_x1 = ( ( static_cast<double>(no_of_vars) *  (*g_itr) ) / output_abs_error );
      
      // Temp = advar_vec[ ctr ]->ref_value; 
     
      // x_exp = Temp.get_exp();

      //log2_val = log( ABS_AD(delta_x1) ) / log( 2. );
      
      log2_val = ( delta_x1 != 0. ? log( ABS_AD( delta_x1 ) / log( 2. ) ) : 0. );	

      BS.advar_vec[ctr]->fp_frac_bw  = MAX( BS.advar_vec[ctr]->fp_frac_bw , ceil( static_cast<double>( log2_val) ) + 1.);


      //log2_MSB = log( ABS_AD( advar_vec[ ctr ]->ref_value ) ) / log( 2. );

     // advar_vec[ctr]->fp_int_bw = MAX( advar_vec[ctr]->fp_int_bw , ceil( log2_MSB ) );

    }

  

  analyse_fx_int_bw();
 
 
}

void advar::analyse_fx_int_bw()
{

	BSClass::advar_veclist::iterator u_iter;

	DTYPE log2_MSB;

	for ( u_iter = BS.advar_vec.begin() ; u_iter != BS.advar_vec.end() ; u_iter++ )
	{
      
	  //log2_MSB = log( ABS_AD( (*u_iter)->ref_maxval ) ) / log( 2. );

	  if( (*u_iter)->fix_int_state == NOTSET )
	    {
	      
	      
	      log2_MSB = log2( abs( (*u_iter)->ref_maxval ) ) ;

	      //log2_MSB = log2( abs( (*u_iter)->ref_maxval /( (*u_iter)->ref_minval != 0 ?  (*u_iter)->ref_minval : 1. )) ); 
	  
	      //cerr << (*u_iter)->var_name << " : " << (*u_iter)->ref_maxval << " : " << log2_MSB << endl;

             (*u_iter)->fp_int_bw = MAX( (*u_iter)->fp_int_bw  , ceil( log2_MSB  ) );



	 
	    }

	}

}



void advar::print_flt_mat( ostream &os )
{
    BSClass::advar_veclist::iterator u_iter;

   for( u_iter = BS.advar_vec.begin(); u_iter != BS.advar_vec.end() ; u_iter++)
    {

      //os << (*u_iter)->var_name << "_NB =" << (*u_iter)->fp_int_bw + (*u_iter)->fp_frac_bw << ";" << endl;
      //os << (*u_iter)->var_name << "_BP =" << (*u_iter)->fp_frac_bw << ";" << endl; 
		// Binary Point from right
      os << (*u_iter)->var_name << " " << (*u_iter)->m_bitwidth;
	  os << " " << 6 << endl;
    }
  
  

}

void advar::print_flt_dfg( ostream &os )
{
   BSClass::advar_veclist::iterator u_iter;

   os << "FLOATPOINT" << endl;

   for( u_iter = BS.advar_vec.begin(); u_iter != BS.advar_vec.end() ; u_iter++)
    {

      os << (*u_iter)->var_name << " " << (*u_iter)->m_bitwidth;
	  os << " " << (*u_iter)->e_bitwidth << endl;
    }

}


void advar::print_fx_mat( ostream &os )
{
    BSClass::advar_veclist::iterator u_iter;

   for( u_iter = BS.advar_vec.begin(); u_iter != BS.advar_vec.end() ; u_iter++)
    {

      //os << (*u_iter)->var_name << "_NB =" << (*u_iter)->fp_int_bw + (*u_iter)->fp_frac_bw << ";" << endl;
      //os << (*u_iter)->var_name << "_BP =" << (*u_iter)->fp_frac_bw << ";" << endl; 
		// Binary Point from right
      os << (*u_iter)->var_name << " " << (*u_iter)->fp_int_bw + (*u_iter)->fp_frac_bw << " " << (*u_iter)->fp_frac_bw << endl;
    }
  
  

}

void advar::print_fx_dfg( ostream &os )
{
   BSClass::advar_veclist::iterator u_iter;

   os << "FIXPOINT" << endl;

   for( u_iter = BS.advar_vec.begin(); u_iter != BS.advar_vec.end() ; u_iter++)
    {
      os << (*u_iter)->var_name << " " << (*u_iter)->fp_int_bw + (*u_iter)->fp_frac_bw << " " << (*u_iter)->fp_frac_bw << endl;
    }



}


void advar::set_error( double err )
{
  output_perr = err;
}



void advar::find_bitwidths()
{

 //  ref_minval.SetPrec( 23 );

//   MpFloat output_abs_error = output_perr * ref_minval;

//   size_t no_of_vars = max_grads.size();

//   MpFloat delta_x,log2_val,x_exp ;
 
//   iter_t g_itr = max_grads.begin();

//   for ( size_t ctr = 0 ;  g_itr != max_grads.end() ; g_itr++ , ctr++ )
//     {
//       delta_x = ( output_abs_error / static_cast<double>(no_of_vars) ) / (*g_itr);
//       x_exp = advar_vec[ ctr ]->ref_maxval.get_exp();
//       log2_val = log( abs(delta_x) ) / log( 2. );
//       advar_vec[ctr]->m_bitwidth  = ceil( MAX( m_bitwidth  , static_cast<double>(x_exp - log2_val)));
//     }



}


void advar::analyse2_bw_flt( double perr )
{

	DTYPE output_abs_error;

	output_abs_error = ( perr * ref_value );
	
	output_abs_error = ( output_abs_error == 0.0 ? perr * DBL_EPSILON : output_abs_error );

	bw_method2( output_abs_error );

	analyse_flt_e_bw();

}


void advar::analyse2_bw_ulp_flt( double ulp_err , int min_bw )
{

	bw_method2( ulp_err , min_bw );

	analyse_flt_e_bw();

}

void advar::bw_method2( double output_abs_error , int min_bw )
{

	iter_t g_itr;
	iter_t g_end = grads.end();

	DTYPE max_err = 0.;

	bool done = false;

	list< int > bw_table;
	
	

        BSClass::advar_veclist::iterator var_itr;
	BSClass::advar_veclist::iterator var_end = BS.advar_vec.end();

	for( size_t i = 0 ; i < BS.advar_vec.size() ; i++ )
		bw_table.push_back( 22 );

	list<int>::iterator bwt_itr;
	list<int>::iterator bwt_end = bw_table.end();
	
	


	while( !done )
	{

		max_err = get_output_err_flt( bw_table.begin() , bw_table.end() );

		if ( max_err > output_abs_error )
			done = true;
		else
		{
			for( bwt_itr = bw_table.begin() ; bwt_itr != bwt_end ; bwt_itr++ )
			{
				if( (*bwt_itr) < 5)
				{
					done = true;
					break;
				}
				else
				{
				(*bwt_itr) -= 1;
				}
			}
			
	  	}


	}

	size_t ctr;
	for( ctr = 0, bwt_itr = bw_table.begin() ; ctr < BS.advar_vec.size() ; ctr++ , bwt_itr++)
	{

		BS.advar_vec[ctr]->m_bitwidth  = MAX( BS.advar_vec[ctr]->m_bitwidth  , (*bwt_itr) );
      
	}



}

 //Mantissa bitwidth reduction by
 //1. Reduce the bitwidth of one node while 
 //   keeping all other nodes constant
 //2. Repeat this for all nodes
 //3. Set node mantissa's to bitwidth found
 //4. Increase the bitwidth of each node by one bit 
 //   until error requirement is met

 //Global used to exchange data between functions


void advar::analyse3_bw_flt( double perr )
{

	DTYPE output_abs_error;

	output_abs_error = ( perr * ref_value );
	
	output_abs_error = ( output_abs_error == 0.0 ? perr * DBL_EPSILON : output_abs_error );

	bw_method3( output_abs_error );

	analyse_flt_e_bw();

}

void advar::analyse3_bw_ulp_flt( double ulp_err )
{

	bw_method3( ulp_err );

	analyse_flt_e_bw();

}

void advar::bw_method3( double output_abs_error )
{

	iter_t g_itr;
	iter_t g_end = grads.end();

	
	DTYPE max_err = 0.;

	bool done = false;

	list< int > bw_table;
	list< int > min_bw_table;

	
	BSClass::advar_veclist::iterator var_itr;
	BSClass::advar_veclist::iterator var_end = BS.advar_vec.end();

	for( size_t i = 0 ; i < BS.advar_vec.size() ; i++ )
	{
		bw_table.push_back( 40 );
		min_bw_table.push_back( 40 );
	}

	list<int>::iterator bwt_itr;
	list<int>::iterator bwt_end = bw_table.end();
	
	list<int>::iterator min_bwt_itr = min_bw_table.begin();
	//list<int>::iterator min_bwt_end = min_bw_table.end();


	double c_err = 0., p_err = 0.;

	
	int no_of_vars = static_cast<int>( BS.advar_vec.size() );

	// Reduce bitwidth of a node while keeping all other nodes constant
	
	bwt_itr = bw_table.begin();

	

	//p_err = c_err;

	for( int i = 0 ; i < no_of_vars ; i++ )
	{
		c_err = get_output_err_flt( min_bw_table.begin() , min_bw_table.end() );
		
		while(( c_err < output_abs_error ) && ( (*min_bwt_itr) > 1 ) )
		{

			/*if ( c_err > p_err )
				break;*/
			/*if ( (*min_bwt_itr) < 15 )
				break;
			else
			{
				p_err = c_err;
				(*min_bwt_itr) -= 1;
			}*/

			(*min_bwt_itr) -= 1;

			c_err = get_output_err_flt( min_bw_table.begin() , min_bw_table.end() );

       		
			

		}

		(*bwt_itr++) = (*min_bwt_itr);

		
		(*min_bwt_itr++) = 22;

		

	}

	//cout << "done 1" << endl;

	// Increase the bitwidths until the error spec is met


	p_err = c_err = 0.;

	done = false;	

	while( !done ) 
	{

        c_err = get_output_err_flt( bw_table.begin() , bw_table.end() );
	
		
		if ( c_err < output_abs_error )
		{
			done = true;
			break;
		}
		else
		{
			for( bwt_itr = bw_table.begin(); bwt_itr != bw_table.end() ;bwt_itr++ )
			{
				(*bwt_itr) += 1;
			}
		}
	
	}



	// Update bitwidth info
	int ctr;
	for( ctr = 0, bwt_itr = bw_table.begin() ; ctr < no_of_vars ; ctr++ , bwt_itr++)
	{

		BS.advar_vec[ctr]->m_bitwidth  = MAX( BS.advar_vec[ctr]->m_bitwidth  , (*bwt_itr) );
      
	}



}

// Calculate the output error at the output, using the gradients
// for Floating-Point
// Equivalent to runsim in ( modelsim bitsim )
// Input : List of bitwidths
// Output : Output error estimate
DTYPE advar::get_output_err_flt(list<int>::iterator bwt_itr , list<int>::iterator bwt_end)
{

	double c_err = 0;

	//list<int>::iterator bwt_itr;

	iter_t g_itr, g_end;

	g_end = grads.end();

	int ctr = 0;

	double exp;	
	for( g_itr = grads.begin(); g_itr != g_end ; bwt_itr++ , g_itr++ )
	{

		exp = _logb( BS.advar_vec[ctr++]->ref_value );
		c_err += (*g_itr) * pow( 2. , -(*bwt_itr) ) * pow( 2. , exp); 
	}

	return ABS_AD(c_err);

}





 /* 1. Start with maximum mantissa bitwidth for all nodes
    2. Remove 1 bit from all the nodes while all the other 
       nodes remain fixed
    3. The node for which the bitwidth is minimised is allowed
       to keep the new bitwidth
    4. Repeat for until the error spec. is met.*/

void advar::analyse4_bw_flt( double perr )
{

	DTYPE output_abs_error;

	output_abs_error = ( perr * ref_value );
	
	output_abs_error = ( output_abs_error == 0.0 ? perr * DBL_EPSILON : output_abs_error );

	bw_method4( output_abs_error );

	analyse_flt_e_bw();

}

void advar::analyse4_bw_ulp_flt( double ulp_err )
{

	bw_method4( ulp_err );

	analyse_flt_e_bw();

}


void advar::bw_method4( double output_abs_error )
{


double maximum_err;
int no_of_vars = static_cast<int>( BS.advar_vec.size() );

list< int > bw_table, min_bw_table;


for( int i = 0 ; i < no_of_vars ; i++ )
	{
		bw_table.push_back( 22 );
		min_bw_table.push_back( 22 );
	}

list<int>::iterator bwt_itr, min_bwt_itr; 
list<int>::iterator bwt_end = bw_table.end();
list<int>::iterator min_bwt_end = min_bw_table.end();

maximum_err = get_output_err_flt( bw_table.begin() , bw_table.end() );

double c_err = maximum_err;

int ctr;
while( maximum_err < output_abs_error )
{
	bwt_itr = bw_table.begin();
	min_bwt_itr = min_bw_table.begin();

	maximum_err = 0.;
	for( ctr = 0;  ctr < no_of_vars ; ctr++ , bwt_itr++ , min_bwt_itr++)
	{
		(*bwt_itr) -= 1;
		c_err = get_output_err_flt( bw_table.begin() , bw_table.end() );
		
		maximum_err = max( c_err , maximum_err );

		if (( c_err < output_abs_error )  )
		{
		  	(*min_bwt_itr) -= 1;
		}
		(*bwt_itr) += 1;

	}

	bw_table.assign( min_bw_table.begin() , min_bw_table.end() );
	//cout << maximum_err << " : " << output_abs_error << endl;
	
}



//int ctr;
for( ctr = 0, min_bwt_itr = min_bw_table.begin() ; ctr < no_of_vars ; ctr++ , min_bwt_itr++)
{

	BS.advar_vec[ctr]->m_bitwidth  = MAX( BS.advar_vec[ctr]->m_bitwidth  , (*min_bwt_itr) );
      
}


}





void advar::print_flt_make_defs( ostream &os  )
{


  list<advar *>::iterator u_iter;

  for ( u_iter = named_vars.begin() ; u_iter != named_vars.end() ; u_iter++  ) 
    {

	  os << endl << "BITWIDTH_CONFIG+= ";
      os << "-D" << (*u_iter)->var_name << "_HWFLOAT_TOTAL=\"" << (*u_iter)->m_bitwidth + (*u_iter)->e_bitwidth << "\"" << endl;
	  os << "BITWIDTH_CONFIG+= ";
	  os << "-D" << (*u_iter)->var_name << "_HWFLOAT_EXPONENT=\"" << (*u_iter)->e_bitwidth << "\"" << endl;
      
	 
    }

  

}


//#ifdef _BITSIZE_DEBUG_
void advar::print_grads_vec()
{

   iter_t g_iter, g_end;
   int i;

     
     g_end = grads.end();


  
  for (i = 0, g_iter = grads.begin() ; g_iter != g_end ; g_iter++ , i++)
   { 
     cout << i << " : ";
   }

  cout << endl;
  for (i = 0, g_iter = grads.begin(); g_iter != g_end ; g_iter++ , i++)
   { 
     cout << (*g_iter) << " : ";
    
   }
  cout << endl;
  


}
//#endif
