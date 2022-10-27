// $Author: amag97 $
// $Id: hwfix.cpp,v 1.34 2006/03/13 13:22:38 amag97 Exp $
// $Log: hwfix.cpp,v $
// Revision 1.34  2006/03/13 13:22:38  amag97
// Major Commit - new rep
//
// Revision 1.33  2006/02/14 19:07:54  amag97
// Added new Pass for generating glitch based power cost function
// Bug Fixes - conv. between range and bitwidth
//
// Revision 1.32  2005/12/13 14:25:40  amag97
// New compiler version fixes
//
// Revision 1.31  2005/05/27 17:15:39  amag97
// More power stuff and fixes
//
// Revision 1.30  2005/01/17 21:06:40  amag97
// Changes - specific to feedback
//
// Revision 1.29  2004/12/31 18:42:00  amag97
// More fixes to divide
//
// Revision 1.28  2004/12/13 01:33:43  amag97
// Added command line argument parsing
// Added Interval based range analysis
// Added ASSIGN node to dag
//
// Revision 1.27  2004/11/28 23:29:02  amag97
// Added back support for HWfloat
// Uniform bit-width optimisation for HWfloat
//
// Revision 1.26  2004/11/17 19:13:10  amag97
// More bug fixes
//
// Revision 1.25  2004/11/16 21:56:19  amag97
// More bug fixes
//
// Revision 1.24  2004/11/12 19:05:24  amag97
// More fixes to affine erromodel
//
// Revision 1.23  2004/10/26 18:32:59  amag97
// Fixed most bugs with fixed-point frac. opt 6
//
// Revision 1.22  2004/10/23 10:37:36  amag97
// Addition of new fraction bw evaluation method
// (work in progress)
//
// Revision 1.21  2004/10/05 19:23:56  amag97
// Fixes to AD code - bug when multiple inputs are used
// Updated Graph info
//
// Revision 1.20  2004/09/30 20:32:51  amag97
// Added Area model
// Lots of fixes to get it to work
//
// Revision 1.19  2004/09/17 16:52:13  amag97
// Added area estimation code
//
// Revision 1.18  2004/09/10 17:21:55  amag97
// Changes to bwopt method 3
//
// Revision 1.17  2004/08/26 18:00:52  amag97
// Update after return from Sri Lanka
// Started work on adding user constraints
//
// Revision 1.16  2004/07/21 18:22:04  amag97
// Pre-vacation
//
// Revision 1.15  2004/07/09 10:22:21  amag97
// More fixes to DAG code
//
// Revision 1.14  2004/07/06 19:48:11  amag97
// Revised DAG generation
//
// Revision 1.13  2004/06/24 18:45:31  amag97
// Added a seperate class to encapsulate the bitwidth optimisation functions
//
// Revision 1.12  2004/06/10 19:57:23  amag97
// Added leading one detect
//
// Revision 1.11  2004/06/09 21:20:51  amag97
// Added more bitwidth analysis methods for fixed-point
//
// Revision 1.10  2004/06/08 21:07:40  amag97
// More bugfixes to - advar
//
// Revision 1.9  2004/06/08 01:35:46  amag97
// Fixed HWvector bitwidth calculation
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


//#include "hwfix.h"
#include "hwvar.h"

BWOPTfix bw_opt;

vector<HWvector<HWfix>* > HWfix::vec_c;


void HWfix::fix_vecbitwidths()
{
   vector<HWvector<HWfix>* >::iterator itr = vec_c.begin();

   vector<HWfix *>::iterator vec_itr;

   int max_frac_bw = 0;
   int max_int_bw = 0;

   while( itr != vec_c.end() )
    {

      max_frac_bw = 0;
      max_int_bw = 0;

      for( vec_itr = (*itr)->var_vec.begin(); vec_itr != (*itr)->var_vec.end() ; vec_itr++ )
	{

	  max_frac_bw = max( (int)(*vec_itr)->ad_val.fp_frac_bw ,  max_frac_bw );
	  max_int_bw = max( (int)(*vec_itr)->ad_val.fp_int_bw ,  max_int_bw );
	}
      
      (*itr)->proto_type->ad_val.fp_frac_bw = max_frac_bw;
      (*itr)->proto_type->ad_val.fp_int_bw = max_int_bw;
      
      cout << (*itr)->proto_type->var_name << " : " << max_int_bw << " : " << max_frac_bw << endl;

       itr++;
     }


 }


void HWfix::init()
{

	advar::advar_setup( AD_AUTONUM );


}

// HWfix HWabs( const HWfix& a )
// {
//   HWfix temp;

//   temp.ad_val.ref_value = fabs( a.ad_val.ref_value );

//   return temp;



// }

HWfix::HWfix(arch_attr Signal_type, range var_range , fsize fractional, sign_attr sign, string name)
{

  init(Signal_type, OPT_BW + fractional , fractional, sign, name);

  myedge->e_min = var_range.min;

  myedge->e_max = var_range.max;

  
}


HWfix::HWfix(arch_attr Signal_type, fsize init_size , fsize fractional , sign_attr sign , string name)
{

  init(Signal_type, init_size, fractional, sign, name);

  int integer_bw = static_cast<int>( int_bw );
  //INTERNAL_ASSERT_EXIT( (integer_bw != 0), "Integer bit width must be specified for variable " << name );


  if ( c_sign == UNSIGNED ){
    myedge->e_min = 0;
    //myedge->e_max =  (integer_bw % 2 ) == 0 ? pow( 2. , integer_bw ) : pow( 2. , integer_bw ) - 1;

    myedge->e_max = ( integer_bw == 0 ) ?  1 : pow( 2. , integer_bw ) - 1;
  }
  else if ( c_sign == TWOSCOMPLEMENT )
  {
     myedge->e_min = -pow( 2. , integer_bw - 1 );
     myedge->e_max = pow( 2. , integer_bw - 1 ) - 1;

  }
  else if ( c_sign = SIGNMAGNITUDE )
    {

      myedge->e_min = -pow( 2. , integer_bw - 1 )+1;
      myedge->e_max = pow( 2. , integer_bw - 1 )-1;

    }
  else{
      myedge->e_min = 0;
      if( frac_bw_state == SET ){

	myedge->e_min = 2 - pow( 2. , -(int)frac_bw);
      }

      myedge->e_max = pow( 2. , integer_bw ) - 1;

  }



}



void HWfix::init(arch_attr Signal_type, fsize init_size, fsize fractional, sign_attr sign, string name)
{

  if ( ( bs->mydag->arith_type == AT_NOTSET ) || ( bs->mydag->arith_type == FIXED ) )
    bs->mydag->arith_type = FIXED;
  else
    ERROR( name << " : Multiple arithmetic types not supported" )
 

  var_name = ( name == "" ) ? vnames.gen_varname() : name;
  
  //myedge->edge_name = var_name;

  DBLOG(  "HWfix const called " << var_name )

 
    //var_number = var_count++;
  
  var_dectype = DECVAR;

   // DAG generation
       
  myedge = bs->mydag->new_edge( this , var_name);
      

      if ( Signal_type == IN )
	{

	  Node* temp_node = bs->mydag->new_node( INPUT , FIXED ); 
	  
	  //src_nodes.push_back( temp_node_n );

	  bs->mydag->add_out_edge( temp_node , myedge );

	  myedge->edge_type = IN;

	}
      else if ( Signal_type == OUT )
	{

	  Node* temp_node_n = bs->mydag->new_node( OUTPUT , FIXED );
	  
	  //dst_nodes.push_back( temp_node_n );
	  
	  bs->mydag->add_in_edge( temp_node_n , myedge );
 
	  myedge->edge_type = OUT;

	}

   
      
    



  // Checks for output variables
      if ( ( Signal_type == OUT ) && (fractional == OPT_BW))
	ERROR( "Output Var :" <<  var_name <<  " must specify a bitwidth" )

  
	  // Add the Signal to the correct collection
	  add_varcollection( Signal_type );  

      if ( myedge->edge_type != OUT ){
	bs->mydag->edges_to_analyse.push_back( myedge->edge_number );
      }

  // Check and set variable bitwidth optimisation modes
  // 4 combinations are possible between TOTAL_BW and FRAC_BW
 
  if ( ( init_size == OPT_BW ) && ( fractional == OPT_BW ) )
  {
    int_bw_state = NOTSET; int_bw = 0;
    frac_bw_state = NOTSET; frac_bw = 0;

  } 
   else if( (init_size >= OPT_BW ) && ( fractional == OPT_BW ) )
  {
    frac_bw_state = NOTSET; frac_bw = 0;

    if ( init_size < OPT_BW )
       ERROR( "Var : " << var_name << " TotalSize should be specified as : OPT_BW + (integer part bitwidth )" )
     else	
      { int_bw_state = SET; int_bw = init_size - OPT_BW;  }
  }
   else if( ( init_size >= OPT_BW ) && ( fractional != OPT_BW ) )
  {
    int_bw_state = NOTSET; int_bw = 0;
    frac_bw_state = SET; frac_bw = fractional;
  }
 else
    {
      int_bw_state = SET; int_bw = init_size - fractional;

      
      frac_bw_state = SET; frac_bw = fractional;
    }


  ad_val.var_name = var_name;
  ad_val <<= 0.0;

  ad_val.fp_frac_bw = static_cast<double>( frac_bw );
  ad_val.fp_int_bw = static_cast<double>( int_bw );

  ad_val.fix_frac_state = frac_bw_state;
  ad_val.fix_int_state = int_bw_state;

  c_sign = sign;
  var_type = Signal_type;
 
  DBLOG( var_name << "FRAC : " << frac_bw )
  DBLOG( var_name << "INT : " << int_bw )


}


// Seperate constructor for TMP variables.
HWfix::HWfix()
{
    
    ad_val <<= 0.0;
    
    var_name = tmpvnames.gen_varname();
    ad_val.var_name = var_name;
    //var_number = var_count++;
    //bs->mydag->new_edge( this );

    frac_bw = 0;
    int_bw = 0;

    var_dectype = TMPVAR;

   
}

HWfix::~HWfix()
{
  DBLOG( "HWfix dist called " << var_name )
}


HWfix::HWfix ( const HWfix &a ):HWvar(a){

  
  frac_bw = a.frac_bw;
  int_bw = a.int_bw;
  c_sign = a.c_sign;

  if ( a.var_dectype == VECVAR )
    add_varcollection(var_type ); 
     
 }





HWfix& HWfix::operator= ( const HWfix &r){

    if ( this != &r )
    { 
      HWvar::operator=( r );

      if ( frac_bw_state == NOTSET )
	frac_bw = r.frac_bw;
      
      if ( int_bw_state == NOTSET )
	int_bw = r.int_bw;

      //c_sign = r.c_sign;

       if ( bs->global_opmode == PARSING )
       {

	 if ( var_dectype == DECVAR )

	 
	  

	  if ( r.var_dectype == DECVAR )
	    bs->mydag->new_assign_node( r.myedge , myedge );
	  else
	     bs->mydag->add_out_edge( bs->mydag->node_vec[ r.myedge->src_nodes.back() ] , myedge );

	  myedge->copy( r.myedge );
       }


    }

	return *this;

 }


 
HWfix& HWfix::operator= (double val) {

        ad_val <<= val;

	if( ad_val.fix_frac_state != SET )
	  ad_val.fix_frac_state = NOTSET;
	if( ad_val.fix_int_state != SET )
	  ad_val.fix_int_state = NOTSET;


	if ( frac_bw_state != SET )
	  frac_bw_state = NOTSET;
	if( int_bw_state != SET )
	  int_bw_state = NOTSET;
	

	if ( bs->global_opmode == PARSING )
	  {
	    Node* const_node = bs->mydag->new_const_node( FIXED , val  );
	    bs->mydag->add_out_edge( const_node , myedge );
	    
	  }


	myedge->e_max = val;
	
	myedge->X0 = (myedge->e_max);
        myedge->Xk = (myedge->e_max);

	return *this;
}


HWfix& HWfix::operator= (const HWint &a) {

  ad_val <<= static_cast<double>( a.var_val );

 
  if( ad_val.fix_frac_state != SET )
       ad_val.fix_frac_state = NOTSET;
  if( ad_val.fix_int_state != SET )
       ad_val.fix_int_state = NOTSET;


 if ( bs->global_opmode == PARSING )
  {
    //Node* hwint_node = bs->mydag->new_hwint_node();
    //  bs->mydag->add_out_edge( hwint_node , myedge  );
    myedge = bs->mydag->new_hwint_edge();

  }

  return *this;
}




HWint HWfix::operator[]( int index )
{

  int temp1, temp2;
  double db_temp = ( ad_val.ref_value )* pow( 2. ,  static_cast<double>(frac_bw) ) ;
  
  if ( index > ( frac_bw + int_bw ) ) ERROR( "Index " << index << " HWfix total bitwidth " << ( frac_bw + int_bw ) )
   

  temp1 = static_cast<int>( db_temp );

  temp2 = ( temp1 & ( 1 << index )) >> index; 

  HWint temp;
  
  temp = temp2;

  return temp;

}

// Overloaded Operators

HWfix operator+ (const HWfix &l, const HWfix &r)  { 
	
	HWfix temp;

	temp.ad_val = l.ad_val + r.ad_val;
  
	
	if ( bs->global_opmode == PARSING )
	  {
	    Node* op_node = bs->mydag->add_op_node( ADD , FIXED , l.myedge , r.myedge  );
	    temp.myedge =  bs->mydag->new_edge( NULL , "TEMP" );
	    bs->mydag->add_out_edge( op_node , temp.myedge );

	  }

	return temp;

}

HWfix operator+  (const double dval,const HWfix &r)  { 

	HWfix temp;

	temp.ad_val = dval + r.ad_val;

	if ( bs->global_opmode == PARSING )
	  {
	    
	    Edge* const_edge = bs->mydag->new_const_edge( dval );
	    Node* op_node = bs->mydag->add_op_node( ADD ,  FIXED ,const_edge , r.myedge  );
	    temp.myedge =  bs->mydag->new_edge( NULL , "TEMP" );
	    bs->mydag->add_out_edge( op_node , temp.myedge );
	    
	  }

	return temp;


}
HWfix operator+  (const HWfix &l, const double dval)  { 

	HWfix temp;

	temp.ad_val = l.ad_val + dval;


	if ( bs->global_opmode == PARSING )
	  {
	    Edge* const_edge = bs->mydag->new_const_edge( dval );
	    Node* op_node = bs->mydag->add_op_node( ADD , FIXED , l.myedge , const_edge );
	    temp.myedge =  bs->mydag->new_edge(  NULL , "TEMP"  );
	    bs->mydag->add_out_edge( op_node , temp.myedge );

          }

	return temp;


}

HWfix operator+  (int val,const HWfix &r)
{

  	HWfix temp;

	temp.ad_val = (double)val + r.ad_val;

	if ( bs->global_opmode == PARSING )
	  {
	    // Create const node and a edge from const node to operator
// 	    dag_index const_node_n = bs->mydag->new_const_node( (double)val );
// 	    HWfix* temp_l = new HWfix();
// 	    dag_index temp_l_n =  bs->mydag->add_out_edge( const_node_n , bs->mydag->new_edge(  temp_l  ) );
	    
// 	    // Create operator node and a temp edge from it 
// 	    dag_index op_node_n = bs->mydag->add_op_node( ADD , temp_l_n , r.var_number );
// 	    bs->mydag->add_out_edge( op_node_n , bs->mydag->new_edge( new HWfix() ) );
// 	    temp.src_nodes.push_back( op_node_n  );


	  }

	return temp;



}
HWfix operator+  (const HWfix &l, int val)
{

  	HWfix temp;

	temp.ad_val = l.ad_val + (double)val;


	if ( bs->global_opmode == PARSING )
	  {
// 	    dag_index const_node_n = bs->mydag->new_const_node( (double)val );
// 	    HWfix* temp_r = new HWfix();
// 	    dag_index temp_r_n = bs->mydag->add_out_edge( const_node_n , bs->mydag->new_edge(  temp_r  ) );
	    

// 	    dag_index op_node_n = bs->mydag->add_op_node( ADD ,l.var_number , temp_r_n  );
// 	    bs->mydag->add_out_edge( op_node_n , bs->mydag->new_edge( new HWfix() ) );
// 	    temp.src_nodes.push_back( op_node_n  );


	  }

	return temp;

}

HWfix operator-  (const HWfix &l, const HWfix &r)  { 
	
	HWfix temp;


	//const_cast<HWfix *>(&l)->ad_val.print_grads_vec();
	//const_cast<HWfix *>(&r)->ad_val.print_grads_vec();
        temp.ad_val = l.ad_val - r.ad_val;
  

	if ( bs->global_opmode == PARSING )
	   {	    
	     Node* op_node = bs->mydag->add_op_node( SUB , FIXED , l.myedge , r.myedge  );
	     temp.myedge =  bs->mydag->new_edge( NULL , "TEMP" );
	     bs->mydag->add_out_edge( op_node , temp.myedge );

	   }


	
	return temp;


}


HWfix operator-  (const double dval, const  HWfix &r)  { 

	HWfix temp;

	temp.ad_val = dval - r.ad_val;

	if ( bs->global_opmode == PARSING )
	  {
	    	    
	    Edge* const_edge = bs->mydag->new_const_edge( dval );
	    Node* op_node = bs->mydag->add_op_node( SUB , FIXED , const_edge , r.myedge  );
	    temp.myedge =  bs->mydag->new_edge( NULL , "TEMP" );
	    bs->mydag->add_out_edge( op_node , temp.myedge );

	  }
	



	return temp;

}

HWfix operator-  (const HWfix &l, const double dval)  { 

	HWfix temp;

	temp.ad_val = l.ad_val - dval;

	  if ( bs->global_opmode == PARSING )
	  {
	    Edge* const_edge = bs->mydag->new_const_edge( dval );
	    Node* op_node = bs->mydag->add_op_node( SUB , FIXED , l.myedge , const_edge );
	    temp.myedge =  bs->mydag->new_edge(  NULL , "TEMP"  );
	    bs->mydag->add_out_edge( op_node , temp.myedge );

	  }
	

	return temp;

}


HWfix operator*  (const HWfix &l, const HWfix &r)  {

	HWfix temp;


	temp.ad_val = l.ad_val * r.ad_val;
  

	if ( bs->global_opmode == PARSING )
	  {
	    Node* op_node = bs->mydag->add_op_node( MUL , FIXED , l.myedge , r.myedge  );
	    temp.myedge =  bs->mydag->new_edge( NULL , "TEMP" );
	    bs->mydag->add_out_edge( op_node , temp.myedge );

	  }

	return temp;

}

HWfix operator*  ( const double dval, const HWfix &r)  { 

	HWfix temp;

	temp.ad_val = dval * r.ad_val;

	if ( bs->global_opmode == PARSING )
	  {
	    Edge* const_edge = bs->mydag->new_const_edge( dval );
	    Node* op_node = bs->mydag->add_op_node( MUL , FIXED , const_edge , r.myedge  );
	    temp.myedge =  bs->mydag->new_edge( NULL , "TEMP" );
	    bs->mydag->add_out_edge( op_node , temp.myedge );
	    
	  }


	return temp;


}


HWfix operator*  (const HWfix &l, const double dval)  { 

        HWfix temp;

	temp.ad_val = l.ad_val * dval;

	if ( bs->global_opmode == PARSING )
	  {
	    Edge* const_edge = bs->mydag->new_const_edge( dval );
	    Node* op_node = bs->mydag->add_op_node( MUL , FIXED , l.myedge , const_edge );
	    temp.myedge =  bs->mydag->new_edge(  NULL , "TEMP"  );
	    bs->mydag->add_out_edge( op_node , temp.myedge );

	  }


	return temp;
  

}


HWfix operator/  (const HWfix &l, const HWfix &r)  {

	HWfix temp;



	temp.ad_val = l.ad_val / r.ad_val;
  
	
	if ( bs->global_opmode == PARSING )
	  {
	    Node* op_node = bs->mydag->add_op_node( DIV , FIXED , l.myedge , r.myedge  );
	    temp.myedge =  bs->mydag->new_edge( NULL , "TEMP" );
	    bs->mydag->add_out_edge( op_node , temp.myedge );


	  }


	return temp;

}

HWfix operator/  ( const double dval, const HWfix &r)  { 

	HWfix temp;

	temp.ad_val = dval / r.ad_val;

	if ( bs->global_opmode == PARSING )
	  {
	    Edge* const_edge = bs->mydag->new_const_edge( dval );
	    Node* op_node = bs->mydag->add_op_node( DIV , FIXED , const_edge , r.myedge  );
	    temp.myedge =  bs->mydag->new_edge( NULL , "TEMP" );
	    bs->mydag->add_out_edge( op_node , temp.myedge );

	  }


	return temp;


}


HWfix operator/  (const HWfix &l, const double dval)  { 

        HWfix temp;

	temp.ad_val = l.ad_val / dval;

	if ( bs->global_opmode == PARSING )
	  {
	    Edge* const_edge = bs->mydag->new_const_edge( dval );
	    Node* op_node = bs->mydag->add_op_node( DIV , FIXED , l.myedge , const_edge );
	    temp.myedge =  bs->mydag->new_edge(  NULL , "TEMP"  );
	    bs->mydag->add_out_edge( op_node , temp.myedge );

	  }

	return temp;
  

}



////////////////////////////////////////////////

void HWfix::output_analysis( int mode )
{


  
   varcollections<HWvar *>::iter_t itr = bs->varC.outvar_c.begin();


   while( itr != bs->varC.outvar_c.end() )
    {
       (*itr)->ad_val.analyse_bw_fx_fracbw( mode );

       cout << (*itr)->var_name << " : " << (*itr)->ad_val.ref_value << endl; 
       itr++;
     }
  

}


void HWfix::setup_max_grads()
{

  varcollections<HWvar *>::iter_t itr = bs->varC.outvar_c.begin();


   while( itr != bs->varC.outvar_c.end() )
    {


      bw_opt.setup_max_grads( dynamic_cast<HWfix *>((*itr)));
      
      itr++;
    }
}


void HWfix::output_analysis2( )
{


  
   varcollections<HWvar *>::iter_t itr = bs->varC.outvar_c.begin();

   //bw_opt.current_method = mode;

   int ctr = 0;

  

   while( itr != bs->varC.outvar_c.end() )
    {
      //(*itr)->ad_val.analyse_bw_fx_fracbw( mode );

      //(*itr)->ad_val.print_grads_vec();
      bw_opt.find_bw( dynamic_cast<HWfix *>((*itr)) , ctr++ );

      //cout << (*itr)->var_name << " : " << (*itr)->ad_val.ref_value << endl;
      itr++;
     }
  

}


void print_bwdata(ostream &os , varcollections<HWvar *>::iter_t itr_begin ,  varcollections<HWvar *>::iter_t itr_end )
{

  varcollections<HWvar *>::iter_t u_iter;

  string ss;

  ss = "BITSIZE_DATA+= ";

  for ( u_iter = itr_begin ; u_iter != itr_end ; u_iter++  ) 
	{

	  // os << endl << "BITSIZE_DATA+= ";
//  	  os << "-D" << (*u_iter)->ad_val.var_name << "_BW=";
// 	  os << (*u_iter)->ad_val.fp_int_bw + (*u_iter)->ad_val.fp_frac_bw  << ",";
//  	  os << (*u_iter)->ad_val.fp_frac_bw << endl; 

	  os << (*u_iter)->var_name << "_INTBW=";
	  os << (*u_iter)->ad_val.fp_int_bw << endl;
	  os << (*u_iter)->var_name << "_FRACBW=";
	  os << (*u_iter)->ad_val.fp_frac_bw << endl; 

	  ss = ss + "-D" + (*u_iter)->var_name + "_BW=$(" + (*u_iter)->var_name + "_INTBW)+$(";
	  ss = ss + (*u_iter)->var_name + "_FRACBW),$(" + (*u_iter)->var_name + "_FRACBW) ";
	  
	}


  os << endl << ss << endl << endl;
}

void HWfix::output_results()
{
	fstream fs_out;

	fs_out.open( "makefile.bitdata" , ios::out );

	fs_out << "# Output bit-widths set at " << endl;
	
	for( varcollections<HWvar *>::iter_t itr = bs->varC.outvar_c.begin(); itr != bs->varC.outvar_c.end() ; itr++ )
	  {
	    fs_out << "# " << (*itr)->var_name << " : " << (*itr)->ad_val.fp_int_bw  + (*itr)->ad_val.fp_frac_bw << " : ";
	    fs_out <<  (*itr)->ad_val.fp_frac_bw << endl;
	  }

	
	//ad_val.print_flt_make_defs( fs_out );

	//	varcollections<HWvar *>::iter_t u_iter;	
	
	fs_out << "# Input VARs " << endl;
	print_bwdata( fs_out , bs->varC.invar_c.begin() , bs->varC.invar_c.end() );
	
	fs_out << "# Output VARs " << endl;
	print_bwdata( fs_out , bs->varC.outvar_c.begin() , bs->varC.outvar_c.end() );

	fs_out << "# Temp VARs " << endl;
	print_bwdata( fs_out , bs->varC.tmpvar_c.begin() , bs->varC.tmpvar_c.end() );


	fs_out.close();

}



void print_bwdata2(ostream &os , varcollections<HWvar *>::iter_t itr_begin ,  varcollections<HWvar *>::iter_t itr_end )
{

  varcollections<HWvar *>::iter_t _u_iter;

  HWfix *u_iter;

  for ( _u_iter = itr_begin ; _u_iter !=  itr_end; _u_iter++  ) 
	{
	  u_iter = static_cast<HWfix *>( *_u_iter );

	  if ( u_iter->var_dectype != HWvar::VECVAR ){
	    os << u_iter->var_name << " : ";
	    os << (( (int)u_iter->ad_val.fp_int_bw <= 0 )? 1 : (int)u_iter->ad_val.fp_int_bw   ) << " : ";
	    os << u_iter->ad_val.fp_frac_bw << " : ";
	    os << (u_iter->int_bw_state )   << " : ";
	    os << (u_iter->frac_bw_state )  << endl;
	  }
	 
	}


  
}

void HWfix::output_results2()
{

	fstream fs_out;

	fs_out.open( "bitsize.dat" , ios::out );
	

	fix_vecbitwidths();

	fs_out << "# Input VARs X" << endl;
	print_bwdata2( fs_out , bs->varC.invar_c.begin() , bs->varC.invar_c.end() );
	
	fs_out << "# Output VARs " << endl;
	print_bwdata2( fs_out , bs->varC.outvar_c.begin() , bs->varC.outvar_c.end() );

	fs_out << "# Temp VARs " << endl;
	print_bwdata2( fs_out , bs->varC.tmpvar_c.begin() , bs->varC.tmpvar_c.end() );


	
// 	vector<HWvector<HWfix>* >::iterator itr = vec_c.begin();

// 	for ( ; itr != vec_c.end() ; itr++ )
// 	  {
	   
// 	    fs_out << (*itr)->proto_type->var_name << " : ";
// 	    fs_out << (( (int)(*itr)->proto_type->ad_val.fp_int_bw <= 0 ) ? 1 :(int)(*itr)->proto_type->ad_val.fp_int_bw )  << " : ";
// 	    fs_out << (*itr)->proto_type->ad_val.fp_frac_bw << endl;
// 	  }

// 	fs_out.close();

	//bw_opt.list_vars();

}

string HWfix::print_dot_bitinfo()
{

  char int_bw[3];
  char frac_bw[3];

  sprintf( int_bw , "%.0f" , ad_val.fp_int_bw );
  sprintf( frac_bw , "%.0f" , ad_val.fp_frac_bw );

  string temp( int_bw );
  
  temp +=  ":";
  temp += frac_bw;

  return temp;


}

HWint operator>  (const HWfix &l,const HWfix &r)
{

  HWint temp;

  temp.var_val = (int)( l.ad_val.ref_value > r.ad_val.ref_value );

  return temp;



}

HWint operator>  (double val,const HWfix &r)
{

  HWint temp;

  temp.var_val = (int)( val > r.ad_val.ref_value );

  return temp; 


}



HWint operator>  (const HWfix &l,double val)
{
  HWint temp;

  temp.var_val = (int)( l.ad_val.ref_value > val );

  return temp;



}

HWint operator>  (int val,const HWfix &r)
{
  HWint temp;

  temp.var_val = ( val > (int)(r.ad_val.ref_value) );

  return temp;



}


HWint operator>  (const HWfix &l,int val)
{

  HWint temp;

  temp.var_val = ( (int)(l.ad_val.ref_value) > val );

  return temp;



}


HWfix operator>> (const HWfix &l,const HWint &r)
{
  HWfix temp;

  double mul_val = pow( 2. , static_cast<double>( -r.var_val ));

  temp.ad_val  = l.ad_val  *  mul_val;

  if ( bs->global_opmode == PARSING )
   {
     Edge* hwint_edge = bs->mydag->new_hwint_edge();
     Node* op_node = bs->mydag->add_op_node( SHIFTR , FIXED , l.myedge , hwint_edge  );
     temp.myedge =  bs->mydag->new_edge( NULL , "TEMP" );
     bs->mydag->add_out_edge( op_node , temp.myedge );

   }


  return temp;
}

HWfix operator>> (const HWfix &l,int val)
{

  HWfix temp;

  double mul_val = pow( 2. , static_cast<double>( -val ));

  temp.ad_val  = l.ad_val  *  mul_val;

  if ( bs->global_opmode == PARSING )
   {
        Edge* const_edge = bs->mydag->new_const_edge( val );
	Node* op_node = bs->mydag->add_op_node( SHIFTR , FIXED , l.myedge , const_edge );
	temp.myedge =  bs->mydag->new_edge(  NULL , "TEMP"  );
	bs->mydag->add_out_edge( op_node , temp.myedge );
     

    }


  return temp;
  
} 


HWfix operator<< (const HWfix &l,const HWint &r)
{
 

  HWfix temp;

  double mul_val = pow( 2. , static_cast<double>( r.var_val ));

  temp.ad_val  = l.ad_val  *  mul_val;

  if ( bs->global_opmode == PARSING )
   {
     Edge* hwint_edge = bs->mydag->new_hwint_edge();
     Node* op_node = bs->mydag->add_op_node( SHIFTL , FIXED , l.myedge , hwint_edge  );
     temp.myedge =  bs->mydag->new_edge( NULL , "TEMP" );
     bs->mydag->add_out_edge( op_node , temp.myedge );


   }

  return temp;


}


HWfix operator<< (const HWfix &l,int val)
{
  HWfix temp;

  double mul_val = pow( 2. , static_cast<double>( val ));

  temp.ad_val  = l.ad_val  * mul_val;

  if ( bs->global_opmode == PARSING )
   {
 
        Edge* const_edge = bs->mydag->new_const_edge( val );
	Node* op_node = bs->mydag->add_op_node( SHIFTL , FIXED , l.myedge , const_edge );
	temp.myedge =  bs->mydag->new_edge(  NULL , "TEMP"  );
	bs->mydag->add_out_edge( op_node , temp.myedge );
     

   }


  //temp.ad_val.ref_value = fmod( temp.ad_val.ref_value , (double)( l.int_bw  ));

  return temp;


}



HWint op_lod( const HWfix& var )
{

  int leading_one = (int)var.ad_val.fp_int_bw;

  double x = var.ad_val.ref_value * pow( 2. , -leading_one );

  int t = 0;

  while(( t == 0 ) && leading_one > 0 )
    {

      x *= 2;

      t = (int)floor( x );

      leading_one--;

    }


  HWint temp;

  temp.var_val = (int)pow( 2. , leading_one + 1);


  return temp;

  


}



 
