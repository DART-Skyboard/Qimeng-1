for a in {1..31}
do
	(( i=$a*32))
	(( j=$a*32+32   ))
	echo $i
	sbatch demo.sh $i $j
done
