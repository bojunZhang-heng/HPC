# Sey output format
set terminal pngcairo size 1000,800 enhanced font 'Arial,12'
set output "plot_u_at_different_t.png"

# u(x) at different times
set title "u(x) at different times"
set xlabel "x"
set ylabel "u(x,t)"
set grid
plot \
    "./postprocess/outputT=000.txt" using 1:2 with lines lw 2 title "t=0", \
    "./postprocess/outputT=025.txt" using 1:2 with lines lw 2 title "t=0.25T", \
    "./postprocess/outputT=050.txt" using 1:2 with lines lw 2 title "t=0.5T", \
    "./postprocess/outputT=075.txt" using 1:2 with lines lw 2 title "t=0.75T", \
    "./postprocess/outputT=100.txt" using 1:2 with lines lw 2 title "t=T"
