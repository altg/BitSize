// Pass to derive schedule node

// $Author: amag97 $
// $Id: node_sched.h,v 1.3 2004/11/26 18:20:41 amag97 Exp $
// $Log: node_sched.h,v $
// Revision 1.3  2004/11/26 18:20:41  amag97
// Data flow graph based automatic differentiation
//
// Revision 1.2  2004/11/05 18:50:04  amag97
// More fixes to error function generation
//
// Revision 1.1  2004/10/31 19:53:10  amag97
// Added node schedular
// Added error propagation function generator
// Integeration of these with BitSize
//

#ifndef _NODE_SCHED_H
#define _NODE_SCHED_H

class Node;

#include "bitsize_defs.h"


class NodeSchedular{

public:
  
  typedef vector<dag_index> node_vec_type;
  
  NodeSchedular( DAG* _dag );


  DAG *c_dag;

  map< dag_index , Node* > unsheduled_nodes;
  map< dag_index , bool > node_scheduled;
  map< dag_index , int >  node_sched_no;
  
  int scheduled_nodes;

  bool graph_scheduled;

  bool nodes_sorted;

  void schedule_graph();
  int schedule_innodes( dag_index c_node_num );
  void sort_schedule();

  vector< node_vec_type > schedule;

};



#endif
