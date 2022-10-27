#include "optbw.h"


OptBW::OptBW( DAG* _dag , ADDAG *_addag , AreaModel *_areamodel   ):cdag( _dag ),ad_dag( _addag ),area_model(_areamodel){}


void OptBW::get_max_ad()
{


  //bool err_spec = true;

  for( vector<dag_index>::iterator itr = cdag->output_edges.begin(); itr != cdag->output_edges.end();
       itr++ )
    {

      get_max_ad( cdag->edge_vec[ (*itr)] );
      
    }

  // return err_spec;

}



void OptBW::get_max_ad( Edge *CEdge )
{

  //cout << "Output Edge :" << CEdge->edge_name; 


  
  vector<double>::iterator g_itr, max_g_itr;

  
  double out_err = 0;
  
   if ( CEdge->max_ad_vec.size() == 0 )
    {

      CEdge->max_ad_vec.assign( CEdge->ad_vec.begin() , CEdge->ad_vec.end() ); 

    }
  else{


    for( g_itr = CEdge->ad_vec.begin(), max_g_itr = CEdge->max_ad_vec.begin()  ; g_itr !=  CEdge->ad_vec.end() ; g_itr++,  max_g_itr++ )
	{
       
	  //if ( ctr < 7 )
	  // cout << "( " << ad_ndx_map[ ctr ]->edge_name << " , " <<  (*g_itr) << ") : ";
	    
	  // out_err += get_signal_error(  ad_dag->ad_ndx_map[ ctr ] ) *  (*g_itr); 

	    (*max_g_itr) = max(  (*max_g_itr) , (*g_itr) );


	} 

  }
    

}


void OptBW::ad_error_func_gen(ostream &os)
{

  srand((unsigned)time(NULL));

  srand( rand() );

  srand( rand() );

  create_vectors();

  while( (*ad_dag)() )
    {

      if ( ad_dag->iter_count % 100 == 0 )
	os << "Running AD " << ad_dag->iter_count << endl;
      
      //get_max_ad();
      get_max_ad2();
     
    }

  //ad_dag->show_ad_vecs();

}

void OptBW::get_max_ad2()
{


  //bool err_spec = true;

  for( vector<dag_index>::iterator itr = cdag->output_edges.begin(); itr != cdag->output_edges.end();
       itr++ )
    {

      //get_max_ad2( cdag->edge_vec[ (*itr)] );
      
      get_max_ad( cdag->edge_vec[ (*itr)] );
      
    }

  // return err_spec;

}

void OptBW::get_max_ad2( Edge *CEdge )
{

  //cout << "Output Edge :" << CEdge->edge_name; 


  
  vector<double>::iterator g_itr, max_g_itr;

  
  double out_err = 0;

  double max_sum, ad_sum;
  
   if ( CEdge->max_ad_vec.size() == 0 )
    {

      CEdge->max_ad_vec.assign( CEdge->ad_vec.begin() , CEdge->ad_vec.end() ); 

    }
  else{

    max_sum = 0;
    ad_sum = 0;
    for( g_itr = CEdge->ad_vec.begin(), max_g_itr = CEdge->max_ad_vec.begin()  ; g_itr !=  CEdge->ad_vec.end() ; g_itr++,  max_g_itr++ )
	{
       
	  //if ( ctr < 7 )
	  // cout << "( " << ad_ndx_map[ ctr ]->edge_name << " , " <<  (*g_itr) << ") : ";
	    
	  // out_err += get_signal_error(  ad_dag->ad_ndx_map[ ctr ] ) *  (*g_itr); 

	  //(*max_g_itr) = max(  (*max_g_itr) , (*g_itr) );

	  max_sum += fabs(*max_g_itr);
	  ad_sum += fabs(*g_itr);

	} 

    if ( ad_sum > max_sum ){
      CEdge->max_ad_vec.assign( CEdge->ad_vec.begin() , CEdge->ad_vec.end() ); 
    }

  }
    

}

void OptBW::find_nuf_fbw()
{

#define MAX_TRIES 10

  selected_vectors.push_back( bw_vector );

  vector<bw_vec_type>::iterator sbw_itr;

  vector<bw_vec_type> new_selected_vectors;
  
  double prev_best_cost = 0;
   double prev_best_cost2 = 0;

  for(  int trys = 0; trys < MAX_TRIES ; trys++ ){

    for( sbw_itr = selected_vectors.begin() ; sbw_itr != selected_vectors.end() ; sbw_itr++ )
      {
	create_new_bw_vectors( *sbw_itr , new_selected_vectors );


      }
    


    cout << trys << " : temp " << sys_temp << " : best_cost" << best_cost;
    cout << " : No of Start vecs: ";
    cout <<  selected_vectors.size() << " :: New vecs: " << new_selected_vectors.size()<< endl;  

    if ( prev_best_cost2 == best_cost )
    break;

    if (  new_selected_vectors.size() > 7000 )
      break;

    prev_best_cost2 = prev_best_cost; 
    prev_best_cost = best_cost;


    swap_vectors( selected_vectors , new_selected_vectors );

    sys_temp = sys_temp * .9;

  }
  


}


void OptBW::show_bw_vector( vector<int>& bw_vector ){

 cerr << " BW vector ";

  for( vector<int>::iterator bw_itr = bw_vector.begin(); bw_itr != bw_vector.end(); bw_itr++ )
    {

      cerr << " : " << (*bw_itr);
       
       
    }

  cerr << endl;

}



void OptBW::save_bw_vector(vector<int>& bw_vector_in , vector<bw_vec_type>& new_selected_vectors)
{

  vector<int> *temp_vec = new vector<int>::vector();

  temp_vec->assign( bw_vector_in.begin() , bw_vector_in.end() );

  new_selected_vectors.push_back( *temp_vec ); 
  


}


void OptBW::swap_vectors( vector<bw_vec_type>& vec1 , vector<bw_vec_type>& vec2 )
{

  // vector<bw_vec_type>::iterator v_itr1 , v_itr2;
  
 //  vector<int> *temp_vec;

//   for( v_itr1 = vec1.begin() ; v_itr1 != vec1.end() ; v_itr1++ )
//     {

//       temp_vec  = (*v_itr1);

//       ~(*temp_vec);
//     }

  vec1.clear();

  vec1.assign( vec2.begin() ,vec2.end() );


}




double OptBW::get_rand_val()
{

  return  fabs( rand() /(double)(RAND_MAX + 1 ) );

}

int OptBW::generate_random_bit()
{

   double r_num = rand();

   double r_num2 = rand();

   //r_num = ( r_num / pow( 2. , ilogb( r_num ) ) ) - 1.;

   r_num = get_rand_val();

   int r_bit;

   if ( r_num < 0.5 )
     r_bit = 1;
   else if ( r_num < 0.75 )
     r_bit = 2;
   else if ( r_num < 0.875 )
     r_bit = 3;
   else if ( r_num < 0.95 )
     r_bit = 4;
   else 
     r_bit = rand() % 8;


     //r_num2 = ( r_num2 / pow( 2. , ilogb( r_num2 ) ) ) - 1.;

   r_num2 = get_rand_val();  

   return ( r_num2 > 0.5 ? r_bit : -r_bit );

}

void OptBW::set_fbw( vector<int>& bw_vector_in , int fbw )
{

  if ( bw_vector_in.size() == 0 ) bw_vector_in.resize( edges_to_consider.size() );

   vector<int>::iterator bw_itr;

  for(bw_itr = bw_vector_in.begin() ; bw_itr != bw_vector_in.end() ;bw_itr++ )
    {

          (*bw_itr) = fbw;
      
    }


}

bool OptBW::accept_bw_vector( vector<int>& bw_vector_in )
{

  bool decision;

  

  if ( get_output_err(  bw_vector_in  ) ){

    c_cost = get_area_cost( bw_vector_in );
    //cout << "Area Cost " << c_cost << endl;


    double c_delta = c_cost - prev_cost;
    
    double r_num = rand();

    // r_num = ( r_num / pow( 2. , ilogb( r_num ) ) ) - 1.;

    r_num = get_rand_val();

    double p_val =  exp( -c_delta / sys_temp );

   
    if ( c_delta < 0 ) {
      decision = true;
      prev_cost = c_cost;
    }else if ( r_num < p_val ){
      //_DBOUT( "R val" << r_num << " :: " << p_val);
      decision = true;
    }
    else{

      decision = false;
    }

  }
  else{

    decision = false;

  }

  return decision;

}



bool OptBW::get_output_err( vector<int>& bw_vector_in  )
{


  bool err_spec = true;

  for( vector<dag_index>::iterator itr = cdag->output_edges.begin(); itr != cdag->output_edges.end();
       itr++ )
    {

      err_spec &= get_output_err( cdag->edge_vec[ (*itr)] , bw_vector_in );
      
    }

  return err_spec;

}



void OptBW::create_new_bw_vectors( bw_vec_type& bw_vector_in  , vector<bw_vec_type>& new_bw_vectors )
{

  bw_vec_type::iterator vec_itr;

      for( vec_itr = bw_vector_in.begin() ; vec_itr != bw_vector_in.end() ; vec_itr++ ){
	int r_bit = 1;
	
	//(*vec_itr)-=r_bit;
	
	(*vec_itr)+= generate_random_bit();

	//	cout << (*vec_itr) << "::";

	if ( accept_bw_vector(bw_vector_in) ){
	  //show_bw_vector( bw_vector_in );
 	   save_bw_vector(bw_vector_in , new_bw_vectors);
	   

	   if ( c_cost < best_cost )
	     {

	       best_cost = c_cost;

	       best_bw_vector.assign( bw_vector_in.begin() , bw_vector_in.end() );
	       
	     } 


	}

      }

      // cout << endl;
}
