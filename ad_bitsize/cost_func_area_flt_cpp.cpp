#include "bitsize_pass.h"


CostFuncAreaFLTCPP::CostFuncAreaFLTCPP( DAG *_dag ):BitSizePass(_dag),cost_nodes(0){}

void CostFuncAreaFLTCPP::cost_function_end(ostream &os ){


  os << " double cost_mul( int exp_bw_A , int man_bw_A , int exp_bw_B , int man_bw_B , int exp_bw_OUT , int man_bw_OUT ){ \n";
  os << " double C[] = {1.9772 , 4.7262 , 99.0829 };\n";
  os << " double X[2];\n";
  os << " X[0] = man_bw_A * man_bw_B;\n";
  os << " X[1]= max( exp_bw_A , exp_bw_B );\n";
  os << " return ( C[0]*X[0] + C[1]*X[1] + C[2]);\n";
  os << " }\n\n";

  
  os << " double cost_addsub( int exp_bw_A , int man_bw_A , int exp_bw_B , int man_bw_B , int exp_bw_OUT , int man_bw_OUT ){ \n";
  os << " double C[] = {0.2895, 18.2743 , 147.8651 };\n";
  os << " double X[1];\n";
  os << " double man_bw , exp_bw;\n";
  os << " man_bw = max( man_bw_A , man_bw_B );\n";
  os << " exp_bw = max( exp_bw_A , exp_bw_B );\n";
  os << " X[0] = man_bw * exp_bw; \n"; 
  os << " return ( C[0]*X[0] + C[1]*man_bw + C[2] );\n";
  os << " }\n";

 
}

string CostFuncAreaFLTCPP::state_bw(  Edge *CEdge )
{

  if (  dynamic_cast<HWfix *>( CEdge->my_var )->frac_bw_state == SET )
    return " 1 ";

  else
    return " 0 ";

}


double CostFuncAreaFLTCPP::range_bw( Edge *CEdge )
{

 if ( dynamic_cast<HWfloat *>( CEdge->my_var )->man_bw_state != SET )
     {
  
       

     //  double r_min = bs->range_analysis->get_range_min( CEdge ); 

//       double r_max = bs->range_analysis->get_range_max( CEdge );

      double max_abs_val = max( fabs( CEdge->max_val ) , fabs( CEdge->min_val ) );

      //double drange = ilogb( r_max )  - ilogb( r_min );

      //double drange = ilogb( fabs( r_max - r_min ) );

      double max_eval = ( max_abs_val == 0 ? 0 : ilogb( max_abs_val ) );



      //double log2_val = log( max( fabs( r_min  ) , fabs( r_max  ) ) )/log( 2. );

      //double e_bw = log(( log2_val <= 0. ? ceil( fabs(log2_val) + 1.) : ceil( log2_val )) + 1)/log(2.) + 1; 

      double e_bw = max_eval == 0. ? 0. : log( fabs(max_eval) ) / log( 2. ) + 1; 

      //cout << max_abs_val << " : " << max_eval <<  " : " << e_bw << endl;

      return ceil( e_bw );

     //  drange = ( drange == 0. ) ? 1. : drange; 

//       if ( drange < 0 )
// 	cout << drange << " : " << r_min << " : " << r_max << endl;

     //  return ceil( log( drange ) / log( 2. ) ) + 1.;

     
    }
   else
     {

       return static_cast<double>(  dynamic_cast<HWfloat *>( CEdge->my_var )->exp_bw );


     }  


}




void CostFuncAreaFLTCPP::gen_parameters_func( ostream &os ){

 
  os <<  "int setup_parameters( long int *param_dim , USER_DEFINES * USER_OPTIONS )\n";

  os << "{\n";

  os << "*param_dim = " << edges_to_analyse << ";" << endl;

   
  os << "USER_OPTIONS->out_dim = new int;\n";

  os << "*(USER_OPTIONS->out_dim) = " <<  c_dag->output_edges.size() << ";\n";

 
  os << "USER_OPTIONS->out_err_spec = new double[*(USER_OPTIONS->out_dim)];\n";


  int ctr = 0;

  Edge *CEdge;
  for( vector<dag_index>::iterator e_itr = c_dag->output_edges.begin(); e_itr != c_dag->output_edges.end() ; e_itr++ )
   {

     CEdge = c_dag->edge_vec[(*e_itr)];

     os << "USER_OPTIONS->out_err_spec[ " <<  ctr++  << " ] = ";

     //double edge_exp = ilogb( CEdge->max_val );

     os << " pow( 2. , -" <<  dynamic_cast<HWfloat *>(CEdge->my_var)->man_bw  << " );\n";

     //     cout << "Output : " << dynamic_cast<HWfloat *>( CEdge->my_var)->var_name << " : ";
     // cout <<  dynamic_cast<HWfloat *>( CEdge->my_var)->man_bw << endl;
     

   }

  os << "return 0;\n\n}\n";
    

}

void CostFuncAreaFLTCPP::gen_costf( ostream& os ,  string err_func_name )
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

 

  HWfloat *temp_var;

  for( vector<Edge *>::iterator e_itr = c_dag->edge_vec.begin(); e_itr != c_dag->edge_vec.end();e_itr++)
    {
	
      CEdge = (*e_itr);

      if ( temp_var =  dynamic_cast<HWfloat *>(CEdge->my_var) ){

      if ( CEdge->edge_type != ECONSTANT ){

     
	if ( CEdge->edge_type == IN )
	  {

	    //os << "int bw_" <<temp_var->var_name << " = ";


	    if (  temp_var->man_bw_state != SET )
	      {
		//os <<  "static_cast<int>( x[" << ctr++ - 1 << "] );"  << endl;
		
		//os << " 0; " << endl;
	      }
	    else{
	     
	    }


	  }
	else if ( CEdge->edge_type == OUT )
	  {

	 	  
	  }
	else{
	 
	   os << "int bw_" << temp_var->var_name << " = ";

	   

	  if (  temp_var->man_bw_state != SET )
	    {
	      os <<  "static_cast<int>( x[" << ctr++  - 1 << "] );"  << endl;

	    }
	  else{
	    os << temp_var->man_bw  << ";" << endl;
	  }


	}

      }
	
      }

    }


   int bw_out_ctr = 1;
  for( vector<dag_index>::iterator e_itr = c_dag->output_edges.begin() ; e_itr != c_dag->output_edges.end() ; e_itr++ )
    {

      os << "int " << signal_bwnames[ c_dag->edge_vec[(*e_itr)] ] <<  " = ";

      os  <<  dynamic_cast<HWfloat *>(c_dag->edge_vec[(*e_itr)]->my_var)->man_bw << ";\n";

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

void CostFuncAreaFLTCPP::gen_ml_node_cf( Node *CNode )
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



void CostFuncAreaFLTCPP::gen_ml_add_node_exp(  Node * CNode )
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

void CostFuncAreaFLTCPP::gen_ml_sub_node_exp(  Node * CNode )
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


void CostFuncAreaFLTCPP::gen_ml_mul_node_exp(  Node * CNode )
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

  HWfloat *var_l, *var_r;

 //  HWfloat* var_l = dynamic_cast<HWfix*>(edge_l->my_var);

//   HWfloat* var_r = dynamic_cast<HWfix*>(edge_r->my_var);

  if (( var_l = dynamic_cast<HWfloat*>(edge_l->my_var) ) && ( var_r = dynamic_cast<HWfloat*>(edge_r->my_var) )){
  
  if ( src_l_node->node_type == CONSTANT ){

    f_body << "Cost[ " << cost_nodes++ << " ]   = cost_mul( ";

    //f_body << ceil( log(  bs->range_analysis->get_range_max( edge_l ) ) / log( 2 ) ) << " , ";

    f_body << var_l->exp_bw << " , " << l_edge << ", "; 

    //f_body << var_r->int_bw << " + " << r_edge;

    f_body <<  var_r->exp_bw << " , " << r_edge << ", ";

    f_body <<  out_range_bw  << " , " << out_edge;

    f_body  << " );" << endl;

  }else if  ( src_r_node->node_type == CONSTANT ){

    f_body << "Cost[ " << cost_nodes++ << " ]  = cost_mul( ";

    //f_body << ceil( log( bs->range_analysis->get_range_max( edge_r ) ) / log(2)  ) << " , ";

    f_body << var_r->exp_bw << " , " << r_edge << ", ";

    //f_body << var_l->int_bw << " + " << l_edge;

    
    f_body << var_l->exp_bw << " , " << l_edge << ", ";

    f_body << out_range_bw  << " , " << out_edge;

    f_body  << " );" << endl;

  }else{
    f_body << "Cost[ " << cost_nodes++ << " ]  = cost_mul( " << l_range_bw  << " , " << l_edge  << " , ";
    f_body <<  r_range_bw << " , " << r_edge << " , " << out_range_bw  << " , " << out_edge  << " );" << endl;
  }
  
  }
 
}

void  CostFuncAreaFLTCPP::gen_ml_div_node_exp(  Node * CNode )
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
