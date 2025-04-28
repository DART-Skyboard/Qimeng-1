for b in {1..10}
do
cd demo/CPU/cluster_$b
rm *.out
rm ret*
rm command.log
rm default.svf
rm filenames.log 

cd ../../../result/CPU/cluster_$b
rm rtl/*.v
rm gtech/*.v
rm plot/*
rm self_produce/*.v
rm *.log

cd ../../..

done
