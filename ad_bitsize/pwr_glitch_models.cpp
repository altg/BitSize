#include "bitsize_pass.h"



PwrPassGlitch::GLITCH_PROF* PwrPassGlitch::G_Prof_Add_ng( Edge* edge_l , Edge* edge_r )
{

  GLITCH_PROF *G_prof = new GLITCH_PROF();

  double P_ADD_k[] = { 0.5757, -0.3266, -0.0101, -0.0644 };

  double P_ADD_h[] = { 0.0134, -0.0017, -0.0192,  2.5109 };

  HWfix* var_l = dynamic_cast<HWfix*>(edge_l->my_var);
  HWfix* var_r = dynamic_cast<HWfix*>(edge_r->my_var);

  int bw_l = var_l->int_bw + var_l->frac_bw;
 
  int bw_r = var_r->int_bw + var_r->frac_bw; 


  //cout << "L :" << bw_l << " R:" << bw_r << endl;

  double peak_pos = max( bw_l , bw_r ) * P_ADD_k[0] + edge_l->data_log.acoef_1 * P_ADD_k[1];

  peak_pos += edge_r->data_log.acoef_1 * P_ADD_k[2] + P_ADD_k[3];


  G_prof->peak_pos = static_cast<int>( peak_pos );


  //cout <<  G_prof->peak_pos << endl;
  
  G_prof->peak_height = 20.;
  G_prof->bit_width = 16;

  return G_prof;


}


PwrPassGlitch::GLITCH_PROF* PwrPassGlitch::G_Prof_Sub_ng( Edge* edge_l , Edge* edge_r )
{

  GLITCH_PROF *G_prof = new GLITCH_PROF();

  G_prof->peak_pos = 1;
  G_prof->peak_height = 20.;
  G_prof->bit_width = 16;

  return G_prof;


}

PwrPassGlitch::GLITCH_PROF* PwrPassGlitch::G_Prof_Mul_ng( Edge* edge_l , Edge* edge_r )
{

  GLITCH_PROF *G_prof = new GLITCH_PROF();

  double P_MUL_k[] = { 1.0673, -0.2212, -0.0639, 1.0311 };

  double P_MUL_h[] = { 22.8325, -2.6574, -5.6276, -209.6078 };

  

  HWfix* var_l = dynamic_cast<HWfix*>(edge_l->my_var);
  HWfix* var_r = dynamic_cast<HWfix*>(edge_r->my_var);

  int bw_l = var_l->int_bw + var_l->frac_bw;
 
  int bw_r = var_r->int_bw + var_r->frac_bw; 


  cout << "L :" << bw_l << " R:" << bw_r << endl;

  double peak_pos = ( bw_l + bw_r ) * P_MUL_k[0] + edge_l->data_log.acoef_1 * P_MUL_k[1];

  peak_pos += edge_r->data_log.acoef_1 * P_MUL_k[2] + P_MUL_k[3];


  G_prof->peak_pos = static_cast<int>( peak_pos );
  G_prof->peak_height = 20.;
  G_prof->bit_width = 16;

  return G_prof;


}

PwrPassGlitch::GLITCH_PROF* PwrPassGlitch::G_Prof_Add_g( GLITCH_PROF* G_prof_l, GLITCH_PROF* G_prof_r  )
{

  GLITCH_PROF *G_prof = new GLITCH_PROF();

  G_prof->peak_pos = 1;
  G_prof->peak_height = 20.;
  G_prof->bit_width = 16;

  return G_prof;


}


PwrPassGlitch::GLITCH_PROF* PwrPassGlitch::G_Prof_Sub_g( GLITCH_PROF* G_prof_l, GLITCH_PROF* G_prof_r  )
{

  GLITCH_PROF *G_prof = new GLITCH_PROF();

  G_prof->peak_pos = 1;
  G_prof->peak_height = 20.;
  G_prof->bit_width = 16;

  return G_prof;


}

PwrPassGlitch::GLITCH_PROF* PwrPassGlitch::G_Prof_Mul_g( GLITCH_PROF* G_prof_l, GLITCH_PROF* G_prof_r  )
{

  GLITCH_PROF *G_prof = new GLITCH_PROF();

  G_prof->peak_pos = 1;
  G_prof->peak_height = 20.;
  G_prof->bit_width = 16;

  return G_prof;


}
