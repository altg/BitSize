
#include "errormodel.h"


ErrorModel::ErrorModel(DAG *_dag , NodeSchedular *_node_sched,   RangeAnalysis *_range_ann):c_dag( _dag ),node_sched(_node_sched),range_analysis(_range_ann),rounding("-1"){}


void ErrorModel::gen_matlab_errfiles(){

  _DBOUT( "Generating Matlab Files" );

 //  fstream err_fs( "error_function.m" , ios::out );

//   gen_error_exp( err_fs );


//   err_fs.close();


  // fstream cost_fs( "cost_function.m" , ios::out );

//   gen_node_cost_func( cost_fs );

//   cost_fs.close();

 //  fstream param_fs( "parameters.m" , ios::out );

//   gen_parameters_func( param_fs );

//   param_fs.close();



  // Affine stuff

  

}




void ErrorModel::gen_error_exp( ostream &os ){

    
  (*range_analysis)();
  
   
  

 
  stringstream ss;

  vector< NodeSchedular::node_vec_type >::iterator sch_itr; 
  vector< dag_index >::iterator n_itr2;

  int ctr = 0;
  for ( sch_itr = node_sched->schedule.begin() ; sch_itr != node_sched->schedule.end() ;  sch_itr++ )
    {

      // cout <<  node_sched->schedule.size() << " :: " << temp_vec->size() << endl;
      ss << endl <<"% Schedule No : " << ctr++ << endl;
      for( n_itr2 = (*sch_itr).begin() ; n_itr2 != (*sch_itr).end() ; n_itr2++ )
	{

	  gen_node_error_exp( ss , (*n_itr2) );


	}  




    }

  


  os << "function r = error_function( x )" << endl;

  os << "[no_of_bw, bw_out] = parameters();" << endl;

  int i = 1;

  for( map<string,int>::iterator s_itr = input_arg_names.begin(); s_itr != input_arg_names.end(); s_itr++ ,  i++ )
    {

      os  << (*s_itr).first << " = x(" << i << ");"<< endl;
      
      //if( ++s_itr != input_arg_names.end()) os << ",";

    }

  


  os << ss.str();

 
  gen_output_vector( os );

  os << endl;


 

}


void ErrorModel::gen_output_vector( ostream &os ){

  vector<dag_index>::iterator e_itr;

  Node *srcNode,*dstNode;

  Edge *CEdge;

  HWfix *temp;

  


  for( e_itr = c_dag->output_edges.begin() ; e_itr != c_dag->output_edges.end() ; e_itr++ )
    {

      stringstream ss,ss2;
      
      CEdge = c_dag->edge_vec[ *e_itr ];
      srcNode = c_dag->node_vec[CEdge->get_src_node()];
      dstNode = c_dag->node_vec[CEdge->get_dst_node()];
      
      ss2 << "NODE_" << dstNode->node_name;
      os << ss2.str() << " = sum([ ";
      output_node_names.push_back( ss2.str() );

      os << "Error_" << srcNode->node_name;
      
      os << " 2^( " <<  "-bw_out"  << rounding << ") ";
      os << "]);" << endl;
 
      output_names.push_back( ss.str() );
    }

  vector<string>::iterator s_itr;
    os << "r = [ ";

    for( s_itr = output_node_names.begin() ; s_itr != output_node_names.end() ;++s_itr )
    {

      os << (*s_itr) << " ";
   


    }

    os << "];" << endl;
}

void ErrorModel::gen_parameters_func( ostream &os ){

  os << "function [no_of_bw, ";

  vector<string>::iterator s_itr;

  for( s_itr = output_names.begin() ; s_itr != output_names.end() ; )
    {

      os << (*s_itr);

      if(++s_itr != output_names.end() ) os << ", ";


    }


  os << "] = parameters" << endl;

  os << "no_of_bw = " << input_arg_names.size() << ";" << endl;

  //os << "bw_out = 12" << ";" << endl;
  
  

  for( s_itr = output_names.begin() ; s_itr != output_names.end() ; s_itr++ )
    {

      os << (*s_itr) << " = 14;" << endl;
     
    }

}

void cost_function_end(ostream &os ){


  os << "\n% Cost Functions " << endl; 

  os << "function C = cost_mul( bw_A , bw_B ) \n";

  os << "C = bw_A * bw_B;\n";


  os << endl;  

  os << "function C = cost_addsub( bw_A , bw_B ) \n";

  os << "C = max( bw_A , bw_B);\n";
  

}

void ErrorModel::gen_node_cost_func( ostream &os ){


  os << "function [cost_value, cost_flag]  = cost_function( x )" << endl;;

  os << "[no_of_bw, bw_out] = parameters();" << endl;

  os << "req_error = 2.^-bw_out;" << endl;

  int i = 1;
  for( map<string,int>::iterator s_itr = input_arg_names.begin(); s_itr != input_arg_names.end(); s_itr++ , i++ )
    {

      os << (*s_itr).first << " = x(" << i << ");"<< endl;
      
      //if( ++s_itr != input_arg_names.end()) os << ",";

    }

  os << "my_error = error_function(x);" << endl;
  
  // os << "if (my_error > req_error)" << endl;
  
  os << "if ( ";

  for( int i = 1 ; i  <= output_names.size() ; i++ )
    {
      os << "( my_error( " << i << " ) > req_error( " << i << " ))";  

      if ( i != output_names.size() ) os << " & ";
    }
  os << " ) " << endl;

  os << "cost_flag = 0;" << endl;
  os << "cost_value = 0;" << endl;
  os << "else" << endl;

  // Produce cost function body
  os << c_ss.str();
  os << "cost_value = sum( [ ";
  
   for( vector<string>::iterator s_itr = expr_names_cost.begin(); s_itr != expr_names_cost.end(); s_itr++ )
    {

       os << (*s_itr) << " ";

    }

    os << " ] );" << endl;

  os << "cost_flag = 1;" << endl; 


  os << "end;" << endl;

  cost_function_end(os);
}


void ErrorModel::gen_node_error_exp( ostream &os , dag_index node_num ){

  Node *CNode = c_dag->node_vec[ node_num ];

  
  

  switch( CNode->node_type )
    {

    case INPUT: gen_input_node_exp( os , CNode ); break;  

    case ADD: gen_add_node_exp( os , CNode ); break;


    case SUB: gen_sub_node_exp( os , CNode ); break;


    case MUL: gen_mul_node_exp( os , CNode ); break;

    case SHIFTL: gen_mul_node_exp( os , CNode ); break;

    case SHIFTR: gen_mul_node_exp( os , CNode ); break;


    case DIV: gen_div_node_exp( os , CNode ); break;

    case CONSTANT: gen_const_node_exp( os ,  CNode ); break;

    defualt:;



    }

  

  

}


void ErrorModel::gen_signal_bwname( stringstream &ss , HWvar* _var)
{

  HWfix *temp;
  
  if( temp =  dynamic_cast<HWfix *>( _var ) )
    {
      ss << "bw_" << temp->var_name;
      input_arg_names[ss.str()] = 1; 
    }

}


void ErrorModel::gen_signal_bwname( stringstream &ss , Node* _node)
{
     
  if( _node->node_type != CONSTANT )
    {
   
      ss << "bw_" << _node->node_name;
     
    }
  else{

    Edge *CEdge = c_dag->edge_vec[ _node->out_edges.back() ];

    if( CEdge->edge_type == ECONSTANT )
      {

	ss << "bw_" << _node->node_name;

      }
    else
      {

	gen_signal_bwname( ss , CEdge->my_var );

      }


  }

   input_arg_names[ss.str()] = 1; 
  
}








void ErrorModel::gen_signal_err_expr( stringstream &out_ss , stringstream &in_ss )
{


  out_ss << "2^(-" << in_ss.str() << rounding << ")";

} 

void ErrorModel::gen_input_node_exp(  ostream &os , Node* CNode )
{

  stringstream expr_name;

  os << "% " << CNode->node_name << " " << c_dag->Node_OPNAME[ CNode->node_type ]<< endl;
  

  expr_name << "Error_" << CNode->node_name;
  
  expr_names.push_back( expr_name.str() ); 

  HWfix *temp_out;

  Edge *edge_out;

  stringstream  e_xpr_out;

  stringstream  arg_name_out;
  
  edge_out = c_dag->edge_vec[ CNode->out_edges.back() ];

  gen_signal_bwname( arg_name_out , edge_out->my_var );
  gen_signal_err_expr( e_xpr_out , arg_name_out );
  
  os << expr_name.str() << " = "<< e_xpr_out.str() <<";" << endl;

}


void ErrorModel::gen_add_node_exp(  ostream &os , Node* CNode )
{

  stringstream expr_name, expr_name_cost;

  os << "% " << CNode->node_name << " " << c_dag->Node_OPNAME[ CNode->node_type ]<< endl;

  expr_name << "Error_" << CNode->node_name;
  expr_name_cost << "Cost_" << CNode->node_name;
  os << expr_name.str() << " = sum( [ ";

  Edge *edge_l, *edge_r, *edge_out;

  edge_l = c_dag->edge_vec[ CNode->in_edges[0] ];
  edge_r = c_dag->edge_vec[ CNode->in_edges[1] ];
 
  edge_out = c_dag->edge_vec[ CNode->out_edges.back() ];
 
  HWfix *temp_l , *temp_r , *temp_out;

  

  stringstream e_xpr_l , e_xpr_r , e_xpr_out;

  stringstream arg_name_l , arg_name_r , arg_name_out;

  Node *srcNode;


  Node *srcNode_l = c_dag->node_vec[ edge_l->get_src_node() ];
  Node *srcNode_r = c_dag->node_vec[ edge_r->get_src_node() ];

  
  if( srcNode_l->node_type == CONSTANT ){

    srcNode = c_dag->node_vec[ edge_l->get_src_node() ];
    gen_signal_bwname( arg_name_l , srcNode );
    gen_signal_err_expr( e_xpr_l , arg_name_l );

  
  }
  else{
    
    gen_signal_bwname( arg_name_l , edge_l->my_var );
    e_xpr_l << propagated_error( edge_l );
   
  }

 
  if( srcNode_r->node_type == CONSTANT ){ 
    srcNode = c_dag->node_vec[ edge_r->get_src_node() ];
    gen_signal_bwname( arg_name_r , srcNode );
    gen_signal_err_expr( e_xpr_r , arg_name_r );



  }
  else{
  
    gen_signal_bwname( arg_name_r , edge_r->my_var );
    e_xpr_r << propagated_error( edge_r );
   
  }
  

  os << e_xpr_l.str() << " ";
  os << e_xpr_r.str() << " ";
  if ( edge_out->edge_type != OUT ){
   
    gen_signal_bwname( arg_name_out , edge_out->my_var );
    gen_signal_err_expr( e_xpr_out , arg_name_out );
   
    os << e_xpr_out.str() ;
  }



 
 
  
  os << " ] );" << endl;

  expr_names.push_back( expr_name.str() );
 
  expr_names_cost.push_back( expr_name_cost.str() );

   c_ss << expr_name_cost.str() << " = cost_mul(" << arg_name_l.str() << " , " << arg_name_r.str() << ");" << endl;

}


string ErrorModel::propagated_error( Edge *CEdge )
{

  Node* src_node = c_dag->node_vec[CEdge->get_src_node()];

  stringstream ss, arg_name;
  
  Node *srcNode;
   if( CEdge->edge_type == ECONSTANT ){
    //e_xpr_l << "CONST";
     srcNode = c_dag->node_vec[ CEdge->get_src_node() ];

    arg_name << "bw_" << srcNode->node_name;

    ss << "2^(-" <<arg_name.str() << " " << rounding << ")";

     input_arg_names[arg_name.str()] = 1; 


  }else
     {

      
       ss << "Error_" << src_node->node_name;

       

     }
  return ss.str();

}


void ErrorModel::gen_const_node_exp( ostream &os , Node* CNode )
{
  
  os << "% " << CNode->node_name << " " << c_dag->Node_OPNAME[ CNode->node_type ]<< endl;

  stringstream expr_name;

  expr_name << "Error_" << CNode->node_name;


  Edge *CEdge = c_dag->edge_vec[ CNode->out_edges.back() ];

  stringstream arg_name;
  stringstream e_xpr;

  if ( CEdge->edge_type == ECONSTANT ) {
 
  gen_signal_bwname( arg_name , CNode );
  gen_signal_err_expr( e_xpr , arg_name );


  }
  else{

  gen_signal_bwname( arg_name , CEdge->my_var );
  gen_signal_err_expr( e_xpr , arg_name );


  }
  
  os << expr_name.str() << " = "<< e_xpr.str() <<";" << endl;

}

double ErrorModel::get_const_value( Edge *CEdge )
{

  Node *CNode = c_dag->node_vec[ CEdge->get_src_node() ];

  return CNode->node_val;


}


void ErrorModel::gen_mul_node_exp(  ostream &os , Node* CNode )
{
  
  stringstream expr_name, expr_name_cost;

  os << "% " << CNode->node_name << " " << c_dag->Node_OPNAME[ CNode->node_type ]<< endl;

  expr_name << "Error_" << CNode->node_name;
  expr_name_cost << "Cost_" << CNode->node_name;
  os << expr_name.str() << " = sum( [ ";

   
  Edge *edge_l, *edge_r, *edge_out;
  HWfix *temp_l , *temp_r , *temp_out;
  stringstream e_xpr_l , e_xpr_r , e_xpr_out;
  stringstream arg_name_l , arg_name_r , arg_name_out;


  edge_l = c_dag->edge_vec[ CNode->in_edges[0] ];
  edge_r = c_dag->edge_vec[ CNode->in_edges[1] ];
  edge_out = c_dag->edge_vec[ CNode->out_edges.back() ];
 
  double max_l , max_r;

  Node *srcNode;

  //range_analysis->show_affine_vecs(); 

  Node *srcNode_l = c_dag->node_vec[ edge_l->get_src_node() ];
  Node *srcNode_r = c_dag->node_vec[ edge_r->get_src_node() ];
 

  if( srcNode_l->node_type == CONSTANT ){
    
    srcNode = c_dag->node_vec[ edge_l->get_src_node() ];
    gen_signal_bwname( arg_name_l , srcNode );
    gen_signal_err_expr( e_xpr_l , arg_name_l );

    max_l = get_const_value( edge_l );

  }
  else{
    //temp_l = dynamic_cast<HWfix *>(edge_l->my_var);
    //max_l =  dynamic_cast<HWfix *>(edge_l->my_var)->ad_val.ref_maxval;
   
    max_l = range_analysis->get_range_max( edge_l );

    _DBOUT(  edge_l->edge_name << " " << max_l )

    gen_signal_bwname( arg_name_l , edge_l->my_var );
    e_xpr_l << propagated_error( edge_l );
    
  }

  if( srcNode_r->node_type == CONSTANT ){
    
    srcNode = c_dag->node_vec[ edge_r->get_src_node() ];
    gen_signal_bwname( arg_name_r , srcNode );
    gen_signal_err_expr( e_xpr_r , arg_name_r );

    max_r = get_const_value( edge_r );

  }
  else{
    //temp_r = dynamic_cast<HWfix *>(edge_r->my_var);
    //max_r = dynamic_cast<HWfix *>(edge_r->my_var)->ad_val.ref_maxval;

    max_r = range_analysis->get_range_max( edge_r );
    
    _DBOUT( edge_r->edge_name << " " << max_r )
    gen_signal_bwname( arg_name_r , edge_r->my_var );
    e_xpr_r << propagated_error( edge_r );
      
  }
  

  os << "(" << e_xpr_l.str() << " * "  << max_r << ") ";
  os << "(" << e_xpr_r.str() << " * "  << max_l << ") ";
  os << e_xpr_l.str() << "*" << e_xpr_r.str() << " ";

  if ( edge_out->edge_type != OUT ){
    gen_signal_bwname( arg_name_out , edge_out->my_var );
    gen_signal_err_expr( e_xpr_out , arg_name_out );
    
    os << e_xpr_out.str() << endl;
  }
  
    
  os << " ] );" << endl;

  expr_names.push_back( expr_name.str() );
 
  expr_names_cost.push_back( expr_name_cost.str() );

  //c_ss << expr_name_cost.str() << " = " << arg_name_l.str() << " * " << arg_name_r.str() << ";" << endl;

  c_ss << expr_name_cost.str() << " = cost_addsub(" << arg_name_l.str() << " , " << arg_name_r.str() << ");" << endl;

}

void ErrorModel::gen_sub_node_exp(  ostream &os , Node* CNode )
{
  gen_add_node_exp( os , CNode );
}

void ErrorModel::gen_div_node_exp(  ostream &os , Node* CNode ){

 

  stringstream expr_name;

  os << "% " << CNode->node_name << endl;

  expr_name << "Error_" << CNode->node_name;
  os << expr_name.str() << " = sum( [ ";

  expr_names.push_back( expr_name.str() );

  
}




