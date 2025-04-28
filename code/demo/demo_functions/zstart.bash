if [ $2 -gt $1 ];
then
for((a=$1;a<$2;a++))
do
	./a.out $a $a
done
else
for((a=$1;a>$2;a--))
do
	./a.out $a $a
done

fi

