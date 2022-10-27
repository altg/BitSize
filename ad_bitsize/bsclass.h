#ifndef _BSClass_H_
#define _BSClass_H_

// $Id: bsclass.h,v 1.1 2004/05/14 17:33:20 amag97 Exp $
// $Author: amag97 $
// $Log: bsclass.h,v $
// Revision 1.1  2004/05/14 17:33:20  amag97
// Intgeration with ASC system
//

#include "advar.h"

#include <algorithm>
#include <map>

using namespace std;

class _DLLDEC_AD BSClass {

public:

	BSClass();

	~BSClass();

	typedef vector<advar *> advar_veclist;
	advar_veclist advar_vec;
	void print_all();

	typedef list<advar *> advar_list;
	advar_list fixed_varlist;
	advar_list output_varlist;
	advar_list input_varlist;
	advar_list temp_varlist;
	
        map< advar::advarType , string > advarType_name;

	int add_to_list( advar * ad );
	void show_lists();

	void test_func();

	
};



#endif
