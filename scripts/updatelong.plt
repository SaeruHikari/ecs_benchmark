# Scale font and line width (dpi) by changing the size! It will always display stretched.
set terminal png size 1280,720 enhanced font "Arial,12"
set output 'updatelong.png'

# Key means label...
set key inside bottom right
set xlabel 'Entities'
set ylabel 'Time per Operation (ns/op)'
set title 'ECS Benchmark System Updates'

set logscale x 2
set logscale y 10
set format x "2^{%L}"
set format y "10^{%L}"

## the labels are in Alphabetical order
plot  "data-systems-update-long.dat" using 1:2 title 'Anax' with lines lw 3 ,   "data-systems-update-long.dat" using 1:3 title 'EnTT' with lines lw 3 ,   "data-systems-update-long.dat" using 1:4 title 'Artemis' with lines lw 3 ,   "data-systems-update-long.dat" using 1:5 title 'Ginseng' with lines lw 3 ,   "data-systems-update-long.dat" using 1:6 title 'EntityX' with lines lw 3 ,   "data-systems-update-long.dat" using 1:7 title 'EntityX (experimental/compile_time)' with lines lw 3 ,   "data-systems-update-long.dat" using 1:8 title 'EnTT (use group)' with lines lw 3 ,   "data-systems-update-long.dat" using 1:9 title 'EnTT (legacy)' with lines lw 3 ,  
