set title "Dynamic Speed Contour (Path 1 ) Unit: mph" 
set xlabel "Time Horizon"
set ylabel "Space (Node Sequence)"  offset -1
set xtics (" 6:00" 0 ," 7:00" 60 ," 8:00" 120 ," 9:00" 180 ,"10:00" 240 ,"11:00" 300 ,"12:00" 360 ) 
set ytics ("3" 0, "4" 10, "5" 20, "6" 30, "7" 40, "8" 50, "9" 60)
set xrange [0:361] 
set yrange [0:60] 
set palette defined (0 "white", 0.1 "red", 40 "yellow", 50 "green")
set pm3d map
splot 'C:\GitHub\dtalite_software_release\learning_document\3_dynamic_user_equilibrum_and_traffic_bottleneck\test cases\test cases\case3\export_path_speed.txt' matrix notitle
