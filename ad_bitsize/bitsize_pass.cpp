#include "bitsize_pass.h"

BitSizePass::BitSizePass( DAG *_dag):c_dag(_dag),rounding(""){}

void BitSizePass::init_name_maps()
{
 for( vector<Edge *>::iterator e_itr = c_dag->edge_vec.begin(); e_itr != c_dag->edge_vec.end();e_itr++)
    {
	
        gen_signal_name(  (*e_itr) );
	
	

    }

  for( vector<Node *>::iterator n_itr = c_dag->node_vec.begin(); n_itr != c_dag->node_vec.end();n_itr++)
    {
	
      gen_node_cost_names(  (*n_itr) );

    }
 


}

void BitSizePass::gen_node_cost_names( Node* CNode )
{

  node_costnames[CNode] = "Cost_" + CNode->node_name;

}

void BitSizePass::gen_signal_name( Edge* CEdge)
{

  HWvar *temp;
  
  if ( CEdge->edge_type != ECONSTANT ){
  
    if ( temp =  CEdge->my_var )
      {
      
	signal_names[CEdge] = temp->var_name;
	signal_bwnames[CEdge] = "bw_" + temp->var_name;
      }

  }
}


int BitSizePass::get_bw_state(  Edge *CEdge )
{

  if (  dynamic_cast<HWfix *>( CEdge->my_var )->frac_bw_state == SET )
    {
      
      return 1;

    }
  {

    return 0;

  }
}


double BitSizePass::get_range_bw( Edge *CEdge )
{

//  if ( dynamic_cast<HWfix *>( CEdge->my_var )->int_bw_state != SET )
//      {
  
//       double r_min = bs->range_analysis->get_range_min( CEdge ); 

//       double r_max = bs->range_analysis->get_range_max( CEdge );

//       //cout << "Rmax" << r_max << " : " << "Rmin" << endl;

//       r_max = static_cast<int>(r_max) % 2 == 0 ? r_max + 1 : r_max;

//       double log2_val = ( log( max( abs( r_min  ) , abs( r_max  ) ) )/log( 2. ) ) + 1.;

//       return ( log2_val <= 0. ? 1. : ceil( log2_val ));
//     }
//    else
//      {

//        return static_cast<double>(  dynamic_cast<HWfix *>( CEdge->my_var )->int_bw );


//      }  

  HWfix*  var  = dynamic_cast<HWfix*>(CEdge->my_var);

  return var->int_bw;
}

string BitSizePass::get_var_type( Edge *CEdge )
{

  switch( CEdge->edge_type ){

  case IN: return "IN";break;
    
  case OUT: return "OUT";break;

  default: 
    return "INT";


  }



}


string BitSizePass::replace_str( string str_in , string srch_str , string rpl_str )
{

  int where, size_s , size_r , size_in;

  string temp;

  where = str_in.find( srch_str , 0 );

  size_r = rpl_str.size();

  size_s = srch_str.size();

  size_in = str_in.size();

  if ( where != string::npos ){
    
   
    
    if ( size_s >= size_r ){

      temp = str_in;

      temp.replace( where , size_r , rpl_str );

      temp.erase( where + size_r  , size_s - size_r  );

     }
    else{
      

      temp = str_in.substr( 0 , where ) + rpl_str + str_in.substr( where + size_s , size_in - where - size_s );

    }
    
     

  }

  return temp;


} 
