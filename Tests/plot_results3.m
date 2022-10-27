matrix2_uni = [ 7713   10209  ]
matrix2_nonuni = [ 5911   9086  ]

rgb2ycc_uni = [ 1681   2479  ]
rgb2ycc_nonuni = [ 1527   2468  ]

dct8_uni = [ 1971   2519  ]
dct8_nonuni = [ 1933   2368  ]

clear all

matdata

figure(1);
hold on;

plot_gantt( FIX_dct8_nonuni , 1 , 'r-x' );
p1 = 'dct8';

plot_gantt( FIX_matrix2_nonuni , 2 , 'r-x' );
p2 = 'matrix2';


plot_gantt( FIX_bspline_nonuni , 3 , 'r-x' );
p3 = 'bspline';

plot_gantt( FIX_rgb2ycc_nonuni , 4 , 'r-x' );
p4 = 'rgb2ycc';

axis( [ 1000 12000 0 5 ] );
set(gca , 'YTick' , [ 1 : 4] )
set(gca,'YTickLabel',{p1 , p2 , p3 , p4 });
set( gca ,  'FontSize' , 12 );



title('Post P&R Area Comparison - Fixed Point' , 'FontSize' , 12);
xlabel('Area (LUTs)' , 'FontSize' , 12 );
box on;



