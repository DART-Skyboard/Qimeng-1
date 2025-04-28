#include<stdlib.h>
#include<stdio.h>
#include<stdbool.h>
#include<stdint.h>
#include<iostream>
#include<algorithm>
#include<math.h>
#include<sys/time.h>
#include<functional>
#include<string>
#include<fstream>
#include<sstream>

using namespace std;
extern const int	bit_width = 20;
int			parameter_sample_amount 	= 10000;
extern const int	max_parameter_sample_amount 	= 400000;
extern const int	parameter_how_often_simplify = 1;
extern const int	parameter_test_amount = 689;
extern const int	parameter_sample_amount_influence_max = 200;
extern const int	parameter_GRN_valid_depth=20000;
extern const int	parameter_GRN_acc_amount=100000;


int	which_bit_output = 1182;//1182;

#include"cvt.h"
#include"compare_bool_list.h"
int*		most_influence 		= new int[bit_width];
int*		BDD_width_each_layer 	= new int [bit_width+1];
bool**	simplify_list;
bool**	random_input_data = new bool*[max_parameter_sample_amount];
bool**	mask_input_data   = new bool*[max_parameter_sample_amount];
bool*	mask_output_data  = new bool [max_parameter_sample_amount];

int*	simplify_list_sum ;

double	total_finish_weight = 0;
class	BDD_node{
public:	
	int	left_node_index;
	int	right_node_index;
	int	depth;
	double	weight;
	bool	mask[bit_width];
	int	sort_in_layer;
	int	GRN_depth;
	BDD_node(){
		int left_node_index 	= 0;
		int right_node_index 	= 0;
		int depth		= 0;
		double weight	= 1;
		bool mask[bit_width]	= {0};
		int	sort_in_layer	= 0;
	};
};
BDD_node** BDD = new BDD_node*[1+bit_width];
class	simplify_node{
public:
	bool	bools_result[max_parameter_sample_amount];
	int	BDD_node_index;
	simplify_node(){
		int BDD_node_index = 0;
	}	
};
//simplify_node*	simplify_list_heap;





int*	truth_table; 
int*	truth_table_all; 

extern const	int	cal_data_width = 10;
bool	io_generator_single(bool input_data[bit_width],int which_bit_output){
	
	bool	output_bit;
	int i,j;

	int random_bit;
	int output_data = cvt_bit_to_number_unsigned(input_data,bit_width);
	if(truth_table[output_data]==0){
			output_bit = 0;
	}else{
			output_bit = 1;
	}
	
	return	output_bit;
};
string	GRN_data[689][20];
bool	GRN_bool[689][20];
int	GRN_bool_number[689];
bool*	has_been_unfold = new bool[bit_width+1];
int	set_random_input_data(){
	int i,j;
	for (i=0;i<parameter_sample_amount;i++){
		for (j=0;j<bit_width;j++){
			mask_input_data[i][j] = rand()%2;
		}
	}
	for (i=0;i<parameter_test_amount;i++){
		for (j=0;j<bit_width;j++){
			mask_input_data[i][j] = GRN_bool[i][j];
		}
	}
	return 0;
};
int	mask_random_input_data(int depth,bool mask[bit_width],int amount){
	int i,j;
	int which_node_this_layer = 0;
	for (i=0;i<amount;i++){
		for (j=0;j<depth+1;j++){
			mask_input_data[i][most_influence[j]] = mask[most_influence[j]];
		}
	}

	return 0;
};
bool***	amount_turn_input_data	= new bool**[bit_width+1];
bool**	amount_turn_output_data	= new bool* [bit_width+1];

int	cal_most_influence_next(int depth){
	int	most_influence_next=0;
	int 	i,j,k;
	int	zz;
	int	zi;
	int*	amount_turn = new int [bit_width];
	for (zz=0;zz<bit_width;zz++){
		amount_turn[zz] = 1;
	}	
	int position_this_layer[689];
	for(zi=0;zi<689;zi++){
		int position = 0;
		for (int zj=0;zj<depth;zj++){
			if(!GRN_bool[zi][most_influence[zj]]){
				if(BDD[zj][position].left_node_index == -1){
					position =  BDD[zj][position].right_node_index;
					break;
				}else if(BDD[zj][position].left_node_index == -2){
					position = -1;
					break;
				}else{
					position = BDD[zj][position].left_node_index;
				}
			}else{
				if(BDD[zj][position].right_node_index == -1){
					position =  BDD[zj][position].left_node_index;
					break;
				}else if(BDD[zj][position].right_node_index == -2){
					position = -1;
					break;
				}else{
					position = BDD[zj][position].right_node_index;
				}
			}
		}
		position_this_layer[zi] = position;
	}

	for(int zzz=0;zzz<bit_width;zzz++){
		for(j=0;j<1;j++){
			int	total_one_GRN_left = 0;
			int	total_zero_GRN_left = 0;
			for(zi=0;zi<689;zi++){
				bool flag_GRN = 0;
				if((GRN_bool[zi][zzz]==1)){
					flag_GRN = 1;
				}
				if(flag_GRN)
					if(GRN_bool[zi][which_bit_output])
						total_one_GRN_left += 1;
					else
						total_zero_GRN_left += 1;
			}
			if(total_one_GRN_left == 0){
				amount_turn[zzz] += total_zero_GRN_left;
			}else if(total_zero_GRN_left == 0){
				amount_turn[zzz] += total_one_GRN_left;
			}
		}
	}

	if(depth<50)
		amount_turn[which_bit_output] = 0;
	most_influence_next = max_element(amount_turn,amount_turn+bit_width) - amount_turn;	
	for (i=0;i<bit_width;i++){
		if(has_been_unfold[most_influence_next]){
			amount_turn[most_influence_next] = -1;
		}else{
			
			break;	//has_been_unfold[most_influence_next] = 1;
		}
		most_influence_next = max_element(amount_turn,amount_turn+bit_width) - amount_turn;
	}	
	has_been_unfold[most_influence_next] = 1;
	//cin>>most_influence_next;
	
	delete[]amount_turn;
	return	most_influence_next;
};

int	cal_most_influence_next_BDD(int depth){
	int	most_influence_next=0;
	int 	i,j,k;
	int	zz;
	int	zi;
	int*	amount_turn = new int [bit_width];
	for (zz=0;zz<bit_width;zz++){
		amount_turn[zz] = 0;
	}
	long	parameter_sample_mul = 10;
			for (j=0;j<20;j++){
				parameter_sample_mul *= 10;
				if((parameter_sample_mul * (1-total_finish_weight/pow(2.0,20))) > 10000){
					break;
				}
			}
	for (zz=0;zz<20;zz++){
		int	parameter_sample_amount_influence = int(min(int(pow(2,zz)),parameter_sample_amount_influence_max));
		int	which_node_this_layer = 0;
		int	which_node_this_layer_sample = 0;
		for (i=0;i<parameter_sample_amount_influence;i++){
			
			which_node_this_layer_sample = rand()%int(parameter_sample_mul*(1-total_finish_weight/pow(2.0,20)));//BDD_width_each_layer[depth];
			double sum_sample = 0;
			for (j=0;j<BDD_width_each_layer[depth];j++){
				sum_sample += BDD[depth][j].weight/pow(2.0,20);
				if(parameter_sample_mul*(sum_sample) > which_node_this_layer_sample){
					which_node_this_layer = j;
					break;
				}
			}
			for (j=0;j<bit_width;j++){
				if(depth == 0){
					mask_input_data[i][j] = rand()%2;
				}
				else{
					if(has_been_unfold[j]){
						mask_input_data[i][j] = BDD[depth][which_node_this_layer].mask[j];
						//mask_input_data[i][j] = random()%2;		
					}else{	
						mask_input_data[i][j] = random()%2;		
					}
				}
			}
		}
		
		for (i=0;i<bit_width+1;i++){
			if(has_been_unfold[i]){
				
			}else{
				for(j=0;j<parameter_sample_amount_influence;j++){
					for(k=0;k<bit_width;k++){
						if(i != k){
							amount_turn_input_data[i][j][k] = mask_input_data[j][k];
						}
						else{
							amount_turn_input_data[i][j][k] = !mask_input_data[j][k];
						}	
					}
					amount_turn_output_data[i][j] = io_generator_single(amount_turn_input_data[i][j],which_bit_output);
				}
			}
		}
		bool	finish_influence_sample = 0;
		for (i=0;i<bit_width;i++){
			if(has_been_unfold[i]){
				amount_turn[i] = -1;	
			}else{
				if(i!=which_bit_output){
					for(j=0;j<parameter_sample_amount_influence;j++){
						if(amount_turn_output_data[i][j] != amount_turn_output_data[bit_width][j]){
							amount_turn[i] += 1;
						}
					}
					if(amount_turn[i] > 20){
						finish_influence_sample = 1;
					}
				}
			}
		}
		if(finish_influence_sample){
			break;
		}
	}
	
	most_influence_next = max_element(amount_turn,amount_turn+bit_width) - amount_turn;	
	for (i=0;i<bit_width;i++){
		if(has_been_unfold[most_influence_next]){
			amount_turn[most_influence_next] = -1;
		}else{
			
			break;	//has_been_unfold[most_influence_next] = 1;
		}
		most_influence_next = max_element(amount_turn,amount_turn+bit_width) - amount_turn;
	}	
	has_been_unfold[most_influence_next] = 1;
	
	delete[]amount_turn;
	return	most_influence_next;
};


int*	simplify_list_sort;

int	maxheap_bools(simplify_node* simplify_nodes,int i,int limit){
	int largest = i;
	if((2*i<=limit) && (compare_bool_list(simplify_nodes[simplify_list_sort[i]].bools_result,simplify_nodes[simplify_list_sort[2*i+1]].bools_result,parameter_sample_amount) == 2)){	
		largest = 2 * i + 1;
	}
	if((2*i<=limit) && (compare_bool_list(simplify_nodes[simplify_list_sort[i]].bools_result,simplify_nodes[simplify_list_sort[2*i+2]].bools_result,parameter_sample_amount) == 2)){	
		largest = 2 * i + 2;
	}
	if(i != largest)
	{
		int	temp;
		temp				=	simplify_list_sort[i];
		simplify_list_sort[i]		= 	simplify_list_sort[largest];
		simplify_list_sort[largest]	= 	temp;
		maxheap_bools(simplify_nodes,largest,limit);
	}
	return 0;
}
int	createmaxheap_bools(simplify_node* simplify_nodes,int size){
	int i;
	for(i=size/2-1;i>-1;i--){
		maxheap_bools(simplify_nodes,i,size);
	}
	return 0;
};
int	heapsort_bools(simplify_node* simplify_nodes,int size){
	createmaxheap_bools(simplify_nodes,size);
	for(int i=size-1;i>-1;i--){
		int temp;
		temp 			= simplify_list_sort[0];
		simplify_list_sort[0]	= simplify_list_sort[i];
		simplify_list_sort[i]	= temp;	
		maxheap_bools(simplify_nodes,1,i-1);
	}
	return 0;	
};



int compare_simplify_list(int list_line_amount,bool* this_line,int this_line_sum){
	int which_list_number = -1;
	bool it_can_simplify  = 0;
	int i,j;

	for (i=0;i<list_line_amount;i++){
		bool it_can_simplify_here = 1;
		if(simplify_list_sum[i] != this_line_sum){
			it_can_simplify_here = 0;
			continue;
		}
		for(j=0;j<parameter_sample_amount;j++){
			if(simplify_list[i][j] != this_line[j]){
				it_can_simplify_here = 0;
				break;
			}
		}
		if(it_can_simplify_here){
			it_can_simplify   = 1;
			which_list_number = i;
			break;
		}
	}
	return which_list_number;
};

long	total_nodes_amount = 0;
int	total_BDD_depth	   = 0;
int train_BDD(int which_bit_output){

	int i,j;
	int zi;
	BDD[0] = new BDD_node[1];
	for (i=0;i<bit_width;i++){
		BDD[0][0].mask[i] = 0;
	}
	BDD[0][0].GRN_depth=0;
	struct timeval	start_time,finish_time;
	gettimeofday(&start_time,NULL);
	for(i=0;i<bit_width+2;i++){
		parameter_sample_amount = max(int(parameter_sample_amount/1),10);
		cout<<parameter_sample_amount<<endl;
		if(BDD_width_each_layer[i]==0){
			total_BDD_depth = i - 1;
			break;
		}
		total_nodes_amount += BDD_width_each_layer[i];
			
		BDD[i+1] = new BDD_node[BDD_width_each_layer[i]*2];
		simplify_list_sum = new int [BDD_width_each_layer[i]*2];
		simplify_list = new bool*[BDD_width_each_layer[i]*2];
		for (zi=0;zi<BDD_width_each_layer[i]*2;zi++){
			simplify_list[zi] = new bool[parameter_sample_amount];
		}
		simplify_list_sort = new int[BDD_width_each_layer[i]*2];
		for(zi=0;zi<BDD_width_each_layer[i]*2;zi++){
			simplify_list_sort[zi] = zi;
		}
		most_influence[i] = cal_most_influence_next(i); 
		cout<<"The BSD at layer: "<<i<<"		";
		cout<<"Input bit this layer = x"<<most_influence[i]<<endl;
		cout<<"The BSD width this layer = "<<BDD_width_each_layer[i]<<endl;
		BDD[0][0].weight = pow(2.0,20);
		set_random_input_data();
		int position_this_layer[689];
		for(zi=0;zi<689;zi++){
			int position = 0;
			for (int zj=0;zj<i;zj++){
				if(!GRN_bool[zi][most_influence[zj]]){
					if(BDD[zj][position].left_node_index == -1){
						position =  BDD[zj][position].right_node_index;
						break;
					}else if(BDD[zj][position].left_node_index == -2){
						position = -1;
						break;
					}else{
						position = BDD[zj][position].left_node_index;
					}
				}else{
					if(BDD[zj][position].right_node_index == -1){
						position =  BDD[zj][position].left_node_index;
						break;
					}else if(BDD[zj][position].right_node_index == -2){
						position = -1;
						break;
					}else{
						position = BDD[zj][position].right_node_index;
					}
				}
			}
			position_this_layer[zi] = position;
		}
		for(j=0;j<BDD_width_each_layer[i];j++){
			bool*	left_son_mask	= new bool [bit_width];
			bool*	right_son_mask	= new bool [bit_width];
			for(zi=0;zi<i;zi++){
				left_son_mask[most_influence[zi]] 	= BDD[i][j].mask[most_influence[zi]]; 
				right_son_mask[most_influence[zi]] 	= BDD[i][j].mask[most_influence[zi]]; 
			}
			left_son_mask[most_influence[i]] 	= 0;
			right_son_mask[most_influence[i]] 	= 1;
			
			bool* left_mask_output_data	= new bool [parameter_sample_amount];
			bool* right_mask_output_data	= new bool [parameter_sample_amount];
			
			int left_mask_output_data_sum = 0;
			mask_random_input_data(i,left_son_mask,parameter_sample_amount);
			for(zi=0;zi<parameter_sample_amount;zi++){
				left_mask_output_data[zi] = io_generator_single(mask_input_data[zi],which_bit_output);
				if(left_mask_output_data[zi]){
					left_mask_output_data_sum += 1;
				}
			}
			int right_mask_output_data_sum = 0;
			mask_random_input_data(i,right_son_mask,parameter_sample_amount);
			for(zi=0;zi<parameter_sample_amount;zi++){
				right_mask_output_data[zi] = io_generator_single(mask_input_data[zi],which_bit_output);
				if(right_mask_output_data[zi]){
					right_mask_output_data_sum += 1;
				}
			}
			bool all_zero_left  = 1;
			bool all_one_left   = 1;
			bool all_one_right  = 1;
			bool all_zero_right = 1;

			float  total_one_GRN_left  = 0;
			float  total_zero_GRN_left = 0;
			float  total_one_GRN_right  = 0;
			float  total_zero_GRN_right = 0;


			for(zi=0;zi<689;zi++){
				bool flag_GRN = 0;
				if((position_this_layer[zi]==j) && (GRN_bool[zi][most_influence[i]]==0)){
					flag_GRN = 1;
				}
				if(flag_GRN)
					if(GRN_bool[zi][which_bit_output])
						total_one_GRN_left += 1;
					else
						total_zero_GRN_left += 1;
			}
			if((total_one_GRN_left>0) && (total_zero_GRN_left>0)){
				if((total_one_GRN_left/total_zero_GRN_left)>parameter_GRN_acc_amount){
					all_zero_left = 0;
					for(zi=0;zi<689;zi++){
						bool flag_GRN = 0;
						if((position_this_layer[zi]==j) && (GRN_bool[zi][most_influence[i]]==0)){
							flag_GRN = 1;
						}
						if(flag_GRN)
							GRN_bool[zi][which_bit_output]=1;
					}
				}else if((total_one_GRN_left/total_zero_GRN_left)>parameter_GRN_acc_amount){
					all_one_left = 0;
					for(zi=0;zi<689;zi++){
						bool flag_GRN = 0;
						if((position_this_layer[zi]==j) && (GRN_bool[zi][most_influence[i]]==0)){
							flag_GRN = 1;
						}
						if(flag_GRN)
							GRN_bool[zi][which_bit_output]=0;
					}
				}
				else{
					all_zero_left = 0;
					all_one_left = 0;
				}
			}else if((total_one_GRN_left>0) && (total_zero_GRN_left==0)){
				all_zero_left = 0;
			}else{
				all_one_left = 0;
			}
			for(zi=0;zi<689;zi++){
				bool flag_GRN = 0;
				if((position_this_layer[zi]==j) && (GRN_bool[zi][most_influence[i]]==1)){
					flag_GRN = 1;
				}
				if(flag_GRN)
					if(GRN_bool[zi][which_bit_output])
						total_one_GRN_right += 1;
					else
						total_zero_GRN_right += 1;
			}
			if((total_one_GRN_right>0) && (total_zero_GRN_right>0)){
				if((total_one_GRN_right/total_zero_GRN_right)>parameter_GRN_acc_amount){
					all_zero_right = 0;
					for(zi=0;zi<689;zi++){
						bool flag_GRN = 0;
						if((position_this_layer[zi]==j) && (GRN_bool[zi][most_influence[i]]==0)){
							flag_GRN = 1;
						}
						if(flag_GRN)
							GRN_bool[zi][which_bit_output]=1;
					}
				}else if((total_one_GRN_right/total_zero_GRN_right)>parameter_GRN_acc_amount){
					all_one_right = 0;
					for(zi=0;zi<689;zi++){
						bool flag_GRN = 0;
						if((position_this_layer[zi]==j) && (GRN_bool[zi][most_influence[i]]==0)){
							flag_GRN = 1;
						}
						if(flag_GRN)
							GRN_bool[zi][which_bit_output]=0;
					}
				}
				else{
					all_zero_right = 0;
					all_one_right = 0;
				}
			}else if((total_one_GRN_right>0) && (total_zero_GRN_right==0)){
				all_zero_right = 0;
			}else{
				all_one_right = 0;
			}

			if(BDD[i][j].GRN_depth>=parameter_GRN_valid_depth){
				total_one_GRN_left=0;
				total_one_GRN_right=0;
				total_zero_GRN_left=10;
				total_zero_GRN_right=10;
				all_one_left=0;
				all_zero_left=1;
				all_one_right=0;
				all_zero_right=1;
			}
			if(all_zero_left){
				BDD[i][j].left_node_index = -2;
				total_finish_weight += BDD[i][j].weight/2;
			}else if(all_one_left){
				BDD[i][j].left_node_index = -1; 
				total_finish_weight += BDD[i][j].weight/2;
			}else{
				if(((i%parameter_how_often_simplify) == 0)&&(i>0)){
					int which_list_number = compare_simplify_list(BDD_width_each_layer[i+1],left_mask_output_data,left_mask_output_data_sum);
					if(which_list_number < 0){
						BDD_width_each_layer[i+1] += 1;
						BDD[i+1][BDD_width_each_layer[i+1]-1].depth  = i+1; 
						BDD[i+1][BDD_width_each_layer[i+1]-1].weight = BDD[i][j].weight/2; 
						for (zi=0;zi<i+1;zi++){
							BDD[i+1][BDD_width_each_layer[i+1]-1].mask[most_influence[zi]]  = left_son_mask[most_influence[zi]]; 
						}
						BDD[i][j].left_node_index = 	BDD_width_each_layer[i+1]-1;
						for (zi=0;zi<parameter_sample_amount;zi++){
							simplify_list[BDD_width_each_layer[i+1]-1][zi] = left_mask_output_data[zi];
						}
						simplify_list_sum[BDD_width_each_layer[i+1]-1]				= left_mask_output_data_sum;
						if(all_one_right)
							BDD[i+1][BDD_width_each_layer[i+1]-1].GRN_depth = BDD[i][j].GRN_depth; 
						else
							BDD[i+1][BDD_width_each_layer[i+1]-1].GRN_depth = BDD[i][j].GRN_depth+1; 
					}else{
						BDD[i][j].left_node_index = 	which_list_number;
						BDD[i+1][which_list_number].weight += BDD[i][j].weight/2;
					}
				}else{
					BDD_width_each_layer[i+1] += 1;
					BDD[i+1][BDD_width_each_layer[i+1]-1].depth = i+1; 
					BDD[i+1][BDD_width_each_layer[i+1]-1].weight = BDD[i][j].weight/2; 
					for (zi=0;zi<i+1;zi++){
						BDD[i+1][BDD_width_each_layer[i+1]-1].mask[most_influence[zi]]  = left_son_mask[most_influence[zi]]; 
					}
					BDD[i][j].left_node_index = 	BDD_width_each_layer[i+1]-1;
				}
			}
			if(all_zero_right){
				BDD[i][j].right_node_index = -2; 
				total_finish_weight += BDD[i][j].weight/2;
			}else if(all_one_right){
				BDD[i][j].right_node_index = -1; 
				total_finish_weight += BDD[i][j].weight/2;
			}else{
				if(((i%parameter_how_often_simplify == 0)&&(i>0))){
					int which_list_number = compare_simplify_list(BDD_width_each_layer[i+1],right_mask_output_data,right_mask_output_data_sum);
					if(which_list_number < 0){
						BDD_width_each_layer[i+1] += 1;
						BDD[i+1][BDD_width_each_layer[i+1]-1].depth = i+1; 
						BDD[i+1][BDD_width_each_layer[i+1]-1].weight = BDD[i][j].weight/2; 
						for (zi=0;zi<i+1;zi++){
							BDD[i+1][BDD_width_each_layer[i+1]-1].mask[most_influence[zi]]  = right_son_mask[most_influence[zi]]; 
						}
						BDD[i][j].right_node_index = 	BDD_width_each_layer[i+1]-1;
						for (zi=0;zi<parameter_sample_amount;zi++){
							simplify_list[BDD_width_each_layer[i+1]-1][zi] 	= right_mask_output_data[zi];
						}
						simplify_list_sum[BDD_width_each_layer[i+1]-1]				= right_mask_output_data_sum;
						if(all_one_left)
							BDD[i+1][BDD_width_each_layer[i+1]-1].GRN_depth = BDD[i][j].GRN_depth; 
						else
							BDD[i+1][BDD_width_each_layer[i+1]-1].GRN_depth = BDD[i][j].GRN_depth+1; 
					}else{
						BDD[i][j].right_node_index = 	which_list_number;
						BDD[i+1][which_list_number].weight += BDD[i][j].weight/2;
					}
				}else{
					BDD_width_each_layer[i+1] += 1;
					BDD[i+1][BDD_width_each_layer[i+1]-1].depth = i+1; 
					BDD[i+1][BDD_width_each_layer[i+1]-1].weight = BDD[i][j].weight/2; 
					for (zi=0;zi<i+1;zi++){
						BDD[i+1][BDD_width_each_layer[i+1]-1].mask[most_influence[zi]]  = right_son_mask[most_influence[zi]]; 
					}
					BDD[i][j].right_node_index = 	BDD_width_each_layer[i+1]-1;
				}
			}
		}
		cout<<"Total Nodes: "<<total_nodes_amount;
		cout<<"		Finished Nodes weight: "<<(total_finish_weight)/pow(2.0,20)<<endl;
		gettimeofday(&finish_time,NULL);
		cout<<"Total Training time: "<<double(finish_time.tv_usec-start_time.tv_usec+1000000*(finish_time.tv_sec-start_time.tv_sec))/1000000<<"s"<<endl;
		cout<<"######################################################################"<<endl;
	}

	return 0;
};
bool	cal_infer_result(bool input_data[bit_width]){
	bool	infer_result = 0;
	int i;
	int position = 0;
	for (i=0;i<bit_width+1;i++){
		if(!input_data[most_influence[i]]){
			if(BDD[i][position].left_node_index == -1){
				infer_result = 1;
				break;
			}else if(BDD[i][position].left_node_index == -2){
				infer_result = 0;
				break;
			}else{
				position = BDD[i][position].left_node_index;
			}
		}else{
			if(BDD[i][position].right_node_index == -1){
				infer_result = 1;
				break;
			}else if(BDD[i][position].right_node_index == -2){
				infer_result = 0;
				break;
			}else{
				position = BDD[i][position].right_node_index;
			}
		}
	}
	return	infer_result;
};
double	circuit_accuracy;


bool*	test_input_data [parameter_test_amount];
int	BDD_infer(int which_bit_output){
	int i,j;
	for (i=0;i<parameter_test_amount;i++){
		test_input_data [i] = new bool [bit_width];
	}
	bool*	test_output_data;
	test_output_data = new bool [parameter_test_amount];
	bool*	infer_output_data;
	infer_output_data = new bool[parameter_test_amount];
	for (i=0;i<parameter_test_amount;i++){
		for (j=0;j<bit_width;j++){
			test_input_data[i][j] = GRN_bool[i][j]; 
		}
		test_output_data[i]  = io_generator_single(test_input_data[i],which_bit_output);
		infer_output_data[i] = cal_infer_result(test_input_data[i]);
	}
	int	error_amount = 0;
	for (i=0;i<parameter_test_amount;i++){
		if((test_output_data[i]==1) && (infer_output_data[i]==0)){
			error_amount = error_amount + 1;
		}
	}
	circuit_accuracy = 1-(double(error_amount)/parameter_test_amount);
	cout<<"Circuit Accuracy = "<<1-(double(error_amount)/parameter_test_amount)<<endl;
	return 0;
};
int	print_circuit(int which_bit_output){

	int i,j;
	int zi;
	
	char		output_file_name[100];
	sprintf(output_file_name,"../../result/GRN/rtl/module_output_bit_%d.v",which_bit_output);
//	string		output_file_name << output_file_name_begin << which_bit_output << output_file_name_end;

	cout<<output_file_name<<endl;

	ofstream	output_module_file(output_file_name);
	output_module_file << "//circuit accuracy	= "<<circuit_accuracy<<endl;	
	output_module_file << "//test amounts		= "<<parameter_test_amount<<endl;	
	output_module_file << "//total BDD nodes	= "<<total_nodes_amount<<endl;	
	output_module_file << "module module_output_bit_"<<which_bit_output<<"(input_data,output_bit);"<<endl<<endl;	
	output_module_file << "input	["<<bit_width-1<<":0]	input_data;"<<endl;	
	output_module_file << "output		output_bit;"<<endl<<endl;


	for (i=0;i<total_BDD_depth+1;i++){
		output_module_file << "wire	["<<BDD_width_each_layer[i]-1<<":0]	layer_"<<i<<";"<<endl;
	}	
	output_module_file <<endl<< "assign	output_bit = layer_0[0];"<<endl<<endl;
	for (i=0;i<total_BDD_depth+1;i++){
		for (j=0;j<BDD_width_each_layer[i];j++){
			if((BDD[i][j].left_node_index >= 0) & (BDD[i][j].right_node_index >= 0)){
				output_module_file << "assign	layer_"<<i<<"["<<j<<"]   	= (layer_"<<i+1<<"	["<<BDD[i][j].left_node_index<<"]	& !input_data["<<most_influence[i]<<"])	| (layer_"<<i+1<<"	["<<BDD[i][j].right_node_index<<"]	&  input_data["<<most_influence[i]<<"]);"<<endl;
			}else if((BDD[i][j].left_node_index == -1) & (BDD[i][j].right_node_index >= 0)){
				output_module_file << "assign	layer_"<<i<<"["<<j<<"]   	= (			  !input_data["<<most_influence[i]<<"])	| (layer_"<<i+1<<"	["<<BDD[i][j].right_node_index<<"]	&  input_data["<<most_influence[i]<<"]);"<<endl;
			}else if((BDD[i][j].left_node_index == -2) & (BDD[i][j].right_node_index >= 0)){
				output_module_file << "assign	layer_"<<i<<"["<<j<<"]   	= (layer_"<<i+1<<"	["<<BDD[i][j].right_node_index<<"]	&  input_data["<<most_influence[i]<<"]);"<<endl;
			}else if((BDD[i][j].left_node_index >= 0) & (BDD[i][j].right_node_index == -1 )){
				output_module_file << "assign	layer_"<<i<<"["<<j<<"]   	= (layer_"<<i+1<<"	["<<BDD[i][j].left_node_index<<"]	& !input_data["<<most_influence[i]<<"])	| (			   input_data["<<most_influence[i]<<"]);"<<endl;
			}else if((BDD[i][j].left_node_index >= 0) & (BDD[i][j].right_node_index == -2)){
				output_module_file << "assign	layer_"<<i<<"["<<j<<"]   	= (layer_"<<i+1<<"	["<<BDD[i][j].left_node_index<<"]	& !input_data["<<most_influence[i]<<"]);"<<endl;
			}else if((BDD[i][j].left_node_index == -1) & (BDD[i][j].right_node_index == -1)){
				output_module_file << "assign	layer_"<<i<<"["<<j<<"]   	= 1;"<<endl;
			}else if((BDD[i][j].left_node_index == -2) & (BDD[i][j].right_node_index == -2)){
				output_module_file << "assign	layer_"<<i<<"["<<j<<"]   	= 0;"<<endl;
			}else if((BDD[i][j].left_node_index == -1) & (BDD[i][j].right_node_index == -2)){
				output_module_file << "assign	layer_"<<i<<"["<<j<<"]   	= !input_data["<<most_influence[i]<<"];"<<endl;
			}else if((BDD[i][j].left_node_index == -2) & (BDD[i][j].right_node_index == -1)){
				output_module_file << "assign	layer_"<<i<<"["<<j<<"]   	=  input_data["<<most_influence[i]<<"];"<<endl;
			}
		}
	}
	
	output_module_file << endl <<"endmodule"<<endl;

	return 0 ;
};
int	main(int argc,char* argv[]){

	int i,j;

	if(argc == 2){
		which_bit_output = atoi(argv[1]);
	}

	BDD_width_each_layer[0] = 1;
	for (i=0;i<max_parameter_sample_amount;i++){
		random_input_data[i] 	= new bool[bit_width];
		mask_input_data[i] 	= new bool[bit_width];
	}
	for (i=0;i<bit_width+1;i++){
		amount_turn_input_data[i]  = new bool* [parameter_sample_amount_influence_max];
		amount_turn_output_data[i] = new bool  [parameter_sample_amount_influence_max];
		for(j=0;j<parameter_sample_amount_influence_max;j++){
			amount_turn_input_data[i][j] = new bool [bit_width];
		}
		has_been_unfold[i] = 0;
	}
	//srand((unsigned)time(0));
	truth_table     = new int[int(pow(2,20))];
	truth_table_all = new int[int(pow(2,20))];

	ifstream infile;
	infile.open("preimpl.txt");
	string temp;
	for(i=0;i<21;i++){
		infile >> temp;
		cout<<temp<<" ";
	}
	cout<<endl;
	int     case_number;
	for(i=0;i<689;i++){
		for(int zi=0;zi<2;zi++){
			infile >> temp;
		}
		for(j=0;j<20;j++){
			infile >> GRN_data[i][j];
			if(GRN_data[i][j]=="TRUE")
				GRN_bool[i][j] = 1;
			else
				GRN_bool[i][j] = 0;
		}
		GRN_bool_number[i] = cvt_bit_to_number_unsigned(GRN_bool[i],20);
	}
	for (i=0;i<pow(2,20);i++){
		truth_table[i]     = 0;
		truth_table_all[i] = 0;
	}
	for (i=0;i<689;i++){
		truth_table[GRN_bool_number[i]] += GRN_bool[i][which_bit_output];
		truth_table_all[GRN_bool_number[i]] += 1;
	}
	for (i=0;i<pow(2,20);i++){
		if((truth_table[i]!=truth_table_all[i])&&(truth_table[i]!=0))
			cout<<i<<"***************************************************************************"<<endl;
	}
	
	cout<<"ready to train"<<endl;
	train_BDD(which_bit_output);
	cout<<"ready to train"<<endl;
	BDD_infer(which_bit_output);
	print_circuit(which_bit_output);
};



