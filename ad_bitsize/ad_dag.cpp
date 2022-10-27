#undef _BITSIZE_DEBUG_

#include "range_analysis.h"


ADDAG::ADDAG(DAG *_dag , NodeSchedular *_node_sched ):BitSizePass( _dag ),
			 node_sched(_node_sched),ad_index(0),iter_count(0),stop_flag( false ){}


bool ADDAG::operator()()
{
  
 

  if( iter_count > 0 )
    {

       do_ad_analysis();
       	iter_count++;

    }
  else{
    fs.open( "asctest.dat" , ios::in );
    d_out.open( "data_out.dat" , ios::out );
     init_name_maps();
        do_ad_analysis();
	iter_count++;
  }

  if( !stop_flag )
    return true;
  else
    {
      fs.close();
      d_out.close();
      return false;
   }
  
  
}



void ADDAG::do_ad_analysis()
{
  
 
  node_sched->sort_schedule();

  _DBOUT( "Doing Automatic Differentiation" )

  vector< NodeSchedular::node_vec_type >::iterator sch_itr; 
  vector< dag_index >::iterator n_itr2;

  

  int ctr = 0;
  for ( sch_itr = node_sched->schedule.begin() ; (!stop_flag) && ( sch_itr != node_sched->schedule.end()) ;  sch_itr++ )
    {

      _DBOUT( "Schedule no " << ctr++ );

      for( n_itr2 = (*sch_itr).begin() ; n_itr2 != (*sch_itr).end() ; n_itr2++ )
	{

	    do_ad_analysis_node((*n_itr2));

	}  




    }

  

}





void ADDAG::do_ad_analysis_node(dag_index node_num)
{
 
  Node* CNode = c_dag->node_vec[ node_num ];

  //  _DBOUT( CNode->node_name << " " << c_dag->Node_OPNAME[ CNode->node_type ] );

  switch( CNode->node_type )
    {

    case INPUT: ad_input_node( CNode ); break;  

    case ADD: ad_add_node( CNode ); break;

    case SUB: ad_sub_node( CNode ); break;

    case MUL: ad_mul_node( CNode ); break;

    case DIV: ad_div_node( CNode ); break;

    case SHIFTL: ad_shiftl_node( CNode ); break;

    case SHIFTR: ad_shiftr_node( CNode ); break;

    case CONSTANT: ad_const_node( CNode ); break;

    case OUTPUT: ad_output_node( CNode ); break;  

    case ASSIGN: ad_assign_node( CNode ); break;

      //case DIV: gen_div_node_exp( os , CNode ); break;


    defualt:;



    }

  


}


void ADDAG::find_min_max( Edge *CEdge )
{

  CEdge->max_val = max( CEdge->val , CEdge->max_val);
  
  //cout <<  CEdge->val << " :: " << CEdge->min_val << endl;
  CEdge->min_val = min( CEdge->val , CEdge->min_val);

 
}



void ADDAG::ad_output_node( Node *CNode )
{

  

  Edge *edge_out = c_dag->edge_vec[ CNode->in_edges.back() ];

  //cout << CNode->node_name << " :: " << edge_out->val << endl;

  d_out << edge_out->val << endl;

}


void ADDAG::ad_const_node( Node *CNode )
{

  Edge *edge_out = c_dag->edge_vec[ CNode->out_edges.back() ];

 

  if ( edge_out->edge_type != ECONSTANT ){

    
    
     if ( edge_out->ad_vec.size() == 0 ){
  
       edge_out->ad_vec.resize( ad_index + 1 );
       edge_out->ad_ndx = ad_index++;
       ad_ndx_map[ edge_out->ad_ndx] = edge_out; 
     }


 
    edge_out->ad_vec[ edge_out->ad_vec.size() - 1 ] = 1.;

    if ( iter_count == 0 )  gen_signal_errorname( edge_out );
    
    // edge_out->val = CNode->node_val;

  }
 
    edge_out->val = CNode->node_val;
    
    find_min_max( edge_out );

}

void ADDAG::ad_assign_node( Node *CNode )
{

  Edge *edge_in = c_dag->edge_vec[ CNode->in_edges.back()];
  
  Edge *edge_out = c_dag->edge_vec[ CNode->out_edges.back()];
  
  edge_out->ad_vec.assign( edge_in->ad_vec.begin() , edge_in->ad_vec.end() );

}








double ADDAG::get_const_value( Edge *CEdge )
{

  Node *CNode = c_dag->node_vec[ CEdge->get_src_node() ];

  return CNode->node_val;


}


void ADDAG::ad_shiftl_node( Node *CNode )
{

  Edge *edge_l = c_dag->edge_vec[ CNode->in_edges[0] ];
  Edge *edge_r = c_dag->edge_vec[ CNode->in_edges[1] ];
  Edge *edge_out = c_dag->edge_vec[ CNode->out_edges.back() ];


  edge_out->ad_vec.resize( ad_index + 1 );
  edge_out->ad_ndx = ad_index++;

  double X_l, X_r;

  X_l = X_r = 0;

  int ctr;

  Node *node_l = c_dag->node_vec[ edge_l->get_src_node() ];

  Node *node_r = c_dag->node_vec[ edge_r->get_src_node() ];

  //if(( edge_l->edge_type != ECONSTANT )&& ( edge_r->edge_type != ECONSTANT )){
 
  
    //X_r = get_const_value( edge_r );

   


  X_r = pow( 2. ,  node_r->node_val);

    
  edge_out->val = edge_l->val * X_r;
  find_min_max( edge_out );
    
    for( ctr =  0 ; ctr < edge_out->ad_vec.size() ; ctr++ )
      {
	
	X_l = ( ctr < edge_l->ad_vec.size() ) ? edge_l->ad_vec[ ctr ] : 0;

	edge_out->ad_vec[ ctr ] = X_r * X_l;
      }
       

 
    //  cout << edge_out->edge_name << " Max :" << get_range_max( edge_out ) << " Min : " << get_range_min( edge_out ) << endl;

    if ( iter_count == 0 )  gen_signal_errorname( edge_out );
}


void ADDAG::ad_shiftr_node( Node *CNode )
{

  Edge *edge_l = c_dag->edge_vec[ CNode->in_edges[0] ];
  Edge *edge_r = c_dag->edge_vec[ CNode->in_edges[1] ];
  Edge *edge_out = c_dag->edge_vec[ CNode->out_edges.back() ];


  edge_out->ad_vec.resize( ad_index + 1 );
  edge_out->ad_ndx = ad_index++;

  double X_l, X_r;

  X_l = X_r = 0;

  int ctr;

  Node *node_l = c_dag->node_vec[ edge_l->get_src_node() ];

  Node *node_r = c_dag->node_vec[ edge_r->get_src_node() ];

  //if(( edge_l->edge_type != ECONSTANT )&& ( edge_r->edge_type != ECONSTANT )){
 
  
    //X_r = get_const_value( edge_r );


  

  X_r = pow( 2. ,  -node_r->node_val);

  
  edge_out->val = edge_l->val * X_r;
  find_min_max( edge_out );
     
    for( ctr =  0 ; ctr < edge_out->ad_vec.size() ; ctr++ )
      {
	
	X_l = ( ctr < edge_l->ad_vec.size() ) ? edge_l->ad_vec[ ctr ] : 0;

	edge_out->ad_vec[ ctr ] = X_r * X_l;
      }
       
    if ( iter_count == 0 )  gen_signal_errorname( edge_out );
 
    //  cout << edge_out->edge_name << " Max :" << get_range_max( edge_out ) << " Min : " << get_range_min( edge_out ) << endl;


}




void ADDAG::ad_add_node( Node *CNode )
{
  Edge *edge_l = c_dag->edge_vec[ CNode->in_edges[0] ];
  Edge *edge_r = c_dag->edge_vec[ CNode->in_edges[1] ];
  Edge *edge_out = c_dag->edge_vec[ CNode->out_edges.back() ];

  if ( edge_out->ad_vec.size() == 0 ){
  
    edge_out->ad_vec.resize( ad_index + 1 );
    edge_out->ad_ndx = ad_index++;
    ad_ndx_map[ edge_out->ad_ndx] = edge_out;
  }

   edge_out->val = edge_l->val + edge_r->val;
   find_min_max( edge_out );
  int ctr;

  double X_l, X_r;
  
  X_l = X_r = 0;

  //Node *node_l = c_dag->node_vec[ edge_l->get_src_node() ];

  //Node *node_r = c_dag->node_vec[ edge_r->get_src_node() ];

  

  for( ctr =  0 ; ctr < edge_out->ad_vec.size() ; ctr++ )
    {
	
      X_l = ( ctr < edge_l->ad_vec.size() ) ? edge_l->ad_vec[ ctr ] : 0;

      X_r = ( ctr < edge_r->ad_vec.size() ) ? edge_r->ad_vec[ ctr ] : 0;

      edge_out->ad_vec[ ctr ] = X_l + X_r;
    }


 edge_out->ad_vec[ edge_out->ad_vec.size() - 1 ]= 1.;
  
 if ( iter_count == 0 )  gen_signal_errorname( edge_out );
}


void ADDAG::ad_sub_node( Node *CNode )
{
  Edge *edge_l = c_dag->edge_vec[ CNode->in_edges[0] ];
  Edge *edge_r = c_dag->edge_vec[ CNode->in_edges[1] ];
  Edge *edge_out = c_dag->edge_vec[ CNode->out_edges.back() ];

  
 
   if ( edge_out->ad_vec.size() == 0 ){
  
    edge_out->ad_vec.resize( ad_index + 1 );
    edge_out->ad_ndx = ad_index++;
    ad_ndx_map[ edge_out->ad_ndx] = edge_out;
  }


   edge_out->val = edge_l->val - edge_r->val;
   find_min_max( edge_out );
   
  int ctr;

  double X_l, X_r;
  
  X_l = X_r = 0;


  Node *node_l = c_dag->node_vec[ edge_l->get_src_node() ];

  Node *node_r = c_dag->node_vec[ edge_r->get_src_node() ];

  for( ctr =  0 ; ctr < edge_out->ad_vec.size() ; ctr++ )
      {
	
	X_l = ( ctr < edge_l->ad_vec.size() ) ? edge_l->ad_vec[ ctr ] : 0;

	X_r = ( ctr < edge_r->ad_vec.size() ) ? edge_r->ad_vec[ ctr ] : 0;

	edge_out->ad_vec[ ctr ] = X_l - X_r;
      }

    edge_out->ad_vec[ edge_out->ad_vec.size() - 1 ]= 1.;

    if ( iter_count == 0 )  gen_signal_errorname( edge_out );

}





double ADDAG::get_abs_eterms( Edge *CEdge )
{

  double ad_sum  = 0;
  //if( CEdge->edge_type != ECONSTANT ){

    vector<double>::iterator a_itr = CEdge->ad_vec.begin();

   
    for(; a_itr != CEdge->ad_vec.end(); a_itr++ )
      {

	ad_sum += fabs(*a_itr);

      }
     
    //}


  return ad_sum;


}


void ADDAG::ad_mul_node( Node *CNode )
{


  Edge *edge_l = c_dag->edge_vec[ CNode->in_edges[0] ];
  Edge *edge_r = c_dag->edge_vec[ CNode->in_edges[1] ];
  Edge *edge_out = c_dag->edge_vec[ CNode->out_edges.back() ];


  if ( edge_out->ad_vec.size() == 0 ){
  
    edge_out->ad_vec.resize( ad_index + 1 );
    edge_out->ad_ndx = ad_index++;
    ad_ndx_map[ edge_out->ad_ndx ] = edge_out;
  }

  //cout << "Mul " << edge_l->val << " " << edge_r->val << endl; 

   edge_out->val = edge_l->val * edge_r->val;
   find_min_max( edge_out );

   double l_val = ( edge_l->edge_type != ECONSTANT )? edge_l->val :  0;
   double r_val = ( edge_r->edge_type != ECONSTANT )? edge_r->val :  0;


  double X_l, X_r;

  X_l = X_r = 0;

  int ctr;

  Node *node_l = c_dag->node_vec[ edge_l->get_src_node() ];

  Node *node_r = c_dag->node_vec[ edge_r->get_src_node() ];

  for( ctr =  0 ; ctr < edge_out->ad_vec.size() ; ctr++ )
      {
	
	     
	X_l = ( ctr < edge_l->ad_vec.size() ) ? edge_l->ad_vec[ ctr ] : 0;

	X_r = ( ctr < edge_r->ad_vec.size() ) ? edge_r->ad_vec[ ctr ] : 0;

	edge_out->ad_vec[ ctr ] = r_val * X_l + l_val * X_r;
      }



   edge_out->ad_vec[ edge_out->ad_vec.size() - 1 ]= 1.;

   if ( iter_count == 0 )  gen_signal_errorname( edge_out );
}


void ADDAG::ad_div_node( Node *CNode ){

 Edge *edge_l = c_dag->edge_vec[ CNode->in_edges[0] ];
  Edge *edge_r = c_dag->edge_vec[ CNode->in_edges[1] ];
  Edge *edge_out = c_dag->edge_vec[ CNode->out_edges.back() ];


  if ( edge_out->ad_vec.size() == 0 ){
  
    edge_out->ad_vec.resize( ad_index + 1 );
    edge_out->ad_ndx = ad_index++;
    ad_ndx_map[ edge_out->ad_ndx] = edge_out;
  }

  //cout << "Mul " << edge_l->val << " " << edge_r->val << endl; 

   edge_out->val = edge_l->val / edge_r->val;

   find_min_max( edge_out );

  
 double X_l, X_r;

  X_l = X_r = 0;

  int ctr;

  Node *node_l = c_dag->node_vec[ edge_l->get_src_node() ];

  Node *node_r = c_dag->node_vec[ edge_r->get_src_node() ];


  //double l_val = (node_l->node_type != CONSTANT )? edge_l->val :  node_l->node_val;
  //double r_val = (node_r->node_type != CONSTANT )? edge_r->val :  node_r->node_val;


   double l_val = ( edge_l->edge_type != ECONSTANT )? edge_l->val :  0;
   double r_val = ( edge_r->edge_type != ECONSTANT )? edge_r->val :  0;


  for( ctr =  0 ; ctr < edge_out->ad_vec.size() ; ctr++ )
      {
	
	     
	X_l = ( ctr < edge_l->ad_vec.size() ) ? edge_l->ad_vec[ ctr ] : 0;

	X_r = ( ctr < edge_r->ad_vec.size() ) ? edge_r->ad_vec[ ctr ] : 0;

	edge_out->ad_vec[ ctr ] = (r_val * X_l - l_val * X_r)/( r_val * r_val );
      }



   edge_out->ad_vec[ edge_out->ad_vec.size() - 1 ]= 1.;

    if ( iter_count == 0 )  gen_signal_errorname( edge_out );
}

double ADDAG::read_from_file()
{

  double temp;

  if( (fs >> temp) == 0 ){

    stop_flag = true;
    return -1;
  } 
  else
    {
      //cout << temp << endl;
      return temp;
    }

}


void ADDAG::ad_input_node( Node *CNode )
{

  Edge *edge_out = c_dag->edge_vec[ CNode->out_edges.back() ];
  
  
  if ( edge_out->ad_vec.size() == 0 ){
    edge_out->ad_vec.resize( ad_index + 1 );
    edge_out->ad_ndx = ad_index++; 
    ad_ndx_map[ edge_out->ad_ndx ] = edge_out;
  }

  edge_out->ad_vec[ edge_out->ad_vec.size() - 1 ]= 1.;
  //Read in inputs from file here

  double temp = read_from_file();

  if( !stop_flag ){
    edge_out->val = temp;
    find_min_max( edge_out );
  }
  else{
    edge_out->val = 0.;
  }

  

  //cout << edge_out->edge_name << " :: " << temp << endl;
  
  if ( iter_count == 0 )  gen_signal_errorname( edge_out );

}


void ADDAG::show_ad_vecs()
{
  vector< Edge * >::iterator e_itr;

  vector<double>::iterator a_itr;

   vector<double>::iterator max_a_itr;

  Edge *CEdge;

   cout << "---------------------------------------------" <<endl;

   for ( map<int, Edge*>::iterator t_itr = ad_ndx_map.begin();  t_itr != ad_ndx_map.end() ; t_itr++ )
     {
       cout << (*t_itr).first << " " << (*t_itr).second->edge_name << endl; 


     }
    
   for( e_itr = c_dag->edge_vec.begin() ; e_itr != c_dag->edge_vec.end() ; e_itr++ )
   //for( e_itr = c_dag->output_edges.begin() ; e_itr != c_dag->output_edges.end() ; e_itr++ )
   {
      
       if ( (*e_itr)->edge_type != ECONSTANT ){
	 CEdge = (*e_itr);

	 cout << (*e_itr)->edge_name << " , " << CEdge->ad_ndx << " :: ";

	 //cout << bs->range_analysis->range_bw_flt(CEdge) << " :: ";
	 //cout << bs->range_analysis->get_range_max(CEdge) << " :: ";
	 //cout << bs->range_analysis->get_range_min(CEdge) << " :: ";

	 int ctr = 0;
	 //for( a_itr = CEdge->ad_vec.begin() ; a_itr !=  CEdge->ad_vec.end() ; a_itr++, ctr++ )
	  for( a_itr = CEdge->max_ad_vec.begin() ; a_itr !=  CEdge->max_ad_vec.end() ; a_itr++, ctr++ )
	   {
       
	     //if ( ctr < 7 )
	     cout << "( " << ad_ndx_map[ ctr ]->edge_name << " , " <<  (*a_itr) << ") : ";


	   } 

	 cout << endl;
           
	 cout << "---------------------------------------------" <<endl;
       }
     }


}


void ADDAG::gen_func_body( ostream &os ){

  
  vector<double>::iterator a_itr;

  int ctr;

  Edge *CEdge;

  for( vector<dag_index>::iterator itr = c_dag->output_edges.begin(); itr != c_dag->output_edges.end();
       itr++ )
    {
      
      CEdge =  c_dag->edge_vec[ (*itr)];

      //os << CEdge->edge_name << " , " << CEdge->ad_ndx << " :: ";

      os << CEdge->edge_name  << " = ";
	 
      ctr = 0;
      for( a_itr = CEdge->max_ad_vec.begin(); a_itr !=  CEdge->max_ad_vec.end(); a_itr++, ctr++ )
	{
       
	  //if ( ctr < 7 )
	  os << "( " << ad_ndx_map[ ctr ]->edge_name << " * " <<  (*a_itr) << " ) ";

	  if ( a_itr !=  CEdge->max_ad_vec.end() - 1 )
	    os << " + ";


	} 

      os << ";" << endl;
           
      //os << "---------------------------------------------" <<endl;
       
    }



}

void ADDAG::gen_signal_errorname( Edge* CEdge )
{

  stringstream ss,tss;
    
  tss << signal_names[ CEdge ];
  ss << "E_" << tss.str();
  error_names[ CEdge ] = ss.str();
  

}

void ADDAG::gen_errvalues( stringstream &ss1 ,Edge *CEdge )
{

  vector<double>::iterator ad_itr;

  int ctr = 0;

  for( ad_itr = CEdge->max_ad_vec.begin(); ad_itr != CEdge->max_ad_vec.end(); ad_itr++ )
    {

      ss1 << error_names[  ad_ndx_map[ ctr++ ] ] << " ";

    }



}


void ADDAG::get_abs_eterms( stringstream &ss , Edge *CEdge )
{
  
  
  //if( CEdge->edge_type != ECONSTANT ){

  stringstream ss2;

    vector<double>::iterator a_itr = CEdge->max_ad_vec.begin();

   
    // string e_name  = CEdge->edge_name;

    //int ctr = 0;

    ss << "[ ";

    for(; a_itr != CEdge->max_ad_vec.end(); a_itr++ )
      {


	ss << (*a_itr) << " ";


      }

    ss << "] ";


 
    
}


void ADDAG::gen_matlab_err_func( ostream &os)
{

  map<Edge *, string>::iterator ename_itr;

  os << "function r = error_function( x ) " << endl;
  os << "global bw_out;" << endl;
  
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
	    // os << " 0;" << endl;

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
      os << "E_" << signal_names[ c_dag->edge_vec[(*e_itr)] ] <<  " = 2^-(bw_out(" << bw_out_ctr++ << ")" <<  "+1" << ");" << endl; 
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
