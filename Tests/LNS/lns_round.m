function r = lns_round( v , fbw )

l1 = log2( v );

rl1 = floor( l1 .* 2^fbw ) .* 2^-fbw;

r = 2.^( rl1 );