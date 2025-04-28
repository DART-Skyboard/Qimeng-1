#include<stdlib.h>
#include<stdio.h>
#include<stdbool.h>
#include<iostream>
#include<iomanip>
#include<algorithm>
#include<math.h>
#include<sys/time.h>
#include<functional>
#include<string>
#include<fstream>
#include<sstream>
#include<vector>
#include<dirent.h>
#include<stdint.h>
using namespace std;

extern const int	parameter_multi_output_index	= 0;

extern const int	bit_width 			= 64+10;
extern const int	max_parameter_sample_amount 	= 4000;
extern const int	parameter_depth_start_simplify	= parameter_multi_output_index-1;
extern const int	parameter_how_many_bits_output	= int(bit_width*2);
extern const int	parameter_how_many_root_nodes	= 32;	//before first train, change with io_generator choice, max set as  bit_width*2 is enough


int			parameter_sample_amount 	= 100;
int			parameter_sample_amount_train 	= 4000;
int			parameter_sample_amount_sort 	= 400;
extern const int	max_parameter_sample_amount_all = 1000000000;
extern const int	parameter_how_often_simplify 	= 1;
long			parameter_test_amount 		= 100000;//10000000000;
extern const int	parameter_sample_amount_influence_max = 1;
int			parameter_early_stop_depth_0	= 10000;
extern const int	parameter_early_stop_depth_1	= 10000000;
extern const double	total_weight_max		= pow(2.0,20);
extern const int	parameter_train_dataset_lines_all	= 1*pow(2,parameter_multi_output_index)/1;
extern const int	parameter_train_dataset_lines_temp	= 10*pow(2,parameter_multi_output_index);
int			parameter_train_dataset_lines	= parameter_train_dataset_lines_all;
int			parameter_train_dataset_lines_init	= parameter_train_dataset_lines_all;
extern const int	parameter_max_BDD_width			= 100000;
extern const int	parameter_how_many_simplify_clusters	= 1000;

int	which_function_output = 9999;

int	BDD_number_counter = 0;

bool*	which_bit_output_general_reg_index;
#include"cvt.h"
//#include"compare_bool_list.h"
int*		most_influence_0	= new int[bit_width];
int*		most_influence_1	= new int[bit_width];
bool**	random_input_data= new bool*[max_parameter_sample_amount];
bool**	mask_input_data  = new bool*[max_parameter_sample_amount];
bool*	mask_output_data = new bool [max_parameter_sample_amount];

int*	simplify_list_sum 		= new int  [parameter_max_BDD_width];
int*	simplify_list_sum_half 		= new int  [parameter_max_BDD_width];
int*	simplify_list_first_zero 	= new int  [parameter_max_BDD_width];
int*	simplify_list_first_one 	= new int  [parameter_max_BDD_width];
bool**	simplify_list     		= new bool*[parameter_max_BDD_width];
double	total_finish_weight = 0;
long	total_non_equal_nodes = 0;
class	BDD_node{
public:
	int	left_node_index		=	0;
	int	right_node_index	=	0;
	int	depth			=	0;
	double	weight			=	pow(2.0,20);
	bool	mask[bit_width] 	= 	{0};
	int	sort_in_layer		=	0;
	bool	left_node_neg		=	0;
	bool	right_node_neg		=	0;
	int	non_equal_number	=	0;
	bool	has_equal_father	=	0;
	bool	which_root_node_all[parameter_how_many_root_nodes] = {0};
	int	which_root_node		=	0;
	int	which_bit_output	=	0;

	int	this_layer_bit_expansion= 0;
	bool	switch_to_another_BDD	= 0;
	int	switch_to_which_BDD	= 0;
	int	switch_to_which_node	= 0;
	BDD_node(){
	};
};
class	BDD_node_mask{
public:	
	bool	mask[bit_width] = {0};
	BDD_node_mask(){
	}
};
class BDD_class{
public:
	int	BDD_number;
	bool	this_is_BDD_temp = 0;	

	int i,j,zi,zj,ii,jj;

	int	*	most_influence			= new int [bit_width];
	int	*	BDD_width_each_layer 		= new int [bit_width+1];
	bool	**	random_input_data		= new bool*[max_parameter_sample_amount];
	bool	**	mask_input_data  		= new bool*[max_parameter_sample_amount];
	bool	*	mask_output_data 		= new bool [max_parameter_sample_amount];
	int	*	simplify_list_sum 		= new int  [parameter_max_BDD_width];
	int	*	simplify_list_sum_half 		= new int  [parameter_max_BDD_width];
	int	*	simplify_list_first_zero 	= new int  [parameter_max_BDD_width];
	int	*	simplify_list_first_one 	= new int  [parameter_max_BDD_width];
	bool	**	simplify_list     		= new bool*[parameter_max_BDD_width];
	double		total_finish_weight = 0;
	long		total_non_equal_nodes = 0;

	BDD_node ** 	BDD 		= new BDD_node*[1+bit_width];
	BDD_node_mask*	BDD_mask_this	= new BDD_node_mask[parameter_max_BDD_width];
	BDD_node_mask*	BDD_mask_next	= new BDD_node_mask[parameter_max_BDD_width];
	long		io_read_times;

//	bool**	train_dataset 			= new bool* [parameter_train_dataset_lines_all];
//	bool*	train_dataset_output		= new bool  [parameter_train_dataset_lines_all];
//	int*	train_dataset_which_node 	= new int   [parameter_train_dataset_lines_all];	
//	bool**	train_dataset_temp		= new bool* [parameter_train_dataset_lines_temp];
//	bool*	train_dataset_output_temp	= new bool  [parameter_train_dataset_lines_temp];
//	int*	train_dataset_which_node_temp 	= new int   [parameter_train_dataset_lines_temp];	
//	double*	train_dataset_bit_influence	= new double[bit_width];
//	int	train_dataset_valid_line_amount = parameter_train_dataset_lines;
//	int*	train_dataset_valid_line_index	= new int [train_dataset_valid_line_amount];
//	bool**	wrong_dataset 		= new bool*[parameter_train_dataset_lines_temp];
//	bool*	wrong_dataset_output 	= new bool [parameter_train_dataset_lines_temp];
	
//	bool*	truth_table = new bool [int(pow(2,bit_width))]; 

	bool*	has_been_unfold = new bool[bit_width+1];
	bool***	amount_turn_input_data	= new bool**[bit_width+1];
	bool**	amount_turn_output_data	= new bool* [bit_width+1];
	double*	amount_turn = new double [bit_width];
	int*	simplify_list_sort= new int[parameter_max_BDD_width];
	
	//bool	cal_infer_result(bool input_data[bit_width],BDD_node** BDD,int* most_influence,int which_root_node);
	bool*	io_generator(bool input_data[bit_width]);
	bool	io_generator_single(bool input_data[bit_width],int which_bit_output);
	int	set_random_input_data();
	int	mask_random_input_data(int depth,bool mask[bit_width],int amount,int* most_influence);
	int	cal_most_influence_next(int depth,BDD_node** BDD,int* most_influence);
	int	cal_most_influence_next_old(int depth,BDD_node** BDD,int* most_influence);
	int	cal_most_influence_next_single(int depth,BDD_node** BDD,int* most_influence,int which_node_this_layer);

	int	simplify_list_cluster_how_many_nodes[parameter_how_many_simplify_clusters];
	int*	simplify_list_cluster_nodes[parameter_how_many_simplify_clusters];
	int 	compare_simplify_list(int list_line_amount,bool* this_line,int this_line_sum,int this_line_sum_half,int this_line_first_zero,int this_line_first_one,int which_cluster);
	int 	compare_simplify_list_neg(int list_line_amount,bool* this_line,int this_line_sum,int this_line_sum_half,int this_line_first_zero,int this_line_first_one,int which_cluster);
	
	long	total_nodes_amount = 0;
	long	total_split_nodes  = 0;
	long	total_nodes_amount_recursive = 0;
	long	total_split_nodes_recursive  = 0;
	int	total_BDD_depth	   = 0;
	double	train_time	   = 0;	
	struct timeval	start_time,finish_time;

	int	start_depth = 0;	
	int	how_many_start_nodes = parameter_how_many_root_nodes;
	BDD_node*	start_nodes;
	int 	train_BDD(BDD_node** BDD,int* most_influence,int start_depth,int how_many_start_nodes,BDD_node* start_nodes);

	double	circuit_accuracy;
	double	circuit_accuracy_all_bits[parameter_how_many_root_nodes];
	int	error_amount = 0;
	int	error_amount_all = 0;
	int	train_error  = 0;
	int	incre_wrong_dataset_size = 0;
		
	bool*	test_input_data = new bool[bit_width];
	bool	test_output_data;
	bool	infer_output_data;
	bool*	infer_output_dataset = new bool[parameter_train_dataset_lines_all];
	
	bool*	train_dataset_i = new bool [bit_width];

	int	BDD_infer(BDD_node** BDD,int* most_influence);
	int 	print_circuit(BDD_node** BDD,int* most_influence);
	int	BDD_FULL_PROCESS();
	
	bool*	left_son_mask		= new bool [bit_width];
	bool*	right_son_mask		= new bool [bit_width];
	bool* 	left_mask_output_data	= new bool [max_parameter_sample_amount];
	bool* 	right_mask_output_data	= new bool [max_parameter_sample_amount];

	int		partition_depth = 10000000;
	int		partition_into_how_many_parts  =0;
	int*		partition_start_node_numbers;
	int**		partition_index;
	BDD_class*	BDD_partition;
	BDD_class*	BDD_temp;
};
BDD_class BDD_class_main;

struct timeval	initial_start_time;
int	which_BDD = 0;

extern const	int	cal_data_width = min(bit_width/2,32);
	
bool	cal_infer_result(bool input_data[bit_width],BDD_class* BDD_class_test,int which_root_node){
	bool	infer_result = 0;
	int i;
	int position = which_root_node;
	bool neg = 0;
	for (i=BDD_class_test->start_depth;i<bit_width+10000;i++){
		if(BDD_class_test->BDD[i][position].switch_to_another_BDD == 0){
			if(!input_data[BDD_class_test->most_influence[i]]){
				if(BDD_class_test->BDD[i][position].left_node_index == -1){
					if(neg){
						infer_result = 0;
					}else{
						infer_result = 1;
					}
					break;
				}else if(BDD_class_test->BDD[i][position].left_node_index == -2){
					if(neg){
						infer_result = 1;
					}else{
						infer_result = 0;
					}
					break;
				}else{
					if(BDD_class_test->BDD[i][position].left_node_neg){
						neg = !neg;
					}
					position = BDD_class_test->BDD[i][position].left_node_index;
				}
			}else{
				if(BDD_class_test->BDD[i][position].right_node_index == -1){
					if(neg){
						infer_result = 0;
					}else{
						infer_result = 1;
					}
					break;
				}else if(BDD_class_test->BDD[i][position].right_node_index == -2){
					if(neg){
						infer_result = 1;
					}else{
						infer_result = 0;
					}
					break;
				}else{
					if(BDD_class_test->BDD[i][position].right_node_neg){
						neg = !neg;
					}
					position = BDD_class_test->BDD[i][position].right_node_index;
				}
			}
		}
		else{
			infer_result 	= cal_infer_result(input_data,BDD_class_test->BDD_partition+(BDD_class_test->BDD[i][position].switch_to_which_BDD),BDD_class_test->BDD[i][position].switch_to_which_node);
			break;
		}
	}
	return	infer_result;
};
#include"../demo/demo_functions/io_generator_function.h"
int	which_demo_function;
bool*	BDD_class::io_generator(bool input_data[bit_width]){
	bool*	output_bits;
	output_bits = io_generator_function(input_data,0,1,0,which_demo_function);
	io_read_times += 1;
	return	output_bits;
};

bool	BDD_class::io_generator_single(bool input_data[bit_width],int which_bit_output){
	
	bool	output_bit;
	bool*	output_bits;
	output_bits 	= io_generator(input_data);
	output_bit		= output_bits[which_bit_output] ;
	delete	[] output_bits;
	return	output_bit;
};
int	BDD_class::set_random_input_data(){
	int i,j;
	for (i=0;i<parameter_sample_amount;i++){
		for (j=0;j<bit_width;j++){
			//srand((int)time(0));
			int zi = 0;
			long randint;
			zi = j%30;
			if(zi == 0){
				randint = rand();
			}
			mask_input_data[i][j] = bool((randint >> (zi))%2);
		}
	}
	return 0;
};
int	BDD_class::mask_random_input_data(int depth,bool mask[bit_width],int amount,int* most_influence){
	int i,j;
	for (i=0;i<amount;i++){
		for (j=0;j<depth+1;j++){
			mask_input_data[i][most_influence[j]] = mask[most_influence[j]];
		}
	}

	return 0;
};
int	BDD_class::cal_most_influence_next_single(int depth,BDD_node** BDD,int* most_influence,int which_node_this_layer){
		
	int	most_influence_next=0;
	int 	i,j,k;
	int	zz;
	int	zi;
	bool	should_not_be_unfold[bit_width] = {0};
	for (zz=0;zz<bit_width;zz++){
		amount_turn[zz] = 0;
	}
	long	parameter_sample_mul = 10;
	for (j=0;j<20;j++){
		parameter_sample_mul *= 10;
		if((parameter_sample_mul * (parameter_how_many_root_nodes*1-total_finish_weight/pow(2.0,20))) > 10000){
			break;
		}
	}
	int	parameter_sample_amount_influence = parameter_sample_amount_influence_max;
	//int	which_node_this_layer = 0;
	int	which_node_this_layer_sample = 0;
	int	which_node_this_layer_array[parameter_sample_amount_influence];
	for (zz=0;zz<10000-1;zz++){
		if(depth < parameter_multi_output_index){
			break;
		}
		

		for (i=0;i<parameter_sample_amount_influence;i++){
			
			for (j=0;j<bit_width;j++){
				int zi = 0;
				long randint;
				zi = j%30;
				if(zi == 0){
					randint = rand();
				}
				if(depth == 0){
					mask_input_data[i][j] = bool((randint >> (zi))%2);
				}
				else{
					if(has_been_unfold[j]){
						mask_input_data[i][j] = BDD_mask_this[which_node_this_layer].mask[j];
						//mask_input_data[i][j] = rand()%2;		
					}else{	
						mask_input_data[i][j] = bool((randint >> (zi))%2);		
					}
					
				}
			}	
			which_node_this_layer_array[i] = which_node_this_layer;
		}
		
		for (i=0;i<bit_width+1;i++){
			if(has_been_unfold[i] || should_not_be_unfold[i]){
				
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
					amount_turn_output_data[i][j] = io_generator_single(amount_turn_input_data[i][j],BDD[depth][which_node_this_layer_array[j]].which_bit_output);
				}
			}
		}
		bool	finish_influence_sample = 0;
		for (i=0;i<bit_width;i++){
			if(has_been_unfold[i] || should_not_be_unfold[i]){
				amount_turn[i] = -1;	
			}else{
				for(j=0;j<parameter_sample_amount_influence;j++){
					if(amount_turn_output_data[i][j] && !amount_turn_output_data[bit_width][j]){
						amount_turn[i] += 1;
					}
					else if(!amount_turn_output_data[i][j] && amount_turn_output_data[bit_width][j]){
						amount_turn[i] += 1;
					}
					//if(amount_turn_output_data[i][j]){
					//	amount_turn[i] += 1;
					//}

					///	amount_turn[i] += train_dataset_bit_influence[i];
				}
					
				if((amount_turn[i] > 50)&&(amount_turn[i]<(zz*parameter_sample_amount_influence/2.0))){
					finish_influence_sample = 1;
				}
				if(((zz*parameter_sample_amount_influence-amount_turn[i]) > 50)&&(amount_turn[i]>(zz*parameter_sample_amount_influence/2.0))){
					finish_influence_sample = 1;
				}
								
			}
		}
		
		if(finish_influence_sample){
			break;
		}
	}
	if(zz>10000-1){
		parameter_early_stop_depth_0 = depth+1;
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
	
	return	most_influence_next;
};
int	BDD_class::cal_most_influence_next(int depth,BDD_node** BDD,int* most_influence){
	int	most_influence_next=999999;
	int	most_influence_single=0;
	if(depth < 2000){
		most_influence_next = 	cal_most_influence_next_old(depth,BDD,most_influence);
	}
	else if(depth==bit_width-1){
		most_influence_next = 	cal_most_influence_next_old(depth,BDD,most_influence);
	}
	if(most_influence_next != 999999){
		return most_influence_next;
	}
	else{
		bool	this_bit_most_influence[bit_width] = {0};
		for(int i=0;i<100;i++){
			int which_node_this_layer;
			if(BDD_width_each_layer[depth] < 100){
				which_node_this_layer = i;
				if(i==BDD_width_each_layer[depth]){
					break;
				}
			}
			else{
				which_node_this_layer = rand()%BDD_width_each_layer[depth];
			}
			most_influence_single = 	cal_most_influence_next_single(depth,BDD,most_influence,which_node_this_layer);
			this_bit_most_influence[most_influence_single] = 1;
		}
		int 	i = depth;
		int 	j,zi,zj;
		int*	how_many_nodes_if_expansion = new int [bit_width];
		bool*	left_son_mask = new bool [bit_width];
		bool*	right_son_mask = new bool [bit_width];
		for(j=0;j<bit_width;j++)
			how_many_nodes_if_expansion[j] = -1;
		for(int this_bit_expansion=0;this_bit_expansion<bit_width;this_bit_expansion++){
			bool	left_son_neg;
			bool	right_son_neg;

			int	left_mask_output_data_sum;
			int	right_mask_output_data_sum;
			int	left_mask_output_data_sum_half;
			int	right_mask_output_data_sum_half;
			
			int	which_list_number; 	
			int	which_list_number_neg;

			set_random_input_data();
			if(this_bit_most_influence[this_bit_expansion]){
				how_many_nodes_if_expansion[this_bit_expansion] = 0;
				most_influence[i] = this_bit_expansion;
				for(j=0;j<BDD_width_each_layer[i];j++){
					left_son_neg 	= 0;
					right_son_neg 	= 0;
					for(zi=0;zi<i;zi++){
						left_son_mask[most_influence[zi]] 	= BDD_mask_this[j].mask[most_influence[zi]]; 
						right_son_mask[most_influence[zi]] 	= BDD_mask_this[j].mask[most_influence[zi]]; 
					}
					left_son_mask[most_influence[i]] 	= 0; 
					right_son_mask[most_influence[i]] 	= 1; 
					left_mask_output_data_sum = 0;
					left_mask_output_data_sum_half = 0;
					mask_random_input_data(i,left_son_mask,parameter_sample_amount,most_influence);
					for(zi=0;zi<parameter_sample_amount;zi++){
						left_mask_output_data[zi] = io_generator_single(mask_input_data[zi],BDD[i][j].which_bit_output);
						if(left_mask_output_data[zi]){
							left_mask_output_data_sum += 1;
						}
						if(left_mask_output_data[zi] && (zi<parameter_sample_amount/2)){
							left_mask_output_data_sum_half += 1;
						}
					}
					right_mask_output_data_sum = 0;
					right_mask_output_data_sum_half = 0;
					mask_random_input_data(i,right_son_mask,parameter_sample_amount,most_influence);
					for(zi=0;zi<parameter_sample_amount;zi++){
						right_mask_output_data[zi] = io_generator_single(mask_input_data[zi],BDD[i][j].which_bit_output);
						if(right_mask_output_data[zi]){
							right_mask_output_data_sum += 1;
						}	
						if(right_mask_output_data[zi] && (zi<parameter_sample_amount/2)){
							right_mask_output_data_sum_half += 1;
						}
					}
					bool	all_zero_left  = 1;
					bool	all_one_left   = 1;
					bool	all_one_right  = 1;
					bool	all_zero_right = 1;
				
					int	first_one_left 		= -1;
					int	first_zero_left 	= -1;
					int	first_one_right 	= -1;
					int	first_zero_right 	= -1;
			
					for(zi=0;zi<parameter_sample_amount;zi++){
						if(left_mask_output_data[zi]){
							all_zero_left 	= 0;
							first_one_left	= zi;
						}else{
							all_one_left 	= 0;
							first_zero_left = zi;
						}
						if((!all_zero_left)&&(!all_one_left)){
							break;
						}
					}
					for(zi=0;zi<parameter_sample_amount;zi++){
						if(right_mask_output_data[zi]){
							all_zero_right 		= 0;
							first_one_right		= zi;
						}else{
							all_one_right 		= 0;
							first_zero_right	= zi;
						}
						if((!all_zero_right)&&(!all_one_right)){
							break;
						}
					}
						if(all_zero_left){
						}else if(all_one_left){
						}else{
							int	which_cluster	= left_mask_output_data_sum % parameter_how_many_simplify_clusters;
							which_list_number 	= compare_simplify_list    (how_many_nodes_if_expansion[this_bit_expansion],left_mask_output_data,left_mask_output_data_sum,left_mask_output_data_sum_half,first_zero_left,first_one_left,which_cluster);
							which_list_number_neg 	= compare_simplify_list_neg(how_many_nodes_if_expansion[this_bit_expansion],left_mask_output_data,parameter_sample_amount-left_mask_output_data_sum,int(parameter_sample_amount/2)-left_mask_output_data_sum_half,first_zero_left,first_one_left,which_cluster);
							bool	it_cannot_simplify 	= (which_list_number < 0)&&(which_list_number_neg<0);
							bool	it_can_simplify 	= (which_list_number 		>=0);
							bool	it_can_simplify_neg 	= (which_list_number_neg	>=0);
							
							if((it_can_simplify_neg == 0) && (it_can_simplify == 0)){
								it_cannot_simplify = 1;
							}
							if(it_cannot_simplify){
								how_many_nodes_if_expansion[this_bit_expansion] += 1;
								for (zi=0;zi<parameter_sample_amount;zi++){
									simplify_list[how_many_nodes_if_expansion[this_bit_expansion]-1][zi] = left_mask_output_data[zi];
								}
								simplify_list_sum[how_many_nodes_if_expansion[this_bit_expansion]-1]				= left_mask_output_data_sum;
								simplify_list_sum_half[how_many_nodes_if_expansion[this_bit_expansion]-1]			= left_mask_output_data_sum_half;
								simplify_list_first_zero[how_many_nodes_if_expansion[this_bit_expansion]-1]			= first_zero_left;
								simplify_list_first_one[how_many_nodes_if_expansion[this_bit_expansion]-1]			= first_one_left;
								simplify_list_cluster_nodes[which_cluster][simplify_list_cluster_how_many_nodes[which_cluster]]	= how_many_nodes_if_expansion[this_bit_expansion]-1;
								simplify_list_cluster_how_many_nodes[which_cluster] += 1;
							}
						}
						if(all_zero_right){
						}else if(all_one_right){
						}else{
							int	which_cluster	= right_mask_output_data_sum % parameter_how_many_simplify_clusters;
							which_list_number 	= compare_simplify_list(how_many_nodes_if_expansion[this_bit_expansion],right_mask_output_data,right_mask_output_data_sum,right_mask_output_data_sum_half,first_zero_right,first_one_right,which_cluster);
							which_list_number_neg 	= compare_simplify_list_neg(how_many_nodes_if_expansion[this_bit_expansion],right_mask_output_data,parameter_sample_amount-right_mask_output_data_sum,int(parameter_sample_amount/2)-right_mask_output_data_sum,first_zero_left,first_one_left,which_cluster);
							bool	it_cannot_simplify 	= (which_list_number < 0)&&(which_list_number_neg<0);
							bool	it_can_simplify 	= (which_list_number 		>=0);
							bool	it_can_simplify_neg 	= (which_list_number_neg	>=0);
							
							if((it_can_simplify_neg == 0) && (it_can_simplify == 0)){
								it_cannot_simplify = 1;
							}
							if(it_cannot_simplify){
								how_many_nodes_if_expansion[this_bit_expansion] += 1;
								for (zi=0;zi<parameter_sample_amount;zi++){
									simplify_list[how_many_nodes_if_expansion[this_bit_expansion]-1][zi] = right_mask_output_data[zi];
								}
								simplify_list_sum[how_many_nodes_if_expansion[this_bit_expansion]-1]				= right_mask_output_data_sum;
								simplify_list_sum_half[how_many_nodes_if_expansion[this_bit_expansion]-1]			= right_mask_output_data_sum_half;
								simplify_list_first_zero[how_many_nodes_if_expansion[this_bit_expansion]-1]			= first_zero_right;
								simplify_list_first_one[how_many_nodes_if_expansion[this_bit_expansion]-1]			= first_one_right;
								simplify_list_cluster_nodes[which_cluster][simplify_list_cluster_how_many_nodes[which_cluster]]	= how_many_nodes_if_expansion[this_bit_expansion]-1;
								simplify_list_cluster_how_many_nodes[which_cluster] += 1;

							}
						}
				}
			}
		}
		
		int best_expansion_node = 0;
		int best_expansion_node_width = 10000000;
		for(zi=0;zi<bit_width;zi++){
			if(has_been_unfold[zi]){
				how_many_nodes_if_expansion[zi] = -2;
			}
			if(how_many_nodes_if_expansion[zi]>=0){
				if(how_many_nodes_if_expansion[zi]<best_expansion_node_width){
					best_expansion_node = zi;
					best_expansion_node_width = how_many_nodes_if_expansion[zi];
				}
			}
		}
			
		most_influence_next = best_expansion_node; 
		has_been_unfold[most_influence_next] = 1;
	}
	return	most_influence_next;
};
int	BDD_class::cal_most_influence_next_old(int depth,BDD_node** BDD,int* most_influence){
		
	int	most_influence_next=0;
	int 	i,j,k;
	int	zz;
	int	zi;
	bool	should_not_be_unfold[bit_width] = {0};
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
	int	parameter_sample_amount_influence = parameter_sample_amount_influence_max;
	for (zz=0;zz<10000;zz++){
		if(depth < parameter_multi_output_index){
			break;
		}
		
		int	which_node_this_layer = 0;
		int	which_node_this_layer_sample = 0;
		int	which_node_this_layer_array[parameter_sample_amount_influence];
		for (i=0;i<parameter_sample_amount_influence;i++){
			
			which_node_this_layer = rand()%BDD_width_each_layer[depth];
			for (j=0;j<bit_width;j++){
				int zi = 0;
				long randint;
				zi = j%30;
				if(zi == 0){
					randint = rand();
				}
				if(depth == 0){
					mask_input_data[i][j] = bool((randint >> (zi))%2);
				}
				else{
					if(has_been_unfold[j]){
						mask_input_data[i][j] = BDD_mask_this[which_node_this_layer].mask[j];
					}else{	
						mask_input_data[i][j] = bool((randint >> (zi))%2);		
					}
					
				}
			}	
			which_node_this_layer_array[i] = which_node_this_layer;
		}
		
		for (i=0;i<bit_width+1;i++){
			if(has_been_unfold[i] || should_not_be_unfold[i]){
				
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
					amount_turn_output_data[i][j] = io_generator_single(amount_turn_input_data[i][j],BDD[depth][which_node_this_layer_array[j]].which_bit_output);
				}
			}
		}
		bool	finish_influence_sample = 0;
		for (i=0;i<bit_width;i++){
			if(has_been_unfold[i] || should_not_be_unfold[i]){
				amount_turn[i] = -1;	
			}else{
				for(j=0;j<parameter_sample_amount_influence;j++){
					if(amount_turn_output_data[i][j] && !amount_turn_output_data[bit_width][j]){
						amount_turn[i] += 1;
					}
					else if(!amount_turn_output_data[i][j] && amount_turn_output_data[bit_width][j]){
						amount_turn[i] += 1;
					}
				}
				if((amount_turn[i] > 80) && ((zz-amount_turn[i])>80)){
					finish_influence_sample = 1;
				}
				
			}
		}
		
		if(finish_influence_sample){
			break;
		}
	}
	double amount_turn_all = 0;
	double amount_turn_valid_bits = 0;
	int	amount_turn_static[bit_width];
	for (i=0;i<bit_width;i++){
		if(amount_turn[i]>((zz*parameter_sample_amount_influence)/2.0)){
			amount_turn_static[i] = amount_turn[i];
		}else{
			amount_turn_static[i] = amount_turn[i];
		}
		if(amount_turn_static[i]>=1){
			amount_turn_all = amount_turn_all + max(1,amount_turn_static[i]);
			amount_turn_valid_bits = amount_turn_valid_bits + 1;
		}
	}
	double amount_turn_average = (amount_turn_all+1)/amount_turn_valid_bits;
	most_influence_next = max_element(amount_turn_static,amount_turn_static+bit_width) - amount_turn_static;	
	for (i=0;i<bit_width;i++){
		if(has_been_unfold[most_influence_next]){
			amount_turn[most_influence_next] = -1;
		}else{
			break;	//has_been_unfold[most_influence_next] = 1;
		}
		most_influence_next = max_element(amount_turn_static,amount_turn_static+bit_width) - amount_turn_static;
	}
	has_been_unfold[most_influence_next] = 1;
	double amount_turn_max_divide_average = amount_turn_static[most_influence_next]/amount_turn_average;
	double amount_turn_max_ratio = amount_turn[most_influence_next]/(zz+1);
	if((depth<bit_width-2) ){
		if((amount_turn_max_divide_average < 4) || (amount_turn_max_ratio < 0.3) || ((amount_turn_max_ratio>0.8)&&((amount_turn_max_ratio<0.999)))){
			has_been_unfold[most_influence_next] = 0;
			most_influence_next = 999999;
		}
	}	
	//delete[]amount_turn;
	return	most_influence_next;
};
int	total_compare_times=0;
int BDD_class::compare_simplify_list(int list_line_amount,bool* this_line,int this_line_sum,int this_line_sum_half,int this_line_first_zero,int this_line_first_one,int which_cluster){
	int which_list_number = -1;
	bool it_can_simplify  = 0;
	int i,j;
	bool it_can_simplify_here;
	which_list_number = -1;
	if(1){
		int zi;
		for (zi=simplify_list_cluster_how_many_nodes[which_cluster]-1;zi>=0;zi--){
			total_compare_times += 1;
			i = simplify_list_cluster_nodes[which_cluster][zi];
			it_can_simplify_here = 1;
			if(simplify_list_sum[i] != this_line_sum){
				it_can_simplify_here = 0;
				continue;
			}
			if(simplify_list_sum_half[i] != this_line_sum_half){
				it_can_simplify_here = 0;
				continue;
			}
			if(simplify_list_first_zero[i] != this_line_first_zero){
				it_can_simplify_here = 0;
				continue;
			}
			if(simplify_list_first_one[i] != this_line_first_one){
				it_can_simplify_here = 0;
				continue;
			}
			int first_change = max(this_line_first_one,this_line_first_zero);
			for(j=first_change;j<parameter_sample_amount;j++){
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
	}else{	
		for (i=list_line_amount-1;i>=0;i--){
			total_compare_times += 1;
			it_can_simplify_here = 1;
			if(simplify_list_sum[i] != this_line_sum){
				it_can_simplify_here = 0;
				continue;
			}
			if(simplify_list_sum_half[i] != this_line_sum_half){
				it_can_simplify_here = 0;
				continue;
			}
			if(simplify_list_first_zero[i] != this_line_first_zero){
				it_can_simplify_here = 0;
				continue;
			}
			if(simplify_list_first_one[i] != this_line_first_one){
				it_can_simplify_here = 0;
				continue;
			}
			int first_change = max(this_line_first_one,this_line_first_zero);
			for(j=first_change;j<parameter_sample_amount;j++){
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
	}
	return which_list_number;
};

int BDD_class::compare_simplify_list_neg(int list_line_amount,bool* this_line,int this_line_sum,int this_line_sum_half,int this_line_first_zero,int this_line_first_one,int which_cluster){
	int which_list_number = -1;
	bool it_can_simplify  = 0;
	int i,j;
	bool it_can_simplify_here = 1;
	return which_list_number;
	if(1){
		int zi;	
		int neg_which_cluster = 0;
		for (zi=simplify_list_cluster_how_many_nodes[neg_which_cluster]-1;zi>=0;zi--){
			total_compare_times += 1;
			i = simplify_list_cluster_nodes[neg_which_cluster][zi];
			it_can_simplify_here = 1;
			if(simplify_list_sum[i] != this_line_sum){
				it_can_simplify_here = 0;
				continue;
			}
			if(simplify_list_sum_half[i] != this_line_sum_half){
				it_can_simplify_here = 0;
				continue;
			}
			if(simplify_list_first_zero[i] != this_line_first_one){
				it_can_simplify_here = 0;
				continue;
			}
			if(simplify_list_first_one[i] != this_line_first_zero){
				it_can_simplify_here = 0;
				continue;
			}
			int first_change = max(this_line_first_one,this_line_first_zero);
			for(j=first_change;j<parameter_sample_amount;j++){
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
	}else{
		for (i=list_line_amount-1;i>=0;i--){
			total_compare_times += 1;
			it_can_simplify_here = 1;
			if(simplify_list_sum[i] != this_line_sum){
				it_can_simplify_here = 0;
				continue;
			}
			if(simplify_list_sum_half[i] != this_line_sum_half){
				it_can_simplify_here = 0;
				continue;
			}
			if(simplify_list_first_zero[i] != this_line_first_one){
				it_can_simplify_here = 0;
				continue;
			}
			if(simplify_list_first_one[i] != this_line_first_zero){
				it_can_simplify_here = 0;
				continue;
			}
			int first_change = max(this_line_first_one,this_line_first_zero);
			for(j=first_change;j<parameter_sample_amount;j++){
				if(simplify_list[i][j] == this_line[j]){
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
	}
	return which_list_number;
};


int BDD_class::train_BDD(BDD_node** BDD,int* most_influence,int start_depth, int how_many_start_nodes, BDD_node* start_nodes){

	int i,j;
	int zi;
	for(i=0;i<start_depth+1;i++)	
		BDD[i] 		= new BDD_node[how_many_start_nodes];
	for (i=0;i<how_many_start_nodes;i++){
		BDD[start_depth][i].which_bit_output = start_nodes[i].which_bit_output;
		BDD[start_depth][i].which_root_node  = start_nodes[i].which_root_node;
		for(zi=0;zi<how_many_start_nodes;zi++){
			BDD[start_depth][i].which_root_node_all[zi] = 0;
		}		
		BDD[start_depth][i].which_root_node_all[i] = 1;
		for (zi=0;zi<bit_width;zi++){
			BDD[start_depth][i].mask[zi] 	= start_nodes[i].mask[zi];
			BDD_mask_this[i].mask[zi] 	= start_nodes[i].mask[zi];
		}
	}
	total_finish_weight = 0;

	BDD_width_each_layer[start_depth] = how_many_start_nodes;
	total_nodes_amount = 0;
	total_split_nodes  = 0;
	total_BDD_depth	   = 0;
	train_time	   = 0;
	for (int zi=0;zi<parameter_how_many_simplify_clusters;zi++){
		simplify_list_cluster_nodes[zi] = new int [2*parameter_max_BDD_width+2];
		simplify_list_cluster_how_many_nodes[zi] = 0;
	}
	
	for(i=start_depth;i</*parameter_early_stop_depth*/bit_width+1;i++){
		cout<<"BSD input bit sequence :	";
		for(int zi=0;zi<i;zi++){
			cout<<most_influence[zi]<<" ";
		}
		cout<<endl;
	
		if(i>start_depth){	
			for(j=0;j<BDD_width_each_layer[i];j++){
				for (zi=0;zi<bit_width;zi++){
					BDD[i][j].mask[zi] 		= BDD_mask_next[j].mask[zi];
					BDD_mask_this[j].mask[zi] 	= BDD_mask_next[j].mask[zi];
				}	
			}
		}
				total_nodes_amount_recursive 	= total_nodes_amount;
		total_split_nodes_recursive 	= total_split_nodes;
		if(i == partition_depth){
			cout<<"go for partition"<<endl;
			BDD_partition 			= new BDD_class [partition_into_how_many_parts];
			partition_start_node_numbers 	= new int  [partition_into_how_many_parts];
			partition_index 		= new int* [partition_into_how_many_parts];
			partition_start_node_numbers[0] = 0;
			partition_start_node_numbers[1] = 0;
			
			for(zi=0;zi<partition_into_how_many_parts;zi++){
				for(int zj = 0; zj < BDD_width_each_layer[i];zj++){
					if(BDD[i][zj].mask[most_influence[i-1]] == zi){
						partition_start_node_numbers[zi] += 1;;
					}
				}
			}
			cout<<"finish partition node numbers"<<endl;	
				for(int zj=0;zj<partition_into_how_many_parts;zj++){	
					partition_index[zj] = new int [partition_start_node_numbers[zj]];
					int counter = 0;
					for(int zk=0;zk<BDD_width_each_layer[i];zk++){
						if(BDD[i][zk].mask[most_influence[i-1]] == zj){
							partition_index[zj][counter] = zk;
							counter += 1;
						}
					}
				}
			cout<<"finish partition index"<<endl;	
			for(zi=0;zi<partition_into_how_many_parts;zi++){
				BDD_partition[zi].BDD_number 	= BDD_number_counter;
				BDD_number_counter		+=1;
				BDD_partition[zi].start_depth = partition_depth;	
				///BDD_partition[zi].how_many_start_nodes = 32;
				BDD_partition[zi].start_nodes = new BDD_node [BDD_partition[zi].how_many_start_nodes];
				for(int zj=0;zj<bit_width+1;zj++){
					BDD_partition[zi].has_been_unfold[zj] 	= has_been_unfold[zj];
					BDD_partition[zi].most_influence[zj] 	= most_influence[zj];
				}
				for(int zj=0;zj<BDD_partition[zi].how_many_start_nodes;zj++){
					if(partition_index[zi][zj] < BDD_width_each_layer[i]){
						BDD_partition[zi].start_nodes[zj].which_bit_output 	= BDD[i][partition_index[zi][zj]].which_bit_output;
						BDD_partition[zi].start_nodes[zj].which_root_node  	= BDD[i][partition_index[zi][zj]].which_root_node;
						for (int zk=0;zk<bit_width;zk++){
							BDD_partition[zi].start_nodes[zj].mask[zk]	= BDD[i][partition_index[zi][zj]].mask[zk];
							BDD_partition[zi].BDD_mask_this[zj].mask[zk] 	= BDD[i][partition_index[zi][zj]].mask[zk];
						}
					}
				}
				for(int zj=0;zj<bit_width+1;zj++){
					BDD_partition[zi].BDD_width_each_layer[zj] = BDD_width_each_layer[zj];
				}
				if(!this_is_BDD_temp){	
					
					cout<<"switch_BDD"<<endl;
					BDD_partition[zi].BDD_FULL_PROCESS();
					total_nodes_amount_recursive 	+= BDD_partition[zi].total_nodes_amount_recursive;	
					total_split_nodes_recursive 	+= BDD_partition[zi].total_split_nodes_recursive;	
				}
			}
			for(j=0;j<BDD_width_each_layer[i];j++){
				BDD[i][j].switch_to_another_BDD = 1;
				for(zi=0;zi<partition_into_how_many_parts;zi++){
					for(int zj=0;zj<BDD_partition[zi].how_many_start_nodes;zj++){
						if(partition_index[zi][zj] == j){
							BDD[i][j].switch_to_which_BDD 	= zi;
							BDD[i][j].switch_to_which_node 	= zj;
						}
					}
				}
			}		
			total_BDD_depth = i;
			cout<<"BDD partition on layer "<<i<<endl;
			break;
		}	
		
		for (zi=0;zi<parameter_how_many_simplify_clusters;zi++){
			simplify_list_cluster_how_many_nodes[zi] = 0;
		}

		if(total_finish_weight/(pow(2.0,20)*parameter_how_many_root_nodes) > 0.9999999999){
			parameter_sample_amount_train = 10;
			if(total_finish_weight/(pow(2.0,20)*parameter_how_many_root_nodes) > 1){
				parameter_sample_amount_train = 2;
			}
		}else{
			parameter_sample_amount_train = max(4,min(int(max_parameter_sample_amount_all * (1-total_finish_weight/(pow(2.0,20)*parameter_how_many_root_nodes))),max_parameter_sample_amount));
		}
	
		if((BDD_width_each_layer[i]==0)){
			total_BDD_depth = i;
			cout<<"total BDD depth:	"<<total_BDD_depth<<endl;
			break;
		}
		
		total_nodes_amount += BDD_width_each_layer[i];

		BDD[i+1] 	= new BDD_node[BDD_width_each_layer[i]*2];
		
		for(zi=0;zi<BDD_width_each_layer[i]*2;zi++){
			simplify_list_sort[zi] = zi;
			BDD[i+1][zi].has_equal_father = 0;
			BDD[i+1][zi].non_equal_number = 0;
		}
		parameter_sample_amount = parameter_sample_amount_sort;
		most_influence[i] = cal_most_influence_next(i,BDD,most_influence);
 
		for (zi=0;zi<parameter_how_many_simplify_clusters;zi++){
			simplify_list_cluster_how_many_nodes[zi] = 0;
		}
		parameter_sample_amount = parameter_sample_amount_train;
		cout<<parameter_sample_amount<<endl;
		cout<<"The BSD is on layer: "<<i<<"		";
		cout<<"The input bit is: x"<<most_influence[i]<<endl;
		cout<<"BSD width at this layer: "<<BDD_width_each_layer[i]<<endl;
		set_random_input_data();
		
		bool	left_son_neg;
		bool	right_son_neg;

		int	left_mask_output_data_sum;
		int	right_mask_output_data_sum;
		int	left_mask_output_data_sum_half;
		int	right_mask_output_data_sum_half;
		
		int	which_list_number; 	
		int	which_list_number_neg;
 		
		for(j=0;j<BDD_width_each_layer[i];j++){
			left_son_neg 	= 0;
			right_son_neg 	= 0;
		
			BDD[i][j].this_layer_bit_expansion = most_influence[i];	
			
			if(BDD[i][j].has_equal_father){
				;
			}else{
				BDD[i][j].non_equal_number = total_non_equal_nodes;
				total_non_equal_nodes += 1;
			}

			for(zi=0;zi<i;zi++){
				left_son_mask[most_influence[zi]] 	= BDD_mask_this[j].mask[most_influence[zi]]; 
				right_son_mask[most_influence[zi]] 	= BDD_mask_this[j].mask[most_influence[zi]]; 
			}
			left_son_mask[most_influence[i]] 	= 0;
			right_son_mask[most_influence[i]] 	= 1;
			
			
			left_mask_output_data_sum = 0;
			left_mask_output_data_sum_half = 0;
			mask_random_input_data(i,left_son_mask,parameter_sample_amount,most_influence);
			for(zi=0;zi<parameter_sample_amount;zi++){
				left_mask_output_data[zi] = io_generator_single(mask_input_data[zi],BDD[i][j].which_bit_output);
				if(left_mask_output_data[zi]){
					left_mask_output_data_sum += 1;
				}
				if(left_mask_output_data[zi] && (zi<parameter_sample_amount/2)){
					left_mask_output_data_sum_half += 1;
				}
			}
			right_mask_output_data_sum = 0;
			right_mask_output_data_sum_half = 0;
			mask_random_input_data(i,right_son_mask,parameter_sample_amount,most_influence);
			for(zi=0;zi<parameter_sample_amount;zi++){
				right_mask_output_data[zi] = io_generator_single(mask_input_data[zi],BDD[i][j].which_bit_output);
				if(right_mask_output_data[zi]){
					right_mask_output_data_sum += 1;
				}	
				if(right_mask_output_data[zi] && (zi<parameter_sample_amount/2)){
					right_mask_output_data_sum_half += 1;
				}
			}
			bool	all_zero_left  = 1;
			bool	all_one_left   = 1;
			bool	all_one_right  = 1;
			bool	all_zero_right = 1;
		
			int	first_one_left 		= -1;
			int	first_zero_left 	= -1;
			int	first_one_right 	= -1;
			int	first_zero_right 	= -1;
	
			for(zi=0;zi<parameter_sample_amount;zi++){
				if(left_mask_output_data[zi]){
					all_zero_left 	= 0;
					first_one_left	= zi;
				}else{
					all_one_left 	= 0;
					first_zero_left = zi;
				}
				if((!all_zero_left)&&(!all_one_left)){
					break;
				}
			}
			for(zi=0;zi<parameter_sample_amount;zi++){
				if(right_mask_output_data[zi]){
					all_zero_right 		= 0;
					first_one_right		= zi;
				}else{
					all_one_right 		= 0;
					first_zero_right	= zi;
				}
				if((!all_zero_right)&&(!all_one_right)){
					break;
				}
			}
			if((i == (parameter_early_stop_depth_0-1))&&(which_BDD==0) || ((i == (parameter_early_stop_depth_1-1))&&(which_BDD==1))){
				if(left_mask_output_data_sum > (parameter_sample_amount/2)){
					all_one_left 	= 1;
					all_zero_left	= 0;
				}else{
					all_one_left 	= 0;
					all_zero_left	= 1;
				}
				if(right_mask_output_data_sum > (parameter_sample_amount/2)){
					all_one_right 	= 1;
					all_zero_right	= 0;
				}else{
					all_one_right 	= 0;
					all_zero_right	= 1;
				}
			}
			if((BDD_width_each_layer[i] > parameter_max_BDD_width) && this_is_BDD_temp){
				if(left_mask_output_data_sum > (parameter_sample_amount/2)){
					all_one_left 	= 1;
					all_zero_left	= 0;
				}else{
					all_one_left 	= 0;
					all_zero_left	= 1;
				}
				if(right_mask_output_data_sum > (parameter_sample_amount/2)){
					all_one_right 	= 1;
					all_zero_right	= 0;
				}else{
					all_one_right 	= 0;
					all_zero_right	= 1;
				}
			}

			if(i>parameter_depth_start_simplify){
				if(all_zero_left){
					BDD[i][j].left_node_index = -2;
					total_finish_weight += BDD[i][j].weight/2;
				}else if(all_one_left){
					BDD[i][j].left_node_index = -1; 
					total_finish_weight += BDD[i][j].weight/2;
				}else{
					if(((i%parameter_how_often_simplify) == 0)){
						int	which_cluster	= left_mask_output_data_sum % parameter_how_many_simplify_clusters;
						which_list_number 	= compare_simplify_list    (BDD_width_each_layer[i+1],left_mask_output_data,left_mask_output_data_sum,left_mask_output_data_sum_half,first_zero_left,first_one_left,which_cluster);
						which_list_number_neg 	= compare_simplify_list_neg(BDD_width_each_layer[i+1],left_mask_output_data,parameter_sample_amount-left_mask_output_data_sum,int(parameter_sample_amount/2)-left_mask_output_data_sum_half,first_zero_left,first_one_left,which_cluster);
						bool	it_cannot_simplify 	= (which_list_number < 0)&&(which_list_number_neg<0);
						bool	it_can_simplify 	= (which_list_number 		>=0);
						bool	it_can_simplify_neg 	= (which_list_number_neg	>=0);
						if((it_can_simplify_neg == 0) && (it_can_simplify == 0)){
							it_cannot_simplify = 1;
						}
						if(it_cannot_simplify){
							BDD_width_each_layer[i+1] += 1;
							BDD[i+1][BDD_width_each_layer[i+1]-1].depth  = i+1; 
							BDD[i+1][BDD_width_each_layer[i+1]-1].weight = BDD[i][j].weight/2;
							for(zi=0;zi<parameter_how_many_root_nodes;zi++){
								BDD[i+1][BDD_width_each_layer[i+1]-1].which_root_node_all[zi] = BDD[i][j].which_root_node_all[zi]; 
							} 
							BDD[i+1][BDD_width_each_layer[i+1]-1].which_root_node = BDD[i][j].which_root_node; 
							BDD[i+1][BDD_width_each_layer[i+1]-1].which_root_node = BDD[i][j].which_root_node; 
							BDD[i+1][BDD_width_each_layer[i+1]-1].which_bit_output = BDD[i][j].which_bit_output; 
							for (zi=0;zi<i+1;zi++){
								BDD_mask_next[BDD_width_each_layer[i+1]-1].mask[most_influence[zi]]  = left_son_mask[most_influence[zi]]; 
							}
							BDD[i][j].left_node_index = 	BDD_width_each_layer[i+1]-1;
							BDD[i][j].left_node_neg   = 	0;
							for (zi=0;zi<parameter_sample_amount;zi++){
								simplify_list[BDD_width_each_layer[i+1]-1][zi] = left_mask_output_data[zi];
							}
							simplify_list_sum[BDD_width_each_layer[i+1]-1]				= left_mask_output_data_sum;
							simplify_list_sum_half[BDD_width_each_layer[i+1]-1]			= left_mask_output_data_sum_half;
							simplify_list_first_zero[BDD_width_each_layer[i+1]-1]			= first_zero_left;
							simplify_list_first_one[BDD_width_each_layer[i+1]-1]			= first_one_left;
							simplify_list_cluster_nodes[which_cluster][simplify_list_cluster_how_many_nodes[which_cluster]]	= BDD_width_each_layer[i+1]-1;
							simplify_list_cluster_how_many_nodes[which_cluster] += 1;
						}else if(it_can_simplify){
							BDD[i][j].left_node_index = 	which_list_number;
							BDD[i][j].left_node_neg   = 	0;
							BDD[i+1][which_list_number].weight += BDD[i][j].weight/2;
							for(zi=0;zi<parameter_how_many_root_nodes;zi++){
								BDD[i+1][which_list_number].which_root_node_all[zi] |= BDD[i][j].which_root_node_all[zi]; 
							} 
						}else if(it_can_simplify_neg){
							BDD[i][j].left_node_index = 	which_list_number_neg;
							BDD[i][j].left_node_neg   = 	1;
							BDD[i+1][which_list_number_neg].weight += BDD[i][j].weight/2;
							for(zi=0;zi<parameter_how_many_root_nodes;zi++){
								BDD[i+1][which_list_number_neg].which_root_node_all[zi] |= BDD[i][j].which_root_node_all[zi]; 
							}
						}
					}else{
						BDD_width_each_layer[i+1] += 1;
						BDD[i+1][BDD_width_each_layer[i+1]-1].depth = i+1; 
						BDD[i+1][BDD_width_each_layer[i+1]-1].weight = BDD[i][j].weight/2; 
						BDD[i+1][BDD_width_each_layer[i+1]-1].which_root_node = BDD[i][j].which_root_node; 
						BDD[i+1][BDD_width_each_layer[i+1]-1].which_bit_output = BDD[i][j].which_bit_output; 
						for(zi=0;zi<parameter_how_many_root_nodes;zi++){
							BDD[i+1][BDD_width_each_layer[i+1]-1].which_root_node_all[zi] = BDD[i][j].which_root_node_all[zi]; 
						} 
						for (zi=0;zi<i+1;zi++){
							BDD_mask_next[BDD_width_each_layer[i+1]-1].mask[most_influence[zi]]  = left_son_mask[most_influence[zi]]; 
						}
						BDD[i][j].left_node_index = 	BDD_width_each_layer[i+1]-1;
						BDD[i][j].left_node_neg   = 	0;
						
					}
				}
			}else{			
						BDD_width_each_layer[i+1] += 1;
						BDD[i+1][BDD_width_each_layer[i+1]-1].depth = i+1; 
						BDD[i+1][BDD_width_each_layer[i+1]-1].weight = BDD[i][j].weight/2; 
						BDD[i+1][BDD_width_each_layer[i+1]-1].which_root_node = BDD[i][j].which_root_node; 
						BDD[i+1][BDD_width_each_layer[i+1]-1].which_bit_output = BDD[i][j].which_bit_output; 	
						for(zi=0;zi<parameter_how_many_root_nodes;zi++){
							BDD[i+1][BDD_width_each_layer[i+1]-1].which_root_node_all[zi] = BDD[i][j].which_root_node_all[zi]; 
						} 
						for (zi=0;zi<i+1;zi++){                                                              	
							BDD_mask_next[BDD_width_each_layer[i+1]-1].mask[most_influence[zi]]  = left_son_mask[most_influence[zi]]; 
						}
						BDD[i][j].left_node_index = 	BDD_width_each_layer[i+1]-1;
						BDD[i][j].left_node_neg   = 	0;

			}
			if(i>parameter_depth_start_simplify){
				if(all_zero_right){
					BDD[i][j].right_node_index = -2; 
					total_finish_weight += BDD[i][j].weight/2;
				}else if(all_one_right){
					BDD[i][j].right_node_index = -1; 
					total_finish_weight += BDD[i][j].weight/2;
				}else{
					if(((i%parameter_how_often_simplify == 0))){
						int	which_cluster	= right_mask_output_data_sum % parameter_how_many_simplify_clusters;
						which_list_number 	= compare_simplify_list(BDD_width_each_layer[i+1],right_mask_output_data,right_mask_output_data_sum,right_mask_output_data_sum_half,first_zero_right,first_one_right,which_cluster);
						which_list_number_neg 	= compare_simplify_list_neg(BDD_width_each_layer[i+1],right_mask_output_data,parameter_sample_amount-right_mask_output_data_sum,int(parameter_sample_amount/2)-right_mask_output_data_sum,first_zero_left,first_one_left,which_cluster);
						bool	it_cannot_simplify 	= (which_list_number < 0)&&(which_list_number_neg<0);
						bool	it_can_simplify 	= (which_list_number 		>=0);
						bool	it_can_simplify_neg 	= (which_list_number_neg	>=0);
						if((it_can_simplify_neg == 0) && (it_can_simplify == 0)){
							it_cannot_simplify = 1;
						}
						if(it_cannot_simplify){
							BDD_width_each_layer[i+1] += 1;
							BDD[i+1][BDD_width_each_layer[i+1]-1].depth = i+1; 
							BDD[i+1][BDD_width_each_layer[i+1]-1].weight = BDD[i][j].weight/2; 
							BDD[i+1][BDD_width_each_layer[i+1]-1].which_root_node = BDD[i][j].which_root_node; 
							BDD[i+1][BDD_width_each_layer[i+1]-1].which_bit_output = BDD[i][j].which_bit_output; 
							for(zi=0;zi<parameter_how_many_root_nodes;zi++){
								BDD[i+1][BDD_width_each_layer[i+1]-1].which_root_node_all[zi] = BDD[i][j].which_root_node_all[zi]; 
							} 
							for (zi=0;zi<i+1;zi++){
								BDD_mask_next[BDD_width_each_layer[i+1]-1].mask[most_influence[zi]]  = right_son_mask[most_influence[zi]]; 
							}
							BDD[i][j].right_node_index = 	BDD_width_each_layer[i+1]-1;
							BDD[i][j].right_node_neg    = 	0;
							for (zi=0;zi<parameter_sample_amount;zi++){
								simplify_list[BDD_width_each_layer[i+1]-1][zi] 	= right_mask_output_data[zi];
							}
							simplify_list_sum[BDD_width_each_layer[i+1]-1]				= right_mask_output_data_sum;
							simplify_list_sum_half[BDD_width_each_layer[i+1]-1]			= right_mask_output_data_sum_half;
							simplify_list_first_zero[BDD_width_each_layer[i+1]-1]			= first_zero_right;
							simplify_list_first_one[BDD_width_each_layer[i+1]-1]			= first_one_right;
							simplify_list_cluster_nodes[which_cluster][simplify_list_cluster_how_many_nodes[which_cluster]]	= BDD_width_each_layer[i+1]-1;
							simplify_list_cluster_how_many_nodes[which_cluster] += 1;
						}else if(it_can_simplify){
							BDD[i][j].right_node_index = 	which_list_number;
							BDD[i][j].right_node_neg   = 0;
							BDD[i+1][which_list_number].weight += BDD[i][j].weight/2;
							for(zi=0;zi<parameter_how_many_root_nodes;zi++){
								BDD[i+1][which_list_number].which_root_node_all[zi] |= BDD[i][j].which_root_node_all[zi]; 
							} 
						}else if(it_can_simplify_neg){
							BDD[i][j].right_node_index = 	which_list_number_neg;
							BDD[i][j].right_node_neg   = 1;
							BDD[i+1][which_list_number_neg].weight += BDD[i][j].weight/2;
							for(zi=0;zi<parameter_how_many_root_nodes;zi++){
								BDD[i+1][which_list_number_neg].which_root_node_all[zi] |= BDD[i][j].which_root_node_all[zi]; 
							} 
						}
					}else{
						BDD_width_each_layer[i+1] += 1;
						BDD[i+1][BDD_width_each_layer[i+1]-1].depth = i+1; 
						BDD[i+1][BDD_width_each_layer[i+1]-1].weight = BDD[i][j].weight/2; 
						BDD[i+1][BDD_width_each_layer[i+1]-1].which_root_node = BDD[i][j].which_root_node; 
						BDD[i+1][BDD_width_each_layer[i+1]-1].which_bit_output = BDD[i][j].which_bit_output; 
						for(zi=0;zi<parameter_how_many_root_nodes;zi++){
							BDD[i+1][BDD_width_each_layer[i+1]-1].which_root_node_all[zi] = BDD[i][j].which_root_node_all[zi]; 
						} 
						for (zi=0;zi<i+1;zi++){
							BDD_mask_next[BDD_width_each_layer[i+1]-1].mask[most_influence[zi]]  = right_son_mask[most_influence[zi]]; 
						}
						BDD[i][j].right_node_index = 	BDD_width_each_layer[i+1]-1;
						BDD[i][j].right_node_neg    = 	0;
					}
				}
			}else{		
						BDD_width_each_layer[i+1] += 1;
						BDD[i+1][BDD_width_each_layer[i+1]-1].depth = i+1; 
						BDD[i+1][BDD_width_each_layer[i+1]-1].weight = BDD[i][j].weight/2; 
						BDD[i+1][BDD_width_each_layer[i+1]-1].which_root_node = BDD[i][j].which_root_node; 
						BDD[i+1][BDD_width_each_layer[i+1]-1].which_bit_output = BDD[i][j].which_bit_output; 
						for(zi=0;zi<parameter_how_many_root_nodes;zi++){
							BDD[i+1][BDD_width_each_layer[i+1]-1].which_root_node_all[zi] = BDD[i][j].which_root_node_all[zi]; 
						} 
						for (zi=0;zi<i+1;zi++){
							BDD_mask_next[BDD_width_each_layer[i+1]-1].mask[most_influence[zi]]  = right_son_mask[most_influence[zi]]; 
						}
						BDD[i][j].right_node_index = 	BDD_width_each_layer[i+1]-1;
						BDD[i][j].right_node_neg    = 	0;

			}
			if(BDD[i][j].left_node_index != BDD[i][j].right_node_index){
				total_split_nodes += 1;	
			}else{
				if((BDD[i][j].left_node_neg == 0) && (BDD[i][j].right_node_neg == 0) && (BDD[i][j].left_node_index >= 0)){
					if(BDD[i+1][BDD[i][j].left_node_index].has_equal_father == 0){
						BDD[i+1][BDD[i][j].left_node_index].has_equal_father = 1;
						BDD[i+1][BDD[i][j].left_node_index].non_equal_number = BDD[i][j].non_equal_number;
					}
				}
			}
		}
		int	root_nodes_leafs[parameter_how_many_root_nodes];
		int	leaf_nodes_roots[parameter_how_many_root_nodes];
		for(zi=0;zi<parameter_how_many_root_nodes;zi++){
			root_nodes_leafs[zi] = 0;
			for(int zj=0;zj<BDD_width_each_layer[i+1];zj++){
				root_nodes_leafs[zi] += int(BDD[i+1][zj].which_root_node_all[zi]);
			}
			cout<<root_nodes_leafs[zi]<<" ";
		}
		cout<<endl;	
		cout<<"Total nodes = "<<total_nodes_amount<<endl;
		cout<<"Total split nodes = "<<total_split_nodes;
		cout<<"		Finish trained weight =  "<<setprecision(12)<<(total_finish_weight)/pow(2.0,20)<<endl;
		gettimeofday(&finish_time,NULL);
		train_time = double(finish_time.tv_usec-start_time.tv_usec+1000000*(finish_time.tv_sec-start_time.tv_sec))/1000000;
		cout<<"Total train time = "<<train_time<<"s"<<endl;
		double all_train_time = double(finish_time.tv_usec-initial_start_time.tv_usec+1000000*(finish_time.tv_sec-initial_start_time.tv_sec))/1000000;
		cout<<"Total program time = "<<all_train_time<<"s"<<endl;
		cout<<"######################################################################"<<endl;
	}

	return 0;
};

int	BDD_class::BDD_infer(BDD_node** BDD,int* most_influence){
	long i,j;
	cout<<"test dataset size = "<<parameter_test_amount<<endl;
	error_amount_all = 0;
	for(int test_bit=0;test_bit<how_many_start_nodes;test_bit++){
		
		error_amount = 0;
		for (i=0;i<parameter_test_amount;i++){
			for (j=0;j<bit_width;j++){
				int zi = 0;
				long randint;
				zi = j%30;
				if(zi == 0){
					randint = rand();
				}
				test_input_data[j] = bool((int(randint >> (zi)))%2);
			}
			
			for(j=0;j<start_depth;j++){
				test_input_data[most_influence[j]] = BDD[start_depth][test_bit].mask[most_influence[j]];
			}
			test_output_data  	= io_generator_single(test_input_data,BDD[start_depth][test_bit].which_bit_output);
			infer_output_data 	= cal_infer_result(test_input_data,this,test_bit);
			if(test_output_data 	!= infer_output_data){
				error_amount = error_amount + 1;
			}
			if(((i%1000000)==0)&&(i>0)){
				cout<<i<<" "<<error_amount<<endl;
			}
		}
		train_error  = 0;
		circuit_accuracy_all_bits[test_bit] = 1-((error_amount)/double(parameter_test_amount));
		cout<<"Testing output bit No. "<<setw(4)<<test_bit<<"	Error amount = "<<error_amount<<"	Accuracy = "<<1-(error_amount/double(parameter_test_amount))<<endl;
	}
	circuit_accuracy = 0;
	for(i=0;i<parameter_how_many_root_nodes;i++){
		circuit_accuracy +=	circuit_accuracy_all_bits[i];
	}
	circuit_accuracy /= how_many_start_nodes;
	cout<<"Average Accuracy among all output bits = "<<circuit_accuracy<<endl;
	for (i=0;i<min(bit_width,100);i++){
		cout<<most_influence[i]<<" ";
	}
	cout<<endl;
	return 0;
};

//#include"new_print_circuit.h"
#include"print_circuit.h"
int	BDD_class::BDD_FULL_PROCESS(){

	cout<<"this is BDD temp = "<<this_is_BDD_temp<<endl;
	//BDD_class	BDD_temp;
	BDD_temp = new BDD_class [2];
	if(!this_is_BDD_temp){
		BDD_temp[0].this_is_BDD_temp	=	1;	
		BDD_temp[0].start_depth		=	start_depth;	
		BDD_temp[0].BDD_number		=	BDD_number;
		BDD_temp[0].how_many_start_nodes	=	how_many_start_nodes;
		BDD_temp[0].start_nodes = new BDD_node [BDD_temp[0].how_many_start_nodes];
		for(int zi=0;zi<BDD_temp[0].how_many_start_nodes;zi++){
			BDD_temp[0].start_nodes[zi].which_bit_output	=	start_nodes[zi].which_bit_output;
			BDD_temp[0].start_nodes[zi].which_bit_output	=	start_nodes[zi].which_root_node ;
			for (int zk=0;zk<bit_width;zk++){
				BDD_temp[0].start_nodes[zi].mask[zk]	= start_nodes[zi].mask[zk];
				BDD_temp[0].BDD_mask_this[zi].mask[zk] 	= BDD_mask_this[zi].mask[zk];
			}
		}
		for(int zi=0;zi<bit_width+1;zi++){
			BDD_temp[0].has_been_unfold[zi] 		= has_been_unfold[zi] 	 ;
			BDD_temp[0].most_influence[zi] 		= most_influence[zi] 	 ;
			BDD_temp[0].BDD_width_each_layer[zi] 	= BDD_width_each_layer[zi];
		}
		cout<<"go for BDD temp"<<endl;
		if(BDD_temp[0].BDD_FULL_PROCESS() == 0){
			cout<<"no need to partition: "<<partition_depth<<endl;
			memcpy(this,&BDD_temp[0],sizeof(BDD_class));
			this_is_BDD_temp = 0;
			BDD_infer(BDD,most_influence);
			cout<<"no need to partition: "<<partition_depth<<endl;
			return	0;	
		}else{
			cout<<"need to partition: "<<partition_depth<<endl;
			partition_depth = start_depth+1;	
			partition_into_how_many_parts = 2;
			partition_index 			= new int* [BDD_class_main.partition_into_how_many_parts];
			partition_start_node_numbers 	= new int  [BDD_class_main.partition_into_how_many_parts];
			//////for(int zi=0;zi<BDD_class_main.partition_into_how_many_parts;zi++){
			//////	BDD_class_main.partition_start_node_numbers[zi] = 32;
			//////	BDD_class_main.partition_index[zi] = new int [BDD_class_main.partition_start_node_numbers[zi]];
			//////	for(int zj=0;zj<32;zj++){
			//////		BDD_class_main.partition_index[zi][zj] = zj*2+zi;
			//////	}
			//////}

		}
	}
	gettimeofday(&start_time,NULL);

	for (i=0;i<bit_width+1;i++){
		amount_turn_input_data[i]  = new bool* [parameter_sample_amount_influence_max];
		amount_turn_output_data[i] = new bool  [parameter_sample_amount_influence_max];
		for(j=0;j<parameter_sample_amount_influence_max;j++){
			amount_turn_input_data[i][j] = new bool [bit_width];
		}
		//has_been_unfold[i] = 0;
	}

	gettimeofday(&finish_time,NULL);
	train_time = double(finish_time.tv_usec-start_time.tv_usec+1000000*(finish_time.tv_sec-start_time.tv_sec))/1000000;


	srand((unsigned)time(0));
	cout<<"ready to train"<<endl;
	
	parameter_train_dataset_lines = 0;
	parameter_train_dataset_lines_init = 0;
	for (j=0;j<max_parameter_sample_amount;j++){
		random_input_data[j] 	= new bool[bit_width];
		mask_input_data[j] 	= new bool[bit_width];
	}
	for (int zi=0;zi<parameter_max_BDD_width;zi++){
		if(zi<1000){
			simplify_list[zi] = new bool[max_parameter_sample_amount];
		}else{
			simplify_list[zi] = new bool[int(max_parameter_sample_amount/10)];
		}
	}
	
	train_BDD(BDD,most_influence,start_depth,how_many_start_nodes,start_nodes);
	BDD_infer(BDD,most_influence);
	if(train_error==0){
		if(this_is_BDD_temp){
			if(circuit_accuracy > 0.98){
				//save as verilog
				print_circuit(BDD,most_influence);
				return 0;
			}
		}else{
			if(circuit_accuracy > 0.98){
				//save as verilog
				print_circuit(BDD,most_influence);
				return 0;
			}
		}
	}
	return -1;
}

int	main(int argc,char* argv[]){

	gettimeofday(&initial_start_time,NULL);
	int i,j;
	if(argc >= 2){
		which_function_output = atoi(argv[1]);
	}
	if(argc>=3){
		which_demo_function = atoi(argv[2]);
	}else{
		which_demo_function = 2;
	}
	//which_bit_output_general_reg_index = cvt_number_to_bit_unsigned(int(which_bit_output/32),5);
	DIR*	output_dir;
	output_dir	= opendir("../../result/demo_functions/rtl/");
	dirent	*ptr;
	vector<string>	files;	
	while((ptr=readdir(output_dir))!=NULL){
		files.push_back(ptr->d_name);
	}
	bool	this_bit_finished = 0;
	for (i=0;i<files.size();i++){
		if(files[i].substr(0,8) == "function"){
			int	this_output_bit = 0;
			for(j=0;j<10;j++){
				//cout<<files[i].substr(18+j,1);
				if(files[i].substr(18+j+1,1) == "."){
					break;
				}
			}
			//cout<<atoi(files[i].substr(9,9+j).c_str());
			if(atoi(files[i].substr(9,9+j).c_str()) == which_function_output){
				this_bit_finished = 1;
				cout<<"this bit finished:	"<<which_function_output<<endl;
				return 0;
				break;
			}
		}
	}
	closedir(output_dir);
	BDD_class_main.start_depth 	= 0;	
	BDD_class_main.BDD_number 	= 0;
	BDD_number_counter		+=1;	
	BDD_class_main.how_many_start_nodes = parameter_how_many_root_nodes;
	BDD_class_main.start_nodes = new BDD_node [BDD_class_main.how_many_start_nodes];
	for(int zi=0;zi<BDD_class_main.how_many_start_nodes;zi++){
		BDD_class_main.start_nodes[zi].which_bit_output 	= zi;
		BDD_class_main.start_nodes[zi].which_root_node  	= zi;
		BDD_class_main.start_nodes[zi].which_root_node_all[BDD_class_main.start_nodes[zi].which_root_node] = 1;
	}
	for(int zi=0;zi<bit_width+1;zi++){
		BDD_class_main.has_been_unfold[zi] = 0;
		BDD_class_main.most_influence[zi] = 0;
		BDD_class_main.BDD_width_each_layer[zi] = 0;
	}
		
	BDD_class_main.BDD_FULL_PROCESS();

};


