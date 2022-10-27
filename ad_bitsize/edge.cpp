#include "edge.h"



Edge::Edge( Edge_TYPE e_type , string name , dag_index e_num )
{

  edge_name = name;
  edge_type = e_type;

  edge_number = e_num;
 

  ad_vec.resize( 0 );

  f_bw = 0;

  flt_man_bw = 40;

  max_val = -DBL_MAX;

  min_val = DBL_MAX;

  next_edge = NULL;


}





Edge::~Edge()
{

}


void Edge::copy( Edge * CEdge )
{

  e_min = CEdge->e_min;

  e_max = CEdge->e_max;


}

dag_index Edge::get_src_node()
{

  return src_nodes.back();

}
 

dag_index Edge::get_dst_node()
{

  return dst_nodes.back();


}

