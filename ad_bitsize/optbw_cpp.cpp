#include "optbw.h"

OptBW_cpp::OptBW_cpp( DAG* _dag , ADDAG *_addag ):OptBW( _dag , _addag , NULL ){}

void OptBW_cpp::run_adanalysis(ostream &os)
{

  if ( ad_dag->iter_count == 0 ){

    create_vectors();


    while( (*ad_dag)() )
      {

	if ( ad_dag->iter_count % 1000 == 0 )
	  os << "Running AD " << ad_dag->iter_count << endl;
      
	//get_max_ad();
	get_max_ad2();
     
      }

  }

  bs->range_analysis->update_bw();
  //ad_dag->show_ad_vecs();

}


void OptBW_cpp::gen_func_body( ostream &os ){

  
  vector<double>::iterator a_itr;

  int ctr;

  Edge *CEdge;

  int out_ctr = 0;


  string temp_name;
  for( vector<dag_index>::iterator itr = cdag->output_edges.begin(); itr != cdag->output_edges.end();
       itr++ )
    {
      
      CEdge =  cdag->edge_vec[ (*itr)];

      temp_name = "Et_" + CEdge->edge_name;

      //os << CEdge->edge_name << " , " << CEdge->ad_ndx << " :: ";

      //os << "Eout[ " << out_ctr++  << " ] = ";
	 
      ctr = 0;
      for( a_itr = CEdge->max_ad_vec.begin(); a_itr !=  CEdge->max_ad_vec.end() - 1; a_itr++, ctr++ )
	{
       
	  //if ( ctr < 7 )
	  os << temp_name << "[ " << ctr << " ] = ";

	  os << "fabs( E_" << ad_dag->ad_ndx_map[ ctr ]->edge_name << " * " <<  (*a_itr) << " ) ";

	  os << ";\n";

// 	  if ( a_itr !=  CEdge->max_ad_vec.end() - 2 )
// 	    os << " + ";


	} 

      //os << ";" << endl;
      
      os << "Eout[ " << out_ctr << " ] = 0.;\n";
      
      os << "for( int ndx = 0 ; ndx < " << ctr << " ; ndx++ )\n";

      os << "\tEout[ " << out_ctr++ << " ] += " << temp_name << "[ ndx ];\n";  
        
      var_decs << "double " << temp_name << "[ " << ctr << " ];\n";
   
      //os << "---------------------------------------------" <<endl;
       
    }



}

void OptBW_cpp::gen_func_top( ostream &os ){

  stringstream var_dec, var_init;

  Edge *CEdge;

  int ctr = 0;

  int param_ctr = 0;

  //for( int i = 0; i < ad_dag->ad_index - 1; i++ ){
  for( int i = 0; i < ad_dag->ad_ndx_map.size() ; i++ ){

    //cout << ad_dag->ad_ndx_map[ i ]->edge_name << "\n";

    var_dec << " E_" << ad_dag->ad_ndx_map[ i ]->edge_name << ",";

    var_init << "E_" << ad_dag->ad_ndx_map[ i ]->edge_name << " = ";

    
    

    CEdge = ad_dag->ad_ndx_map[ i ];

    if (  dynamic_cast< HWfix *>(CEdge->my_var)->frac_bw_state != SET )
       {
    	 var_init << "pow( 2. , -x[ " << ctr++ << " ] );" << endl;

	 bs->param_name_map[ param_ctr++ ] = CEdge;
       }
    else{
      var_init <<  " 0;" << endl;
    }

  }



  os << "double " << var_dec.str() << ";\n\n";

  os << var_init.str() << endl;

}


void OptBW_cpp::gen_error_func( ostream &os ){


   os << "int error_function(double *x, long int*in_para_dim, int *in_para_int_real ,USER_DEFINES * USER_OPTIONS )";

  os << "\n{\n";

  gen_func_top( os );

  os << "\ndouble Eout[ "  << cdag->output_edges.size() << " ];\n";

  stringstream temp_os;

  gen_func_body( temp_os );

  os << var_decs.str() << endl;

  os << temp_os.str() << endl;

  //os << "cout << Eout[ 0 ] << \" :: \" << USER_OPTIONS->out_err_spec[ 0 ] << endl;\n";

  os << "if ( ";

  for ( int i = 0 ; i < cdag->output_edges.size() ; i++ ){

    os << "( Eout[ " << i << " ] > USER_OPTIONS->out_err_spec[ " << i << " ] )";
    
    if ( i < cdag->output_edges.size() - 1 )
      os << "\n\t && ";

  }

  os << ")\n\t{return 0;}\n else\n\t{return 1;}\n";  
  
  os << "\n}\n";

}


void OptBW_cpp::update_edges_uni(double uni_bw ){


  Edge *CEdge;

  HWfix *temp;

  for ( int ctr = 0; ctr < bs->param_name_map.size() ; ctr++ ){

    CEdge = bs->param_name_map[ ctr ];

    if ( temp = dynamic_cast<HWfix *>( CEdge->my_var ) ){


      temp->frac_bw =  static_cast<fsize>( uni_bw );

      temp->int_bw =  static_cast<fsize>(bs->range_analysis->range_bw_fx(CEdge));


    }


  }



}

void OptBW_cpp::update_edges(double *cost_params){

  Edge *CEdge;

  HWfix *temp;

  for ( int ctr = 0; ctr < bs->param_name_map.size() ; ctr++ ){

    CEdge = bs->param_name_map[ ctr ];

    if ( temp = dynamic_cast<HWfix *>( CEdge->my_var ) ){


      temp->frac_bw =  static_cast<fsize>( cost_params[ ctr ] );

      temp->int_bw =  static_cast<fsize>(bs->range_analysis->range_bw_fx(CEdge));


    }


  }

  



}


//   Edge *CEdge;

//   HWfix *temp;
  
//   for (int index = 0; index <  ad_dag->ad_index - 1; ++index){

//     CEdge =  ad_dag->ad_ndx_map[ index ];

//      if( temp = dynamic_cast<HWfix *>( CEdge->my_var ) ){
//        if ( temp->frac_bw_state == NOTSET ) temp->frac_bw = static_cast<fsize>(uni_bw);
//        if ( temp->int_bw_state == NOTSET ) temp->int_bw = static_cast<fsize>(bs->range_analysis->range_bw_fx(CEdge));
//      }
//   }

//   for (int index = 0; index <  ad_dag->ad_index - 1; ++index){

//     CEdge =  ad_dag->ad_ndx_map[ index ];

//      if( temp = dynamic_cast<HWfix *>( CEdge->my_var ) ){
//        if ( temp->frac_bw_state == NOTSET ) temp->frac_bw = static_cast<fsize>(cost_params[ index ]);
//        if ( temp->int_bw_state == NOTSET ) temp->int_bw = static_cast<fsize>(bs->range_analysis->range_bw_fx(CEdge));
//      }
//   }
