%test2

% f = a + b
clear all;

fbw = 10;

a = rand_gen( 2 , 3  , 10000);

b = rand_gen( 2 , 3 , 10000);

a_r = lns_round( a , fbw );
b_r = lns_round( b , fbw );

%e_a = a - a_r;

e_a = round_err( 2 , fbw );

e_b = round_err( 2 , fbw );


%e_b = b - b_r;

c_r = a_r .* b_r;

c = a .* b;

e = c - c_r;

ef = b.*e_a+ a.*e_b;

E = [ (e-ef)./getulp( c , fbw) ];