#ifndef OPTENGINE_H
#define OPTENGINE_H

#include <sstream>
#include <iostream>

#include <cstdlib>

#include "myasa.h"

#include "dag.h"
class Node;
class Edge;

extern "C" {
#include <dlfcn.h>
}

using namespace std;

typedef int (*ParamFunc) ( long int *, USER_DEFINES *);

typedef int (*ErrFunc) ( double *, long int*, int *,USER_DEFINES *);

typedef double (*CostFunc) (double *,double *,double *,double *,double *,long int *, int *, int *,int *, USER_DEFINES *);

class OptEngine{

public:

  OptEngine();

  void run_optimization();

  void update_edges_uni( vector<Edge *>& analyzed_edges );

  void update_edges( vector<Edge *>& analyzed_edges );

  void print_opt_summary( ostream &os );

  ~OptEngine();

  double ubw_cost;
  
  double final_cost;

  double *cost_params;

  double uni_bw;

  double start_bw;

  double upper_limit;

  double lower_limit;

private:
  
  

  long int *number_of_params;

  

};



#endif

