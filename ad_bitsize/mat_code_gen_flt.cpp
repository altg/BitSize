// Simple backend to print ASC code

#include "bitsize_pass.h"


MatCodeGenFlt::MatCodeGenFlt(DAG *_dag , NodeSchedular *_node_sched ):BitSizePass(_dag),node_sched(_node_sched),rmode("truncate"),signal_ctr(1),input_ctr(1),input_bw(0){}


void MatCodeGenFlt::operator()( ostream &os )
{

  do_matcodegen( os );

}


void MatCodeGenFlt::do_matcodegen( ostream &os )
{
  
   _DBOUT( "Doing Matlab code generation" );

  vector< NodeSchedular::node_vec_type >::iterator sch_itr; 
  vector< dag_index >::iterator n_itr2;

  int ctr = 0;
  for ( sch_itr = node_sched->schedule.begin() ; sch_itr != node_sched->schedule.end() ;  sch_itr++ )
    {

     
      
      for( n_itr2 = (*sch_itr).begin() ; n_itr2 != (*sch_itr).end() ; n_itr2++ )
	{

	    do_matcodegen_node((*n_itr2));

	}  




    }

  //gen_var_decs();

  input_ctr--;

  gen_header( os );

  os << dec_body.str();

  os << f_body.str();

  gen_footer( os );

}


void MatCodeGenFlt::do_matcodegen_node( dag_index node_num )
{

  Node* CNode = c_dag->node_vec[ node_num ];
 

  switch( CNode->node_type )
    {

   
    case ADD: gen_add_node_exp( CNode ); break;


    case SUB: gen_sub_node_exp( CNode ); break;


    case MUL: gen_mul_node_exp( CNode ); break;

    case DIV: gen_div_node_exp( CNode ); break;

    case CONSTANT: gen_const_node_exp( CNode ); break;
   
    case OUTPUT : gen_output_node_exp( CNode ); break;

    case INPUT : gen_input_node_exp( CNode ); break;

    defualt:;



    }


}


int MatCodeGenFlt::set_signal_order( Edge *CEdge )
{

  //  if ( (c_dag->node_vec[ CEdge->get_dst_node() ])->node_type != OUTPUT ){

  //     signal_order[  signal_ctr++  ] = out_edge;

  //     return signal_order - 1;
  //   }
  //   else
  //     {

  //       return -1;
  //     }



   

  HWfloat *t_var =  dynamic_cast<HWfloat *>( CEdge->my_var );

  if ( t_var->man_bw_state == NOTSET )
    {
      
      signal_order[  signal_ctr++  ] = CEdge;

      return signal_ctr - 1;


    }
  else
    {

      return -1;
    }
   

}





void MatCodeGenFlt::gen_const_node_exp( Node *CNode )
{

  Edge *out_edge = c_dag->edge_vec[ CNode->out_edges.back() ];

  if ( out_edge->edge_type != ECONSTANT ){

    f_body << endl << "% Const Edge " << endl;

    int sig_num = set_signal_order( out_edge );

    f_body << out_edge->my_var->var_name << "_r = " << CNode->node_val << ";" << endl;
    f_body << out_edge->my_var->var_name << " = my_"<< rmode << "( " << CNode->node_val;

    if (sig_num > 0)
      f_body << " , bw_vec( " << sig_num << ") );" << endl <<  endl;
    else
      f_body << " , " << (dynamic_cast<HWfloat *>( out_edge->my_var )->man_bw) << " );" << endl <<  endl;


    used_edges[ out_edge ] = 1;


 

  }
}

void MatCodeGenFlt::gen_output_node_exp( Node *CNode )
{

  Edge *in_edge = c_dag->edge_vec[ CNode->in_edges.back() ];

  

  f_body << endl << "% Output Error calculation " << endl;

  f_body << in_edge->my_var->var_name << "_error( i ) = ";
  f_body << in_edge->my_var->var_name << "_r - " << in_edge->my_var->var_name << ";" << endl << endl;

  f_body << in_edge->my_var->var_name << "_r_vec(i) = " << in_edge->my_var->var_name << "_r;" << endl; 
  

  used_edges[ in_edge ] = 1;


}

void MatCodeGenFlt::gen_input_node_exp( Node *CNode )
{

  Edge *out_edge = c_dag->edge_vec[ CNode->out_edges.back() ];

  //int sig_num = set_signal_order( out_edge );

  

  f_body << endl << "% Input Node  " << endl;

  //f_body << "rand_temp = rand();" << endl; 
  f_body << out_edge->my_var->var_name << "_r = rand_temp( k + " << input_ctr - 1 << ");" << endl;
  f_body << out_edge->my_var->var_name << " = rand_temp( k + " << input_ctr - 1 << ");"<< endl << endl;
  used_edges[ out_edge ] = 1;

  input_ctr++;


  HWfloat *t_var;

  if ( t_var = dynamic_cast<HWfloat *>( out_edge->my_var ) ){

    input_sign = ( t_var->c_sign == UNSIGNED ? "unsigned" : "twoscomp" );

    input_bw = max( input_bw , (int)t_var->exp_bw );
    
  }


}




void MatCodeGenFlt::gen_add_node_exp(  Node * CNode )
{
 

  Edge *l_edge, *r_edge, *out_edge;

  


  l_edge = c_dag->edge_vec[ CNode->in_edges[0] ];
  r_edge = c_dag->edge_vec[ CNode->in_edges[1] ];
  out_edge = c_dag->edge_vec[ CNode->out_edges.back() ];
  
  int sig_num = set_signal_order( out_edge );

  f_body << endl << "% Add Node  " << endl;


  f_body << out_edge->my_var->var_name << " = my_" << rmode << "( ";
  f_body << l_edge->my_var->var_name << " + " << r_edge->my_var->var_name;

  if (sig_num > 0)
    f_body << " , bw_vec( " << sig_num << ") );" << endl <<  endl;
  else
    f_body << " , " << (dynamic_cast<HWfloat *>( out_edge->my_var )->man_bw) << " );" <<  endl;


   

  f_body << out_edge->my_var->var_name << "_r = ";
  f_body << l_edge->my_var->var_name << "_r + " << r_edge->my_var->var_name << "_r;" << endl << endl; 
  


  used_edges[ l_edge ] = 1;
  used_edges[ r_edge ] = 1;
  used_edges[ out_edge ] = 1;


  
 
}

void MatCodeGenFlt::gen_sub_node_exp(  Node * CNode )
{

  Edge  *l_edge, *r_edge, *out_edge;

  l_edge = c_dag->edge_vec[ CNode->in_edges[0] ];
  r_edge = c_dag->edge_vec[ CNode->in_edges[1] ];
  out_edge = c_dag->edge_vec[ CNode->out_edges.back() ];
 
  int sig_num = set_signal_order( out_edge );

  f_body << endl << "% Sub Node  " << endl;


  f_body << out_edge->my_var->var_name << " = my_" << rmode << "( ";
  f_body << l_edge->my_var->var_name << " - " << r_edge->my_var->var_name;

  if (sig_num > 0)
    f_body << " , bw_vec( " << sig_num << ") );" << endl <<  endl;
  else
    f_body << " , " << (dynamic_cast<HWfloat *>( out_edge->my_var )->man_bw) << " );" << endl <<  endl;
   

  f_body << out_edge->my_var->var_name << "_r = ";
  f_body << l_edge->my_var->var_name << "_r - " << r_edge->my_var->var_name << "_r;" << endl << endl; 


  used_edges[ l_edge ] = 1;
  used_edges[ r_edge ] = 1;
  used_edges[ out_edge ] = 1;

  
}


void MatCodeGenFlt::gen_mul_node_exp(  Node *CNode )
{
  Edge *l_edge, *r_edge, *out_edge;

  l_edge = c_dag->edge_vec[ CNode->in_edges[0] ];
  r_edge = c_dag->edge_vec[ CNode->in_edges[1] ];
  out_edge = c_dag->edge_vec[ CNode->out_edges.back() ];
  
  int sig_num = set_signal_order( out_edge );
  
  f_body << endl << "% Mul Node  " << endl;


  f_body << out_edge->my_var->var_name << " = my_" << rmode << "( ";
  f_body << l_edge->my_var->var_name << " * " << r_edge->my_var->var_name;


  if (sig_num > 0)
    f_body << " , bw_vec( " << sig_num << ") );" << endl <<  endl;
  else
    f_body << " , " << (dynamic_cast<HWfloat *>( out_edge->my_var )->man_bw) << " );" << endl <<  endl;

   

  f_body << out_edge->my_var->var_name << "_r = ";
  f_body << l_edge->my_var->var_name << "_r * " << r_edge->my_var->var_name << "_r;" << endl << endl; 


  used_edges[ l_edge ] = 1;
  used_edges[ r_edge ] = 1;
  used_edges[ out_edge ] = 1;

  

}


void MatCodeGenFlt::gen_div_node_exp(  Node *CNode )
{
  Edge *l_edge, *r_edge, *out_edge;

  l_edge = c_dag->edge_vec[ CNode->in_edges[0] ];
  r_edge = c_dag->edge_vec[ CNode->in_edges[1] ];
  out_edge = c_dag->edge_vec[ CNode->out_edges.back() ];
  
  int sig_num = set_signal_order( out_edge );
  
  f_body << endl << "% Div Node  " << endl;


  f_body << out_edge->my_var->var_name << " = my_" << rmode << "( ";
  f_body << l_edge->my_var->var_name << " / " << r_edge->my_var->var_name;


  if (sig_num > 0)
    f_body << " , bw_vec( " << sig_num << ") );" << endl <<  endl;
  else
    f_body << " , " << (dynamic_cast<HWfloat *>( out_edge->my_var )->man_bw) << " );" << endl <<  endl;

   

  f_body << out_edge->my_var->var_name << "_r = ";
  f_body << l_edge->my_var->var_name << "_r / " << r_edge->my_var->var_name << "_r;" << endl << endl; 


  used_edges[ l_edge ] = 1;
  used_edges[ r_edge ] = 1;
  used_edges[ out_edge ] = 1;

  
}

void  MatCodeGenFlt::gen_var_decs()
{

  for( map<Edge *, int>::iterator e_itr = used_edges.begin(); e_itr != used_edges.end();e_itr++)
    {
	
     
	dec_body << (*e_itr).first->my_var->var_name << " HWfloat();" << endl;
     
    }


}


void MatCodeGenFlt::gen_header( ostream &os ){

  os << "function mat_test( max_count , bw_vec )\n" << endl; 


  os << "%% Code Generated by BitSize - MatCodeGenFlt\n" << endl;

  os << "%% (c) \n" << endl;


  Edge *CEdge;
 
  os << "if ( nargin == 1 )\n";

  os << "bw_vec = [ ";

  os << "end;\n";

  for( int i = 1 ; i < signal_ctr ; i++ )
    {

      CEdge = signal_order[ i ];

     

      os << dynamic_cast<HWfloat *>( CEdge->my_var)->man_bw << " ";

    }

  os << "];" << endl;

  os << "no_of_inputs = " << input_ctr << ";"<< endl;
  os << "input_sign = '" << input_sign << "';" << endl;
  os << "input_exp_bw = " << input_bw << ";" << endl;


  os << endl << "rand_temp = gen_rand_flt( input_exp_bw ,  no_of_inputs , max_count , '' );" << endl;

  os << endl << "k = 1;" << endl;

  os << "for i=1:max_count" << endl;
  


}


void MatCodeGenFlt::gen_footer( ostream &os ){


  os << endl << "k = k + " << input_ctr << ";" << endl;

  os << endl << "end;" << endl;

  vector<dag_index>::iterator e_itr;

  stringstream ss1,ss2;

  Edge *CEdge;

  int output_frac;

  for ( e_itr = c_dag->output_edges.begin();  e_itr != c_dag->output_edges.end();  e_itr++ )
    {
      
      CEdge = c_dag->edge_vec[ *e_itr ]; 
      ss1 << CEdge->my_var->var_name << "_r_vec ";
      ss2 << CEdge->my_var->var_name << "_error ";

      output_frac = (int)(dynamic_cast<HWfloat *>( CEdge->my_var )->man_bw);
    }


  os << "output_frac = " << output_frac << ";\n";

  os << "all_outputs = [ " << ss1.str() << "];" << endl;
  os << "all_errors = [ " << ss2.str() << "];" << endl;

  os << "max_ulp_error = max(abs(all_errors))*2^output_frac;\n";
  os << "avg_ulp_error = mean(abs(all_errors))*2^output_frac;\n";
  os << "snr = get_snr(all_outputs, all_errors);\n";

  os << "max_rel_error = max( (all_errors ./ all_outputs) * 100 );\n";
  os << "avg_rel_error = mean( (all_errors ./ all_outputs) * 100 );\n";

  os << "fprintf(1,' max ulp error = %f\\n', max_ulp_error)\n";
  os << "fprintf(1,' avg ulp error = %f\\n', avg_ulp_error)\n";

  os << "fprintf(1,' max rel error = %f\\n', max_rel_error)\n";
  os << "fprintf(1,' avg rel error = %f\\n', avg_rel_error)\n";


  os << "fprintf(1,' snr           = %f\\n', snr)\n";



  os << "function snr = get_snr(fx_real, fx_error)\n";
  os << "for i=1:length(fx_real)\n";
  os << "fx_real_sq(i) = fx_real(i)^2;\n";
  os << "fx_error_sq(i) = fx_error(i)^2;\n";
  os << "end;\n\n";

  os << "snr = 10*log10(mean(fx_real_sq)/mean(fx_error_sq));\n\n";

  os << "function fin_pr = my_round(val,bw);\n";
  os << "[ m, e ] = log2( val );\n";
  os << "m = m * 2;\n";
  os << "e = e - 1;\n";
  os << "fin_pr_m = (2^-bw)*floor(m*2^bw);\n";
  os << "fin_pr = pow2( fin_pr_m , e );\n";
  

  os << "function fin_pr = my_truncate(val,bw);\n";
  os << "[ m, e ] = log2( val );\n";
  os << "m = m * 2;\n";
  os << "e = e - 1;\n";
  os << "fin_pr_m = (2^-bw)*round(m*2^bw);\n";
  os << "fin_pr = pow2( fin_pr_m , e );\n";



  os << "%---------------------------------------------------------------" << endl;



}
