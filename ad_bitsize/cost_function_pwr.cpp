#include "bitsize_pass.h"


CostFunctionPWR::CostFunctionPWR( DAG *_dag ):BitSizePass(_dag),coeff_vec_ctr(1){}

void CostFunctionPWR::cost_function_end(ostream &os ){


  os << "\n% Cost Functions " << endl; 

  os << "function C = cost_mul( range_bw_A , prec_bw_A , range_bw_B , prec_bw_B , range_bw_OUT , prec_bw_OUT  ) \n";

 //  os << "tot_A = range_bw_A + prec_bw_A; \n";

//   os << "tot_B = range_bw_B + prec_bw_B; \n";

//   os << "C = tot_A * tot_B;\n";

  os << "tot_ASC_OUT = max(range_bw_A, range_bw_B) + max(prec_bw_A, prec_bw_B);\n";
  os << "C = tot_ASC_OUT^2;\n";
  os << endl;  


  os << "function C = cost_div( range_bw_A , prec_bw_A , range_bw_B , prec_bw_B , range_bw_OUT , prec_bw_OUT  ) \n";
  os << "tot_ASC_OUT = max(range_bw_A, range_bw_B) + max(prec_bw_A, prec_bw_B);\n";
  os << "C = tot_ASC_OUT^2 * 2;\n";
  os << endl;  




  os << "function C = cost_addsub( range_bw_A , prec_bw_A , range_bw_B , prec_bw_B , range_bw_OUT , prec_bw_OUT  ) \n";
  os << "tot_A = range_bw_A + prec_bw_A; \n";
  os << "tot_B = range_bw_B + prec_bw_B; \n";
  os << "tot_OUT = range_bw_OUT + prec_bw_OUT; \n";

   //os << "C = min( tot_OUT,  max( tot_A , tot_B));\n";
  
   os << "C = max( tot_A , tot_B);\n";
}


string CostFunctionPWR::state_bw(  Edge *CEdge )
{

  if (  dynamic_cast<HWfix *>( CEdge->my_var )->frac_bw_state == SET )
    {
      
      return " 1 ";

    }
  {

    return " 0 ";

  }
}


double CostFunctionPWR::range_bw( Edge *CEdge )
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


void CostFunctionPWR::gen_parameters_func( ostream &os ){

  os << "function [no_of_bw, bw_out , bw_names, bw_state , bw_ranges ]";

  vector<dag_index>::iterator e_itr;

 
  os << "= parameters" << endl;

  os << "no_of_bw = " << edges_to_analyse << ";" << endl;

  //os << "bw_out = 12" << ";" << endl;
  
  
  os << "bw_out " << " =[ ";

 

  for( e_itr = c_dag->output_edges.begin() ; e_itr != c_dag->output_edges.end() ; e_itr++ )
    {

      os  <<  dynamic_cast< HWfix *>(c_dag->edge_vec[(*e_itr)]->my_var)->frac_bw << " ";
     
    }

  os << "];" << endl;

  stringstream ss_names, ss_ranges ,ss_state;

  int ctr = 1;

    ss_names << "bw_names = { ";
    ss_ranges << "bw_ranges = [ ";
    
    ss_state << "bw_state = [";

     // Print IN and TMP var info

    init_name_maps();

  for( map<Edge*,string>::iterator s_itr = signal_names.begin(); s_itr != signal_names.end(); s_itr++  )
    {
      if ( (*s_itr).first->edge_type != OUT ){

      ss_names << " '" << (*s_itr).second <<  "' "; 
       
       ss_ranges << range_bw( (*s_itr).first ) << " ";

       Edge *CEdge = (*s_itr).first;

       if (  dynamic_cast<HWfix *>( CEdge->my_var )->frac_bw_state != SET ){
	 ss_state << state_bw( (*s_itr).first );
       }
       else{

	 ss_state << " " << dynamic_cast<HWfix *>( CEdge->my_var )->frac_bw << " "; 
       }
      }
    }


    // Print OUT var info

   for( vector<dag_index>::iterator e_itr = c_dag->output_edges.begin() ; e_itr != c_dag->output_edges.end() ; e_itr++ )
    {

      //os  <<  dynamic_cast< HWfix *>(c_dag->edge_vec[(*e_itr)]->my_var)->frac_bw << " ";
      // os << signal_bwnames[ c_dag->edge_vec[(*e_itr)] ] <<  " = bw_out(" << bw_out_ctr++ << ");" << endl; 

      ss_names << " '" << signal_names[ c_dag->edge_vec[(*e_itr)] ] <<  "' ";
      
      ss_ranges << range_bw(  c_dag->edge_vec[(*e_itr)]  ) << " ";

      ss_state << state_bw(  c_dag->edge_vec[(*e_itr)] );

    }

  ss_names << "};" << endl;

  ss_ranges << "];" << endl;

  ss_state << "];" << endl;

  os << ss_names.str();

  os << ss_ranges.str();

  
  os << ss_state.str();


 
    

}

void CostFunctionPWR::gen_matlab_costf( ostream& os ,  string err_func_name )
{
 
  init_name_maps();

  vector<Node *>::iterator n_itr;


  for( n_itr = c_dag->node_vec.begin() ; n_itr != c_dag->node_vec.end() ; n_itr++ )
    {

      gen_ml_node_cf( (*n_itr) );

    }


  os << "function [cost_value, cost_flag]  = cost_function_pwr( x )" << endl;
  os << "global bw_out;" << endl;

  os << "[no_of_bw, bw_out_ , bw_names , bw_ranges ] = parameters();" << endl;

  os << "% [ C0_vec , C1_vec ] = coeff_gen();" << endl;

  os << "global C0_vec;" << endl;
  os << "global C1_vec;" << endl;

  os << "req_error = 2.^-bw_out;" << endl;

  int i = 1;

  Edge *CEdge;

  int ctr = 1;

 
  for( map<Edge*,string>::iterator s_itr = signal_bwnames.begin(); s_itr != signal_bwnames.end(); s_itr++  )
    {

      CEdge = (*s_itr).first;

      if ( CEdge->edge_type != ECONSTANT ){

       

	// os << (*s_itr).second << " = x(" << i << ");" << endl;
      
	if ( CEdge->edge_type == IN )
	  {

	     os << (*s_itr).second << " = ";
	    if (  dynamic_cast< HWfix *>(CEdge->my_var)->frac_bw_state != SET )
	      {
		os <<  "x(" << ctr++ << ");"  << endl;
		//os << " bw_out(" << 1 << ");" << endl; 
	      }
	    else{
	      //os << dynamic_cast< HWfix *>(CEdge->my_var)->frac_bw  << ";" << endl;
	      os << " bw_out(" << 1 << ");" << endl; 
	    }


	  }
	else if ( CEdge->edge_type == OUT )
	  {

	 
	    //os << dynamic_cast< HWfix *>(CEdge->my_var)->frac_bw << ";" << endl;
	    // os << "bw_out(" << bw_out_ctr++ << ");" << endl;
	  
	  }
	else{
	 
	   os << (*s_itr).second << " = ";

	  if (  dynamic_cast< HWfix *>(CEdge->my_var)->frac_bw_state != SET )
	    {
	      os <<  "x(" << ctr++ << ");"  << endl;

	    }
	  else{
	    os << dynamic_cast< HWfix *>(CEdge->my_var)->frac_bw  << ";" << endl;
	  }


	}

      }
    }

   int bw_out_ctr = 1;
  for( vector<dag_index>::iterator e_itr = c_dag->output_edges.begin() ; e_itr != c_dag->output_edges.end() ; e_itr++ )
    {

      //os  <<  dynamic_cast< HWfix *>(c_dag->edge_vec[(*e_itr)]->my_var)->frac_bw << " ";
      os << signal_bwnames[ c_dag->edge_vec[(*e_itr)] ] <<  " = bw_out(" << bw_out_ctr++ << ");" << endl; 
    }


  edges_to_analyse = ctr-1;

  os << "my_error = " << err_func_name << "(x , bw_out );" << endl;
  
  // os << "if (my_error > req_error)" << endl;
  
  os << "if ( ";

  //  for( int i = 1 ; i  <= output_names.size() ; i++ )
  //     {
  //       os << "( my_error( " << i << " ) > req_error( " << i << " ))";  

  //       if ( i != output_names.size() ) os << " & ";
  //     }

  if ( c_dag->output_edges.size() == 1 ){
    os << "( my_error  > req_error)"; 
  }
  else
    {
      for( i = 1 ; i  <= c_dag->output_edges.size() ; i++ )
	{
	  os << "( my_error( " << i << " ) > req_error( " << i << " ))";  

	  if ( i != c_dag->output_edges.size() ) os << " | ";
	}

    }

  os << " ) " << endl;

  os << "cost_flag = 0;" << endl;
  os << "cost_value = 0;" << endl;
  os << "else" << endl;

  // Produce cost function body
  os << f_body.str();

  os << "cost_value = sum( [ ";
  
  os << used_node_names.str();

  os << " ] );" << endl;

  os << "cost_flag = 1;" << endl; 


  os << "end;" << endl;

  //cost_function_end(os);

  fstream coeff_file("coeff_gen.m" , ios::out);
  coeff_file << "function [ C0_vec , C1_vec ] = coeff_gen" << endl;
  coeff_file << "C0_vec = [];" << endl;
  coeff_file << "C1_vec = [];" << endl;
  coeff_file << coeff_fs.str() << endl;
  coeff_file.close();
 
}

void CostFunctionPWR::gen_ml_node_cf( Node *CNode )
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



void CostFunctionPWR::gen_ml_add_node_exp(  Node * CNode )
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
 
  f_body << node_costnames[ CNode ] << " = cost_addsub2( ";
  f_body << "C0_vec( " << coeff_vec_ctr << " ) , C1_vec( " << coeff_vec_ctr++ << " ) ,"; 

  f_body << l_range_bw  << " , " << l_edge  << " , ";
  //f_body << edge_l->data_log.var << " , " << edge_l->data_log.acoef_1   << " , ";
  
  f_body <<  r_range_bw << " , " << r_edge << " , ";
  //f_body << edge_r->data_log.var << " , " << edge_r->data_log.acoef_1   << " , ";
  f_body << out_range_bw  << " , " << out_edge  << " );" << endl;

  
  coeff_fs << "% " << node_costnames[ CNode ] << endl;

  coeff_fs << "c1_coeff  = get_adder_coeff_c1( "<< "sqrt( " <<  edge_l->data_log.var << " ) " << " , " << edge_l->data_log.acoef_1 << " , "; 
 coeff_fs << "sqrt( " <<  edge_r->data_log.var << " )" << " , " << edge_r->data_log.acoef_1 << " );";
  coeff_fs << "C1_vec = [ C1_vec c1_coeff ];" << endl;

  coeff_fs << "c0_coeff  = get_adder_coeff_c0( "<< "sqrt( " <<  edge_l->data_log.var << " ) " << " , " << edge_l->data_log.acoef_1 << " , "; 
  coeff_fs << "sqrt( " <<  edge_r->data_log.var << " )" << " , " << edge_r->data_log.acoef_1 << " );";
  coeff_fs << "C0_vec = [ C0_vec c0_coeff ];" << endl;
 
  used_node_names << node_costnames[ CNode ] << " "; 
}

void CostFunctionPWR::gen_ml_sub_node_exp(  Node * CNode )
{

  
  Edge *edge_l , *edge_r , *edge_out;

  edge_l = c_dag->edge_vec[ CNode->in_edges[0] ];
  edge_r =  c_dag->edge_vec[ CNode->in_edges[1] ];
  edge_out =  c_dag->edge_vec[ CNode->out_edges.back() ];


  string l_edge, r_edge , out_edge;

  l_edge = signal_bwnames[ edge_l ];
  double l_range_bw =  range_bw(edge_l);
  r_edge = signal_bwnames[ edge_r ];
  double r_range_bw =  range_bw(edge_r);
  out_edge = signal_bwnames[ edge_out ];
  double out_range_bw =  range_bw(edge_out );

  f_body << node_costnames[ CNode ] << " = cost_addsub2( "; 
  f_body << "C0_vec( " << coeff_vec_ctr << " ) , C1_vec( " << coeff_vec_ctr++ << " ) ,";
  f_body << l_range_bw  << " , " << l_edge  << " , ";

    //  f_body << edge_l->data_log.var  << " , " << edge_l->data_log.acoef_1   << " , ";
  f_body <<  r_range_bw << " , " << r_edge << " , ";
  //f_body << edge_r->data_log.var << " , " << edge_r->data_log.acoef_1  << " , ";
  f_body << out_range_bw  << " , " << out_edge  << " );" << endl;

  
  coeff_fs << "% " << node_costnames[ CNode ] << endl;
  coeff_fs << "c1_coeff  = get_adder_coeff_c1( "<< "sqrt( " <<  edge_l->data_log.var << " )" << " , " << edge_l->data_log.acoef_1 << " , "; 
  coeff_fs << "sqrt( " <<  edge_r->data_log.var << " ) " << " , " << edge_r->data_log.acoef_1 << " );";
  coeff_fs << "C1_vec = [ C1_vec c1_coeff ];" << endl;


  coeff_fs << "c0_coeff  = get_adder_coeff_c0( "<< "sqrt( " <<  edge_l->data_log.var << " )" << " , " << edge_l->data_log.acoef_1 << " , "; 
  coeff_fs <<  "sqrt( " << edge_r->data_log.var << " ) " << " , " << edge_r->data_log.acoef_1 << " );";
  coeff_fs << "C0_vec = [ C0_vec c0_coeff ];" << endl;


  used_node_names << node_costnames[ CNode ] << " "; 

}


void CostFunctionPWR::gen_ml_mul_node_exp(  Node * CNode )
{

  Edge *edge_l , *edge_r , *edge_out;

  edge_l = c_dag->edge_vec[ CNode->in_edges[0] ];
  edge_r =  c_dag->edge_vec[ CNode->in_edges[1] ];
  edge_out =  c_dag->edge_vec[ CNode->out_edges.back() ];


  string l_edge, r_edge , out_edge;

  l_edge = signal_bwnames[ edge_l ];
  double l_range_bw =  range_bw(edge_l);
  r_edge = signal_bwnames[ edge_r ];
  double r_range_bw =  range_bw(edge_r);
  out_edge = signal_bwnames[ edge_out ];
  double out_range_bw =  range_bw(edge_out );

  
  f_body << node_costnames[ CNode ] << " = cost_mul2( "; 
  //f_body << node_costnames[ CNode ] << "_C.C0 , " <<  node_costnames[ CNode ] << "_C.C1 ,"

  f_body << "C0_vec( " << coeff_vec_ctr << " ) , C1_vec( " << coeff_vec_ctr++ << " ) , "; 
  
  f_body <<  l_range_bw  << " , " << l_edge  << " , ";

    //  f_body << edge_l->data_log.var << " , " << edge_l->data_log.acoef_1   << " , ";
  f_body <<  r_range_bw << " , " << r_edge << " , ";

  //  f_body << edge_r->data_log.var << " , " << edge_r->data_log.acoef_1   << " , ";
  f_body << out_range_bw  << " , " << out_edge  << " );" << endl;
  

  coeff_fs << "% " << node_costnames[ CNode ] << endl;
  coeff_fs << "c1_coeff  = get_mult_coeff_c1( " << edge_l->data_log.acoef_1 << " , "; 
  coeff_fs << edge_r->data_log.acoef_1 << " );";

  coeff_fs << "C1_vec = [ C1_vec c1_coeff ];" << endl;

  coeff_fs << "c0_coeff = get_mult_coeff_c0( " << edge_l->data_log.acoef_1 << " , "; 
  coeff_fs << edge_r->data_log.acoef_1 << " );";

  coeff_fs << "C0_vec = [ C0_vec c0_coeff ];" << endl;
 
  
  used_node_names << node_costnames[ CNode ] << " "; 
}

void  CostFunctionPWR::gen_ml_div_node_exp(  Node * CNode )
{
  Edge *edge_l , *edge_r , *edge_out;

  edge_l = c_dag->edge_vec[ CNode->in_edges[0] ];
  edge_r =  c_dag->edge_vec[ CNode->in_edges[1] ];
  edge_out =  c_dag->edge_vec[ CNode->out_edges.back() ];


  string l_edge, r_edge , out_edge;

  l_edge = signal_bwnames[ edge_l ];
  double l_range_bw =  range_bw(edge_l);
  r_edge = signal_bwnames[ edge_r ];
  double r_range_bw =  range_bw(edge_r);
  out_edge = signal_bwnames[ edge_out ];
  double out_range_bw =  range_bw(edge_out );



  f_body << node_costnames[ CNode ] << " = cost_addsub( " << l_range_bw  << " , " << l_edge  << " , ";
  f_body << edge_l->data_log.var << " , " << edge_l->data_log.acoef_1   << " , ";
  f_body <<  r_range_bw << " , " << r_edge << " , ";
  f_body << edge_r->data_log.var << " , " << edge_r->data_log.acoef_1   << " , ";
  f_body << out_range_bw  << " , " << out_edge  << " );" << endl;

  
  used_node_names << node_costnames[ CNode ] << " "; 

}
