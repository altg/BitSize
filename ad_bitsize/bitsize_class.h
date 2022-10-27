#ifndef _BITSIZE_CLASS_H
#define _BITSIZE_CLASS_H

#include <string>
class AreaModel;
class ErrorModel;
class RangeAnalysis;
class ErrorModel_Affine;
class ErrorModel_Affine2;

class ErrorModel_Affine_CPP;
class ErrModel_Affine_FLT_CPP;
class ErrModel_Affine_LNS_CPP;

class CostFunction;

class CostFunctionAreaCPP;
class CostFuncAreaFLTCPP;
class CostFuncAreaLNSCPP;

class CostFunctionPWR;
class DAG;

class BitSizePass;
class NodeSchedular;
class ASCBackend;
class ADDAG;
class OptBW_flt;
class OptBW_fx;
class OptBW_cpp;
class OptBWFLT_cpp;
class OptBWLNS_cpp;

class PwrDataPass;
class PwrPassGlitch;


class ILPGenPass;
class VHDLGen;

class BWResultsGen;

class OptEngine;

#include "hwvar.h"
//#include "hwfloat.h"
#include "hwint.h"
#include "hwvector.h"

#include "varcollections.h"

#include "dag.h"

#include "node_sched.h"
#include "areamodel.h"
#include "errormodel.h"
#include "range_analysis.h"
#include "errormodel_affine.h"

#include "ad_dag.h"
#include "optbw.h"

#include "bw_results_gen.h"

#include "optengine.h"


enum OPMODE_TYPE { PARSING , PARSED };

enum DEBUGMODE_TYPE { OFF , L1 , L2 };

//typedef unsigned int size_t;

//typedef unsigned int 

class bitsize{


public:

  enum RANGE_ANN_MODES { AFFINE_RANGE , INTERVAL_RANGE , SIM_RANGE , NOOPT_RANGE };

  enum PREC_ANN_MODES { AFFINE_PREC,  AFFINE_PREC2 , AD_PREC , NOOPT_PREC , AFFINE_PREC_CPP , AD_PREC_CPP};
  
  enum BW_MODES { UNIFORM , NONUNIFORM };

  enum OPT_METRICS { AREA_OPT , POWER_OPT , POWER_OPT_GLITCH , AREA_OPT_CPP, NO_OPT };

  bitsize( int argc , char *argv[] );


  void bitsize_main();

  //std::string system_name;

  int opt_mode;

  DAG *mydag;

  OPMODE_TYPE global_opmode;

  DEBUGMODE_TYPE global_dbgmode;

  bool execute_once;
  
  double get_area();
  void list_vars();
  void list_vars2();

  void test_var(HWvar *t);


  void start_parse();
  void end_parse();

  varcollections<HWvar *> varC;

 

  // System Global settings

  RANGE_ANN_MODES range_ann_mode;

  PREC_ANN_MODES prec_ann_mode;

  BW_MODES bw_mode;

  OPT_METRICS opt_metric;

  bool mat_code_gen;

  bool ad_err_func_only;


  int db_level;


  bool run_time_args;

  int num_r_args;

  vector<string> r_args;

  // BitSize Passes
  AreaModel *myarea;

  NodeSchedular *mynode_sched;

  RangeAnalysis *range_analysis;

  ErrorModel_Affine *errmodel_affine;
  ErrorModel_Affine2 *errmodel_affine2;

  ErrorModel_Affine_CPP *errmodel_affine_cpp;
  
  ErrModel_Affine_FLT_CPP *errmodel_affine_flt_cpp;
  ErrModel_Affine_LNS_CPP *errmodel_affine_lns_cpp;

  ErrorModel *myerror;

  CostFunction *cost_func;

  CostFunctionAreaCPP *cost_func_cpp;

  CostFuncAreaFLTCPP  *cost_func_flt_cpp;

  CostFuncAreaLNSCPP  *cost_func_lns_cpp;

  ASCBackend *asc_backend;

  ADDAG *ad_dag;

  BitSizePass *bs_pass;

  OptBW_flt *opt_flt;
  OptBW_fx *opt_fx;

  OptBW_cpp *opt_cpp;

  OptBWFLT_cpp *optflt_cpp;
  OptBWLNS_cpp *optlns_cpp;

  // Power Passes
  PwrDataPass *pwr_opt_datapass;
  
  PwrPassGlitch *pwr_pass_glitch;

  CostFunctionPWR *cost_func_pwr;
  
  bool ilp_gen;

  int ilp_ub_val;

  ILPGenPass *ilp_pass;

  bool vhdl_gen;

  VHDLGen *vhdlgen_pass;

  BWResultsGen *result_gen;

  void list_edges();
  

  map<int ,  Edge* >param_name_map;
  
private:


  void run_passes();

  void run_passes_2();


  void init_passes();

  void run_passes_pwr();

  void run_passes_pwr_glitch();

  void run_fixedpoint_error();

  void run_fltpoint_error();

  void run_lns_error();

  void gen_mat_testcode_fix();

  void gen_ilp_code();

  void gen_vhdlcode();

  // Input argument passing
  void show_help();
  void setup_defaults();
  void parse_arguments( int argc , char *argv[] );


  bool do_bit_opt;

  string bwdata_file;

  //void propagate_error();

};



#endif
