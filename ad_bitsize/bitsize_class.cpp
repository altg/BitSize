#include "bitsize_class.h"
#include "dag.h"
#include <cstdlib>

#include "CmdLine.h"

int def_opt_mode = 1;

bitsize::bitsize( int argc , char *argv[]  )
{
  
   parse_arguments( argc , argv );

   mydag = new DAG();

   myarea = new AreaModel( mydag );

   mynode_sched = new NodeSchedular( mydag );



   switch( range_ann_mode ){

   case AFFINE_RANGE : range_analysis = new AffineRange(  mydag , mynode_sched ); break;

   case INTERVAL_RANGE :  range_analysis = new  IntervalRange(  mydag , mynode_sched ); break;

   case SIM_RANGE :  range_analysis = new SimRange(  mydag , mynode_sched );  break;

   case NOOPT_RANGE : break;

   }


   
   init_passes();
  

   cout << endl;
   cout << "Welcome to BitSize - BitWidth Optimisation System" << endl;
   #if defined(__GNUC__)
   cout << "Compiled with GCC " << __GNUC__<<"."<< __GNUC_MINOR__ << " on " << __DATE__ << " at " << __TIME__ << endl;
   #endif
   cout << "-------------------------------------------------" << endl;
   cout << endl;
 


}


void bitsize::bitsize_main(){

  run_passes_2();


}

void bitsize::init_passes()
{

 // range_analysis = new SimRange(  mydag , mynode_sched );

   myerror = new ErrorModel( mydag , mynode_sched , range_analysis);


   errmodel_affine = new ErrorModel_Affine( mydag , mynode_sched );
   errmodel_affine2 = new ErrorModel_Affine2( mydag , mynode_sched );

   errmodel_affine_cpp = new ErrorModel_Affine_CPP(  mydag , mynode_sched );

   errmodel_affine_flt_cpp = new ErrModel_Affine_FLT_CPP(  mydag , mynode_sched );

   errmodel_affine_lns_cpp = new ErrModel_Affine_LNS_CPP(  mydag , mynode_sched );
   
  
   cost_func = new CostFunction( mydag );

   cost_func_cpp = new CostFunctionAreaCPP( mydag );

   cost_func_flt_cpp = new CostFuncAreaFLTCPP( mydag );

   cost_func_lns_cpp = new CostFuncAreaLNSCPP( mydag );


   cost_func_pwr = new CostFunctionPWR( mydag );

   asc_backend = new ASCBackend( mydag , mynode_sched );


   ad_dag = new ADDAG( mydag , mynode_sched );

   opt_flt = new OptBW_flt(  mydag , ad_dag ,  myarea);

   opt_fx = new OptBW_fx(  mydag , ad_dag ,  myarea);

   opt_cpp = new OptBW_cpp( mydag , ad_dag );

   optflt_cpp = new OptBWFLT_cpp( mydag , ad_dag );

   optlns_cpp = new OptBWLNS_cpp( mydag , ad_dag );

   pwr_opt_datapass = new PwrDataPass( mydag , mynode_sched ); 

   pwr_pass_glitch = new PwrPassGlitch( mydag , mynode_sched ); 


   result_gen = new BWResultsGen( mydag );




}


void bitsize::show_help()
{

  cout << "BitSize Help" << endl;
  
  cout << "------------" << endl;

  cout << "stream.exe -r [affine|sim|interval|no_opt] -p [affine|autodiff|no_opt]";

  cout << " -o [nouni|uni] -v [0|1|2] [--testgen]\n";


}


void bitsize::setup_defaults()
{


  range_ann_mode = AFFINE_RANGE;

  prec_ann_mode = AD_PREC;

  opt_mode = def_opt_mode;

}

void bitsize::parse_arguments( int argc , char *argv[] )
{

  CCmdLine cmdLine;


  if (cmdLine.SplitLine(argc, argv) < 1)
    {
      cout << "BitSize using default options " << endl;

      setup_defaults();
      
      return;

    }

  if (cmdLine.HasSwitch("-h"))
    {
      show_help();
      exit(0);
    }

  


  StringType r1_0;

  
  r1_0 = cmdLine.GetSafeArgument("-r", 0, "affine");

  
  if ( r1_0 == "affine" ){
    
    range_ann_mode = AFFINE_RANGE;
  }
  else if ( r1_0 == "sim" ){

    range_ann_mode = SIM_RANGE;
  }
  else if ( r1_0 == "interval" ){

    range_ann_mode = INTERVAL_RANGE;

  }
  else if ( r1_0 == "no_opt" ){

    range_ann_mode = NOOPT_RANGE;

  }
  else {

    cerr << "Range mode " << r1_0 << " not found!!\n";

    cerr << "Valid modes: affine|sim|interval|no_opt" << endl;

    exit( -1 );


  }
  
  StringType p1_0;


  
  p1_0 = cmdLine.GetSafeArgument("-p", 0, "autodiff");
  
  if ( p1_0 == "affine" ){
    
    prec_ann_mode = AFFINE_PREC;
  }
  else if ( p1_0 == "affine2" ){ 

    prec_ann_mode = AFFINE_PREC2;
  }
  else if ( p1_0 == "affine_cpp" ){

     prec_ann_mode = AFFINE_PREC_CPP;

  } 
  else if ( p1_0 == "autodiff" ){

    prec_ann_mode = AD_PREC;
    
    if ( cmdLine.HasSwitch("--error_func_only") ){
      ad_err_func_only = true;

    }else{

      ad_err_func_only = false;
    }

  }
  else if ( p1_0 == "ad_cpp" ){

    prec_ann_mode = AD_PREC_CPP;

    //opt_mode = NO_OPT;

  }
  else if ( p1_0 == "no_opt" ){

    prec_ann_mode = NOOPT_PREC;


  }
  else {

    cerr << "Prec mode - '" << p1_0 << "' not valid!!\n";

    cerr << "Valid modes: affine|autodiff|no_opt" << endl;

    exit( -1 );


  }


  StringType p2_0;

  p2_0 = cmdLine.GetSafeArgument("-o", 0, "nonuni");

  if ( p2_0 == "uni" ){

    bw_mode = UNIFORM;

  }else if ( p2_0 == "nonuni" ){

    bw_mode = NONUNIFORM;

  }
  else{

    cerr << "Opt Mode - '" << p2_0 << "' not valid!!\n";

    cerr << "Valid modes: nouni|uni" << endl;

    exit( -1 );


  }


  StringType p3_0;

  p3_0  = cmdLine.GetSafeArgument("-m", 0 , "area");

  
  if ( p3_0 == "area" )
    {

      opt_metric = AREA_OPT;


    }
  else if ( p3_0 == "power" )
    {


      opt_metric = POWER_OPT;


    }
  else if ( p3_0 == "power_g" )
    {


      opt_metric = POWER_OPT_GLITCH;


    }
  else if ( p3_0 == "area_cpp" )
    {

      opt_metric = AREA_OPT_CPP;


    }
  
  else
    {

      cerr << "Opt Metric - '" << p3_0 << "' not valid!!\n";

      cerr << "Valid metrics: area|power|power_g" << endl;

      exit( -1 );


    }
  
  string ilp_ub_str;

  if ( cmdLine.HasSwitch("-ilp") )
    {
      ilp_gen = true;

      ilp_ub_str = cmdLine.GetSafeArgument("-ilp", 0, "50"); 

      ilp_ub_val = atoi( ilp_ub_str.c_str());

    }
  else
    {
      ilp_gen = false;

    }
  
  if ( cmdLine.HasSwitch("-vhdlgen") )
    {
      vhdl_gen = true;

      bwdata_file  = cmdLine.GetSafeArgument("-bwf", 0 , "bitsize_data.dat");

    }
  else{

    vhdl_gen = false;

  }

  

  if ( cmdLine.HasSwitch("-nobopt") )
    {

      do_bit_opt = false;

    }
  else
    {

      do_bit_opt = true;

    }


  if ( cmdLine.HasSwitch("-d") ){
    
    global_dbgmode = L1;

  }


  if ( cmdLine.HasSwitch("-R") ){

    run_time_args = true;


    num_r_args = cmdLine.GetArgumentCount( "-R" );

    for( int ctr = 0; ctr < num_r_args ; ctr++ ){

      r_args.push_back( cmdLine.GetArgument( "-R" , ctr ) );

    }

  }
  else{

    run_time_args = false;

  }
  

  // Check for verbosity

  StringType v_1;
  if ( cmdLine.HasSwitch("-v") ){
    
    v_1 = cmdLine.GetSafeArgument("-v", 0, "0"); 

    db_level = atoi( v_1.c_str() );
  }  

  if (cmdLine.HasSwitch("--testgen"))
    mat_code_gen = true;
  else
    mat_code_gen = false;

  opt_mode = def_opt_mode;

}

void bitsize::start_parse()
{

  global_opmode = PARSING;
  
}

void bitsize::end_parse()
{
  
  global_opmode = PARSED;

}


double bitsize::get_area()
{

  return myarea->get_area();

}


// Power pass
void bitsize::run_passes_pwr()
{
  
  (*pwr_opt_datapass)();

  for( vector<Edge *>::iterator e_itr = mydag->edge_vec.begin(); e_itr != mydag->edge_vec.end(); e_itr++ )
    {
      (*e_itr)->data_log.cal_values();

      cout << (*e_itr)->edge_name << " " << (*e_itr)->data_log.mean << " :: ";
      cout << (*e_itr)->data_log.var << " :: " << (*e_itr)->data_log.acoef_1 << endl;


    }

  (*range_analysis)();
  
 
  run_fixedpoint_error();

  fstream cost_fs( "cost_function_pwr.m" , ios::out );
  cost_func_pwr->gen_matlab_costf( cost_fs , "error_function" );
  cost_fs.close();


  fstream param_fs( "parameters.m" , ios::out );
  cost_func_pwr->gen_parameters_func( param_fs );
  param_fs.close();



}


void bitsize::run_passes_pwr_glitch()
{
  
  (*pwr_opt_datapass)();

 
  fstream cost_fs( "cost_func_pwrglitch.m" , ios::out );
  (*pwr_pass_glitch)( cost_fs );
  cost_fs.close();

  fstream param_fs( "parameters.m" , ios::out );
  pwr_pass_glitch->gen_parameters_func( param_fs );
  param_fs.close();

}


// A single method to invoke the passes.

void bitsize::run_fixedpoint_error()
{

   if ( prec_ann_mode == AFFINE_PREC ){

      errmodel_affine->do_affine_analysis();
    
      errmodel_affine->show_affine_vecs();

      fstream fs( "error_function.m" , ios::out );
      errmodel_affine->gen_matlab_err_func(fs );  
      fs.close();

     
    }
    else if ( prec_ann_mode == AFFINE_PREC2 ){
      errmodel_affine2->do_affine_analysis();
      errmodel_affine2->show_affine_vecs();

      fstream fs( "error_function_af2.m" , ios::out );
      errmodel_affine2->gen_matlab_err_func(fs );  
      fs.close();
  
    }
    else if ( prec_ann_mode == AFFINE_PREC_CPP ){

      errmodel_affine_cpp->do_affine_analysis();
      //errmodel_affine_cpp->show_affine_vecs();

      fstream fs( "cost_func.cpp" , ios::out );
      
      fs << "#include \"myasa.h\"\n";
      fs << "\n\nextern \"C\" { \n\n";
      errmodel_affine_cpp->gen_matlab_err_func(fs );  
      fs.close();



    }
    else if ( prec_ann_mode == AD_PREC_CPP ){
      
      opt_cpp->run_adanalysis( cout );

      range_analysis->show_range_vecs();

      fstream fs( "cost_func.cpp" , ios::out );
      fs << "#include \"myasa.h\"\n";
      fs << "\n\nextern \"C\" { \n\n";
      opt_cpp->gen_error_func( fs  );
      fs.close();
    }
    else 
      {

	

	INTERNAL_ASSERT_EXIT( 1 , "Invalid Internal Prec Mode" )



      }
   
   gen_mat_testcode_fix();

}

void bitsize::run_fltpoint_error()
{

  if ( prec_ann_mode == AFFINE_PREC_CPP ){

    errmodel_affine_flt_cpp->do_affine_analysis();
      //errmodel_affine_cpp->show_affine_vecs();

    fstream fs( "cost_func.cpp" , ios::out );
      
    fs << "#include \"myasa.h\"\n";
    fs << "\n\nextern \"C\" { \n\n";
    errmodel_affine_flt_cpp->gen_matlab_err_func(fs );
    fs.close();

  }
  else if ( prec_ann_mode == AD_PREC_CPP ){


    optflt_cpp->run_adanalysis( cout );

    fstream fs( "cost_func.cpp" , ios::out );
    fs << "#include \"myasa.h\"\n";
    fs << "\n\nextern \"C\" { \n\n";
    optflt_cpp->gen_error_func( fs  );
    fs.close();
  
  }
  else 
    {

	

      INTERNAL_ASSERT_EXIT( 1 , "Invalid Internal Prec Mode" )



	}





  if( mat_code_gen ){
    bs_pass = new MatCodeGenFlt(  mydag , mynode_sched );
    fstream mat_fs( "mat_test.m" , ios::out );
    (*bs_pass)(mat_fs);
    mat_fs.close();
  }

}


void bitsize::run_lns_error()
{

  if ( prec_ann_mode == AFFINE_PREC_CPP ){

   errmodel_affine_lns_cpp->do_affine_analysis();
      //errmodel_affine_cpp->show_affine_vecs();

   fstream fs( "cost_func.cpp" , ios::out );
      
   fs << "#include \"myasa.h\"\n";
   fs << "\n\nextern \"C\" { \n\n";
   errmodel_affine_lns_cpp->gen_matlab_err_func(fs );
   fs.close();

   
  }
  else if ( prec_ann_mode == AD_PREC_CPP ){


    optlns_cpp->run_adanalysis( cout );

    fstream fs( "cost_func.cpp" , ios::out );
    fs << "#include \"myasa.h\"\n";
    fs << "\n\nextern \"C\" { \n\n";
    optlns_cpp->gen_error_func( fs  );
    fs.close();

     }
  else 
    {

	

      INTERNAL_ASSERT_EXIT( 1 , "Invalid Internal Prec Mode" )


   }





  if( mat_code_gen ){
    bs_pass = new MatCodeGenFlt(  mydag , mynode_sched );
    fstream mat_fs( "mat_test.m" , ios::out );
    (*bs_pass)(mat_fs);
    mat_fs.close();
  }

}



void bitsize::run_passes_2()
{

 

  if ( do_bit_opt ){

    (*range_analysis)();


    if ( mydag->arith_type == FIXED ){

      run_fixedpoint_error();


    }
    else if  ( mydag->arith_type == FLOAT ){

      run_fltpoint_error();

      //(*opt_flt)( cout );
      //(*opt_flt)( cout );
     
    }
    else if (  mydag->arith_type == LNS ){

      run_lns_error();

    }

  }


  if ( ilp_gen )  gen_ilp_code();

  if ( vhdl_gen ) gen_vhdlcode();

  if ( opt_metric == POWER_OPT ){

    run_passes_pwr();


  }
  else if ( opt_metric == POWER_OPT_GLITCH ){

    run_passes_pwr_glitch();

  }
  else if ( opt_metric == AREA_OPT ){

    fstream cost_fs( "cost_func_area.m" , ios::out );
    cost_func->gen_matlab_costf( cost_fs , "error_function" );
    cost_fs.close();


    //  fstream param_fs( "parameters.m" , ios::out );
    //     cost_func->gen_parameters_func( param_fs );
    //     param_fs.close();


  }else if ( opt_metric == AREA_OPT_CPP ){

    fstream optsum_fs;

    

    if ( mydag->arith_type == FIXED ){

      
      fstream cost_fs( "cost_func.cpp" , ios::out | ios::app );
      cost_func_cpp->gen_costf( cost_fs , "error_function" );
    
      cost_func_cpp->gen_parameters_func( cost_fs );
      cost_fs << "\n}\n";
      cost_fs.close();

      OptEngine *myopt = new OptEngine();
     
      myopt->run_optimization();

      if ( prec_ann_mode == AFFINE_PREC_CPP )
	myopt->update_edges_uni( errmodel_affine_cpp->analyzed_edges );
      else if ( prec_ann_mode == AD_PREC_CPP )
	opt_cpp->update_edges_uni( myopt->uni_bw );

      fstream bwdata_fs( "bitsize_uni.dat" , ios::out );
      result_gen->gen_bitsize_dat( bwdata_fs );
      bwdata_fs.close();

      if ( prec_ann_mode == AFFINE_PREC_CPP )
	myopt->update_edges( errmodel_affine_cpp->analyzed_edges );
      else if ( prec_ann_mode == AD_PREC_CPP )
	opt_cpp->update_edges( myopt->cost_params );

      bwdata_fs.open( "bitsize_nonuni.dat" , ios::out );
      result_gen->gen_bitsize_dat( bwdata_fs );
      bwdata_fs.close();
         
      optsum_fs.open( "bwopt.dat" , ios::out | ios::app );
      myopt->print_opt_summary( optsum_fs );
      optsum_fs.close();

    }
    else if  ( mydag->arith_type == FLOAT ){

      fstream cost_fs( "cost_func.cpp" , ios::out | ios::app );
      cost_func_flt_cpp->gen_costf( cost_fs , "error_function" );
    
      cost_func_flt_cpp->gen_parameters_func( cost_fs );
      cost_fs << "\n}\n";
      cost_fs.close();
      
      OptEngine *myopt = new OptEngine();

      myopt->run_optimization();

      optsum_fs.open( "bwopt.dat" , ios::out | ios::app );
      myopt->print_opt_summary( optsum_fs );
      optsum_fs.close();

      if ( prec_ann_mode == AFFINE_PREC_CPP )
	myopt->update_edges_uni( errmodel_affine_flt_cpp->analyzed_edges );
      else if ( prec_ann_mode == AD_PREC_CPP )
	optflt_cpp->update_edges_uni( myopt->uni_bw );
      
      fstream bwdata_fs( "bitsize_uni.dat" , ios::out );
      result_gen->gen_bitsize_dat( bwdata_fs );
      bwdata_fs.close();
      

      if ( prec_ann_mode == AFFINE_PREC_CPP )
	myopt->update_edges( errmodel_affine_flt_cpp->analyzed_edges );
      else if ( prec_ann_mode == AD_PREC_CPP )
	optflt_cpp->update_edges( myopt->cost_params );

      bwdata_fs.open( "bitsize_nonuni.dat" , ios::out );
      result_gen->gen_bitsize_dat( bwdata_fs );
      bwdata_fs.close();



    }
    else if  ( mydag->arith_type == LNS ){

      fstream cost_fs( "cost_func.cpp" , ios::out | ios::app );
      cost_func_lns_cpp->gen_costf( cost_fs , "error_function" );
    
      cost_func_lns_cpp->gen_parameters_func( cost_fs );
      cost_fs << "\n}\n";
      cost_fs.close();

      OptEngine *myopt = new OptEngine();

      myopt->run_optimization();

      optsum_fs.open( "bwopt.dat" , ios::out | ios::app );
      myopt->print_opt_summary( optsum_fs );
      optsum_fs.close();
      
      if ( prec_ann_mode == AFFINE_PREC_CPP )
	myopt->update_edges_uni( errmodel_affine_lns_cpp->analyzed_edges );
      else if ( prec_ann_mode == AD_PREC_CPP )
	optlns_cpp->update_edges_uni( myopt->uni_bw );
      
      fstream bwdata_fs( "bitsize_uni.dat" , ios::out );
      result_gen->gen_bitsize_dat( bwdata_fs );
      bwdata_fs.close();
      

      if ( prec_ann_mode == AFFINE_PREC_CPP )
	myopt->update_edges( errmodel_affine_lns_cpp->analyzed_edges );
      else if ( prec_ann_mode == AD_PREC_CPP )
	optlns_cpp->update_edges( myopt->cost_params );

      bwdata_fs.open( "bitsize_nonuni.dat" , ios::out );
      result_gen->gen_bitsize_dat( bwdata_fs );
      bwdata_fs.close();


      //cout << "LNS not ready yet !!\n";

      

    }
    

   
    

  }else{



  }

  


}



void bitsize::run_passes()
{

  //range_analysis->do_affine_analysis();
  (*range_analysis)();

  //range_analysis->show_interval_vecs()
  //range_analysis->show_affine_vecs();
  //myerror->gen_matlab_errfiles();
  
  if ( mydag->arith_type == FIXED ){

    //  if (( range_ann_mode == SIM_RANGE ) && ((  prec_ann_mode == AFFINE_PREC ) || (  prec_ann_mode ==  NOOPT_PREC ) ))  
// 	(*opt_fx)( cout );

    if ( opt_metric == POWER_OPT )
      {

	run_passes_pwr();

      }
    else if ( ilp_gen )
      {

	gen_ilp_code();

      }
    else{


    if ( prec_ann_mode == AFFINE_PREC ){

      errmodel_affine->do_affine_analysis();
      //errmodel_affine->show_affine_vecs();

      fstream fs( "error_function.m" , ios::out );
      errmodel_affine->gen_matlab_err_func(fs );  
      fs.close();


      //myerror->gen_matlab_errfiles();

      fstream cost_fs( "cost_function.m" , ios::out );
      cost_func->gen_matlab_costf( cost_fs , "error_function" );
      cost_fs.close();

      fstream param_fs( "parameters.m" , ios::out );
      cost_func->gen_parameters_func( param_fs );
      param_fs.close();

     
    }
    else if ( prec_ann_mode == AFFINE_PREC2 ){
      errmodel_affine2->do_affine_analysis();
      errmodel_affine2->show_affine_vecs();

      fstream fs( "error_function_af2.m" , ios::out );
      errmodel_affine2->gen_matlab_err_func(fs );  
      fs.close();

      fstream cost_fs( "cost_function.m" , ios::out );
      cost_func->gen_matlab_costf( cost_fs , "error_function" );
      cost_fs.close();


      fstream param_fs( "parameters.m" , ios::out );
      cost_func->gen_parameters_func( param_fs );
      param_fs.close();

    }
    else if ( prec_ann_mode == AD_PREC ){

      if ( ad_err_func_only == true ) {
	opt_fx->ad_error_func_gen( cout );
      }else{

      (*opt_fx)( cout );

      }

      
      fstream fs( "error_function_ad.m" , ios::out );
      ad_dag->gen_matlab_err_func(fs );  
      fs.close();

    }

    gen_mat_testcode_fix();
    }
  }
  else if  ( mydag->arith_type == FLOAT ){

    //asc_backend->do_ascbackend( cout );
    //(*asc_backend)( cout );

    //ad_dag->do_ad_analysis();
    //(*ad_dag)();
    (*opt_flt)( cout ); 
    //(*opt_flt)( cout );
    if( mat_code_gen ){
      bs_pass = new MatCodeGenFlt(  mydag , mynode_sched );
      fstream mat_fs( "mat_test.m" , ios::out );
      (*bs_pass)(mat_fs);
      mat_fs.close();
    }


  }

  fstream bwdata_fs( "bitsize.dat" , ios::out );
  result_gen->gen_bitsize_dat( bwdata_fs );
  bwdata_fs.close();


  fstream bwdata_fs2( "bitsize_range.dat" , ios::out );
  result_gen->gen_bitsize_dat( bwdata_fs2 );
  bwdata_fs2.close();


  //ad_dag->do_ad_analysis();

  // ad_dag->show_ad_vecs();

}


void bitsize::gen_ilp_code()
{
  if( ilp_gen ){
    errmodel_affine2->do_affine_analysis();
    errmodel_affine2->show_affine_vecs();

    fstream fs( "err_func2.m" , ios::out );
    errmodel_affine2->gen_matlab_err_func(fs );  
    fs.close();


    ilp_pass = new ILPGenPass( mydag , 1 , ilp_ub_val );
    fstream ilp_fs( "bitsize.lp" , ios::out );
    ilp_pass->gen_ilp_costfunc( ilp_fs );
    ilp_fs.close();
    
    fstream varnames_fs( "bs_varnames.txt" , ios::out );
    ilp_pass->gen_varnames_list( varnames_fs );
    varnames_fs.close();


  }


}

void bitsize::gen_mat_testcode_fix()
{
 
  if( mat_code_gen ){
      bs_pass = new MatCodeGen(  mydag , mynode_sched );
      fstream mat_fs( "mat_test.m" , ios::out );
      (*bs_pass)(mat_fs);
      mat_fs.close();
  }

}


void bitsize::gen_vhdlcode()
{
 
  vhdlgen_pass = new VHDLGen(  mydag , mynode_sched );
  
  fstream vhdl_fs( "bitsize_test.vhd" , ios::out );

  (*vhdlgen_pass)(bwdata_file , vhdl_fs);

  vhdl_fs.close();
    
  fstream vhdl_tb( "bitsize_tb.vhd" , ios::out );

  vhdlgen_pass->gen_test_bench( vhdl_tb );

  vhdl_tb.close();
}






////////////////////////////////////////////////////////////////////////////////////////////////////////
/// DEPRECIATED



void bitsize::list_vars()
{
  cout << "Listing vars =" << endl;

  DAG *c_dag = mydag;

  vector<dag_index>::iterator edge_itr;

 
  cout << "Input Edges" << endl;
  for( edge_itr = c_dag->input_edges.begin(); edge_itr != c_dag->input_edges.end() ; edge_itr++ )
    {


      cout << (c_dag->edge_vec[  (*edge_itr) ])->my_var->var_name << " : " << (c_dag->edge_vec[ (*edge_itr) ])->my_var->ad_val.var_number << " : " << (*edge_itr) <<  endl;

    }

  

  cout << "Mul Edges"  << endl;
  for( edge_itr = c_dag->mul_edges.begin(); edge_itr != c_dag->mul_edges.end() ; edge_itr++ )
    {


      cout << (c_dag->edge_vec[ (*edge_itr) ])->my_var->ad_val.var_number << " : " << (*edge_itr) << endl;

    }
  
  cout << "Add Edges"  << endl;
  for( edge_itr = c_dag->add_edges.begin(); edge_itr != c_dag->add_edges.end() ; edge_itr++ )
    {


      cout << (c_dag->edge_vec[ (*edge_itr) ])->my_var->var_name << endl;

    }

  cout << "Sub Edges"  << endl;
  for( edge_itr = c_dag->sub_edges.begin(); edge_itr != c_dag->sub_edges.end() ; edge_itr++ )
    {


      //cout << (c_dag->edge_vec[ (*edge_itr) ])->my_var->var_name << endl;
      cout << (c_dag->edge_vec[ (*edge_itr) ])->my_var->var_name << " : " << (c_dag->edge_vec[ (*edge_itr) ])->my_var->ad_val.var_number << " : " << (c_dag->edge_vec[ (*edge_itr) ])->edge_number <<  endl;

    }
  

}



void bitsize::list_vars2()
{


  for( int ctr = 0; ctr < varC.hwvar_c.size() ; ctr++ )
    {


      cout << ctr << " : " << (varC.hwvar_c[ctr])->var_name << endl;

    }



}


void bitsize::test_var( HWvar *t)
{

  vector<dag_index>::iterator edge_itr;

  
  HWfix *temp;

  // for( edge_itr = mydag->add_edges.begin(); edge_itr != mydag->add_edges.end() ; edge_itr++ )
//     {

//       if( temp = dynamic_cast<HWfix *>( ( mydag->edge_vec[ *edge_itr ] )->my_var ) )
// 	{

// 	  cout << temp->var_name << " is HWfix " << endl;

// 	} 
//       else
// 	{

// 	  cout << "NO" << endl;

// 	}


//     }


  if( temp = dynamic_cast<HWfix *>( t ))
     { 

	  cout << temp->var_name << " is HWfix " << endl;

	} 
      else
	{

	  cout << "NO" << endl;

	}

}


void bitsize::list_edges()
{

  mydag->list_edges( mydag->output_edges[0] );



}


