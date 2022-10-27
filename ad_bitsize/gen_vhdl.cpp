// $Author: amag97 $
// $Id: gen_vhdl.cpp,v 1.4 2006/04/28 17:11:59 amag97 Exp $


#include "bitsize_pass.h"

VHDLGen::VHDLGen(DAG *_dag , NodeSchedular *_node_sched ):BitSizePass(_dag),node_sched(_node_sched){}


void VHDLGen::operator()( string inputfile_name , ostream &os )
{
 

  process_signal();

  if ( inputfile_name == "" ){

    cerr << "BWFILE not supplied using bwdata in stream_opt.cxx\n";
    update_edges_bw();

  }
  else{

    update_edges_bw( inputfile_name );

  }

  gen_struct_sec();

  gen_entity_sec();

  gen_arch_sec();

  gen_components();

  output_code( os );
  
  fstream fs( "bs_struct.m" , ios::out );

  fs << "function nd = bs_struct \n";
  
  fs << bs_struct.str() << endl;

  fs.close();
  
}


void VHDLGen::output_code( ostream &os ){

  os << "LIBRARY ieee;\n";   
  os << "USE ieee.std_logic_1164.ALL;\n";   
  os << "USE ieee.std_logic_arith.ALL;\n\n";   


  os << entity_code.str() << endl;

  
  os << arch_code.str() << endl;

  os << comp_code.str() << endl;

  os << struct_code.str() << endl;
  

}


void VHDLGen::do_vhdlgen()
{
  
   _DBOUT( "Doing VHDL code generation" );

   node_sched->sort_schedule();

  vector< NodeSchedular::node_vec_type >::iterator sch_itr; 
  vector< dag_index >::iterator n_itr2;

  int ctr = 0;
  for ( sch_itr = node_sched->schedule.begin() ; sch_itr != node_sched->schedule.end() ;  sch_itr++ )
    {

     
      
      for( n_itr2 = (*sch_itr).begin() ; n_itr2 != (*sch_itr).end() ; n_itr2++ )
	{

	    do_vhdlgen_node((*n_itr2));

	}  




    }



}


void VHDLGen::do_vhdlgen_node( dag_index node_num )
{

  Node* CNode = c_dag->node_vec[ node_num ];
 

  switch( CNode->node_type )
    {

   
    case ADD: gen_add_node( CNode ); break;


    case SUB: gen_sub_node( CNode ); break;


    case MUL: gen_mul_node( CNode ); break;

    case DIV: gen_div_node( CNode ); break;

    case CONSTANT: gen_const_node( CNode ); break;
   
    case OUTPUT : gen_output_node( CNode ); break;

    case INPUT : gen_input_node( CNode ); break;

    case SHIFTL : gen_shiftl_node( CNode ); break;

    case SHIFTR : gen_shiftr_node( CNode ); break;

    defualt:;



    }


}


bool VHDLGen::is_output_edge( Edge *CEdge )
{

  Node* dst_node = c_dag->node_vec[ CEdge->get_dst_node() ];
 
  return (  dst_node->node_type == OUTPUT ? true : false ); 

}

bool VHDLGen::is_input_edge( Edge *CEdge )
{

  Node* src_node = c_dag->node_vec[ CEdge->get_src_node() ];
 
  return (  src_node->node_type == INPUT ? true : false ); 

}


void VHDLGen::process_signal()
{

  string signal_name;

  string var_name;

  

 for( vector<Edge *>::iterator e_itr = c_dag->edge_vec.begin(); e_itr != c_dag->edge_vec.end();e_itr++)
    {
	
      HWfix *e_var; //= dynamic_cast<HWfix *>( (*e_itr)->my_var );

      if ( is_input_edge( (*e_itr) ) ){
	
	e_var =  dynamic_cast<HWfix *>( (*e_itr)->my_var );

	//signal_name = (*e_itr)->edge_name;
	signal_name = e_var->var_name;

	input_signals.push_back( (*e_itr) );

	signal_type[ (*e_itr) ] = INPUT_SIG;

      }
      else if ( is_output_edge( (*e_itr) ) ){

	//signal_name = (*e_itr)->edge_name;

	e_var = dynamic_cast<HWfix *>( (*e_itr)->my_var );

	signal_name = e_var->var_name;

	output_signals.push_back( (*e_itr) );

	signal_type[ (*e_itr) ] = OUTPUT_SIG;


      }
      else
      {
	
	signal_name = "sig_" + (*e_itr)->edge_name;

	//signal_name = "sig_" + e_var->var_name;

	//internal_signals.push_back( (*e_itr) ); 

	//signal_type[ (*e_itr) ] = INTERNAL_SIG;

      }


      signal_names[ (*e_itr) ] = signal_name;

      
      if ( HWfix *c_var = dynamic_cast<HWfix *>( (*e_itr)->my_var ) )
	{
	  varnames_map[ c_var->var_name ] = (*e_itr);
	  
	}

    }



}


void VHDLGen::gen_bs_struct( Node *c_node , Node *l_node , Node *r_node )
{

  string node_type = replace_str( c_dag->Node_OPNAME[ c_node->node_type ] , "_NODE" , "" );

  string node_name = replace_str( c_node->node_name , "NODE_" , "" );

  bs_struct << "nd." << node_type << "_" << node_name << " = ";

  string l_node_name,r_node_name;

  if ( l_node->node_type == INPUT ){

    l_node_name = replace_str( l_node->node_name , "NODE" , "INREG" ); 

  }
  
  if ( r_node->node_type == INPUT ){

    r_node_name = replace_str( r_node->node_name , "NODE" , "INREG" );
   

  }
  
  if (( l_node->node_type != INPUT ) && ( r_node->node_type != INPUT )){

    l_node_name = replace_str( c_dag->Node_OPNAME[ l_node->node_type ] , "_NODE" , "" ) + "_" + replace_str( l_node->node_name , "NODE_" , "" ); 

    r_node_name = replace_str( c_dag->Node_OPNAME[ r_node->node_type ] , "_NODE" , "" ) + "_" + replace_str( r_node->node_name , "NODE_" , "" ); 

    

  }

  bs_struct << "{ '" << l_node_name  << "' , '";
  bs_struct <<  r_node_name << "' }\n";


}

// string VHDLGen::get_signal_name( Edge* CEdge ){

//   if ( is_input_edge( CEdge ) ){
	
	
//         return input_signals[ CEdge ]; 

//       }
//       else if ( is_output_edge( CEdge ) ){

	

// 	return output_signals[ CEdge ];


//       }
//       else
//       {
	
// 	return internal_signals[ CEdge ]; 


//       }
  



// }

void VHDLGen::add_internal_signal( Edge *CEdge ){

  if ( !is_output_edge( CEdge ) && !is_input_edge( CEdge ) )
    {

      internal_signals.push_back( CEdge ); 

      signal_type[ CEdge ] = INTERNAL_SIG;

    }

}


void VHDLGen::gen_add_node(  Node * CNode )
{
 

  Edge *l_edge, *r_edge, *out_edge;

  
  l_edge = c_dag->edge_vec[ CNode->in_edges[0] ];
  r_edge = c_dag->edge_vec[ CNode->in_edges[1] ];
  out_edge = c_dag->edge_vec[ CNode->out_edges.back() ];
  
  add_internal_signal( out_edge );

  int bw_a = (edge_bwdata[l_edge])->total_bw;
  int bw_b = (edge_bwdata[r_edge])->total_bw;

  int bw_out  = (edge_bwdata[out_edge])->total_bw;


  int comp_out_bw = max( bw_a , bw_b ) + 1;


  

  stringstream comp_name;
  comp_name << "add_" << bw_a << "x" << bw_b;

  string outtemp_sig_name( "tmp_" + CNode->node_name );
  //outtemp_sig_name << "tmp_" << CNode->node_name;

  sig_dec << "\tsignal " << outtemp_sig_name << " : ";

  sig_dec << "std_logic_vector( " << comp_out_bw << " - 1 downto 0 ) := ( others => '0' );\n";


  Node *src_l_node = c_dag->get_src_node( l_edge );
  Node *src_r_node = c_dag->get_src_node( r_edge );

  gen_bs_struct( CNode , src_l_node , src_r_node );
 

  if ( src_l_node->node_type == CONSTANT ){

     int int_bw = (edge_bwdata[l_edge])->int_bw;
      int frac_bw = (edge_bwdata[l_edge])->frac_bw;

      sig_dec << "\tsignal " << signal_names[ l_edge ] << " : ";

      sig_dec << "std_logic_vector( " << (edge_bwdata[l_edge])->total_bw << " - 1 downto 0 ) := ( others => '0'";
      
      sig_dec << " );\n";

      struct_code << signal_names[ l_edge ] << " <= \"";
      gen_binvalue( struct_code , src_l_node->node_val , int_bw , frac_bw );
      
      struct_code << "\";\n";




  }else if ( src_r_node->node_type == CONSTANT )
    {
      int int_bw = (edge_bwdata[r_edge])->int_bw;
      int frac_bw = (edge_bwdata[r_edge])->frac_bw;

      sig_dec << "\tsignal " << signal_names[ r_edge ] << " : ";

      sig_dec << "std_logic_vector( " << (edge_bwdata[r_edge])->total_bw << " - 1 downto 0 ) := ( others => '0'";
      
      sig_dec << " );\n";

      struct_code << signal_names[ r_edge ] << " <= \"";
      gen_binvalue( struct_code , src_r_node->node_val , int_bw , frac_bw );
      struct_code << "\";\n";
      

    }
  
  struct_code << endl << "-- Add Node  " << endl;


  struct_code << "ADD_" << CNode->node_name << ": " << comp_name.str() << endl;

  struct_code << "port map ( " << endl;

  struct_code << "a => " << signal_names[ l_edge ] << ","<< endl;
  struct_code << "b => " << signal_names[ r_edge ] << ","<< endl;

  struct_code << "s => " << outtemp_sig_name << endl;

 
  struct_code << ");\n\n";

  struct_code << signal_names[ out_edge ] << " <= " << outtemp_sig_name;
  

  adjust_signal_bw( comp_out_bw , bw_out , CNode->node_name );
  
 
  struct_code << ";\n\n";
  
  comp_table[ comp_name.str() ] = new COMP_TYPE( bw_a , bw_b , false , comp_out_bw , ADD_COMP );  //comp_data;

  

}

void VHDLGen::gen_binvalue( ostream &ss , double val , int int_bw , int frac_bw ){

   long long temp = static_cast<long long>( val * pow( 2. , frac_bw ) );

  //ss << sizeof(long long) << endl;
   int total_bw = frac_bw + int_bw;

   for (int i=total_bw - 1; i>=0; i--) {
             ss << "01"[((temp >> i) & 0x1)];
	   //  if ( i % 4 == 0 )
// 	      ss << " ";
        }
   //ss << endl;


}


void VHDLGen::adjust_signal_bw(int comp_out_bw , int bw_out , string node_name )
{

 if ( comp_out_bw > bw_out ){

    struct_code << "( " << comp_out_bw << " - 1 downto ";

    struct_code << comp_out_bw - bw_out;

    struct_code << ")";

  }
  else if ( comp_out_bw < bw_out ){ 


    struct_code << "&";

    string outtemp_sig_dummy_name( "tmp_" + node_name + "_dummy" );

    struct_code << outtemp_sig_dummy_name; 

    sig_dec << "\tsignal " << outtemp_sig_dummy_name << " : ";

    sig_dec << "std_logic_vector( " << bw_out - comp_out_bw << " - 1 downto 0 ) := ( others => '0' );\n";


  }


}


void VHDLGen::gen_sub_node(  Node * CNode )
{
 
   Edge *l_edge, *r_edge, *out_edge;

  
  l_edge = c_dag->edge_vec[ CNode->in_edges[0] ];
  r_edge = c_dag->edge_vec[ CNode->in_edges[1] ];
  out_edge = c_dag->edge_vec[ CNode->out_edges.back() ];
  
  add_internal_signal( out_edge );

  int bw_a = (edge_bwdata[l_edge])->total_bw;
  int bw_b = (edge_bwdata[r_edge])->total_bw;

  int bw_out  = (edge_bwdata[out_edge])->total_bw;


  int comp_out_bw = max( bw_a , bw_b ) + 1;

  stringstream comp_name;
  comp_name << "add_" << bw_a << "x" << bw_b;

  string outtemp_sig_name( "tmp_" + CNode->node_name );
  //outtemp_sig_name << "tmp_" << CNode->node_name;

  sig_dec << "\tsignal " << outtemp_sig_name << " : ";

  sig_dec << "std_logic_vector( " << comp_out_bw << " - 1 downto 0 ) := ( others => '0' );\n";
  
   Node *src_l_node = c_dag->get_src_node( l_edge );
  Node *src_r_node = c_dag->get_src_node( r_edge );

  gen_bs_struct( CNode , src_l_node , src_r_node );

  

  if ( src_l_node->node_type == CONSTANT ){

     int int_bw = (edge_bwdata[l_edge])->int_bw;
      int frac_bw = (edge_bwdata[l_edge])->frac_bw;

      sig_dec << "\tsignal " << signal_names[ l_edge ] << " : ";

      sig_dec << "std_logic_vector( " << (edge_bwdata[l_edge])->total_bw << " - 1 downto 0 ) := ( others => '0'";
      
      sig_dec << " );\n";

      struct_code << signal_names[ l_edge ] << " <= \"";
      gen_binvalue( struct_code , src_l_node->node_val , int_bw , frac_bw );
      
      struct_code << "\";\n";




  }else if ( src_r_node->node_type == CONSTANT )
    {
      int int_bw = (edge_bwdata[r_edge])->int_bw;
      int frac_bw = (edge_bwdata[r_edge])->frac_bw;

      sig_dec << "\tsignal " << signal_names[ r_edge ] << " : ";

      sig_dec << "std_logic_vector( " << (edge_bwdata[r_edge])->total_bw << " - 1 downto 0 ) := ( others => '0'";
      
      sig_dec << " );\n";

      struct_code << signal_names[ r_edge ] << " <= \"";
      gen_binvalue( struct_code , src_r_node->node_val , int_bw , frac_bw );
      struct_code << "\";\n";
      

    }


  struct_code << endl << "-- Sub Node  " << endl;


  struct_code <<  "Sub_" << CNode->node_name << ": " << comp_name.str() << endl;

  struct_code << "port map ( " << endl;

  struct_code << "a => " << signal_names[ l_edge ] << ","<< endl;
  struct_code << "b => " << signal_names[ r_edge ] << ","<< endl;

  struct_code << "s => " << outtemp_sig_name << endl;

 
  struct_code << ");\n\n";

  struct_code << signal_names[ out_edge ] << " <= " << outtemp_sig_name;
  

  adjust_signal_bw( comp_out_bw , bw_out , CNode->node_name );
  
 
  struct_code << ";\n\n";
  
  comp_table[ comp_name.str() ] = new COMP_TYPE( bw_a , bw_b , false , comp_out_bw , ADD_COMP );  //comp_data;

  
  
 
}



void VHDLGen::gen_mul_node(  Node * CNode )
{
 
  Edge *l_edge, *r_edge, *out_edge;

  
  l_edge = c_dag->edge_vec[ CNode->in_edges[0] ];
  r_edge = c_dag->edge_vec[ CNode->in_edges[1] ];
  out_edge = c_dag->edge_vec[ CNode->out_edges.back() ];
  
  add_internal_signal( out_edge );

  int bw_a, bw_b, bw_out, comp_out_bw;

  COMP_TYPE *t_comp;

  stringstream comp_name;
  

  string outtemp_sig_name( "tmp_" + CNode->node_name );
  //outtemp_sig_name << "tmp_" << CNode->node_name;

 
  
  bw_out  = (edge_bwdata[out_edge])->total_bw;

  Node *src_l_node = c_dag->get_src_node( l_edge );
  Node *src_r_node = c_dag->get_src_node( r_edge );

  gen_bs_struct( CNode , src_l_node , src_r_node );

  if ( src_l_node->node_type == CONSTANT ){

    bw_a = (edge_bwdata[l_edge])->total_bw;
    bw_b = (edge_bwdata[r_edge])->total_bw;
    comp_out_bw = bw_a + bw_b;

    comp_name << "cmul_" << bw_b << "x" << bw_a;
    
    struct_code << "CMul_" << CNode->node_name << ": " << comp_name.str() << endl;
    struct_code << "port map ( " << endl;

    struct_code << "a => " << signal_names[ r_edge ] << ","<< endl;

    struct_code << "o => " << outtemp_sig_name << endl;
      
    struct_code << ");\n\n";

   
    t_comp =  new COMP_TYPE( bw_b , bw_a , false , comp_out_bw , CMUL_COMP );

    t_comp->const_value =  static_cast<unsigned int>( bs->range_analysis->get_range_max( l_edge ) * pow( 2. , bw_a ));

    

  }
  else if ( src_r_node->node_type == CONSTANT ){

    bw_b = (edge_bwdata[r_edge])->total_bw;
    bw_a = (edge_bwdata[l_edge])->total_bw;
    comp_out_bw = bw_a + bw_b;
    comp_name << "cmul_" << bw_a << "x" << bw_b;

    struct_code << "CMul_" << CNode->node_name << ": " << comp_name.str() << endl;
    struct_code << "port map ( " << endl;
    
    struct_code << "a => " << signal_names[ l_edge ] << ","<< endl;

    struct_code << "o => " << outtemp_sig_name << endl;

 
    struct_code << ");\n\n";
   
    t_comp =  new COMP_TYPE( bw_a , 0 , false , comp_out_bw , CMUL_COMP );

    t_comp->const_value =  static_cast<unsigned int>( bs->range_analysis->get_range_max( r_edge ) * pow( 2. , bw_b ));

    

  }
  else{

    bw_a = (edge_bwdata[l_edge])->total_bw;
    bw_b = (edge_bwdata[r_edge])->total_bw;
    comp_out_bw = bw_a + bw_b;

    comp_name << "mul_" << bw_a << "x" << bw_b;

    struct_code << endl << "-- MUL Node  " << endl;


    struct_code << "MUL_" << CNode->node_name << ": " << comp_name.str() << endl;

    struct_code << "port map ( " << endl;

    struct_code << "a => " << signal_names[ l_edge ] << ","<< endl;
    struct_code << "b => " << signal_names[ r_edge ] << ","<< endl;

    struct_code << "o => " << outtemp_sig_name << endl;

 
    struct_code << ");\n\n";

 

    t_comp =  new COMP_TYPE( bw_a , bw_b , false , comp_out_bw , MUL_COMP );

  }

  struct_code << signal_names[ out_edge ] << " <= " << outtemp_sig_name;
  

  adjust_signal_bw( comp_out_bw , bw_out , CNode->node_name );
  
 
  struct_code << ";\n\n";
  
  comp_table[ comp_name.str() ] = t_comp;  //comp_data;

  sig_dec << "\tsignal " << outtemp_sig_name << " : ";

  sig_dec << "std_logic_vector( " << comp_out_bw << " - 1 downto 0 ) := ( others => '0' );\n";
  
 
}


void VHDLGen::gen_shiftl_node( Node *CNode ){

  Edge *l_edge, *r_edge, *out_edge;

  int bw_a, bw_b, bw_out, comp_out_bw;

 

  l_edge = c_dag->edge_vec[ CNode->in_edges[0] ];
  r_edge = c_dag->edge_vec[ CNode->in_edges[1] ];
  out_edge = c_dag->edge_vec[ CNode->out_edges.back() ];
  bw_out  = (edge_bwdata[out_edge])->total_bw;
  add_internal_signal( out_edge );

  int const_val;

  Node *node_l = c_dag->get_src_node( l_edge );
  Node *node_r = c_dag->get_src_node( r_edge );
  
  struct_code << endl << "-- ShiftL Node  " << endl;

  string outtemp_sig_name( "tmp_" + CNode->node_name );

  if (node_l->node_type == CONSTANT ){
     bw_a = 0;
     bw_b = (edge_bwdata[r_edge])->total_bw;

     const_val = static_cast<unsigned int>( node_l->node_val );
     comp_out_bw = bw_b;
    
     struct_code << outtemp_sig_name << " <= " << signal_names[ r_edge ];

     struct_code << "( " << bw_b << " - " << const_val << " - 1 downto 0 )&";

     struct_code << "shiftdummy_" << CNode->node_name << ";\n";

  }else if ( node_r->node_type == CONSTANT ){
    
     bw_b = 0;
     bw_a = (edge_bwdata[l_edge])->total_bw;

     const_val = static_cast<unsigned int>( node_r->node_val );
     comp_out_bw = bw_a;
    
     struct_code << outtemp_sig_name << " <= " << signal_names[ l_edge ];

     struct_code << "( " << bw_a << " - " << const_val << " - 1 downto 0 )&";

     struct_code << "shiftdummy_" << CNode->node_name << ";\n";
    

  }else {}

  struct_code << signal_names[ out_edge ] << " <= " << outtemp_sig_name;

  adjust_signal_bw( comp_out_bw , bw_out , CNode->node_name );

  struct_code << ";\n\n";

  sig_dec << "\tsignal " << outtemp_sig_name << " : ";
  sig_dec << "std_logic_vector( " << comp_out_bw << " - 1 downto 0 ) := ( others => '0' );\n";

  sig_dec << "\tsignal " << "shiftdummy_" << CNode->node_name  << " : ";
  sig_dec << "std_logic_vector( " << const_val << " - 1 downto 0 ) := ( others => '0' );\n";

}
  

void VHDLGen::gen_shiftr_node( Node *CNode ){

  Edge *l_edge, *r_edge, *out_edge;

  int bw_a, bw_b, bw_out, comp_out_bw;

  

  l_edge = c_dag->edge_vec[ CNode->in_edges[0] ];
  r_edge = c_dag->edge_vec[ CNode->in_edges[1] ];
  out_edge = c_dag->edge_vec[ CNode->out_edges.back() ];
  bw_out  = (edge_bwdata[out_edge])->total_bw;
  add_internal_signal( out_edge );

  int const_val;

  Node *node_l = c_dag->get_src_node( l_edge );
  Node *node_r = c_dag->get_src_node( r_edge );
  
  struct_code << endl << "-- ShiftR Node  " << endl;

  string outtemp_sig_name( "tmp_" + CNode->node_name );

  if (node_l->node_type == CONSTANT ){
     bw_a = 0;
     bw_b = (edge_bwdata[r_edge])->total_bw;

     const_val = static_cast<unsigned int>( node_l->node_val );
     comp_out_bw = bw_b;
    
     struct_code << outtemp_sig_name << " <= ";
     
     struct_code << "shiftdummy_" << CNode->node_name << "&";

     struct_code << signal_names[ r_edge ];

     struct_code << "( " << bw_b << " - 1 downto " << const_val << " );\n";

    
  }else if ( node_r->node_type == CONSTANT ){
    
     bw_b = 0;
     bw_a = (edge_bwdata[l_edge])->total_bw;

     const_val = static_cast<unsigned int>( node_r->node_val );
     comp_out_bw = bw_a;
    
     struct_code << outtemp_sig_name << " <= ";
     
     struct_code << "shiftdummy_" << CNode->node_name << "&";

     struct_code << signal_names[ l_edge ];

     struct_code << "( " << bw_a << " - 1 downto " << const_val << " );\n";
    

  }else {}

  struct_code << signal_names[ out_edge ] << " <= " << outtemp_sig_name;

  adjust_signal_bw( comp_out_bw , bw_out , CNode->node_name );

  struct_code << ";\n\n";

  sig_dec << "\tsignal " << outtemp_sig_name << " : ";
  sig_dec << "std_logic_vector( " << comp_out_bw << " - 1 downto 0 ) := ( others => '0' );\n";

  sig_dec << "\tsignal " << "shiftdummy_" << CNode->node_name  << " : ";
  sig_dec << "std_logic_vector( " << const_val << " - 1 downto 0 ) := ( others => '0' );\n";

}

void VHDLGen::gen_const_node(  Node * CNode )
{
 

  
  
 
}

void VHDLGen::gen_output_node(  Node * CNode )
{
 
   Edge *in_edge;
 
  in_edge = c_dag->edge_vec[ CNode->in_edges.back() ];
  
  add_internal_signal( in_edge );
  
  int bw_out  = (edge_bwdata[in_edge])->total_bw;

  int bw_a = bw_out;
  int bw_b = 0;
  
  
  int comp_out_bw = bw_a + bw_b;

  stringstream comp_name;
  comp_name << "reg_" << bw_a << "x" << bw_b;
 
   struct_code << endl << "-- OUTREG Node  " << endl;


  struct_code << "OUTREG_" << CNode->node_name << ": " << comp_name.str() << endl;

  struct_code << "port map ( " << endl;

  struct_code << "D => " << signal_names[ in_edge ] <<  " ," << endl;
  struct_code << "Q => " << signal_names[ in_edge ] << "_out ,"<< endl;
  struct_code << "CLK => clk"; 
 
  struct_code << ");\n\n";

  sig_dec << "\tsignal " << signal_names[ in_edge ] << " : ";

  sig_dec << "std_logic_vector( " << bw_out << " - 1 downto 0 ) := ( others => '0' );\n";

  comp_table[ comp_name.str() ] = new COMP_TYPE( bw_a , bw_b , true , comp_out_bw , REG_COMP );  //comp_data;



  
  
 
}

void VHDLGen::gen_input_node(  Node * CNode )
{
 
  Edge *out_edge;
 
  out_edge = c_dag->edge_vec[ CNode->out_edges.back() ];
  
  add_internal_signal( out_edge );
  
  int bw_out  = (edge_bwdata[out_edge])->total_bw;

  int bw_a = bw_out;
  int bw_b = 0;
  
  
  int comp_out_bw = bw_a + bw_b;

  stringstream comp_name;
  comp_name << "reg_" << bw_a << "x" << bw_b;
 
   struct_code << endl << "-- INReg Node  " << endl;


  struct_code << "INREG_" << CNode->node_name << ": " << comp_name.str() << endl;

  struct_code << "port map ( " << endl;

  struct_code << "D => " << signal_names[ out_edge ] << "_in ,"<< endl;
  struct_code << "Q => " << signal_names[ out_edge ] << ","<< endl;
  struct_code << "CLK => clk"; 
 
  struct_code << ");\n\n";

  sig_dec << "\tsignal " << signal_names[ out_edge ] << " : ";

  sig_dec << "std_logic_vector( " << bw_out << " - 1 downto 0 ) := ( others => '0' );\n";

  comp_table[ comp_name.str() ] = new COMP_TYPE( bw_a , bw_b , true , comp_out_bw , REG_COMP );  //comp_data;



}

void VHDLGen::gen_entity_sec()
{

  entity_code << "entity bitsize_test is " << endl << endl;


  entity_code << "\t port ( " << endl;

 
  for ( vector<Edge*>::iterator s_itr = input_signals.begin();
	s_itr != input_signals.end(); s_itr++ )
    {
     
      entity_code << signal_names[ (*s_itr) ] << "_in : in ";
      entity_code << "std_logic_vector( " << (edge_bwdata[ (*s_itr) ])->total_bw;
      entity_code << " - 1 downto 0  );" << endl;
    }

  entity_code << "clk : in std_logic;\n"; 

  for ( vector<Edge*>::iterator s_itr = output_signals.begin();
	s_itr != output_signals.end(); s_itr++ )
    {
     
      entity_code << signal_names[ (*s_itr) ] << "_out : out ";
      entity_code << "std_logic_vector( " << (edge_bwdata[ (*s_itr) ])->total_bw;
      entity_code << " - 1 downto 0  )";
      
       if ( s_itr + 1 != output_signals.end() )
	 entity_code << ";" << endl;
     
     
    }

   entity_code << ");" << endl;

    entity_code << endl << "end bitsize_test; " << endl;


}


void VHDLGen::gen_arch_sec()
{

  arch_code << "architecture bitsize_test_arch of bitsize_test is  " << endl;

  
 for ( vector<Edge*>::iterator s_itr = internal_signals.begin();
	s_itr != internal_signals.end(); s_itr++ )
    {
     
      sig_dec << "\tsignal " << signal_names[ (*s_itr) ] << " : ";
      sig_dec << "std_logic_vector( " << (edge_bwdata[ (*s_itr) ])->total_bw;

      sig_dec << " - 1 downto 0  ) := ( others => '0' );" << endl;

    }

 arch_code << sig_dec.str() << endl;

}


void VHDLGen::gen_struct_sec()
{

  struct_code << "begin" << endl << endl;

  do_vhdlgen();

  struct_code << "end bitsize_test_arch;" << endl;

}


void VHDLGen::gen_components()
{

  comp_code << "attribute syn_black_box : boolean;\n\n";

  COMP_TYPE *comp_data;
  for ( map< string , COMP_TYPE * >::iterator c_itr = comp_table.begin();
	c_itr != comp_table.end(); c_itr++ ){
 
    comp_data = (*c_itr).second;

    comp_code << "\t-- BS_GEN_CORE "; 

    comp_code << (*c_itr).first;

    if ( comp_data->clocked )
      comp_code << " REG";
    else
      comp_code << " NOREG";

    if ( comp_data->c_type == CMUL_COMP ){

      comp_code << " " << comp_data->const_value;

    }

    comp_code << "\ncomponent " << (*c_itr).first << endl;

   
    
    comp_code << "port ( " << endl;

    if (( comp_data->c_type == ADD_COMP ) || ( comp_data->c_type == MUL_COMP )) {

    comp_code << "a: in std_logic_vector( " << comp_data->bw_a << " -1 downto 0 );" << endl;

    comp_code << "b: in std_logic_vector( " << comp_data->bw_b << " -1 downto 0 );" << endl;

    }
    else if ( comp_data->c_type == CMUL_COMP ){

     comp_code << "a: in std_logic_vector( " << comp_data->bw_a << " -1 downto 0 );" << endl;


    }
    else 
      {

	comp_code << "D: in std_logic_vector( " << comp_data->bw_a << " -1 downto 0 );" << endl;
	
      }

    switch( comp_data->c_type ){

    case ADD_COMP : comp_code << "s: out std_logic_vector( " << comp_data->bw_c << " -1 downto 0 )";break;
      
    case MUL_COMP : comp_code << "o: out std_logic_vector( " << comp_data->bw_c << " -1 downto 0 )";break;

    case CMUL_COMP : comp_code << "o: out std_logic_vector( " << comp_data->bw_c << " -1 downto 0 )";break;


    case REG_COMP : comp_code <<  "Q: out std_logic_vector( " << comp_data->bw_a << " -1 downto 0 )";break;

    default:;

    }

    if ( comp_data->clocked )
     comp_code << ";\nCLK : in std_logic" << endl;

     comp_code << ");" << endl;

     comp_code << "end component; \n\n";

     comp_code << "attribute syn_black_box of " << (*c_itr).first;

     comp_code << ": component is true;" << endl;
     
  }  

 

}

void VHDLGen::update_edges_bw()
{

  

  Edge *CEdge;

  string var_name,var_arithtype , var_type;

  unsigned int int_bw, frac_bw;

  HWfix *fxvar;

  map<string,Edge*>::iterator edg_itr;
  

  char *bwout_file;

  bwout_file = getenv( "BWFILE_OUT" );

  fstream bwout;

  if ( bwout_file == NULL )
    {

    
      bwout.open( "bs_bwdata_out.dat" ,ios::out );
      

    }
    else
    {
      bwout.open( bwout_file , ios::out );

    }


  for(  edg_itr = varnames_map.begin(); edg_itr != varnames_map.end(); edg_itr++ ){

    CEdge = (*edg_itr).second;

    fxvar = dynamic_cast<HWfix *>( CEdge->my_var );

    edge_bwdata[ CEdge ] = new BW_DATA( fxvar->int_bw , fxvar->frac_bw ); 


    bwout << (*edg_itr).first << " " << "FIX ";

    bwout << c_dag->Edge_TYPENAME[ CEdge->edge_type ] << " ";

    bwout << fxvar->int_bw << " " <<  fxvar->frac_bw << endl;

  }

  bwout.close();

}


void VHDLGen::update_edges_bw( string bwfilename )
{

  //string bwfilename = "bitsize_data.dat";

  
  fstream bw_fs( bwfilename.c_str() , ios::in );

  INTERNAL_ASSERT_EXIT( bw_fs.is_open() , "Error Opening " << bwfilename );

  Edge *CEdge;

  string var_name,var_arithtype , var_type;

  unsigned int int_bw, frac_bw;

  while( bw_fs >> var_name >> var_arithtype >> var_type >> int_bw >> frac_bw ){
    
    CEdge = varnames_map[ var_name ];

    edge_bwdata[ CEdge ] = new BW_DATA( int_bw , frac_bw ); 

  }


}

void VHDLGen::gen_test_bench( ostream &os ){


  stringstream tb_top;
  stringstream tb_arch;
  stringstream tb_str;

  gen_tb_top( tb_top );

  gen_tb_arch( tb_arch );

  gen_tb_struct( tb_str );



  os << tb_top.str() << endl;

  os << tb_arch.str() << endl;

  os << tb_str.str() << endl;


}


void VHDLGen::gen_tb_top(stringstream &tb_top){


  tb_top << "LIBRARY ieee;\n";
  tb_top << "USE ieee.std_logic_1164.ALL;\n";
  tb_top << "USE IEEE.std_logic_unsigned.all;\n";
  tb_top << "USE ieee.std_logic_arith.ALL;\n"; 
  tb_top << "USE std.textio.ALL;\n"; 
 
 
  tb_top << "\n\nentity bitsize_tb is\n";

#ifdef TB_EXT_CLK
  tb_top << "port( \n";
  tb_top << "sys_clk : in std_logic\n";
  tb_top << ");";
#endif

  tb_top << "\nend bitsize_tb;\n\n"; 


}


void VHDLGen::gen_tb_arch(stringstream &tb_arch){

  tb_arch << "architecture tb_arch of bitsize_tb is \n\n"; 
  

  tb_arch << "component bitsize_test\n";

  tb_arch << "port (\n";

  for ( vector<Edge*>::iterator s_itr = input_signals.begin();
	s_itr != input_signals.end(); s_itr++ )
    {

      tb_arch << signal_names[ (*s_itr) ] << "_in : in ";
      tb_arch << "std_logic_vector( " << (edge_bwdata[ (*s_itr) ])->total_bw;
      tb_arch << " - 1 downto 0  );" << endl;

    }

  tb_arch << "clk : in std_logic;\n";
  
  for ( vector<Edge*>::iterator s_itr = output_signals.begin();
	s_itr != output_signals.end(); s_itr++ )
    {

      tb_arch << signal_names[ (*s_itr) ] << "_out : out ";
      tb_arch << "std_logic_vector( " << (edge_bwdata[ (*s_itr) ])->total_bw;
      tb_arch << " - 1 downto 0  )";

      if ( s_itr + 1 != output_signals.end() )
	tb_arch << ";";

	
	      
      tb_arch << endl;

    }


   tb_arch << ");\nend component; " << endl;

#ifndef TB_EXT_CLK
   tb_arch << "constant PERIOD : time := 100 ns;\n";

   tb_arch << "signal sys_clk : std_logic := '0';\n";
#endif

    for ( vector<Edge*>::iterator s_itr = input_signals.begin();
	s_itr != input_signals.end(); s_itr++ )
    {
     
      tb_arch << "\tsignal " << signal_names[ (*s_itr) ] << "_temp : ";
      tb_arch << "std_logic_vector( " << (edge_bwdata[ (*s_itr) ])->total_bw;

      tb_arch << " - 1 downto 0  ) := ( others => '0' );" << endl;

    }

      for ( vector<Edge*>::iterator s_itr = output_signals.begin();
	s_itr != output_signals.end(); s_itr++ )
    {
     
      tb_arch << "\tsignal " << signal_names[ (*s_itr) ] << "_temp : ";
      tb_arch << "std_logic_vector( " << (edge_bwdata[ (*s_itr) ])->total_bw;

      tb_arch << " - 1 downto 0  ) := ( others => '0' );" << endl;

    }
   
      tb_arch << endl;

      tb_arch << "function str_to_stdvec(inp: string) return std_logic_vector is" << endl;

      tb_arch << "variable temp: std_logic_vector(inp'range) := (others => 'X');" << endl;

      tb_arch << "begin\n";
      tb_arch << "\tfor i in inp'range loop" << endl;
      tb_arch << "\t\tif (inp(i) = '1') then" << endl;
      tb_arch << "\t\t\ttemp(i) := '1';" << endl;
      tb_arch << "\t\telsif (inp(i) = '0') then" << endl;
      tb_arch << "\t\t\ttemp(i) := '0';" << endl;
      tb_arch << "\t\tend if;" << endl;
      tb_arch << "\tend loop;\n";
      tb_arch << "return temp;\n";
      tb_arch << "end function str_to_stdvec;\n\n";


}


void VHDLGen::gen_tb_struct( stringstream &tb_str ){

  tb_str << "begin  -- tb_arch \n\n";

#ifndef TB_EXT_CLK
   tb_str << "sys_clk <= not sys_clk after PERIOD/2;\n";
#endif


  int no_of_inputs = input_signals.size();

  tb_str << "readin_proc: process(sys_clk)\n"; 

  for ( int i = 1 ; i < no_of_inputs + 1 ; i++ ){

    tb_str << "\tFILE f" << i << "\t  : TEXT OPEN read_mode IS \"input" << i << ".dat\";\n";
    tb_str << "\tVARIABLE in_buf" << i << "\t : LINE;\n";
    //tb_str << "\tVARIABLE in_var" << i << "\t : natural;\n";
    tb_str << "\tVARIABLE in_var" << i << "\t : std_logic_vector(63 downto 0);\n";
    tb_str << "\tVARIABLE str_in_var" << i << "\t : string(64 downto 1);\n";
  }

  tb_str << "\n\nbegin\n\n";

  tb_str << "if falling_edge(sys_clk) and not endfile(f1) then \n\n";

  for ( int i = 1 ; i < no_of_inputs + 1 ; i++ ){

    tb_str << "\treadline( f" << i << ", in_buf"<< i <<");\n";

    tb_str << "\tread( in_buf" << i << ", str_in_var" << i << ");\n";

    tb_str << "\tin_var" << i << " := str_to_stdvec (str_in_var" << i << ");\n";

    tb_str << "\t" << signal_names[ input_signals[ i - 1 ] ] << "_temp <= ";

    
    //tb_str << "conv_std_logic_vector( in_var" << i << " , ";
    //tb_str << (edge_bwdata[ input_signals[ i - 1] ])->total_bw << " );\n";

    tb_str << "in_var" << i << "( ";
    tb_str << (edge_bwdata[ input_signals[ i - 1] ])->total_bw - 1 << " downto 0 );\n";
   }

  tb_str << "end if;\n";

  tb_str << "end process readin_proc;\n";

  tb_str << "writein_proc: process(sys_clk)\n\n";

  int no_of_outputs = output_signals.size();

   for ( int i = 1 ; i < no_of_outputs + 1 ; i++ ){

    tb_str << "FILE out_f" << i << "\t  : TEXT OPEN write_mode IS \"output" << i << ".dat\";\n";
    tb_str << "VARIABLE out_buf" << i << "\t : LINE;\n";
    tb_str << "VARIABLE out_var" << i << "\t : natural;\n";

  }

   
   tb_str << "begin \nif rising_edge(sys_clk) then \n\n";

    for ( int i = 1 ; i < no_of_outputs + 1 ; i++ ){

      tb_str << "\tout_var" << i << " := ";
      
      tb_str << "conv_integer( " << signal_names[ output_signals[ i - 1 ] ] << "_temp );\n";

    tb_str << "\twrite( out_buf" << i << ", out_var" << i << ");\n";


    tb_str << "\twriteline( out_f" << i << ", out_buf"<< i <<");\n";

    
   }

  tb_str << "end if;\n";

  tb_str << "end process writein_proc;\n\n";

  
  tb_str << "tb_comp : bitsize_test \n";

  tb_str << "port map ( \n";

   for ( vector<Edge*>::iterator s_itr = input_signals.begin();
	s_itr != input_signals.end(); s_itr++ )
    {

      tb_str << signal_names[ (*s_itr) ] << "_in => ";
      tb_str << signal_names[ (*s_itr) ] << "_temp,\n";
    
    }
  
   tb_str <<  "clk => sys_clk,\n";

  for ( vector<Edge*>::iterator s_itr = output_signals.begin();
	s_itr != output_signals.end(); s_itr++ )
    {

      tb_str << signal_names[ (*s_itr) ] << "_out => ";
      tb_str << signal_names[ (*s_itr) ] << "_temp";

      if ( s_itr + 1 != output_signals.end() )
	tb_str << ",";
      
      tb_str << endl;
    

    }


  tb_str << ");\n";


  tb_str << "\nend tb_arch;\n";

 
 
}
