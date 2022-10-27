#include "varnames.h"

// $Id: varnames.cpp,v 1.3 2004/07/06 19:48:12 amag97 Exp $
// $Author: amag97 $

#include <sstream>

varnames::varnames(string vpfix , string vsfix ,int vstart , varmodetype vmode ) 
{
  var_count = 0;

  prefix = vpfix;
  suffix = vsfix;

  var_start = vstart;

  var_mode = vmode;
}

varnames::~varnames()
{

  

}

string varnames::gen_varname()
{

  

  stringstream ss;
  string temp;

  if ( var_mode == ALPHA )
    {
        ss << (var_count++ + 'A');

    }
  else 
    {
       ss << var_count++;

    }

 
  
   temp = prefix + "_" + ss.str();

   //ss << prefix << "_" << var_count++;

    if ( suffix != "" )
    {
      temp = temp + "_" + suffix;
    }

  return temp;

  
}

string varnames::gen_varname( string prefix2 )
{

  stringstream ss;

  if ( var_mode == ALPHA )
    {
        ss << (var_count++ + 'A');

    }
  else 
    {
        ss << var_count++;

    }

  string temp;

  temp = prefix2 + "_" + ss.str();

  if ( suffix != "" )
    {
      temp = temp + "_" + suffix;
    }

  return temp;


}
