#include "optengine.h"


OptEngine::OptEngine():ubw_cost(0),final_cost(0),start_bw(40.0),upper_limit(5.),lower_limit(5.){

  stringstream ss1;


  // if ( ostype == "cygwin" ){

//   ss1 << "g++ -c -g -Wall ";

//   }
//   else{
  ss1 << "g++ -c -g -Wall ";

  //ss1 << "g++  -fPIC -c -g -Wall ";

 //  }

  //ss1 << "-I " << getenv( "ASA_HOME" ) << " -o cost_func.o cost_func.cpp"; 

  string asa_home = getenv( "ASA_HOME" );

  //string asa_home = "/homes/amag97/Projects/bitsize/MyASA";

  // /homes/amag97/Projects/ASA/MyASA

  ss1 << "-I " << asa_home << " -o cost_func.o cost_func.cpp"; 

  //cout << "Compiling \n" << ss1.str() << endl;

  cout << "Starting Optimization ";

  if ( system( ss1.str().c_str() ) != 0 ) {

    cerr << "Compilation Error !!\n";

    exit(-1);

  };

  string s2; 

  s2 =  "g++ -shared -Wl,-soname,libcostfunc.so -o libcostfunc.so cost_func.o";
  
  //cout << "Creating Shared lib\n" << s2 << endl;

  cout << "...";

  if ( system( s2.c_str() ) != 0 ){

    cerr << "Error Creating shared library !!\n";

    exit(-1);

  };

}


OptEngine::~OptEngine(){

  // cout << "OptEngine Destructor Called \n";

  delete cost_params;
  delete number_of_params;

}

void OptEngine::run_optimization(){

  
  void* theLib = dlopen("./libcostfunc.so", RTLD_LAZY);
  if (!theLib)
    {
      cout << dlerror() << "\n";
      exit(-1);
    }
  //cout << "Cost Function Library opened!\n";
  cout << " .";


  number_of_params = new long int;

  USER_DEFINES *USER_OPTIONS;// = new USER_DEFINES;

  USER_OPTIONS = new USER_DEFINES;

  double *param_lb, *param_ub;

  int *param_type;

  long int index;


  ParamFunc theParamFunc = (ParamFunc)dlsym(theLib , "setup_parameters" );
  
  if (!theParamFunc)
    {
      cout << dlerror() << "\n";
      exit(-1);
    }
  //cout << "Parameter Function located!\n";
  cout << " .";

  (*theParamFunc)( number_of_params , USER_OPTIONS );


  param_lb = new double[ *number_of_params ];
  
  param_ub = new double[ *number_of_params ];

  cost_params  = new double[ *number_of_params ];

  param_type = new int[ *number_of_params ];

 

  for (index = 0; index <  *number_of_params; ++index)
    cost_params[index] = start_bw;
 
  for (index = 0; index <  *number_of_params; ++index)
    param_type[index] = ((int) 1);  


  ErrFunc theErrFunc = (ErrFunc)dlsym(theLib, "error_function");
  if (!theErrFunc)
    {
      cout << dlerror() << "\n";
      exit(-1);
    }
  //cout << "Error Function located!\n";
  cout << " .\n";

  cout << "Finding uniform bit-width solution ";

  
  while( (*theErrFunc)( cost_params , number_of_params , param_type , USER_OPTIONS ) ){

    for (index = 0; index <  *number_of_params; ++index)
      cost_params[index]--;
    
    
  }

  for (index = 0; index <  *number_of_params; ++index)
    cost_params[index]++;


  uni_bw =  cost_params[0];

  cout << "\nUniform BW : " << uni_bw  << endl;


  double *cost_tan, *cost_curve;

  //  int *cost_flag , *exit_code;

  int *cost_flag = new int;

  int *exit_code = new int;

  CostFunc theCostFunc = (CostFunc) dlsym(theLib, "cost_function");

  cout << "Starting SA based optimization ";

  if (!theCostFunc)
	{
		cout << dlerror() << "\n";
		exit(-1);
	}

  cout << " ...\n";
  //cout << "Cost Function located!\n";

  ubw_cost = (*theCostFunc)( cost_params , param_lb , param_ub , cost_tan , cost_curve , number_of_params , param_type , cost_flag , exit_code , USER_OPTIONS );
 

  cout << "Starting Cost function value :" << ubw_cost << endl;


  final_cost = 0;

  
  for (index = 0; index <  *number_of_params; index++)
    param_lb[index] = uni_bw - lower_limit;
  for (index = 0; index <  *number_of_params; index++)
    param_ub[index] = uni_bw + upper_limit;
  
  //cout << "Done Alloc\n";

  cout << "Running SA optimization ... "; 

  final_cost = run_asa_opt( theCostFunc , *number_of_params , param_type , cost_params , param_lb , param_ub , USER_OPTIONS );

  cout << "done\n";

  cout << "Final Cost function value : " << final_cost << endl;

  
  cout << "Allocated signal bit-widths : \n";

  for (index = 0; index <  *number_of_params; ++index){
    cout << index << " :\t " << bs->param_name_map[ index ]->edge_name << " <- ";
    cout << cost_params[index] << endl;

  }

 

}


void OptEngine::update_edges( vector<Edge *>& analyzed_edges )
{

  Edge *CEdge;

  if ( bs->mydag->arith_type == FIXED ){

    HWfix *temp;
  
    for (int index = 0; index <  *number_of_params; ++index){

      CEdge = analyzed_edges[ index ];

      if( temp = dynamic_cast<HWfix *>( CEdge->my_var ) ){
	temp->frac_bw = static_cast<fsize>(cost_params[ index ]);
	if ( temp->int_bw_state == NOTSET ) temp->int_bw = static_cast<fsize>(bs->range_analysis->range_bw_fx(CEdge));
      }
    }

  }
  else if (  bs->mydag->arith_type == FLOAT ){

    HWfloat *temp;
  
    for (int index = 0; index <  *number_of_params; ++index){

      CEdge = analyzed_edges[ index ];

      if( temp = dynamic_cast<HWfloat *>( CEdge->my_var ) ){
	temp->man_bw = static_cast<fsize>(cost_params[ index ]);
	if ( temp->exp_bw_state == NOTSET ) temp->exp_bw = static_cast<fsize>(bs->range_analysis->range_bw_flt(CEdge));
      }
    }
  }
  else if (  bs->mydag->arith_type == LNS ){

    HWlns *temp;
    
    
    for (int index = 0; index <  *number_of_params; ++index){

      CEdge = analyzed_edges[ index ];

      if( temp = dynamic_cast<HWlns *>( CEdge->my_var ) ){
	temp->frac_bw = static_cast<fsize>(cost_params[ index ]);
	if ( temp->int_bw_state == NOTSET ) temp->int_bw = static_cast<fsize>(bs->range_analysis->range_bw_lns(CEdge));
      }
    }


  }

}


void OptEngine::update_edges_uni( vector<Edge *>& analyzed_edges  )
{

  Edge *CEdge;

  if ( bs->mydag->arith_type == FIXED ){

    HWfix *temp;
  
    for (int index = 0; index <  *number_of_params; ++index){

      CEdge = analyzed_edges[ index ];

      if( temp = dynamic_cast<HWfix *>( CEdge->my_var ) ){
	temp->frac_bw = static_cast<fsize>(uni_bw);
	if ( temp->int_bw_state == NOTSET ) temp->int_bw = static_cast<fsize>(bs->range_analysis->range_bw_fx(CEdge));
      }
    }
  }
  else if (  bs->mydag->arith_type == FLOAT ){

    HWfloat *temp;
  
    for (int index = 0; index <  *number_of_params; ++index){

      CEdge = analyzed_edges[ index ];

      if( temp = dynamic_cast<HWfloat *>( CEdge->my_var ) ){
	temp->man_bw = static_cast<fsize>(uni_bw);
	if ( temp->exp_bw_state == NOTSET ) temp->exp_bw = static_cast<fsize>(bs->range_analysis->range_bw_flt(CEdge));
      }
    }


  }
  else if (  bs->mydag->arith_type == LNS ){

    HWlns *temp;
    
    
    for (int index = 0; index <  *number_of_params; ++index){

      CEdge = analyzed_edges[ index ];

      if( temp = dynamic_cast<HWlns *>( CEdge->my_var ) ){
	temp->frac_bw = static_cast<fsize>(uni_bw);
	if ( temp->int_bw_state == NOTSET ) temp->int_bw = static_cast<fsize>(bs->range_analysis->range_bw_lns(CEdge));
      }
    }


  }


}
  


void OptEngine::print_opt_summary( ostream &os )
{


  
  switch( bs->mydag->arith_type ){

  case FIXED : os << "FIX : "; break;

  case FLOAT : os << "FLT : "; break;

  case LNS : os << "LNS : "; break;

  }

  os << ubw_cost << " : " << final_cost << " : ";

  os << 100.*(ubw_cost - final_cost ) / ubw_cost << " : ";
  
  switch(  bs->range_ann_mode ){

  case bitsize::AFFINE_RANGE : os << "affine :";break;

  case bitsize::SIM_RANGE : os << "sim :"; break;

  }
  
  switch(  bs->prec_ann_mode ){

  case bitsize::AFFINE_PREC_CPP : os << "affine ";break;

  case bitsize::AD_PREC_CPP : os << "ad " ; break;

  }

  os << endl;

}
