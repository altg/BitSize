// $Author: amag97 $
// $Id: hwvector.h,v 1.9 2005/12/13 14:25:40 amag97 Exp $

#ifndef _HWVECTOR_H
#define _HWVECTOR_H

#include "hwvar.h"
//class HWvar;


#include <vector>
#include <cstdlib>

#define ERROR(_x) { cerr << _x << endl; exit(1); }

using namespace std;


template <class TYPE>
class HWvector{ 
public:
 
  HWvector(int n,TYPE *value, int *initvals=NULL);  
  HWvector(int n,TYPE *value, double *initvals);

  ~HWvector();
  
  vector<TYPE*> var_vec;

  TYPE& operator[]( int index );

  TYPE* proto_type;

  int num_elms;
  
  

 
};






template <class TYPE>
HWvector<TYPE>::HWvector( int n,TYPE *value, int *initvals)
{

  char t_num[3];
    
  for ( int ctr = 0 ; ctr < n ; ctr++ )
    {

      TYPE * temp_var = new TYPE(*value); 
      var_vec.push_back( temp_var );
      sprintf( t_num , "%d" , ctr );
      temp_var->var_name = t_num + temp_var->var_name;
      
      temp_var->frac_bw_state = NOTSET;
      
      if ( initvals != NULL )
	*temp_var = initvals[ctr];
      else
	*temp_var = 0;


      temp_var->ad_val <<= 0.0;

    }
  
  num_elms = n;
}



template<class TYPE>
HWvector<TYPE>::HWvector(int n,TYPE *value, double *initvals)
{
  char t_num[5];
  //char *t_num;
  //t_num = new char[5];

  //value->var_dectype = VECVAR;

  proto_type = value;

   for ( int ctr = 0 ; ctr < n ; ctr++ )
    {
      TYPE * temp_var = new TYPE(*value); 
      var_vec.push_back( temp_var );
      sprintf( t_num , "%d" , ctr );
      temp_var->var_name = temp_var->var_name + "_" + string(t_num);
      temp_var->ad_val.fix_frac_state = NOTSET;
      temp_var->ad_val.fix_int_state = NOTSET;
      
      

      *temp_var = initvals[ctr];

       temp_var->frac_bw_state = SET;
       temp_var->int_bw_state = SET;
    }


   num_elms = n;

   TYPE::vec_c.push_back( this );

   value->frac_bw_state = SET;
   
}


template <class TYPE>
HWvector<TYPE>::~HWvector()
{
  cout << "HWvector dest called " << endl;

}

template<class TYPE>
TYPE& HWvector<TYPE>::operator[]( int index )
{

  if ( index < num_elms )
    return *var_vec[ index ];
  else
    {
      ERROR( "HWvector: index " << index << " larger than vector size " << num_elms )   
	return *(new HWfix());
    }

}



#endif
