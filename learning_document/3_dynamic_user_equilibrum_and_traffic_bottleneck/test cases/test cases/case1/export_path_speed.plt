set title "Dynamic Speed Contour (Path 1 ) Unit: mph" 
set xlabel "Time Horizon"
set ylabel "Space (Node Sequence)"  offset -1
set xtics (" 4:00" 0 ," 5:00" 60 ," 6:00" 120 ," 7:00" 180 ," 8:00" 240 ," 9:00" 300 ,"10:00" 360 ,"11:00" 420 ,"12:00" 480 ) 
set ytics ("1" 0, "3" 10, "4" 20, "5" 30, "6" 40, "7" 50, "8" 60, "9" 70, "10" 80, "11" 90, "12" 100, "2" 110)
set xrange [0:481] 
set yrange [0:110] 
set palette defined (0 "white", 0.1 "red", 40 "yellow", 50 "green")
set pm3d map
splot 'C:\GitHub\dtalite_software_release\learning_document\3_dynamic_user_equilibrum_and_traffic_bottleneck\test cases\test cases\case1\export_path_speed.txt' matrix notitle
