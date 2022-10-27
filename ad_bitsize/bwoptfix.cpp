// $Author: amag97 $
// $Id: bwoptfix.cpp,v 1.12 2004/10/29 17:42:26 amag97 Exp $

#define  _BITSIZE_DEBUG

#include "bwoptfix.h"
#include <sys/time.h>

BWOPTfix::BWOPTfix()
{

  // Default constructor

  current_method = 3;

}


void BWOPTfix::find_bw( HWfix *var , int vnumber ) 
{

  var_number = vnumber;

   c_var = var;

   _DBOUT( "Frac_BW :: " << ( static_cast<int>(c_var->frac_bw) ) )

   output_error = pow( 2.  , -( static_cast<int>(c_var->frac_bw) )  );

   //   setup_max_grads();

   find_frac_bw();

   find_int_bw();
 

}











   
#include<cassert>



void BWOPTfix::find_frac_bw()
{


  double out_err = pow( 2.  ,-( static_cast<int>(c_var->frac_bw) )  );

  
  //cout << bs->system_name << endl;

  current_method = bs->opt_mode;


  //  ASSERT( 1 > 2 )

  switch( current_method )
    {
    case 1: frac_method1();break;

    case 2: frac_method2();break;      

    case 3: frac_method3( out_err );break; 

    case 4: frac_method4( out_err );break;

    case 5: frac_method5( c_var->myedge->edge_number , out_err );break;

    case 6: frac_method6( out_err );break;

    default: break;


    }

  //  find_output_err2(  );
  find_int_bw();

}


void BWOPTfix::frac_method1()
{

  c_var->ad_val.analyse_bw_fx_fracbw( 1 );
}
 


void BWOPTfix::frac_method2()
{



}


//Fractional bitwidth reduction by
 //1. Reduce the bitwidth of one node while 
 //   keeping all other nodes constant
 //2. Repeat this for all nodes
 //3. Set node fraction to bitwidth found
 //4. Increase the bitwidth of each node by one bit 
 //   until error requirement is met


void BWOPTfix::frac_method3(double out_err)
{
  advar::iter_t g_itr;
  advar::iter_t g_end = c_var->ad_val.grads.end();

	
  DTYPE max_err = 0.;

  bool done = false;

  vector< int > bw_table;
  vector< int > min_bw_table;

  DAG *c_dag = bs->mydag;


	
  BSClass::advar_veclist::iterator var_itr;
  BSClass::advar_veclist::iterator var_end = advar::BS.advar_vec.end();

  // Set intial starting bitsize

  int start_size = 50;


  //fsize no_of_vars = c_dag->edge_vec.size();
  fsize no_of_vars = 0;

  for( size_t i = 0 ; i < c_dag->edge_vec.size() ; i++ )
    {

      HWfix* t_var = dynamic_cast<HWfix*>(c_dag->edge_vec[ i ]->my_var);

      if( t_var != NULL ){

	no_of_vars++;

	if ( t_var->frac_bw_state == NOTSET ){
	  bw_table.push_back( start_size );
	  min_bw_table.push_back( start_size );
	}
	else 
	  {
	    //  bw_table.push_back( (int)advar::BS.advar_vec[ i ]->fp_frac_bw  );
	    // 	      min_bw_table.push_back(  (int)advar::BS.advar_vec[ i ]->fp_frac_bw  );
	    bw_table.push_back( start_size );
	    min_bw_table.push_back( start_size );
	      
	  }

      }
    }

  vector<int>::iterator bwt_itr;
  vector<int>::iterator bwt_end = bw_table.end();
	
  vector<int>::iterator min_bwt_itr = min_bw_table.begin();
	//list<int>::iterator min_bwt_end = min_bw_table.end();


  double c_err = 0., p_err = 0.;

	
  

  // Reduce bitwidth of a node while keeping all other nodes constant
	
  bwt_itr = bw_table.begin();

  //p_err = c_err;

  
  for( int i = 0 ; i < no_of_vars ; i++ )
    {
      c_err = find_output_err( min_bw_table.begin() , min_bw_table.end() );
		
      //cout << c_err << " : " << bit_err << " : " << c_err - bit_err << endl;
      while(( c_err < out_err ) && ( (*min_bwt_itr) > 1 ) )
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

      //cout << "Here " <<advar::BS.advar_vec[ i ]->var_number << endl;
      (*min_bwt_itr) -= 1;
      c_err = find_output_err( min_bw_table.begin() , min_bw_table.end() );

      //   cout << c_err << " " << out_err << " " << (*min_bwt_itr) << endl;	
			

    }

    (*bwt_itr++) = (*min_bwt_itr);

		
    (*min_bwt_itr++) = start_size;

		

}

	//cout << "done 1" << endl;

	// Increase the bitwidths until the error spec is met
        // Uniform increase

p_err = c_err = 0.;

done = false;	

int add_iter = 0;

int sub_iter = 0;

int mul_iter = 0;


int input_iter = 0;


int l1_itr = 0;
int l2_itr = 0;

int l1_itr_max = 2;

 
vector<dag_index>::iterator edg_itr;

while( !done ) 
  {

    c_err = find_output_err( bw_table.begin() , bw_table.end() );
    
    
	
    if ( c_err < out_err )
      {
	done = true;
	break;
      }
    else
      {



	while( l1_itr < l1_itr_max )
	  {


	    for( edg_itr = c_dag->input_edges.begin(); edg_itr != c_dag->input_edges.end() ; edg_itr++ )
	      {
		bw_table[  (*edg_itr) ] += 1;
		//	_DBOUT( "Input inc"  );
	      }

	    for( edg_itr = c_dag->add_edges.begin(); edg_itr != c_dag->add_edges.end() ; edg_itr++ )
	      {
		bw_table[  (*edg_itr) ] += 1;
		//	_DBOUT( "Add inc" << bw_table[  (*edg_itr) ]);
	      }

	    for( edg_itr = c_dag->sub_edges.begin(); edg_itr != c_dag->sub_edges.end() ; edg_itr++ )
	      {
		bw_table[  (*edg_itr) ] += 1;
		//_DBOUT( "Sub inc" );
	      }

		      
	    l1_itr++;
	  }




	if ( l1_itr == l1_itr_max )
	  {

	    //  add_iter = 0;
	    // 		      sub_iter = 0;
	    // 		      input_iter = 0;

	    l1_itr = 0;
		     
	    for( edg_itr = c_dag->mul_edges.begin(); edg_itr != c_dag->mul_edges.end() ; edg_itr++ )
	      {
		bw_table[  (*edg_itr) ] += 1;
		//_DBOUT(  "Mul inc" << bw_table[  (*edg_itr) ]);
	      }

	  }

      }
	
  }



	// Update bitwidth info
	
        int ctr;

        HWfix *tempvar;
	for( ctr = 0, bwt_itr = bw_table.begin() ; ctr < no_of_vars ; ctr++ , bwt_itr++)
	{

	 if( advar::BS.advar_vec[ctr]->fix_frac_state == NOTSET )
	  advar::BS.advar_vec[ctr]->fp_frac_bw  = MAX( advar::BS.advar_vec[ctr]->fp_frac_bw  , (*bwt_itr) );
	  
	  

	  tempvar = dynamic_cast<HWfix *>( ( c_dag->edge_vec[ ctr ])->my_var);
					   
	  if( tempvar != NULL ){
	  if( tempvar->frac_bw_state == NOTSET ) {
	   tempvar->frac_bw = max( tempvar->frac_bw , static_cast<fsize>((*bwt_itr)) );
 	  }
	  }
	}


}

//Fractional bitwidth reduction by
// 1. Start with maximum fractional bitwidth for all nodes
// 2. Remove 1 bit from all the nodes while all the other 
//       nodes remain fixed
// 3. The node for which the bitwidth is minimised is allowed
//       to keep the new bitwidth
// 4. Repeat for until the error spec. is met.*/



void BWOPTfix::frac_method4( double out_err )
{

  double maximum_err;
  int no_of_vars = static_cast<int>( advar::BS.advar_vec.size() );

  vector< int > bw_table_1, min_bw_table_1;
  vector< int > bw_table, min_bw_table;

  


  for( int i = 0 ; i < no_of_vars ; i++ )
	{
	  bw_table_1.push_back( ( advar::BS.advar_vec[i]->fp_frac_bw != 0. ?  (int)advar::BS.advar_vec[i]->fp_frac_bw :0)  );
	  min_bw_table_1.push_back( ( advar::BS.advar_vec[i]->fp_frac_bw != 0. ?  (int)advar::BS.advar_vec[i]->fp_frac_bw :0 ) );
	  // bw_table_1.push_back( ( advar::BS.advar_vec[i]->fp_frac_bw)  );
	  //min_bw_table_1.push_back( ( advar::BS.advar_vec[i]->fp_frac_bw ) );
	 
	}

 double bias = find_output_err( bw_table_1.begin() , bw_table_1.end() );

 


 //cout << "B : " << bias << endl;

 for( int i = 0 ; i < no_of_vars ; i++ )
	{
	  
	  bw_table.push_back( 30  );
	  min_bw_table.push_back( 30 );

	}

 vector<int>::iterator bwt_itr, min_bwt_itr;
 vector<int>::iterator bwt_end = bw_table.end();
 vector<int>::iterator min_bwt_end = min_bw_table.end();

 maximum_err = find_output_err( bw_table.begin() , bw_table.end() );

  
 vector<int> bw_table_test;

 HWfix *temp_var;

 vector<Edge *>::iterator e_itr2;

 //cout << bs->mydag->edge_vec.size() << endl;

 //for ( e_itr2 = bs->mydag->edge_vec.begin() ; e_itr2 !=  bs->mydag->edge_vec.end() ; e_itr2++ );

 int ctr_x;
 //for( ctr_x = 0 ; ctr_x < bs->mydag->edge_vec.size() ; ctr_x++ )

   
 for ( e_itr2 = bs->mydag->edge_vec.begin() ; e_itr2 !=  bs->mydag->edge_vec.end() ; e_itr2++ )
 {

  
   if ( temp_var = dynamic_cast<HWfix *>( (*e_itr2)->my_var ) ){
     // cout << temp_var->frac_bw << endl;
	 bw_table_test.push_back( temp_var->frac_bw );
	 
   }
  
 }



 //double err2 = find_output_err( bw_table_test );



 //cout << ":: " << maximum_err << " :: " << err2 << endl; 


 double c_err = maximum_err;

 int ctr;
while( maximum_err < out_err )
{
	bwt_itr = bw_table.begin();
	min_bwt_itr = min_bw_table.begin();

	maximum_err = 0.;
	for( ctr = 0;  ctr < no_of_vars ; ctr++ , bwt_itr++ , min_bwt_itr++)
	{
		(*bwt_itr) -= 1;
		c_err = find_output_err( bw_table.begin() , bw_table.end() );
		
		maximum_err = max( c_err , maximum_err );

		if (( c_err < out_err )  )
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
  if( advar::BS.advar_vec[ctr]->fix_frac_state == NOTSET )
	advar::BS.advar_vec[ctr]->fp_frac_bw  = MAX( advar::BS.advar_vec[ctr]->fp_frac_bw  , (*min_bwt_itr) );
      
}


}


void BWOPTfix::find_int_bw()
{
  
  //c_var->ad_val.analyse_fx_int_bw();

        
  	BSClass::advar_veclist::iterator u_iter;

	DTYPE log2_MSB;

	for ( u_iter = advar::BS.advar_vec.begin() ; u_iter != advar::BS.advar_vec.end() ; u_iter++ )
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



// Calculate the output error at the output, using the gradients
// for Fixed-Point
// Equivalent to runsim in ( modelsim bitsim )
// Input : List of bitwidths
// Output : Output error estimate
double BWOPTfix::find_output_err(vector<int>::iterator bwt_itr , vector<int>::iterator bwt_end)
{

	double c_err = 0.;

	//list<int>::iterator bwt_itr;

	advar::iter_t g_itr, g_end;

	g_end = c_var->ad_val.grads.end();

	int ctr = 0;

	double exp;	

	advar *temp_var;
	for( g_itr = c_var->ad_val.grads.begin(); g_itr != g_end ; bwt_itr++ , g_itr++ , ctr++ )
	{

	  temp_var = advar::BS.advar_vec[ctr];

	  //cout << advar::BS.advar_vec[ctr]->var_number << endl;

		//exp = _logb( advar_vec[ctr++]->ref_value );
	  if( advar::BS.advar_vec[ctr]->fix_frac_state == NOTSET )
	    c_err += fabs((*g_itr)) * pow( 2. , -(*bwt_itr) ); 
	}

	return fabs(c_err);

}


double BWOPTfix::find_output_err(vector<int> &bw_table)
{

  double c_err = 0.;
 

  INTERNAL_ASSERT(  bs->mydag->edge_vec.size() <= bw_table.size() , "bw_table size too large") 

  vector<int>::iterator bwt_itr = bw_table.begin();

  vector<Edge *>::iterator edge_itr = bs->mydag->edge_vec.begin();
   
  HWfix *temp;

  double delta_y = 0.;

  for( ; bwt_itr != bw_table.end() ; bwt_itr++ , edge_itr++ ){

    if ( temp = dynamic_cast<HWfix *>( (*edge_itr)->my_var ) ){

      // if ( temp->frac_bw_state == NOTSET )

      cout << delta_y << " : " <<(*edge_itr)->grad_vec[ var_number ] << " : " << (*bwt_itr) << endl;
	delta_y += (*edge_itr)->grad_vec[ var_number ] * pow( 2. , -(*bwt_itr) );
      

    }
    
   

  }

 return fabs( delta_y );

 

}


//Fractional bitwidth reduction by
 //1. Propagting output error up the DFG 
 //2. At each node use the gradients calculated at the
 //   node to propagate the error down to the input nodes
 //3. Set the error at the node to the calculated value

void BWOPTfix::frac_method5( dag_index start  , double m_err)
{

  DAG *c_dag = bs->mydag;

  Edge *c_edge = c_dag->edge_vec[ start ];

  Node *src_node = c_dag->node_vec[ c_edge->get_src_node() ];

  Node_OPTYPE src_ntype = src_node->node_type;

  if( ( src_ntype  == CONSTANT ) || ( src_ntype == INPUT ) )
    {

      if ( src_ntype == INPUT ){

	//c_edge->f_bw = dynamic_cast<HWfix *>( c_edge->my_var)->frac_bw;

	c_edge->f_bw = max( c_edge->f_bw , -ceil(  log( abs( m_err ) ) / log( 2.) ) );

      }

      cout << c_edge->edge_name << " : " << m_err << endl;



    }
    else{

      
      double c_err;

      for( vector<dag_index>::iterator e_iter = src_node->in_edges.begin() ; e_iter != src_node->in_edges.end(); e_iter++ )
	{

	  
	  if ( c_dag->node_vec[ c_edge->get_dst_node() ]->node_type == OUTPUT ){
	    c_err = m_err;

	  }
	  else{
	    c_err = m_err / c_edge->my_var->ad_val.grads[ (*e_iter) ];
	   	  
	  }

	  frac_method5( *(e_iter) ,  c_err  );

	  //c_edge->f_bw = max( c_edge->f_bw , (int)ceil( log( abs( m_err ) ) / log( 2. )) );

	  

	}
  
      c_edge->f_bw = max( c_edge->f_bw , -ceil(  log( abs( m_err ) ) / log( 2.) ) );
      //c_edge->f_bw = max( c_edge->f_bw , m_err);
      

      //cout << c_edge->edge_number << " : " <<  c_edge->edge_name << " : " << ilogb(m_err) << " : " << c_err << " : " <<  c_edge->f_bw <<endl;

    }



}


 /* 1. Start with maximum mantissa bitwidth for all nodes
    2. Remove 1 bit from all the nodes while all the other 
       nodes remain fixed
    3. The node for which the bitwidth is minimised is allowed
       to keep the new bitwidth
    4. Repeat for until the error spec. is met.*/

double find_ouput_err5(vector< int > &bw_vector , vector< double > & grad_vector)
{
     vector< int >::iterator bw_itr = bw_vector.begin();
     vector< double >::iterator g_itr = grad_vector.begin();
     
     double c_err = 0.;

     for ( ; bw_itr != bw_vector.end() ; bw_itr++ , g_itr++ )
       {
	 
	 c_err += fabs((*g_itr)) * pow( 2. , -(*bw_itr) );


       } 


     return fabs(c_err);

}



// Fraction optimisation method 6
// 

void BWOPTfix::frac_method6( double out_err )
{
  DAG *c_dag = bs->mydag;
  
  //vector<Edge *>::iterator e_itr;
  vector<dag_index>::iterator e_itr;

  vector<HWfix *> vars_to_consider;

  HWfix *t_var;
  
  for( e_itr = c_dag->edges_to_analyse.begin() ;e_itr != c_dag->edges_to_analyse.end() ; e_itr++ ){

   
	      
    if (  c_dag->edge_vec[(*e_itr)]->edge_type != ECONSTANT ){

      t_var =  dynamic_cast<HWfix *>( c_dag->edge_vec[ *e_itr ]->my_var );

      if ( t_var->frac_bw_state == NOTSET )
	{
	  vars_to_consider.push_back(t_var);
	}
    }
        
  }
  

 
 
  vector< bw_vec_type > new_select_bw_vec;
  bw_vec_type bw_vector;
  vector< bw_vec_type > select_bw_vec;

  typedef vector< double > grad_vector_type;
  grad_vector_type grad_vector;
  grad_vector_type  max_grad_vector;
  // Set all the bitwidths to the starting bitwidth

 

  int start_bw = 40;

  for( vector<HWfix *>::iterator v_iter = vars_to_consider.begin() ; v_iter != vars_to_consider.end(); v_iter++ )
    {
      //if ( (*v_iter)->frac_bw < 100 ){

      grad_vector.push_back( c_var->ad_val.grads[ (*v_iter)->ad_val.var_number ] );
      max_grad_vector.push_back(  c_var->ad_val.max_grads[ (*v_iter)->ad_val.var_number ] );
      //c_var->ad_val.print_grads_vec();
      
      _DBOUT( (*v_iter)->var_name ) 
      _DBOUT( (*v_iter)->ad_val.var_number << " # " << grad_vector.back() << " # " << max_grad_vector.back() )
      
      if ( (*v_iter)->frac_bw_state == NOTSET )
	{  bw_vector.push_back( start_bw );
	 	 	  
	}
      else{

	bw_vector.push_back( (*v_iter)->frac_bw );
      }
      //}
    }

  COST_RETURN _cost_return;

 
  double curr_cost = 0;
  double prev_cost = 0;

  

#define grad_vector max_grad_vector

#define USE_LINEAR_SEARCH

#ifdef USE_LINEAR_SEARCH
  cout << "Using Linear Search" << endl;
  find_uniform_bw_linear( bw_vector , grad_vector );
#else
  cout << "Using Binary Search" << endl;
  find_uniform_bw_binary( bw_vector , grad_vector , 40 , 0 );

#endif
  
  
  show_bw_vector( bw_vector );
 
  select_bw_vec.push_back( bw_vector );
 

  _cost_return = cost_function( bw_vector , grad_vector , vars_to_consider );     
  prev_cost = _cost_return.cost_value;
  cerr << "Uniform Cost " << prev_cost << endl;
  // Iterative cost function minimiser

#define MAX_TRIES 2
  for(  int trys = 0; trys < MAX_TRIES ; trys++ ){

    curr_cost = try_selected_vectors( select_bw_vec , grad_vector ,  vars_to_consider , new_select_bw_vec , prev_cost ); 

    cerr << "Tried  " << select_bw_vec.size() << " New " <<  new_select_bw_vec.size()<< " Min Cost " << curr_cost << endl;

    prev_cost = curr_cost;


    if( new_select_bw_vec.size() > 0 ){
    select_bw_vec.assign( new_select_bw_vec.begin() , new_select_bw_vec.end()  );
    //select_bw_vec.assign( new_select_bw_vec.end() - 1 , new_select_bw_vec.end()  );
    }
    
    show_bw_vector( select_bw_vec );

  }
   
  vector<int> best_bw_vector;
  
  curr_cost = find_best_bw_vector(  select_bw_vec , grad_vector , vars_to_consider , best_bw_vector );

  show_bw_vector( best_bw_vector ); 
  
  cout << "Minimised Cost " << curr_cost << endl;

  //#undef grad_vector


}


int BWOPTfix::find_uniform_bw_linear( vector<int>& bw_vector , vector<double> grad_vector )
{

  bool accept_flag = true;
  int i_ctr = 0;
  

 while( accept_flag ){

    for( vector<int>::iterator bw_itr = bw_vector.begin(); bw_itr != bw_vector.end(); bw_itr++ )
      (*bw_itr) -= 1;
    //for_each( bw_vector.begin(), bw_vector.end(), -- ); 

    //_cost_return = cost_function( bw_vector , grad_vector , vars_to_consider );
     
     double c_err = find_error( bw_vector , grad_vector );

     if ( c_err > output_error ){
       accept_flag = false;
     }

        
    i_ctr++;

  }
 
  inc_bw_vec( bw_vector , 1);

  return i_ctr;

}

void BWOPTfix::find_uniform_bw_binary( vector<int>& bw_vector , vector<double> grad_vector , int upper_bw , int lower_bw )
{

  int middle_bw = ( upper_bw + lower_bw ) /2;

  
  if ( middle_bw > lower_bw )
    {
    
      for( vector<int>::iterator bw_itr = bw_vector.begin() ; bw_itr != bw_vector.end() ; bw_itr++ )
	(*bw_itr) = middle_bw;

      double c_err = find_error( bw_vector , grad_vector );

      if ( c_err > output_error )
	lower_bw = middle_bw;
      else
	upper_bw = middle_bw;

      find_uniform_bw_binary( bw_vector , grad_vector , upper_bw , lower_bw );


    }
  else{
    //inc_bw_vec( bw_vector , 1 );

  }

}

void BWOPTfix::inc_bw_vec( vector<int>& bw_vector , int inc ){

   for( vector<int>::iterator bw_itr = bw_vector.begin(); bw_itr != bw_vector.end(); bw_itr++ )
    {

      (*bw_itr) += inc;
       
       
    }

}

void BWOPTfix::show_bw_vector( vector<bw_vec_type>& sel_bw_vector ){

  cerr << "Selected BWs" << endl;

  for( vector<bw_vec_type>::iterator bw_itr = sel_bw_vector.begin(); bw_itr != sel_bw_vector.end(); bw_itr++ )
    {

      show_bw_vector( (*bw_itr) );
       
    }
}


void BWOPTfix::show_bw_vector( vector<int>& bw_vector ){

 cerr << " BW vector ";

  for( vector<int>::iterator bw_itr = bw_vector.begin(); bw_itr != bw_vector.end(); bw_itr++ )
    {

      cerr << " : " << (*bw_itr);
       
       
    }

  cerr << endl;

}


double BWOPTfix::find_best_bw_vector( vector<bw_vec_type>& sel_bw_vector , vector<double>& grad_vec , vector<HWfix *>& vars_to_consider , bw_vec_type& best_bw_vector ){

  _DBOUT( " Selecting Best bit-width vector " );

    COST_RETURN _cost_return;

    double min_cost = 0;

   for( vector<bw_vec_type>::iterator bw_itr = sel_bw_vector.begin(); bw_itr != sel_bw_vector.end(); bw_itr++ )
     {
   
       _cost_return = cost_function( (*bw_itr) , grad_vec , vars_to_consider );

      
       min_cost = ( min_cost == 0 ? _cost_return.cost_value : min_cost );

       if ( (_cost_return.accept_flag) && ( _cost_return.cost_value < min_cost )){
	 
	 best_bw_vector.assign( (*bw_itr).begin() ,(*bw_itr).end() ); 

	 min_cost = _cost_return.cost_value;

       }

     } 

   return min_cost;

}

double BWOPTfix::try_selected_vectors( vector< bw_vec_type >& sel_bw_vector_in, vector<double> &grad_vec, vector<HWfix *>& vars_to_consider, vector< bw_vec_type >& sel_bw_vector_out, double start_cost ){

  double c_cost, min_cost;

  c_cost = 0;
  min_cost  = start_cost;

  for (  vector< bw_vec_type >::iterator vec_itr = sel_bw_vector_in.begin(); vec_itr != sel_bw_vector_in.end(); vec_itr++ ){

    c_cost = try_selected_vectors( *(vec_itr) , grad_vec ,  vars_to_consider , sel_bw_vector_out , start_cost ); 

    min_cost = min( c_cost , min_cost );

  }

  return min_cost;
}




double BWOPTfix::try_selected_vectors( vector<  int  >& bw_vector_in, vector<double> &grad_vec, vector<HWfix *>& vars_to_consider, vector< bw_vec_type  >& sel_bw_vector_out, double start_cost )
{
  
  vector< int  >::iterator vec_itr; 

  double c_cost = 0;

  COST_RETURN _cost_return;

  vector< int > *temp_vec;
  //cerr << endl;
  //show_bw_vector( bw_vector_in );

  c_cost = start_cost;
  for( vec_itr = bw_vector_in.begin() ; vec_itr != bw_vector_in.end() ; vec_itr++ ){

    
   

    int r_bit =  generate_random_bit();

    //int r_bit = 1;

      //_DBOUT( r_bit )

    (*vec_itr)-=r_bit;

     _cost_return = cost_function( bw_vector_in , grad_vec , vars_to_consider );
    
     double cost_delta = fabs( _cost_return.cost_value - c_cost );

     
     // _DBOUT( cost_delta << " : " << cost_delta / c_cost )
     if (( _cost_return.accept_flag == true ) && ( (cost_delta / c_cost ) < 0.01 )){
     //if (( _cost_return.accept_flag == true )){  
       temp_vec = new vector<int>::vector();
       
       temp_vec->assign( bw_vector_in.begin(), bw_vector_in.end() );
       sel_bw_vector_out.push_back( *temp_vec );

       //c_cost =  min( c_cost , _cost_return.cost_value);
       
       c_cost = _cost_return.cost_value;
       //cout << "NewCost :" << c_cost << endl;
       //show_bw_vector( sel_bw_vector_out.back() );
     }

     (*vec_itr) +=r_bit;

  }

  return c_cost;

}  



int BWOPTfix::generate_random_bit()
{
  rand();
  
  timeval result;

  gettimeofday( &result , NULL );

  
  srand( result.tv_usec );

  int random_seed1 = rand();
  
  int random_seed2 = rand();

  srand( random_seed1 );

  int rand1 = rand();

  srand( random_seed2 );
  int rand2 = rand();

  rand2 = ( rand2 == 0 ? rand() : rand2 );


  double num = rand1 / rand2;

  
 
 

  return ( num > 0 ? 1 : -1 );

}

double BWOPTfix::find_error( vector<int>& bw_vector ,  vector<double>& grad_vector )
{

   vector< int >::iterator bw_itr = bw_vector.begin();
   vector< double >::iterator g_itr = grad_vector.begin();
   double c_err = 0.;
   
   for ( ; bw_itr != bw_vector.end() ; bw_itr++ , g_itr++ )
       {
	 //c_err += fabs((*g_itr)) * pow( 2. , -(*bw_itr) );

	 //_DBOUT((*bw_itr) << " ::" <<  pow( 2. , -(*bw_itr) ) << " :: " << scalbn( 2. , -(*bw_itr) - 1 ) ) 

	 c_err += fabs((*g_itr)) * scalbn( 2. , -(*bw_itr) - 1);//   fabs((*g_itr)) * pow( 2. , -(*bw_itr) );

       } 
   
   return c_err;

}

BWOPTfix::COST_RETURN BWOPTfix::cost_function( vector<int>& bw_vector , vector<double>& grad_vector , vector<HWfix *>& var_vector )
{
  DAG *c_dag = bs->mydag;

  // Calculate output error first

  double c_err = find_error( bw_vector , grad_vector );
     
  bool accept_flag = false;
  COST_RETURN _cost_return;
  vector< int >::iterator bw_itr;
  if ( c_err > output_error )
    {

      _cost_return.accept_flag = false;
      _cost_return.cost_value = 0;

		 
    }
  else
    {
      _cost_return.accept_flag = true;
	 
      int bw_ctr = 0;
      vector<int> original_bw;
	 
      for ( bw_itr = bw_vector.begin(); bw_itr != bw_vector.end() ; bw_itr++ , bw_ctr++ ){
	original_bw.push_back( var_vector[ bw_ctr ]->frac_bw );
	var_vector[ bw_ctr ]->frac_bw =  bw_vector[ bw_ctr ];
      }
      _cost_return.cost_value = bs->get_area();

      for ( bw_ctr = 0, bw_itr = bw_vector.begin(); bw_itr != bw_vector.end() ; bw_itr++ , bw_ctr++){
	var_vector[ bw_ctr ]->frac_bw =  original_bw[ bw_ctr ];
      }
    }

  return _cost_return;
}



void BWOPTfix::setup_max_grads(HWfix *var)
{

  c_var = var;

  if ( c_var->ad_val.max_grads.size() == 0 )
    c_var->ad_val.max_grads.resize( c_var->ad_val.grads.size() , 0 );

  vector<double>::iterator g_iter, max_g_iter;

  for( g_iter = c_var->ad_val.grads.begin(),  max_g_iter = c_var->ad_val.max_grads.begin();
       g_iter !=  c_var->ad_val.grads.end(); g_iter++ , max_g_iter++)
    {

      (*max_g_iter) = max(  (*max_g_iter) , fabs( (*g_iter) ) );

    }



}



