
bash scripts/1_a_CPU.sh
for a in {1..10}
do

cd demo/CPU/cluster_$a

dc_shell -f to_gtech.tcl
rm command.log
rm default.svf
rm filenames.log 

cd ../../..

done

rm command.log
rm default.svf
rm filenames.log 


