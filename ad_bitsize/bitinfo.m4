include( bitspec.m4 )

dnl Bit data transfer 
dnl m4 support macros

define( `_bitspec',  decbits( nvars ) $1 )

define( `decbits', size_t u_m[$1]; )

define( `_nvar_dec', const size_t no_of_vars = nvars; )

define( `_set_prec', for( int i = 0; i < no_of_vars; i++ ){
			 u[i].SetPrec( u_m[i] );
     			 Ru[i].SetPrec( $1 );
   			} ) 

define( `_decvarsetup', 
	           `MpFloat u[ nvars ] , Ru[ nvars ];
		   MpFloat diff, r_err,tt;
	           double max_err = 0.;'
     			)

define( `_cal_diff',
	           diff = u[no_of_vars -1 ] - Ru[no_of_vars - 1];
      		   r_err.rdiff( Ru[ no_of_vars-1 ] , u[ no_of_vars-1 ] );
      		   max_err = max( max_err , fabs((double)r_err) );
		       )

define( `_dec_str', fstream data_out( $1 , ios::out ); )

define( `_out_ulp', 
		 data_out << ( diff / u[no_of_vars - 1].get_ulp() ) << endl;
      )
