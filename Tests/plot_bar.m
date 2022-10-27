% plot bar

clear all
matdata

D1 = [ FIX_dct8_nonuni(1) , FIX_matrix2_nonuni(1) , FIX_bspline_nonuni(1) ,  FIX_rgb2ycc_nonuni(1) ];
D2 = [ FIX_dct8_uni(1) , FIX_matrix2_uni(1) , FIX_bspline_uni(1) ,  FIX_rgb2ycc_uni(1) ];

bar( 1:4 , [ D1' D2' ] );

p1 = 'dct8';
p2 = 'matrix2';
p3 = 'bspline';
p4 = 'rgb2ycc';

set(gca,'XTickLabel',{p1 , p2 , p3 , p4 });
set( gca ,  'FontSize' , 12 );
title('Post P&R Area Usage - Fixed Point' , 'FontSize' , 12);
xlabel('Area (LUTs)' , 'FontSize' , 12 );
xlabel('Circuit' , 'FontSize' , 12 );
legend( 'nonuni' , 'uni' );
box on;
