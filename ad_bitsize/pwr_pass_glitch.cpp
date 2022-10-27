#include "bitsize_pass.h"
#define PASS_ONE 1
#define PASS_TWO 2

PwrPassGlitch::PwrPassGlitch(DAG *_dag , NodeSchedular *_node_sched ):BitSizePass(_dag),node_sched(_node_sched),mat_code_gen(true){}



void PwrPassGlitch::operator()( ostream &os )
{
  init_name_maps();

  do_pwr_pass();

  // for( map<Edge*, GLITCH_PROF *>::iterator s_itr = edge_glitch_map.begin(); s_itr != edge_glitch_map.end(); s_itr++ )
  //     {

  //       GLITCH_PROF* G_Prof = (*s_itr).second;

  //       Edge* c_edge = (*s_itr).first;

  //       os << c_edge->edge_name << " " << G_Prof->peak_pos << " " << G_Prof->peak_height << " " << G_Prof->bit_width << endl;

  //       //os << c_edge->edge_name << endl;

  //     }


  pwr_cost_header();

  os << h_body.str() << endl;

  os << f_body.str() << endl;

  pwr_cost_footer();

  os << foot_body.str() << endl;
}



void PwrPassGlitch::do_pwr_pass()
{

  _DBOUT( "Doing PWR pass 1" );

  node_sched->sort_schedule();

  vector< NodeSchedular::node_vec_type >::iterator sch_itr; 
  vector< dag_index >::iterator n_itr2;

  int ctr = 0;
  for ( sch_itr = node_sched->schedule.begin() ; sch_itr != node_sched->schedule.end() ;  sch_itr++ )
    {

     
      
      for( n_itr2 = (*sch_itr).begin() ; n_itr2 != (*sch_itr).end() ; n_itr2++ )
	{

	    do_pwr_pass_node((*n_itr2));

	}  




    }



}


void PwrPassGlitch::do_pwr_pass_node( DAG_NODE node_num )
{

  Node* CNode = c_dag->node_vec[ node_num ];
 


  switch( CNode->node_type )
    {

   
    case ADD: pwr_add_node_exp( CNode ); break;


    case SUB: pwr_sub_node_exp( CNode); break;


    case MUL: pwr_mul_node_exp( CNode); break;

    case DIV: pwr_div_node_exp( CNode); break;

    case CONSTANT: pwr_const_node_exp( CNode); break;
   
    case OUTPUT : pwr_output_node_exp( CNode); break;

    case INPUT : pwr_input_node_exp( CNode); break;

    case SHIFTL : pwr_shiftl_node( CNode ); break;

     case SHIFTR : pwr_shiftr_node( CNode ); break;

    defualt:;



    }

}



void PwrPassGlitch::pwr_add_node_exp(  Node * CNode  ){


 Edge *edge_l = c_dag->edge_vec[ CNode->in_edges[0] ];
 Edge *edge_r = c_dag->edge_vec[ CNode->in_edges[1] ];
 Edge *edge_out = c_dag->edge_vec[ CNode->out_edges.back() ];


 //cout << edge_l->edge_name << "  " << edge_r->edge_name << " " << edge_out->edge_name << endl;


 Node *node_l = c_dag->get_src_node( edge_l );
 Node *node_r = c_dag->get_src_node( edge_r );


 GLITCH_PROF *G_prof;

 

 if ( ( node_l->node_type == INPUT ) && ( node_r->node_type == INPUT ) ){
   
   G_prof = G_Prof_Add_ng( edge_l , edge_r );
   
   edge_glitch_map[ edge_out ] = G_prof;

   if( mat_code_gen ){

     MAT_G_Prof_Add_ng( edge_l , edge_r  , edge_out );

   }


 }
 else{
   G_prof = G_Prof_Add_g( edge_glitch_map[ edge_l ] , edge_glitch_map[ edge_r ]  );
   
   edge_glitch_map[ edge_out ] = G_prof;

    if( mat_code_gen ){

     MAT_G_Prof_Add( edge_l , edge_r  , edge_out);

   }

   
 }



}




void PwrPassGlitch::pwr_sub_node_exp(  Node * CNode  ){

  Edge *edge_l = c_dag->edge_vec[ CNode->in_edges[0] ];
  Edge *edge_r = c_dag->edge_vec[ CNode->in_edges[1] ];
  Edge *edge_out = c_dag->edge_vec[ CNode->out_edges.back() ];

  Node *node_l = c_dag->get_src_node( edge_l );
  Node *node_r = c_dag->get_src_node( edge_r );


  GLITCH_PROF *G_prof;

 


  if ( ( node_l->node_type == INPUT ) && ( node_r->node_type == INPUT ) ){
   
    G_prof = G_Prof_Sub_ng( edge_l , edge_r );
   
    edge_glitch_map[ edge_out ] = G_prof;

    if( mat_code_gen ){

      MAT_G_Prof_Sub_ng( edge_l , edge_r  , edge_out);

    }


  }
  else{
    G_prof = G_Prof_Sub_g( edge_glitch_map[ edge_l ] , edge_glitch_map[ edge_r ]  );
   
    edge_glitch_map[ edge_out ] = G_prof;

    if( mat_code_gen ){

      MAT_G_Prof_Sub( edge_l , edge_r  , edge_out);

    }

   
  }
 

  //cout << edge_l->edge_name << "  " << edge_r->edge_name << " " << edge_out->edge_name << endl;


}


void PwrPassGlitch::pwr_mul_node_exp(  Node * CNode  ){

 Edge *edge_l = c_dag->edge_vec[ CNode->in_edges[0] ];
 Edge *edge_r = c_dag->edge_vec[ CNode->in_edges[1] ];
 Edge *edge_out = c_dag->edge_vec[ CNode->out_edges.back() ];
 


 //cout << edge_l->edge_name << "  " << edge_r->edge_name << " " << edge_out->edge_name << endl;

 Node *node_l = c_dag->get_src_node( edge_l );
 Node *node_r = c_dag->get_src_node( edge_r );


 GLITCH_PROF *G_prof;

 
  if ( ( node_l->node_type == INPUT ) && ( node_r->node_type == INPUT ) ){
   
    G_prof = G_Prof_Mul_ng( edge_l , edge_r );
   
    edge_glitch_map[ edge_out ] = G_prof;

    if( mat_code_gen ){

      MAT_G_Prof_Mul_ng( edge_l , edge_r  , edge_out);

    }

  }
  else{
    G_prof = G_Prof_Mul_g( edge_glitch_map[ edge_l ] , edge_glitch_map[ edge_r ]  );
   
    edge_glitch_map[ edge_out ] = G_prof;

    if( mat_code_gen ){

      MAT_G_Prof_Mul( edge_l , edge_r  , edge_out);

    }

   
  }

}

void PwrPassGlitch::pwr_shiftl_node(  Node * CNode ){


  Edge *edge_l = c_dag->edge_vec[ CNode->in_edges[0] ];
  Edge *edge_r = c_dag->edge_vec[ CNode->in_edges[1] ];
  Edge *edge_out = c_dag->edge_vec[ CNode->out_edges.back() ];

  Node *node_l = c_dag->get_src_node( edge_l );
  Node *node_r = c_dag->get_src_node( edge_r );


  if (node_l->node_type == CONSTANT ){
    if( mat_code_gen ){
      MAT_G_Prof_CShift_l( edge_r , node_l->node_val , edge_out );
    }

  }else if ( node_r->node_type == CONSTANT ){
    if( mat_code_gen ){  
      MAT_G_Prof_CShift_l( edge_l , node_r->node_val , edge_out );
    }
  }else {}


}

void PwrPassGlitch::pwr_shiftr_node(  Node * CNode ){

  Edge *edge_l = c_dag->edge_vec[ CNode->in_edges[0] ];
  Edge *edge_r = c_dag->edge_vec[ CNode->in_edges[1] ];
  Edge *edge_out = c_dag->edge_vec[ CNode->out_edges.back() ];

  Node *node_l = c_dag->get_src_node( edge_l );
  Node *node_r = c_dag->get_src_node( edge_r );


  if (node_l->node_type == CONSTANT ){
    if( mat_code_gen ){
      MAT_G_Prof_CShift_r( edge_r , node_l->node_val , edge_out );
    }

  }else if ( node_r->node_type == CONSTANT ){
    if( mat_code_gen ){  
      MAT_G_Prof_CShift_r( edge_l , node_r->node_val , edge_out );
    }
  }else {}


}


void PwrPassGlitch::pwr_div_node_exp(  Node * CNode  ){


}

void PwrPassGlitch::pwr_const_node_exp( Node * CNode ){

}


void PwrPassGlitch::pwr_output_node_exp( Node *CNode  ){



}
  
void PwrPassGlitch::pwr_input_node_exp( Node *CNode  ){


 Edge *edge_out = c_dag->edge_vec[ CNode->out_edges.back() ];

 edge_out->data_log.cal_values();

 MAT_G_Prof_Input( edge_out );

 // cout << edge_out->edge_name << " " << edge_out->data_log.mean << " :: ";
//  cout << edge_out->data_log.var << " :: " << edge_out->data_log.acoef_1 << endl; 
 

}
