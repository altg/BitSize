// $Author: amag97 $
// $Id: node.h,v 1.13 2004/12/13 01:33:43 amag97 Exp $
// $Log: node.h,v $
// Revision 1.13  2004/12/13 01:33:43  amag97
// Added command line argument parsing
// Added Interval based range analysis
// Added ASSIGN node to dag
//
// Revision 1.12  2004/12/08 00:21:08  amag97
// Added non uniform analysis for floating point
//
// Revision 1.11  2004/12/07 00:20:19  amag97
// Fixed simulated annealing based optimisation v2 for fixed point
//
// Revision 1.10  2004/11/28 23:29:02  amag97
// Added back support for HWfloat
// Uniform bit-width optimisation for HWfloat
//
// Revision 1.9  2004/10/29 17:42:26  amag97
// A few more fixes
//
// Revision 1.8  2004/09/30 20:32:52  amag97
// Added Area model
// Lots of fixes to get it to work
//
// Revision 1.7  2004/09/17 16:52:13  amag97
// Added area estimation code
//
// Revision 1.6  2004/07/06 19:48:12  amag97
// Revised DAG generation
//
// Revision 1.5  2004/06/05 17:34:18  amag97
// More bug fixes to ASC integration
//
// Revision 1.4  2004/06/03 22:01:31  amag97
// More bug fixes
// Added HWint
//
// Revision 1.3  2004/06/02 19:37:59  amag97
// Added HWvector
//
// Revision 1.2  2004/06/01 11:31:55  amag97
// DAG code
//
// Revision 1.1  2004/05/28 20:14:27  amag97
// ASC Integration
//

#ifndef _NODE_H
#define _NODE_H

#include<vector>
#include<string>

enum Node_OPTYPE { INPUT , OUTPUT , ADD , SUB , MUL , DIV , MOD , SHIFTL , SHIFTR, CONSTANT , HWINT , START , END, ASSIGN };
enum Node_Arithmetic_TYPE { INTEGER , FIXED , FLOAT , LNS , AT_NOTSET };


#include "bitsize_dbg.h"

#include "varnames.h"
#include "dag.h"

#include "hwvar.h"
#include "hwfix.h"


using namespace std;


//class DAG;
//class HWfix;
class Edge;

class Node{


public:


  Node ( Node_OPTYPE n_type , Node_Arithmetic_TYPE n_atype, string name , dag_index n_num , double n_val = 0.0);
  //Node() {};
  ~Node();
  
  

  int node_number;
  string node_name;
  Node_OPTYPE node_type; 

  Node_Arithmetic_TYPE node_atype;

  double get_area_usage();
  double get_fixed_area_usage( dag_index l , dag_index r );
  double get_float_area_usage( dag_index l_n , dag_index r_n );

  double get_fixed_area_usage( dag_index l_n , dag_index r_n , dag_index  out_n );
   double get_float_area_usage( dag_index l_n , dag_index r_n , dag_index  out_n );

  vector<dag_index> in_edges;
  vector<dag_index> out_edges;


  // For constant nodes 
  double node_val;

 



};







#endif
