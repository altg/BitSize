#include "range_analysis.h"

SimRange::SimRange( DAG *_dag , NodeSchedular *_node_sched ):RangeAnalysis( _dag , _node_sched ){}

void SimRange::operator()()
{

  // _DBOUT( "Doing Simulation range analysis" );


  if  ( ( bs->prec_ann_mode != bitsize::AD_PREC ) && ( bs->prec_ann_mode != bitsize::AD_PREC_CPP ) )
    {
      ADDAG *ad_dag = bs->ad_dag;
      
      while( (*ad_dag)() )
	{
	  
	  if (  ad_dag->iter_count % 1000 == 0 )
	    _DBOUT(  "Running AD for Simulation :" << ad_dag->iter_count );
	 
	}

      //update_bw();

      if ( bs->db_level > 0  ) show_range_vecs();

    }

   
}



double SimRange::get_range_max( Edge *CEdge)
{

      return CEdge->max_val;

}


double SimRange::get_range_min( Edge *CEdge)
{
  double range_min = 0;


  // if ( c_dag->arith_type == FIXED ){
 
//     if ( dynamic_cast<HWfix *>(CEdge->my_var)->c_sign == UNSIGNED )
//       {

// 	range_min = 0;

//       }
//     else 
//       {


// 	range_min = CEdge->min_val;

//       }

//   }
//   else  
    

 range_min = CEdge->min_val;


  return range_min;

   
}

void SimRange::show_range_vecs()
{
  vector< Edge * >::iterator e_itr;

  
  Edge *CEdge;

   cout << "---------------------------------------------" <<endl;

  for( e_itr = c_dag->edge_vec.begin() ; e_itr != c_dag->edge_vec.end() ; e_itr++ )
    {
      
      CEdge = (*e_itr);
      
   
      cout << CEdge->edge_name << " Max :" << get_range_max( CEdge ) << " Min : " << get_range_min( CEdge ) << endl;
      
      cout << "---------------------------------------------" <<endl;
    }



}
