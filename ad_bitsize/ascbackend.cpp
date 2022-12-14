// Simple backend to print ASC code

#include "bitsize_pass.h"


ASCBackend::ASCBackend(DAG *_dag , NodeSchedular *_node_sched ):BitSizePass(_dag),node_sched(_node_sched){}


void ASCBackend::operator()( ostream &os )
{

  do_ascbackend( os );

}


void ASCBackend::do_ascbackend( ostream &os )
{
  
   _DBOUT( "Doing ASC Backend code generation" );

  vector< NodeSchedular::node_vec_type >::iterator sch_itr; 
  vector< dag_index >::iterator n_itr2;

  int ctr = 0;
  for ( sch_itr = node_sched->schedule.begin() ; sch_itr != node_sched->schedule.end() ;  sch_itr++ )
    {

     
      
      for( n_itr2 = (*sch_itr).begin() ; n_itr2 != (*sch_itr).end() ; n_itr2++ )
	{

	    do_ascbackend_node((*n_itr2));

	}  




    }

  gen_var_decs();

  gen_header( os );

  os << dec_body.str();

  os << f_body.str();

}


void ASCBackend::do_ascbackend_node( dag_index node_num )
{

  Node* CNode = c_dag->node_vec[ node_num ];
 

  switch( CNode->node_type )
    {

   
    case ADD: gen_add_node_exp( CNode ); break;


    case SUB: gen_sub_node_exp( CNode ); break;


    case MUL: gen_mul_node_exp( CNode ); break;

    case CONSTANT: gen_const_node_exp( CNode ); break;
   

    defualt:;



    }


}


void ASCBackend::gen_const_node_exp( Node *CNode )
{

  Edge *out_edge = c_dag->edge_vec[ CNode->out_edges.back() ];

  f_body << out_edge->my_var->var_name << " = " << CNode->node_val << ";" << endl; 
  used_edges[ out_edge ] = 1;
}

void ASCBackend::gen_add_node_exp(  Node * CNode )
{
 

  Edge *l_edge, *r_edge, *out_edge;

  l_edge = c_dag->edge_vec[ CNode->in_edges[0] ];
  r_edge = c_dag->edge_vec[ CNode->in_edges[1] ];
  out_edge = c_dag->edge_vec[ CNode->out_edges.back() ];
  
  f_body << out_edge->my_var->var_name << " = " << l_edge->my_var->var_name << " + " << r_edge->my_var->var_name << ";" << endl; 
  
  used_edges[ l_edge ] = 1;
  used_edges[ r_edge ] = 1;
  used_edges[ out_edge ] = 1;
 
}

void ASCBackend::gen_sub_node_exp(  Node * CNode )
{

  Edge  *l_edge, *r_edge, *out_edge;

  l_edge = c_dag->edge_vec[ CNode->in_edges[0] ];
  r_edge = c_dag->edge_vec[ CNode->in_edges[1] ];
  out_edge = c_dag->edge_vec[ CNode->out_edges.back() ];
  
  f_body << out_edge->my_var->var_name << " = " << l_edge->my_var->var_name << " - " << r_edge->my_var->var_name << ";" << endl; 

  used_edges[ l_edge ] = 1;
  used_edges[ r_edge ] = 1;
  used_edges[ out_edge ] = 1;
}


void ASCBackend::gen_mul_node_exp(  Node * CNode )
{
  Edge *l_edge, *r_edge, *out_edge;

  l_edge = c_dag->edge_vec[ CNode->in_edges[0] ];
  r_edge = c_dag->edge_vec[ CNode->in_edges[1] ];
  out_edge = c_dag->edge_vec[ CNode->out_edges.back() ];
  
  f_body << out_edge->my_var->var_name << " = " << l_edge->my_var->var_name << " * " << r_edge->my_var->var_name << ";" << endl; 
 
  used_edges[ l_edge ] = 1;
  used_edges[ r_edge ] = 1;
  used_edges[ out_edge ] = 1;

}


void  ASCBackend::gen_var_decs()
{

  for( map<Edge *, int>::iterator e_itr = used_edges.begin(); e_itr != used_edges.end();e_itr++)
    {
	
     
	dec_body << (*e_itr).first->my_var->var_name << " HWfix();" << endl;
     
    }


}


void ASCBackend::gen_header( ostream &os ){


  os << "//Code Generated by BitSize - ASCBackend\n" << endl;

  os << "// (c) \n" << endl;



}
