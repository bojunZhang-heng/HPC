# Set output format
set terminal pngcairo size 1000,800 enhanced font 'Arial,12'
set output "u_WRT_X_at_different_t.png"

# u(x) at different times
set title "u(x) at different times"
set xlabel "x"
set ylabel "u(x,t)"
set grid
plot \
  "outputT=000.txt" using 1:2 with lines lt 1 lc rgb "red"    lw 2 title "t=0", \
  "outputT=025.txt" using 1:2 with lines lt 2 lc rgb "blue"   lw 2 title "t=0.25T", \
  "outputT=050.txt" using 1:2 with lines lt 3 lc rgb "green"  lw 2 title "t=0.5T", \
  "outputT=075.txt" using 1:2 with lines lt 4 lc rgb "orange" lw 2 title "t=0.75T", \
  "outputT=100.txt" using 1:2 with lines lt 5 lc rgb "black"  lw 2 title "t=T"

