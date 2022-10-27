// $Author: amag97 $
// $Id: bw_results_gen.h,v 1.1 2004/12/13 01:33:44 amag97 Exp $


// Class to generate results

#ifndef _BW_RESULTS_GEN_H
#define _BW_RESULTS_GEN_H

#include "dag.h"

class BWResultsGen{


public:

  BWResultsGen( DAG *_dag );

  
  void gen_bitsize_dat( ostream &os );

  //void gen_bitsize_dat_range( ostream &os );


  void gen_float_data( Edge *CEdge , ostream &os );

  void gen_fixed_data( Edge *CEdge , ostream &os );

  void gen_lns_data( Edge *CEdge , ostream &os );

  DAG *c_dag;

  int index;

  //Edge* get_assigned_to_node( Edge *CEdge );

};

#endif
