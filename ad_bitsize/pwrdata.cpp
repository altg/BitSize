// BitSize pass to generate data for pwr analysis
#undef _BITSIZE_DEBUG_

#include "bitsize_pass.h"


PwrDataPass::PwrDataPass(DAG *_dag , NodeSchedular *_node_sched ):BitSizePass( _dag ),
			 node_sched(_node_sched),iter_count(0),stop_flag( false ){}

bool PwrDataPass::operator()()
{

  while( do_pwrdata_fullsim() ){

    if( iter_count % 100 == 0 ) cout << iter_count << endl;
  }
  
  return false;

}


bool PwrDataPass::do_pwrdata_fullsim()
{


  if( iter_count > 0 )
    {

      do_pwrdata_sim();
      iter_count++;

    }
  else{
    fs.open( "asctest.dat" , ios::in );
    d_out.open( "data_out.dat" , ios::out );
    
    do_pwrdata_sim();
       
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



void PwrDataPass::do_pwrdata_sim()
{
  
 
  node_sched->sort_schedule();

  _DBOUT( "Doing Power Data extraction" )

  vector< NodeSchedular::node_vec_type >::iterator sch_itr; 
  vector< dag_index >::iterator n_itr2;

  

  int ctr = 0;
  for ( sch_itr = node_sched->schedule.begin() ; (!stop_flag) && ( sch_itr != node_sched->schedule.end()) ;  sch_itr++ )
    {

      _DBOUT( "Schedule no " << ctr++ );

      for( n_itr2 = (*sch_itr).begin() ; n_itr2 != (*sch_itr).end() ; n_itr2++ )
	{

	    do_pwrdata_sim_node((*n_itr2));

	}  




    }

  

}


void PwrDataPass::do_pwrdata_sim_node(dag_index node_num)
{
 
  Node* CNode = c_dag->node_vec[ node_num ];

  //  _DBOUT( CNode->node_name << " " << c_dag->Node_OPNAME[ CNode->node_type ] );

  switch( CNode->node_type )
    {

    case INPUT: sim_input_node( CNode ); break;  

    case ADD: sim_add_node( CNode ); break;

    case SUB: sim_sub_node( CNode ); break;

    case MUL: sim_mul_node( CNode ); break;

    case DIV: sim_div_node( CNode ); break;

    case SHIFTL: sim_shiftl_node( CNode ); break;

    case SHIFTR: sim_shiftr_node( CNode ); break;

    case CONSTANT: sim_const_node( CNode ); break;

    case OUTPUT: sim_output_node( CNode ); break;  

    case ASSIGN: sim_assign_node( CNode ); break;

      //case DIV: gen_div_node_exp( os , CNode ); break;


    defualt:;



    }

  


}


double PwrDataPass::read_from_file()
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

void PwrDataPass::sim_const_node( Node *CNode )
{
  Edge *edge_out = c_dag->edge_vec[ CNode->out_edges.back() ];

  edge_out->val = CNode->node_val;
  
  edge_out->data_log( edge_out->val );
}


void PwrDataPass::sim_input_node( Node *CNode ){


  Edge *edge_out = c_dag->edge_vec[ CNode->out_edges.back() ];
  
  double temp = read_from_file();


  if( !stop_flag ){
    edge_out->val = temp;
     edge_out->data_log( edge_out->val );
  }
  else{
    edge_out->val = 0.;
  }

 

}


// double PwrDataPass::get_const_value( Edge *CEdge )
// {

//   Node *CNode = c_dag->node_vec[ CEdge->get_src_node() ];

//   return CNode->node_val;


// }
  
void PwrDataPass::sim_add_node( Node *CNode ){

  Edge *edge_l = c_dag->edge_vec[ CNode->in_edges[0] ];
  Edge *edge_r = c_dag->edge_vec[ CNode->in_edges[1] ];
  Edge *edge_out = c_dag->edge_vec[ CNode->out_edges.back() ];
  
  edge_out->val = edge_l->val + edge_r->val;

  edge_out->data_log( edge_out->val );
  


}
  

void PwrDataPass::sim_sub_node( Node *CNode ){

  Edge *edge_l = c_dag->edge_vec[ CNode->in_edges[0] ];
  Edge *edge_r = c_dag->edge_vec[ CNode->in_edges[1] ];
  Edge *edge_out = c_dag->edge_vec[ CNode->out_edges.back() ];
  
  edge_out->val = edge_l->val - edge_r->val;

  edge_out->data_log( edge_out->val );


}
  

void PwrDataPass::sim_mul_node( Node *CNode ){

  Edge *edge_l = c_dag->edge_vec[ CNode->in_edges[0] ];
  Edge *edge_r = c_dag->edge_vec[ CNode->in_edges[1] ];
  Edge *edge_out = c_dag->edge_vec[ CNode->out_edges.back() ];
  
  Node *node_l = c_dag->node_vec[ edge_l->get_src_node() ];
  Node *node_r = c_dag->node_vec[ edge_r->get_src_node() ];

  if ( node_l->node_type == CONSTANT ){

    edge_l->data_log( edge_l->val );
  }

  if ( node_r->node_type == CONSTANT ){

    edge_r->data_log( edge_r->val );
  }


  

  edge_out->val = edge_l->val * edge_r->val;

  edge_out->data_log( edge_out->val );



}


void PwrDataPass::sim_div_node( Node *CNode ){

  Edge *edge_l = c_dag->edge_vec[ CNode->in_edges[0] ];
  Edge *edge_r = c_dag->edge_vec[ CNode->in_edges[1] ];
  Edge *edge_out = c_dag->edge_vec[ CNode->out_edges.back() ];
  
  edge_out->val = edge_l->val / edge_r->val;

  edge_out->data_log( edge_out->val );


}
  

void PwrDataPass::sim_shiftl_node( Node *CNode ){

  Edge *edge_l = c_dag->edge_vec[ CNode->in_edges[0] ];
  Edge *edge_r = c_dag->edge_vec[ CNode->in_edges[1] ];
  Edge *edge_out = c_dag->edge_vec[ CNode->out_edges.back() ];

  Node *node_r = c_dag->node_vec[ edge_r->get_src_node() ];


  edge_out->val = edge_l->val * pow( 2. ,  node_r->node_val);

   edge_out->data_log( edge_out->val );

}
  

void PwrDataPass::sim_shiftr_node( Node *CNode ){

  Edge *edge_l = c_dag->edge_vec[ CNode->in_edges[0] ];
  Edge *edge_r = c_dag->edge_vec[ CNode->in_edges[1] ];
  Edge *edge_out = c_dag->edge_vec[ CNode->out_edges.back() ];

  Node *node_r = c_dag->node_vec[ edge_r->get_src_node() ];

  edge_out->val = edge_l->val * pow( 2. ,  -node_r->node_val);

  edge_out->data_log( edge_out->val );
}

void  PwrDataPass::sim_output_node( Node *CNode )
{

  

  Edge *edge_out = c_dag->edge_vec[ CNode->in_edges.back() ];

  _DBOUT( CNode->node_name << " :: " << edge_out->val )

  edge_out->data_log( edge_out->val );

  d_out << edge_out->val << endl;

}


void PwrDataPass::sim_assign_node( Node *CNode )
{

  Edge *edge_in = c_dag->edge_vec[ CNode->in_edges.back()];
  
  Edge *edge_out = c_dag->edge_vec[ CNode->out_edges.back()];
  
  edge_out->data_log( edge_in->val );
}
