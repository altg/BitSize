#include "hwint.h"
#include "hwfix.h"

HWint::HWint(arch_attr Signal_type, fsize init_size , sign_attr sign, char *name)
{

  var_signaltype = Signal_type;

  var_size = init_size;

  var_sign = sign;
  
  var_name = ( name != NULL ) ? name : "NO_NAME";


}


HWint& HWint::operator=( int int_val )
{

  var_val = int_val;
 
return *this; }


HWint& HWint::operator=( const HWint &r )
{

  var_val = r.var_val;
 
  return *this; 
}


HWint& HWint::operator=( const HWfix &r )
{

  var_val = (int)r.ad_val.ref_value;

  
  if ( bs->global_opmode == PARSING )
  {
    Edge* hwint_edge = bs->mydag->new_hwint_edge();
    bs->mydag->add_out_edge( bs->mydag->node_vec[ r.myedge->src_nodes.back() ] , hwint_edge );

  }

  return *this; 
}

HWint HWint::operator[]( int index )
{

  HWint temp;
  
  int bit_mask = 1;
  
  if ( index > 31 ) ERROR( "Index " << index << " exceeds HWint bitwidth 32" )

    temp.var_val = ( var_val & ( bit_mask << index )) >> index; 

  
  return temp;

}
  

HWint HWint::operator[]( const HWint& index )
{

  HWint temp;
  
  int bit_mask = 1;
  
  if ( index.var_val > 31 ) ERROR( "Index " << index.var_val << " exceeds HWint bitwidth 32" )

 

   temp.var_val = ( var_val & ( bit_mask << index.var_val )) >> index.var_val; 

  return temp;
  

}

HWint& HWint::operator+= (const HWint &r)
{

  var_val += r.var_val;


return *this; }


HWint::operator int()
{
  return var_val;
  
}


HWint& HWint::operator-= (const HWint &r)
{

  var_val -= r.var_val;


return *this; }


HWint& HWint::operator*= (const HWint &r)
{

  var_val *= r.var_val;

return *this; }


HWint& HWint::operator/= (const HWint &r)
{

  var_val /= r.var_val;

return *this; }


HWint& HWint::operator%= (const HWint &r)
{

 var_val %= r.var_val;

return *this; }

HWint& HWint::operator|= (const HWint &r)
{

  var_val |= r.var_val;
return *this; }


HWint& HWint::operator&= (const HWint &r)
{

  var_val &= r.var_val;
return *this; }


HWint& HWint::operator^= (const HWint &r)
{
 var_val ^= r.var_val;

return *this; }


HWint& HWint::operator+= (int val)
{
  var_val += val;
return *this; }

HWint& HWint::operator-= (int val)
{
  var_val -= val;
return *this; }

HWint& HWint::operator*= (int val)
{
  var_val *= val;
return *this; }

HWint& HWint::operator/= (int val)
{
  var_val /= val;
return *this; }

HWint& HWint::operator%= (int val)
{
  var_val %= val;
return *this; }

HWint& HWint::operator|= (int val)
{
  var_val |= val;
return *this; }

HWint& HWint::operator&= (int val)
{
  var_val &= val;

return *this; }
  

HWint& HWint::operator^= (int val)
{
  var_val ^= val;
return *this; }


HWint& HWint::operator++ ()
{
  var_val++;

  return *this;

}
  

//HWint& operator++ (HWint &l,int);
  
HWint& HWint::operator-- ()
{
  var_val--;

  return *this;
}

  
//HWint& operator-- (HWint &l,int);

HWint operator+  (const HWint &l,const HWint &r)
{
  HWint temp;

  temp.var_val = l.var_val + r.var_val;

  return temp;
}

HWint operator+  (const HWint &l,int val)
{

 HWint temp;

 temp.var_val = l.var_val + val;

 return temp;


}

HWint operator+  (int val,const HWint &r)
{
  HWint temp;

  temp.var_val = val + r.var_val;

  return temp;


}


HWint operator-  (const HWint &l,const HWint &r)
{
  HWint temp;

  temp.var_val = l.var_val - r.var_val;

  return temp;
}

HWint operator-  (const HWint &l,int val)
{

 HWint temp;

 temp.var_val = l.var_val - val;

 return temp;


}

HWint operator-  (int val,const HWint &r)
{
  HWint temp;

  temp.var_val = val - r.var_val;

  return temp;


}


HWint operator*  (const HWint &l,const HWint &r)
{
  HWint temp;

  temp.var_val = l.var_val * r.var_val;

  return temp;
}

HWint operator*  (const HWint &l,int val)
{

 HWint temp;

 temp.var_val = l.var_val * val;

 return temp;


}

HWint operator*  (int val,const HWint &r)
{
  HWint temp;

  temp.var_val = val * r.var_val;

  return temp;


}

HWint operator/  (const HWint &l,const HWint &r)
{
  HWint temp;

  temp.var_val = l.var_val / r.var_val;

  return temp;
}

HWint operator/  (const HWint &l,int val)
{

 HWint temp;

 temp.var_val = l.var_val / val;

 return temp;


}

HWint operator/  (int val,const HWint &r)
{
  HWint temp;

  temp.var_val = val / r.var_val;

  return temp;


}



HWint operator>> (const HWint &l, const HWint &r)
{
  HWint temp;

  temp.var_val = l.var_val >> r.var_val;

  return temp;
  
}
   
HWint operator>> (const HWint &l,int val)
{

  HWint temp;

  temp.var_val = l.var_val >> val;

  return temp;

}

HWint operator>> (int val,const HWint &r)
{
  
  HWint temp;

  temp.var_val = val >> r.var_val;

  return temp;


}



HWint operator<< (const HWint &l,const HWint &r)
{

  HWint temp;

  temp.var_val = l.var_val << r.var_val;

  return temp;


}


HWint operator<< (const HWint &l,const int val)
{

  HWint temp;

  temp.var_val = l.var_val << val;

  return temp;

}

HWint operator<< (const int val,const HWint &r)
{

  HWint temp;

  temp.var_val = val << r.var_val;

  return temp;

}


HWint operator>  (const HWint &l,const HWint &r)
{

  HWint temp;

  temp.var_val = l.var_val > r.var_val;

  return temp;

}





HWint operator>  (int val,const HWint &r)
{

  HWint temp;

  temp.var_val = val > r.var_val;

  return temp;


}

HWint operator>  (const HWint &l,int val)
{

  HWint temp;

  temp.var_val = l.var_val > val;

  return temp;


}


HWint operator<  (const HWint &l,const HWint &r)
{

  HWint temp;

  temp.var_val = l.var_val < r.var_val;

  return temp;

}





HWint operator<  (int val,const HWint &r)
{

  HWint temp;

  temp.var_val = val < r.var_val;

  return temp;


}

HWint operator<  (const HWint &l,int val)
{

  HWint temp;

  temp.var_val = l.var_val < val;

  return temp;


}

HWint op_encoder( const HWint& var )
{

  HWint temp;

  double log_val =  log( (double)var.var_val ) / log(2.);

  temp.var_val = (int)log_val;

  return temp;

}
