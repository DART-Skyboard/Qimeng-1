
for b in {1..10}
do
cd cluster_$b
rm ret*
rm *.out
g++ BSD.cpp -O3
for a in {0..1826}
do
	(( i=$a ))
	(( j=$a+1 ))
	sbatch demo.sh $i $j
done
cd ..
done
