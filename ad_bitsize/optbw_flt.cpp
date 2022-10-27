#include "optbw.h"

//#include "time.h"
//#include <sys/timeb.h>
//#include <sys/time.h>

OptBW_flt::OptBW_flt(  DAG *_dag , ADDAG *_addag ,  AreaModel *_areamodel ):OptBW( _dag , _addag ,_areamodel){}

// void OptBW_flt::operator() ( ostream &os )
// {

//    create_vectors();
   
//    os << "Doing Floating point analysis " << endl;

//   while( (*ad_dag)() )
//     {

//       os << "Running AD " << ad_dag->iter_count << endl;
//       //get_output_err();
//       find_uf_mbw();
//     }

  


// }

void OptBW_flt::operator() ( ostream &os )
{

  srand((unsigned)time(NULL));

  srand( rand() );

  srand( rand() );

  //srand( 658258 );

  create_vectors();

  
  struct timespec s_t,e_t;
  

  
  //clock_gettime( CLOCK_REALTIME , &s_t );

  while( (*ad_dag)() )
    {
      if ( ad_dag->iter_count % 100 == 0 )
	os << "Running AD " << ad_dag->iter_count << endl;
      //get_output_err();
      //find_uf_mbw();
      get_max_ad();
    }

  
  //clock_gettime( CLOCK_REALTIME , &e_t );

  // struct timespec clock_res;

//   clock_getres( CLOCK_REALTIME , &clock_res );
  
//   cout << "Clock Res " << clock_res.tv_sec << "s : " << clock_res.tv_nsec << "ns" << endl;

//   double  elapsed_sec = ( e_t.tv_sec + e_t.tv_nsec / (10e9) ) - ( s_t.tv_sec  + s_t.tv_nsec / (10e9) ); 
  
//   cout << "Elapsed Time : " << elapsed_sec << "s" << endl;

  
  bs->range_analysis->update_bw();

  //find_uf_fbw();

 

  int u_bw = find_uf_mbw(53);


  if ( bs->bw_mode == bitsize::UNIFORM ){
    update_bws( bw_vector );
    return;
  }


  set_fbw(  bw_vector , u_bw + 2 );

  prev_cost = get_area_cost( bw_vector );

  // sys_temp = prev_cost;

  sys_temp = prev_cost * 0.5;

  best_cost = prev_cost;
  
  double uniform_cost = prev_cost;
   best_bw_vector.assign( bw_vector.begin() , bw_vector.end() );
 
  cout << "Uniform Area Cost " << prev_cost << endl;

  find_nuf_fbw();

  //find_uf_mbw();

  cout << "Best non-unform area " << best_cost << endl;

  cout << "Saving : " << uniform_cost - best_cost  << " : ";
  cout << (uniform_cost - best_cost)*100/uniform_cost << "%" << endl;

  show_bw_vector( best_bw_vector );

}



void OptBW_flt::create_vectors()
{
  _DBOUT( "Doing FLT analysis" );

  vector<dag_index>::iterator e_itr;

  HWfloat *t_var;

  for( e_itr = cdag->edges_to_analyse.begin() ;e_itr != cdag->edges_to_analyse.end() ; e_itr++ ){
	      
    if (  cdag->edge_vec[(*e_itr)]->edge_type != ECONSTANT ){

      t_var =  dynamic_cast<HWfloat *>( cdag->edge_vec[ *e_itr ]->my_var );

      if ( t_var->man_bw_state == NOTSET )
	{
	  edges_to_consider.push_back( cdag->edge_vec[ *e_itr ]);
	}
    }

  }

  


}


int OptBW_flt::find_uf_mbw(int u_bw)
{

    //set_mbw( u_bw );

  set_fbw( bw_vector , u_bw );

  while(OptBW::get_output_err(  bw_vector ) )
    {

      //set_mbw( --u_bw );

       set_fbw(  bw_vector , --u_bw );

    }

  
  //cout << "Uniform bit-width " << u_bw << endl;

  cout << "Uniform bit-width " << endl;

  show_bw_vector( bw_vector );

  return u_bw;

}

double OptBW_flt::update_bws( vector<int>& bw_vector_in )
{

  vector< int >::iterator bw_itr;
  
  HWfloat *temp;

  Edge *CEdge;
  
  int bw_ctr = 0;

   for ( bw_itr = bw_vector_in.begin(); bw_itr != bw_vector_in.end() ; bw_itr++ , bw_ctr++ ){
     
     CEdge = edges_to_consider[ bw_ctr ];

     if( temp = dynamic_cast<HWfloat *>( CEdge->my_var ) ){
       temp->man_bw = (*bw_itr);
       if ( temp->exp_bw_state == NOTSET ) temp->exp_bw = static_cast<fsize>(bs->range_analysis->range_bw_flt(CEdge));
     }
   }



} 


// void OptBW_flt::set_mbw( int mbw )
// {
  
//    if ( bw_vector.size() == 0 ) bw_vector.resize( edges_to_consider.size() );

//    vector<int>::iterator bw_itr;

//   for(bw_itr = bw_vector.begin() ; bw_itr != bw_vector.end() ;bw_itr++ )
//     {

    
//       (*bw_itr) = mbw;
     
//     }


// }


// void OptBW_flt::find_nuf_fbw()
// {

// #define MAX_TRIES 10

//   selected_vectors.push_back( bw_vector );

//   vector<bw_vec_type>::iterator sbw_itr;

//   vector<bw_vec_type> new_selected_vectors;
  
//   double prev_best_cost = 0;
//    double prev_best_cost2 = 0;

//   for(  int trys = 0; trys < MAX_TRIES ; trys++ ){

//     for( sbw_itr = selected_vectors.begin() ; sbw_itr != selected_vectors.end() ; sbw_itr++ )
//       {
// 	create_new_bw_vectors( *sbw_itr , new_selected_vectors );


//       }
    


//     cout << trys << " : temp " << sys_temp << " : best_cost" << best_cost;
//     cout << " : No of Start vecs: ";
//     cout <<  selected_vectors.size() << " :: New vecs: " << new_selected_vectors.size()<< endl;  

//     if ( prev_best_cost2 == best_cost )
//     break;

//     if (  new_selected_vectors.size() > 150000 )
//       break;

//     prev_best_cost2 = prev_best_cost; 
//     prev_best_cost = best_cost;


//     swap_vectors( selected_vectors , new_selected_vectors );

//     sys_temp = sys_temp * .9;

//   }
  


// }


bool OptBW_flt::get_output_err( Edge *CEdge , vector<int>& bw_vector_in )
{

  //cout << "Output Edge :" << CEdge->edge_name; 

  //double output_err_spec = 0.001;

  double output_err_spec = dynamic_cast<HWfloat *>( CEdge->my_var )->rel_err;

  //_DBOUT( output_err_spec );

  vector<double>::iterator g_itr;

  
  double out_err = 0;
  
  int ctr = 0;
  

  vector<Edge *>::iterator e_itr;
  vector<int>::iterator bw_itr;

  double edge_exp;

  for(  e_itr = edges_to_consider.begin(), bw_itr = bw_vector_in.begin() ; e_itr != edges_to_consider.end() ; e_itr++, bw_itr++ )
      {


	//out_err +=  pow( 2. , -*(bw_itr) ) *  fabs(CEdge->max_ad_vec[  (*e_itr)->ad_ndx ]) ;


	 edge_exp = ilogb( CEdge->max_val );

	 out_err +=  pow( 2. ,  edge_exp - (*bw_itr) )  *  fabs(CEdge->ad_vec[  (*e_itr)->ad_ndx ]);

      }


  //_DBOUT( output_ulp_error << " : " << out_err ) 
    
    

   double output_perr = ( out_err / CEdge->max_val ) * 100;
 

  return (( output_err_spec - output_perr ) >= 0.);

}

double OptBW_flt::get_area_cost(  vector<int>& bw_vector_in )
{

  vector< int >::iterator bw_itr;
  int bw_ctr = 0;

  double cost_val;

   for ( bw_itr = bw_vector_in.begin(); bw_itr != bw_vector_in.end() ; bw_itr++ , bw_ctr++ ){
     dynamic_cast<HWfloat *>( edges_to_consider[ bw_ctr ]->my_var )->man_bw = (*bw_itr);
   }

   cost_val =  bs->get_area();

   return cost_val;


}

// bool OptBW_flt::get_output_err()
// {


//   bool err_spec = true;

//   for( vector<dag_index>::iterator itr = cdag->output_edges.begin(); itr != cdag->output_edges.end();
//        itr++ )
//     {

//       err_spec &= get_output_err( cdag->edge_vec[ (*itr)] );
      
//     }

//   return err_spec;

// }



// bool OptBW_flt::get_output_err( Edge *CEdge )
// {

//   //cout << "Output Edge :" << CEdge->edge_name; 


  
//   vector<double>::iterator g_itr;

  
//   double out_err = 0;
  
//   int ctr = 0;
//     for( g_itr = CEdge->ad_vec.begin() ; g_itr !=  CEdge->ad_vec.end() ; g_itr++, ctr++ )
// 	{
       
// 	  //if ( ctr < 7 )
// 	  // cout << "( " << ad_ndx_map[ ctr ]->edge_name << " , " <<  (*g_itr) << ") : ";
	    
// 	    out_err += get_signal_error(  ad_dag->ad_ndx_map[ ctr ] ) *  (*g_itr); 


// 	} 

//     double output_err_spec = 0.001;

//     double output_perr = ( out_err / CEdge->val ) * 100;

//     // _DBOUT( " Output Error : " << output_perr );

//     return (( output_err_spec - output_perr ) >= 0.);

// }



// bool OptBW_flt::get_output_err( Edge *CEdge )
// {

    
//   double output_err_spec = 0.001;

//   vector<double>::iterator g_itr;
  
//   double out_err = 0;
  
//   int ctr = 0;
 

//   vector<Edge *>::iterator e_itr;
//   vector<int>::iterator bw_itr;
//   double edge_exp = 0.;
//   for(  e_itr = edges_to_consider.begin(), bw_itr = bw_vector.begin() ; e_itr != edges_to_consider.end() ; e_itr++, bw_itr++ )
//     {

//       edge_exp = ilogb( CEdge->val );

//      out_err +=  pow( 2. ,  edge_exp - (*bw_itr) )  *  fabs(CEdge->ad_vec[  (*e_itr)->ad_ndx ]) ;

//     }


//   //_DBOUT( output_ulp_error << " : " << out_err ) 

//   double output_perr = ( out_err / CEdge->val ) * 100;

//   return (( output_err_spec - output_perr ) >= 0.);
    
  
// }





// double OptBW_flt::get_signal_error( Edge *CEdge )
// {

//   double edge_exp = ilogb( CEdge->val );

 
//   int edge_mbw = CEdge->flt_man_bw; // temporary fix


//   return ( pow( 2. , -edge_mbw )  *  pow(  2. , edge_exp ) );


// }
