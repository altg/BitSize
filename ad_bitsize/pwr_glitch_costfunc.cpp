#include "bitsize_pass.h"


void PwrPassGlitch::MAT_G_Prof_Add_ng( Edge* edge_l , Edge* edge_r , Edge* edge_out)
{

  
  HWfix* var_l = dynamic_cast<HWfix*>(edge_l->my_var);
  HWfix* var_r = dynamic_cast<HWfix*>(edge_r->my_var);

  HWfix* var_out = dynamic_cast<HWfix*>(edge_out->my_var);

 

  f_body << "[ PWR_" << edge_out->edge_name << " , G_" << edge_out->edge_name << " ] = ";

  f_body << " F_ADD_NG( ";

  f_body << "max( " << var_l->int_bw << " + bw_" << edge_l->edge_name << " , ";
  f_body << var_r->int_bw << " + bw_" << edge_r->edge_name << " ) ";
  f_body << " , " << edge_l->data_log.acoef_1 << " , ";
  f_body << edge_r->data_log.acoef_1 << " );" << endl;

  f_body << "G_" << edge_out->edge_name << " = bw_trunc( ";

  f_body << "G_" << edge_out->edge_name << " , ( ";
  f_body << var_out->int_bw << " + bw_" << edge_out->edge_name  << " ) );" << endl;

  PWR_nodes.push_back(edge_out->edge_name);

}


void PwrPassGlitch::MAT_G_Prof_Sub_ng( Edge* edge_l , Edge* edge_r , Edge* edge_out)
{

 

  HWfix* var_l = dynamic_cast<HWfix*>(edge_l->my_var);
  HWfix* var_r = dynamic_cast<HWfix*>(edge_r->my_var);

   HWfix* var_out = dynamic_cast<HWfix*>(edge_out->my_var);

  f_body << "[ PWR_" << edge_out->edge_name << " , G_" << edge_out->edge_name << " ] = ";

  f_body << " F_ADD_NG( ";

  f_body << "max( " << var_l->int_bw << " + bw_" << edge_l->edge_name << " , ";
  f_body << var_r->int_bw << " + bw_" << edge_r->edge_name << " ) ";
  f_body << " , " << edge_l->data_log.acoef_1 << " , ";
  f_body << edge_r->data_log.acoef_1 << " );" << endl;

  f_body << "G_" << edge_out->edge_name << " = bw_trunc( ";

  f_body << "G_" << edge_out->edge_name << " , ( ";
  f_body << var_out->int_bw << " + bw_" << edge_out->edge_name  << " ) );" << endl;


  PWR_nodes.push_back(edge_out->edge_name);


}

void PwrPassGlitch::MAT_G_Prof_Mul_ng( Edge* edge_l , Edge* edge_r , Edge* edge_out)
{

 

  HWfix* var_l = dynamic_cast<HWfix*>(edge_l->my_var);
  HWfix* var_r = dynamic_cast<HWfix*>(edge_r->my_var);

   HWfix* var_out = dynamic_cast<HWfix*>(edge_out->my_var);

  f_body << "[ PWR_" << edge_out->edge_name << " , G_" << edge_out->edge_name << " ] = ";

  Node *src_l_node = c_dag->get_src_node( edge_l );

  Node *src_r_node = c_dag->get_src_node( edge_r );

  if ( src_l_node->node_type == CONSTANT ){
    
  f_body << " F_CMUL_NG( ";

  f_body << bs->range_analysis->get_range_max( edge_l );

  f_body << " , " << edge_r->data_log.acoef_1 << " );" << endl;


   }
  else if ( src_r_node->node_type == CONSTANT ){

    f_body << " F_CMUL_NG( ";

    f_body << bs->range_analysis->get_range_max( edge_r );
  
    f_body << " , " <<edge_l->data_log.acoef_1 << " );" << endl;


  }
  else{

  f_body << " F_MUL_NG( ";

  f_body << "max( " << var_l->int_bw << " + bw_" << edge_l->edge_name << " , ";
  f_body << var_r->int_bw << " + bw_" << edge_r->edge_name << " ) ";
  f_body << " , " << edge_l->data_log.acoef_1 << " , ";
  f_body << edge_r->data_log.acoef_1 << " );" << endl;

  }


  f_body << "G_" << edge_out->edge_name << " = bw_trunc( ";

  f_body << "G_" << edge_out->edge_name << " , ( ";
  f_body << var_out->int_bw << " + bw_" << edge_out->edge_name  << " ) );" << endl;

  PWR_nodes.push_back(edge_out->edge_name);


}

void PwrPassGlitch::MAT_G_Prof_Add( Edge* edge_l , Edge* edge_r , Edge* edge_out)
{

  
  HWfix* var_l = dynamic_cast<HWfix*>(edge_l->my_var);
  HWfix* var_r = dynamic_cast<HWfix*>(edge_r->my_var);

  HWfix* var_out = dynamic_cast<HWfix*>(edge_out->my_var);


  
  f_body << "[ PWR_" << edge_out->edge_name << " , G_" << edge_out->edge_name << " ] = ";

  f_body << " F_ADD( ";

  f_body << "G_" << edge_l->edge_name << " , " << "G_" << edge_r->edge_name;

  f_body  << " );" << endl;


  f_body << "G_" << edge_out->edge_name << " = bw_trunc( ";

  f_body << "G_" << edge_out->edge_name << " , ( ";
  f_body << var_out->int_bw << " + bw_" << edge_out->edge_name  << " ) );" << endl;


  PWR_nodes.push_back(edge_out->edge_name);
 

}


void PwrPassGlitch::MAT_G_Prof_Sub( Edge* edge_l , Edge* edge_r , Edge* edge_out)
{

 

  HWfix* var_l = dynamic_cast<HWfix*>(edge_l->my_var);
  HWfix* var_r = dynamic_cast<HWfix*>(edge_r->my_var);
  HWfix* var_out = dynamic_cast<HWfix*>(edge_out->my_var);


 
  f_body << "[ PWR_" << edge_out->edge_name << " , G_" << edge_out->edge_name << " ] = ";

  f_body << " F_ADD( ";

  f_body << "G_" << edge_l->edge_name << " , " << "G_" << edge_r->edge_name;

  f_body  << " );" << endl;


  f_body << "G_" << edge_out->edge_name << " = bw_trunc( ";

   f_body << "G_" << edge_out->edge_name << " , ( ";
  f_body << var_out->int_bw << " + bw_" << edge_out->edge_name  << " ) );" << endl;



  PWR_nodes.push_back(edge_out->edge_name);


}

void PwrPassGlitch::MAT_G_Prof_Mul( Edge* edge_l , Edge* edge_r , Edge* edge_out)
{

 

  HWfix* var_l = dynamic_cast<HWfix*>(edge_l->my_var);
  HWfix* var_r = dynamic_cast<HWfix*>(edge_r->my_var);
  

  HWfix* var_out = dynamic_cast<HWfix*>(edge_out->my_var);



  f_body << "[ PWR_" << edge_out->edge_name << " , G_" << edge_out->edge_name << " ] = ";


  Node *src_l_node = c_dag->get_src_node( edge_l );

  Node *src_r_node = c_dag->get_src_node( edge_r );



  if ( src_l_node->node_type == CONSTANT ){

   f_body << " F_CMUL( ";

   f_body << bs->range_analysis->get_range_max( edge_l ) << " , ";

   f_body << var_l->int_bw << " , bw_" << edge_l->edge_name << ", G_" << edge_r->edge_name;

   f_body  << " );" << endl;

  }
  else if ( src_r_node->node_type == CONSTANT ){

   f_body << " F_CMUL( ";

   f_body << bs->range_analysis->get_range_max( edge_r ) << " , ";

   f_body << var_r->int_bw << " , bw_" << edge_r->edge_name << ", G_" << edge_l->edge_name;

   f_body  << " );" << endl;


  }
  else{
  f_body << " F_MUL( ";

  f_body << "G_" << edge_l->edge_name << " , " << "G_" << edge_r->edge_name;

  f_body  << " );" << endl;

  }
  

  f_body << "G_" << edge_out->edge_name << " = bw_trunc( ";

  f_body << "G_" << edge_out->edge_name << " , ( ";
  f_body << var_out->int_bw << " + bw_" << edge_out->edge_name  << " ) );" << endl;

  PWR_nodes.push_back(edge_out->edge_name);

}


void PwrPassGlitch::MAT_G_Prof_CShift_r( Edge* edge_in , double const_val , Edge *edge_out ){

  HWfix* var_out = dynamic_cast<HWfix*>(edge_out->my_var);

   f_body << "[ PWR_" << edge_out->edge_name << " , G_" << edge_out->edge_name << " ] = ";

   f_body << " F_SHIFTL( ";

   f_body << "G_" << edge_in->edge_name << " , " << const_val;

   f_body  << " );" << endl;

  f_body << "G_" << edge_out->edge_name << " = bw_trunc( ";

  f_body << "G_" << edge_out->edge_name << " , ( ";
  f_body << var_out->int_bw << " + bw_" << edge_out->edge_name  << " ) );" << endl;

  PWR_nodes.push_back(edge_out->edge_name);


}


void PwrPassGlitch::MAT_G_Prof_CShift_l( Edge* edge_in , double const_val , Edge *edge_out ){

  HWfix* var_out = dynamic_cast<HWfix*>(edge_out->my_var);

   f_body << "[ PWR_" << edge_out->edge_name << " , G_" << edge_out->edge_name << " ] = ";

   f_body << " F_SHIFTR( ";

   f_body << "G_" << edge_in->edge_name << " , " << const_val;

   f_body  << " );" << endl;

   f_body << "G_" << edge_out->edge_name << " = bw_trunc( ";

   f_body << "G_" << edge_out->edge_name << " , ( ";
   f_body << var_out->int_bw << " + bw_" << edge_out->edge_name  << " ) );" << endl;

   PWR_nodes.push_back(edge_out->edge_name);

}



void PwrPassGlitch::MAT_G_Prof_Input( Edge* edge_out)
{

  HWfix* var_out = dynamic_cast<HWfix*>(edge_out->my_var);

  f_body << "G_" << edge_out->edge_name << " = ";

  f_body << "G_input_sig( ";
  
  f_body << var_out->int_bw << " + bw_" << edge_out->edge_name;

  f_body << " );\n ";

  

}


void PwrPassGlitch::pwr_cost_header(){



  h_body << "function [cost_value, cost_flag] = cost_func_pwrglitch( bw )" << endl;

  h_body << "global bw_out;"  << endl;

  h_body << "[no_of_bw, bw_out_ , bw_names, bw_type,  bw_set , bw_ranges ] = parameters();" << endl;

  h_body << "req_error = 2.^-bw_out;" << endl;

  

  int ctr = 1;
  int output_sigs = 1;
  for( map<Edge*,string>::iterator s_itr = signal_names.begin(); s_itr != signal_names.end(); s_itr++ )
    {

      if ( (*s_itr).first->edge_type != OUT ){
	if (  dynamic_cast< HWfix *>((*s_itr).first->my_var)->frac_bw_state != SET )
	  {
	
	    h_body << "bw_" << (*s_itr).first->edge_name << " = bw( " << ctr++ << " );" << endl;
	
	  }
	else {
	  
	  h_body << "bw_" << (*s_itr).first->edge_name << " = bw_out(1);" << endl;


	}
      }
      else{
	h_body << "bw_" << (*s_itr).first->edge_name << " = bw_out( ";
	h_body << output_sigs++ << " );\n";
      }

    }
  signal_count = ctr - 1;

  h_body << "my_error = error_function( bw  , bw_out );" << endl;
  //h_body << "if ( ( my_error  > req_error) )" << endl; 
  h_body << "if ( ";


  if ( c_dag->output_edges.size() == 1 ){
    h_body << "( my_error  > req_error)"; 
  }
  else
    {
      for( int i = 1 ; i  <= c_dag->output_edges.size() ; i++ )
	{
	  h_body << "( my_error( " << i << " ) > req_error( " << i << " ))";  
	  //h_body << "( my_error( " << i << " ) > req_error )";


	  if ( i != c_dag->output_edges.size() ) h_body << " | ";
	}

    }

  h_body << " ) " << endl;


  h_body << "cost_flag = 0;" << endl;
  h_body << "cost_value = 0;" << endl;
  h_body << "else" << endl;

}


void PwrPassGlitch::pwr_cost_footer(){


  foot_body << "cost_value = sum( [ ";


  for ( list<string>::iterator itr = PWR_nodes.begin(); itr != PWR_nodes.end() ; itr++ )
    {

      foot_body << "PWR_" << (*itr) << " ";
  
    }



  foot_body << "]);" << endl;

  
  foot_body << "cost_flag = 1;" << endl;
  foot_body << "end;" << endl;

 
}



void PwrPassGlitch::gen_parameters_func( ostream &os ){

  os << "function [no_of_bw, bw_out , bw_names, bw_type, bw_state , bw_ranges ]";

  vector<dag_index>::iterator e_itr;

 
  os << "= parameters" << endl;

  // os << "no_of_bw = " << signal_count - output_signal_count << ";" << endl;

  
  
  os << "bw_out " << " =[ ";

 

  for( e_itr = c_dag->output_edges.begin() ; e_itr != c_dag->output_edges.end() ; e_itr++ )
    {

       os  <<  dynamic_cast< HWfix *>(c_dag->edge_vec[(*e_itr)]->my_var)->frac_bw << " ";
     
    }

  os << "];" << endl;

  stringstream ss_names, ss_ranges ,ss_state, ss_type, ss_frac_bw;

  int ctr = 1;

    ss_names << "bw_names = { ";
    ss_ranges << "bw_ranges = [ ";
    
    ss_state << "bw_state = [";

    ss_type << "bw_type = { ";

    ss_frac_bw << "bw_set_frac = [ ";
    
     // Print IN and TMP var info

    init_name_maps();

    int no_sig_bws = 0;

  for( map<Edge*,string>::iterator s_itr = signal_names.begin(); s_itr != signal_names.end(); s_itr++  )
    {
      if ( (*s_itr).first->edge_type != OUT ){

      ss_names << " '" << (*s_itr).second <<  "' "; 
       
       ss_ranges << get_range_bw( (*s_itr).first ) << " ";

       //ss_state << get_bw_state( (*s_itr).first ) << " ";
       //if ( get_bw_state( (*s_itr).first )  == 0 ){
	 ss_state << get_bw_state( (*s_itr).first ) << " ";
	 //}
       if ( get_bw_state( (*s_itr).first )  == 1 ) {
	 Edge *CEdge = (*s_itr).first;
	 ss_frac_bw << " " << dynamic_cast<HWfix *>( CEdge->my_var )->frac_bw << " "; 
       }
        

     
      }

       if ( get_bw_state( (*s_itr).first )  == 0 ) no_sig_bws++;
       
       ss_type << " '" << get_var_type( (*s_itr).first ) <<  "' ";


    }



    // Print OUT var info

   for( vector<dag_index>::iterator e_itr = c_dag->output_edges.begin() ; e_itr != c_dag->output_edges.end() ; e_itr++ )
    {

      //os  <<  dynamic_cast< HWfix *>(c_dag->edge_vec[(*e_itr)]->my_var)->frac_bw << " ";
      // os << signal_bwnames[ c_dag->edge_vec[(*e_itr)] ] <<  " = bw_out(" << bw_out_ctr++ << ");" << endl; 

      ss_names << " '" << signal_names[ c_dag->edge_vec[(*e_itr)] ] <<  "' ";
      
      ss_ranges << get_range_bw(  c_dag->edge_vec[(*e_itr)]  ) << " ";

      

      ss_state << get_bw_state(  c_dag->edge_vec[(*e_itr)] )  << " ";
      
      ss_type << " '" << get_var_type( c_dag->edge_vec[(*e_itr)] ) <<  "' ";


    }

  ss_names << "};" << endl;

  ss_ranges << "];" << endl;

  ss_state << "];" << endl;

  ss_type << "};" << endl;
  
  ss_frac_bw << "];" << endl;

  os << "no_of_bw = " << no_sig_bws << ";" << endl;

  os << ss_names.str();

  os << ss_type.str();

  os << ss_ranges.str();

  
  os << ss_state.str();

  os << ss_frac_bw.str();


 
    

}
