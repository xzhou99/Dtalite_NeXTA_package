set title "Dynamic Speed Contour (Path 1 ) Unit: mph" 
set xlabel "Time Horizon"
set ylabel "Space (Node Sequence)"  offset -1
set xtics (" 7:00" 0 ," 7:10" 10 ," 7:20" 20 ," 7:30" 30 ," 7:40" 40 ," 7:50" 50 ," 8:00" 60 ," 8:10" 70 ," 8:20" 80 ," 8:30" 90 ," 8:40" 100 ," 8:50" 110 ," 9:00" 120 ) 
set ytics ("1" 0, "3" 10, "4" 20, "5" 30, "6" 40, "7" 50, "8" 60, "9" 70, "10" 80, "11" 90, "12" 100, "2" 110)
set xrange [0:121] 
set yrange [0:110] 
set palette defined (0 "white", 0.1 "red", 40 "yellow", 50 "green")
set pm3d map
splot 'F:\STALite-master-latest\release_0903\case2\export_path_speed.txt' matrix notitle
