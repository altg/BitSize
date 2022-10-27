// Class to log power data for each signal

class DataLog{

public:
  DataLog();

  void operator()( double val );
  void cal_values();

  double mean;
  double var;
  double acoef_1;

  int k;

private:
  double s1,s2,s1s2,s1_sqr;
  double prev_val;
  

};
