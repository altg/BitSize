#ifndef _VARNAMES_H
#define _VARNAMES_H

// $Id: varnames.h,v 1.3 2004/07/06 19:48:12 amag97 Exp $
// $Author: amag97 $
// $Log: varnames.h,v $
// Revision 1.3  2004/07/06 19:48:12  amag97
// Revised DAG generation
//
// Revision 1.2  2004/05/28 20:14:27  amag97
// ASC Integration
//
// Revision 1.1  2004/05/14 17:33:20  amag97
// Intgeration with ASC system
//

#include <string>
#include <sstream>


using namespace std;

class varnames{

public:
 
  enum varmodetype { ALPHA , NUMERIC };
  
  varnames( string prefix = "VAR" , string sufix = "" ,  int vstart = 0 , varmodetype vmode = ALPHA );
  
  ~varnames();

  string gen_varname();
  
  string gen_varname( string );

  

private:
  int var_count;
  string prefix;
  string suffix;

  int var_start;

  varmodetype var_mode;

};



#endif
