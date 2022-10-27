#include "datalog.h"

DataLog::DataLog():mean(0.),var(0.),acoef_1(0),k(0),s1(0.),s2(0.),s1s2(0.),s1_sqr(0.),prev_val(0.){}


void DataLog::operator()( double val )
{
  k++;
  
  mean = (mean*(k - 1) + val) / k;
  
  // if( k > 1 ){
//     var = var * (k - 1)/k + (val - mean)*(val - mean)/(k-1);
//   }

  s1 += val;
 
  if ( k > 1 )
    s2 += val;

  s1s2 += val * prev_val;

  
  s1_sqr += val * val;

  prev_val = val;
  
  
  // Calculate mean , std , ac_1


}


void DataLog::cal_values(){


  double acc;

  var = (s1_sqr * k  - ( s1 * s1 ))/( k * (k - 1 ));

  s1 -= prev_val;
  
  acc = (s1s2 * k - (s1 * s2))/(k * ( k - 1 ));


  acoef_1 = acc/var;

}
