// $Author: amag97 $
// $Id: node.cpp,v 1.15 2004/12/29 13:51:39 amag97 Exp $
// $Log: node.cpp,v $
// Revision 1.15  2004/12/29 13:51:39  amag97
// Added DIV support - not tested
//
// Revision 1.14  2004/12/08 00:21:08  amag97
// Added non uniform analysis for floating point
//
// Revision 1.13  2004/12/07 00:20:19  amag97
// Fixed simulated annealing based optimisation v2 for fixed point
//
// Revision 1.12  2004/12/06 00:46:52  amag97
// Added simulation based range analysis
// Added multi-prec fixed-point prec. analysis ver 2 ( Beta )
//
// Revision 1.11  2004/11/28 23:29:02  amag97
// Added back support for HWfloat
// Uniform bit-width optimisation for HWfloat
//
// Revision 1.10  2004/10/29 17:42:26  amag97
// A few more fixes
//
// Revision 1.9  2004/10/25 14:31:14  amag97
// Fixed fraction optimisation mode 6
//
// Revision 1.8  2004/10/23 10:37:37  amag97
// Addition of new fraction bw evaluation method
// (work in progress)
//
// Revision 1.7  2004/09/30 20:32:52  amag97
// Added Area model
// Lots of fixes to get it to work
//
// Revision 1.6  2004/09/17 16:52:13  amag97
// Added area estimation code
//
// Revision 1.5  2004/07/06 19:48:12  amag97
// Revised DAG generation
//
// Revision 1.4  2004/06/05 17:34:18  amag97
// More bug fixes to ASC integration
//
// Revision 1.3  2004/06/03 22:01:31  amag97
// More bug fixes
// Added HWint
//
// Revision 1.2  2004/06/01 11:31:55  amag97
// DAG code
//
// Revision 1.1  2004/05/28 20:14:27  amag97
// ASC Integration
//

#include "node.h"
#include "varnames.h"

//varnames node_names( "NODE" , "" , 1 , varnames::NUMERIC ); 


Node::Node( Node_OPTYPE n_type ,  Node_Arithmetic_TYPE n_atype, string name , dag_index n_num , double n_val )
{
  
  //node_name = ( name == NULL ) ? node_names.gen_varname() : name;

 
  node_name = ( name == "" ) ? "NO_NAME" : name;

  node_type = n_type;
  
  node_number = n_num;

  node_val = n_val;

  node_atype = n_atype;

}

Node::~Node()
{
  

}

double Node::get_fixed_area_usage( dag_index l_n , dag_index r_n , dag_index  out_n )
{

  Edge  *l, *r, *out_e;

  DAG *c_dag = bs->mydag;

  l =  c_dag->edge_vec[ l_n ];
  r =  c_dag->edge_vec[ r_n ];
  out_e =  c_dag->edge_vec[ out_n ];

  int l_int_bw, l_frac_bw, r_int_bw, r_frac_bw;
  int out_int_bw, out_frac_bw;

  l_int_bw = l_frac_bw = r_int_bw = r_frac_bw = 0;

  out_int_bw =  out_frac_bw = 0;

  HWfix *l_var,*r_var, *out_var;

  if( l_var = dynamic_cast<HWfix *>(l->my_var) )
    {
      
      l_int_bw = (int)bs->range_analysis->range_bw_fx(l);
      
      l_frac_bw = (int)l_var->frac_bw;

    } 

   if( r_var = dynamic_cast<HWfix *>(r->my_var) )
    {
      
      r_int_bw = (int)bs->range_analysis->range_bw_fx(r);
      
      r_frac_bw = (int)r_var->frac_bw;

    } 
  
   if( out_var = dynamic_cast<HWfix *>(out_e->my_var) )
    {
      
      out_int_bw = (int)bs->range_analysis->range_bw_fx(out_e);
      
      out_frac_bw = (int)out_var->frac_bw;

    } 
  
   double area_use = 0.;

   switch( node_type ){

   case ADD:area_use = min( (max( (l_int_bw + l_frac_bw) , ( r_int_bw + r_frac_bw ))), (out_int_bw +out_frac_bw ) ); break;

 case SUB:area_use =  min( (max( (l_int_bw + l_frac_bw) , ( r_int_bw + r_frac_bw ))), (out_int_bw +out_frac_bw ) ); break;

   case MUL:area_use =  ( (l_int_bw + l_frac_bw) * ( r_int_bw + r_frac_bw )); break;

  case DIV:area_use = ( (l_int_bw + l_frac_bw) * ( r_int_bw + r_frac_bw )); break;

  default:area_use = 0.;cerr<< "Node not supported" << endl;
  }


  // cout << area_use << endl;

  return area_use;
}


double Node::get_float_area_usage( dag_index l_n , dag_index r_n , dag_index  out_n )
{

  Edge  *l, *r, *out_e;

  DAG *c_dag = bs->mydag;

  l =  c_dag->edge_vec[ l_n ];
  r =  c_dag->edge_vec[ r_n ];
  out_e =  c_dag->edge_vec[ out_n ];

  int l_exp_bw, l_man_bw, r_exp_bw, r_man_bw;
  int out_exp_bw, out_man_bw;

  l_exp_bw = l_man_bw = r_exp_bw = r_man_bw = 0;

  out_exp_bw =  out_man_bw = 0;

  HWfloat *l_var,*r_var, *out_var;

  if( l_var = dynamic_cast<HWfloat *>(l->my_var) )
    {
      
      l_exp_bw = (int)bs->range_analysis->range_bw_flt(l);
      
      l_man_bw = (int)l_var->man_bw;

    } 

   if( r_var = dynamic_cast<HWfloat *>(r->my_var) )
    {
      
      r_exp_bw = (int)bs->range_analysis->range_bw_flt(r);
      
      r_man_bw = (int)r_var->man_bw;

    } 
  
   if( out_var = dynamic_cast<HWfloat *>(out_e->my_var) )
    {
      
      out_exp_bw = (int)bs->range_analysis->range_bw_flt(out_e);
      
      out_man_bw = (int)out_var->man_bw;

    } 
  
   double area_use = 0.;

   switch( node_type ){

   case ADD:area_use = min( (max( (l_exp_bw + l_man_bw) , ( r_exp_bw + r_man_bw ))), (out_exp_bw +out_man_bw ) ); break;

 case SUB:area_use =  min( (max( (l_exp_bw + l_man_bw) , ( r_exp_bw + r_man_bw ))), (out_exp_bw +out_man_bw ) ); break;

   case MUL:area_use =  ( (l_exp_bw + l_man_bw) * ( r_exp_bw + r_man_bw )); break;

  case DIV:area_use = ( (l_exp_bw + l_man_bw) * ( r_exp_bw + r_man_bw ));break;

  default:area_use = 0.;cerr<< "Node not supported" << endl;
  }


  // cout << area_use << endl;

  return area_use;
}



double Node::get_fixed_area_usage( dag_index l_n , dag_index r_n )
{

  
  Edge  *l, *r;

  DAG *c_dag = bs->mydag;

  l =  c_dag->edge_vec[ l_n ];
  r =  c_dag->edge_vec[ r_n ];

  fsize l_int_bw, l_frac_bw, r_int_bw, r_frac_bw;


  HWfix *l_var , *r_var;

  l_var = dynamic_cast<HWfix *>(l->my_var);
  r_var = dynamic_cast<HWfix *>(r->my_var);


  //string lname , rname;

  if ( l_var != NULL ){

    //l_int_bw = (fsize)l_var->ad_val.fp_int_bw;
    //l_frac_bw = (fsize)l_var->ad_val.fp_frac_bw;

     l_int_bw = (fsize)l_var->int_bw;
     l_frac_bw = (fsize)l_var->frac_bw;

     //lname = l_var->var_name;

    //cout << lname << " " <<  l_int_bw  << " " << l_frac_bw << endl;

  }
  else
    {
      l_int_bw = l_frac_bw = 0;

      //lname = "Not found";
    }



  if ( r_var != NULL ) {

    //r_int_bw = (fsize)r_var->ad_val.fp_int_bw;
    //r_frac_bw = (fsize)r_var->ad_val.fp_frac_bw;

    r_int_bw = (fsize)r_var->int_bw;
    r_frac_bw = (fsize)r_var->frac_bw;

    //rname = r_var->var_name;


    // cout << rname << " " <<  r_int_bw  << " " << r_var->frac_bw << endl;
  }
  else
    {


      r_int_bw = r_frac_bw = 0;

      //rname = "Not found";

    }


//   if (( (c_dag->node_vec[l->get_src_node()])->node_type == ADD ) || 
//       ( (c_dag->node_vec[l->get_src_node()])->node_type == SUB ) ||
//       ( (c_dag->node_vec[l->get_src_node()])->node_type == MUL ))
      
//     {
 
//       l_int_bw = (fsize)dynamic_cast<HWfix *>(l->my_var)->ad_val.fp_int_bw;
//       l_frac_bw = (fsize)dynamic_cast<HWfix *>(l->my_var)->ad_val.fp_frac_bw;

//     }
//   else
//     {
//       l_int_bw = l_frac_bw = 0;

//     }
  

//   if (( (c_dag->node_vec[r->get_src_node()])->node_type == ADD ) ||
//       ( (c_dag->node_vec[r->get_src_node()])->node_type == SUB ) ||
//       ( (c_dag->node_vec[r->get_src_node()])->node_type == MUL ))
//     {
       
//       r_int_bw = (fsize)dynamic_cast<HWfix *>(r->my_var)->ad_val.fp_int_bw;
//       r_frac_bw = (fsize)dynamic_cast<HWfix *>(r->my_var)->ad_val.fp_frac_bw;

//     }
//   else
//     {
//       r_int_bw = r_frac_bw = 0;

//     }
  

  fsize i_bw = max( l_int_bw , r_int_bw );
  fsize f_bw = max( l_frac_bw , r_frac_bw );

  double area_use = 0.;

  //cout << c_dag->Node_OPNAME[node_type] << " " << node_name << " " << lname << " " << rname << " : " << c_dag->Edge_TYPENAME[ r->edge_type ] <<" "; 
  //cout << " :IBW " << i_bw << " FBW " << f_bw <<  " :: ";

  switch( node_type ){

  case ADD:area_use = 2.94*i_bw + 3.72 * f_bw + 42.96; break;

  case SUB:area_use = 2.94*i_bw + 3.72 * f_bw + 42.96; break;

  case MUL:area_use = i_bw*( ( 0.1059*f_bw + 1.2354 )*i_bw + ( 2.4424*f_bw + 20.9600 ) ) + ( 19.766*f_bw + 12.5389 ); break;

  case DIV:area_use = 1.0; break;

  default:area_use = 0.;cerr<< "Node not supported" << endl;
  }


  // cout << area_use << endl;

  return area_use;

}



double Node::get_float_area_usage( dag_index l_n , dag_index r_n )
{

  
  Edge  *l, *r , *out_e;

  DAG *c_dag = bs->mydag;

  l =  c_dag->edge_vec[ l_n ];
  r =  c_dag->edge_vec[ r_n ];

  // out_e = c_dag->edge_vec[ out_edge ];

  fsize l_int_bw, l_frac_bw, r_int_bw, r_frac_bw;


  HWfix *l_var , *r_var;

  l_var = dynamic_cast<HWfix *>(l->my_var);
  r_var = dynamic_cast<HWfix *>(r->my_var);


  string lname , rname;

  if ( l_var != NULL ){

    //l_int_bw = (fsize)l_var->ad_val.fp_int_bw;
    //l_frac_bw = (fsize)l_var->ad_val.fp_frac_bw;

     l_int_bw = (fsize)l_var->int_bw;
     l_frac_bw = (fsize)l_var->frac_bw;

    lname = l_var->var_name;

    //cout << lname << " " <<  l_int_bw  << " " << l_frac_bw << endl;

  }
  else
    {
      l_int_bw = l_frac_bw = 0;

      lname = "Not found";
    }



  if ( r_var != NULL ) {

    //r_int_bw = (fsize)r_var->ad_val.fp_int_bw;
    //r_frac_bw = (fsize)r_var->ad_val.fp_frac_bw;

    r_int_bw = (fsize)r_var->int_bw;
    r_frac_bw = (fsize)r_var->frac_bw;

    rname = r_var->var_name;


    // cout << rname << " " <<  r_int_bw  << " " << r_var->frac_bw << endl;
  }
  else
    {


      r_int_bw = r_frac_bw = 0;

       rname = "Not found";

    }


//   if (( (c_dag->node_vec[l->get_src_node()])->node_type == ADD ) || 
//       ( (c_dag->node_vec[l->get_src_node()])->node_type == SUB ) ||
//       ( (c_dag->node_vec[l->get_src_node()])->node_type == MUL ))
      
//     {
 
//       l_int_bw = (fsize)dynamic_cast<HWfix *>(l->my_var)->ad_val.fp_int_bw;
//       l_frac_bw = (fsize)dynamic_cast<HWfix *>(l->my_var)->ad_val.fp_frac_bw;

//     }
//   else
//     {
//       l_int_bw = l_frac_bw = 0;

//     }
  

//   if (( (c_dag->node_vec[r->get_src_node()])->node_type == ADD ) ||
//       ( (c_dag->node_vec[r->get_src_node()])->node_type == SUB ) ||
//       ( (c_dag->node_vec[r->get_src_node()])->node_type == MUL ))
//     {
       
//       r_int_bw = (fsize)dynamic_cast<HWfix *>(r->my_var)->ad_val.fp_int_bw;
//       r_frac_bw = (fsize)dynamic_cast<HWfix *>(r->my_var)->ad_val.fp_frac_bw;

//     }
//   else
//     {
//       r_int_bw = r_frac_bw = 0;

//     }
  

  fsize i_bw = max( l_int_bw , r_int_bw );
  fsize f_bw = max( l_frac_bw , r_frac_bw );

  double area_use = 0.;

  //cout << c_dag->Node_OPNAME[node_type] << " " << node_name << " " << lname << " " << rname << " : " << c_dag->Edge_TYPENAME[ r->edge_type ] <<" "; 
  //cout << " :IBW " << i_bw << " FBW " << f_bw <<  " :: ";

  switch( node_type ){

  case ADD:area_use = 2.94*i_bw + 3.72 * f_bw + 42.96; break;

  case SUB:area_use = 2.94*i_bw + 3.72 * f_bw + 42.96; break;

  case MUL:area_use = i_bw*( ( 0.1059*f_bw + 1.2354 )*i_bw + ( 2.4424*f_bw + 20.9600 ) ) + ( 19.766*f_bw + 12.5389 ); break;

  case DIV:area_use = 1.0; break;

  default:area_use = 0.;cerr<< "Node not supported" << endl;
  }


  // cout << area_use << endl;

  return area_use;

}


double Node::get_area_usage()
{

  
  dag_index left = in_edges[0];
  dag_index right = in_edges[1];
  

  dag_index out_edge = out_edges.back();

  double area_use;

  switch( node_atype )
    {
      
    case INTEGER: cerr << "Integer Area Usage not supported" << endl; area_use = 0.; break;
      

    case FIXED: area_use = get_fixed_area_usage( left , right , out_edge ); break;


    case FLOAT:  area_use =  get_float_area_usage( left , right , out_edge); break;
     

    case AT_NOTSET:  cerr << "Floating Point Area usage not supported" << endl; area_use = 0.; break;



    default:;


    }



  return area_use;



}
