set output "dudx_surface_map.png"
set title "∂u/∂x as heatmap (x,t)"
set xlabel "x-index"
set ylabel "time-index"
unset key
set view map
set pm3d map
set palette rgb 21,22,23
splot "./postprocess/dudx_surface.txt" matrix with image

