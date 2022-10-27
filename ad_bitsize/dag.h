// $Author: amag97 $
// $Id: dag.h,v 1.16 2006/02/14 19:07:54 amag97 Exp $
// $Log: dag.h,v $
// Revision 1.16  2006/02/14 19:07:54  amag97
// Added new Pass for generating glitch based power cost function
// Bug Fixes - conv. between range and bitwidth
//
// Revision 1.15  2004/12/13 01:33:43  amag97
// Added command line argument parsing
// Added Interval based range analysis
// Added ASSIGN node to dag
//
// Revision 1.14  2004/11/28 23:29:02  amag97
// Added back support for HWfloat
// Uniform bit-width optimisation for HWfloat
//
// Revision 1.13  2004/10/23 10:37:35  amag97
// Addition of new fraction bw evaluation method
// (work in progress)
//
// Revision 1.12  2004/10/05 19:23:56  amag97
// Fixes to AD code - bug when multiple inputs are used
// Updated Graph info
//
// Revision 1.11  2004/09/30 20:32:51  amag97
// Added Area model
// Lots of fixes to get it to work
//
// Revision 1.10  2004/09/17 16:52:13  amag97
// Added area estimation code
//
// Revision 1.9  2004/08/31 19:10:06  amag97
// Fixes to bwopt method 3
//
// Revision 1.8  2004/08/26 18:00:52  amag97
// Update after return from Sri Lanka
// Started work on adding user constraints
//
// Revision 1.7  2004/07/09 10:22:21  amag97
// More fixes to DAG code
//
// Revision 1.6  2004/07/06 19:48:11  amag97
// Revised DAG generation
//
// Revision 1.5  2004/06/03 22:01:30  amag97
// More bug fixes
// Added HWint
//
// Revision 1.4  2004/06/02 19:37:59  amag97
// Added HWvector
//
// Revision 1.3  2004/06/01 19:31:17  amag97
// More fixes to DAG code
//
// Revision 1.2  2004/06/01 11:31:55  amag97
// DAG code
//
// Revision 1.1  2004/05/28 20:14:27  amag97
// ASC Integration
//


#ifndef _DAG_H
#define _DAG_H

#include <map>
//#include <multimap>
#include <string>
#include <iostream>
#include <fstream>
using namespace std;

typedef unsigned int dag_index;
typedef dag_index DAG_NODE;
typedef dag_index DAG_EDGE;
#include "edge.h" 
#include "node.h"


#include "hwvar.h"
#include "hwfix.h"
#include "varnames.h"


class Edge;
class Node;
class HWvar;

#include "bitsize_defs.h"


//enum OPMODE_TYPE { PARSING , PARSED };


class DAG{



public:
  DAG();

  Node* new_node( Node_OPTYPE n_type , Node_Arithmetic_TYPE n_atype, string name = "" );
  Node* new_const_node( Node_Arithmetic_TYPE n_atype, double c_val , string name = "" );
  Node* new_hwint_node( string name = "" );
  Node* add_op_node( Node_OPTYPE n_type , Node_Arithmetic_TYPE n_atype , Edge* l , Edge* r );
  
  Node* new_assign_node( Edge *in_edge , Edge *out_edge );

  // Create a new un assigned edge
  Edge* new_edge(HWvar *var = NULL , string name = "");

  // Create a new edge from src node to dst node
  Edge* new_edge( Node* src , Node* dst ,  string name = ""  );
 
  Edge* new_const_edge( double val , string name = "" );
  Edge* new_hwint_edge( string name = "" );

  Node* get_src_node( Edge *in_edge );

  Edge* add_out_edge( Node* node , Edge* edge_n );
  Edge* add_in_edge( Node* node , Edge* edge_n );

  void print_dag();

  void print_dot_graph( ostream& os );
  void print_dot_graph2( ostream& os );

  vector<Node *> node_vec;
  vector<Edge *> edge_vec;

  inline Edge* get_edge( dag_index di ){ return edge_vec[ di ]; }
 
  dag_index node_count;
  dag_index edge_count;
  
  map<Node_OPTYPE,string> Node_OPNAME;
  map<Edge_TYPE,string> Edge_TYPENAME;
  
  Node* start_node;
  Node* end_node;


  multimap< Node_OPTYPE,Node* > Node_opmap;
 

  // Sorted nodes and node lists for quick access
  //list<Node *> input_node_list, add_node_list, output_node_list;

  vector<dag_index> input_edges, add_edges, sub_edges, mul_edges, div_edges;
  
  vector<dag_index> output_edges;

  vector<dag_index> edges_to_analyse;

  void sort_edges();
  void add_to_list( Node_OPTYPE node_type , vector<dag_index> &edge_list );

  
  void update_edge_grads();

  void show_edge_grads();

  
  void list_edges(dag_index start);

  //void propagate_error( dag_index start  , double m_err);

  
  Node_Arithmetic_TYPE arith_type;

};





#endif
