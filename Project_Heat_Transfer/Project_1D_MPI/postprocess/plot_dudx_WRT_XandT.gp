set terminal pngcairo size 800,600 enhanced font 'Arial,12'
set output 'dudx_points.png'

set title 'dudx WRT x,t'
set xlabel 'x-index'
set ylabel 'time-step'

unset pm3d
set view map
set pointsize 3
set palette rgb 21,22,23
set colorbox

splot 'dudx_t.txt' using 1:2:3 with points palette

