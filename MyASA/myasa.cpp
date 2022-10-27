#include "myasa.h"

char user_exit_msg[160];
FILE *ptr_out;

double randflt (long int * rand_seed);

double resettable_randflt (long int * rand_seed, int reset);



#define MULT ((LONG_INT) 25173)
#define MOD ((LONG_INT) 65536)
#define INCR ((LONG_INT) 13849)
#define FMOD ((double) 65536.0)

double myrand (long int * rand_seed)
{
#if TRUE                 /* (change to FALSE for alternative RNG) */
  *rand_seed = (LONG_INT) ((MULT * (*rand_seed) + INCR) % MOD);
  return ((double) (*rand_seed) / FMOD);
#else
  /* See "Random Number Generators: Good Ones Are Hard To Find,"
     Park & Miller, CACM 31 (10) (October 1988) pp. 1192-1201.
     ***********************************************************
     THIS IMPLEMENTATION REQUIRES AT LEAST 32 BIT INTEGERS
     *********************************************************** */
#define _A_MULTIPLIER  16807L
#define _M_MODULUS     2147483647L      /* (2**31)-1 */
#define _Q_QUOTIENT    127773L  /* 2147483647 / 16807 */
#define _R_REMAINDER   2836L    /* 2147483647 % 16807 */
  long lo;
  long hi;
  long test;

  hi = *rand_seed / _Q_QUOTIENT;
  lo = *rand_seed % _Q_QUOTIENT;
  test = _A_MULTIPLIER * lo - _R_REMAINDER * hi;
  if (test > 0) {
    *rand_seed = test;
  } else {
    *rand_seed = test + _M_MODULUS;
  }
  return ((double) *rand_seed / _M_MODULUS);
#endif /* alternative RNG */
}



void Exit_USER (char *statement)
{

  printf ("\n\n*** EXIT calloc failed *** %s\n\n", statement);
}


double resettable_randflt (long int * rand_seed, int reset)

{
  double rranf;
  unsigned kranf;
  int n;
  static int initial_flag = 0;
  long int initial_seed;



  static double random_array[256];


  if (*rand_seed < 0)
    *rand_seed = -*rand_seed;

  if ((initial_flag == 0) || reset) {
    initial_seed = *rand_seed;

    for (n = 0; n < 256; ++n)
      random_array[n] = myrand (&initial_seed);

    initial_flag = 1;

    for (n = 0; n < 1000; ++n)
      rranf = randflt (&initial_seed);

    rranf = randflt (rand_seed);

    return (rranf);
  }

  kranf = (unsigned) (myrand (rand_seed) * 256) % 256;
  rranf = *(random_array + kranf);
  *(random_array + kranf) = myrand (rand_seed);

  return (rranf);
}


double randflt (long int * rand_seed)
{
  return (resettable_randflt (rand_seed, 0));
}




/* Library main function */

double run_asa_opt( double (*user_cost_function)
         (double *, double *, double *, double *, double *, long int *,
          int *, int *, int *, USER_DEFINES *),  long int number_parameters,
		 int *param_types,
		 double *start_params, double *param_lb , double *param_ub, USER_DEFINES *USER_OPTIONS)
{
  int *exit_code;
  long int n_param;


  //int compile_cnt;
  double *parameter_lower_bound, *parameter_upper_bound, *cost_parameters,
    *cost_tangents, *cost_curvature;
  double cost_value;

  int initialize_parameters_value;


  long int *parameter_dimension;


  int *parameter_int_real;


  int *cost_flag;


  long int *rand_seed;

  // USER_DEFINES *USER_OPTIONS;

  if (!strcmp ("asa_usr_out", "STDOUT")) {

    ptr_out = stdout;

  } else {
    ptr_out = fopen ("asa_usr_out", "w");
  }
  fprintf (ptr_out, "%s\n\n", "/* test 2 */");




 /*  if (argc > 1) { */
/*     fprintf (ptr_out, "CC = %s\n", argv[1]); */
/*     for (compile_cnt = 2; compile_cnt < argc; ++compile_cnt) { */
/*       fprintf (ptr_out, "\t%s\n", argv[compile_cnt]); */
/*     } */
/*     fprintf (ptr_out, "\n"); */
/*   } */


/*   fflush (ptr_out); */


 
 
  rand_seed = new long int;

  parameter_dimension = new long int;

  exit_code = new int;

  cost_flag = new int;

  

  reset_options( rand_seed , USER_OPTIONS );


  resettable_randflt (rand_seed, 1);


  /*  Number of Parameters */
  *parameter_dimension = number_parameters;
 
 

  parameter_lower_bound = param_lb;

  
 

  parameter_upper_bound = param_ub;

 
  cost_parameters = start_params;

 

  parameter_int_real = param_types; 

  cost_tangents =  new double[ *parameter_dimension ];


  if (USER_OPTIONS->Curvature_0 == 0 || USER_OPTIONS->Curvature_0 == -1) {

    if ((cost_curvature =
         (double *) calloc ((*parameter_dimension) *
                            (*parameter_dimension),
                            sizeof (double))) == ((void *)0)) {
      strcpy (user_exit_msg, "main()/asa_main(): cost_curvature");
      Exit_USER (user_exit_msg);
      return (-2);
    }
  } else {
    cost_curvature = (double *) ((void *)0);
  }

 /*  initialize_parameters_value = (*user_init_parameters) (cost_parameters, */
/*                                                        parameter_lower_bound, */
/*                                                        parameter_upper_bound, */
/*                                                        cost_tangents, */
/*                                                        cost_curvature, */
/*                                                        parameter_dimension, */
/*                                                        parameter_int_real, */
/*                                                        USER_OPTIONS); */



  if (initialize_parameters_value == -2)
    return (initialize_parameters_value);

 

  //printf( "R: %ld \n" , *rand_seed  );

  /* Call to ASA optimization */

     cost_value =
        asa (user_cost_function,
             randflt,
             rand_seed,
             cost_parameters,
             parameter_lower_bound,
             parameter_upper_bound,
             cost_tangents,
             cost_curvature,
             parameter_dimension,
             parameter_int_real, cost_flag, exit_code, USER_OPTIONS);
   
    if (*exit_code == -1) {

        printf ("\n\n*** error in calloc in ASA ***\n\n");

        fprintf (ptr_out, "\n\n*** error in calloc in ASA ***\n\n");
        fflush (ptr_out);
        return (-1);
      }
      fprintf (ptr_out, "exit code = %d\n", *exit_code);
      fprintf (ptr_out, "final cost value = %12.7g\n", cost_value);
      fprintf (ptr_out, "parameter\tvalue\n");
      for (n_param = 0; n_param < *parameter_dimension; ++n_param) {
        fprintf (ptr_out,
                "%ld\t\t%12.7g\n",
                 n_param, cost_parameters[n_param]);
      }
  fclose (ptr_out);

 //  printf( "Final Cost %f\nParameters \n" , cost_value );

//    for (n_param = 0; n_param < *parameter_dimension; ++n_param) {
//         fprintf (stdout,"%ld\t\t%12.7g\n", n_param, cost_parameters[n_param]);
//       }

  

  if (USER_OPTIONS->Curvature_0 == 0 || USER_OPTIONS->Curvature_0 == -1)
    free (cost_curvature);

  //delete USER_OPTIONS;
  //delete parameter_dimension;
  delete exit_code;
  delete cost_flag;
  // delete parameter_lower_bound;
  //delete parameter_upper_bound;
  //delete cost_parameters;
  //delete parameter_int_real;
  delete cost_tangents;
  delete rand_seed;

  return cost_value;

}


void reset_options (LONG_INT * rand_seed, USER_DEFINES * USER_OPTIONS)
{
  *rand_seed = 696969;
  /**test_in_cost_func = 0;
   *use_rejected_cost = FALSE;*/
  /* USER_OPTIONS->Limit_Acceptances = 10000; */
  USER_OPTIONS->Limit_Acceptances = 1000;
  USER_OPTIONS->Limit_Generated = 99999;
  USER_OPTIONS->Limit_Invalid_Generated_States = 1000;
  /* USER_OPTIONS->Accepted_To_Generated_Ratio = 1.0E-6; */
  USER_OPTIONS->Accepted_To_Generated_Ratio = 1.0E-4;

  USER_OPTIONS->Cost_Precision = 1.0E-18;
  USER_OPTIONS->Maximum_Cost_Repeat = 5;
  USER_OPTIONS->Number_Cost_Samples = 5;
  USER_OPTIONS->Temperature_Ratio_Scale = 1.0E-5;
  USER_OPTIONS->Cost_Parameter_Scale_Ratio = 1.0;
  USER_OPTIONS->Temperature_Anneal_Scale = 100.0;

  USER_OPTIONS->Include_Integer_Parameters = FALSE;
  USER_OPTIONS->User_Initial_Parameters = FALSE;
  USER_OPTIONS->Sequential_Parameters = -1;
  USER_OPTIONS->Initial_Parameter_Temperature = 1.0;

  USER_OPTIONS->Acceptance_Frequency_Modulus = 100;
  USER_OPTIONS->Generated_Frequency_Modulus = 10000;
  USER_OPTIONS->Reanneal_Cost = 1;
  USER_OPTIONS->Reanneal_Parameters = TRUE;

  USER_OPTIONS->Delta_X = 0.001;
  USER_OPTIONS->User_Tangents = FALSE;
  USER_OPTIONS->Curvature_0 = FALSE;

 
/*   if ((USER_OPTIONS->Asa_Out_File = */
/*        (char *) calloc (25, sizeof (char))) == ((void *)0)) { */
/*     strcpy (user_exit_msg, "main()/asa_main(): USER_OPTIONS->Asa_Out_File"); */
/*     Exit_USER (user_exit_msg); */
/*     return (-2); */
/*   } */

  //strcpy (USER_OPTIONS->Asa_Out_File, "asa.log");
}

