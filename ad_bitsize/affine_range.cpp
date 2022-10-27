#undef _BITSIZE_DEBUG_

#include "range_analysis.h"

#include<iomanip>

AffineRange::AffineRange(DAG *_dag , NodeSchedular *_node_sched ):RangeAnalysis( _dag , _node_sched ),affine_index(1),affine_range_analysis_done(false){}


void AffineRange::operator()()
{

  do_affine_analysis();

  //show_affine_vecs();

  update_bw();

  show_range_vecs();

  //if ( bs->db_level > 0  ) show_affine_vecs();

}

void AffineRange::do_affine_analysis()
{
  
  if ( affine_range_analysis_done == true ) return;

  node_sched->sort_schedule();

  cout << "Doing Affine range analysis" << endl;

  vector< NodeSchedular::node_vec_type >::iterator sch_itr; 
  vector< dag_index >::iterator n_itr2;

  int ctr = 0;
  for ( sch_itr = node_sched->schedule.begin() ; sch_itr != node_sched->schedule.end() ;  sch_itr++ )
    {

      _DBOUT( "Schedule no " << ctr++ );

      for( n_itr2 = (*sch_itr).begin() ; n_itr2 != (*sch_itr).end() ; n_itr2++ )
	{

	    do_affine_analysis_node((*n_itr2));

	}  


    }

  affine_range_analysis_done = true;

}

void AffineRange::do_affine_analysis_node(dag_index node_num)
{
 
  Node* CNode = c_dag->node_vec[ node_num ];

  switch( CNode->node_type )
    {

    case CONSTANT: affine_const_node( CNode ); break;

    case INPUT: affine_input_node( CNode ); break;  

    case ADD: affine_add_node( CNode ); break;

    case SUB: affine_sub_node( CNode ); break;

    case MUL: affine_mul_node( CNode ); break;

    case DIV: affine_div_node( CNode ); break;

    case SHIFTL: affine_shiftl_node( CNode ); break;

    case SHIFTR: affine_shiftr_node( CNode ); break;

    case ASSIGN:  affine_assign_node( CNode ); break;

    defualt:;

    }

}



double AffineRange::get_range_max( Edge *CEdge)
{

  double range_max;
  HWfix* t_var;
  if ( t_var = dynamic_cast<HWfix *>( CEdge->my_var ) ) {
    //   if ( t_var->int_bw_state == SET )
    //       {

    // 	if (  t_var->c_sign  == TWOSCOMPLEMENT ){
    // 	  range_max = pow( 2. , (double)t_var->int_bw - 1 ) - 1;
    // 	}
    // 	else if (  t_var->c_sign  == UNSIGNED ){
    // 	  range_max = pow( 2. , (double)t_var->int_bw ) - 1;	
    // 	}

    //       }
    //     else{

    //range_max =  CEdge->X0 + get_abs_eterms( CEdge );
    range_max =  CEdge->X0 + get_sum_eterms( CEdge );

    //}
  }
  else{

    //range_max =  CEdge->X0 + get_abs_eterms( CEdge );
    range_max =  CEdge->X0 + get_sum_eterms( CEdge );


  }

  //cout << "RANGE MAX " << range_max << endl;
  return range_max;
}


double AffineRange::get_range_min( Edge *CEdge)
{
  double range_min = 0;


  //   if ( c_dag->arith_type == FIXED ){
 
  //     if ( dynamic_cast<HWfix *>(CEdge->my_var)->c_sign == UNSIGNED )
  //       {

  // 	range_min = 0;

  //       }
  //     else 
  //       {


  // 	range_min = CEdge->X0 - get_abs_eterms( CEdge );

  //       }

  //   }
  //   else  

  HWfix* t_var;


  if ( t_var = dynamic_cast<HWfix *>( CEdge->my_var ) ) {
    if (  t_var->int_bw_state == SET )
      {

	range_min = 0;

      }
    else{

      range_min = CEdge->X0 - get_abs_eterms( CEdge );

      if ( CEdge->affine_vec.size() > 0 )
	if ( ( fabs(range_min) - 2* CEdge->affine_vec.back() ) == 0. )
	  range_min = 0;


    }
  }
  else{

    range_min = CEdge->X0 - get_abs_eterms( CEdge );

    if ( CEdge->affine_vec.size() > 0 )
      if ( ( fabs(range_min) - 2*CEdge->affine_vec.back() ) == 0. )
	range_min = 0;


  }

  return range_min;

   
}

void AffineRange::affine_const_node( Node *CNode )
{
  
  Edge *edge_out = c_dag->edge_vec[ CNode->out_edges.back() ];

  edge_out->X0 =  CNode->node_val;
}

double AffineRange::get_const_value( Edge *CEdge )
{

  Node *CNode = c_dag->node_vec[ CEdge->get_src_node() ];

  CEdge->X0 = CNode->node_val;

  return CNode->node_val;


}


void AffineRange::affine_shiftl_node( Node *CNode )
{

   Edge *edge_l = c_dag->edge_vec[ CNode->in_edges[0] ];
  Edge *edge_r = c_dag->edge_vec[ CNode->in_edges[1] ];
  Edge *edge_out = c_dag->edge_vec[ CNode->out_edges.back() ];


  edge_out->affine_vec.resize( affine_index );
  edge_out->aff_ndx = affine_index++;

  double X_l, X_r;

  X_l = X_r = 0;

  int ctr;

  Node *node_l = c_dag->node_vec[ edge_l->get_src_node() ];

  Node *node_r = c_dag->node_vec[ edge_r->get_src_node() ];

  //if(( edge_l->edge_type != ECONSTANT )&& ( edge_r->edge_type != ECONSTANT )){
 
  
    //X_r = get_const_value( edge_r );

  X_r = pow( 2. ,  node_r->node_val);

    edge_out->X0 = edge_l->X0 * X_r;

    
    for( ctr =  0 ; ctr < edge_out->affine_vec.size() ; ctr++ )
      {
	
	X_l = ( ctr < edge_l->affine_vec.size() ) ? edge_l->affine_vec[ ctr ] : 0;

	edge_out->affine_vec[ ctr ] = X_r * X_l;
      }
       

 
    //cout << edge_out->edge_name << " Max :" << get_range_max( edge_out ) << " Min : " << get_range_min( edge_out ) << endl;


}


void AffineRange::affine_shiftr_node( Node *CNode )
{

   Edge *edge_l = c_dag->edge_vec[ CNode->in_edges[0] ];
  Edge *edge_r = c_dag->edge_vec[ CNode->in_edges[1] ];
  Edge *edge_out = c_dag->edge_vec[ CNode->out_edges.back() ];


  edge_out->affine_vec.resize( affine_index );
  edge_out->aff_ndx = affine_index++;

  double X_l, X_r;

  X_l = X_r = 0;

  int ctr;

  Node *node_l = c_dag->node_vec[ edge_l->get_src_node() ];

  Node *node_r = c_dag->node_vec[ edge_r->get_src_node() ];

  //if(( edge_l->edge_type != ECONSTANT )&& ( edge_r->edge_type != ECONSTANT )){
 
  
    //X_r = get_const_value( edge_r );

  X_r = pow( 2. ,  -node_r->node_val);

    edge_out->X0 = edge_l->X0 * X_r;

    
    for( ctr =  0 ; ctr < edge_out->affine_vec.size() ; ctr++ )
      {
	
	X_l = ( ctr < edge_l->affine_vec.size() ) ? edge_l->affine_vec[ ctr ] : 0;

	edge_out->affine_vec[ ctr ] = X_r * X_l;
      }
       

 
    //cout << edge_out->edge_name << " Max :" << get_range_max( edge_out ) << " Min : " << get_range_min( edge_out ) << endl;


}




void AffineRange::affine_add_node( Node *CNode )
{
  Edge *edge_l = c_dag->edge_vec[ CNode->in_edges[0] ];
  Edge *edge_r = c_dag->edge_vec[ CNode->in_edges[1] ];
  Edge *edge_out = c_dag->edge_vec[ CNode->out_edges.back() ];

  
  edge_out->affine_vec.resize( affine_index );
  edge_out->aff_ndx = affine_index++;


  int ctr;

  double X_l, X_r;
  
  X_l = X_r = 0;

  Node *node_l = c_dag->node_vec[ edge_l->get_src_node() ];

  Node *node_r = c_dag->node_vec[ edge_r->get_src_node() ];


  if(( node_l->node_type != CONSTANT )&& ( node_r->node_type != CONSTANT )){

    edge_out->X0 = edge_l->X0 + edge_r->X0;
    
           
    for( ctr =  0 ; ctr < edge_out->affine_vec.size() ; ctr++ )
      {
	
	X_l = ( ctr < edge_l->affine_vec.size() ) ? edge_l->affine_vec[ ctr ] : 0;

	X_r = ( ctr < edge_r->affine_vec.size() ) ? edge_r->affine_vec[ ctr ] : 0;

	edge_out->affine_vec[ ctr ] = X_l + X_r;
      }

    //edge_out->affine_vec[ edge_out->aff_ndx - 1 ]= 222;

     

  }else if ( node_l->node_type == CONSTANT ){


    edge_out->X0 = edge_r->X0 + get_const_value( edge_l );

    
    for( ctr =  0 ; ctr < edge_out->affine_vec.size() ; ctr++ )
      {
	
	X_r = ( ctr < edge_r->affine_vec.size() ) ? edge_r->affine_vec[ ctr ] : 0;

	edge_out->affine_vec[ ctr ] = X_r;
      }

    

  }else if ( node_r->node_type == CONSTANT ){

    edge_out->X0 = edge_l->X0 + get_const_value( edge_r );


    for( ctr =  0 ; ctr < edge_out->affine_vec.size() ; ctr++ )
      {
	
	X_l = ( ctr < edge_l->affine_vec.size() ) ? edge_l->affine_vec[ ctr ] : 0;

	edge_out->affine_vec[ ctr ] = X_l;
      }

       

  }

  

}


void AffineRange::affine_sub_node( Node *CNode )
{
  Edge *edge_l = c_dag->edge_vec[ CNode->in_edges[0] ];
  Edge *edge_r = c_dag->edge_vec[ CNode->in_edges[1] ];
  Edge *edge_out = c_dag->edge_vec[ CNode->out_edges.back() ];

  
  edge_out->affine_vec.resize( affine_index );
  edge_out->aff_ndx = affine_index++;


  int ctr;

  double X_l, X_r;
  
  X_l = X_r = 0;


  Node *node_l = c_dag->node_vec[ edge_l->get_src_node() ];

  Node *node_r = c_dag->node_vec[ edge_r->get_src_node() ];

  

  if(( node_l->node_type != CONSTANT )&& ( node_r->node_type != CONSTANT )){

    edge_out->X0 = edge_l->X0 - edge_r->X0;
    
           
    for( ctr =  0 ; ctr < edge_out->affine_vec.size() ; ctr++ )
      {
	
	X_l = ( ctr < edge_l->affine_vec.size() ) ? edge_l->affine_vec[ ctr ] : 0;

	X_r = ( ctr < edge_r->affine_vec.size() ) ? edge_r->affine_vec[ ctr ] : 0;

	edge_out->affine_vec[ ctr ] = X_l - X_r;
      }


     
  }else if ( node_l->node_type == CONSTANT ){
 


    edge_out->X0 =  get_const_value( edge_l ) - edge_r->X0;

    
    for( ctr =  0 ; ctr < edge_out->affine_vec.size() ; ctr++ )
      {
	
	X_r = ( ctr < edge_r->affine_vec.size() ) ? edge_r->affine_vec[ ctr ] : 0;

	edge_out->affine_vec[ ctr ] = -X_r;
      }

    

  }else if ( node_r->node_type == CONSTANT ){

 

    edge_out->X0 = edge_l->X0 - get_const_value( edge_r );


    for( ctr =  0 ; ctr < edge_out->affine_vec.size() ; ctr++ )
      {
	
	X_l = ( ctr < edge_l->affine_vec.size() ) ? edge_l->affine_vec[ ctr ] : 0;

	edge_out->affine_vec[ ctr ] = X_l;
      }

       

  }

  

}



double AffineRange::get_abs_eterms( Edge *CEdge )
{

  double affine_sum  = 0;
  //if( CEdge->edge_type != ECONSTANT ){

    vector<double>::iterator a_itr = CEdge->affine_vec.begin();

   
    for(; a_itr != CEdge->affine_vec.end(); a_itr++ )
      {

	affine_sum += fabs(*a_itr);

      }
     
    //}


  return affine_sum;


}


double AffineRange::get_sum_eterms( Edge *CEdge )
{

  double affine_sum  = 0;
  //if( CEdge->edge_type != ECONSTANT ){

    vector<double>::iterator a_itr = CEdge->affine_vec.begin();

   
    for(; a_itr != CEdge->affine_vec.end(); a_itr++ )
      {

	affine_sum += *a_itr;

      }
     
    //}


  return affine_sum;


}

void AffineRange::affine_div_node( Node *CNode )
{
  Edge *edge_l = c_dag->edge_vec[ CNode->in_edges[0] ];
  Edge *edge_r = c_dag->edge_vec[ CNode->in_edges[1] ];
  Edge *edge_out = c_dag->edge_vec[ CNode->out_edges.back() ];


  edge_out->affine_vec.resize( affine_index );
  edge_out->aff_ndx = affine_index++;

  double X_l, X_r;

  X_l = X_r = 0;

  
  Node *node_l = c_dag->node_vec[ edge_l->get_src_node() ];

  Node *node_r = c_dag->node_vec[ edge_r->get_src_node() ];

  edge_out->X0 = edge_l->X0 / edge_r->X0;

  //double l_val = (node_l->node_type != CONSTANT )? edge_l->val :  node_l->node_val;
  //double r_val = (node_r->node_type != CONSTANT )? edge_r->val :  node_r->node_val;

  double l_hi = get_range_max( edge_l );
  double l_lo = get_range_min( edge_l );

  double r_hi = get_range_max( edge_r );
  double r_lo = get_range_min( edge_r );
  


  for( int ctr =  0 ; ctr < edge_out->affine_vec.size() ; ctr++ )
      {
	
	     
	X_l = ( ctr < edge_l->affine_vec.size() ) ? edge_l->affine_vec[ ctr ] : 0;

	X_r = ( ctr < edge_r->affine_vec.size() ) ? edge_r->affine_vec[ ctr ] : 0;

	//edge_out->affine_vec[ ctr ] = (edge_r->X0 * X_l - edge_l->X0 * X_r)/( edge_r->X0 * edge_r->X0);

	edge_out->affine_vec[ ctr ] = (X_l /edge_r->X0)  - (edge_l->X0 * X_r)/( edge_r->X0 * edge_r->X0);

	//edge_out->affine_vec[ ctr ] = (X_l /r_lo)  - (l_lo * X_r)/(r_hi * r_hi);

      }
  

    double d_max = get_range_max( edge_out ) - l_hi / r_lo;

    double d_min = get_range_min( edge_out ) - l_lo / r_hi;

    double d_mid = fabs(d_max - d_min)/2;

    edge_out->affine_vec[ edge_out->aff_ndx - 1 ] = d_mid;

    edge_out->X0 += max( d_mid - d_min , d_max - d_mid );

}


void AffineRange::affine_mul_node( Node *CNode )
{


  Edge *edge_l = c_dag->edge_vec[ CNode->in_edges[0] ];
  Edge *edge_r = c_dag->edge_vec[ CNode->in_edges[1] ];
  Edge *edge_out = c_dag->edge_vec[ CNode->out_edges.back() ];


  edge_out->affine_vec.resize( affine_index );
  edge_out->aff_ndx = affine_index++;

  double X_l, X_r;

  X_l = X_r = 0;

  int ctr;

  Node *node_l = c_dag->node_vec[ edge_l->get_src_node() ];

  Node *node_r = c_dag->node_vec[ edge_r->get_src_node() ];

  //if(( edge_l->edge_type != ECONSTANT )&& ( edge_r->edge_type != ECONSTANT )){
  if(( node_l->node_type != CONSTANT ) && ( node_r->node_type != CONSTANT ) ){

    edge_out->X0 = edge_l->X0 * edge_r->X0;

    for( ctr =  0 ; ctr < edge_out->affine_vec.size() ; ctr++ )
      {
	
	     
	X_l = ( ctr < edge_l->affine_vec.size() ) ? edge_l->affine_vec[ ctr ] : 0;

	X_r = ( ctr < edge_r->affine_vec.size() ) ? edge_r->affine_vec[ ctr ] : 0;

	edge_out->affine_vec[ ctr ] = edge_r->X0 * X_l + edge_l->X0 * X_r;
      }


    double u = get_abs_eterms( edge_l );

    double v = get_abs_eterms( edge_r );

    edge_out->affine_vec[ edge_out->aff_ndx - 1 ] = u*v;


  }else if ( node_l->node_type == CONSTANT ){

    //X_l = get_const_value( edge_l );

    X_l = node_l->node_val;

    

    edge_out->X0 = edge_r->X0 * X_l;
    
    
    for( ctr =  0 ; ctr < edge_out->affine_vec.size() ; ctr++ )
      {
	
	X_r = ( ctr < edge_r->affine_vec.size() ) ? edge_r->affine_vec[ ctr ] : 0;

	//cout << "Tot " << X_r * X_l << endl;

	edge_out->affine_vec[ ctr ] = X_r * X_l;
      }

    

  }else if ( node_r->node_type == CONSTANT ){

   
    //X_r = get_const_value( edge_r );

    X_r = node_r->node_val;

    edge_out->X0 = edge_l->X0 * X_r;

    
    for( ctr =  0 ; ctr < edge_out->affine_vec.size() ; ctr++ )
      {
	
	X_l = ( ctr < edge_l->affine_vec.size() ) ? edge_l->affine_vec[ ctr ] : 0;

	edge_out->affine_vec[ ctr ] = X_r * X_l;
      }
       

  }

  //cout << edge_out->edge_name << " Max :" << get_range_max( edge_out ) << " Min : " << get_range_min( edge_out ) << endl;
}


void AffineRange::affine_assign_node( Node *CNode )
{

  Edge *in_edge = c_dag->edge_vec[ CNode->in_edges.back()];
  
  Edge *out_edge = c_dag->edge_vec[ CNode->out_edges.back()];
  
  out_edge->X0 = in_edge->X0;

  out_edge->Xk = in_edge->Xk;

  out_edge->affine_vec.assign( in_edge->affine_vec.begin() , in_edge->affine_vec.end() );



}

void AffineRange::affine_input_node( Node *CNode )
{

  //vector<dag_index>::iterator e_itr;

  //for( e_itr = CNode->out_edges.begin() ; e_itr != CNode->out_edges.end(); e_itr++ )
  //{
      Edge *edge_out = c_dag->edge_vec[ CNode->out_edges.back() ];
      //Edge *edge_out = c_dag->edge_vec[ *(e_itr) ];

      //cout << "Node :: " << CNode->node_name << endl;

      //edge_out->e_max = 10;
      //edge_out->e_min = -10;

      edge_out->X0 = (edge_out->e_max + edge_out->e_min)/2;
      edge_out->Xk = (edge_out->e_max - edge_out->e_min)/2;
    
      edge_out->affine_vec.resize( affine_index );
      edge_out->aff_ndx = affine_index++; 

      edge_out->affine_vec[ edge_out->aff_ndx - 1 ]= edge_out->Xk;
 
      //}
}


void AffineRange::show_affine_vecs()
{
  vector< Edge * >::iterator e_itr;

  vector<double>::iterator a_itr;

  Edge *CEdge;

   cout << "---------------------------------------------" <<endl;

  for( e_itr = c_dag->edge_vec.begin() ; e_itr != c_dag->edge_vec.end() ; e_itr++ )
    {
      
      CEdge = (*e_itr);
      
    //  cout << CEdge->X0 << " : ";
//      for( a_itr = CEdge->affine_vec.begin() ; a_itr !=  CEdge->affine_vec.end() ; a_itr++ )
// 	{
      
// 	  cout << (*a_itr) << " : ";


// 	} 

//       cout << endl;

      cout << CEdge->edge_name << " Max :";
      cout << setprecision(10) << get_range_max( CEdge );
      cout << " Min : ";
      cout << setprecision(10) << get_range_min( CEdge ) << endl;
      
      cout << "---------------------------------------------" <<endl;
    }


}
