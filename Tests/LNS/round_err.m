function e = round_err( ibw , fbw )

e = 2^( 2^ibw - 2^(-fbw - 1) ) -  2^( 2^ibw - 2^(-fbw) );