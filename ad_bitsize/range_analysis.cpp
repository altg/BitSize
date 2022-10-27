#include "range_analysis.h"

RangeAnalysis::RangeAnalysis( DAG * _dag , NodeSchedular *_sched ):c_dag(_dag),node_sched(_sched){}


void RangeAnalysis::update_bw()
{

   if ( c_dag->arith_type == FIXED ){

     update_bw_fx();

   }
   else if ( c_dag->arith_type == FLOAT ){
     update_bw_flt();

   }
   else if ( c_dag->arith_type == LNS ){
     update_bw_lns();

   }

}

void RangeAnalysis::update_bw_fx()
{

  vector<Edge *>::iterator e_itr;

  HWfix *tvar;

  for( e_itr = c_dag->edge_vec.begin() ; e_itr != c_dag->edge_vec.end(); e_itr++ )
    {

      if( tvar = dynamic_cast<HWfix *>( (*e_itr)->my_var ) )
	{

	  //if ( tvar->int_bw_state != SET )
	    tvar->int_bw = (fsize)range_bw_fx( (*e_itr) );

	   

	}


    } 


}


void RangeAnalysis::update_bw_flt()
{

  vector<Edge *>::iterator e_itr;

  HWfloat *tvar;

  for( e_itr = c_dag->edge_vec.begin() ; e_itr != c_dag->edge_vec.end(); e_itr++ )
    {

      if( tvar = dynamic_cast<HWfloat *>( (*e_itr)->my_var ) )
	{

	  tvar->exp_bw = (fsize)range_bw_flt( (*e_itr) );

	 

	}


    } 

}


void RangeAnalysis::update_bw_lns()
{

  vector<Edge *>::iterator e_itr;

  HWlns *tvar;

  for( e_itr = c_dag->edge_vec.begin() ; e_itr != c_dag->edge_vec.end(); e_itr++ )
    {

      if( tvar = dynamic_cast<HWlns *>( (*e_itr)->my_var ) )
	{

	  tvar->int_bw = (fsize)range_bw_lns( (*e_itr) );

	}


    } 


}



double RangeAnalysis::range_bw_flt( Edge *CEdge )
{
  HWfloat* t_var;

  if ( t_var = dynamic_cast<HWfloat *>( CEdge->my_var ) )
    {


      if ( t_var->exp_bw_state == NOTSET )
	{
  
	  //  double r_min = bs->range_analysis->get_range_min( CEdge ); 

	  // 	  double r_max = bs->range_analysis->get_range_max( CEdge );

	  // 	  r_max = static_cast<int>(r_max) % 2 == 0 ? r_max + 1 : r_max;

	  // 	  double log2_val = log( max( abs( r_min  ) , abs( r_max  ) ) )/log( 2. );

	  // 	  double e_bw = log(( log2_val <= 0. ? ceil(abs(log2_val) + 1.) : ceil( log2_val )) + 1)/log(2.) + 1; 

	  // 	  return ceil( e_bw );

	  double r_min = bs->range_analysis->get_range_min( CEdge ); 

	  CEdge->min_val = r_min;

	  double r_max = bs->range_analysis->get_range_max( CEdge );

	  CEdge->max_val = r_max;

	 //  double drange = ilogb( r_max )  - ilogb( r_min );

// 	  drange = ( drange == 0. ) ? 1. : drange;

// 	  return ceil( log( drange ) / log( 2. ) ) + 1.;


	  //cout << CEdge->var_name << " : " << 

	  double log2_val = log( max( abs( r_min  ) , abs( r_max  ) ) )/log( 2. );

	  double e_bw = log(( log2_val <= 0. ? ceil(abs(log2_val) + 1.) : ceil( log2_val )) + 1)/log(2.) + 1;

	  return ceil( e_bw );


	}
      else
	{

	  CEdge->min_val =  bs->range_analysis->get_range_min( CEdge ); 

	  CEdge->max_val = bs->range_analysis->get_range_max( CEdge );

	  return t_var->exp_bw;


	}
    }  
  else{

    return 0;
  }


    
}

double RangeAnalysis::range_bw_fx( Edge *CEdge )
{
  HWfix* t_var;

  if ( t_var = dynamic_cast<HWfix *>( CEdge->my_var ) )
    {


      if ( ( t_var->int_bw_state == NOTSET ) )
	{

	  double r_min = bs->range_analysis->get_range_min( CEdge ); 

	  double r_max = bs->range_analysis->get_range_max( CEdge );

	  r_max = static_cast<int>(r_max) % 2 == 0 ? r_max + 1 : r_max;

	 
	  //r_max = floor( r_max );
	  double log2_val;
	  if ( t_var->c_sign  == TWOSCOMPLEMENT ){
	    // log2_val = log( abs( r_max  ) + 1)/log( 2. ) + 1;

	    log2_val = ( log( max( abs( r_min  ) , abs( r_max  ) )  )/log( 2. )) + 1;
	    //	    cout << t_var->var_name << " : : " << r_max << endl;
	  }
	  else if ( t_var->c_sign  ==  SIGNMAGNITUDE ){

	    log2_val = log( max( abs( r_min  ) , abs( r_max  ) ) + 1 )/log( 2. ) + 1;
	  }
	  else{

	    log2_val = log( max( abs( r_min  ) , abs( r_max  ) )  )/log( 2. );
	  }

	  return ( log2_val <= 0. ? 1. : ceil( log2_val ));
	}
      else
	{

	  
	  CEdge->min_val =  bs->range_analysis->get_range_min( CEdge ); 

	  CEdge->max_val = bs->range_analysis->get_range_max( CEdge );


	 
	  return t_var->int_bw;


	}
    }  
  else{

    return 0;
  }


    
}


double RangeAnalysis::range_bw_lns( Edge *CEdge )
{

  HWlns* t_var;

  if ( t_var = dynamic_cast<HWlns *>( CEdge->my_var ) )
    {


      if ( t_var->int_bw_state == NOTSET )
	{
  

	  double r_min = bs->range_analysis->get_range_min( CEdge ); 

	  CEdge->min_val = r_min;

	  double r_max = bs->range_analysis->get_range_max( CEdge );

	  CEdge->max_val = r_max;


	  double log2_val = log( max( abs( r_min  ) , abs( r_max  ) ) )/log( 2. );

	  double e_bw = log(( log2_val <= 0. ? ceil(abs(log2_val) + 1.) : ceil( log2_val )) + 1)/log(2.) + 1;



	  return ceil( e_bw );


	}
      else
	{

	  CEdge->min_val =  bs->range_analysis->get_range_min( CEdge ); 

	  CEdge->max_val = bs->range_analysis->get_range_max( CEdge );

	  return t_var->int_bw;


	}
    }  
  else{

    return 0;
  }


    
}


void RangeAnalysis::show_range_vecs()
{
  vector< Edge * >::iterator e_itr;

  
  Edge *CEdge;

   cout << "---------------------------------------------" <<endl;

  for( e_itr = c_dag->edge_vec.begin() ; e_itr != c_dag->edge_vec.end() ; e_itr++ )
    {
      
      CEdge = (*e_itr);
      
   
      cout << CEdge->edge_name << " Max :" << CEdge->max_val;
      cout << " Min : " << CEdge->min_val << " : ";

      if ( bs->mydag->arith_type == FIXED ){

	cout << range_bw_fx( CEdge );

      }
      else if ( bs->mydag->arith_type == FLOAT ){

	cout << range_bw_flt( CEdge );


      }else if ( bs->mydag->arith_type == LNS ){

	cout << range_bw_lns( CEdge );

      }


      cout << endl;
      
      cout << "---------------------------------------------" <<endl;
    }



}
