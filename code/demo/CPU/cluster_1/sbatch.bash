for a in {1696..1727}
do
	(( i=$a ))
	(( j=$a+1 ))
	echo $i
	sbatch demo.sh $i $j
done
