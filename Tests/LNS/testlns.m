%test round

%v = 5.657966225442204e-002;

v = rand( 100000,1 );

fbw = 2;

rv1 = lns_round( v , fbw );

e1 = v - rv1;

ulp = getulp( v , fbw );

R1 = rv1 ./ v;

R2 = (e1) ./ ulp;