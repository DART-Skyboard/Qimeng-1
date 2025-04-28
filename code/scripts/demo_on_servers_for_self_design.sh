# The executed program (binary file) in test_dataset can only work on our chip/board. 
# The reason is that on the chip/board, OS, system softwares, compiler, ISA, and physical SOC design are very different from our server environment.
# This is the original version of the program in cpp, finished while developing our method. You can see what the program is supposed to output.

which_cluster=1		#from 1 to 10
which_bit=64		#from 0 to 1826

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


cd demo/CPU/cluster_$which_cluster
g++ self_produce.cpp -O3 -o self_produce.out

./self_produce.out $which_bit 10
./self_produce.out $which_bit 20
./self_produce.out $which_bit 30


