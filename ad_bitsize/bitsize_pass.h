// BitSize Pass base class

// $Author: amag97 $
// $Id: bitsize_pass.h,v 1.22 2006/04/28 17:11:49 amag97 Exp $
// $Log: bitsize_pass.h,v $
// Revision 1.22  2006/04/28 17:11:49  amag97
// Improvements to
//   Power Model generation
//    Xilinx Compilation - xflow
//
// Revision 1.21  2006/03/21 17:49:19  amag97
// More Fixes
//
// Revision 1.20  2006/03/13 13:22:37  amag97
// Major Commit - new rep
//
// Revision 1.19  2006/02/17 18:14:55  amag97
// Added VHDL generation pass
//
// Revision 1.18  2006/02/14 19:07:54  amag97
// Added new Pass for generating glitch based power cost function
// Bug Fixes - conv. between range and bitwidth
//
// Revision 1.17  2005/06/03 18:02:11  amag97
// ILP Generation + Power Cost function fixes
//
// Revision 1.16  2005/05/30 23:35:06  amag97
// Added ILP generation
//
// Revision 1.15  2005/05/20 17:15:09  amag97
// Power opt. extensions
//
// Revision 1.14  2005/05/13 17:14:51  amag97
// Power data logging
//
// Revision 1.13  2005/05/06 17:51:41  amag97
// Power data - Start
//
// Revision 1.12  2005/01/07 18:24:59  amag97
// More fixes
//
// Revision 1.11  2004/12/31 18:42:00  amag97
// More fixes to divide
//
// Revision 1.10  2004/12/29 13:51:39  amag97
// Added DIV support - not tested
//
// Revision 1.9  2004/12/20 21:28:08  amag97
// Added floating-point verification matlab code generation
//
// Revision 1.8  2004/12/20 00:24:08  amag97
// Added affine errormodel2 - matrix version
//
// Revision 1.7  2004/12/16 20:58:08  amag97
// Many changes and fixes
//
// Revision 1.6  2004/12/13 01:33:43  amag97
// Added command line argument parsing
// Added Interval based range analysis
// Added ASSIGN node to dag
//
// Revision 1.5  2004/12/10 18:18:12  amag97
// Added matlab test code generator
//
// Revision 1.4  2004/11/26 18:20:41  amag97
// Data flow graph based automatic differentiation
//
// Revision 1.3  2004/11/22 21:54:43  amag97
// DAC'05 changes
// ASCBackend
//
// Revision 1.2  2004/11/16 21:56:19  amag97
// More bug fixes
//
// Revision 1.1  2004/11/15 00:21:11  amag97
// Seperate Cost function
//

#ifndef _BITSIZE_PASS_H
#define _BITSIZE_PASS_H

#include "dag.h"
class Node;
class Edge;

class BitSizePass{

public:
  BitSizePass( DAG *_dag );

   void init_name_maps();

  DAG *c_dag;
 
  void gen_signal_name( Edge* CEdge);

  void gen_node_cost_names( Node* CNode );

  map<Edge *, string> signal_names;

  map<Edge *, string> signal_bwnames;

  map<Node *, string> node_costnames;

  map<Edge *, string> error_names;

  string rounding;

  stringstream f_body; 

  stringstream used_node_names;

  virtual void operator() ( ostream &os ){}
 
  int get_bw_state(  Edge *CEdge );
  
  double get_range_bw( Edge *CEdge );

  string get_var_type( Edge *CEdge );

  string replace_str( string str_in , string srch_str , string rpl_str );

};



class ErrorModel_Affine:public BitSizePass{

  public:

  typedef map<Edge *, string> edge_map_string;

  ErrorModel_Affine( DAG * , NodeSchedular * );

   

  void affine_input_node( Node *CNode );
  void affine_add_node( Node *CNode );
  void affine_sub_node( Node *CNode );
  void affine_mul_node( Node *CNode );
  void affine_div_node( Node *CNode );
  void affine_const_node_exp( Node *CNode );
  void affine_shiftl_node( Node *CNode );
  void affine_shiftr_node( Node *CNode );
  void affine_assign_node( Node *CNode );

  double get_const_value( Edge *CEdge );

  double get_abs_eterms( Edge *CEdge );

  void show_affine_vecs();

  void do_affine_analysis_node(dag_index node_num);
  
  void do_affine_analysis();

  //double get_range_max( Edge *CEdge);

  //double get_range_min( Edge *CEdge);

  void gen_signal_bwname( stringstream & , Edge* CEdge);

  void gen_signal_err_expr(  stringstream & , Edge* CEdge );

  void gen_signal_errorname(  stringstream & , Edge* CEdge);

   

  string get_abs_eterms( stringstream &ss , Edge *CEdge );

  void gen_matlab_err_func(ostream &os);


  int affine_index;

  bool affine_error_analysis_done;

  map<string, int> input_arg_names;

 
  
  
   stringstream os;

   NodeSchedular *node_sched;

  void operator() ( ostream &os ){}
 
};

class ErrorModel_Affine_CPP:public BitSizePass{

  public:

  typedef map<Edge *, string> edge_map_string;

  ErrorModel_Affine_CPP( DAG * , NodeSchedular * );

   

  void affine_input_node( Node *CNode );
  void affine_add_node( Node *CNode );
  void affine_sub_node( Node *CNode );
  void affine_mul_node( Node *CNode );
  void affine_div_node( Node *CNode );
  void affine_const_node_exp( Node *CNode );
  void affine_shiftl_node( Node *CNode );
  void affine_shiftr_node( Node *CNode );
  void affine_assign_node( Node *CNode );

  double get_const_value( Edge *CEdge );

  double get_abs_eterms( Edge *CEdge );

  void show_affine_vecs();

  void do_affine_analysis_node(dag_index node_num);
  
  void do_affine_analysis();

  //double get_range_max( Edge *CEdge);

  //double get_range_min( Edge *CEdge);

  void gen_signal_bwname( stringstream & , Edge* CEdge);

  void gen_signal_err_expr(  stringstream & , Edge* CEdge );

  void gen_signal_errorname(  stringstream & , Edge* CEdge);

   

  string get_abs_eterms( stringstream &ss , Edge *CEdge , stringstream &eterm );

  void gen_matlab_err_func(ostream &os);


  int affine_index;

  bool affine_error_analysis_done;

  map<string, int> input_arg_names;

  vector<Edge *> analyzed_edges;

 
  stringstream var_decs;
  
  
   stringstream os;

   NodeSchedular *node_sched;

  void operator() ( ostream &os ){}
 
};


class ErrModel_Affine_FLT_CPP:public BitSizePass{

  public:

  typedef map<Edge *, string> edge_map_string;

  ErrModel_Affine_FLT_CPP( DAG * , NodeSchedular * );

  double get_emax( Edge *CEdge );

  void affine_input_node( Node *CNode );
  void affine_add_node( Node *CNode );
  void affine_sub_node( Node *CNode );
  void affine_mul_node( Node *CNode );
  void affine_div_node( Node *CNode );
  void affine_const_node_exp( Node *CNode );
  void affine_shiftl_node( Node *CNode );
  void affine_shiftr_node( Node *CNode );
  void affine_assign_node( Node *CNode );

  double get_const_value( Edge *CEdge );

  double get_abs_eterms( Edge *CEdge );

  void show_affine_vecs();

  void do_affine_analysis_node(dag_index node_num);
  
  void do_affine_analysis();

  //double get_range_max( Edge *CEdge);

  //double get_range_min( Edge *CEdge);

  void gen_signal_bwname( stringstream & , Edge* CEdge);

  void gen_signal_err_expr(  stringstream & , Edge* CEdge );

  void gen_signal_errorname(  stringstream & , Edge* CEdge);

   

  string get_abs_eterms( stringstream &ss , Edge *CEdge , stringstream &eterm );

  void gen_matlab_err_func(ostream &os);

  double get_abs_maxval( Edge *CEdge );


  int affine_index;

  bool affine_error_analysis_done;

  map<string, int> input_arg_names;

  vector<Edge *> analyzed_edges;

 
  stringstream var_decs;
  
  
   stringstream os;

   NodeSchedular *node_sched;

  void operator() ( ostream &os ){}
 
};


class ErrModel_Affine_LNS_CPP:public BitSizePass{

  public:

  typedef map<Edge *, string> edge_map_string;

  ErrModel_Affine_LNS_CPP( DAG * , NodeSchedular * );

  double get_emax( Edge *CEdge );

  void affine_input_node( Node *CNode );
  void affine_add_node( Node *CNode );
  void affine_sub_node( Node *CNode );
  void affine_mul_node( Node *CNode );
  void affine_div_node( Node *CNode );
  void affine_const_node_exp( Node *CNode );
  void affine_shiftl_node( Node *CNode );
  void affine_shiftr_node( Node *CNode );
  void affine_assign_node( Node *CNode );

  double get_const_value( Edge *CEdge );

  double get_abs_eterms( Edge *CEdge );

  void show_affine_vecs();

  void do_affine_analysis_node(dag_index node_num);
  
  void do_affine_analysis();

  //double get_range_max( Edge *CEdge);

  //double get_range_min( Edge *CEdge);

  void gen_signal_bwname( stringstream & , Edge* CEdge);

  void gen_signal_err_expr(  stringstream & , Edge* CEdge );

  void gen_signal_errorname(  stringstream & , Edge* CEdge);

   

  string get_abs_eterms( stringstream &ss , Edge *CEdge , stringstream &eterm );

  void gen_matlab_err_func(ostream &os);

  double get_abs_maxval( Edge *CEdge );


  int affine_index;

  bool affine_error_analysis_done;

  map<string, int> input_arg_names;

  vector<Edge *> analyzed_edges;

 
  stringstream var_decs;
  
  
   stringstream os;

   NodeSchedular *node_sched;

  void operator() ( ostream &os ){}
 
};



class ErrorModel_Affine2:public BitSizePass{

  public:

  typedef map<Edge *, string> edge_map_string;

  ErrorModel_Affine2( DAG * , NodeSchedular * );

   

  void affine_input_node( Node *CNode );
  void affine_add_node( Node *CNode );
  void affine_sub_node( Node *CNode );
  void affine_mul_node( Node *CNode );
  void affine_div_node( Node *CNode );
  void affine_const_node_exp( Node *CNode );
  void affine_shiftl_node( Node *CNode );
  void affine_shiftr_node( Node *CNode );
  void affine_assign_node( Node *CNode );

  double get_const_value( Edge *CEdge );

  void get_abs_eterms( Edge *CEdge );

  void show_affine_vecs();

  void do_affine_analysis_node(dag_index node_num);
  
  void do_affine_analysis();

  void gen_signal_bwname( stringstream & , Edge* CEdge);

  void gen_signal_err_expr(  stringstream & , Edge* CEdge );

  void gen_signal_errorname(  stringstream & , Edge* CEdge);

  void gen_errvalues( stringstream &ss1 , Edge *CEdge );

  void get_abs_eterms( stringstream &ss , Edge *CEdge );

 
  void gen_matlab_err_func(ostream &os);


  int affine_index;

  bool affine_error_analysis_done;

  map<string, int> input_arg_names;

  //map<Edge *, string> error_names;
  
   
  map<int , Edge* > af_ndx_map;

   stringstream os;

   NodeSchedular *node_sched;

  void operator() ( ostream &os ){}
 
};


class CostFunction:public BitSizePass{

public:
  CostFunction( DAG *_dag  );

 
  
  
  void gen_ml_add_node_exp(  Node * CNode );
  void gen_ml_sub_node_exp(  Node * CNode );
  void gen_ml_mul_node_exp(  Node * CNode );
  void gen_ml_div_node_exp(  Node * CNode );

  void gen_ml_node_cf( Node *CNode );

  void gen_matlab_costf( ostream &os , string err_func_name );
 
  void gen_parameters_func( ostream &os );

  void cost_function_end(ostream &os );
  
  double range_bw( Edge *CEdge );

  string state_bw(  Edge *CEdge );

  int edges_to_analyse;
 
  void operator() ( ostream &os ){}
};


class CostFunctionAreaCPP:public BitSizePass{

public:
  CostFunctionAreaCPP( DAG *_dag  );

 
  
  
  void gen_ml_add_node_exp(  Node * CNode );
  void gen_ml_sub_node_exp(  Node * CNode );
  void gen_ml_mul_node_exp(  Node * CNode );
  void gen_ml_div_node_exp(  Node * CNode );

  void gen_ml_node_cf( Node *CNode );

  void gen_costf( ostream &os , string err_func_name );
 
  void gen_parameters_func( ostream &os );

  void cost_function_end(ostream &os );
  
  double range_bw( Edge *CEdge );

  string state_bw(  Edge *CEdge );

  int edges_to_analyse;
 
  int cost_nodes;

  void operator() ( ostream &os ){}
};

class CostFuncAreaFLTCPP:public BitSizePass{

public:
  CostFuncAreaFLTCPP( DAG *_dag  );

 
  
  
  void gen_ml_add_node_exp(  Node * CNode );
  void gen_ml_sub_node_exp(  Node * CNode );
  void gen_ml_mul_node_exp(  Node * CNode );
  void gen_ml_div_node_exp(  Node * CNode );

  void gen_ml_node_cf( Node *CNode );

  void gen_costf( ostream &os , string err_func_name );
 
  void gen_parameters_func( ostream &os );

  void cost_function_end(ostream &os );
  
  double range_bw( Edge *CEdge );

  string state_bw(  Edge *CEdge );

  int edges_to_analyse;
 
  int cost_nodes;

  void operator() ( ostream &os ){}
};


class CostFuncAreaLNSCPP:public BitSizePass{

public:
  CostFuncAreaLNSCPP( DAG *_dag  );


  void gen_ml_add_node_exp(  Node * CNode );
  void gen_ml_sub_node_exp(  Node * CNode );
  void gen_ml_mul_node_exp(  Node * CNode );
  void gen_ml_div_node_exp(  Node * CNode );

  void gen_ml_node_cf( Node *CNode );

  void gen_costf( ostream &os , string err_func_name );
 
  void gen_parameters_func( ostream &os );

  void cost_function_end(ostream &os );
  
  double range_bw( Edge *CEdge );

  string state_bw(  Edge *CEdge );

  int edges_to_analyse;
 
  int cost_nodes;

  void operator() ( ostream &os ){}
};


class CostFunctionPWR:public BitSizePass{

public:
  CostFunctionPWR( DAG *_dag  );

 
  
  
  void gen_ml_add_node_exp(  Node * CNode );
  void gen_ml_sub_node_exp(  Node * CNode );
  void gen_ml_mul_node_exp(  Node * CNode );
  void gen_ml_div_node_exp(  Node * CNode );

  void gen_ml_node_cf( Node *CNode );

  void gen_matlab_costf( ostream &os , string err_func_name );
 
  void gen_parameters_func( ostream &os );

  void cost_function_end(ostream &os );
  
  double range_bw( Edge *CEdge );

  string state_bw(  Edge *CEdge );

  int edges_to_analyse;


  stringstream coeff_fs;

  int coeff_vec_ctr;

   void operator() ( ostream &os ){}
  


};


class ASCBackend:public BitSizePass{
  
public:
  ASCBackend( DAG *, NodeSchedular * );

  void do_ascbackend( ostream &os );
  void do_ascbackend_node( dag_index node_num  );

  void gen_var_decs();

  void gen_add_node_exp(  Node * CNode );
  void gen_sub_node_exp(  Node * CNode );
  void gen_mul_node_exp(  Node * CNode );

  void gen_const_node_exp( Node * CNode );

  void gen_header( ostream &os );

  stringstream dec_body;
  NodeSchedular *node_sched;


  map<Edge *,int> used_edges;

  void operator() ( ostream &os );

};


class MatCodeGen:public BitSizePass{
  
public:
  MatCodeGen( DAG *, NodeSchedular * );

  void do_matcodegen( ostream &os );
  void do_matcodegen_node( dag_index node_num  );

  void gen_var_decs();

  void gen_add_node_exp(  Node * CNode );
  void gen_sub_node_exp(  Node * CNode );
  void gen_mul_node_exp(  Node * CNode );
  void gen_div_node_exp(  Node * CNode );
  void gen_shiftl_node( Node *CNode );
  void gen_shiftr_node( Node *CNode );
  

  void gen_const_node_exp( Node * CNode );

  void gen_output_node_exp( Node *CNode );
  void gen_input_node_exp( Node *CNode );

  void gen_header( ostream &os );

  void gen_footer( ostream &os );

  int set_signal_order( Edge *CEdge );

  bool is_output_edge( Edge *CEdge );


  stringstream dec_body;
  NodeSchedular *node_sched;

  string rmode;

  map<Edge *,int> used_edges;

  void operator() ( ostream &os );

  map<int , Edge*> signal_order;

  int signal_ctr;

  int input_ctr;
  int out_bw_count;

  string input_sign;
  
  int input_bw_int;
  int input_bw_frac;

};




class MatCodeGenFlt:public BitSizePass{
  
public:
  MatCodeGenFlt( DAG *, NodeSchedular * );

  void do_matcodegen( ostream &os );
  void do_matcodegen_node( dag_index node_num  );

  void gen_var_decs();

  void gen_add_node_exp(  Node * CNode );
  void gen_sub_node_exp(  Node * CNode );
  void gen_mul_node_exp(  Node * CNode );
  void gen_div_node_exp(  Node * CNode );

  void gen_const_node_exp( Node * CNode );

  void gen_output_node_exp( Node *CNode );
  void gen_input_node_exp( Node *CNode );

  void gen_header( ostream &os );

  void gen_footer( ostream &os );

  int set_signal_order( Edge *CEdge );

  stringstream dec_body;
  NodeSchedular *node_sched;

  string rmode;

  map<Edge *,int> used_edges;

  void operator() ( ostream &os );

  map<int , Edge*> signal_order;

  int signal_ctr;

  int input_ctr;

  string input_sign;
  
  int input_bw;

};


class ADDAG:public BitSizePass{

  
public:
  ADDAG( DAG * , NodeSchedular * );

  //DAG* c_dag;
  NodeSchedular *node_sched;

  void ad_input_node( Node *CNode );
  void ad_add_node( Node *CNode );
  void ad_sub_node( Node *CNode );
  void ad_mul_node( Node *CNode );
  void ad_div_node( Node *CNode );
  void ad_shiftl_node( Node *CNode );
  void ad_shiftr_node( Node *CNode );

  void ad_assign_node( Node *CNode );

  void ad_const_node( Node *CNode );

  void ad_output_node( Node *CNode );

  double get_const_value( Edge *CEdge );

  double get_abs_eterms( Edge *CEdge );

  void show_ad_vecs();

  void gen_func_body( ostream &os );

  void do_ad_analysis_node(dag_index node_num);
  
  void do_ad_analysis();

  void find_min_max( Edge *CEdge );

  int ad_index;
 

  bool stop_flag;

  bool operator()();

  double read_from_file();

  fstream fs,d_out;

  map<int , Edge* > ad_ndx_map;

  int iter_count;

  void gen_signal_errorname( Edge* CEdge );

  void get_abs_eterms( stringstream &ss , Edge *CEdge );
 
  void gen_matlab_err_func(ostream &os);

  void gen_errvalues( stringstream &ss1 , Edge *CEdge );


 

};

class PwrDataPass:public BitSizePass{ 

public:
  PwrDataPass(  DAG * , NodeSchedular * );

  bool operator()();

  fstream fs,d_out;

  int iter_count;
  
  void do_pwrdata_sim_node(dag_index node_num);
  
  bool do_pwrdata_fullsim();

  void do_pwrdata_sim();

  double read_from_file();
		      
  NodeSchedular *node_sched;

  void sim_input_node( Node *CNode );
  void sim_const_node( Node *CNode );
  void sim_add_node( Node *CNode );
  void sim_sub_node( Node *CNode );
  void sim_mul_node( Node *CNode );
  void sim_div_node( Node *CNode );
  void sim_shiftl_node( Node *CNode );
  void sim_shiftr_node( Node *CNode );

  void sim_assign_node( Node *CNode );
  void sim_output_node( Node *CNode );

private:
   bool stop_flag;


};




class ILPGenPass:public BitSizePass{

public:
   ILPGenPass( DAG *_dag , int _lbound , int _ubound );

   void gen_ilp_add_node_exp(  Node * CNode );
   void gen_ilp_sub_node_exp(  Node * CNode );
   void gen_ilp_mul_node_exp(  Node * CNode );
  //void gen_ilp_div_node_exp(  Node * CNode );

   void gen_ilp_node_cf( Node *CNode );

   void gen_ilp_costfunc( ostream &os );
 
   void gen_parameters_func( ostream &os );

   void cost_function_end(ostream &os );
  
   double range_bw( Edge *CEdge );

  void list_binvars( stringstream &bin_list , stringstream &bin_const , stringstream &bw_val , stringstream &ulp_val);

  void operator() ( ostream &os ){}
  
  void gen_binvar( stringstream &ss , string edge_name , int binvar_num );

  void gen_error_const( stringstream &ss );


  void gen_varnames_list( ostream& os );

private:

  int u_bound, l_bound;
  
  int bound_num;
  
  int node_num;

  list<string> cf_nodes;

};

class PwrPassGlitch:public BitSizePass{

public:
   PwrPassGlitch( DAG *_dag , NodeSchedular *_node_sched );

  
  void pwr_add_node_exp(  Node * CNode  );

  void pwr_sub_node_exp(  Node * CNode  );
  void pwr_mul_node_exp(  Node * CNode  );
  void pwr_div_node_exp(  Node * CNode  );

  void pwr_const_node_exp( Node * CNode );

  void pwr_output_node_exp( Node *CNode  );
  void pwr_input_node_exp( Node *CNode  );


  void pwr_shiftr_node(  Node * CNode );
  void pwr_shiftl_node(  Node * CNode );

   void gen_pwrcost_node_cf( Node *CNode );

   void gen_pwrcost_costfunc( ostream &os );
 
   void gen_parameters_func( ostream &os );

   void cost_function_end(ostream &os );
  
   double range_bw( Edge *CEdge );

  void list_binvars( stringstream &bin_list , stringstream &bin_const , stringstream &bw_val , stringstream &ulp_val);

  void operator() ( ostream &os );
  
  void gen_binvar( stringstream &ss , string edge_name , int binvar_num );

  void gen_error_const( stringstream &ss );


  void gen_varnames_list( ostream& os );

  
private:


  void do_pwr_pass();

  NodeSchedular *node_sched;

  void do_pwr_pass_node( DAG_NODE node );

  
  struct GLITCH_PROF {

    int peak_pos;
    double peak_height;
    int bit_width; 

  };


  bool mat_code_gen;
  
  stringstream f_body,h_body,foot_body;

  GLITCH_PROF* G_Prof_Add_ng(Edge* edge_l , Edge* edge_r );
  GLITCH_PROF* G_Prof_Sub_ng(Edge* edge_l , Edge* edge_r );
  GLITCH_PROF* G_Prof_Mul_ng(Edge* edge_l , Edge* edge_r );

  void MAT_G_Prof_Add_ng(Edge* edge_l , Edge* edge_r , Edge* edge_out);
  void MAT_G_Prof_Sub_ng(Edge* edge_l , Edge* edge_r , Edge* edge_out);
  void MAT_G_Prof_Mul_ng(Edge* edge_l , Edge* edge_r , Edge* edge_out);

  void MAT_G_Prof_Add(Edge* edge_l , Edge* edge_r , Edge* edge_out);
  void MAT_G_Prof_Sub(Edge* edge_l , Edge* edge_r , Edge* edge_out);
  void MAT_G_Prof_Mul(Edge* edge_l , Edge* edge_r , Edge* edge_out);

  void MAT_G_Prof_CShift_l( Edge* edge_in , double const_val , Edge *edge_out );
  void MAT_G_Prof_CShift_r( Edge* edge_in , double const_val , Edge *edge_out );


  void MAT_G_Prof_Input( Edge* edge_out);

 //  double G_Prof_Add_pwr_ng( GLITCH_PROF* G_prof_l, GLITCH_PROF* G_prof_r );
//   double G_Prof_Sub_pwr_ng( GLITCH_PROF* G_prof_l, GLITCH_PROF* G_prof_r );
//   double G_Prof_Mul_pwr_ng( GLITCH_PROF* G_prof_l, GLITCH_PROF* G_prof_r );

  GLITCH_PROF* G_Prof_Add_g(GLITCH_PROF* G_prof_l, GLITCH_PROF* G_prof_r  );
  GLITCH_PROF* G_Prof_Sub_g(GLITCH_PROF* G_prof_l, GLITCH_PROF* G_prof_r  );
  GLITCH_PROF* G_Prof_Mul_g(GLITCH_PROF* G_prof_l, GLITCH_PROF* G_prof_r  );
  
  
  void pwr_cost_header();
  void pwr_cost_footer();
  


  map< Edge *, GLITCH_PROF *> edge_glitch_map;

  list<string> PWR_nodes;

  int signal_count;

  int output_signal_count;

};


class VHDLGen:public BitSizePass{
  
public:


  VHDLGen( DAG *, NodeSchedular * );


  void operator() ( string input_filename , ostream &os );

  void gen_test_bench( ostream &os );

private:

  enum SIG_TYPE { INPUT_SIG , OUTPUT_SIG , INTERNAL_SIG };

  enum AC_TYPE { ADD_COMP , SUB_COMP , MUL_COMP , CMUL_COMP, REG_COMP};

  struct COMP_TYPE{

    COMP_TYPE( unsigned int a , unsigned int b , bool c , unsigned int o , AC_TYPE _c_type ):bw_a(a),bw_b(b),clocked(c),bw_c(o),c_type(_c_type){}

    unsigned int bw_a;
    unsigned int bw_b;
    bool clocked;
    unsigned int bw_c;
    AC_TYPE c_type;
    unsigned int const_value;

  };


  struct BW_DATA{

    BW_DATA( unsigned int ibw , unsigned int fbw):int_bw(ibw),frac_bw(fbw),total_bw(ibw+fbw){}

    unsigned int int_bw;
    unsigned int frac_bw;
    unsigned int total_bw;
    

  };

  void do_vhdlgen();
  void do_vhdlgen_node( dag_index node_num);
  
  
  void gen_add_node(  Node * CNode );
  void gen_sub_node(  Node * CNode );
  void gen_mul_node(  Node * CNode );
  void gen_div_node(  Node * CNode ){}
  void gen_shiftl_node( Node *CNode );
  void gen_shiftr_node( Node *CNode );
  

  void gen_const_node( Node * CNode );

  void gen_output_node( Node *CNode );
  void gen_input_node( Node *CNode );

  

  bool is_output_edge( Edge *CEdge );
  bool is_input_edge( Edge *CEdge );


  void process_signal();

  string get_signal_name( Edge* CEdge );

  stringstream struct_code;
  stringstream entity_code;
  stringstream arch_code;

  stringstream sig_dec;

  stringstream comp_code;
  
  stringstream code_bottom;
  
  stringstream dec_body;

  stringstream bs_struct;

  void gen_bs_struct( Node *c_node , Node *l_node , Node *r_node );

  NodeSchedular *node_sched;

  string rmode;

  map<Edge *, SIG_TYPE>  signal_type;
  
  map<Edge* , string > signal_names;

  map<string , Edge*> varnames_map;

  vector<Edge * > input_signals;

  vector<Edge * > output_signals;

  vector<Edge * > internal_signals;

  map< string , COMP_TYPE * > comp_table;

  map< Edge* , BW_DATA * > edge_bwdata;

  void gen_entity_sec();

  void gen_arch_sec();

  void gen_struct_sec();
 
  void output_code( ostream &os );

  void add_internal_signal( Edge *CEdge );

  void gen_components();

  void update_edges_bw( string inputfile_name );

  void update_edges_bw( );


  // Test Bench Generation

  void gen_tb_top(stringstream &tb_top);

  void gen_tb_arch(stringstream &tb_arch);

  void gen_tb_struct( stringstream &tb_str );
  
  void adjust_signal_bw(int comp_out_bw , int bw_out , string node_name);

  void gen_binvalue( ostream &ss , double val , int int_bw , int frac_bw );
};

#endif
