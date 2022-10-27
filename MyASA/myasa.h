#ifndef MY_ASA_H
#define MY_ASA_H

#include <errno.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>             /* misc defs on most machines */
#include <string.h>  

#include <iostream>

using namespace std;

#define LONG_INT long int

#define TRUE 1
#define FALSE 0


typedef struct {
    long int Limit_Acceptances;
    long int Limit_Generated;
    int Limit_Invalid_Generated_States;
    double Accepted_To_Generated_Ratio;

    double Cost_Precision;
    int Maximum_Cost_Repeat;
    int Number_Cost_Samples;
    double Temperature_Ratio_Scale;
    double Cost_Parameter_Scale_Ratio;
    double Temperature_Anneal_Scale;
#if USER_ASA_OUT
    char *Asa_Out_File;
#endif    


    int Include_Integer_Parameters;
    int User_Initial_Parameters;
    long int Sequential_Parameters;
    double Initial_Parameter_Temperature;







    int Acceptance_Frequency_Modulus;
    int Generated_Frequency_Modulus;
    int Reanneal_Cost;
    int Reanneal_Parameters;

    double Delta_X;



    int User_Tangents;
    int Curvature_0;
    long int N_Accepted;
    long int N_Generated;
    int Locate_Cost;
    int Immediate_Exit;

    double *Best_Cost;
    double *Best_Parameters;
    double *Last_Cost;
    double *Last_Parameters;
    int Asa_Recursive_Level;

  // BitSize Options

    double *out_err_spec;
    int *out_dim;

  } USER_DEFINES;

#ifdef __cplusplus
extern "C" {
#endif
  double
    asa (double (*user_cost_function)


         (double *, double *, double *, double *, double *, long int *,
          int *, int *, int *, USER_DEFINES *),
         double (*user_random_generator) (long int *), long int * rand_seed,
         double *parameter_initial_final, double *parameter_minimum,
         double *parameter_maximum, double *tangents, double *curvature,
         long int * number_parameters, int *parameter_type,
         int *valid_state_generated_flag, int *exit_status,
         USER_DEFINES * OPTIONS);

#ifdef __cplusplus
}
#endif

extern "C" {

  int setup_parameters( long int *param_dim , USER_DEFINES * USER_OPTIONS );

  double cost_function (double *cost_parameters,
                             double *parameter_lower_bound,
                             double *parameter_upper_bound,
                             double *cost_tangents,
                             double *cost_curvature,
                             long int * parameter_dimension,
                             int *parameter_int_real,
                             int *cost_flag,
                             int *exit_code, USER_DEFINES * USER_OPTIONS);


 int error_function(double *in_x, long int*in_para_dim, 
		    int *in_para_int_real , USER_DEFINES * OPTIONS );

}

//int main (int argc, char **argv);







//   int initialize_parameters (double *cost_parameters,
//                              double *parameter_lower_bound,
//                              double *parameter_upper_bound,
//                              double *cost_tangents,
//                              double *cost_curvature,
//                              long int * parameter_dimension,
//                              int *parameter_int_real,
//                              USER_DEFINES * USER_OPTIONS);


  void reset_options (LONG_INT * rand_seed, USER_DEFINES * USER_OPTIONS);

  double myrand (long int * rand_seed);
  double randflt (long int * rand_seed);
  double resettable_randflt (long int * rand_seed, int reset);
  void Exit_USER (char *statement);


 //  int run_asa_opt( double (*user_cost_function)
//          (double *, double *, double *, double *, double *, long int *,
//           int *, int *, int *, USER_DEFINES *), 
// 	  int (*user_init_parameters)(double *, double *, double *,
//           double *, double *, long int *,int *, USER_DEFINES * ),
// 		   long int number_parameters);

  
  double run_asa_opt( double (*user_cost_function)
         (double *, double *, double *, double *, double *, long int *,
          int *, int *, int *, USER_DEFINES *),  long int number_parameters,
		 int *param_types, double *start_params, double *param_lb , 
                            double *param_ub, USER_DEFINES *USER_OPTIONS);

#endif
