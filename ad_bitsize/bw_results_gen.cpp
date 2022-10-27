#include "bw_results_gen.h"

BWResultsGen::BWResultsGen( DAG *_dag ):c_dag(_dag),index(0){}

void BWResultsGen::gen_bitsize_dat( ostream &os ){


  vector<dag_index>::iterator e_itr;

  Edge *CEdge;

  index = 0;
  for( e_itr = c_dag->edges_to_analyse.begin() ;e_itr != c_dag->edges_to_analyse.end() ; e_itr++ ){

    CEdge = c_dag->edge_vec[(*e_itr)];

     if (  CEdge->edge_type != ECONSTANT ){

       if( c_dag->arith_type == FIXED ){
	  gen_fixed_data( CEdge , os );
	
       }
       else if ( c_dag->arith_type == FLOAT ){
	  gen_float_data( CEdge , os );


       }
       else if ( c_dag->arith_type == LNS ){
	  gen_lns_data( CEdge , os );


       }

     }

  }


  for( e_itr = c_dag->output_edges.begin() ;e_itr != c_dag->output_edges.end() ; e_itr++ ){

    CEdge = c_dag->edge_vec[(*e_itr)];

     if (  CEdge->edge_type != ECONSTANT ){

       if( c_dag->arith_type == FIXED ){
	  gen_fixed_data( CEdge , os );
	
       }
       else if ( c_dag->arith_type == FLOAT ){
	  gen_float_data( CEdge , os );

       }
       else if ( c_dag->arith_type == LNS ){
	  gen_lns_data( CEdge , os );

       }

     }

  }


}




void BWResultsGen::gen_fixed_data( Edge *CEdge , ostream &os ){


  HWfix *temp;

  if( temp = dynamic_cast<HWfix*>( CEdge->my_var ) ){

    os << temp->var_name << " : FIX : ";
    os << temp->int_bw << " : ";
    os << temp->frac_bw <<  " : ";
    os << temp->int_bw_state << " : ";
    os << temp->frac_bw_state << endl;
    

  }


}

void BWResultsGen::gen_lns_data( Edge *CEdge , ostream &os ){


  HWlns *temp;

  if( temp = dynamic_cast<HWlns*>( CEdge->my_var ) ){

    os << temp->var_name << " : LNS : ";
    os << temp->int_bw << " : ";
    os << temp->frac_bw <<  " : ";
    os << temp->int_bw_state << " : ";
    os << temp->frac_bw_state << endl;
    

  }


}


// Edge* BWResultsGen::get_assigned_to_node( Edge *CEdge ){

//   Node* CNode = bs->mydag->node_vec[ CEdge->get_dst_node() ];

//   return bs->mydag->edge_vec[ CNode->out_edges.back() ];

// }

fsize fix_exp_bw( fsize exp_bw , fsize man_bw ){

  fsize temp = abs(  static_cast<int>( man_bw )  - static_cast<int>( exp_bw )  );

  return ( temp <= 3 ? exp_bw + 3  : exp_bw );

  

}


void BWResultsGen::gen_float_data( Edge *CEdge , ostream &os ){

  HWfloat *temp;

  

  if( temp = dynamic_cast<HWfloat*>( CEdge->my_var ) ){

    os << temp->var_name << " : FLT : ";

    
    if ( CEdge->edge_type == IN ){
      
      if ( CEdge->next_edge != NULL ){
	
	if( temp = dynamic_cast<HWfloat*>( CEdge->next_edge->my_var  ) ){

	  os << fix_exp_bw( temp->exp_bw , temp->man_bw ) << " : ";
	  os << temp->man_bw <<  " : ";
	  os << temp->exp_bw_state << " : ";
	  os << temp->man_bw_state << endl;

	}



      }

    }else{
      os << fix_exp_bw( temp->exp_bw , temp->man_bw ) << " : ";
      os << temp->man_bw <<  " : ";
      os << temp->exp_bw_state << " : ";
      os << temp->man_bw_state << endl;
    }
    
    // cout << index++ << " : " <<  temp->var_name << " <- " << temp->man_bw << endl;

  }



}


