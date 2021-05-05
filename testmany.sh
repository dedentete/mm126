n=$1
g++ -std=c++11 Slider.cpp 
for i in `seq 1 $n`
do
    if [ $i = 1 ]; then
        java -jar tester.jar -exec "./a.out" -novis -seed $i > scores.txt
    else
        java -jar tester.jar -exec "./a.out" -novis -seed $i >> scores.txt
    fi
done
python3 score.py