# Enhanced plot_error_dt.gp

set terminal pngcairo enhanced font "Helvetica,14" size 1000,700
set output 'error_plot_dx.png'

set title "Convergence Study" font ",16"
set xlabel "ln(dx)" offset 0,0.5
set ylabel "ln(Error)" offset 1,0

set grid lw 1 lc rgb "#cccccc"
set key top right box opaque
set border lw 1.5

set style line 1 lc rgb '#1f77b4' pt 7 ps 1.8 lt 1 lw 2  # blue points
set style line 2 lc rgb '#d62728' lt 1 lw 2              # red line

filename = "error_dx.dat"

f(x) = a * x + b
fit f(x) filename using (log($1)):(log($2)) via a, b

set label sprintf("Slope (rate) = %.3f", a) at graph 0.05, 0.85 font ",14" tc rgb "black"

plot filename using (log($1)):(log($2)) title "Data" with points ls 1, \
     f(x) title sprintf("Fit: slope = %.3f", a) with lines ls 2


