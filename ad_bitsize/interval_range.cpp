#undef _BITSIZE_DEBUG_

#include "range_analysis.h"


IntervalRange::IntervalRange(DAG *_dag , NodeSchedular *_node_sched ):RangeAnalysis( _dag , _node_sched ),
						     interval_range_analysis_done(false){}


void IntervalRange::operator()()
{

  do_interval_analysis();

  update_bw();

  if ( bs->db_level > 0  ) show_interval_vecs();
}

void IntervalRange::do_interval_analysis()
{
  
  if ( interval_range_analysis_done == true ) return;

  node_sched->sort_schedule();

  cout <<  "Doing Interval range analysis" << endl;

  vector< NodeSchedular::node_vec_type >::iterator sch_itr; 
  vector< dag_index >::iterator n_itr2;

  int ctr = 0;
  for ( sch_itr = node_sched->schedule.begin() ; sch_itr != node_sched->schedule.end() ;  sch_itr++ )
    {

      _DBOUT( "Schedule no " << ctr++ );

      for( n_itr2 = (*sch_itr).begin() ; n_itr2 != (*sch_itr).end() ; n_itr2++ )
	{
	  
	  
	  do_interval_analysis_node((*n_itr2));

	}  




    }

  interval_range_analysis_done = true;

}

void IntervalRange::do_interval_analysis_node(dag_index node_num)
{
 
  
  Node* CNode = c_dag->node_vec[ node_num ];


  _DBOUT( CNode->node_name );
  switch( CNode->node_type )
    {

    case CONSTANT: interval_const_node( CNode ); break;

    case INPUT: interval_input_node( CNode ); break;  

    case ADD: interval_add_node( CNode ); break;

    case SUB: interval_sub_node( CNode ); break;

    case MUL: interval_mul_node( CNode ); break;

    case DIV: interval_div_node( CNode ); break;

    case SHIFTL: interval_shiftl_node( CNode ); break;

    case SHIFTR: interval_shiftr_node( CNode ); break;

    case ASSIGN: interval_assign_node( CNode ); break;


      //case DIV: gen_div_node_exp( os , CNode ); break;


    defualt:;



    }

  


}



double IntervalRange::get_range_max( Edge *CEdge)
{
  double range_max;

  HWfix* t_var;

 //  if ( t_var = dynamic_cast<HWfix *>( CEdge->my_var ) ) {
//     if ( t_var->int_bw_state == SET )
//     {

//       range_max = pow( 2. , (double)t_var->int_bw );

//     }
//     else
//       {

// 	range_max = edge_int_map[ CEdge ].hi;
//       }
//   }
//   else{
//       range_max = edge_int_map[ CEdge ].hi;

//   }


  range_max = edge_int_map[ CEdge ].hi;

  return range_max;

}


double IntervalRange::get_range_min( Edge *CEdge)
{
  double range_min = 0;


 //  if ( c_dag->arith_type == FIXED ){
 
//     if ( dynamic_cast<HWfix *>(CEdge->my_var)->c_sign == UNSIGNED )
//       {

// 	range_min = 0;

//       }
//     else 
//       {


// 	range_min =  edge_int_map[ CEdge ].lo;

//       }

//   }
//   else  

  HWfix* t_var;

 //  if ( t_var = dynamic_cast<HWfix *>( CEdge->my_var ) ) {

//    if ( t_var->int_bw_state == SET )
//     {

//       range_min = 0;

//     }
//    else{

//       range_min =  edge_int_map[ CEdge ].lo;
//    }

//   }
//   else{

//   range_min =  edge_int_map[ CEdge ].lo;

//   }


  range_min =  edge_int_map[ CEdge ].lo;

  return range_min;

   
}

void IntervalRange::interval_const_node( Node *CNode )
{
  
  Edge *edge_out = c_dag->edge_vec[ CNode->out_edges.back() ];

  INTERVAL_TYPE X;

  X.lo = X.hi =  CNode->node_val;

  edge_int_map[ edge_out ] = X;


}

double IntervalRange::get_const_value( Edge *CEdge )
{

  Node *CNode = c_dag->node_vec[ CEdge->get_src_node() ];

  
  //INTERVAL_TYPE X;

  //X.low = X.hi =  CNode->node_val;


  return CNode->node_val;


}


void IntervalRange::interval_shiftl_node( Node *CNode )
{

  Edge *edge_l = c_dag->edge_vec[ CNode->in_edges[0] ];
  Edge *edge_r = c_dag->edge_vec[ CNode->in_edges[1] ];
  Edge *edge_out = c_dag->edge_vec[ CNode->out_edges.back() ];

  
  double X_l, X_r;

  X_l = X_r = 0;

  int ctr;

  Node *node_l = c_dag->node_vec[ edge_l->get_src_node() ];

  Node *node_r = c_dag->node_vec[ edge_r->get_src_node() ];

  INTERVAL_TYPE X;

  X_r = pow( 2. ,  node_r->node_val);

  X.lo = X_r * edge_int_map[ edge_l ].lo;

  X.hi = X_r * edge_int_map[ edge_l ].hi;
    

  edge_int_map[ edge_out ] = X;

  //cout << "lshift " << edge_out->edge_name << "::" << edge_int_map[ edge_out ].hi << endl;
}


void IntervalRange::interval_shiftr_node( Node *CNode )
{

  Edge *edge_l = c_dag->edge_vec[ CNode->in_edges[0] ];
  Edge *edge_r = c_dag->edge_vec[ CNode->in_edges[1] ];
  Edge *edge_out = c_dag->edge_vec[ CNode->out_edges.back() ];
 
  double X_l, X_r;

  X_l = X_r = 0;

  int ctr;

  Node *node_l = c_dag->node_vec[ edge_l->get_src_node() ];

  Node *node_r = c_dag->node_vec[ edge_r->get_src_node() ];

  INTERVAL_TYPE X;

  X_r = pow( 2. ,  -node_r->node_val);

  X.lo = X_r * edge_int_map[ edge_l ].lo;

  X.hi = X_r * edge_int_map[ edge_l ].hi;

  edge_int_map[ edge_out ] = X;

}




void IntervalRange::interval_add_node( Node *CNode )
{
  Edge *edge_l = c_dag->edge_vec[ CNode->in_edges[0] ];
  Edge *edge_r = c_dag->edge_vec[ CNode->in_edges[1] ];
  Edge *edge_out = c_dag->edge_vec[ CNode->out_edges.back() ];

  
  int ctr;

  double X_l, X_r;
  
  X_l = X_r = 0;

  Node *node_l = c_dag->node_vec[ edge_l->get_src_node() ];

  Node *node_r = c_dag->node_vec[ edge_r->get_src_node() ];


  INTERVAL_TYPE X;

 
  if(( node_l->node_type != CONSTANT )&& ( node_r->node_type != CONSTANT )){

        
    X.lo = edge_int_map[ edge_l ].lo + edge_int_map[ edge_r ].lo;

    X.hi = edge_int_map[ edge_l ].hi + edge_int_map[ edge_r ].hi; 

  
  }else if ( node_l->node_type == CONSTANT ){



    X.lo = edge_int_map[ edge_r ].lo + get_const_value( edge_l );

    X.hi = edge_int_map[ edge_r ].hi + get_const_value( edge_l );

    
       

  }else if ( node_r->node_type == CONSTANT ){

  
    X.lo = edge_int_map[ edge_l ].lo + get_const_value( edge_r );

    X.hi = edge_int_map[ edge_l ].hi + get_const_value( edge_r );

  }

  edge_int_map[ edge_out ] = X;

}


void IntervalRange::interval_sub_node( Node *CNode )
{
  Edge *edge_l = c_dag->edge_vec[ CNode->in_edges[0] ];
  Edge *edge_r = c_dag->edge_vec[ CNode->in_edges[1] ];
  Edge *edge_out = c_dag->edge_vec[ CNode->out_edges.back() ];

  
  int ctr;

  double X_l, X_r;
  
  X_l = X_r = 0;


  Node *node_l = c_dag->node_vec[ edge_l->get_src_node() ];

  Node *node_r = c_dag->node_vec[ edge_r->get_src_node() ];

  
  INTERVAL_TYPE X;

  if(( node_l->node_type != CONSTANT )&& ( node_r->node_type != CONSTANT )){

       
    X.lo = edge_int_map[ edge_l ].lo - edge_int_map[ edge_r ].hi;

    X.hi = edge_int_map[ edge_l ].hi - edge_int_map[ edge_r ].lo; 
          
  
     
  }else if ( node_l->node_type == CONSTANT ){
 
    
    X.lo = get_const_value( edge_l ) - edge_int_map[ edge_r ].lo;

    X.hi = get_const_value( edge_l ) - edge_int_map[ edge_r ].hi;

  }else if ( node_r->node_type == CONSTANT ){
   
    X.lo = edge_int_map[ edge_l ].lo - get_const_value( edge_r );

    X.hi = edge_int_map[ edge_l ].hi - get_const_value( edge_r );
    

  }

  edge_int_map[ edge_out ] = X;

}

void IntervalRange::interval_div_node( Node *CNode )
{

  Edge *edge_l = c_dag->edge_vec[ CNode->in_edges[0] ];
  Edge *edge_r = c_dag->edge_vec[ CNode->in_edges[1] ];
  Edge *edge_out = c_dag->edge_vec[ CNode->out_edges.back() ];


  double X_l, X_r;

  X_l = X_r = 0;

  int ctr;

  Node *node_l = c_dag->node_vec[ edge_l->get_src_node() ];

  Node *node_r = c_dag->node_vec[ edge_r->get_src_node() ];

 
  INTERVAL_TYPE X;

  double high_l, low_l, high_r , low_r;

  high_l = ( node_l->node_type != CONSTANT ) ? edge_int_map[ edge_l ].hi : node_l->node_val;

  low_l = ( node_l->node_type != CONSTANT ) ? edge_int_map[ edge_l ].lo : node_l->node_val;

  high_r = ( node_r->node_type != CONSTANT ) ? edge_int_map[ edge_r ].hi : node_r->node_val;

  low_r = ( node_r->node_type != CONSTANT ) ? edge_int_map[ edge_r ].lo : node_r->node_val;


       
  double r_1 = low_l / low_r;

  double r_2 = low_l / high_r;

  double r_3 = high_l / low_r;

  double r_4 = high_l / high_r;


  X.hi = max( max( r_1 , r_2 ) , max( r_3 , r_4 ) );

  X.lo = min( min( r_1 , r_2 ) , min( r_3 , r_4 ) );

    


  edge_int_map[ edge_out ] = X;

}

void IntervalRange::interval_mul_node( Node *CNode )
{


  Edge *edge_l = c_dag->edge_vec[ CNode->in_edges[0] ];
  Edge *edge_r = c_dag->edge_vec[ CNode->in_edges[1] ];
  Edge *edge_out = c_dag->edge_vec[ CNode->out_edges.back() ];


  double X_l, X_r;

  X_l = X_r = 0;

  int ctr;

  Node *node_l = c_dag->node_vec[ edge_l->get_src_node() ];

  Node *node_r = c_dag->node_vec[ edge_r->get_src_node() ];

 
  INTERVAL_TYPE X;

  double high_l, low_l, high_r , low_r;

  high_l = ( node_l->node_type != CONSTANT ) ?  edge_int_map[ edge_l ].hi : node_l->node_val;

  low_l = ( node_l->node_type != CONSTANT ) ? edge_int_map[ edge_l ].lo : node_l->node_val;

  high_r = ( node_r->node_type != CONSTANT ) ?  edge_int_map[ edge_r ].hi : node_r->node_val;

  low_r = ( node_r->node_type != CONSTANT ) ? edge_int_map[ edge_r ].lo : node_r->node_val;


       
  double r_1 = low_l * low_r;

  double r_2 = low_l * high_r;

  double r_3 = high_l * low_r;

  double r_4 = high_l * high_r;


  X.hi = max( max( r_1 , r_2 ) , max( r_3 , r_4 ) );

  X.lo = min( min( r_1 , r_2 ) , min( r_3 , r_4 ) );

    

  edge_int_map[ edge_out ] = X;
}

void IntervalRange::interval_assign_node( Node *CNode )
{

  Edge *in_edge = c_dag->edge_vec[ CNode->in_edges.back()];
  
  Edge *out_edge = c_dag->edge_vec[ CNode->out_edges.back()];
  
  
  INTERVAL_TYPE X_in, X;

  //cout << in_edge->edge_name << " : " << in_edge->e_max  << " : " <<  out_edge->edge_name << endl;

  //out_edge->e_max = in_edge->e_max;
  //out_edge->e_min = in_edge->e_min;
  X_in = edge_int_map[ in_edge ];

  X.hi = X_in.hi;
  X.lo = X_in.lo;


  edge_int_map[ out_edge ] = X;

}


void IntervalRange::interval_input_node( Node *CNode )
{

  Edge *edge_out = c_dag->edge_vec[ CNode->out_edges.back() ];

  cout << "Node :: " << CNode->node_name << endl;

  INTERVAL_TYPE X;

  X.hi = edge_out->e_max;
  X.lo = edge_out->e_min;

  edge_int_map[ edge_out ] = X;
 
}


void IntervalRange::show_interval_vecs()
{
  vector< Edge * >::iterator e_itr;

  vector<double>::iterator a_itr;

  Edge *CEdge;

   cout << "---------------------------------------------" <<endl;

  for( e_itr = c_dag->edge_vec.begin() ; e_itr != c_dag->edge_vec.end() ; e_itr++ )
    {
      
      CEdge = (*e_itr);
           
      cout << CEdge->edge_name << " Max :" << get_range_max( CEdge ) << " Min : " << get_range_min( CEdge ) << endl;
      
      cout << "---------------------------------------------" <<endl;
    }


}
