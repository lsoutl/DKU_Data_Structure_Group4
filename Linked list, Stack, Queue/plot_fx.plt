# gnuplot script for f(x)
# Run with:  gnuplot plot_fx.plt
set title 'f(x) = 6.7x^4 + 3.2x^3 - x^2 + x - 2'
set xlabel 'x'
set ylabel 'f(x)'
set xrange [-5:5]
set grid
set arrow from -5,0 to 5,0 nohead lc rgb 'black' lw 1 dt 2
plot 'fx_data.dat' using 1:2 with lines lw 2 lc rgb 'blue' title 'f(x)'
pause -1 'Press Enter to close'
