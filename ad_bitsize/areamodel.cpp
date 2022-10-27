
#undef _BITSIZE_DEBUG_
#include "areamodel.h"

AreaModel::AreaModel(DAG *a_dag)
{

  c_dag = a_dag;

 
}


double AreaModel::get_area()
{

  
  double total_area = 0.;

  double add_area, sub_area,  mul_area, div_area;

  double add_count, sub_count, mul_count, div_count;

  add_area = sub_area = mul_area = div_area = 0.;

  add_count = sub_count = mul_count = div_count = 0;

  vector<Node *>::iterator n_iter;

  for ( n_iter = c_dag->node_vec.begin(); n_iter != c_dag->node_vec.end() ; n_iter++ )
    {

      switch( (*n_iter)->node_type )
	{

	case ADD: add_count++; add_area += (*n_iter)->get_area_usage(); break;


	case SUB: sub_count++; sub_area += (*n_iter)->get_area_usage(); break;


	case MUL: mul_count++; mul_area += (*n_iter)->get_area_usage(); break;


	case DIV: div_count++; div_area += (*n_iter)->get_area_usage(); break;


	defualt:;


	}

    }
  
  
  _DBOUT( "Area Usage " )
  _DBOUT( "===========" )
  _DBOUT( "Add\t: " << add_count << " : " << add_area << " luts " )
  _DBOUT( "Sub\t: " << sub_count << " : " << sub_area << " luts " )
  _DBOUT( "Mul\t: " << mul_count << " : " << mul_area << " luts " )
  _DBOUT( "Div\t: " << div_count << " : " << div_area << " luts " )
  

  total_area = add_area + sub_area + mul_area + div_area;

  _DBOUT( endl << "Total Area :" << total_area )

  


  return total_area;

}
