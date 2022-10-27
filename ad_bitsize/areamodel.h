// Class to handle Area Model

// $Id: areamodel.h,v 1.1 2004/10/31 19:53:10 amag97 Exp $
// $Author: amag97 $
// $Date: 2004/10/31 19:53:10 $


#ifndef AREA_MODEL_H
#define AREA_MODEL_H

#include "dag.h"

class AreaModel{

public:
  AreaModel(DAG *);


  DAG *c_dag;


  double get_area();


};




#endif
