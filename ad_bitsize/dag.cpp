// $Author: amag97 $
// $Id: dag.cpp,v 1.21 2006/02/14 19:07:54 amag97 Exp $
// $Log: dag.cpp,v $
// Revision 1.21  2006/02/14 19:07:54  amag97
// Added new Pass for generating glitch based power cost function
// Bug Fixes - conv. between range and bitwidth
//
// Revision 1.20  2004/12/13 01:33:43  amag97
// Added command line argument parsing
// Added Interval based range analysis
// Added ASSIGN node to dag
//
// Revision 1.19  2004/11/28 23:29:02  amag97
// Added back support for HWfloat
// Uniform bit-width optimisation for HWfloat
//
// Revision 1.18  2004/10/26 18:32:59  amag97
// Fixed most bugs with fixed-point frac. opt 6
//
// Revision 1.17  2004/10/23 10:37:34  amag97
// Addition of new fraction bw evaluation method
// (work in progress)
//
// Revision 1.16  2004/10/05 19:23:56  amag97
// Fixes to AD code - bug when multiple inputs are used
// Updated Graph info
//
// Revision 1.15  2004/09/30 20:32:51  amag97
// Added Area model
// Lots of fixes to get it to work
//
// Revision 1.14  2004/09/17 16:52:13  amag97
// Added area estimation code
//
// Revision 1.13  2004/09/10 17:21:55  amag97
// Changes to bwopt method 3
//
// Revision 1.12  2004/08/31 19:10:06  amag97
// Fixes to bwopt method 3
//
// Revision 1.11  2004/08/26 18:00:52  amag97
// Update after return from Sri Lanka
// Started work on adding user constraints
//
// Revision 1.10  2004/07/09 10:22:21  amag97
// More fixes to DAG code
//
// Revision 1.9  2004/07/06 19:48:11  amag97
// Revised DAG generation
//
// Revision 1.8  2004/06/24 18:45:31  amag97
// Added a seperate class to encapsulate the bitwidth optimisation functions
//
// Revision 1.7  2004/06/09 21:20:51  amag97
// Added more bitwidth analysis methods for fixed-point
//
// Revision 1.6  2004/06/07 19:51:08  amag97
// Global HWvector collection
// Bugfixes to copy constructor
//
// Revision 1.5  2004/06/03 22:01:30  amag97
// More bug fixes
// Added HWint
//
// Revision 1.4  2004/06/02 19:37:59  amag97
// Added HWvector
//
// Revision 1.3  2004/06/01 19:31:17  amag97
// More fixes to DAG code
//
// Revision 1.2  2004/06/01 11:31:55  amag97
// DAG code
//
// Revision 1.1  2004/05/28 20:14:27  amag97
// ASC Integration
//

#include "dag.h"



// varnames edge_names( "EDGE" , "" , 0 , varnames::NUMERIC );
// varnames node_names( "NODE" , "" , 0 , varnames::NUMERIC );
// varnames const_names( "CONST" , "" , 0 ,  varnames::NUMERIC );
// varnames hwint_names( "HWINT" , "" , 0 , varnames::NUMERIC );

varnames *edge_names;
varnames *node_names;
varnames *const_names;
varnames *hwint_names;
varnames *assign_names;

DAG::DAG()
{

  node_count = 0;
  edge_count = 0;

  // Node OP type translation MAP
  Node_OPNAME[ INPUT ] = "IN_NODE";
  Node_OPNAME[ OUTPUT ] = "OUT_NODE";
  Node_OPNAME[ ADD ] = "ADD_NODE";
  Node_OPNAME[ SUB ] = "SUB_NODE";
  Node_OPNAME[ MUL ] = "MUL_NODE";
  Node_OPNAME[ DIV ] = "DIV_NODE";
  Node_OPNAME[ SHIFTL ] = "<<_NODE";
  Node_OPNAME[ SHIFTR ] = ">>_NODE";
  Node_OPNAME[ MOD ] = "MOD_NODE";
  Node_OPNAME[ CONSTANT ] = "CONST_NODE";
  Node_OPNAME[ HWINT ] = "HWINT_NODE";
  Node_OPNAME[ START ] = "START_NODE";
  Node_OPNAME[ END ] = "END_NODE";


  Edge_TYPENAME[ IN ] = "IN";
  Edge_TYPENAME[ OUT ] = "OUT";
  Edge_TYPENAME[ TMP ] = "TMP";

  edge_names = new varnames( "EDGE" , "" , 0 , varnames::NUMERIC );
  node_names = new varnames( "NODE" , "" , 0 , varnames::NUMERIC );
  const_names = new varnames( "CONST" , "" , 0 ,  varnames::NUMERIC );
  hwint_names = new varnames( "HWINT" , "" , 0 , varnames::NUMERIC );
  assign_names = new varnames( "ASSIGN" , "" , 0 , varnames::NUMERIC );

  start_node = new_node( START , AT_NOTSET );
  end_node = new_node( END , AT_NOTSET );

  arith_type =  AT_NOTSET;

}


// Collect similar type of edges in to a list 
void DAG::add_to_list( Node_OPTYPE node_type , vector<dag_index> &edge_list )
{

  // Use a multimap to sort the nodes in to different groups
  pair< multimap<Node_OPTYPE , Node*>::iterator , multimap<Node_OPTYPE , Node*>::iterator > f_res;

  f_res = Node_opmap.equal_range( node_type );

  Node *c_node;
  Edge *c_edge;

  vector<dag_index>::iterator edge_itr;


      for( ;f_res.first != f_res.second ; f_res.first++ )
	  {

	    c_node = f_res.first->second;
	 
	    for( edge_itr = c_node->in_edges.begin(); edge_itr != c_node->in_edges.end() ; edge_itr++ )
	      {

		edge_list.push_back(  *edge_itr  );
	       
	
	      }
	    

	   	    
	  }


     
 
}


// Sort the edges in to seperate lists, this is useful for the
// optimisation phase.

void DAG::sort_edges()
{

  if ( bs->global_opmode == PARSING )
    {

      add_to_list( INPUT , input_edges );
      add_to_list( ADD , add_edges );
      add_to_list( SUB , sub_edges );
      add_to_list( MUL , mul_edges );
      add_to_list( DIV , div_edges );
      
      add_to_list( OUTPUT , output_edges );
  
    }


  

}

Node* DAG::new_node( Node_OPTYPE n_type , Node_Arithmetic_TYPE n_atype, string name  )
{
  
  string node_name =  ( name == "" ) ? node_names->gen_varname() : name;
  
  Node *temp = new Node( n_type , n_atype , node_name , node_count++ );

  
  //Node_opmap[ n_type ] = temp;

  Node_opmap.insert( pair< Node_OPTYPE , Node*>( n_type , temp ) );

  node_vec.push_back( temp );

  return temp;


}

Node* DAG::new_const_node( Node_Arithmetic_TYPE n_atype,  double c_val , string name )
{
  string node_name = ( name == "" ) ? const_names->gen_varname() : name;

  Node* temp = new Node( CONSTANT ,n_atype ,  node_name , node_count++ , c_val);

  node_vec.push_back( temp );

  return temp;
}


Node* DAG::new_assign_node( Edge *in_edge , Edge *out_edge )
{

  string node_name = assign_names->gen_varname();

  Node* temp = new Node( ASSIGN , INTEGER , node_name , node_count++ );

  //in_edge->dst_nodes.push_back( temp->node_number );

  //out_edge->src_nodes.push_back( temp->node_number );

  node_vec.push_back( temp );

  add_in_edge( temp , in_edge );

  add_out_edge( temp , out_edge );


}

Node* DAG::new_hwint_node( string name )
{

  string node_name = ( name == "" ) ? hwint_names->gen_varname() : name;

  Node* temp = new Node( HWINT , INTEGER , node_name , node_count++ );
  
  node_vec.push_back( temp  );

  return temp;

}

// dag_index DAG::new_edge( HWvar *_edge )
// {

//   _edge->var_number = edge_count++;

//   edge_vec.push_back( _edge );

//   //_edge->src_nodes.push_back( start_node );

//   add_out_edge( start_node , _edge->var_number );

//   add_in_edge( end_node , _edge->var_number );

//   //_edge->dst_nodes.push_back( end_node );

//   return _edge->var_number;

// }

Edge* DAG::new_edge(HWvar *var , string name )
{

  Edge* temp = new_edge( start_node , end_node , name );

  temp->my_var = var;
  
  return temp;

}

Edge* DAG::new_edge( Node* src , Node* dst , string name  )
{

  string edge_name =  ( name == "" ) ? edge_names->gen_varname() : edge_names->gen_varname( name );

  Edge* temp = new Edge( TMP , edge_name , edge_count++ );

  edge_vec.push_back( temp );
  
  add_out_edge( src , temp );

  add_in_edge( dst , temp );

  temp->my_var = NULL;

  return edge_vec.back();
}




Edge* DAG::new_const_edge( double val , string name )
{
  Node* temp_n = new_const_node( AT_NOTSET , val );

  Edge* temp_e = new_edge( temp_n , end_node , "CONST" );
  
  temp_e->edge_type = ECONSTANT;

  //bs->end_parse();
  temp_e->my_var = new HWfix( );
  //bs->start_parse();

  //*(dynamic_cast<HWfix *>(temp_e->my_var)).ad_val = val;

  temp_e->my_var->ad_val = val;

  return temp_e;

}

Edge* DAG::new_hwint_edge( string name )
{
  Node* temp_n = new_hwint_node( name );

  Edge* temp_e = new_edge( temp_n  , end_node , "HWINT" );

  temp_e->my_var = new HWfix( TMP );

  // *(dynamic_cast<HWfix *>(temp_e->my_var)) = val;

  return temp_e;

}


// Node: append output edge 
// Edge: append source node
Edge* DAG::add_out_edge( Node* node , Edge* edge )
{
  //Node *temp_node = node_vec[ node_n ];

  node->out_edges.push_back( edge->edge_number );

  edge->src_nodes.push_back( node->node_number );
  
  return edge;


}

// Node: append input edge
// Edge: append destination node
Edge* DAG::add_in_edge( Node* node , Edge* edge )
{
  //Node *temp_node = node_vec[ node_n ];
  
  
  node->in_edges.push_back( edge->edge_number );
  
  edge->dst_nodes.push_back( node->node_number );

  return edge;


}





// void DAG::start_parse()
// {

//   global_opmode = PARSING;
//   //global_opmode = PARSED;
// }

// void DAG::end_parse()
// {
  
//   global_opmode = PARSED;

// }


Node* DAG::add_op_node( Node_OPTYPE n_type , Node_Arithmetic_TYPE n_atype, Edge* l, Edge* r )
{

  Node* op_node = new_node( n_type , n_atype );

  add_in_edge( op_node , l );

  add_in_edge( op_node , r );

  return op_node;


}



void DAG::print_dag()
{

  vector<Node *>::iterator node_itr;
  vector<Edge *>::iterator edge_itr; 

  vector<dag_index>::iterator itr;

  node_itr = node_vec.begin();

  while( node_itr != node_vec.end() )
    {
      cout << (*node_itr)->node_name << " : " << Node_OPNAME[(*node_itr)->node_type] <<  " : [" ;
      
     itr = (*node_itr)->in_edges.begin();
 
      while( itr != (*node_itr)->in_edges.end() )
	{
	  cout << (edge_vec[ (*itr++) ])->edge_name << " , ";
	  //cout << (*itr++) << " , "; 
	}  

  
      cout << "] : [";

      itr = (*node_itr)->out_edges.begin();
 
      while( itr != (*node_itr)->out_edges.end() )
	{
	  cout << (edge_vec[(*itr++)])->edge_name << " , ";
	  //cout << (*itr++) << " , "; 
	}     
  
      cout << "] " << endl ;
    
       
      node_itr++;
    }

 
    edge_itr = edge_vec.begin();

  while( edge_itr != edge_vec.end() )
    {
      cout << (*edge_itr)->edge_name << " : [";

     
      
      itr = (*edge_itr)->src_nodes.begin();

      while( itr != (*edge_itr)->src_nodes.end() )
	{
	  cout << (node_vec[(*itr++)])->node_name << " , ";
	}

      cout << "] : [";

      itr = (*edge_itr)->dst_nodes.begin();

      while( itr != (*edge_itr)->dst_nodes.end() )
	{
	  cout << (node_vec[(*itr++)])->node_name << " , ";
	}

      
      cout << "]" << endl;

      edge_itr++;
    }
  
}

void DAG::print_dot_graph( ostream& os )
{
  vector<Node *>::iterator node_itr = node_vec.begin();
  vector<dag_index>::iterator edge_itr;

  os << "digraph G {" << endl;

  while ( node_itr != node_vec.end() )
   {

     os << (*node_itr)->node_name << " " << "[ label = \""  << Node_OPNAME[(*node_itr)->node_type];
     
     // Output nodes
     if ( (*node_itr)->node_type == CONSTANT )
       os << "\\n" << (*node_itr)->node_val; 
     
     os << "\"];" << endl;
     // Output nodes and edges
     if ( ( (*node_itr)->node_type != OUTPUT ) && ( (*node_itr)->node_type != END  )  && ( (*node_itr)->node_type != START  ) )
       {
	 edge_itr = edge_vec[(*node_itr)->out_edges.back()]->dst_nodes.begin();
	 
	 while( edge_itr != edge_vec[(*node_itr)->out_edges.back()]->dst_nodes.end() ){
	   
	   //os << (*node_itr)->node_name << " -> " << node_vec[edge_vec[(*node_itr)->out_edges.back()]->get_dst_node()]->node_name;
	   if ( node_vec[ (*edge_itr) ]->node_type != END  ){
	     os << (*node_itr)->node_name << " -> " << node_vec[ (*edge_itr) ]->node_name;
	     os << " [ label = \""<< edge_vec[(*node_itr)->out_edges.back()]->edge_name << "\\n";
	   //os << " [ label = \""<< edge_vec[(*edge_itr)]->edge_name << "\\n";

	 //os << edge_vec[(*node_itr)->out_edges.back()]->print_dot_bitinfo() << "\"";
	//  if ( edge_vec[(*node_itr)->out_edges.back()]->var_dectype == HWvar::TMPVAR )
// 	   os << ", color = yellow"; 
	 
	     os << "\"];" << endl;}
	 edge_itr++;
	 }
       }

     node_itr++;
   }


  os << "}" << endl;


}


void DAG::print_dot_graph2( ostream& os )
{
  vector<Node *>::iterator node_itr = node_vec.begin();
  vector<dag_index>::iterator edge_itr;
  vector<dag_index>::iterator in_edge_itr;

  vector<dag_index>::iterator src_node_itr;

  os << "digraph G {" << endl;

  Node *srcNode;

  while ( node_itr != node_vec.end() )
   {

     //os << (*node_itr)->node_name << " " << "[ label = \""  << Node_OPNAME[(*node_itr)->node_type];
     os << (*node_itr)->node_name << " " << "[ label = \""  << Node_OPNAME[(*node_itr)->node_type] << " "<<(*node_itr)->node_name;
     // Output nodes
     if ( (*node_itr)->node_type == CONSTANT )
       os << "\\n" << (*node_itr)->node_val; 
     
     os << "\"];" << endl;
     
     // Output nodes and edges
     if ( ( (*node_itr)->node_type != END  )  && ( (*node_itr)->node_type != START  ) && ( (*node_itr)->node_type != ASSIGN  ) )
       {
	 
	 for( in_edge_itr = (*node_itr)->in_edges.begin() ; in_edge_itr != (*node_itr)->in_edges.end() ; in_edge_itr++ )
	 {

	   //for( src_node_itr = edge_vec[(*in_edge_itr)]->src_nodes.begin() ;  src_node_itr != edge_vec[(*in_edge_itr)]->src_nodes.end();src_node_itr++ )
	   //  {

	   srcNode = node_vec[ edge_vec[(*in_edge_itr)]->src_nodes.back() ];

	   if ( srcNode->node_type == ASSIGN ){

	     srcNode = node_vec[ edge_vec[srcNode->in_edges.back()]->get_src_node() ];
	   }

	   os << srcNode->node_name << "->";
	   //os << node_vec[ edge_vec[(*in_edge_itr)]->src_nodes.back() ]->node_name << "->";

	       // if (node_vec[ *src_node_itr ]->node_type != START  ){

	       //os << node_vec[ *src_node_itr ]->node_name << "->";

	       os << (*node_itr)->node_name;

	       os << " [ label = \"" << edge_vec[(*in_edge_itr)]->edge_name <<  "\\n"; 

	      //  for ( vector<double>::iterator e_itr = edge_vec[(*in_edge_itr)]->grad_vec.begin(); e_itr != edge_vec[(*in_edge_itr)]->grad_vec.end() ; e_itr++ ){

// 		 os << (*e_itr) << ":";

// 	       }

	       os << "FBW # " << edge_vec[(*in_edge_itr)]->f_bw;

	       os << "\"];" << endl;
	       //  }
	       // }
	 }
	 
	 

	
       }

     node_itr++;
   }


  os << "}" << endl;


}


// void DAG::print_dot_graph3( ostream& os )
// {

//   vector<Node *>::iterator node_itr;
//   vector<Edge *>::iterator edge_itr;

//   // Output all the nodes

//   for( node_itr  = node_vec.begin(); node_itr != node_vec.end() ; node_itr++ )
//     {

//       os << (*node_itr)->node_name << " " << "[ label = \""  << Node_OPNAME[(*node_itr)->node_type];
     
   
//      if ( (*node_itr)->node_type == CONSTANT )
//        os << "\\n" << (*node_itr)->node_val; 
     
//      os << "\"];" << endl;


//     }


//   // Output all the edges

//   Node* src_node,dst_node;

//   for( edge_itr = edge_vec.begin(); edge_itr != edge_vec.end() ; edge_itr++ )
//     {

//       src_node = node_vec[ (*edge_itr)->src_nodes.back() ];

      

//     }


// }


void DAG::update_edge_grads()
{


  list<HWvar *>::iterator outvar_itr; 


  int ctr,ctr2;
  for ( ctr = 0, outvar_itr = bs->varC.outvar_c.begin();  outvar_itr != bs->varC.outvar_c.end() ; outvar_itr++ , ctr++)
    {
  
      //(*outvar_itr)->ad_val.print_grads_vec();
      ctr2 = 0;
      for (  vector<Edge *>::iterator e_itr = edge_vec.begin(); e_itr != edge_vec.end() ; e_itr++, ctr2++){

	//cout << (*outvar_itr)->ad_val.grads[ctr2 ] << " : "; 

	if  ( (*e_itr)->grad_vec.size() < ctr + 1 ){
	  (*e_itr)->grad_vec.push_back( (*outvar_itr)->ad_val.grads[ (*e_itr)->edge_number ] );
	  //(*e_itr)->grad_vec.push_back(  (*e_itr)->edge_number  );
	}
	else{
	  //(*e_itr)->grad_vec[ ctr ] = (*e_itr)->edge_number;
	  (*e_itr)->grad_vec[ ctr ] = (*outvar_itr)->ad_val.grads[ (*e_itr)->edge_number ];
	}


      }


      // cout << endl;
    }

}


void DAG::show_edge_grads()
{


 for ( vector<Edge *>::iterator e_itr = edge_vec.begin(); e_itr != edge_vec.end() ; e_itr++ ){

	
	cout <<  (*e_itr)->edge_name << " " <<  (*e_itr)->edge_name << " : ";

	for ( vector<double>::iterator g_itr = (*e_itr)->grad_vec.begin(); g_itr != (*e_itr)->grad_vec.end(); g_itr++ ){


	  cout << (*g_itr) << " : ";


	}


	cout << endl;

      }


}


void DAG::list_edges( dag_index start )
{

  Edge *c_edge = edge_vec[ start ];

  Node *c_node = node_vec[ c_edge->get_src_node() ];

  Node_OPTYPE src_ntype = c_node->node_type;

  if( ( src_ntype  == CONSTANT ) || ( src_ntype == INPUT ) )
    {

      cout << c_edge->edge_name << endl;

    }
    else{

      
      for( vector<dag_index>::iterator e_iter = c_node->in_edges.begin() ; e_iter != c_node->in_edges.end(); e_iter++ )
	{

	  list_edges( *(e_iter) );
	  cout << c_edge->edge_name << endl;
	  
	  
	}
  


    }



}


Node* DAG::get_src_node( Edge *in_edge ){


  return (node_vec[ in_edge->get_src_node() ]);


}



// void DAG::propagate_error( dag_index start  , double m_err)
// {

//   Edge *c_edge = edge_vec[ start ];

//   Node *src_node = node_vec[ c_edge->get_src_node() ];

//   Node_OPTYPE src_ntype = src_node->node_type;

//   if( ( src_ntype  == CONSTANT ) || ( src_ntype == INPUT ) )
//     {

//       if ( src_ntype == INPUT ){

// 	//c_edge->f_bw = dynamic_cast<HWfix *>( c_edge->my_var)->frac_bw;

// 	c_edge->f_bw = max( c_edge->f_bw , -ceil(  log( abs( m_err ) ) / log( 2.) ) );

//       }

//       cout << c_edge->edge_name << " : " << m_err << endl;



//     }
//     else{

      
//       double c_err;

//       for( vector<dag_index>::iterator e_iter = src_node->in_edges.begin() ; e_iter != src_node->in_edges.end(); e_iter++ )
// 	{

	  
// 	  if ( node_vec[ c_edge->get_dst_node() ]->node_type == OUTPUT ){
// 	    c_err = m_err;

// 	  }
// 	  else{
// 	    c_err = m_err / c_edge->my_var->ad_val.grads[ (*e_iter) ];
	   	  
// 	  }

// 	  propagate_error( *(e_iter) ,  c_err  );

// 	  //c_edge->f_bw = max( c_edge->f_bw , (int)ceil( log( abs( m_err ) ) / log( 2. )) );

	  

// 	}
  
//       c_edge->f_bw = max( c_edge->f_bw , -ceil(  log( abs( m_err ) ) / log( 2.) ) );
//       //c_edge->f_bw = max( c_edge->f_bw , m_err);
      

//       //cout << c_edge->edge_number << " : " <<  c_edge->edge_name << " : " << ilogb(m_err) << " : " << c_err << " : " <<  c_edge->f_bw <<endl;

//     }



// }
