for a in {1..31}
do
	(( i=$a*32+31))
	(( j=$a*32+30   ))
	echo $i
	sbatch demo.sh $i $j
done
