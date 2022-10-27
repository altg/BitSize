function ulp = getulp( v , fbw )

l1 = log2( v );

I = ceil( l1 );

ulp = 2.^( I - 2^-fbw-1 ) - 2.^( I - 2^-fbw );

%ulp = 2.^( I + 2^-fbw-1 );