#include "bsclass.h"

// $Id: bsclass.cpp,v 1.1 2004/05/14 17:33:20 amag97 Exp $
// $Author: amag97 $

BSClass::BSClass()
{


        advarType_name.insert( pair< advar::advarType , string>( advar::TMP , "TMP" ) );
	advarType_name.insert( pair< advar::advarType , string>( advar::IN , "IN" ) );
	advarType_name.insert( pair< advar::advarType , string>( advar::OUT , "OUT" ) );
	advarType_name.insert( pair< advar::advarType , string>( advar::AUTO_TMP , "AUTO_TMP" ) );


}


BSClass::~BSClass()
{

}
int BSClass::add_to_list( advar* ad )
{

	switch ( ad->varType )
	{
		case advar::IN: input_varlist.push_back( ad ); break;
		case advar::OUT: input_varlist.push_back( ad ); break;
		case advar::TMP: temp_varlist.push_back( ad ); break;
//		case AUTO_TMP: atemp_varlist.push_back( ad ); break; 
//		default:
	}
	
	
	return 0;

}


void BSClass::test_func()
{

	cout << "test func" << endl;

}



void BSClass::print_all()
{

  advar_veclist::iterator iter;

  for( iter = advar_vec.begin() ; iter != advar_vec.end() ; iter++ )
    {

      cout << *(*iter) << endl;
    }

}

void printName( advar* ad )
	{
		cout << ad->var_name << endl;
	}

void BSClass::show_lists()
{

	

	cout << "Input Vars " << endl;

	for_each( input_varlist.begin() , input_varlist.end() , printName );
 


}

