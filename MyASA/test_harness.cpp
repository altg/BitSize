#include "myasa.h"

extern "C" {
#include <dlfcn.h>
}


/* Parameter Function, Error Function and Cost Function Proto-types */

typedef int (*ParamFunc) ( long int *, USER_DEFINES *);

typedef int (*ErrFunc) ( double *, long int*, int *,USER_DEFINES *);

typedef double (*CostFunc) (double *,double *,double *,double *,double *,long int *, int *, int *,int *, USER_DEFINES *);

int main ( int argc , char **argv ){


  long int *number_of_params = new long int;

  USER_DEFINES *USER_OPTIONS;// = new USER_DEFINES;

  USER_OPTIONS = new USER_DEFINES;

  double *param_lb, *param_ub, *start_params;

  int *param_type;

  long int index;


  cout << "Compiling " << endl;
  system( "g++ -fPIC -c -g -Wall mycost_func3.cpp -o mycost_func3.o" );

  cout << "Creating Shared library" << endl;
  system( "g++ -shared -Wl,-soname,libmycost.so -o libmycost.so mycost_func3.o" );

  void* theLib = dlopen("./libmycost.so", RTLD_LAZY);
	if (!theLib)
	{
		cout << dlerror() << "\n";
		return -1;
	}
  cout << "Cost Function Library opened!\n";

  
  ParamFunc theParamFunc = (ParamFunc)dlsym(theLib , "setup_parameters" );
  
  if (!theParamFunc)
	{
	  cout << dlerror() << "\n";
	  return -1;
	}
  cout << "Parameter Function located!\n";


  (*theParamFunc)( number_of_params , USER_OPTIONS );


  param_lb = new double[ *number_of_params ];
  
  param_ub = new double[ *number_of_params ];

  start_params  = new double[ *number_of_params ];

  param_type = new int[ *number_of_params ];

  for (index = 0; index <  *number_of_params; ++index)
    param_lb[index] = 20.0;
  for (index = 0; index <  *number_of_params; ++index)
    param_ub[index] = 30.0;

   for (index = 0; index <  *number_of_params; ++index)
    start_params[index] = 30.0;
 
  for (index = 0; index <  *number_of_params; ++index)
    param_type[index] = ((int) 1);  


  ErrFunc theErrFunc = (ErrFunc)dlsym(theLib, "error_function");
  if (!theErrFunc)
	{
		cout << dlerror() << "\n";
		return -1;
	}
  cout << "Error Function located!\n";


  while( (*theErrFunc)( start_params , number_of_params , param_type , USER_OPTIONS ) ){

     for (index = 0; index <  *number_of_params; ++index)
       start_params[index]--;
    
  }

   for (index = 0; index <  *number_of_params; ++index)
       start_params[index]++;


  cout << "Uniform BW " <<  start_params[0] << endl;


  double *cost_tan, *cost_curve;

  //  int *cost_flag , *exit_code;

  int *cost_flag = new int;

  int *exit_code = new int;

  CostFunc theCostFunc = (CostFunc) dlsym(theLib, "cost_function");


  if (!theCostFunc)
	{
		cout << dlerror() << "\n";
		return -1;
	}
  cout << "Cost Function located!\n";

  double ubw_cost = (*theCostFunc)( start_params , param_lb , param_ub , cost_tan , cost_curve , number_of_params , param_type , cost_flag , exit_code , USER_OPTIONS );
 
 cout << "Starting Cost :" << ubw_cost << endl;


  double final_cost = 0;


  final_cost = run_asa_opt( theCostFunc , *number_of_params , param_type , start_params , param_lb , param_ub , USER_OPTIONS );

 
  cout << "Final Cost : " << final_cost << endl;

   for (index = 0; index <  *number_of_params; ++index)
     cout << start_params[index] << endl;

  return 0;

}
