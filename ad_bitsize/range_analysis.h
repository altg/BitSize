// $Author: amag97 $
// $Id: range_analysis.h,v 1.4 2005/02/23 20:32:44 amag97 Exp $

#ifndef _RANGE_AFFINE_H
#define _RANGE_AFFINE_H

#include "dag.h"
class Node;
class Edge;


class RangeAnalysis{

public:
  RangeAnalysis( DAG * _dag , NodeSchedular *_sched ); 

  DAG* c_dag;
  NodeSchedular *node_sched;

  virtual void operator()(){}

  virtual double get_range_max( Edge *CEdge){}

  virtual double get_range_min( Edge *CEdge){}


  void update_bw();
  void update_bw_fx();
  void update_bw_flt();
  void update_bw_lns();


  double range_bw_flt( Edge *CEdge );
  double range_bw_lns( Edge *CEdge );
  double range_bw_fx( Edge *CEdge );

  
  void show_range_vecs();


  
};


class AffineRange:public RangeAnalysis{

  
public:
  AffineRange( DAG * , NodeSchedular * );

  void operator()();

  void affine_input_node( Node *CNode );
  void affine_add_node( Node *CNode );
  void affine_sub_node( Node *CNode );
  void affine_mul_node( Node *CNode );
  void affine_shiftl_node( Node *CNode );
  void affine_shiftr_node( Node *CNode );
  void affine_const_node( Node *CNode );
  void affine_div_node( Node *CNode );

  void affine_assign_node( Node *CNode );


  double get_const_value( Edge *CEdge );

  double get_abs_eterms( Edge *CEdge );

  double get_sum_eterms( Edge *CEdge );

  void show_affine_vecs();

  void do_affine_analysis_node(dag_index node_num);
  
  void do_affine_analysis();

  double get_range_max( Edge *CEdge);

  double get_range_min( Edge *CEdge);

  int affine_index;

  bool affine_range_analysis_done;

};


class IntervalRange:public RangeAnalysis{

  
public:
  struct INTERVAL_TYPE{
    double hi;
    double lo;
  };


  IntervalRange( DAG * , NodeSchedular * );

  void operator()();

  void interval_input_node( Node *CNode );
  void interval_add_node( Node *CNode );
  void interval_sub_node( Node *CNode );
  void interval_mul_node( Node *CNode );
  void interval_shiftl_node( Node *CNode );
  void interval_shiftr_node( Node *CNode );
  void interval_const_node( Node *CNode );
  void interval_div_node( Node *CNode );

  void interval_assign_node( Node *CNode );

  double get_const_value( Edge *CEdge );

  void show_interval_vecs();

  void do_interval_analysis_node(dag_index node_num);
  
  void do_interval_analysis();

  double get_range_max( Edge *CEdge);

  double get_range_min( Edge *CEdge);

  int interval_index;

  bool interval_range_analysis_done;

  map<Edge *, INTERVAL_TYPE > edge_int_map;

};


class SimRange:public RangeAnalysis{
public:
  SimRange( DAG * , NodeSchedular * );

  double get_range_max( Edge *CEdge);

  double get_range_min( Edge *CEdge);

  void operator()();

  void show_range_vecs();

};




#endif
