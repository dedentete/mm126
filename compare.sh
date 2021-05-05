n=$3
g++ -std=c++11 $1
for i in `seq 1 $n`
do
    if [ $i = 1 ]; then
        time java -jar tester.jar -exec "./a.out" -novis -seed $i > scores.txt
    else
        time java -jar tester.jar -exec "./a.out" -novis -seed $i >> scores.txt
    fi
done
g++ -std=c++11 $2
for i in `seq 1 $n`
do
    if [ $i = 1 ]; then
        time java -jar tester.jar -exec "./a.out" -novis -seed $i > compare.txt
    else
        time java -jar tester.jar -exec "./a.out" -novis -seed $i >> compare.txt
    fi
done
python3 compare.py