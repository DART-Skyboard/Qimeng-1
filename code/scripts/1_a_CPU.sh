
for a in {1..10}
do

cd demo/CPU/cluster_$a
rm a.out
g++ BSD.cpp -O3

bash zstart.bash 0 1826

cd ../../..

done


