# plot_error.gp - Plot log-log error and fit convergence rate

set terminal pngcairo enhanced size 800,600
set output 'error_plot_dt.png'

set title "Convergence Study"
set xlabel "log_{10}(dt)"
set ylabel "log_{10}(Error)"
set grid

set style line 1 lc rgb 'blue' pt 7 ps 1.5 lt 1 lw 2
set style line 2 lc rgb 'red'  lt 2 lw 2

filename = "error_dt.dat" 

# 使用 log10 scale，第一列是 Δx 或 Δt，第二列是误差 e
f(x) = a * x + b

fit f(x) filename using (log10($1)):(log10($2)) via a, b

set label sprintf("Slope (convergence rate) = %.3f", a) at graph 0.05, 0.85

plot filename using (log10($1)):(log10($2)) title "Data" with points ls 1, \
     f(x) title "Fit: slope = %.3f" with lines ls 2


