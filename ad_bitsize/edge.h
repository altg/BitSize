#ifndef _EDGE_H_
#define _EDGE_H_

//#include "bitsize_defs.h"
#include "hwvar.h"
#include "dag.h"

#include "datalog.h"
//enum Edge_TYPE {IN , OUT , TMP };

//typedef Edge_TYPE arch_attr;


//#define Edge_TYPE arch_attr

class Edge{

public:
  
  Edge( Edge_TYPE e_type , string name , dag_index e_num );
  Edge( ){};

  ~Edge();

  dag_index edge_number;
  string edge_name;
  Edge_TYPE edge_type;

  
  vector<dag_index> src_nodes;
  vector<dag_index> dst_nodes;
  

  HWvar* my_var;

  dag_index get_src_node();
  dag_index get_dst_node();


  Edge* next_edge;

  vector<double> grad_vec;


  double f_bw;

  // Affine arithmetic stuff

  double e_min, e_max;


  double X0;

  double Xk;

  vector<double> affine_vec;

  int aff_ndx;

  // Affine Stuff
  int af_ndx_err;
  vector<string> af_err_vec;
  
  // Affine2 Stuff
  int af_ndx_err2;
  vector<double> af_err_vec2;


  vector<string> ErrorNames_vec;
  vector<double> TransFuncVals_vec;
  

  // Automatic differentiation stuff
  int ad_ndx;
  vector<double> ad_vec;
  vector<double> max_ad_vec;

  double val;
  double max_val;
  double min_val;


  int flt_man_bw;

  int fx_frac_bw;
  
  void copy( Edge *CEdge );

  // pwrdate stuff

  DataLog data_log;

};




#endif
