#include "node_sched.h"

NodeSchedular::NodeSchedular( DAG* _dag ):c_dag( _dag ),graph_scheduled(false),nodes_sorted(false){}


int NodeSchedular::schedule_innodes( dag_index c_node_num )
{

	if ( node_scheduled[ c_node_num ] ) // If node is already scheduled do nothing
		return node_sched_no[ c_node_num ];
	else
	{   // Schedule all innode and find the nodes sched_no
		int c_node_sched = 0;
		
		Node *CNode = c_dag->node_vec[ c_node_num ];
		vector<dag_index>::iterator e_it = CNode->in_edges.begin();
		vector<dag_index>::iterator e_end = CNode->in_edges.end();
		
		int temp;
		while( e_it != e_end )
		{
		  temp = schedule_innodes( (c_dag->edge_vec[(*e_it)])->get_src_node() );
			c_node_sched = ( temp > c_node_sched ? temp : c_node_sched );
			e_it++;
		}
		
		node_sched_no[ c_node_num ] = c_node_sched + 1;

		node_scheduled[ c_node_num ] = true;
		scheduled_nodes++;
		
		return node_sched_no[ c_node_num ];
	}


}


void NodeSchedular::schedule_graph()
{

  if ( graph_scheduled == true ) return; 

  scheduled_nodes = 0;

  vector<Node *>::iterator n_itr = c_dag->node_vec.begin();
		
  for( ; n_itr != c_dag->node_vec.end() ; n_itr++ ){

    node_scheduled[ (*n_itr)->node_number ] = false;
    node_sched_no[ (*n_itr)->node_number ] = 0;
  }


  int total_nodes = c_dag->node_vec.size();


  

  while ( scheduled_nodes < total_nodes )
    {
	
      vector<dag_index>::iterator e_itr = c_dag->input_edges.begin();
	  
      for( ; e_itr != c_dag->input_edges.end() ; e_itr++ ){

	node_scheduled[ (*e_itr) ] = true;
	node_sched_no[ (*e_itr) ] = 1;

	scheduled_nodes++;
      }


	  
      n_itr = c_dag->node_vec.begin();
	
			
      while( n_itr != c_dag->node_vec.end() ){
	schedule_innodes( (*n_itr)->node_number );
	n_itr++;
      }

    }
	
  graph_scheduled = true;

	
}

void NodeSchedular::sort_schedule()
{
  
  if ( nodes_sorted == true ) return; 

  schedule_graph();

  typedef pair < int , dag_index > SC_PAIR;

  typedef multimap < int , dag_index > SC_MAP_T;

  SC_MAP_T sc_map;

  pair <SC_MAP_T::iterator, SC_MAP_T::iterator> f_res;

 
  vector<Node *>::iterator n_itr = c_dag->node_vec.begin();

  dag_index node_num;

  for( ; n_itr != c_dag->node_vec.end() ; n_itr++ ){
	  
    node_num = (*n_itr)->node_number;
    sc_map.insert( SC_PAIR( node_sched_no[ node_num ] , node_num ) );

  }


  node_vec_type *temp_vec;

  bool sched_found = true;

   for( int i = 0 ; sched_found ; i++ )
    {

      temp_vec = new node_vec_type::vector();

      f_res = sc_map.equal_range( i );

      if ( ( f_res.first == sc_map.end( ) ) && ( f_res.second == sc_map.end( ) ) )
	{
	  
	  sched_found = false;
        }
      else
	{
	  for( ;f_res.first != f_res.second ;f_res.first++) {
	    temp_vec->push_back( f_res.first->second );
	  }
	}
      
      schedule.push_back( *temp_vec );
       
     
	  
    }


   nodes_sorted = true;

}





