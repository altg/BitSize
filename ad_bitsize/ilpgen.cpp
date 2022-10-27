// Pass to Generate ILP formulation for CPLEX
#include "bitsize_pass.h"


ILPGenPass::ILPGenPass( DAG *_dag , int _lbound , int _ubound ):BitSizePass(_dag),l_bound(_lbound),u_bound(_ubound),bound_num(_ubound - _lbound + 1),node_num(1){}


void ILPGenPass::gen_ilp_node_cf( Node *CNode )
{

   

  switch( CNode->node_type )
    {

   
    case ADD: gen_ilp_add_node_exp( CNode ); break;


    case SUB: gen_ilp_sub_node_exp( CNode ); break;


    case MUL: gen_ilp_mul_node_exp( CNode ); break;

      // case DIV: gen_ilp_div_node_exp( CNode ); break;
   

    defualt:;



    }


}


void ILPGenPass::gen_ilp_costfunc( ostream &os ){


  init_name_maps();

  vector<Node *>::iterator n_itr;


  for( n_itr = c_dag->node_vec.begin() ; n_itr != c_dag->node_vec.end() ; n_itr++ )
    {

      gen_ilp_node_cf( (*n_itr) );

    }

  stringstream bin_list, bin_const, bw_val , ulp_val;

  stringstream err_const;
  list_binvars( bin_list , bin_const , bw_val , ulp_val );

  gen_error_const( err_const );

  os << "minimize" << endl;
  //os << used_node_names.str() << endl;

  int str_ctr = 1;
  for ( list<string>::iterator str_itr = cf_nodes.begin() ; str_itr != cf_nodes.end() ; str_itr++ , str_ctr++ )
    {

      os << (*str_itr);

      if ( str_ctr < cf_nodes.size() )
	os << " + ";

    }
  os << endl;

  os << "subject to" << endl;

  os << bw_val.str() << endl;

  os << f_body.str() << endl;

  os << ulp_val.str() << endl;

  os << err_const.str() << endl;

  os << bin_const.str() << endl;

  os << "binary" << endl;

  os << bin_list.str() << endl;

  os << "end" << endl;
 
}

double ILPGenPass::range_bw( Edge *CEdge )
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

void ILPGenPass::gen_ilp_add_node_exp(  Node * CNode )
{
 
  string l_edge, r_edge , out_edge;

  Edge *edge_l , *edge_r , *edge_out;

  edge_l = c_dag->edge_vec[ CNode->in_edges[0] ];
  edge_r =  c_dag->edge_vec[ CNode->in_edges[1] ];
  edge_out =  c_dag->edge_vec[ CNode->out_edges.back() ];

  out_edge = signal_names[ edge_out  ];
  double out_range_bw =  range_bw( edge_out );

  if ( ( edge_l->edge_type == ECONSTANT ) && ( edge_r->edge_type != ECONSTANT ) )
    {
      l_edge = out_edge;
      r_edge = out_edge;

    }
  else{
    l_edge = ( edge_l->edge_type != ECONSTANT ) ? signal_names[ edge_l ] : signal_names[ edge_r ];
    r_edge = ( edge_r->edge_type != ECONSTANT ) ? signal_names[ edge_r ] : signal_names[ edge_l ];
  }

  double l_range_bw =  range_bw( edge_l );
  double r_range_bw =  range_bw( edge_r );
 
  f_body << "N" << node_num << "l:" << node_costnames[ CNode ] << " - " << l_edge << " >= " << l_range_bw << endl;
  f_body <<  "N" << node_num++ << "r:" << node_costnames[ CNode ] << " - " << r_edge << " >= " << r_range_bw << endl;

  f_body << endl;

  //used_node_names << node_costnames[ CNode ] << " + "; 


  cf_nodes.push_back( node_costnames[ CNode ] );
}

void ILPGenPass::gen_ilp_sub_node_exp(  Node * CNode )
{

  
  Edge *edge_l , *edge_r , *edge_out;

  edge_l = c_dag->edge_vec[ CNode->in_edges[0] ];
  edge_r =  c_dag->edge_vec[ CNode->in_edges[1] ];
  edge_out =  c_dag->edge_vec[ CNode->out_edges.back() ];


  string l_edge, r_edge , out_edge;

  l_edge = signal_names[ edge_l ];
  double l_range_bw =  range_bw(edge_l);
  r_edge = signal_names[ edge_r ];
  double r_range_bw =  range_bw(edge_r);
  

  f_body <<  "N" << node_num << "l:" << node_costnames[ CNode ] << " - " << l_edge << " >= " << l_range_bw << endl;
  f_body <<  "N" << node_num++ << "r:" << node_costnames[ CNode ] << " - " << r_edge << " >= " << r_range_bw << endl;

  f_body << endl;

  //used_node_names << node_costnames[ CNode ] << " + "; 

  cf_nodes.push_back( node_costnames[ CNode ] );
}


void ILPGenPass::gen_ilp_mul_node_exp(  Node * CNode )
{

  Edge *edge_l , *edge_r , *edge_out;

  edge_l = c_dag->edge_vec[ CNode->in_edges[0] ];
  edge_r =  c_dag->edge_vec[ CNode->in_edges[1] ];
  edge_out =  c_dag->edge_vec[ CNode->out_edges.back() ];


  string l_edge, r_edge , out_edge;

  l_edge = signal_names[ edge_l ];
  double l_range_bw =  range_bw(edge_l);
  r_edge = signal_names[ edge_r ];
  double r_range_bw =  range_bw(edge_r);
  out_edge = signal_names[ edge_out ];
  double out_range_bw =  range_bw(edge_out );

  
  double bound_num = u_bound - l_bound + 1;

  double U_max = bound_num * ( u_bound + l_bound ) / 2;

  f_body << "N" << node_num << ":"<< node_costnames[ CNode ] << " - Q_" << node_costnames[ CNode ] << " - ";
  f_body << l_range_bw << " " << r_edge << " - ";
  f_body << r_range_bw << " " << l_edge;
  f_body << " = " << l_range_bw * r_range_bw << endl;


  for ( int i = 1 ; i <= bound_num ; i++ )
    {
      
      f_body << "NQ" << node_num << "_" << i << ": Q_" << node_costnames[ CNode ];
      f_body << " - " << i << " " << l_edge << " - ";
      f_body << i * U_max << " "; 
      gen_binvar( f_body , r_edge , i );
      f_body << " >= " << -i * U_max << endl;


    }

  f_body << endl;
  node_num++;
  //used_node_names << node_costnames[ CNode ] << " + ";   

  cf_nodes.push_back( node_costnames[ CNode ] );
}



void ILPGenPass::gen_binvar( stringstream &ss , string edge_name , int binvar_num )
{


  ss << "B" << edge_name << "_" << binvar_num;


}


void ILPGenPass::gen_error_const( stringstream &ss )
{

  vector<dag_index>::iterator e_itr;
  
  Edge *CEdge;

  int e_const = 1;
  ss << "E" << e_const << ":";

  

  for( e_itr = c_dag->output_edges.begin() ; e_itr != c_dag->output_edges.end() ; e_itr++ )
    {  
      
         
      CEdge = c_dag->edge_vec[ (*e_itr) ];
 
      for( int i = 0; i < CEdge->ErrorNames_vec.size() ; i++ )
	{

	  ss << CEdge->TransFuncVals_vec[ i ] << " ";
	  ss << CEdge->ErrorNames_vec[ i ];

	  if ( i != CEdge->ErrorNames_vec.size() - 1 ){

	    ss << " + ";
	  }


	}

      ss << " <= " << setprecision(10) << pow( 2. , -(int)( dynamic_cast<HWfix *>(CEdge->my_var)->frac_bw )  ) << endl;
     
           
    }


}



void ILPGenPass::gen_varnames_list( ostream& os )
{

   for( map<Edge*,string>::iterator s_itr = signal_names.begin(); s_itr != signal_names.end(); s_itr++ )
    {

      os << (*s_itr).second << endl;


    }

  


}


void ILPGenPass::list_binvars( stringstream &bin_list , stringstream &bin_const , stringstream &bw_val , stringstream &ulp_val)
{
  int BinC1, BinC2 , BinC3;

  stringstream ulp_val2;

  BinC1 = BinC2 = BinC3 = 1;

   for( map<Edge*,string>::iterator s_itr = signal_names.begin(); s_itr != signal_names.end(); s_itr++ )
    {
      if ( (*s_itr).first->edge_type != OUT ){

	bw_val << "BC1_" << BinC1++ << ":" << (*s_itr).second << " - ";

	bin_const << "BC2_" << BinC2++ << ":";

	ulp_val << "BC3_" << BinC3 << ":";

	int k = 1;
	for ( int i = 1; i <= bound_num ; i++ )
	  {

	    bin_list << "B" << (*s_itr).second << "_" << i;
	    bin_const << "B" << (*s_itr).second << "_" << i;
	    
	    bw_val <<  i << " B" << (*s_itr).second << "_" << i;

	    //ulp_val << setprecision(5) << "  " << pow( 2. , -i ) << " B" << (*s_itr).second << "_" << i;

	    if ( ( i - 1 ) % 10 == 0 ){

	      ulp_val << "S" << k << "E_" << signal_names[ (*s_itr).first ];

	      if ( ( i + 9 ) != bound_num ){
		ulp_val << " + ";
		}
	      
	      ulp_val2 << "BC3_" << BinC3 << "_" << k <<":";
	      ulp_val2 << "S" << k++ << "E_" << signal_names[ (*s_itr).first ];

	    }

	    if ( i != bound_num ){
	      bin_list << " ";
	      bin_const << " + ";

	      bw_val << " - ";

	      //ulp_val << " + ";
	    }

	    ulp_val2 << setprecision(5) << " -" << pow( 2. , -i ) << " B" << (*s_itr).second << "_" << i;

	    if ( ( i % 10 ) == 0 )
	      {

		ulp_val2 << " = 0" << endl;
	      }

	    if ( ( i % 20 ) == 0 )
	      {
		bin_list << endl;
	      }

	  }

	BinC3++;

	bin_list << endl;

	bin_const << " = 1" << endl;

	bw_val << " = 0" << endl;

	ulp_val << " - E_" << signal_names[ (*s_itr).first ] << " = 0" << endl;

	ulp_val2 << endl;

      }

      

    }
   ulp_val << ulp_val2.str();

}
