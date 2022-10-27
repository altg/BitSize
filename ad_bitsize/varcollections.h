// $Author: amag97 $
// $Id: varcollections.h,v 1.4 2004/06/24 18:45:31 amag97 Exp $
// $Log: varcollections.h,v $
// Revision 1.4  2004/06/24 18:45:31  amag97
// Added a seperate class to encapsulate the bitwidth optimisation functions
//
// Revision 1.3  2004/06/07 19:51:08  amag97
// Global HWvector collection
// Bugfixes to copy constructor
//
// Revision 1.2  2004/05/28 20:14:27  amag97
// ASC Integration
//

#ifndef _VARCOLLECTIONS_H
#define _VARCOLLECTIONS_H




#include<list>
#include<vector>

using namespace std;

template <class T>
class varcollections
{

public:
  varcollections()
  {};

  list<T> invar_c;
  list<T> outvar_c;
  list<T> tmpvar_c;

 

  vector<T> hwvar_c;

  typedef typename list<T>::iterator iter_t;


};

#endif
