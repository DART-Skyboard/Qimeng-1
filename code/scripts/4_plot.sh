which_cluster_default=9999
which_bit_default=9999

which_cluster=9999	#default.interactive use is from 1 to 10
which_bit=9999		#default.interactive use is from 0 to 1826

usage() {
  echo "Usage: ${0} [-c|--cluster] [-b|--bit]" 1>&2
  exit 1 
}
while [[ $# -gt 0 ]];do
  key=${1}
  case ${key} in
    -c|--cluster)
      which_cluster=${2}
      shift 2
      ;;
    -b|--bit)
      which_bit=${2}
      shift 2
      ;;
    *)
      usage
      shift
      ;;
  esac
done

if [[ $which_cluster = $which_cluster_default && $which_bit = $which_bit_default ]]
then
	echo "this is default case"	
	
	cd demo/CPU

	python generate_plot_BDD.py $which_bit $which_cluster

	cd ../../result/CPU/

	python plot_extended_data_figure_4a.py

	echo "finish saving the figure"
else
	echo "you choose your own case"	

	if [ $which_cluster = $which_cluster_default ]
	then
		which_cluster=1
	fi
	if [ $which_bit = $which_bit_default ]
	then
		which_bit=64
	fi
	
	echo -n "your case BSD cluster id = "
	echo $which_cluster
	echo -n "your case single-output BSD id = "
	echo $which_bit

	bash scripts/1_c_CPU_arbitrary_cases.sh -s $which_bit -e $(expr $which_bit + 1) -c $which_cluster
 
	cd demo/CPU

	python generate_plot_BDD.py $which_bit $which_cluster

	cd ../../result/CPU/cluster_$which_cluster/plot

	python plot_BDD_${which_bit}.py

	echo "finish saving the figure"
fi
