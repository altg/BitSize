// $Id: hwfloat.cpp,v 1.7 2004/12/29 13:51:39 amag97 Exp $
// $Author: amag97 $

#include "hwfloat.h"


 /*HWfloat::HWfloat(arch_attr Signal_type=DefaultArch, 
          fsize fullsize=DefaultSize, 
                   fsize msize=(DefaultSize>>2)*3,
          sign_attr sign=SIGNMAGNITUDE,char *name=NULL):HWfix(TMP,1,0){

  }*/





void HWfloat::init()
{

	advar::advar_setup( AD_AUTONUM );


}

void HWfloat::init(arch_attr Signal_type, fsize tsize, fsize msize, sign_attr sign, string name)
{

  if ( ( bs->mydag->arith_type == AT_NOTSET ) || ( bs->mydag->arith_type == FLOAT ) )
    bs->mydag->arith_type = FLOAT;
  else
    ERROR( name << " : Multiple arithmetic types not supported" )
  
  var_name = ( name == "" ) ? vnames.gen_varname() : name;
  
  //myedge->edge_name = var_name;

  // _DBOUT(  "HWfloat const called " << var_name )

 
    //var_number = var_count++;
  
  var_dectype = DECVAR;

   // DAG generation
       
  myedge = bs->mydag->new_edge( this , var_name);
      

  if ( Signal_type == IN )
    {

      Node* temp_node = bs->mydag->new_node( INPUT , FLOAT ); 
	  
      //src_nodes.push_back( temp_node_n );

      bs->mydag->add_out_edge( temp_node , myedge );

      myedge->edge_type = IN;

    }
  else if ( Signal_type == OUT )
    {

      Node* temp_node_n = bs->mydag->new_node( OUTPUT , FLOAT );
	  
      //dst_nodes.push_back( temp_node_n );
	  
      bs->mydag->add_in_edge( temp_node_n , myedge );
 
      myedge->edge_type = OUT;

    }

  // Checks for output variables
  if ( ( Signal_type == OUT ) && (msize == OPT_BW))
    ERROR( "Output Var :" <<  var_name <<  " must specify a bitwidth" )

  
      // Add the Signal to the correct collection
      add_varcollection( Signal_type );  

  if ( myedge->edge_type != OUT ){
    bs->mydag->edges_to_analyse.push_back( myedge->edge_number );
  }

  // Check and set variable bitwidth optimisation modes
  // 4 combinations are possible between TOTAL_BW and FRAC_BW
 
  if ( (tsize == OPT_BW ) && ( msize == OPT_BW ) )
    {
      exp_bw_state = NOTSET; exp_bw = 0;
      man_bw_state = NOTSET; man_bw = 0;

    } 
  else if( ( tsize >= OPT_BW ) && ( msize != OPT_BW ) )
    {
      exp_bw_state = NOTSET; exp_bw = 0;
      man_bw_state = SET; man_bw = msize;
    }
  else if( (tsize != OPT_BW ) && ( msize == OPT_BW ) )
    {
      man_bw_state = NOTSET; man_bw = 0;

      if ( tsize < OPT_BW )
	ERROR( "Var : " << var_name << " TotalSize should be specified as : OPT_BW + (integer part bitwidth )" )
	else	
	  { exp_bw_state = SET; exp_bw = tsize - OPT_BW;  }
    }
  else
    {
      exp_bw_state = SET; exp_bw = tsize - msize;
      man_bw_state = SET; man_bw = msize;
    }


  ad_val.var_name = var_name;
  ad_val <<= 0.0;

  //  ad_val.fp_frac_bw = static_cast<double>( frac_bw );
  //   ad_val.fp_int_bw = static_cast<double>( int_bw );

  //   ad_val.fix_frac_state = frac_bw_state;
  //   ad_val.fix_int_state = int_bw_state;

  c_sign = sign;
  var_type = Signal_type;
 
  // DBLOG( var_name << "FRAC : " << frac_bw )
  //  DBLOG( var_name << "INT : " << int_bw )


}



HWfloat::HWfloat()
{
  
    ad_val <<= 0.0;

    ad_val.var_name = tmpvnames.gen_varname();
    //cout << ad_val.var_name << endl;

}


HWfloat::HWfloat(arch_attr Signal_type, fsize tsize, R_ERR _r_err, sign_attr sign, string name)
{


  init( Signal_type ,  tsize,  23 , sign, name ); 

  rel_err = _r_err.rel_err;

  double exp_bw2 = static_cast<double>( exp_bw );
  
  double bias = pow( 2. , exp_bw2 - 1 ) - 1;

  double maxE = pow( 2. , exp_bw2 ) -2 - bias;



  myedge->e_min = -pow( 2. , maxE );
  myedge->e_max = pow( 2. , maxE );


}


HWfloat::HWfloat(arch_attr Signal_type, fsize tsize, fsize msize, sign_attr sign, string name)
{


  init( Signal_type ,  tsize,  msize, sign, name );  

// //HWfloat::HWfloat ( char* name , advarType vtype , double  vman_bw , double vexp_bw , double  vouterr_spec  ){
	
// 	ad_val <<= 0.0;

// 	if ( name == NULL )
// 	{
	      
// 	  ad_val.var_name = vnames.gen_varname();
// 	  //cout << ad_val.var_name << endl;
// 	}
// 	else
// 	{
// 	  ad_val.var_name = name;
// 	}

	

	
// 	ad_val.m_bitwidth = ( msize == OPT_BW ) ? 0 : msize;
// 	ad_val.e_bitwidth = ( tsize == OPT_BW ) ? 0 : tsize - msize;
 


// 	// switch( Signal_type )
// // 	  {
// // 	  case IN: varC.invar_c.push_back( this ); ad_val.varType = advar::IN; break;

// // 	  case OUT: varC.outvar_c.push_back( this ); ad_val.varType = advar::OUT; break;

// // 	  case TMP: varC.tmpvar_c.push_back( this ); ad_val.varType = advar::TMP; break;

// // 	  default: break;
// // 	  }

// 	add_varcollection( Signal_type );


// 	// if ( Signal_type == OUT  )
// // 	  {
// // 	    ad_val.BWState = SET;

// //           }


// 	if ( msize != OPT_BW )
// 	  {

// 	    ad_val.BWState = SET;
	    
// 	  }
// 	//	cout << varC.invar_c.size() << endl;


  double exp_bw2 = static_cast<double>( exp_bw );
  
  double bias = pow( 2. , exp_bw2 - 1 ) - 1;

  //double maxE = pow( 2. , exp_bw2 ) -2 - bias;
  double maxE = pow( 2. , exp_bw2 ) - bias;


  myedge->e_min = -pow( 2. , maxE );
  myedge->e_max = pow( 2. , maxE );

  //_DBOUT( man_bw << " :: " << maxE )  

}


HWfloat::~HWfloat()
{


}

HWfloat::HWfloat ( const HWfloat &a ):HWvar(a){
	
  //	ad_val = a.ad_val;

}

double HWfloat::get_val() {

	return static_cast<double>(ad_val);

 }
 
HWfloat& HWfloat::operator= ( const HWfloat &r){

  //ad_val = r.ad_val;

   if ( this != &r )
    { 
      HWvar::operator=( r );

      if ( man_bw_state == NOTSET )
	man_bw = r.man_bw;
      
      if ( exp_bw_state == NOTSET )
	exp_bw = r.exp_bw;

      c_sign = r.c_sign;

       if ( bs->global_opmode == PARSING )
       {

	 if ( var_dectype == DECVAR )
	  bs->mydag->add_out_edge( bs->mydag->node_vec[ r.myedge->src_nodes.back() ] , myedge );


	 if ( r.myedge->edge_type == IN ){
	   
	   r.myedge->next_edge = myedge; 


	 }


	    
       }


    }


	return *this;

 }

 
HWfloat& HWfloat::operator= (double val) {

  //ad_val = val;

    ad_val <<= val;

// 	if( ad_val.fix_frac_state != SET )
// 	  ad_val.fix_frac_state = NOTSET;
// 	if( ad_val.fix_int_state != SET )
// 	  ad_val.fix_int_state = NOTSET;


	if ( man_bw_state != SET )
	  man_bw_state = NOTSET;
	if( exp_bw_state != SET )
	  exp_bw_state = NOTSET;
	

	if ( bs->global_opmode == PARSING )
	  {
	    Node* const_node = bs->mydag->new_const_node( FLOAT , val  );
	    bs->mydag->add_out_edge( const_node , myedge );
	    
	  }


// 	myedge->e_max = val;
	
// 	myedge->X0 = (myedge->e_max);
//         myedge->Xk = (myedge->e_max);

	
	return *this;
 }


HWfloat& HWfloat::operator<<= (double val){

    ad_val <<= val;
    return *this;


}




void HWfloat::analyse_bw( int bw , int mode )
{

	 	

 //   double m_err = pow( 2.  , -bw );

	//double ref_value = 1.;//static_cast<double>(ad_val);

	//double exp_value = ( ref_value != 0. ? _logb( ref_value ) : 0. );

 //   double out_err = m_err * pow( 2. , exp_value );

    ad_val.analyse_bw_flt_mbw( bw , mode);

	


}



void HWfloat::output_results()
{
	fstream fs_out;

	fs_out.open( "makefile.bitdata" , ios::out );

	fs_out << "# Output bit-widths set at " << endl;
	
	for( varcollections<HWvar *>::iter_t itr = bs->varC.outvar_c.begin(); itr != bs->varC.outvar_c.end() ; itr++ )
	  {
	    fs_out << "# " << (*itr)->ad_val.var_name << " : " << (*itr)->ad_val.m_bitwidth + (*itr)->ad_val.e_bitwidth << " : ";
	    fs_out <<  (*itr)->ad_val.e_bitwidth << endl;
	  }

	double e_bw = 5;

	//ad_val.print_flt_make_defs( fs_out );

	varcollections<HWvar *>::iter_t u_iter;	

	for ( u_iter = bs->varC.invar_c.begin() ; u_iter != bs->varC.invar_c.end() ; u_iter++  ) 
	{

	  fs_out << endl << "BITSIZE_DATA+= ";
 	  fs_out << "-D" << (*u_iter)->ad_val.var_name << "_BW=";
	
	 //  fs_out << (*u_iter)->ad_val.m_bitwidth + (*u_iter)->ad_val.e_bitwidth + 
// 	    ((*u_iter)->ad_val.e_bitwidth > 3. ? 0 : 3. - (*u_iter)->ad_val.e_bitwidth)  << ",";
	  fs_out << (*u_iter)->ad_val.m_bitwidth + e_bw << ","; 

	  fs_out << (*u_iter)->ad_val.m_bitwidth << endl; 
	  
	}

	for ( u_iter = bs->varC.outvar_c.begin() ; u_iter != bs->varC.outvar_c.end() ; u_iter++  ) 
	{

	  fs_out << endl << "BITSIZE_DATA+= ";
 	  fs_out << "-D" << (*u_iter)->ad_val.var_name << "_BW=";
	  
	 //  fs_out << (*u_iter)->ad_val.m_bitwidth + (*u_iter)->ad_val.e_bitwidth + 
// 	    ((*u_iter)->ad_val.e_bitwidth > 3. ? 0 : 3. - (*u_iter)->ad_val.e_bitwidth)  << ",";
	   fs_out << (*u_iter)->ad_val.m_bitwidth + e_bw << ","; 

	  fs_out << (*u_iter)->ad_val.m_bitwidth << endl; 
	  
	}

	u_iter = bs->varC.tmpvar_c.begin();
	bs->varC.tmpvar_c.size();
	while(  u_iter != bs->varC.tmpvar_c.end()  ) 
	{

	  fs_out << endl << "BITSIZE_DATA+= ";
          fs_out << "-D" << (*u_iter)->ad_val.var_name << "_BW=";
	  
	 //  fs_out << (*u_iter)->ad_val.m_bitwidth + (*u_iter)->ad_val.e_bitwidth + 
// 	    ((*u_iter)->ad_val.e_bitwidth > 3. ? 0 : 3. - (*u_iter)->ad_val.e_bitwidth)  << ",";
	   fs_out << (*u_iter)->ad_val.m_bitwidth + e_bw << ","; 
	  
	  fs_out << (*u_iter)->ad_val.m_bitwidth << endl;
	  u_iter++;
	}
	
	


	fs_out.close();

}

void HWfloat::print_advar()
{

	advar::BS.print_all();

	advar::BS.show_lists();
	//cout << ad_val << endl;

}


// Arithmatic operators 

HWfloat operator+ (const HWfloat &l,const HWfloat &r)  { 
	
	HWfloat temp;

	temp.ad_val = l.ad_val + r.ad_val;

	 if ( bs->global_opmode == PARSING )
	  {
	    Node* op_node = bs->mydag->add_op_node( ADD , FLOAT , l.myedge , r.myedge  );
	    temp.myedge =  bs->mydag->new_edge( NULL , "TEMP" );
	    bs->mydag->add_out_edge( op_node , temp.myedge );

	  }



	return temp;

}

HWfloat operator+  (const double dval,const HWfloat &r)  { 

	HWfloat temp;

	temp.ad_val = dval + r.ad_val;

	 if ( bs->global_opmode == PARSING )
	  {
	    
	    Edge* const_edge = bs->mydag->new_const_edge( dval );
	    Node* op_node = bs->mydag->add_op_node( ADD ,  FLOAT ,const_edge , r.myedge  );
	    temp.myedge =  bs->mydag->new_edge( NULL , "TEMP" );
	    bs->mydag->add_out_edge( op_node , temp.myedge );
	    
	  }



	return temp;


}
HWfloat operator+  (const HWfloat &l, const double dval)  { 

	HWfloat temp;

	temp.ad_val = l.ad_val + dval;

	 if ( bs->global_opmode == PARSING )
	  {
	    Edge* const_edge = bs->mydag->new_const_edge( dval );
	    Node* op_node = bs->mydag->add_op_node( ADD , FLOAT , l.myedge , const_edge );
	    temp.myedge =  bs->mydag->new_edge(  NULL , "TEMP"  );
	    bs->mydag->add_out_edge( op_node , temp.myedge );

          }



	return temp;


}



HWfloat operator-  (const HWfloat &l, const HWfloat &r)  { 
	
	HWfloat temp;

	temp.ad_val = l.ad_val - r.ad_val;

		if ( bs->global_opmode == PARSING )
	   {	    
	     Node* op_node = bs->mydag->add_op_node( SUB , FLOAT , l.myedge , r.myedge  );
	     temp.myedge =  bs->mydag->new_edge( NULL , "TEMP" );
	     bs->mydag->add_out_edge( op_node , temp.myedge );

	   }




	return temp;


}


HWfloat operator-  (const double dval, const  HWfloat &r)  { 

	HWfloat temp;

	temp.ad_val = dval - r.ad_val;

	if ( bs->global_opmode == PARSING )
	  {
	    	    
	    Edge* const_edge = bs->mydag->new_const_edge( dval );
	    Node* op_node = bs->mydag->add_op_node( SUB , FLOAT , const_edge , r.myedge  );
	    temp.myedge =  bs->mydag->new_edge( NULL , "TEMP" );
	    bs->mydag->add_out_edge( op_node , temp.myedge );

	  }
	

	return temp;

}

HWfloat operator-  (const HWfloat &l, const double dval)  { 

	HWfloat temp;

	temp.ad_val = l.ad_val - dval;

	  if ( bs->global_opmode == PARSING )
	  {
	    Edge* const_edge = bs->mydag->new_const_edge( dval );
	    Node* op_node = bs->mydag->add_op_node( SUB , FLOAT , l.myedge , const_edge );
	    temp.myedge =  bs->mydag->new_edge(  NULL , "TEMP"  );
	    bs->mydag->add_out_edge( op_node , temp.myedge );

	  }

	return temp;

}


HWfloat operator*  (const HWfloat &l, const HWfloat &r)  {

	HWfloat temp;

	temp.ad_val = l.ad_val * r.ad_val;

	if ( bs->global_opmode == PARSING )
	  {
	    Node* op_node = bs->mydag->add_op_node( MUL , FLOAT , l.myedge , r.myedge  );
	    temp.myedge =  bs->mydag->new_edge( NULL , "TEMP" );
	    bs->mydag->add_out_edge( op_node , temp.myedge );

	  }


	return temp;

}

HWfloat operator*  ( const double dval, const HWfloat &r)  { 

	HWfloat temp;

	temp.ad_val = dval * r.ad_val;

	if ( bs->global_opmode == PARSING )
	  {
	    Edge* const_edge = bs->mydag->new_const_edge( dval );
	    Node* op_node = bs->mydag->add_op_node( MUL , FLOAT , const_edge , r.myedge  );
	    temp.myedge =  bs->mydag->new_edge( NULL , "TEMP" );
	    bs->mydag->add_out_edge( op_node , temp.myedge );
	    
	  }	

	return temp;


}


HWfloat operator*  (const HWfloat &l, const double dval)  { 

    HWfloat temp;

	temp.ad_val = l.ad_val * dval;

	if ( bs->global_opmode == PARSING )
	  {
	    Edge* const_edge = bs->mydag->new_const_edge( dval );
	    Node* op_node = bs->mydag->add_op_node( MUL , FLOAT , l.myedge , const_edge );
	    temp.myedge =  bs->mydag->new_edge(  NULL , "TEMP"  );
	    bs->mydag->add_out_edge( op_node , temp.myedge );

	  }


	return temp;
  

}


HWfloat operator/  (const HWfloat &l, const HWfloat &r)  {

	HWfloat temp;

	temp.ad_val = l.ad_val / r.ad_val;

	if ( bs->global_opmode == PARSING )
	  {
	    Node* op_node = bs->mydag->add_op_node( DIV , FLOAT , l.myedge , r.myedge  );
	    temp.myedge =  bs->mydag->new_edge( NULL , "TEMP" );
	    bs->mydag->add_out_edge( op_node , temp.myedge );

	  }


	return temp;

}

HWfloat operator/  ( const double dval, const HWfloat &r)  { 

	HWfloat temp;

	temp.ad_val = dval / r.ad_val;

	if ( bs->global_opmode == PARSING )
	  {
	    Edge* const_edge = bs->mydag->new_const_edge( dval );
	    Node* op_node = bs->mydag->add_op_node( DIV , FLOAT , const_edge , r.myedge  );
	    temp.myedge =  bs->mydag->new_edge( NULL , "TEMP" );
	    bs->mydag->add_out_edge( op_node , temp.myedge );
	    
	  }	

	return temp;


}


HWfloat operator/  (const HWfloat &l, const double dval)  { 

    HWfloat temp;

	temp.ad_val = l.ad_val / dval;

	if ( bs->global_opmode == PARSING )
	  {
	    Edge* const_edge = bs->mydag->new_const_edge( dval );
	    Node* op_node = bs->mydag->add_op_node( DIV , FLOAT , l.myedge , const_edge );
	    temp.myedge =  bs->mydag->new_edge(  NULL , "TEMP"  );
	    bs->mydag->add_out_edge( op_node , temp.myedge );

	  }


	return temp;
  

}


HWfloat operator<< (HWfloat &l, double &r ) {


	HWfloat temp;

	temp.ad_val = l.ad_val * pow( 2. , r );

	return temp;

}

void HWfloat::read_inputdata( double dbval )
{

  ad_val = dbval;

}





void HWfloat::output_analysis( int mode )
{

  varcollections<HWvar *>::iter_t itr = bs->varC.outvar_c.begin();


  while( itr != bs->varC.outvar_c.end() )
    {

      (*itr++)->ad_val.analyse_bw_flt_mbw( mode );
    }


}
