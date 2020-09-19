set title "Dynamic Density Contour (Path 1 ) Unit: veh/mile/lane" 
set xlabel "Time Horizon"
set ylabel "Space (Node Sequence)"  offset -1
set xtics (" 4:00" 0 ," 4:30" 30 ," 5:00" 60 ," 5:30" 90 ," 6:00" 120 ," 6:30" 150 ," 7:00" 180 ," 7:30" 210 ," 8:00" 240 ," 8:30" 270 ," 9:00" 300 ) 
set ytics ("3" 0, "4" 10, "5" 20, "6" 30, "7" 40, "8" 50, "9" 60, "10" 70, "11" 80)
set xrange [0:301] 
set yrange [0:80] 
set palette defined (0 "white", 10 "green", 30 "yellow", 50 "red")
set pm3d map
splot 'C:\GitHub\dtalite_software_release\release\version_GMNS0.9\6_three_corridor\export_path_density.txt' matrix notitle
