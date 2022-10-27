//#undef _BITSIZE_DEBUG_

#include "errormodel_affine.h"

ErrorModel_Affine2::ErrorModel_Affine2(DAG *_dag , NodeSchedular *_node_sched  ):BitSizePass( _dag ),
	      node_sched(_node_sched),affine_index(1),affine_error_analysis_done(false){}


void ErrorModel_Affine2::do_affine_analysis()
{
  
  if ( affine_error_analysis_done == true ) return;

  init_name_maps();
  
  _DBOUT( "Doing Affine2 error analysis" );

  vector< NodeSchedular::node_vec_type >::iterator sch_itr; 
  vector< dag_index >::iterator n_itr2;

  int ctr = 0;
  for ( sch_itr = node_sched->schedule.begin() ; sch_itr != node_sched->schedule.end() ;  sch_itr++ )
    {

     
      
      for( n_itr2 = (*sch_itr).begin() ; n_itr2 != (*sch_itr).end() ; n_itr2++ )
	{

	    do_affine_analysis_node((*n_itr2));

	}  




    }

  

  affine_error_analysis_done = true;

}

void ErrorModel_Affine2::do_affine_analysis_node(dag_index node_num)
{
 
  Node* CNode = c_dag->node_vec[ node_num ];

  switch( CNode->node_type )
    {

    case INPUT: affine_input_node( CNode ); break; 

    case ASSIGN: affine_assign_node( CNode );break;

    case ADD: affine_add_node( CNode ); break;

    case SUB: affine_sub_node( CNode ); break;

    case MUL: affine_mul_node( CNode ); break;

    case DIV: affine_div_node( CNode ); break;
      
    case CONSTANT: affine_const_node_exp( CNode ); break;  

    case SHIFTL: affine_shiftl_node( CNode ); break;

    case SHIFTR: affine_shiftr_node( CNode ); break;


      //case DIV: gen_div_node_exp( os , CNode ); break;


    defualt:;



    }

  


}





void ErrorModel_Affine2::get_abs_eterms( stringstream &ss , Edge *CEdge )
{
  
  
  //if( CEdge->edge_type != ECONSTANT ){

  stringstream ss2;

    vector<double>::iterator a_itr = CEdge->af_err_vec2.begin();

   
    string e_name  = CEdge->edge_name;

    int ctr = 0;

    ss << "[ ";

    for(; a_itr != CEdge->af_err_vec2.end(); a_itr++ )
      {


	ss << setprecision(15) << fabs((*a_itr)) << " ";

	CEdge->TransFuncVals_vec.push_back(  fabs((*a_itr)) );

      }

    ss << "] ";


 
    
}


void ErrorModel_Affine2::affine_const_node_exp( Node *CNode )
{


  Edge *edge_out = c_dag->edge_vec[ CNode->out_edges.back() ];

 

  if ( edge_out->edge_type != ECONSTANT ){

    

    edge_out->af_err_vec2.resize( affine_index );
    edge_out->af_ndx_err2 = affine_index++;
    af_ndx_map[ edge_out->af_ndx_err2] = edge_out;

 
    edge_out->af_err_vec2[ edge_out->af_err_vec2.size() - 1 ] = 1.;

    stringstream ss_err;
    
    gen_signal_errorname( ss_err , edge_out );

  }


}

void ErrorModel_Affine2::affine_assign_node( Node *CNode )
{

  Edge *edge_in = c_dag->edge_vec[ CNode->in_edges.back()];
  
  Edge *edge_out = c_dag->edge_vec[ CNode->out_edges.back()];
  
  edge_out->af_err_vec2.assign( edge_in->af_err_vec2.begin() , edge_in->af_err_vec2.end() );



}



void ErrorModel_Affine2::affine_add_node( Node *CNode )
{
  Edge *edge_l = c_dag->edge_vec[ CNode->in_edges[0] ];
  Edge *edge_r = c_dag->edge_vec[ CNode->in_edges[1] ];
  Edge *edge_out = c_dag->edge_vec[ CNode->out_edges.back() ];

    
    edge_out->af_err_vec2.resize( affine_index );
    edge_out->af_ndx_err2 = affine_index++;
    af_ndx_map[ edge_out->af_ndx_err2] = edge_out;
    //ad_ndx_map[ edge_out->ad_ndx] = edge_out;
 

    //edge_out->val = edge_l->val + edge_r->val;
   // find_min_max( edge_out );
  int ctr;

  double X_l, X_r;
  
  X_l = X_r = 0;

  //Node *node_l = c_dag->node_vec[ edge_l->get_src_node() ];

  //Node *node_r = c_dag->node_vec[ edge_r->get_src_node() ];

  

  for( ctr =  0 ; ctr < edge_out->af_err_vec2.size() ; ctr++ )
    {
	
      X_l = ( ctr < edge_l->af_err_vec2.size() ) ? edge_l->af_err_vec2[ ctr ] : 0;

      X_r = ( ctr < edge_r->af_err_vec2.size() ) ? edge_r->af_err_vec2[ ctr ] : 0;

      edge_out->af_err_vec2[ ctr ] = X_l + X_r;
    }


 edge_out->af_err_vec2[ edge_out->af_err_vec2.size() - 1 ]= 1.;
  
  stringstream ss_err;
    
  gen_signal_errorname( ss_err , edge_out );

}


void ErrorModel_Affine2::affine_sub_node( Node *CNode )
{
  Edge *edge_l = c_dag->edge_vec[ CNode->in_edges[0] ];
  Edge *edge_r = c_dag->edge_vec[ CNode->in_edges[1] ];
  Edge *edge_out = c_dag->edge_vec[ CNode->out_edges.back() ];
  
 
  edge_out->af_err_vec2.resize( affine_index );
  edge_out->af_ndx_err2 = affine_index++;
  af_ndx_map[ edge_out->af_ndx_err2] = edge_out;
  //ad_ndx_map[ edge_out->ad_ndx] = edge_out;
 


  //edge_out->val = edge_l->val - edge_r->val;
  // find_min_max( edge_out );
   
  int ctr;

  double X_l, X_r;
  
  X_l = X_r = 0;


  Node *node_l = c_dag->node_vec[ edge_l->get_src_node() ];

  Node *node_r = c_dag->node_vec[ edge_r->get_src_node() ];

  for( ctr =  0 ; ctr < edge_out->af_err_vec2.size() ; ctr++ )
    {
	
      X_l = ( ctr < edge_l->af_err_vec2.size() ) ? edge_l->af_err_vec2[ ctr ] : 0;

      X_r = ( ctr < edge_r->af_err_vec2.size() ) ? edge_r->af_err_vec2[ ctr ] : 0;

      edge_out->af_err_vec2[ ctr ] = X_l - X_r;
    }

  edge_out->af_err_vec2[ edge_out->af_err_vec2.size() - 1 ]= 1.;

  stringstream ss_err;
    
  gen_signal_errorname( ss_err , edge_out );

}







void ErrorModel_Affine2::affine_mul_node( Node *CNode )
{


  Edge *edge_l = c_dag->edge_vec[ CNode->in_edges[0] ];
  Edge *edge_r = c_dag->edge_vec[ CNode->in_edges[1] ];
  Edge *edge_out = c_dag->edge_vec[ CNode->out_edges.back() ];


   edge_out->af_err_vec2.resize( affine_index );
   edge_out->af_ndx_err2 = affine_index++;
   af_ndx_map[ edge_out->af_ndx_err2] = edge_out;

  //cout << "Mul " << edge_l->val << " " << edge_r->val << endl; 

   //edge_out->val = edge_l->val * edge_r->val;
   //find_min_max( edge_out );

  
  double X_l, X_r;

  X_l = X_r = 0;

  int ctr;

  Node *node_l = c_dag->node_vec[ edge_l->get_src_node() ];

  Node *node_r = c_dag->node_vec[ edge_r->get_src_node() ];


 //  double l_val = ( node_l->node_type != CONSTANT )? bs->range_analysis->get_range_max( edge_l ) : node_l->node_val;
//   double r_val = ( node_r->node_type != CONSTANT )? bs->range_analysis->get_range_max( edge_r ) : node_r->node_val;

  double l_val = ( edge_l->edge_type != ECONSTANT )? bs->range_analysis->get_range_max( edge_l ) : node_l->node_val;
  double r_val = ( edge_r->edge_type != ECONSTANT )? bs->range_analysis->get_range_max( edge_r ) : node_r->node_val;


  for( ctr =  0 ; ctr < edge_out->af_err_vec2.size() ; ctr++ )
      {
		     
	X_l = ( ctr < edge_l->af_err_vec2.size() ) ? edge_l->af_err_vec2[ ctr ] : 0;

	X_r = ( ctr < edge_r->af_err_vec2.size() ) ? edge_r->af_err_vec2[ ctr ] : 0;

	edge_out->af_err_vec2[ ctr ] = r_val * X_l + l_val * X_r;
      }



   edge_out->af_err_vec2[ edge_out->af_err_vec2.size() - 1 ]= 1.;
   
   stringstream ss_err;
    
   gen_signal_errorname( ss_err , edge_out );
}


void ErrorModel_Affine2::affine_div_node( Node *CNode )
{


  Edge *edge_l = c_dag->edge_vec[ CNode->in_edges[0] ];
  Edge *edge_r = c_dag->edge_vec[ CNode->in_edges[1] ];
  Edge *edge_out = c_dag->edge_vec[ CNode->out_edges.back() ];


  edge_out->af_err_vec2.resize( affine_index );
  edge_out->af_ndx_err2 = affine_index++;
  af_ndx_map[ edge_out->af_ndx_err2] = edge_out;


  double X_l, X_r;

  X_l = X_r = 0;

  Node *node_l = c_dag->node_vec[ edge_l->get_src_node() ];
  Node *node_r = c_dag->node_vec[ edge_r->get_src_node() ];

  //double l_val = ( node_l->node_type != CONSTANT )? bs->range_analysis->get_range_max( edge_l ) : node_l->node_val;
  //double r_val = ( node_r->node_type != CONSTANT )? bs->range_analysis->get_range_max( edge_r ) : node_r->node_val;

  double l_val = ( edge_l->edge_type != ECONSTANT )? bs->range_analysis->get_range_max( edge_l ) : node_l->node_val;
  double r_val = ( edge_r->edge_type != ECONSTANT )? bs->range_analysis->get_range_min( edge_r ) : node_r->node_val;


  for( int ctr =  0 ; ctr < edge_out->af_err_vec2.size() ; ctr++ )
    {
		     
      X_l = ( ctr < edge_l->af_err_vec2.size() ) ? edge_l->af_err_vec2[ ctr ] : 0;

      X_r = ( ctr < edge_r->af_err_vec2.size() ) ? edge_r->af_err_vec2[ ctr ] : 0;

      //edge_out->af_err_vec2[ ctr ] = (r_val * X_l - l_val * X_r)/( r_val * r_val );

      edge_out->af_err_vec2[ ctr ] = (X_l / r_val) - ( l_val * X_r ) / ( r_val * r_val ); 
    }




  edge_out->af_err_vec2[ edge_out->af_err_vec2.size() - 1 ]= 1.;
   
  stringstream ss_err;
    
  gen_signal_errorname( ss_err , edge_out );
}


void ErrorModel_Affine2::affine_shiftl_node( Node *CNode )
{

  Edge *edge_l = c_dag->edge_vec[ CNode->in_edges[0] ];
  Edge *edge_r = c_dag->edge_vec[ CNode->in_edges[1] ];
  Edge *edge_out = c_dag->edge_vec[ CNode->out_edges.back() ];


  edge_out->af_err_vec2.resize( affine_index );
  edge_out->af_ndx_err2 = affine_index++;
  af_ndx_map[ edge_out->af_ndx_err2] = edge_out;


  double X_l, X_r;

  X_l = X_r = 0;

  int ctr;

  Node *node_l = c_dag->node_vec[ edge_l->get_src_node() ];

  Node *node_r = c_dag->node_vec[ edge_r->get_src_node() ];

 
  X_r = pow( 2. ,  node_r->node_val);

    
  //edge_out->val = edge_l->val * X_r;
  // find_min_max( edge_out );
    
    for( ctr =  0 ; ctr < edge_out->af_err_vec2.size() ; ctr++ )
      {
	
	X_l = ( ctr < edge_l->af_err_vec2.size() ) ? edge_l->af_err_vec2[ ctr ] : 0;

	edge_out->af_err_vec2[ ctr ] = X_r * X_l;
      }
       
    edge_out->af_err_vec2[ edge_out->af_err_vec2.size() - 1 ]= 1.;
   
   stringstream ss_err;
    
   gen_signal_errorname( ss_err , edge_out );
 
    //  cout << edge_out->edge_name << " Max :" << get_range_max( edge_out ) << " Min : " << get_range_min( edge_out ) << endl;


}

void ErrorModel_Affine2::affine_shiftr_node( Node *CNode )
{

  Edge *edge_l = c_dag->edge_vec[ CNode->in_edges[0] ];
  Edge *edge_r = c_dag->edge_vec[ CNode->in_edges[1] ];
  Edge *edge_out = c_dag->edge_vec[ CNode->out_edges.back() ];


  edge_out->af_err_vec2.resize( affine_index );
  edge_out->af_ndx_err2 = affine_index++;
 

  double X_l, X_r;

  X_l = X_r = 0;

  int ctr;

  Node *node_l = c_dag->node_vec[ edge_l->get_src_node() ];

  Node *node_r = c_dag->node_vec[ edge_r->get_src_node() ];


  X_r = pow( 2. ,  -node_r->node_val);

  
  //edge_out->val = edge_l->val * X_r;
  //find_min_max( edge_out );
     
    for( ctr =  0 ; ctr < edge_out->af_err_vec2.size() ; ctr++ )
      {
	
	X_l = ( ctr < edge_l->af_err_vec2.size() ) ? edge_l->af_err_vec2[ ctr ] : 0;

	edge_out->af_err_vec2[ ctr ] = X_r * X_l;
      }
       
    edge_out->af_err_vec2[ edge_out->af_err_vec2.size() - 1 ]= 1.;
   
   stringstream ss_err;
    
   gen_signal_errorname( ss_err , edge_out );
 
    //  cout << edge_out->edge_name << " Max :" << get_range_max( edge_out ) << " Min : " << get_range_min( edge_out ) << endl;


}






void ErrorModel_Affine2::gen_signal_bwname( stringstream &ss, Edge* CEdge)
{

  stringstream temp;
  
  //gen_signal_name( temp , CEdge );

  temp << signal_names[ CEdge ];

  ss << "bw_" << temp.str();//temp.str();
  
}


void ErrorModel_Affine2::gen_signal_errorname( stringstream &ss, Edge* CEdge)
{

  stringstream tss;

  //gen_signal_name( tss , CEdge );
  //af_ndx_map[ edge_out->af_ndx_err2] = CEdge;
  
  tss << signal_names[ CEdge ];
  ss << "E_" << tss.str();
  error_names[ CEdge ] = ss.str();
  

}





void ErrorModel_Affine2::gen_signal_err_expr( stringstream &ss, Edge *CEdge )
{
 
  stringstream temp;

  gen_signal_bwname(temp , CEdge);
  ss << "2^(-" <<  temp.str() <<  rounding << ")";
  
} 



void ErrorModel_Affine2::affine_input_node( Node *CNode )
{

  Edge *edge_out = c_dag->edge_vec[ CNode->out_edges.back() ];

  stringstream err_expr;

  
  edge_out->af_err_vec2.resize( affine_index );
  edge_out->af_ndx_err2 = affine_index++;
  af_ndx_map[ edge_out->af_ndx_err2] = edge_out;
    
  edge_out->af_err_vec2[ edge_out->af_err_vec2.size() - 1 ]= 1.;

  stringstream ss_err;
    
  gen_signal_errorname( ss_err , edge_out );

}


void ErrorModel_Affine2::show_affine_vecs()
{
  vector< Edge * >::iterator e_itr;

  vector<double>::iterator a_itr;

  Edge *CEdge;

  for( e_itr = c_dag->edge_vec.begin() ; e_itr != c_dag->edge_vec.end() ; e_itr++ )
    {
      
      CEdge = (*e_itr);

      cout << endl << CEdge->edge_name << " :: " << CEdge->af_err_vec2.size() << endl;
      
      cout << "---------------------------------------------" <<endl;

     for( a_itr = CEdge->af_err_vec2.begin() ; a_itr !=  CEdge->af_err_vec2.end() ; a_itr++ )
	{
      
	  cout << (*a_itr) << " : ";


	} 
       cout << endl;
       cout << "---------------------------------------------" <<endl;
       

      //cout << CEdge->edge_name << " Max :" << get_range_max( CEdge ) << " Min : " << get_range_min( CEdge ) << endl;
      
    }


}

void ErrorModel_Affine2::gen_errvalues( stringstream &ss1 ,Edge *CEdge )
{

  vector<double>::iterator af_itr;

  int ctr = 1;

  for( af_itr = CEdge->af_err_vec2.begin(); af_itr != CEdge->af_err_vec2.end(); af_itr++ )
    {

      ss1 << error_names[ af_ndx_map[ ctr ] ] << " ";
      
      CEdge->ErrorNames_vec.push_back( error_names[ af_ndx_map[ ctr++ ] ] );

    }



}


void ErrorModel_Affine2::gen_matlab_err_func( ostream &os)
{

  map<Edge *, string>::iterator ename_itr;

  os << "function r = error_function_af( x  , bw_out ) " << endl;
  os << "% global bw_out;" << endl;
  
  os << "[no_of_bw, bw_out_ , bw_names , bw_ranges ] = parameters();" << endl;
  int ctr = 1;
  
  Edge *CEdge;

  stringstream e_ss;
  for ( ename_itr = error_names.begin() ; ename_itr != error_names.end() ; ename_itr++  )
     {

        CEdge = (*ename_itr).first;

	e_ss << (*ename_itr).second << " ";

       if ( CEdge->edge_type == IN )
	{
	   os << (*ename_itr).second << " = ";
	 
	  if (  dynamic_cast< HWfix *>(CEdge->my_var)->frac_bw_state != SET )
	    {
 	      os <<  "2^-( x(" << ctr++ << ")" << rounding << ");" << endl;
	      // 	       os <<  " = 2^-(bw_out(" << 1 << ")" << ");" << endl;

 	      // os <<  " = 2^-(bw_out_" << ");" << endl;
 	    }
	  else{
	      os << " 0;" << endl;
 	  }

	  
	}
       else if ( CEdge->edge_type == OUT )
	{

	  // Not Used 
// 	  // os <<  "2^-(" << dynamic_cast< HWfix *>(CEdge->my_var)->frac_bw  << rounding << ");" << endl;
	  
// 	  //os << "2^-(bw_out(" << bw_out_ctr++ << "));" << endl;
	}
       else{
	  os << (*ename_itr).second << " = ";
	  if (  dynamic_cast< HWfix *>(CEdge->my_var)->frac_bw_state != SET )
	    {
	      os <<  "2^-( x(" << ctr++ << ")" << rounding << ");" << endl;
	    }
	  else{
	    //os << " 0;" << endl;

	    os <<  "2^-(" << dynamic_cast< HWfix *>(CEdge->my_var)->frac_bw  << ");" << endl;
	  }

	 

       }
       

     }

  os << endl;


  // Specify output Edge BitWidths
  int bw_out_ctr = 1;
  for( vector<dag_index>::iterator e_itr = c_dag->output_edges.begin() ; e_itr != c_dag->output_edges.end() ; e_itr++ )
    {

      //os  <<  dynamic_cast< HWfix *>(c_dag->edge_vec[(*e_itr)]->my_var)->frac_bw << " ";
      os << "E_" << signal_names[ c_dag->edge_vec[(*e_itr)] ];
      
      os << " = 0; " << endl; 
      
      //os <<  " = 2^-(bw_out(" << bw_out_ctr++ << ")" <<  "+1" << ");" << endl; 
    }


  vector<dag_index>::iterator e_itr;

 
  for( e_itr = c_dag->input_edges.begin() ; e_itr != c_dag->input_edges.end() ; e_itr++ )
    {
      
      CEdge = c_dag->edge_vec[ (*e_itr ) ];

      os << signal_names[ CEdge ] << "  = " << error_names[ CEdge ] << ";" << endl;
      

    }

  os << endl;




  vector<string>::iterator a_itr;
  for( ctr = 0, e_itr = c_dag->output_edges.begin() ; e_itr != c_dag->output_edges.end() ; e_itr++ )
    {  
      
      stringstream ss,ss1;
      
      CEdge = c_dag->edge_vec[ (*e_itr) ];
 
      

      get_abs_eterms( ss , CEdge );
      os << "ErrTransFunc_" << ++ctr << " = " << ss.str() << ";" << endl;

      gen_errvalues( ss1 , CEdge );

      os << "ErrValues_" << ctr << " =  [ " << ss1.str() << " ];" <<endl;
           
    }
  
  

  os << "r = [ ";

 for ( int i = 0 ; i < ctr ; )
     {
      
       os <<  "ErrValues_" << ++i << " * ErrTransFunc_" << i << "' ";
     }


   os << "];" << endl;
  
}


