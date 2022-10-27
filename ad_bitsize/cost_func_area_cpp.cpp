#include "bitsize_pass.h"


CostFunctionAreaCPP::CostFunctionAreaCPP( DAG *_dag ):BitSizePass(_dag),cost_nodes(0){}

void CostFunctionAreaCPP::cost_function_end(ostream &os ){


  os << " double cost_mul( int range_bw_A , int prec_bw_A , int range_bw_B , int prec_bw_B , int range_bw_OUT , int prec_bw_OUT ){ \n";
  os << " return ( range_bw_A + prec_bw_A ) * (  range_bw_B +  prec_bw_B );\n";
  os << " }\n\n";

  
  os << " double cost_addsub( int range_bw_A , int prec_bw_A , int range_bw_B , int prec_bw_B , int range_bw_OUT , int prec_bw_OUT ){ \n";
  os << " double tot_A , tot_B, tot_OUT; \n";
  os << " tot_A = range_bw_A + prec_bw_A; \n"; 
  os << " tot_B = range_bw_B + prec_bw_B; \n"; 
  os << " tot_OUT = range_bw_OUT + prec_bw_OUT;\n"; 
  os << " return max( tot_A , tot_B); \n";
  os << " }\n";

 
}

string CostFunctionAreaCPP::state_bw(  Edge *CEdge )
{

  if (  dynamic_cast<HWfix *>( CEdge->my_var )->frac_bw_state == SET )
    return " 1 ";

  else
    return " 0 ";

}


double CostFunctionAreaCPP::range_bw( Edge *CEdge )
{

 if ( dynamic_cast<HWfix *>( CEdge->my_var )->int_bw_state != SET )
     {
  
      double r_min = bs->range_analysis->get_range_min( CEdge ); 

      double r_max = bs->range_analysis->get_range_max( CEdge );

      r_max = static_cast<int>(r_max) % 2 == 0 ? r_max + 1 : r_max;

      double log2_val = ( log( max( abs( r_min  ) , abs( r_max  ) ) )/log( 2. ) ) + 1.;

      return ( log2_val <= 0. ? 1. : ceil( log2_val ));
    }
   else
     {

       return static_cast<double>(  dynamic_cast<HWfix *>( CEdge->my_var )->int_bw );


     }  


}




void CostFunctionAreaCPP::gen_parameters_func( ostream &os ){

 
  os <<  "int setup_parameters( long int *param_dim , USER_DEFINES * USER_OPTIONS )\n";

  os << "{\n";

  os << "*param_dim = " << edges_to_analyse << ";" << endl;

   
  os << "USER_OPTIONS->out_dim = new int;\n";

  os << "*(USER_OPTIONS->out_dim) = " <<  c_dag->output_edges.size() << ";\n";

 
  os << "USER_OPTIONS->out_err_spec = new double[*(USER_OPTIONS->out_dim)];\n";


  int ctr = 0;
  for( vector<dag_index>::iterator e_itr = c_dag->output_edges.begin(); e_itr != c_dag->output_edges.end() ; e_itr++ )
   {

     os << "USER_OPTIONS->out_err_spec[ " <<  ctr++  << " ] = ";
     os << " pow( 2. , -" <<  dynamic_cast< HWfix *>(c_dag->edge_vec[(*e_itr)]->my_var)->frac_bw  << " );\n";

     cout << "Output : " << dynamic_cast< HWfix *>(c_dag->edge_vec[(*e_itr)]->my_var)->var_name << " : ";
     cout <<  dynamic_cast< HWfix *>(c_dag->edge_vec[(*e_itr)]->my_var)->frac_bw << endl;
     

   }

  os << "return 0;\n\n}\n";
    

}

void CostFunctionAreaCPP::gen_costf( ostream& os ,  string err_func_name )
{
 
  init_name_maps();

  vector<Node *>::iterator n_itr;

  cost_function_end(os);


  for( n_itr = c_dag->node_vec.begin() ; n_itr != c_dag->node_vec.end() ; n_itr++ )
    {

      gen_ml_node_cf( (*n_itr) );

    }


  os << "\n\n\ndouble cost_function(double *x,\n";
  os << "\t\tdouble *parameter_lower_bound,\n";
  os << "\t\tdouble *parameter_upper_bound,\n";		
  os << "\t\tdouble *cost_tangents,\n";			
  os << "\t\tdouble *cost_curvature,\n";
  os << "\t\tlong int * parameter_dimension,\n";		
  os << "\t\tint *parameter_int_real,\n";					
  os << "\t\tint *cost_flag, int *exit_code, USER_DEFINES * USER_OPTIONS)\n";
  os << "{\n ";




  int i = 1;

  Edge *CEdge;

  int ctr = 1;

 
//   for( map<Edge*,string>::iterator s_itr = signal_bwnames.begin(); s_itr != signal_bwnames.end(); s_itr++  )
//     {

//       CEdge = (*s_itr).first;

//       if ( CEdge->edge_type != ECONSTANT ){

       

// 	// os << (*s_itr).second << " = x(" << i << ");" << endl;
      
// 	if ( CEdge->edge_type == IN )
// 	  {

// 	    os << "int " <<(*s_itr).second << " = ";

// 	    //cout << (*s_itr).second << endl;


// 	    if (  dynamic_cast< HWfix *>(CEdge->my_var)->frac_bw_state != SET )
// 	      {
// 		os <<  "static_cast<int>( x[" << ctr++ - 1 << "] );"  << endl;
// 		//os << " bw_out(" << 1 << ");" << endl; 
// 	      }
// 	    else{
// 	      //os << dynamic_cast< HWfix *>(CEdge->my_var)->frac_bw  << ";" << endl;
// 	      os << " bw_out(" << 1 << ");" << endl; 
// 	    }


// 	  }
// temp_var->var_name	else if ( CEdge->edge_type == OUT )
// 	  {

	 
// 	    //os << dynamic_cast< HWfix *>(CEdge->my_var)->frac_bw << ";" << endl;
// 	    // os << "bw_out(" << bw_out_ctr++ << ");" << endl;
	  
// 	  }
// 	else{
	 
// 	   os << "int " << (*s_itr).second << " = ";

// 	   cout << (*s_itr).second << endl;

// 	  if (  dynamic_cast< HWfix *>(CEdge->my_var)->frac_bw_state != SET )
// 	    {
// 	      os <<  "static_cast<int>( x[" << ctr++  - 1 << "] );"  << endl;

// 	    }
// 	  else{
// 	    os << dynamic_cast< HWfix *>(CEdge->my_var)->frac_bw  << ";" << endl;
// 	  }


// 	}

//       }
//     }

  HWfix *temp_var;

  for( vector<Edge *>::iterator e_itr = c_dag->edge_vec.begin(); e_itr != c_dag->edge_vec.end();e_itr++)
    {
	
      CEdge = (*e_itr);

      if ( temp_var =  dynamic_cast< HWfix *>(CEdge->my_var) ){

      if ( CEdge->edge_type != ECONSTANT ){

     
	if ( CEdge->edge_type == IN )
	  {

	    os << "int bw_" <<temp_var->var_name << " = ";


	    if (  temp_var->frac_bw_state != SET )
	      {
		os <<  "static_cast<int>( x[" << ctr++ - 1 << "] );"  << endl;
		
	      }
	    else{
	     
	    }


	  }
	else if ( CEdge->edge_type == OUT )
	  {

	 	  
	  }
	else{
	 
	   os << "int bw_" << temp_var->var_name << " = ";

	   

	  if (  temp_var->frac_bw_state != SET )
	    {
	      os <<  "static_cast<int>( x[" << ctr++  - 1 << "] );"  << endl;

	    }
	  else{
	    os << temp_var->frac_bw  << ";" << endl;
	  }


	}

      }
	
      }

    }


   int bw_out_ctr = 1;
  for( vector<dag_index>::iterator e_itr = c_dag->output_edges.begin() ; e_itr != c_dag->output_edges.end() ; e_itr++ )
    {

      os << "int " << signal_bwnames[ c_dag->edge_vec[(*e_itr)] ] <<  " = ";

      os  <<  dynamic_cast< HWfix *>(c_dag->edge_vec[(*e_itr)]->my_var)->frac_bw << ";\n";

      //os << signal_bwnames[ c_dag->edge_vec[(*e_itr)] ] <<  " = bw_out(" << bw_out_ctr++ << ");" << endl; 
   
      // os << signal_bwnames[ c_dag->edge_vec[(*e_itr)] ] <<  " = bw_out;" << endl; 
   
 }


  edges_to_analyse = ctr-1;

  os << "\n\n*cost_flag = error_function( x , parameter_dimension , parameter_int_real , USER_OPTIONS );\n\n";

 
  os << "double *Cost = new double[ " << cost_nodes << " ];\n";

  os << "if ( *cost_flag ) {\n\n";

  os <<  f_body.str() << endl;


  os << "double Total_Cost = 0;\n";

  os << "for (int index = 0; index <  " << cost_nodes << "; ++index)\n";

  os << "\tTotal_Cost += Cost[index];\n";  

  os << "return Total_Cost;";

  os << "\n}\n";

  os << "else";

  os << "\n{\n";
      
  os << "return 0;\n";

  os << "}\n";


  os << "\n}\n";
 
   


 
}

void CostFunctionAreaCPP::gen_ml_node_cf( Node *CNode )
{

   

  switch( CNode->node_type )
    {

   
    case ADD: gen_ml_add_node_exp( CNode ); break;


    case SUB: gen_ml_sub_node_exp( CNode ); break;


    case MUL: gen_ml_mul_node_exp( CNode ); break;

    case DIV: gen_ml_div_node_exp( CNode ); break;
   

    defualt:;



    }


}



void CostFunctionAreaCPP::gen_ml_add_node_exp(  Node * CNode )
{
 
  string l_edge, r_edge , out_edge;

  Edge *edge_l , *edge_r , *edge_out;

  edge_l = c_dag->edge_vec[ CNode->in_edges[0] ];
  edge_r =  c_dag->edge_vec[ CNode->in_edges[1] ];
  edge_out =  c_dag->edge_vec[ CNode->out_edges.back() ];

  out_edge = signal_bwnames[ edge_out  ];
  double out_range_bw =  range_bw( edge_out );

  if ( ( edge_l->edge_type == ECONSTANT ) && ( edge_r->edge_type != ECONSTANT ) )
    {
      l_edge = out_edge;
      r_edge = out_edge;

    }
  else{
    l_edge = ( edge_l->edge_type != ECONSTANT ) ? signal_bwnames[ edge_l ] : signal_bwnames[ edge_r ];
    r_edge = ( edge_r->edge_type != ECONSTANT ) ? signal_bwnames[ edge_r ] : signal_bwnames[ edge_l ];
  }

  double l_range_bw =  range_bw( edge_l );
  double r_range_bw =  range_bw( edge_r );
 
  f_body << "Cost[ " << cost_nodes++ << " ] = cost_addsub( " << l_range_bw  << " , " << l_edge  << " , ";
  f_body <<  r_range_bw << " , " << r_edge << " , " << out_range_bw  << " , " << out_edge  << " );" << endl;

  
}

void CostFunctionAreaCPP::gen_ml_sub_node_exp(  Node * CNode )
{

  string l_edge, r_edge , out_edge;

  l_edge = signal_bwnames[ c_dag->edge_vec[ CNode->in_edges[0] ] ];
  double l_range_bw =  range_bw(c_dag->edge_vec[ CNode->in_edges[0] ]);
  r_edge = signal_bwnames[ c_dag->edge_vec[ CNode->in_edges[1] ] ];
  double r_range_bw =  range_bw(c_dag->edge_vec[ CNode->in_edges[1] ]);
  out_edge = signal_bwnames[ c_dag->edge_vec[ CNode->out_edges.back() ] ];
  double out_range_bw =  range_bw(c_dag->edge_vec[ CNode->out_edges.back() ] );

  

  f_body  << "Cost[ " << cost_nodes++ << " ] = cost_addsub( " << l_range_bw  << " , " << l_edge  << " , ";
  f_body <<  r_range_bw << " , " << r_edge << " , " << out_range_bw  << " , " << out_edge  << " );" << endl;


 

}


void CostFunctionAreaCPP::gen_ml_mul_node_exp(  Node * CNode )
{

  string l_edge, r_edge , out_edge;

  l_edge = signal_bwnames[ c_dag->edge_vec[ CNode->in_edges[0] ] ];
  double l_range_bw =  range_bw(c_dag->edge_vec[ CNode->in_edges[0] ]);
  r_edge = signal_bwnames[ c_dag->edge_vec[ CNode->in_edges[1] ] ];
  double r_range_bw =  range_bw(c_dag->edge_vec[ CNode->in_edges[1] ]);
  out_edge = signal_bwnames[ c_dag->edge_vec[ CNode->out_edges.back() ] ];
  double out_range_bw =  range_bw(c_dag->edge_vec[ CNode->out_edges.back() ] );

  Edge *edge_l =  c_dag->edge_vec[ CNode->in_edges[0] ];
  Edge *edge_r =  c_dag->edge_vec[ CNode->in_edges[1] ];

  Node *src_l_node = c_dag->get_src_node( edge_l );

  Node *src_r_node = c_dag->get_src_node( edge_r );

  HWfix* var_l = dynamic_cast<HWfix*>(edge_l->my_var);

  HWfix* var_r = dynamic_cast<HWfix*>(edge_r->my_var);
  
  
  if ( src_l_node->node_type == CONSTANT ){

    f_body << "Cost[ " << cost_nodes++ << " ]   = cost_mul( ";

    //f_body << ceil( log(  bs->range_analysis->get_range_max( edge_l ) ) / log( 2 ) ) << " , ";

    f_body << var_l->int_bw << " , " << l_edge << ", "; 

    //f_body << var_r->int_bw << " + " << r_edge;

    f_body <<  var_r->int_bw << " , " << r_edge << ", ";

    f_body <<  out_range_bw  << " , " << out_edge;

    f_body  << " );" << endl;

  }else if  ( src_r_node->node_type == CONSTANT ){

    f_body << "Cost[ " << cost_nodes++ << " ]  = cost_mul( ";

    //f_body << ceil( log( bs->range_analysis->get_range_max( edge_r ) ) / log(2)  ) << " , ";

    f_body << var_r->int_bw << " , " << r_edge << ", ";

    //f_body << var_l->int_bw << " + " << l_edge;

    
    f_body << var_l->int_bw << " , " << l_edge << ", ";

    f_body << out_range_bw  << " , " << out_edge;

    f_body  << " );" << endl;

  }else{
    f_body << "Cost[ " << cost_nodes++ << " ]  = cost_mul( " << l_range_bw  << " , " << l_edge  << " , ";
    f_body <<  r_range_bw << " , " << r_edge << " , " << out_range_bw  << " , " << out_edge  << " );" << endl;
  }
  
 
}

void  CostFunctionAreaCPP::gen_ml_div_node_exp(  Node * CNode )
{

  string l_edge, r_edge , out_edge;

  l_edge = signal_bwnames[ c_dag->edge_vec[ CNode->in_edges[0] ] ];
  double l_range_bw =  range_bw(c_dag->edge_vec[ CNode->in_edges[0] ]);
  r_edge = signal_bwnames[ c_dag->edge_vec[ CNode->in_edges[1] ] ];
  double r_range_bw =  range_bw(c_dag->edge_vec[ CNode->in_edges[1] ]);
  out_edge = signal_bwnames[ c_dag->edge_vec[ CNode->out_edges.back() ] ];
  double out_range_bw =  range_bw(c_dag->edge_vec[ CNode->out_edges.back() ] );

  f_body << "Cost[ " << cost_nodes++ << " = cost_div( " << l_range_bw  << " , " << l_edge  << " , ";
  f_body <<  r_range_bw << " , " << r_edge << " , " << out_range_bw  << " , " << out_edge  << " );" << endl;

  
 

}
