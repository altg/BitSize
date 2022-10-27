#include "optbw.h"
#include "time.h"


OptBW_fx::OptBW_fx(  DAG *_dag , ADDAG *_addag ,  AreaModel *_areamodel ):OptBW( _dag , _addag ,_areamodel){}




void OptBW_fx::operator() ( ostream &os )
{

  srand((unsigned)time(NULL));

  srand( rand() );

  srand( rand() );

  create_vectors();

  while( (*ad_dag)() )
    {

      if ( ad_dag->iter_count % 100 == 0 )
	os << "Running AD " << ad_dag->iter_count << endl;
      //get_output_err();
      //find_uf_mbw();
      
      //get_max_ad();
      get_max_ad2();
     
    }


 
  if ( bs->db_level > 0 )
   ad_dag->show_ad_vecs();

  int u_bw = find_uf_fbw( 40 );

  if ( bs->bw_mode == bitsize::UNIFORM ){
     update_bws( bw_vector );
     return;
  }

  set_fbw(  bw_vector , u_bw + 2 );

  prev_cost = get_area_cost( bw_vector );

  // sys_temp = prev_cost;

  sys_temp = prev_cost *0.5;

  best_cost = prev_cost;
  
  double uniform_cost = prev_cost;
   best_bw_vector.assign( bw_vector.begin() , bw_vector.end() );
 
  cout << "Uniform Area Cost " << prev_cost << endl;

  find_nuf_fbw();

 

  cout << "Best non-unform area " << best_cost << endl;

  cout << "Saving : " << uniform_cost - best_cost  << " : ";
  cout << (uniform_cost - best_cost)*100/uniform_cost << "%" << endl;

  update_bws( best_bw_vector );

  show_bw_vector( best_bw_vector );

}


void OptBW_fx::create_vectors()
{

   _DBOUT( "Doing FIX analysis" );
  vector<dag_index>::iterator e_itr;

  HWfix *t_var;

  for( e_itr = cdag->edges_to_analyse.begin() ;e_itr != cdag->edges_to_analyse.end() ; e_itr++ ){
	      
    if (  cdag->edge_vec[(*e_itr)]->edge_type != ECONSTANT ){

      t_var =  dynamic_cast<HWfix *>( cdag->edge_vec[ *e_itr ]->my_var );

      if ( t_var->frac_bw_state == NOTSET )
	{
	  edges_to_consider.push_back( cdag->edge_vec[ *e_itr ]);
	}
    }

  }

  


}

int OptBW_fx::find_uf_fbw(int u_bw)
{

  
  set_fbw( bw_vector , u_bw );

  while( OptBW::get_output_err(  bw_vector ) )
    {

      set_fbw(  bw_vector , --u_bw );

    }
  
   
  cout << "Uniform bit-width " << endl;

  show_bw_vector( bw_vector );

  return u_bw;


}








double OptBW_fx::get_area_cost(  vector<int>& bw_vector_in )
{

  vector< int >::iterator bw_itr;
  int bw_ctr = 0;

  double cost_val;

   for ( bw_itr = bw_vector_in.begin(); bw_itr != bw_vector_in.end() ; bw_itr++ , bw_ctr++ ){
     dynamic_cast<HWfix *>( edges_to_consider[ bw_ctr ]->my_var )->frac_bw = (*bw_itr);
   }

   cost_val =  bs->get_area();

   return cost_val;


}


double OptBW_fx::update_bws( vector<int>& bw_vector_in )
{

  vector< int >::iterator bw_itr;
  
  HWfix *temp;

  Edge *CEdge;
  
  int bw_ctr = 0;

   for ( bw_itr = bw_vector_in.begin(); bw_itr != bw_vector_in.end() ; bw_itr++ , bw_ctr++ ){
     
     CEdge = edges_to_consider[ bw_ctr ];

     if( temp = dynamic_cast<HWfix *>( CEdge->my_var ) ){
       temp->frac_bw = (*bw_itr);
       if ( temp->int_bw_state == NOTSET ) temp->int_bw = static_cast<fsize>(bs->range_analysis->range_bw_fx(CEdge));
     }
   }



} 



// void OptBW_fx::create_new_bw_vectors( bw_vec_type& bw_vector_in  , vector<bw_vec_type>& new_bw_vectors )
// {

//   bw_vec_type::iterator vec_itr;


//   int num_bws = rand() % bw_vector_in.size();

//   int bw_ctr;

//   _DBOUT( num_bws )

//   for( int ctr = 0 ; ctr < num_bws ; ctr++ )
//     {
      
//       bw_ctr =  bw_vector_in.size();
      
//       _DBOUT( "bwctr : " << bw_ctr );

//       bw_vector_in[ bw_ctr ] = generate_random_bit(); 
      
//       	if ( accept_bw_vector(bw_vector_in) ){
// 	  //show_bw_vector( bw_vector_in );
//  	   save_bw_vector(bw_vector_in , new_bw_vectors);
// 	   if ( c_cost < best_cost )
// 	     {
// 	       best_cost = c_cost;
// 	       best_bw_vector.assign( bw_vector_in.begin() , bw_vector_in.end() );
// 	     }
// 	} 

//     }

     

//       // cout << endl;
// }



bool OptBW_fx::get_output_err( Edge *CEdge , vector<int>& bw_vector_in )
{

  //cout << "Output Edge :" << CEdge->edge_name; 

  double output_ulp_error = pow( 2. , -static_cast<int>( dynamic_cast<HWfix *>(CEdge->my_var)->frac_bw ) - 1 );
  
  vector<double>::iterator g_itr;

  
  double out_err = 0;
  
  int ctr = 0;
  

  vector<Edge *>::iterator e_itr;
  vector<int>::iterator bw_itr;

  for(  e_itr = edges_to_consider.begin(), bw_itr = bw_vector_in.begin() ; e_itr != edges_to_consider.end() ; e_itr++, bw_itr++ )
      {


	out_err +=  pow( 2. , -*(bw_itr) ) *  fabs(CEdge->max_ad_vec[  (*e_itr)->ad_ndx ]) ;

      }


  //_DBOUT( output_ulp_error << " : " << out_err ) 
    
    return (( output_ulp_error - out_err ) >= 0.);

}

// double OptBW_fx::get_signal_error( Edge *CEdge )
// {

//   double edge_exp = ilogb( CEdge->val );

 
//   int edge_fbw = CEdge->fx_frac_bw; // temporary fix


//   return ( pow( 2. , -edge_fbw ) );


// }





