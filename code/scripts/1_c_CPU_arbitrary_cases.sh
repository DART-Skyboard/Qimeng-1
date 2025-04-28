

which_cluster=1		#from 1 to 10
which_start_bit=32	#from 0 to 1826
which_end_bit=64	#from 0 to 1826

usage() {
  echo "Usage: ${0} [-s|--start] [-e|--end] [-c|--cluster]" 1>&2
  exit 1 
}
while [[ $# -gt 0 ]];do
  key=${1}
  case ${key} in
    -s|--start)
      which_start_bit=${2}
      shift 2
      ;;
    -e|--end)
      which_end_bit=${2}
      shift 2
      ;;
    -c|--cluster)
      which_cluster=${2}
      shift 2
      ;;
    *)
      usage
      shift
      ;;
  esac
done

cd demo/CPU/cluster_$which_cluster
rm a.out
g++ BSD.cpp -O3

bash zstart.bash $which_start_bit $which_end_bit
