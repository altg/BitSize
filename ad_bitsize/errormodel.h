// Pass to derive error propagation model

// $Author: amag97 $
// $Id: errormodel.h,v 1.6 2004/12/06 00:46:52 amag97 Exp $
// $Log: errormodel.h,v $
// Revision 1.6  2004/12/06 00:46:52  amag97
// Added simulation based range analysis
// Added multi-prec fixed-point prec. analysis ver 2 ( Beta )
//
// Revision 1.5  2004/11/12 19:05:24  amag97
// More fixes to affine erromodel
//
// Revision 1.4  2004/11/08 18:36:28  amag97
// Added affine arithmetic based range analysis
// Bug fixes
//
// Revision 1.3  2004/11/06 19:38:37  amag97
// More changes to affine analysis
//
// Revision 1.2  2004/11/05 18:50:04  amag97
// More fixes to error function generation
//
// Revision 1.1  2004/10/31 19:53:10  amag97
// Added node schedular
// Added error propagation function generator
// Integeration of these with BitSize
//

#ifndef _ERRORMODEL_H
#define _ERRORMODEL_H

#include <sstream>

#include "dag.h"
class Node;
class Edge;


#include <ostream>
using namespace std;


class ErrorModel{


public:
  ErrorModel(DAG * , NodeSchedular * , RangeAnalysis * );


  DAG *c_dag;


  void gen_error_exp(ostream &os);

  NodeSchedular *node_sched;
  
  RangeAnalysis *range_analysis;

  
  void gen_node_error_exp( ostream &os , dag_index node_num );
  void gen_node_cost_func( ostream &os );
  void gen_parameters_func( ostream &os );

  


  void gen_input_node_exp(  ostream &os , Node* CNode );

  void gen_add_node_exp(  ostream &os , Node* CNode );
  void gen_sub_node_exp(  ostream &os , Node* CNode );
  void gen_mul_node_exp(  ostream &os , Node* CNode );
  void gen_div_node_exp(  ostream &os , Node* CNode );
  void gen_const_node_exp( ostream &os , Node* CNode );


  double get_const_value( Edge *CEdge );

  void gen_matlab_errfiles();

  void gen_output_vector( ostream &os );
  

  string rounding;

  vector<string> expr_names;
  vector<string> expr_names_cost;

  vector<string> output_names;
  vector<string> output_node_names;
  

  map<string, int> input_arg_names;

  string propagated_error( Edge *CEdge );

  stringstream c_ss;

  void gen_signal_bwname( stringstream &ss , HWvar* _var);

  void gen_signal_err_expr( stringstream &out_ss , stringstream &in_ss );
  
  void gen_signal_bwname( stringstream &ss , Node* _node);


 
};
#endif
