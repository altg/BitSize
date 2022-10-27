//#undef _BITSIZE_DEBUG_

#include "errormodel_affine.h"

ErrModel_Affine_LNS_CPP::ErrModel_Affine_LNS_CPP(DAG *_dag , NodeSchedular *_node_sched  ):BitSizePass( _dag ),node_sched(_node_sched),affine_index(1),affine_error_analysis_done(false){}


void ErrModel_Affine_LNS_CPP::do_affine_analysis()
{
  
  
  //bs->range_analysis->update_bw();

  if ( affine_error_analysis_done == true ) return;

   init_name_maps();
  
  _DBOUT( "Doing Affine error analysis - for LNS" );

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

void ErrModel_Affine_LNS_CPP::do_affine_analysis_node(dag_index node_num)
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





string ErrModel_Affine_LNS_CPP::get_abs_eterms( stringstream &ss , Edge *CEdge  , stringstream &eterm)
{
  
  
  //if( CEdge->edge_type != ECONSTANT ){

  stringstream ss2;

  vector<string>::iterator a_itr = CEdge->af_err_vec.begin();

   
  string e_name  ="E"+ signal_names[ CEdge ];//CEdge->edge_name;


 

  int ctr = 0;

  for(; a_itr != CEdge->af_err_vec.end(); a_itr++ )
    {

      //affine_sum += fabs(*a_itr);
      if ( (*a_itr) != "" ){
	  
	if ( (*a_itr) != "0" ){
	  //ss << e_name << "_" << ++ctr << " = fabs( " << (*a_itr) << " );" << endl;

	  ss << e_name << "[ " << ++ctr - 1<< " ] = fabs( " << (*a_itr) << " );" << endl;
	}
	else{
	  //ss << e_name << "_" << ++ctr << " = 0;" << endl;
	  ss << e_name << "[ " << ++ctr - 1<< " ] = 0;" << endl;
	}

      }
    }
     

  var_decs << "double " << e_name << "[ " << ctr << " ];\n";  

  if ( ss.str() != "" ){

    ss2 << eterm.str() << " = 0;\n";

    ss2 << "for( int index = 0 ; index < " << ctr << "; index++ ){ \n";

    ss2 << eterm.str() << " += " << e_name << " [  index  ];\n";

    ss2 << "}\n"; 

 
  }

    
  return ss2.str();

}


void ErrModel_Affine_LNS_CPP::affine_const_node_exp( Node *CNode )
{


  Edge *edge_out = c_dag->edge_vec[ CNode->out_edges.back() ];

 

  //stringstream sig_name, err_expr;

  //gen_signal_bwname( sig_name , edge_out );
  // gen_signal_err_expr( err_expr , sig_name );

  if ( edge_out->edge_type != ECONSTANT ){

    stringstream err_expr;

    gen_signal_errorname(err_expr, edge_out);
    
    edge_out->af_err_vec.resize( affine_index );
    edge_out->af_ndx_err = affine_index++; 

 
    edge_out->af_err_vec[ edge_out->af_err_vec.size() - 1 ] = err_expr.str();

 

  }


}

void ErrModel_Affine_LNS_CPP::affine_assign_node( Node *CNode )
{

  Edge *edge_in = c_dag->edge_vec[ CNode->in_edges.back()];
  
  Edge *edge_out = c_dag->edge_vec[ CNode->out_edges.back()];
  
  edge_out->af_err_vec.assign( edge_in->af_err_vec.begin() , edge_in->af_err_vec.end() );



}


void ErrModel_Affine_LNS_CPP::affine_add_node( Node *CNode )
{
  Edge *edge_l = c_dag->edge_vec[ CNode->in_edges[0] ];
  Edge *edge_r = c_dag->edge_vec[ CNode->in_edges[1] ];
  Edge *edge_out = c_dag->edge_vec[ CNode->out_edges.back() ];

  
  edge_out->af_err_vec.resize( affine_index );
  edge_out->af_ndx_err = affine_index++;


  int ctr;
 

  Node *node_l = c_dag->node_vec[ edge_l->get_src_node() ];

  Node *node_r = c_dag->node_vec[ edge_r->get_src_node() ];


  string X_l , X_r;

  string temp;
  
  //if(( node_l->node_type != CONSTANT )&& ( node_r->node_type != CONSTANT )){


    for( ctr =  0 ; ctr < edge_out->af_err_vec.size() ; ctr++ )
      {

    	string l_val , r_val;
	stringstream temp;
	stringstream temp_l, temp_r;

	

	if  ( ctr < edge_l->af_err_vec.size() )
	  {


	    l_val = edge_l->af_err_vec[ ctr ];

	  }


	  if  ( ctr < edge_r->af_err_vec.size() )
	  {

	    r_val = edge_r->af_err_vec[ ctr ];  


	  }

	  if( l_val != "" ){

	    if( dynamic_cast<HWlns *>(edge_l->my_var)->frac_bw_state == NOTSET )
	      {
		temp_l << l_val;
	      }
	    else
	      {
		temp_l << 0;
	      }
	    
	  }


	  if( r_val != "" ){

	     if( dynamic_cast<HWlns *>(edge_r->my_var)->frac_bw_state == NOTSET )
	       {
		 temp_r << r_val;
	       }
	      else
	      {
		temp_r << 0;
	      }

	  }





	  if ( ( l_val != "" ) && ( r_val != "" ) ){

	  temp << "(" <<  temp_l.str() << " + " << temp_r.str() <<  ")" ;
	 
	  }
	  else if ( ( l_val == "" ) && ( r_val != "" ) )
	    {

	      temp << temp_r.str()  ;


	    }
	   else if ( ( l_val != "" ) && ( r_val == "" ) )
	    {

	      temp << temp_l.str()  ;

	    }

	  edge_out->af_err_vec[ ctr ] = temp.str();
      }


    stringstream ss_err;
    
    gen_signal_errorname( ss_err , edge_out );
    
    edge_out->af_err_vec[ edge_out->af_err_vec.size() - 1 ] = ss_err.str();
 

  

}


void ErrModel_Affine_LNS_CPP::affine_sub_node( Node *CNode )
{
  Edge *edge_l = c_dag->edge_vec[ CNode->in_edges[0] ];
  Edge *edge_r = c_dag->edge_vec[ CNode->in_edges[1] ];
  Edge *edge_out = c_dag->edge_vec[ CNode->out_edges.back() ];

  
  edge_out->af_err_vec.resize( affine_index );
  edge_out->af_ndx_err = affine_index++;


  int ctr;
 

  Node *node_l = c_dag->node_vec[ edge_l->get_src_node() ];

  Node *node_r = c_dag->node_vec[ edge_r->get_src_node() ];


  string X_l , X_r;

  string temp;
  
  // if(( node_l->node_type != CONSTANT )&& ( node_r->node_type != CONSTANT )){


    for( ctr =  0 ; ctr < edge_out->af_err_vec.size() ; ctr++ )
      {

    	string l_val , r_val;
	stringstream temp;
	stringstream temp_l, temp_r;

	

	if  ( ctr < edge_l->af_err_vec.size() )
	  {


	    l_val = edge_l->af_err_vec[ ctr ];

	  }


	  if  ( ctr < edge_r->af_err_vec.size() )
	  {

	    r_val = edge_r->af_err_vec[ ctr ];  


	  }

	  if( l_val != "" ){

	    if( dynamic_cast<HWlns *>(edge_l->my_var)->frac_bw_state == NOTSET )
	      {
		temp_l << l_val;
	      }
	    else
	      {
		temp_l << 0;
	      }
	    
	  }


	  if( r_val != "" ){

	     if( dynamic_cast<HWlns *>(edge_r->my_var)->frac_bw_state == NOTSET )
	       {
		 temp_r << r_val;
	       }
	      else
	      {
		temp_r << 0;
	      }

	  }


	  if ( ( l_val != "" ) && ( r_val != "" ) ){

	  temp << "(" <<  temp_l.str() << " + " << temp_r.str() <<  ")" ;
	 
	  }
	  else if ( ( l_val == "" ) && ( r_val != "" ) )
	    {

	      temp  << temp_r.str() ;


	    }
	   else if ( ( l_val != "" ) && ( r_val == "" ) )
	    {

	      temp << temp_l.str()  ;

	    }

	  edge_out->af_err_vec[ ctr ] = temp.str();
      }

    
     stringstream ss_err;
    
    gen_signal_errorname( ss_err , edge_out );
    
    edge_out->af_err_vec[ edge_out->af_err_vec.size() - 1 ] = ss_err.str();
  

}

void  ErrModel_Affine_LNS_CPP::affine_div_node( Node *CNode ){
  Edge *edge_l = c_dag->edge_vec[ CNode->in_edges[0] ];
  Edge *edge_r = c_dag->edge_vec[ CNode->in_edges[1] ];
  Edge *edge_out = c_dag->edge_vec[ CNode->out_edges.back() ];

  edge_out->af_err_vec.resize( affine_index );
  edge_out->af_ndx_err = affine_index++;


  string X_l, X_r;

  X_l = X_r = "";

  for( int ctr =  0 ; ctr < edge_out->af_err_vec.size() ; ctr++ )
      {
	string l_val , r_val;
	stringstream temp;
	stringstream temp_l, temp_r;

	

	if  ( ctr < edge_l->af_err_vec.size() )
	  {


	    l_val = edge_l->af_err_vec[ ctr ];

	  }


	  if  ( ctr < edge_r->af_err_vec.size() )
	  {

	    r_val = edge_r->af_err_vec[ ctr ];  


	  }


	  if( l_val != "" ){
	    

	    double r_mid = ( bs->range_analysis->get_range_min( edge_r ) + bs->range_analysis->get_range_max( edge_r ) )/2;

	    if( dynamic_cast<HWlns *>(edge_l->my_var)->frac_bw_state == NOTSET )
	      {

		//temp_l <<  l_val << " / " << bs->range_analysis->get_range_min( edge_r );

		temp_l <<  l_val << " * ( ( 1/ " << bs->range_analysis->get_range_min( edge_r ) << ") + ";
		temp_l << "( 1/ " << bs->range_analysis->get_range_max( edge_r ) << ") )/2";
		

		//temp_l <<  l_val << " / " << r_mid;
	      }
	    else
	      {

		//temp_l << 0;

		//temp_l <<  l_val << " / " << bs->range_analysis->get_range_min( edge_r );

		temp_l <<  l_val << " * ( ( 1/ " << bs->range_analysis->get_range_min( edge_r ) << ") + ";
		temp_l << "( 1/ " << bs->range_analysis->get_range_max( edge_r ) << ") )/2";


		//temp_l <<  l_val << " / " << r_mid;
	
	      }

	  }


	  if( r_val != "" ){

	   //  double r_mid = ( bs->range_analysis->get_range_min( edge_r ) + bs->range_analysis->get_range_max( edge_r ) )/2;
// 	    double l_mid = ( bs->range_analysis->get_range_min( edge_l ) + bs->range_analysis->get_range_max( edge_l ) )/2;

	     if( dynamic_cast<HWlns *>(edge_r->my_var)->frac_bw_state == NOTSET )
	       {
		 
		 // temp_r << "-(" << bs->range_analysis->get_range_max( edge_l ) << " / ";
//                  temp_r << bs->range_analysis->get_range_min( edge_r ) << "^2)";

		 temp_r << "-( ( ( " << bs->range_analysis->get_range_min( edge_l ) << " / ";
                 temp_r << bs->range_analysis->get_range_max( edge_r ) << "^2)";


		 temp_r << " + (" << bs->range_analysis->get_range_max( edge_l ) << " / ";
                 temp_r << bs->range_analysis->get_range_min( edge_r ) << "^2) )/2 )";



		//  temp_r << "-(" << l_mid << " / ";
                // temp_r << r_mid << "^2)";


		 temp_r << " * " << r_val;

	       }
	     else
	      {

		//temp_r << 0;

		//temp_r << "-(" << l_mid << " / ";

		 // temp_r << "-(" << bs->range_analysis->get_range_max( edge_l ) << " / ";
//                  temp_r << bs->range_analysis->get_range_min( edge_r ) << "^2)";

		 temp_r << "-( ( ( " << bs->range_analysis->get_range_min( edge_l ) << " / ";
                 temp_r << bs->range_analysis->get_range_max( edge_r ) << "^2)";


		 temp_r << " + (" << bs->range_analysis->get_range_max( edge_l ) << " / ";
                 temp_r << bs->range_analysis->get_range_min( edge_r ) << "^2) )/2 )";


		//  temp_r << "-(" << l_mid << " / ";
                 // temp_r << r_mid << "^2)";


		 temp_r << " * " << r_val;
		 //temp_r << bs->range_analysis->get_range_max( edge_l ) << " * " << r_val;
		
	      }

	  }


	  if ( ( l_val != "" ) && ( r_val != "" ) ){

	  temp << "( " <<  temp_l.str() << " + " << temp_r.str() <<  " )" ;
	 
	  }
	  else if ( ( l_val == "" ) && ( r_val != "" ) )
	    {

	      temp  << temp_r.str() ;


	    }
	   else if ( ( l_val != "" ) && ( r_val == "" ) )
	    {

	      temp   << temp_l.str()  ;

	    }

	  edge_out->af_err_vec[ ctr ] = temp.str();


      }

  stringstream ss_err;
    
    gen_signal_errorname( ss_err , edge_out );
    
    edge_out->af_err_vec[ edge_out->af_err_vec.size() - 1 ] = ss_err.str();



}



double ErrModel_Affine_LNS_CPP::get_abs_maxval( Edge *CEdge ){

  return max( fabs( CEdge->max_val ) , fabs( CEdge->min_val ) );

}



void ErrModel_Affine_LNS_CPP::affine_mul_node( Node *CNode )
{


  Edge *edge_l = c_dag->edge_vec[ CNode->in_edges[0] ];
  Edge *edge_r = c_dag->edge_vec[ CNode->in_edges[1] ];
  Edge *edge_out = c_dag->edge_vec[ CNode->out_edges.back() ];


  edge_out->af_err_vec.resize( affine_index );
  edge_out->af_ndx_err = affine_index++;

  string X_l, X_r;

  X_l = X_r = "";

  int ctr;

  Node *node_l = c_dag->node_vec[ edge_l->get_src_node() ];

  Node *node_r = c_dag->node_vec[ edge_r->get_src_node() ];

  
   
    for( ctr =  0 ; ctr < edge_out->af_err_vec.size() ; ctr++ )
      {
	string l_val , r_val;
	stringstream temp;
	stringstream temp_l, temp_r;

	

	if  ( ctr < edge_l->af_err_vec.size() )
	  {


	    l_val = edge_l->af_err_vec[ ctr ];

	  }


	  if  ( ctr < edge_r->af_err_vec.size() )
	  {

	    r_val = edge_r->af_err_vec[ ctr ];  


	  }


	  if( l_val != "" ){
	    
	    if( dynamic_cast<HWlns *>(edge_l->my_var)->frac_bw_state == NOTSET )
	      {

		//temp_l << bs->range_analysis->get_range_max( edge_r ) << " * " << l_val;

		temp_l << get_abs_maxval( edge_r ) << " * " << l_val;

	      }
	    else
	      {

		//temp_l << 0;
		//temp_l << bs->range_analysis->get_range_max( edge_r ) << " * " << l_val;
		
		temp_l << get_abs_maxval( edge_r ) << " * " << l_val;
		

	      }

	  }


	  if( r_val != "" ){


	     if( dynamic_cast<HWlns *>(edge_r->my_var)->frac_bw_state == NOTSET )
	       {

		 //temp_r << bs->range_analysis->get_range_max( edge_l ) << " * " << r_val;

		 temp_r << get_abs_maxval( edge_l ) << " * " << r_val;


	       }
	     else
	      {

		//temp_r << 0;
		//temp_r << bs->range_analysis->get_range_max( edge_l ) << " * " << r_val;

	        temp_r << get_abs_maxval( edge_l ) << " * " << r_val;

		
	      }

	  }


	  if ( ( l_val != "" ) && ( r_val != "" ) ){

	  temp << "( " <<  temp_l.str() << " + " << temp_r.str() <<  " )" ;
	 
	  }
	  else if ( ( l_val == "" ) && ( r_val != "" ) )
	    {

	      temp  << temp_r.str() ;


	    }
	   else if ( ( l_val != "" ) && ( r_val == "" ) )
	    {

	      temp   << temp_l.str()  ;

	    }

	  edge_out->af_err_vec[ ctr ] = temp.str();

      }






    stringstream ss_err;
    
    gen_signal_errorname( ss_err , edge_out );
    
    edge_out->af_err_vec[ edge_out->af_err_vec.size() - 1 ] = ss_err.str();

 


}


void ErrModel_Affine_LNS_CPP::affine_shiftl_node( Node *CNode )
{
  Edge *edge_l = c_dag->edge_vec[ CNode->in_edges[0] ];
  Edge *edge_r = c_dag->edge_vec[ CNode->in_edges[1] ];
  Edge *edge_out = c_dag->edge_vec[ CNode->out_edges.back() ];

  
  edge_out->af_err_vec.resize( affine_index );
  edge_out->af_ndx_err = affine_index++;


  int ctr;
 

  // Node *node_l = c_dag->node_vec[ edge_l->get_src_node() ];

  Node *node_r = c_dag->node_vec[ edge_r->get_src_node() ];


//   string X_l , X_r;

//   string temp;
  
  
    for( ctr =  0 ; ctr < edge_out->af_err_vec.size() ; ctr++ )
      {

    	string l_val;
	stringstream temp_l;

	

	if  ( ctr < edge_l->af_err_vec.size() )
	  {

	    l_val = edge_l->af_err_vec[ ctr ];

	  }

	 if( l_val != "" ){
	    
	    if( dynamic_cast<HWlns *>(edge_l->my_var)->frac_bw_state == NOTSET )
	      {

		//temp_l << 2 << " * " << l_val;
		//temp_l << "2^" << node_r->node_val  << " * " << l_val;

		temp_l << "pow( 2. , " << node_r->node_val << " ) * " << l_val;
	      }
	    else
	      {

		temp_l << 0;
	      }

	     edge_out->af_err_vec[ ctr ] = temp_l.str();


	  }


	
      }

    stringstream err_expr;

    gen_signal_errorname(err_expr, edge_out);

    edge_out->af_err_vec[ edge_out->af_err_vec.size() - 1 ] = err_expr.str();


  //   if ( dynamic_cast<HWfix *>(edge_l->my_var)->frac_bw_state == NOTSET )
//       {
// 	 edge_out->af_err_vec[ edge_out->af_err_vec.size() - 1 ] = err_expr.str();
//       }
//     else
//       {
// 	dynamic_cast<HWfix *>(edge_out->my_var)->frac_bw = dynamic_cast<HWfix *>(edge_l->my_var)->frac_bw - 1;
// 	dynamic_cast<HWfix *>(edge_out->my_var)->frac_bw_state = SET;
//       }

}


void ErrModel_Affine_LNS_CPP::affine_shiftr_node( Node *CNode )
{
  Edge *edge_l = c_dag->edge_vec[ CNode->in_edges[0] ];
  Edge *edge_r = c_dag->edge_vec[ CNode->in_edges[1] ];
  Edge *edge_out = c_dag->edge_vec[ CNode->out_edges.back() ];

  
  edge_out->af_err_vec.resize( affine_index );
  edge_out->af_ndx_err = affine_index++;


  int ctr;
 

  // Node *node_l = c_dag->node_vec[ edge_l->get_src_node() ];

    Node *node_r = c_dag->node_vec[ edge_r->get_src_node() ];


//   string X_l , X_r;

//   string temp;
  
  
    for( ctr =  0 ; ctr < edge_out->af_err_vec.size() ; ctr++ )
      {

    
    	string l_val;
	stringstream temp_l;


	if  ( ctr < edge_l->af_err_vec.size() )
	  {

	    l_val = edge_l->af_err_vec[ ctr ];

	  }

	 if( l_val != "" ){
	    
	    if( dynamic_cast<HWlns *>(edge_l->my_var)->frac_bw_state == NOTSET )
	      {

		temp_l << "2^-" << node_r->node_val  << " * " << l_val;

	     

	      }
	    else
	      {

		temp_l << 0;
	      }

	     edge_out->af_err_vec[ ctr ] = temp_l.str();


	  }


	
      }
      
  stringstream err_expr;

  gen_signal_errorname(err_expr, edge_out);
 
  edge_out->af_err_vec[ edge_out->af_err_vec.size() - 1 ] = err_expr.str();


  //  if ( dynamic_cast<HWfix *>(edge_l->my_var)->frac_bw_state == NOTSET )
//       {
// 	 edge_out->af_err_vec[ edge_out->af_err_vec.size() - 1 ] = err_expr.str();
//       }
//     else
//       {
// 	dynamic_cast<HWfix *>(edge_out->my_var)->frac_bw = dynamic_cast<HWfix *>(edge_l->my_var)->frac_bw + 1;
// 	dynamic_cast<HWfix *>(edge_out->my_var)->frac_bw_state = SET;
//       }
 

}



void ErrModel_Affine_LNS_CPP::gen_signal_bwname( stringstream &ss, Edge* CEdge)
{

  stringstream temp;
  
  //gen_signal_name( temp , CEdge );

  temp << signal_names[ CEdge ];

  ss << "bw_" << temp.str();//temp.str();
  
}


void ErrModel_Affine_LNS_CPP::gen_signal_errorname( stringstream &ss, Edge* CEdge)
{

  stringstream tss;

  //gen_signal_name( tss , CEdge );
  
  tss << signal_names[ CEdge ];
  ss << "E_" << tss.str();
  error_names[ CEdge ] = ss.str();
  

}





void ErrModel_Affine_LNS_CPP::gen_signal_err_expr( stringstream &ss, Edge *CEdge )
{
 
  stringstream temp;

  gen_signal_bwname(temp , CEdge);
  ss << "2^(-" <<  temp.str() <<  rounding << ")";
  
} 



void ErrModel_Affine_LNS_CPP::affine_input_node( Node *CNode )
{

  Edge *edge_out = c_dag->edge_vec[ CNode->out_edges.back() ];

 

  //stringstream sig_name, err_expr;

  //gen_signal_bwname( sig_name , edge_out );
  // gen_signal_err_expr( err_expr , sig_name );

  stringstream err_expr;

  gen_signal_errorname(err_expr, edge_out);
    
  edge_out->af_err_vec.resize( affine_index );
  edge_out->af_ndx_err = affine_index++; 

  edge_out->af_err_vec[ edge_out->af_ndx_err - 1 ]= err_expr.str();

  //cout << "Node :: " << CNode->node_name << " " << edge_out->af_ndx_err << endl;
  
  // os << err_expr.str() << " = "  << ";" << endl;

}


void ErrModel_Affine_LNS_CPP::show_affine_vecs()
{
  vector< Edge * >::iterator e_itr;

  vector<string>::iterator a_itr;

  Edge *CEdge;

  for( e_itr = c_dag->edge_vec.begin() ; e_itr != c_dag->edge_vec.end() ; e_itr++ )
    {
      
      CEdge = (*e_itr);

      cout << endl << CEdge->edge_name << " :: " << CEdge->af_err_vec.size() << endl;
      
      cout << "---------------------------------------------" <<endl;

     for( a_itr = CEdge->af_err_vec.begin() ; a_itr !=  CEdge->af_err_vec.end() ; a_itr++ )
	{
      
	  cout << (*a_itr) << " : ";


	} 
       cout << endl;
       cout << "---------------------------------------------" <<endl;
       

      //cout << CEdge->edge_name << " Max :" << get_range_max( CEdge ) << " Min : " << get_range_min( CEdge ) << endl;
      
    }


}

double  ErrModel_Affine_LNS_CPP::get_emax( Edge *CEdge ){


  double max_abs_val = max( fabs( CEdge->max_val ) , fabs( CEdge->min_val ) );

  double log_max_val = ( max_abs_val == 0 ? 0 : (log( max_abs_val ) / log( 2. )));

  //double max_abs_val = max( ( CEdge->max_val ) , ( CEdge->min_val ) );

  return ( log_max_val < 0 ? ceil( log_max_val) : floor( log_max_val )) ;


}

void ErrModel_Affine_LNS_CPP::gen_matlab_err_func( ostream &os)
{

  map<Edge *, string>::iterator ename_itr;

  os << "int error_function(double *x, long int*in_para_dim, int *in_para_int_real ,USER_DEFINES * USER_OPTIONS )";

  os << "\n{";

 stringstream temp_os;

 int ctr = 1;
  
 Edge *CEdge;

 var_decs << "double ";

 
	  
  HWlns *temp_var;

  int param_ctr = 0;

  for( vector<Edge *>::iterator e_itr = c_dag->edge_vec.begin(); e_itr != c_dag->edge_vec.end();e_itr++)
    {
	
      CEdge = (*e_itr);

      if ( temp_var =  dynamic_cast< HWlns *>(CEdge->my_var) ){

      if ( CEdge->edge_type != ECONSTANT ){

     
	if ( CEdge->edge_type == IN )
	  {

	    var_decs << "E_" << temp_var->var_name << ", ";

	    temp_os << "E_" <<temp_var->var_name << " = ";


	    if (  temp_var->frac_bw_state != SET )
	      {
		//temp_os << "pow( 2. , " << get_emax( CEdge ) << " -x[ " << ctr++ - 1 << " ] );\n" ;
		temp_os << "0;\n";
		//	analyzed_edges.push_back( CEdge );
		
		//bs->param_name_map[ param_ctr++ ] = CEdge;
	      }
	    else{
	     
	      temp_os << " 0;" << endl;
	    }


	  }
	else if ( CEdge->edge_type == OUT )
	  {

	 	  
	  }
	else{
	 
	   var_decs << "E_" << temp_var->var_name << ", ";

	   temp_os << "E_" << temp_var->var_name << " = ";

	   

	  if (  temp_var->frac_bw_state != SET )
	    {
	     
	      //temp_os << "pow( 2. , " <<  get_emax( CEdge ) << " -x[ " << ctr++ - 1 << " ] );\n" ;
	      
	      double i_max = get_emax( CEdge );

	      if ( i_max < 0 ){
		//temp_os << "pow( 2. , " <<  get_emax( CEdge ) << " - pow( 2. , -x[ " << ctr++ - 1 << " ] ) );\n" ;

		temp_os << "pow( 2. , " << get_emax( CEdge ) << " + pow( 2. , -x[ " << ctr - 1  << " ] - 1 ) ) - ";

		temp_os << "pow( 2. , " << get_emax( CEdge ) << " + pow( 2. , -x[ " << ctr - 1  << " ]  ) );\n";

		ctr++;

	      }
	      else{

		//temp_os << "pow( 2. , " <<  get_emax( CEdge ) << " + pow( 2. , -x[ " << ctr++ - 1 << " ] ) );\n" ;
		 
		temp_os << "pow( 2. , " << get_emax( CEdge ) << " - pow( 2. , -x[ " << ctr - 1  << " ] - 1 ) ) - ";

		temp_os << "pow( 2. , " << get_emax( CEdge ) << " - pow( 2. , -x[ " << ctr - 1  << " ]  ) );\n";

		ctr++;

	      }
	      

	      

	      analyzed_edges.push_back( CEdge );

	      bs->param_name_map[ param_ctr++ ] = CEdge;

	    }
	  else{
	      temp_os << " 0;" << endl;
	  }


	}

      }
	
      }

    }

  temp_os << endl;

  var_decs << ";\n";


  int bw_out_ctr = 1;
  for( vector<dag_index>::iterator e_itr = c_dag->output_edges.begin() ; e_itr != c_dag->output_edges.end() ; e_itr++ )
    {
      
      temp_os << "E_" << signal_names[ c_dag->edge_vec[(*e_itr)] ];

      var_decs <<  "double E_" << signal_names[ c_dag->edge_vec[(*e_itr)] ] << ";\n";
      
      temp_os << " = 0;" << endl; 
    }


  vector<dag_index>::iterator e_itr;

 
  for( e_itr = c_dag->input_edges.begin() ; e_itr != c_dag->input_edges.end() ; e_itr++ )
    {
      
      CEdge = c_dag->edge_vec[ (*e_itr ) ];

      temp_os << signal_names[ CEdge ] << "  = " << error_names[ CEdge ] << ";" << endl;
      

    }

  temp_os << endl;




  vector<string>::iterator a_itr;
  for( ctr = 0, e_itr = c_dag->output_edges.begin() ; e_itr != c_dag->output_edges.end() ; e_itr++ )
    {  
      
      stringstream ss;
      
      string temp_s;

      CEdge = c_dag->edge_vec[ (*e_itr) ];
 
      stringstream eterm;

      eterm << "Eout[ " << ctr++ << " ]"; 

      temp_s = get_abs_eterms( ss , CEdge , eterm );
	
      temp_os << ss.str();

      temp_os << temp_s << endl;

     
      
       
    }


  //temp_os << "cout << Eout[ 0 ] << \" :: \" << USER_OPTIONS->out_err_spec[ 0 ] << endl;\n";
  
  temp_os << "if ( ";

  for ( int i = 0 ; i < ctr ; i++ ){

    temp_os << "( Eout[ " << i << " ] > USER_OPTIONS->out_err_spec[ " << i << " ] )";
    
    if ( i < ctr - 1 )
      temp_os << "\n\t && ";

  }

  temp_os << ")\n\t{return 0;}\n else\n\t{return 1;}\n"; 

  var_decs << "double Eout[ " << ctr << " ];\n";
 

  os << endl << var_decs.str() << endl;

  os << endl << temp_os.str() << endl;

  os << "\n}\n";
  
}


