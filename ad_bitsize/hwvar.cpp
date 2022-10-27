// $Author: amag97 $
// $Id: hwvar.cpp,v 1.14 2004/12/13 01:33:43 amag97 Exp $
// $Log: hwvar.cpp,v $
// Revision 1.14  2004/12/13 01:33:43  amag97
// Added command line argument parsing
// Added Interval based range analysis
// Added ASSIGN node to dag
//
// Revision 1.13  2004/10/23 10:37:37  amag97
// Addition of new fraction bw evaluation method
// (work in progress)
//
// Revision 1.12  2004/09/30 20:32:52  amag97
// Added Area model
// Lots of fixes to get it to work
//
// Revision 1.11  2004/07/21 18:22:04  amag97
// Pre-vacation
//
// Revision 1.10  2004/07/09 10:22:21  amag97
// More fixes to DAG code
//
// Revision 1.9  2004/07/06 19:48:11  amag97
// Revised DAG generation
//
// Revision 1.8  2004/06/24 18:45:31  amag97
// Added a seperate class to encapsulate the bitwidth optimisation functions
//
// Revision 1.7  2004/06/08 21:07:40  amag97
// More bugfixes to - advar
//
// Revision 1.6  2004/06/07 19:51:08  amag97
// Global HWvector collection
// Bugfixes to copy constructor
//
// Revision 1.5  2004/06/03 22:01:31  amag97
// More bug fixes
// Added HWint
//
// Revision 1.4  2004/06/02 19:37:59  amag97
// Added HWvector
//
// Revision 1.3  2004/06/01 19:31:17  amag97
// More fixes to DAG code
//
// Revision 1.2  2004/06/01 11:31:55  amag97
// DAG code
//
// Revision 1.1  2004/05/28 20:14:27  amag97
// ASC Integration
//

#include "hwvar.h"




#define BITSIZE_DEFS
#include "bitsize_defs.h"


//varcollections<HWvar *> HWvar::varC;

varnames HWvar::vnames( "VAR" , "" , 1 , varnames::NUMERIC );
varnames HWvar::tmpvnames( "TMP" , "VAR" , 1 , varnames::NUMERIC );

//DAG HWvar::mydag;

dag_index HWvar::var_count = 0;

HWvar::HWvar()
{

}


HWvar::~HWvar()
{


}


HWvar::HWvar(const HWvar &a )
{

  

  //ad_val = a.ad_val;

  ad_val.copy( a.ad_val );

  var_type = a.var_type;

  var_dectype = a.var_dectype;

  if ( var_dectype == DECVAR )
    {
      var_name = vnames.gen_varname();
     if (  bs->global_opmode == PARSING  )
       {
	 myedge = bs->mydag->new_edge(this);
	 //myedge = a.myedge;
       }
     
    }
  else if ( var_dectype == VECVAR )
    {
      var_name = (a.var_name);
     if (  bs->global_opmode == PARSING  )
     {
         myedge = bs->mydag->new_edge(this);
     }

    }
  else
    {
      var_name = tmpvnames.gen_varname();
    }
  
  if (  bs->global_opmode == PARSING  )
  {
    //myedge->src_nodes.assign( a.myedge->src_nodes.begin() , a.myedge->src_nodes.end() );
    //myedge->dst_nodes.assign( a.myedge->dst_nodes.begin() , a.myedge->dst_nodes.end() );
  }
}


HWvar& HWvar::operator=( const HWvar &a )
{
  if ( this != &a )
    {
      if (  bs->global_opmode == PARSING  )
      { 
        //myedge->src_nodes.assign( a.myedge->src_nodes.begin() , a.myedge->src_nodes.end() );
	//myedge->src_nodes.push_back( a.myedge->src_nodes.back() );
      }
      //dst_nodes.assign( a.dst_nodes.begin() , a.dst_nodes.end() );
       ad_val = a.ad_val;

    }

 

  return *this;

}

HWvar::operator double()
{

  return static_cast<double>(ad_val); 

}

int HWvar::read_inputs( istream &os )
{

  
      varcollections<HWvar *>::iter_t itr = bs->varC.invar_c.begin();
      double tt;
      while( itr != bs->varC.invar_c.end() )
      {
	  if( (os >> tt) == 0 ) 
	    return -1;
	  //cerr << tt << endl;
	  (*itr++)->ad_val=tt;
      }

   

  return 0;
}


void HWvar::add_varcollection( arch_attr var_type )
{

  
  advar::BS.add_to_list( &ad_val );

  bs->varC.hwvar_c.push_back( this );
  
  switch( var_type )
    {
    case IN: bs->varC.invar_c.push_back( this ); ad_val.varType = advar::IN; break;

    case OUT: bs->varC.outvar_c.push_back( this ); ad_val.varType = advar::OUT; break;

    case TMP: bs->varC.tmpvar_c.push_back( this ); ad_val.varType = advar::TMP; break;

    default: break;

    }



  if ( var_type == OUT  )
    ad_val.BWState = SET;

}




// dag_index HWvar::get_src_node()
// {

//   return myedge->src_nodes.back();

// }
 

// dag_index HWvar::get_dst_node()
// {

//   return myedge->dst_nodes.back();


// }


void HWvar::reset_vars()
{

  vector<HWvar*>::iterator itr;

  for( itr = bs->varC.hwvar_c.begin() ; itr != bs->varC.hwvar_c.end() ; itr++ )
    (*itr)->ad_val.reset_grad_vec();


}


double HWvar::round_val( double val , int bits )
{

  int64 temp;

  //temp = static_cast<int>( val * pow( 2. , bits ) );

  temp = static_cast<int64>( scalbn( val , bits ) );

  return scalbn( temp , -bits );

  
}
