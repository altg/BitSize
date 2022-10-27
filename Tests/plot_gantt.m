function plot_gantt( val , ndx , marker )

plot( val , ones(1,2) *  ndx , marker );
text( val(1) ,  ndx   + .1 , 'D' );
text( val(2) ,  ndx   + .1 , 'S' );

