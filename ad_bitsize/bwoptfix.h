// $Author: amag97 $
// $Id: bwoptfix.h,v 1.8 2004/10/26 18:32:59 amag97 Exp $
// $Log: bwoptfix.h,v $
// Revision 1.8  2004/10/26 18:32:59  amag97
// Fixed most bugs with fixed-point frac. opt 6
//
// Revision 1.7  2004/10/25 14:31:14  amag97
// Fixed fraction optimisation mode 6
//
// Revision 1.6  2004/10/23 10:37:34  amag97
// Addition of new fraction bw evaluation method
// (work in progress)
//
// Revision 1.5  2004/09/17 16:52:13  amag97
// Added area estimation code
//
// Revision 1.4  2004/09/10 17:21:55  amag97
// Changes to bwopt method 3
//
// Revision 1.3  2004/08/26 18:00:51  amag97
// Update after return from Sri Lanka
// Started work on adding user constraints
//
// Revision 1.2  2004/07/06 19:48:11  amag97
// Revised DAG generation
//
// Revision 1.1  2004/06/24 18:45:31  amag97
// Added a seperate class to encapsulate the bitwidth optimisation functions
//

// Class to encapsulate the bitwidth optimisation methods for HWfix

#ifndef _BWOPTFIX_H
#define _BWOPTFIX_H



#include "bitsize_defs.h"

#include "hwfix.h"
#include "advar.h"
#include "bsclass.h"

#include <list>
#include <vector>
#include <algorithm>

using namespace std;

class BWOPTfix {

public:
  BWOPTfix();

  void find_bw( HWfix *var , int vnumber );

  int current_method;
  
  void list_vars();

   void setup_max_grads(HWfix *var);

private:
   typedef struct {

    bool accept_flag;
    double cost_value;
  }COST_RETURN;
  
  typedef vector< int > bw_vec_type;

  void find_frac_bw();
  
  void frac_method1();

  void frac_method2();

  void frac_method3( double );
  
  void frac_method4( double );

  void frac_method5( dag_index , double );

  void frac_method6( double );

  void find_int_bw();

  

  double find_output_err(vector<int>::iterator bwt_itr , vector<int>::iterator bwt_end);  
  double find_output_err(vector<int> &bw_table);  


  int find_uniform_bw_linear( vector<int>& bw_vector , vector<double> grad_vector );
  void find_uniform_bw_binary( vector<int>& bw_vector , vector<double> grad_vector , int upper_bw , int lower_bw );

  double find_best_bw_vector( vector<bw_vec_type>& sel_bw_vector , vector<double>& grad_vec , vector<HWfix *>& vars_to_consider , bw_vec_type& best_bw_vector );

  double find_error( vector<int>& bw_vector ,  vector<double>& grad_vector );
  COST_RETURN cost_function( vector<int>& bw_vector , vector<double>& grad_vector , vector<HWfix *>& var_vector );
  
  void show_bw_vector( vector<bw_vec_type>& sel_bw_vector );
  void show_bw_vector( vector<int>& bw_vector );

  HWfix *c_var;

  int var_number;

  double output_error;

  double try_selected_vectors( vector< bw_vec_type >& sel_bw_vector_in, vector<double> &grad_vec, vector<HWfix *>& vars_to_consider, vector< bw_vec_type >& sel_bw_vector_out, double start_cost );
  
  double try_selected_vectors( vector<  int  >& bw_vector_in, vector<double> &grad_vec, vector<HWfix *>& vars_to_consider, vector< bw_vec_type >& sel_bw_vector_out, double start_cost );
 
  void inc_bw_vec( vector<int>& bw_vector , int inc );

  int generate_random_bit();

};



#endif 
