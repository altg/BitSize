clear all

matdata2

figure(1);
hold on;

plot_gantt( FIX_matrix2_12_nonuni , 1 , 'r-x' );
p1 = '12';

plot_gantt(  FIX_matrix2_14_nonuni , 2 , 'r-x' );
p2 = '14';

plot_gantt( FIX_matrix2_16_nonuni , 3 , 'r-x' );
p3 = '16';

plot_gantt(  FIX_matrix2_18_nonuni , 4 , 'r-x' );
p4 = '18';

plot_gantt(  FIX_matrix2_20_nonuni , 5 , 'r-x' );
p5 = '20';


axis( [ 2500 12000 0 6 ] );
set(gca , 'YTick' , [ 1 : 5] )
set(gca,'YTickLabel',{p1 , p2 , p3 , p4 , p5 });
set( gca , 'FontSize' , 12 );

title('Post P&R Area Comparison - Fixed Point' , 'FontSize' , 12);
xlabel('Area (LUTs)' , 'FontSize' , 12 );
ylabel('Output Fractional bit-width', 'FontSize' , 12);
box on;
