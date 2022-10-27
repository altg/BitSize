// $Author: amag97 $
// $Id: bitsize.h,v 1.23 2006/02/14 19:07:53 amag97 Exp $
// $Log: bitsize.h,v $
// Revision 1.23  2006/02/14 19:07:53  amag97
// Added new Pass for generating glitch based power cost function
// Bug Fixes - conv. between range and bitwidth
//
// Revision 1.22  2004/12/07 00:20:19  amag97
// Fixed simulated annealing based optimisation v2 for fixed point
//
// Revision 1.21  2004/11/28 23:29:02  amag97
// Added back support for HWfloat
// Uniform bit-width optimisation for HWfloat
//
// Revision 1.20  2004/11/16 21:56:19  amag97
// More bug fixes
//
// Revision 1.19  2004/11/08 18:36:28  amag97
// Added affine arithmetic based range analysis
// Bug fixes
//
// Revision 1.18  2004/11/06 19:38:37  amag97
// More changes to affine analysis
//
// Revision 1.17  2004/11/05 18:50:04  amag97
// More fixes to error function generation
//
// Revision 1.16  2004/10/31 19:53:10  amag97
// Added node schedular
// Added error propagation function generator
// Integeration of these with BitSize
//
// Revision 1.15  2004/10/26 18:32:59  amag97
// Fixed most bugs with fixed-point frac. opt 6
//
// Revision 1.14  2004/10/23 10:37:31  amag97
// Addition of new fraction bw evaluation method
// (work in progress)
//
// Revision 1.13  2004/10/05 19:23:55  amag97
// Fixes to AD code - bug when multiple inputs are used
// Updated Graph info
//
// Revision 1.12  2004/09/30 20:32:51  amag97
// Added Area model
// Lots of fixes to get it to work
//
// Revision 1.11  2004/09/17 16:52:13  amag97
// Added area estimation code
//
// Revision 1.10  2004/09/10 17:21:55  amag97
// Changes to bwopt method 3
//
// Revision 1.9  2004/08/31 19:10:05  amag97
// Fixes to bwopt method 3
//
// Revision 1.8  2004/08/26 18:00:49  amag97
// Update after return from Sri Lanka
// Started work on adding user constraints
//
// Revision 1.7  2004/07/21 18:22:04  amag97
// Pre-vacation
//
// Revision 1.6  2004/07/09 10:22:20  amag97
// More fixes to DAG code
//
// Revision 1.5  2004/07/06 19:48:11  amag97
// Revised DAG generation
//
// Revision 1.4  2004/06/21 16:23:20  amag97
// Post FPT04 commit
//
// Revision 1.3  2004/06/03 22:01:30  amag97
// More bug fixes
// Added HWint
//
// Revision 1.2  2004/06/01 19:31:17  amag97
// More fixes to DAG code
//
// Revision 1.1  2004/05/28 20:14:27  amag97
// ASC Integration
//

#ifndef _BWOPT_H
#define _BWOPT_H


#ifdef _BITSIZE_ 

#ifdef OPT_BW
#undef OPT_BW
#endif

#define OPT_BW 9999

#include "bitsize_class.h"

typedef unsigned int fsize;

// #include "hwfix.h"
// #include "hwvector.h"
// #include "hwint.h"

bitsize* bs;


#define STREAM_START  BS_OPTIONS_START HWfix::init();

#define STREAM_LOOP( _VAL ) fstream fs( "asctest.dat" , ios::in );	\
  if ( !fs.is_open()){cerr << "File Open error asctest.dat" <<endl;;exit(-1);} \
                            bool stream_loop_exit = false;              \
                            while(( HWvar::read_inputs( fs ) != - 1 ) && ( ! stream_loop_exit )){ 

 //HWfix::output_analysis2();	    \

#define STREAM_END BS_OPTIONS_LOOP_END              \
                   HWvar::reset_vars(); } 	\
                   BS_OPTIONS_END


// #define STREAM_END BS_OPTIONS_LOOP_END              \
// 		   HWfix::setup_max_grads();       \
//                    HWvar::reset_vars(); } 	\
//                    HWfix::output_analysis2();			      \
//                    HWfix::output_results2();        \
//                    BS_OPTIONS_END


#define BS_OPTIONS_START bs = new bitsize( argc , argv );   \
                         bool execute_once = EXEC_ONCE_FLAG; \
                         _GRAPH_START		    \
                         bs->start_parse(); \
                         DAG* g_dag = bs->mydag;  

#define BS_OPTIONS_LOOP_END bs->mydag->sort_edges();      \
                            bs->end_parse(); \
                            bs->mydag->update_edge_grads(); \
                            if ( execute_once ) stream_loop_exit = true;

 //bs->propagate_error();					\

#define BS_OPTIONS_END _GRAPH_END \
		      bs->bitsize_main(); 

//bs->myerror->gen_matlab_errfiles();	 \
                       //bs->get_area();	\
  //bs->propagate_error();
  //bs->mydag->show_edge_grads();
                       //bs->list_vars2();


 //  bs->range_analysis->do_affine_analysis(); \
//                        bs->range_analysis->show_affine_vecs();



#define R_ARGS(VAL) atoi( bs->r_args[VAL].c_str() )
                       
#ifdef _GRAPH_GEN

#define _GRAPH_START fstream g_out( "graph.dot" , ios::out );
#define _GRAPH_END bs->mydag->print_dot_graph2( g_out ); g_out.close();

#else

#define _GRAPH_START
#define _GRAPH_END






#endif


#else

#include <cstdio>
#include "asc.h"

#define STREAM_END_ARG( _VAL ) STREAM_END


#endif



#endif
