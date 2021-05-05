g++ -O0 Slider.cpp 
valgrind --tool=callgrind --callgrind-out-file=./callgrind.out ./a.out < 2.in
gprof2dot -f callgrind ./callgrind.out | dot -Tsvg -o report.svg
explorer.exe report.svg