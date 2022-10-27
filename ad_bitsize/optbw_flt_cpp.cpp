#include "optbw.h"

OptBWFLT_cpp::OptBWFLT_cpp( DAG* _dag , ADDAG *_addag ):OptBW( _dag , _addag , NULL ){}

void OptBWFLT_cpp::run_adanalysis(ostream &os)
{

  if ( ad_dag->iter_count == 0 ){

    create_vectors();


    while( (*ad_dag)() )
      {

	if ( ad_dag->iter_count % 1000 == 0 )
	  //os << "Running AD " << ad_dag->iter_count << endl;
      
	//get_max_ad();
	get_max_ad2();
     
      }

  }

  bs->range_analysis->update_bw();
  

  // bs->range_analysis->show_range_vecs();
  
  //ad_dag->show_ad_vecs();

}


void OptBWFLT_cpp::gen_func_body( ostream &os ){

  
  vector<double>::iterator a_itr;

  int ctr;

  Edge *CEdge;

  int out_ctr = 0;

  int temp_ctr;

  string temp_name;
  for( vector<dag_index>::iterator itr = cdag->output_edges.begin(); itr != cdag->output_edges.end();
       itr++ )
    {
      
      CEdge =  cdag->edge_vec[ (*itr)];

      temp_name = "Et_" + CEdge->edge_name;

     	 
      ctr = 0;
      temp_ctr = 0;
      for( a_itr = CEdge->max_ad_vec.begin(); a_itr !=  CEdge->max_ad_vec.end() - 1; a_itr++, ctr++ )
	{
       
	
	  if ( (*a_itr) != 0. ){
	  os << temp_name << "[ " << temp_ctr++ << " ] = ";

	  os << "fabs( E_" << ad_dag->ad_ndx_map[ ctr ]->edge_name << " * " <<  fabs((*a_itr)) << " ) ";

	  os << ";\n";
	  
	  }

	} 

      //os << ";" << endl;
      
      os << "Eout[ " << out_ctr << " ] = 0.;\n";
      
      os << "for( int ndx = 0 ; ndx < " << temp_ctr << " ; ndx++ )\n";

      os << "\tEout[ " << out_ctr++ << " ] += " << temp_name << "[ ndx ];\n";  
        
      var_decs << "double " << temp_name << "[ " << temp_ctr << " ];\n";
   
      //os << "---------------------------------------------" <<endl;
       
    }



}


double OptBWFLT_cpp::get_emax( Edge *CEdge ){


  double max_abs_val = max( fabs( CEdge->max_val ) , fabs( CEdge->min_val ) );

  //double max_abs_val = max( ( CEdge->max_val ) , ( CEdge->min_val ) );

  return ( max_abs_val == 0 ? 0 : ilogb( max_abs_val ) );


}

void OptBWFLT_cpp::gen_func_top( ostream &os ){

  stringstream var_dec, var_init;

  Edge *CEdge;

  int ctr = 0;

  double edge_exp;

  //DBL_PREC test_val;

  //for( int i = 0; i < ad_dag->ad_index - 1; i++ ){
  
  for( int i = 0; i < ad_dag->ad_ndx_map.size() ; i++ ){

    //cout << ad_dag->ad_ndx_map[ i ]->edge_name << "\n";

    var_dec << " E_" << ad_dag->ad_ndx_map[ i ]->edge_name << ",";

    

    var_init << "E_" << ad_dag->ad_ndx_map[ i ]->edge_name << " = ";

    
    

    CEdge = ad_dag->ad_ndx_map[ i ];

    if (  dynamic_cast< HWfloat *>(CEdge->my_var)->man_bw_state != SET )
      {
	//edge_exp = ilogb( CEdge->max_val );
	 
	edge_exp = get_emax( CEdge );
	//test_val.dbl_as_flt = CEdge->max_val;

	//cout << edge_exp << " : " <<  test_val.dbl_as_fld.exp - pow(2. , 10 ) + 1  << " : " << CEdge->max_val << endl;

	var_init << "pow( 2. , " << edge_exp << " - x[ " << ctr << " ] );" << endl;

	bs->param_name_map[ ctr++ ] = CEdge;

      }
    else{
      var_init <<  " 0;" << endl;
    }

    
    

  }



  os << "double " << var_dec.str() << ";\n\n";

  os << var_init.str() << endl;

}


void OptBWFLT_cpp::gen_error_func( ostream &os ){


   os << "int error_function(double *x, long int*in_para_dim, int *in_para_int_real ,USER_DEFINES * USER_OPTIONS )";

  os << "\n{\n";

  gen_func_top( os );

  os << "\ndouble Eout[ "  << cdag->output_edges.size() << " ];\n";

  stringstream temp_os;

  gen_func_body( temp_os );

  os << var_decs.str() << endl;

  os << temp_os.str() << endl;

//   os << "cout << Eout[ 0 ] << \" :: \" << USER_OPTIONS->out_err_spec[ 0 ] << endl;\n";
//    os << "cout << Eout[ 1 ] << \" :: \" << USER_OPTIONS->out_err_spec[ 1 ] << endl;\n";
  

  os << "if ( ";

  for ( int i = 0 ; i < cdag->output_edges.size() ; i++ ){

    os << "( Eout[ " << i << " ] > USER_OPTIONS->out_err_spec[ " << i << " ] )";
    
    if ( i < cdag->output_edges.size() - 1 )
      os << "\n\t && ";

  }

  os << ")\n\t{return 0;}\n else\n\t{return 1;}\n";  
  
  os << "\n}\n";

}





void  OptBWFLT_cpp::update_edges(double *cost_params){

  Edge *CEdge;

  HWfloat *temp;

  int diff;

  for ( int ctr = 0; ctr < bs->param_name_map.size() ; ctr++ ){

    CEdge = bs->param_name_map[ ctr ];

    if ( temp = dynamic_cast<HWfloat *>( CEdge->my_var ) ){


      temp->man_bw =  static_cast<fsize>( cost_params[ ctr ] );

      temp->exp_bw =  static_cast<fsize>(bs->range_analysis->range_bw_flt(CEdge));

      diff = abs(  static_cast<int>( temp->man_bw - temp->exp_bw  ) );

      if ( diff < 3 ) temp->exp_bw += diff;


    }


  }

 

}


void OptBWFLT_cpp::update_edges_uni(double uni_bw ){


  Edge *CEdge;

  HWfloat *temp;

  int diff;

  for ( int ctr = 0; ctr < bs->param_name_map.size() ; ctr++ ){

    CEdge = bs->param_name_map[ ctr ];

    if ( temp = dynamic_cast<HWfloat *>( CEdge->my_var ) ){


      temp->man_bw =  static_cast<fsize>( uni_bw );

      temp->exp_bw =  static_cast<fsize>(bs->range_analysis->range_bw_flt(CEdge));

      diff = abs(  static_cast<int>( temp->man_bw - temp->exp_bw  ) );

      if ( diff < 3 ) temp->exp_bw += diff;


    }


  }

 

}


// void OptBWFLT_cpp::update_edges_uni(double uni_bw ){

//   Edge *CEdge;

//   HWfloat *temp;
  
//   int diff;

//   for (int index = 0; index <  ad_dag->ad_index - 1; ++index){

//     CEdge =  ad_dag->ad_ndx_map[ index ];

//      if( temp = dynamic_cast<HWfloat *>( CEdge->my_var ) ){
//        if ( temp->man_bw_state == NOTSET ) temp->man_bw = static_cast<fsize>(uni_bw);
//        if ( temp->exp_bw_state == NOTSET ) temp->exp_bw = static_cast<fsize>(bs->range_analysis->range_bw_flt(CEdge));

//        diff = abs( static_cast<int>( temp->man_bw - temp->exp_bw ) );

//        if ( diff < 3 ) temp->exp_bw += diff;
       
//      }
//   }


// }

// void OptBWFLT_cpp::update_edges(double *cost_params){

//   Edge *CEdge;

//   HWfloat *temp;
  
//   int diff;

//   for (int index = 0; index <  ad_dag->ad_index - 1; ++index){

//     CEdge =  ad_dag->ad_ndx_map[ index ];

//      if( temp = dynamic_cast<HWfloat *>( CEdge->my_var ) ){
//         if ( temp->man_bw_state == NOTSET ) temp->man_bw = static_cast<fsize>(cost_params[ index ]);
//        if ( temp->exp_bw_state == NOTSET ) temp->exp_bw = static_cast<fsize>(bs->range_analysis->range_bw_flt(CEdge));

//        diff = abs(  static_cast<int>( temp->man_bw - temp->exp_bw  ) );

//        if ( diff < 3 ) temp->exp_bw += diff;

//      }
//   }


// }
