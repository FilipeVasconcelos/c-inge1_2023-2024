set term unknown
set xr [0:4]
set yr [0:4]
set style line 1 lc rgb 'blue' pt 5
plot 'data' with points ls 1 
replot 'bezier.log' u 1:2 w l 
set terminal png size 800,600 
set output '340.png'
replot
