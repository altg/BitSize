#include "myasa.h"

extern "C" {

  int setup_parameters( long int *param_dim , USER_DEFINES * USER_OPTIONS ){

    *param_dim = 13;

    USER_OPTIONS->out_dim = new int;

    *(USER_OPTIONS->out_dim) = 1;
 
    USER_OPTIONS->out_err_spec = new double[*(USER_OPTIONS->out_dim)];

    USER_OPTIONS->out_err_spec[0] =  pow( 2 , -20 );

    return 0;
  }

  int error_function(double *x, long int*in_para_dim, int *in_para_int_real ,USER_DEFINES * USER_OPTIONS )
  {

    double E_x, E_c4, E_c3, E_c2 , E_c1, E_c0, E_dp0, E_dp1, E_dp2;
    double E_dp3, E_dp4, E_dp5, E_dp6; 

    double y[14];

    double *E_out = new double[ *(USER_OPTIONS->out_dim) ];

    E_x = pow( 2 , -x[1 - 1]);
    E_c4 = pow( 2 , -x[2 - 1]);
    E_c3 = pow( 2 , -x[3 - 1]);
    E_c2 = pow( 2 , -x[4 - 1]);
    E_c1 = pow( 2 , -x[5 - 1]);
    E_c0 = pow( 2 , -x[6 - 1]);
    E_dp0 = pow( 2 , -x[7 - 1]);
    E_dp1 = pow( 2 , -x[8 - 1]);
    E_dp2 = pow( 2 , -x[9 - 1]);
    E_dp3 = pow( 2 , -x[10 - 1]);
    E_dp4 = pow( 2 , -x[11 - 1]);
    E_dp5 = pow( 2 , -x[12 - 1]);
    E_dp6 = pow( 2 , -x[13 - 1]);

    double E_y = 0;

    y[1 - 1] = fabs( ( 1.74187 * E_x + 1 * ( 0.745327 * E_x + 1 * ( 0.277485 * E_x + 1 * 0.0565797 * E_x ) ) ) );
    y[2 - 1] = fabs( 1 * 1 * 1 * 1 * E_c4 );
    y[3 - 1] = fabs( 1 * 1 * 1 * E_c3 );
    y[4 - 1] = fabs( 1 * 1 * E_c2 );
    y[5 - 1] = fabs( 1 * E_c1 );
    y[6 - 1] = fabs( E_c0 );
    y[7 - 1] = fabs( 1 * 1 * 1 * E_dp0 );
    y[8 - 1] = fabs( 1 * 1 * 1 * E_dp1 );
    y[9 - 1] = fabs( 1 * 1 * E_dp2 );
    y[10 - 1] = fabs( 1 * 1 * E_dp3 );
    y[11 - 1] = fabs( 1 * E_dp4 );
    y[12 - 1] = fabs( 1 * E_dp5 );
    y[13 - 1] = fabs( E_dp6 );
    y[14 - 1] = fabs( E_y );

    E_out[0] = 0;
  

    for (int index = 0; index <  14; ++index)
      E_out[0] += y[index];  


    //cout << E_out[0] << " : " <<  USER_OPTIONS->out_err_spec[0] << endl;

    if ( E_out[0] > USER_OPTIONS->out_err_spec[0] ){

      return 0;

    }
    else{
    
      return 1;

    }

  }

  double cost_mul( int range_bw_A , double prec_bw_A , int range_bw_B , double prec_bw_B , int range_bw_OUT , double prec_bw_OUT ){


    return ( range_bw_A + prec_bw_A ) * (  range_bw_B +  prec_bw_B );


  }


  double cost_addsub( int range_bw_A , double prec_bw_A , int range_bw_B , double prec_bw_B , int range_bw_OUT , double prec_bw_OUT ){

    double tot_A , tot_B, tot_OUT;

    tot_A = range_bw_A + prec_bw_A; 
    tot_B = range_bw_B + prec_bw_B; 
    tot_OUT = range_bw_OUT + prec_bw_OUT; 
    return max( tot_A , tot_B);


  }

  double cost_function(double *x,
		       double *parameter_lower_bound,
		       double *parameter_upper_bound,
		       double *cost_tangents,
		       double *cost_curvature,
		       long int * parameter_dimension,
		       int *parameter_int_real,
		       int *cost_flag, int *exit_code, USER_DEFINES * USER_OPTIONS)
  {

    *cost_flag = error_function( x , parameter_dimension , parameter_int_real , USER_OPTIONS );


    double *Cost;

    if ( *cost_flag ){

      Cost = new double[8];

      Cost[0] = cost_mul( 2 , x[1], 2 , 1 + x[0] , 2 , 0. );
      Cost[1] = cost_addsub( 2 , x[6] , 2 , x[2] , 2 , x[7] );
      Cost[2] = cost_mul( 1 , x[0] , 2 , x[7] , 2 , x[8] );
      Cost[3] = cost_addsub( 2 , x[8] , 2 , x[3] , 2 , x[9] );
      Cost[4] = cost_mul( 1 , x[0] , 2 , x[9] , 2 , x[10] );
      Cost[5] = cost_addsub( 2 , x[10] , 2 , x[4] , 2 , x[11] );
      Cost[6] = cost_mul( 1 , x[0] , 2 , x[11] , 2 , x[12] );
      Cost[7] = cost_addsub( 2 , x[12] , 2 , x[5] , 2 , 0 );

      double Total_Cost = 0;

      for (int index = 0; index <  8; ++index)
	Total_Cost += Cost[index];  

      return Total_Cost;


    }
    else{

      return 0;


    }
    

  }

}
