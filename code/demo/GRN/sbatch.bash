#######for a in {1..31}
#######do
#######	(( i=$a*32 ))
#######	(( j=$a*32+32 ))
#######	echo $i
#######	sbatch demo.sh $i $j
#######done

##for a in {1792..1823}
##do
##	(( i=$a ))
##	(( j=$a+1 ))
##	echo $i
##	sbatch demo.sh $i $j
##done
####for a in {1728..1759}
####do
####	(( i=$a ))
####	(( j=$a+1 ))
####	echo $i
####	sbatch demo.sh $i $j
####done
for a in {1696..1727}
do
	(( i=$a ))
	(( j=$a+1 ))
	echo $i
	sbatch demo.sh $i $j
done
#########for a in {1760..1791}
#########do
#########	(( i=$a ))
#########	(( j=$a+1 ))
#########	echo $i
#########	sbatch demo.sh $i $j
#########done
