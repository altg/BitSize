// $Author: amag97 $
// $Id: optbw.h,v 1.8 2005/04/03 22:15:20 amag97 Exp $


#ifndef OPTBW_H
#define OPTBW_H 

#include "dag.h"
#include "ad_dag.h"
#include "areamodel.h"
class ADDAG;
class DAG;
class AreaModel;

typedef struct  dbl_ints {
  unsigned      lsw;
  unsigned      msw;
} DBL_INT;


typedef struct  dbl_flds {
  unsigned      lsw     : 32; // 0xFFFF FFFF 0000 0000
  unsigned      msw     : 20; // 0x0000 0000 FFFF F000
  unsigned      exp     : 11; // 0x0000 0000 0000 0FFE
  unsigned      sgn     :  1; // 0x0000 0000 0000 0001
} DBL_FLD;


typedef union   dbl_prec        {
  double        dbl_as_flt;
  DBL_FLD       dbl_as_fld;
  DBL_INT       dbl_as_int;
} DBL_PREC;


class OptBW{

public:
  OptBW( DAG* _dag , ADDAG *_addag , AreaModel *_areamodel   );

   typedef vector< int > bw_vec_type;
  
  ADDAG *ad_dag;

  DAG *cdag;

  AreaModel *area_model;

  void get_max_ad();

  void get_max_ad( Edge *CEdge );

  void get_max_ad2();

  void get_max_ad2( Edge *CEdge );

  void ad_error_func_gen(ostream &os);
  

  void show_bw_vector( vector<int>& bw_vector );
  
  vector<Edge *> edges_to_consider;

  vector<int> bw_vector;

  vector<bw_vec_type> selected_vectors;

  double sys_temp;

  double c_cost;
  
  double best_cost;

  double prev_cost;
  
  vector<int>  best_bw_vector;

  int generate_random_bit();

  double get_rand_val();

  virtual double get_area_cost( vector<int>& bw_vector ){}

  void set_fbw( vector<int>& bw_vector_in , int fbw );

  void save_bw_vector(vector<int>& bw_vector_in , vector<bw_vec_type>& new_selected_vectors);

  void swap_vectors( vector<bw_vec_type>& vec1 , vector<bw_vec_type>& vec2 );

  bool accept_bw_vector( vector<int>& bw_vector_in );

 void create_new_bw_vectors( bw_vec_type& bw_vector_in  , vector<bw_vec_type>& new_bw_vectors );

 

 bool get_output_err( vector<int>& bw_vector_in );

  virtual bool get_output_err( Edge *CEdge ,  vector<int>& bw_vector_in ){}
  
  void find_nuf_fbw();

  virtual  void create_vectors(){}
  
};


class OptBW_flt:public OptBW{


 public:
  OptBW_flt( DAG* , ADDAG * , AreaModel * );

  void operator() ( ostream &os );

  void create_vectors();
  //bool get_output_err();
  //bool get_output_err( Edge *CEdge );

  double get_area_cost( vector<int>& bw_vector );
   bool get_output_err( Edge *CEdge ,  vector<int>& bw_vector_in );

  //double get_signal_error( Edge *CEdge );

  int find_uf_mbw(int u_bw);

  double update_bws( vector<int>& bw_vector_in );

  //void find_nuf_fbw();
  //void set_mbw( int mbw );

  //void find_nuf_fbw();

};

 
class OptBW_fx:public OptBW{


 public:
  OptBW_fx( DAG* , ADDAG * , AreaModel * );

 
  void operator() ( ostream &os );

  

  
   bool get_output_err( Edge *CEdge ,  vector<int>& bw_vector_in );

  double get_area_cost( vector<int>& bw_vector );

  //double get_signal_error( Edge *CEdge );

  int find_uf_fbw(int u_bw);

  //void find_nuf_fbw();

   void create_vectors();

 
  double update_bws( vector<int>& bw_vector_in );

};

class OptBW_cpp:public OptBW{

public:
  OptBW_cpp( DAG* , ADDAG * );

  void run_adanalysis(ostream &os);

  void gen_error_func( ostream &os );

  void update_edges_uni(double uni_bw );

  void update_edges(double *cost_params);
  
private:

  void gen_func_body( ostream &os );

  void gen_func_top( ostream &os );

  stringstream var_decs;

};



class OptBWFLT_cpp:public OptBW{

public:
  OptBWFLT_cpp( DAG* , ADDAG * );

  void run_adanalysis(ostream &os);

  void gen_error_func( ostream &os );

  void update_edges_uni(double uni_bw );

  void update_edges(double *cost_params);
  
private:

  double get_emax( Edge *CEdge );

  void gen_func_body( ostream &os );

  void gen_func_top( ostream &os );

  stringstream var_decs;

};


class OptBWLNS_cpp:public OptBW{

public:
  OptBWLNS_cpp( DAG* , ADDAG * );

  void run_adanalysis(ostream &os);

  void gen_error_func( ostream &os );

  void update_edges_uni(double uni_bw );

  void update_edges(double *cost_params);
  
private:

  double get_emax( Edge *CEdge );

  void gen_func_body( ostream &os );

  void gen_func_top( ostream &os );

  stringstream var_decs;

};

#endif
