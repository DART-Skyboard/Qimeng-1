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
extern const int	bit_width 			= 1798+parameter_multi_output_index+96;
int			parameter_sample_amount 	= 200;
extern const int	max_parameter_sample_amount 	= 2000000;
extern const int	max_parameter_sample_amount_all = 1000000000;
extern const int	parameter_how_often_simplify 	= 1;
long			parameter_test_amount 		= 1000000;//10000000000;
extern const int	parameter_sample_amount_influence_max = 1;
int			parameter_early_stop_depth_0	= 10000;
extern const int	parameter_early_stop_depth_1	= 10000000;
extern const int	parameter_depth_start_simplify	= parameter_multi_output_index-1;
extern const double	total_weight_max		= pow(2.0,20);
extern const int	parameter_train_dataset_lines_all	= 1*pow(2,parameter_multi_output_index)/1;
extern const int	parameter_train_dataset_lines_temp	= 100000*pow(2,parameter_multi_output_index);
int			parameter_train_dataset_lines	= parameter_train_dataset_lines_all;
int			parameter_train_dataset_lines_init	= parameter_train_dataset_lines_all;
extern const int	parameter_max_BDD_width		= 1000000;

int	which_bit_output = 127;

bool*	which_bit_output_general_reg_index;
#include"cvt.h"
#include"compare_bool_list.h"
#include"io_generator_RISCV.h"
int*		most_influence_0	= new int[bit_width];
int*		most_influence_1	= new int[bit_width];
int*		BDD_width_each_layer 	= new int [bit_width+1];
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
	int	left_node_index;
	int	right_node_index;
	int	depth;
	double	weight;
	//bool	mask[bit_width];
	int	sort_in_layer;
	bool	left_node_neg;
	bool	right_node_neg;
	int	non_equal_number;
	bool	has_equal_father;
	//BDD_node(){
	//	int left_node_index 	= 0;
	//	int right_node_index 	= 0;
	//	int depth		= 0;
	//	double weight	= 1;
	//	bool mask[bit_width]	= {0};
	//	int	sort_in_layer	= 0;
	//	bool	left_node_neg	= 0;
	//	bool	right_node_neg	= 0;
	//	bool	has_equal_father = 0;
	//	int	non_equal_number = 0;
	//};
};
class	BDD_node_mask{
public:	
	bool	mask[bit_width];
};
int	which_BDD = 0;
BDD_node** BDD_0 		 = new BDD_node*[1+bit_width];
BDD_node** BDD_1 		 = new BDD_node*[1+bit_width];
BDD_node_mask*	BDD_mask_this= new BDD_node_mask[parameter_max_BDD_width];
BDD_node_mask* 	BDD_mask_next= new BDD_node_mask[parameter_max_BDD_width];
//BDD_node_mask** BDD_mask = new BDD_node_mask*[1+bit_width];
///////class	simplify_node{
///////public:
///////	bool	bools_result[max_parameter_sample_amount];
///////	int	BDD_node_index;
///////	simplify_node(){
///////		int BDD_node_index = 0;
///////	}	
///////};
//simplify_node*	simplify_list_heap;

bool**	train_dataset 			= new bool* [parameter_train_dataset_lines_all];
bool*	train_dataset_output		= new bool  [parameter_train_dataset_lines_all];
int*	train_dataset_which_node 	= new int   [parameter_train_dataset_lines_all];	
bool**	train_dataset_temp		= new bool* [parameter_train_dataset_lines_temp];
bool*	train_dataset_output_temp	= new bool  [parameter_train_dataset_lines_temp];
int*	train_dataset_which_node_temp 	= new int   [parameter_train_dataset_lines_temp];	
double*	train_dataset_bit_influence	= new double[bit_width];
int	train_dataset_valid_line_amount = parameter_train_dataset_lines;
int*	train_dataset_valid_line_index	= new int [train_dataset_valid_line_amount];

bool**	wrong_dataset 		= new bool*[parameter_train_dataset_lines_temp];
bool*	wrong_dataset_output 	= new bool [parameter_train_dataset_lines_temp];

bool*	truth_table; 

extern const	int	cal_data_width = 32;
	
long	io_read_times = 0;
bool	cal_infer_result(bool input_data[bit_width],BDD_node** BDD,int* most_influence){
	bool	infer_result = 0;
	int i;
	int position = 0;
	bool neg = 0;
	for (i=0;i<bit_width+1;i++){
		//cout<<most_influence[i]<<" ";
		//cout<<position<<" ";	
		if(!input_data[most_influence[i]]){
			if(BDD[i][position].left_node_index == -1){
				if(neg){
					infer_result = 0;
				}else{
					infer_result = 1;
				}
				break;
			}else if(BDD[i][position].left_node_index == -2){
				if(neg){
					infer_result = 1;
				}else{
					infer_result = 0;
				}
				break;
			}else{
				if(BDD[i][position].left_node_neg){
					neg = !neg;
				}
				position = BDD[i][position].left_node_index;
			}
		}else{
			if(BDD[i][position].right_node_index == -1){
				if(neg){
					infer_result = 0;
				}else{
					infer_result = 1;
				}
				break;
			}else if(BDD[i][position].right_node_index == -2){
				if(neg){
					infer_result = 1;
				}else{
					infer_result = 0;
				}
				break;
			}else{
				if(BDD[i][position].right_node_neg){
					neg = !neg;
				}
				position = BDD[i][position].right_node_index;
			}
		}
	}
	//cout<<endl;
	return	infer_result;
};
bool	io_generator_single_2(bool input_data[bit_width],int which_bit_output){

	bool	output_bit;
	output_bit = io_generator_RISCV(input_data,which_bit_output);
	return	output_bit;
}
bool	io_generator_single(bool input_data[bit_width],int which_bit_output){
	
	bool	output_bit;

////	int i,j;
////	long	data_a,data_b;
////	bool	input_data_a[cal_data_width],input_data_b[cal_data_width];
////	for (i=0;i<cal_data_width;i++){
////		input_data_a[i] = input_data[i];
////		input_data_b[i] = input_data[i+cal_data_width];
////	}
////	data_a = cvt_bit_to_number_unsigned(input_data_a,cal_data_width);
////	data_b = cvt_bit_to_number_unsigned(input_data_b,cal_data_width);
////
////	long	output_data = data_a + data_b;
////	//long output_data = cvt_bit_to_number(input_data,24);	
////	//output_bit  = truth_table[output_data];
////	output_bit = (output_data >> cal_data_width) & (1);
////	//cout<<data_a<<' '<<data_b<<' '<<output_data<<endl;

	output_bit = io_generator_RISCV(input_data,which_bit_output);
//	output_bit = ALU_pc(input_data);
//	output_bit = ALU_general_regs(input_data);
//	if(which_BDD==0){
//		output_bit = io_generator_RISCV_multi_output(input_data);
//	}else{
//		bool output_bit_a = io_generator_RISCV_multi_output(input_data);
//		bool output_bit_b = cal_infer_result(input_data,BDD_0,most_influence_0);
//		output_bit = (output_bit_b^output_bit_a);
//	}
	io_read_times += 1;
	return	output_bit;
};
bool*	has_been_unfold = new bool[bit_width+1];
bool	which_op_code_data[12][7] 	= {
						0,1,1,0,1,1,1,
						0,0,1,0,1,1,1,
						1,1,0,1,1,1,1,
						1,1,0,0,1,1,1,
						1,1,0,0,0,1,1,
						0,0,0,0,0,1,1,
						0,1,0,0,0,1,1,
						0,0,1,0,0,1,1,
						0,1,1,0,0,1,1,
						0,0,0,1,1,1,1,
						1,1,1,0,0,1,1,
						0,1,0,1,1,1,1

					};
bool	which_csr_data[23][12]	=
				{
					0,0,1,1,	0,0,0,0,	0,1,0,1,	//305			
        				0,0,1,1,	0,1,0,0,	0,0,0,1,	//341
        				0,0,1,1,	0,1,0,0,	0,0,1,0,	//342
        				0,0,1,1,	0,0,0,0,	0,1,0,0,	//304
        				0,0,1,1,	0,1,0,0,	0,1,0,0,	//344
        				0,0,1,1,	0,1,0,0,	0,0,1,1,	//343
        				0,0,1,1,	0,1,0,0,	0,0,0,0,	//340
        				0,0,1,1,	0,0,0,0,	0,0,0,0,	//300
        				0,0,1,1,	0,0,0,0,	0,0,1,1,	//303
        				0,0,1,1,	0,0,0,0,	0,0,1,0,	//302
        				0,0,0,1,	0,1,0,0,	0,0,0,1,	//141
        				0,0,0,1,	0,0,0,0,	0,1,0,1,	//105
        				0,0,0,1,	0,1,0,0,	0,0,1,0,	//142
        				0,0,0,1,	0,1,0,0,	0,0,0,0,	//140
        				0,0,0,1,	0,1,0,0,	0,0,1,1,	//143
        				0,0,0,1,	0,0,0,0,	0,0,0,0,	//100		
        				0,0,0,1,	0,0,0,0,	0,1,0,0,	//104
        				0,0,0,1,	0,1,0,0,	0,1,0,0,	//144
        				0,0,0,1,	1,0,0,0,	0,0,0,0,	//180
        				1,1,1,1,	0,0,0,1,	0,1,0,0,	//f14
        				0,0,1,1,	0,0,0,0,	0,0,0,1,	//301
        				1,1,0,0,	0,0,0,0,	0,0,0,1,	//c01
        				1,1,0,0,	1,0,0,0,	0,0,0,1		//c81
				};
int	set_random_input_data(){
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
			//cout<<random_input_data[i][j];
		}
		bool	rs_a_code[5];
		bool	rs_b_code[5];
		for (j=0;j<5;j++){
			if(i%5==0){
				mask_input_data[i][1696+12+j] = mask_input_data[i][1696+7+j];
			}
			rs_a_code[j] = mask_input_data[i][12+1696+j];
			rs_b_code[j] = mask_input_data[i][ 7+1696+j];
		}
		int	reg_a_index = cvt_bit_to_number_unsigned(rs_a_code,5);
		int 	reg_b_index = cvt_bit_to_number_unsigned(rs_b_code,5);
		for(j=0;j<32;j++){
			if((i%3)==0){
				if(j<16){
					mask_input_data[i][32*reg_a_index+j] = mask_input_data[i][32*reg_b_index+j] ;
				}
			}
			mask_input_data[i][1798+j] = mask_input_data[i][32*reg_a_index+j];
			mask_input_data[i][1830+j] = mask_input_data[i][32*reg_b_index+j];
		}	
			
		if((i%2)!=0){
			
			if(which_bit_output<1024){
				for(j=0;j<5;j++)
					mask_input_data[i][20+1696+j] = which_bit_output_general_reg_index[j];
			}
			if((which_bit_output<1024)||(which_bit_output>(32*53-1))){
				int	which_op_code = rand()%2;
				if(which_op_code==1){
					for(j=0;j<7;j++){
						mask_input_data[i][j+1721] = which_op_code_data[4][j];
					}
				}
				
			}
			else{
				int	which_op_code 	= rand()%12;
				int	which_op_code_a = rand()%2;
				if(which_op_code_a==1){
					for(j=0;j<7;j++){
						mask_input_data[i][j+1721] = which_op_code_data[which_op_code][j];
					}
				}else{
					for(j=0;j<7;j++){
						mask_input_data[i][j+1721] = which_op_code_data[10][j];
					}
				}
				
			}
			if(i%8==1){
				for(j=2;j<20;j++){
					mask_input_data[i][j+1728] = 0;
				}
			}
			if(i%8==5){
				for(j=2;j<20;j++){
					mask_input_data[i][j+1728] = 1;
				}
			}
			if(i%3==1){
				for(j=2;j<7;j++){
					mask_input_data[i][j+1696] = 0;
				}
			}
		}
					
		//cout<<endl;
	}
	return 0;
};
int	mask_random_input_data(int depth,bool mask[bit_width],int amount,int* most_influence){
	int i,j;
	int which_node_this_layer = 0;
	for (i=0;i<amount;i++){
		for (j=0;j<depth+1;j++){
			mask_input_data[i][most_influence[j]] = mask[most_influence[j]];
		}
		//cout<<endl;
		//mask_output_data[i] = io_generator_single(mask_input_data[i]);
	}

	return 0;
};
bool***	amount_turn_input_data	= new bool**[bit_width+1];
bool**	amount_turn_output_data	= new bool* [bit_width+1];
double*	amount_turn = new double [bit_width];

int	cal_most_influence_next(int depth,BDD_node** BDD,int* most_influence){
	if(which_bit_output<32){
		return 0;
	}
		
	int	most_influence_next=0;
	int 	i,j,k;
	int	zz;
	int	zi;
	bool	should_not_be_unfold[bit_width] = {0};
	for (zz=0;zz<bit_width;zz++){
		amount_turn[zz] = 0;
	}
		for(i=0;i<32;i++){
			amount_turn[i+1728] = 0.2;
			amount_turn[i+1696] = 0.1;
			amount_turn[i+1798] = (20+32-i)/10.0;
			amount_turn[i+1830] = (20+32-i)/10.0;
		}
	long	parameter_sample_mul = 10;
			for (j=0;j<20;j++){
				parameter_sample_mul *= 10;
				if((parameter_sample_mul * (1-total_finish_weight/pow(2.0,20))) > 10000){
					break;
				}
			}
	if(depth>1+pow(2,parameter_multi_output_index)){
		for(j=0;j<1024;j++){
			should_not_be_unfold[j] = 1;
		}	
	}
	int	parameter_sample_amount_influence = parameter_sample_amount_influence_max;
	for (zz=0;zz<10000;zz++){
		if(depth < parameter_multi_output_index){
			break;
		}
		
		int	which_node_this_layer = 0;
		int	which_node_this_layer_sample = 0;
		for (i=0;i<parameter_sample_amount_influence;i++){
			
			if(zz%2 == 0){	
				which_node_this_layer_sample = rand()%BDD_width_each_layer[depth];
			}else{
				which_node_this_layer_sample = rand()%max(int(parameter_sample_mul*(1-total_finish_weight/pow(2.0,20))),1);//BDD_width_each_layer[depth];
			}
			double sum_sample = 0;
			for (j=0;j<BDD_width_each_layer[depth];j++){
				sum_sample += BDD[depth][j].weight/pow(2.0,20);
				if(parameter_sample_mul*(sum_sample) > which_node_this_layer_sample){
					which_node_this_layer = j;
					break;
				}
			}
			
			//for(j=0;j<depth;j++){
			//	cout<<BDD[depth][which_node_this_layer].mask[most_influence[i]];
			//}
			//cout<<"	"<<which_node_this_layer<<endl;
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
				//cout<<random_input_data[i][j];
			}	
			bool	rs_a_code[5];
			bool	rs_b_code[5];
			for (j=0;j<5;j++){
				if(i%5==0){
					mask_input_data[i][1696+12+j] = mask_input_data[i][1696+7+j];
				}
				rs_a_code[j] = mask_input_data[i][12+1696+j];
				rs_b_code[j] = mask_input_data[i][ 7+1696+j];
			}
			int	reg_a_index = cvt_bit_to_number_unsigned(rs_a_code,5);
			int 	reg_b_index = cvt_bit_to_number_unsigned(rs_b_code,5);
			for(j=0;j<32;j++){
				if((i%3)==0){
					if(j<16){
						mask_input_data[i][32*reg_a_index+j] = mask_input_data[i][32*reg_b_index+j] ;
					}
				}
				mask_input_data[i][1798+j] = mask_input_data[i][32*reg_a_index+j];
				mask_input_data[i][1830+j] = mask_input_data[i][32*reg_b_index+j];
			}
			if((i%2==0) && (which_bit_output >= 1024) && (which_bit_output<32*53)){	
				int	reg_a_index = cvt_bit_to_number_unsigned(rs_a_code,5);
				int 	reg_b_index = cvt_bit_to_number_unsigned(rs_b_code,5);
				for(j=0;j<32;j++){
					mask_input_data[i][1798+j] = mask_input_data[i][32*reg_a_index+j];
					mask_input_data[i][1830+j] = mask_input_data[i][32*reg_b_index+j];
				}
				int	which_op_code	= rand()%12;
				int	which_op_code_a = rand()%2;
				if(which_op_code_a==1){
					for(j=0;j<7;j++){
						mask_input_data[i][j+1721] = which_op_code_data[which_op_code][j];
					}
				}else{
					for(j=0;j<7;j++){
						mask_input_data[i][j+1721] = which_op_code_data[10][j];
					}
				}
			}
			if((i%2==0) && ((which_bit_output < 1024) ||(which_bit_output>1695))){	
				if(which_bit_output<1024){
					for(j=0;j<5;j++)
						mask_input_data[i][20+1696+j] = which_bit_output_general_reg_index[j];
				}
				int	which_op_code	= rand()%2;
				if(which_op_code){
					for(j=0;j<7;j++){
						mask_input_data[i][j+1721] = which_op_code_data[4][j];
					}
				}
				if(i%8==0){
					for(j=2;j<20;j++){
						mask_input_data[i][j+1728] = 1;
					}
				}
				if(i%8==4){
					for(j=2;j<20;j++){
						mask_input_data[i][j+1728] = 0;
					}
				}
				if(i%3==1){
					for(j=2;j<7;j++){
						mask_input_data[i][j+1696] = 0;
					}
				}
			}
			//cout<<endl;
			//mask_output_data[i] = io_generator_single(mask_input_data[i]);
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
						//cout<<amount_turn_input_data[i][j][k];
					}
					amount_turn_output_data[i][j] = io_generator_single(amount_turn_input_data[i][j],which_bit_output);
					//cout<<i<<" "<<j<<" "<<amount_turn_output_data[i][j]<<endl;
				}
			}
		}
		//cout<<endl;
		////////////if(zz == 2001){
		////////////	for(i=0;i<bit_width;i++){
		////////////		if(has_been_unfold[i]){
		////////////			amount_turn[i] = -1;	
		////////////		}
		////////////		else{
		////////////			for(j=0;j<min(100,train_dataset_valid_line_amount);j++){
		////////////				if(train_dataset_which_node >= 0){
		////////////					int zi = rand()%train_dataset_valid_line_amount;
		////////////					bool* bit_switch_input  = new bool[bit_width];
		////////////					for(int zj=0;zj<bit_width;zj++){
		////////////						bit_switch_input[zj] = train_dataset_temp[zi][zj];
		////////////						if(zj==i){
		////////////							bit_switch_input[zj] = !train_dataset_temp[zi][zj];
		////////////						}
		////////////					}
		////////////					bool  bit_switch_output = io_generator_single(bit_switch_input,which_bit_output);
		////////////					if(bit_switch_output != train_dataset_output_temp[zi]){
		////////////						amount_turn[i] += 1;
		////////////					} 
		////////////					delete[] bit_switch_input;								
		////////////				}	
		////////////			}
		////////////			//cout<<i<<endl;
		////////////		}
		////////////	}
		////////////}
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
					//if((rand()%2) == 1){
					//	amount_turn[i] += 1;
					//}
					amount_turn[i] += train_dataset_bit_influence[i];
				}
					if((i<1862) && (i>1829) && (most_influence[depth-1]==(i-32))){
							amount_turn[i] += 100;
					}
					if((i<1830) && (i>1797) && (most_influence[depth-1]==(i+32))){
							amount_turn[i] += 100;
					}
				
				if((amount_turn[i] > 50)&&(amount_turn[i]<(zz*parameter_sample_amount_influence/2))){
					finish_influence_sample = 1;
				}
				if(((zz*parameter_sample_amount_influence-amount_turn[i]) > 50)&&(amount_turn[i]>(zz*parameter_sample_amount_influence/2))){
					finish_influence_sample = 1;
				}
				///if((amount_turn[i] > 20)){
				///	finish_influence_sample = 1;
				///}
				if((amount_turn[i] > 5) && (zz>100)){
					finish_influence_sample = 1;
				}
				if((amount_turn[i] > 0) && (zz>1000)){
					finish_influence_sample = 1;
				}
				//if((parameter_sample_amount < 5) && (zz > 100)){
				//	finish_influence_sample = 1;
				//}	
				
			}
		}
		//////if( zz==500000){
		//////	for(i=0;i<10000;i++){
		//////		int random_bit = rand()%bit_width;
		//////		if(has_been_unfold[random_bit] == 0){
		//////			amount_turn[random_bit] += 1;
		//////			break;
		//////		}
		//////	}
		//////}
		
		if(finish_influence_sample){
			break;
		}
	//cout<<endl;
	}
	if(zz>10000-1){
		parameter_early_stop_depth_0 = depth+1;
	}
	for (i=0;i<bit_width;i++){
		//if(amount_turn[i]>(zz*parameter_sample_amount_influence/2)){
		//	amount_turn[i] = zz*parameter_sample_amount_influence - amount_turn[i];
		//}
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
	if(depth < parameter_multi_output_index){
		most_influence_next = 1862+depth;
	}
	//if(most_influence_next>1797)
//		cin>>most_influence_next;	
	has_been_unfold[most_influence_next] = 1;

	
	//delete[]amount_turn;
	return	most_influence_next;
};


int*	simplify_list_sort= new int[parameter_max_BDD_width];

//////////////////int	maxheap_bools(simplify_node* simplify_nodes,int i,int limit){
//////////////////	int largest = i;
//////////////////	if((2*i<=limit) && (compare_bool_list(simplify_nodes[simplify_list_sort[i]].bools_result,simplify_nodes[simplify_list_sort[2*i+1]].bools_result,parameter_sample_amount) == 2)){	
//////////////////		largest = 2 * i + 1;
//////////////////	}
//////////////////	if((2*i<=limit) && (compare_bool_list(simplify_nodes[simplify_list_sort[i]].bools_result,simplify_nodes[simplify_list_sort[2*i+2]].bools_result,parameter_sample_amount) == 2)){	
//////////////////		largest = 2 * i + 2;
//////////////////	}
//////////////////	if(i != largest)
//////////////////	{
//////////////////		//simplify_node temp;
//////////////////		//temp 	= simplify_nodes[i];
//////////////////		//simplify_nodes[i]	= simplify_nodes[largest];
//////////////////		//simplify_nodes[largest] = temp;
//////////////////		int	temp;
//////////////////		temp				=	simplify_list_sort[i];
//////////////////		simplify_list_sort[i]		= 	simplify_list_sort[largest];
//////////////////		simplify_list_sort[largest]	= 	temp;
//////////////////		maxheap_bools(simplify_nodes,largest,limit);
//////////////////	}
//////////////////	return 0;
//////////////////}
//////////////////int	createmaxheap_bools(simplify_node* simplify_nodes,int size){
//////////////////	int i;
//////////////////	for(i=size/2-1;i>-1;i--){
//////////////////		maxheap_bools(simplify_nodes,i,size);
//////////////////	}
//////////////////	return 0;
//////////////////};
//////////////////int	heapsort_bools(simplify_node* simplify_nodes,int size){
//////////////////	createmaxheap_bools(simplify_nodes,size);
//////////////////	for(int i=size-1;i>-1;i--){
//////////////////		int temp;
//////////////////		temp 			= simplify_list_sort[0];
//////////////////		simplify_list_sort[0]	= simplify_list_sort[i];
//////////////////		simplify_list_sort[i]	= temp;	
//////////////////		maxheap_bools(simplify_nodes,1,i-1);
//////////////////	}
//////////////////	return 0;	
//////////////////};
int compare_simplify_list(int list_line_amount,bool* this_line,int this_line_sum,int this_line_sum_half,int this_line_first_zero,int this_line_first_one){
	int which_list_number = -1;
	bool it_can_simplify  = 0;
	int i,j;
	bool it_can_simplify_here;

	which_list_number = -1;
	//heapsort_bools(simplify_list_heap,list_line_amount);
	//if((compare_bool_list(this_line,simplify_list_heap[simplify_list_sort[0]].bools_result,parameter_sample_amount) == 0)){
	//	which_list_number = simplify_list_heap[simplify_list_sort[0]].BDD_node_index;
	//}else if(list_line_amount == 0){
	//	
	//}
	//else if((compare_bool_list(this_line,simplify_list_heap[simplify_list_sort[list_line_amount-1]].bools_result,parameter_sample_amount) == 0)){
	//	which_list_number = simplify_list_heap[simplify_list_sort[list_line_amount-1]].BDD_node_index;
	//}else{
	//	int	start_interval 	= 0;
	//	int	end_interval	= list_line_amount-1;
	//	int	half_interval	= 0;
	//	int	half_interval_result	= 0;
	//	for (i=0;i<list_line_amount;i++){
	//		half_interval	= int((end_interval + start_interval) * 0.5);
	//		half_interval_result = compare_bool_list(this_line,simplify_list_heap[simplify_list_sort[half_interval]].bools_result,parameter_sample_amount);
	//		if( half_interval_result == 0){
	//			which_list_number = simplify_list_heap[simplify_list_sort[half_interval]].BDD_node_index;
	//			break;
	//		}else if( half_interval_result == 1){
	//			if(half_interval == (start_interval+1)){
	//				break;
	//			}
	//			end_interval	= half_interval;
	//		}else if( half_interval_result == 2){
	//			if(half_interval == (end_interval-1)){
	//				break;
	//			}
	//			start_interval	= half_interval;
	//		}
	//	}
	//}
	//cout<<which_list_number;
	//which_list_number = -1;
	//it_can_simplify  = 0;
	//make_heap(simplify_list,simplify_list+list_line_amount);
	//sort_heap(simplify_list_sum,simplify_list_sum+list_line_amount);
	
	for (i=list_line_amount-1;i>=0;i--){
		it_can_simplify_here = 1;
		//cout<<simplify_list_sum[i]<<" ";
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
	//cout<<" "<<endl;
	return which_list_number;
};



int compare_simplify_list_neg(int list_line_amount,bool* this_line,int this_line_sum,int this_line_sum_half,int this_line_first_zero,int this_line_first_one){
	int which_list_number = -1;
	bool it_can_simplify  = 0;
	int i,j;
	bool it_can_simplify_here = 1;

	//heapsort_bools(simplify_list_heap,list_line_amount);
	//if((compare_bool_list(this_line,simplify_list_heap[simplify_list_sort[0]].bools_result,parameter_sample_amount) == 0)){
	//	which_list_number = simplify_list_heap[simplify_list_sort[0]].BDD_node_index;
	//}else if(list_line_amount == 0){
	//	
	//}
	//else if((compare_bool_list(this_line,simplify_list_heap[simplify_list_sort[list_line_amount-1]].bools_result,parameter_sample_amount) == 0)){
	//	which_list_number = simplify_list_heap[simplify_list_sort[list_line_amount-1]].BDD_node_index;
	//}else{
	//	int	start_interval 	= 0;
	//	int	end_interval	= list_line_amount-1;
	//	int	half_interval	= 0;
	//	int	half_interval_result	= 0;
	//	for (i=0;i<list_line_amount;i++){
	//		half_interval	= int((end_interval + start_interval) * 0.5);
	//		half_interval_result = compare_bool_list(this_line,simplify_list_heap[simplify_list_sort[half_interval]].bools_result,parameter_sample_amount);
	//		if( half_interval_result == 0){
	//			which_list_number = simplify_list_heap[simplify_list_sort[half_interval]].BDD_node_index;
	//			break;
	//		}else if( half_interval_result == 1){
	//			if(half_interval == (start_interval+1)){
	//				break;
	//			}
	//			end_interval	= half_interval;
	//		}else if( half_interval_result == 2){
	//			if(half_interval == (end_interval-1)){
	//				break;
	//			}
	//			start_interval	= half_interval;
	//		}
	//	}
	//}
	//cout<<which_list_number;
	//which_list_number = -1;
	//it_can_simplify  = 0;
	//make_heap(simplify_list,simplify_list+list_line_amount);
	//sort_heap(simplify_list_sum,simplify_list_sum+list_line_amount);
	
	for (i=list_line_amount-1;i>=0;i--){
		it_can_simplify_here = 1;
		//cout<<simplify_list_sum[i]<<" ";
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
	//cout<<" "<<endl;
	//which_list_number = -1;
	return which_list_number;
};

long	total_nodes_amount = 0;
long	total_split_nodes  = 0;
int	total_BDD_depth	   = 0;
double	train_time	   = 0;
	
struct timeval	start_time,finish_time;

int train_BDD(int which_bit_output,BDD_node** BDD,int* most_influence){

	int i,j;
	int zi;
	//printf("%d\n",BDD[0][0].left_node_index);	
	//printf("%d\n",BDD[0][0].right_node_index);	
	//printf("%d\n",BDD[0][0].depth);	
	//printf("%d\n",BDD[0][0].mask[1]);	
	BDD[0] 		= new BDD_node[1];
	for (i=0;i<bit_width;i++){
		BDD_mask_this[0].mask[i] = 0;
		BDD_mask_next[0].mask[i] = 0;
	}
	BDD[0][0].has_equal_father =0;
	BDD[0][0].non_equal_number =0;
	BDD[0][0].weight = pow(2.0,20);
	
	total_finish_weight = 0;
	train_dataset_valid_line_amount = parameter_train_dataset_lines;
	for(i=0;i<bit_width+1;i++){
		BDD_width_each_layer[i] = 0;
		has_been_unfold[i] 	= 0;
	}	
	BDD_width_each_layer[0] = 1;
	total_nodes_amount = 0;
	total_split_nodes  = 0;
	total_BDD_depth	   = 0;
	train_time	   = 0;

	bool*	left_son_mask		= new bool [bit_width];
	bool*	right_son_mask		= new bool [bit_width];
	bool* 	left_mask_output_data	= new bool [max_parameter_sample_amount];
	bool* 	right_mask_output_data	= new bool [max_parameter_sample_amount];
	
	for(i=0;i</*parameter_early_stop_depth*/bit_width;i++){
		for(j=0;j<BDD_width_each_layer[i];j++){
			for (zi=0;zi<bit_width;zi++){
				BDD_mask_this[j].mask[zi] = BDD_mask_next[j].mask[zi];
			}	
		}
		if(total_finish_weight/pow(2.0,20) > 0.9999999999){
			parameter_sample_amount = 10;
			if(total_finish_weight/pow(2.0,20) > 1){
				parameter_sample_amount = 2;
			}
		}else{
			parameter_sample_amount = max(4,int(max_parameter_sample_amount_all * (1-total_finish_weight/pow(2.0,20))));
		}
		if(parameter_sample_amount < max_parameter_sample_amount){
			parameter_sample_amount *= 100;
		}
		if(parameter_sample_amount < max_parameter_sample_amount){
			parameter_sample_amount *= 100;
		}
		if	(which_BDD < 3000){
			parameter_sample_amount = min(max_parameter_sample_amount/1,max(int(parameter_sample_amount/1),100));
		}else if(which_BDD < 6){
			parameter_sample_amount = min(max_parameter_sample_amount/1,max(int(parameter_sample_amount/1),100));
		}else if(which_BDD < 9){
			parameter_sample_amount = min(max_parameter_sample_amount/1,max(int(parameter_sample_amount/1),100));
		}else if(which_BDD < 12){
			parameter_sample_amount = min(max_parameter_sample_amount/1,max(int(parameter_sample_amount/1),100));
		}
	////	if((50<BDD_width_each_layer[i]) || (i>50)){
	////		parameter_sample_amount /= 5;
	////	}
	//	if(total_nodes_amount > 8000000){
	//		parameter_sample_amount = min(10000,max(int(max_parameter_sample_amount/10 * (1-total_finish_weight/pow(2.0,20))),1));
	//	}
	//	if(BDD_width_each_layer[i] > 50000){
	//		parameter_sample_amount = min(10000,max(int(max_parameter_sample_amount/60 * (1-total_finish_weight/pow(2.0,20))),1));
	//	}
	//	if(BDD_width_each_layer[i] > 10000){
	//		parameter_sample_amount = min(10000,max(int(max_parameter_sample_amount/20 * (1-total_finish_weight/pow(2.0,20))),1));
	//	}
//		if(BDD_width_each_layer[i] > 20000){
//			parameter_sample_amount = min(10000,max(int(max_parameter_sample_amount_all/50 * (1-total_finish_weight/pow(2.0,20))),1));
//		}
		if((BDD_width_each_layer[i]==0)){
			total_BDD_depth = i;
			break;
		}
		total_nodes_amount += BDD_width_each_layer[i];

		if(i>0){	
			delete[]	train_dataset_valid_line_index;
			train_dataset_valid_line_index = new int [train_dataset_valid_line_amount];
			int	last_train_dataset_valid_line_amount = train_dataset_valid_line_amount;
			train_dataset_valid_line_amount = 0;
			for(zi=0;zi<last_train_dataset_valid_line_amount;zi++){
				if(train_dataset_which_node[zi] < 0){
					continue;
				}else{
					
					if(train_dataset_temp[zi][most_influence[i-1]] == 0){
						if(BDD[i-1][train_dataset_which_node[zi]].left_node_neg){
							train_dataset_output_temp[zi] = !train_dataset_output_temp[zi];
						}
						train_dataset_which_node[train_dataset_valid_line_amount] = BDD[i-1][train_dataset_which_node[zi]].left_node_index;
					}else{
						if(BDD[i-1][train_dataset_which_node[zi]].right_node_neg){
							train_dataset_output_temp[zi] = !train_dataset_output_temp[zi];
						}
						train_dataset_which_node[train_dataset_valid_line_amount] = BDD[i-1][train_dataset_which_node[zi]].right_node_index;
					}
				}
				//if(train_dataset_which_node[zi] < 0){
				//	continue;
				//}
				for(int zj=0;zj<i-1;zj++){
					train_dataset_temp[zi][most_influence[zj]] = BDD_mask_this[train_dataset_which_node[zi]].mask[most_influence[zj]];
				}
				
				//bool	not_equal_train_data = 1;
				//for(int zz=0;zz<zi;zz++){
				//	if(train_dataset_which_node[zi] != train_dataset_which_node[zz]){
				//		continue;
				//	}
				//	bool equal_train_data_here = 1;
				//	for(int zj=0;zj<bit_width;zj++){
				//		if(train_dataset[zi][zj] != train_dataset[zz][zj]){
				//			equal_train_data_here = 0;
				//			break;
				//		}
				//	}
				//	if(equal_train_data_here){
				//		not_equal_train_data = 0;
				//		break;
				//	}
				//}
				//if(not_equal_train_data == 0){
				//	train_dataset_which_node[zi] = -3;
				//}
				//if(train_dataset_which_node[zi] >= 0){
				    train_dataset_valid_line_index[train_dataset_valid_line_amount] = zi;
				    train_dataset_valid_line_amount += 1;
				//}
			}
			
			bool** train_dataset_temp_a 		= new bool*[train_dataset_valid_line_amount];	
			bool* train_dataset_output_temp_a 	= new bool [train_dataset_valid_line_amount];	
			for(zi=0;zi<train_dataset_valid_line_amount;zi++){
				train_dataset_temp_a[zi] = new bool[bit_width];
				for (int zj=0;zj<bit_width;zj++){
					train_dataset_temp_a[zi][zj] 	= train_dataset_temp[train_dataset_valid_line_index[zi]][zj];
				}
				train_dataset_output_temp_a[zi] 	= train_dataset_output_temp[train_dataset_valid_line_index[zi]];
			}
			for(zi=0;zi<train_dataset_valid_line_amount;zi++){
				for (int zj=0;zj<bit_width;zj++){
					train_dataset_temp[zi][zj] 	= train_dataset_temp_a[zi][zj];
				}
				train_dataset_output_temp[zi] 	= train_dataset_output_temp_a[zi];
			}
			for(zi=0;zi<train_dataset_valid_line_amount;zi++){
				delete[]	train_dataset_temp_a[zi];
			}
			delete[]	train_dataset_temp_a;	
			delete[]	train_dataset_output_temp_a;	
		}

		BDD[i+1] 	= new BDD_node[BDD_width_each_layer[i]*2];
		//for (zi=0;zi<parameter_sample_amount;zi++){
		
		for(zi=0;zi<BDD_width_each_layer[i]*2;zi++){
			simplify_list_sort[zi] = zi;
			BDD[i+1][zi].has_equal_father = 0;
			BDD[i+1][zi].non_equal_number = 0;
		}
		most_influence[i] = cal_most_influence_next(i,BDD,most_influence); 
		cout<<parameter_sample_amount<<endl;
		cout<<"The BSD at layer: "<<i<<"		";
		cout<<"Input bit this layer:  x"<<most_influence[i]<<endl;
		cout<<"The BSD width this layer: "<<BDD_width_each_layer[i]<<endl;
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
			//for (zi=0;zi<i;zi++){
			//	cout<<BDD[i][j].mask[most_influence[zi]]<<" ";
			//}
			//cout<<endl;
			//cout<<j<<endl;
			left_son_neg 	= 0;
			right_son_neg 	= 0;
			
			
			if(BDD[i][j].has_equal_father){
				;
			}else{
				BDD[i][j].non_equal_number = total_non_equal_nodes;
				total_non_equal_nodes += 1;
			}

			//cout<<BDD[i][j].weight<<" ";
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
				left_mask_output_data[zi] = io_generator_single(mask_input_data[zi],which_bit_output);
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
				right_mask_output_data[zi] = io_generator_single(mask_input_data[zi],which_bit_output);
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
			//cout<<zi<<endl;
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
		////////	if(BDD[i][j].weight < 0.00001){
		////////		if(left_mask_output_data_sum > (parameter_sample_amount/2)){
		////////			all_one_left 	= 1;
		////////			all_zero_left	= 0;
		////////		}else{
		////////			all_one_left 	= 0;
		////////			all_zero_left	= 1;
		////////		}
		////////		if(right_mask_output_data_sum > (parameter_sample_amount/2)){
		////////			all_one_right 	= 1;
		////////			all_zero_right	= 0;
		////////		}else{
		////////			all_one_right 	= 0;
		////////			all_zero_right	= 1;
		////////		}
		////////	}
			if(all_one_left == 1){
				for(zi=0;zi<train_dataset_valid_line_amount;zi++){
					if(train_dataset_which_node[zi] == j){
						if((train_dataset_temp[zi][most_influence[i]] == 0) && (train_dataset_output_temp[zi] == 0)){
							all_one_left = 0;
						}
					}
				}
			}
			if(all_one_right == 1){
				for(zi=0;zi<train_dataset_valid_line_amount;zi++){
					if(train_dataset_which_node[zi] == j){
						if((train_dataset_temp[zi][most_influence[i]] == 1) && (train_dataset_output_temp[zi] == 0)){
							all_one_right = 0;
						}
					}
				}
			}
			if(all_zero_left == 1){
				for(zi=0;zi<train_dataset_valid_line_amount;zi++){
					if(train_dataset_which_node[zi] == j){
						if((train_dataset_temp[zi][most_influence[i]] == 0) && (train_dataset_output_temp[zi] == 1)){
							all_zero_left = 0;
						}
					}
				}
			}
			if(all_zero_right == 1){
				for(zi=0;zi<train_dataset_valid_line_amount;zi++){
					if(train_dataset_which_node[zi] == j){
						if((train_dataset_temp[zi][most_influence[i]] == 1) && (train_dataset_output_temp[zi] == 1)){
							all_zero_right = 0;
						}
					}
				}
			}
			//cout<<zi<<endl;
			if(i>parameter_depth_start_simplify){
				if(all_zero_left){
					BDD[i][j].left_node_index = -2;
					total_finish_weight += BDD[i][j].weight/2;
				}else if(all_one_left){
					BDD[i][j].left_node_index = -1; 
					total_finish_weight += BDD[i][j].weight/2;
				}else{
					if(((i%parameter_how_often_simplify) == 0)){
						which_list_number 	= compare_simplify_list    (BDD_width_each_layer[i+1],left_mask_output_data,left_mask_output_data_sum,left_mask_output_data_sum_half,first_zero_left,first_one_left);
						which_list_number_neg 	= compare_simplify_list_neg(BDD_width_each_layer[i+1],left_mask_output_data,parameter_sample_amount-left_mask_output_data_sum,int(parameter_sample_amount/2)-left_mask_output_data_sum_half,first_zero_left,first_one_left);
						//cout<<which_list_number<<endl;
						bool	it_cannot_simplify 	= (which_list_number < 0)&&(which_list_number_neg<0);
						bool	it_can_simplify 	= (which_list_number 		>=0);
						bool	it_can_simplify_neg 	= (which_list_number_neg	>=0);
						if(it_can_simplify){
							for(zi=0;zi<train_dataset_valid_line_amount;zi++){
								if((train_dataset_which_node[zi] == j) && (train_dataset_temp[zi][most_influence[i]] == 0)){
									bool* simplify_bits_input = new bool [bit_width];
									for(int zj=0;zj<bit_width;zj++){
										simplify_bits_input[zj] = train_dataset_temp[zi][zj];
									}
									for(int zj=0;zj<i+1;zj++){
										simplify_bits_input[most_influence[zj]] = BDD_mask_next[which_list_number].mask[most_influence[zj]];
									}
									bool simplify_bit_output = io_generator_single(simplify_bits_input,which_bit_output);
									if(simplify_bit_output != train_dataset_output_temp[zi]){
										it_can_simplify = 0;
									}
									delete[] simplify_bits_input;
								}
							}
						}
						if(it_can_simplify_neg){
							for(zi=0;zi<train_dataset_valid_line_amount;zi++){
								if((train_dataset_which_node[zi] == j) && (train_dataset_temp[zi][most_influence[i]] == 0)){
									bool* simplify_bits_input = new bool [bit_width];
									for(int zj=0;zj<bit_width;zj++){
										simplify_bits_input[zj] = train_dataset_temp[zi][zj];
									}
									for(int zj=0;zj<i+1;zj++){
										simplify_bits_input[most_influence[zj]] = BDD_mask_next[which_list_number_neg].mask[most_influence[zj]];
									}
									bool simplify_bit_output = io_generator_single(simplify_bits_input,which_bit_output);
									if(simplify_bit_output == train_dataset_output_temp[zi]){
										it_can_simplify_neg = 0;
									}
									delete[] simplify_bits_input;
								}
							}
						}
						if((it_can_simplify_neg == 0) && (it_can_simplify == 0)){
							it_cannot_simplify = 1;
						}
						if(it_cannot_simplify){
							BDD_width_each_layer[i+1] += 1;
							BDD[i+1][BDD_width_each_layer[i+1]-1].depth  = i+1; 
							BDD[i+1][BDD_width_each_layer[i+1]-1].weight = BDD[i][j].weight/2; 
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
						}else if(it_can_simplify){
							BDD[i][j].left_node_index = 	which_list_number;
							BDD[i][j].left_node_neg   = 	0;
							BDD[i+1][which_list_number].weight += BDD[i][j].weight/2;
						}else if(it_can_simplify_neg){
							BDD[i][j].left_node_index = 	which_list_number_neg;
							BDD[i][j].left_node_neg   = 	1;
							BDD[i+1][which_list_number_neg].weight += BDD[i][j].weight/2;
						}
					}else{
						BDD_width_each_layer[i+1] += 1;
						BDD[i+1][BDD_width_each_layer[i+1]-1].depth = i+1; 
						BDD[i+1][BDD_width_each_layer[i+1]-1].weight = BDD[i][j].weight/2; 
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
						for (zi=0;zi<i+1;zi++){
							BDD_mask_next[BDD_width_each_layer[i+1]-1].mask[most_influence[zi]]  = left_son_mask[most_influence[zi]]; 
						}
						BDD[i][j].left_node_index = 	BDD_width_each_layer[i+1]-1;
						BDD[i][j].left_node_neg   = 	0;

			}
			//cout<<all_zero_right<<all_one_right<<all_zero_left<<all_one_left<<endl;
			if(i>parameter_depth_start_simplify){
				if(all_zero_right){
					BDD[i][j].right_node_index = -2; 
					total_finish_weight += BDD[i][j].weight/2;
				}else if(all_one_right){
					BDD[i][j].right_node_index = -1; 
					total_finish_weight += BDD[i][j].weight/2;
				}else{
					if(((i%parameter_how_often_simplify == 0))){
						which_list_number 	= compare_simplify_list(BDD_width_each_layer[i+1],right_mask_output_data,right_mask_output_data_sum,right_mask_output_data_sum_half,first_zero_right,first_one_right);
						which_list_number_neg 	= compare_simplify_list_neg(BDD_width_each_layer[i+1],right_mask_output_data,parameter_sample_amount-right_mask_output_data_sum,int(parameter_sample_amount/2)-right_mask_output_data_sum,first_zero_left,first_one_left);
						//cout<<which_list_number<<endl;
						bool	it_cannot_simplify 	= (which_list_number < 0)&&(which_list_number_neg<0);
						bool	it_can_simplify 	= (which_list_number 		>=0);
						bool	it_can_simplify_neg 	= (which_list_number_neg	>=0);
						if(it_can_simplify){
							for(zi=0;zi<train_dataset_valid_line_amount;zi++){
								if((train_dataset_which_node[zi] == j) && (train_dataset_temp[zi][most_influence[i]] == 1)){
									bool* simplify_bits_input = new bool [bit_width];
									for(int zj=0;zj<bit_width;zj++){
										simplify_bits_input[zj] = train_dataset_temp[zi][zj];
									}
									for(int zj=0;zj<i+1;zj++){
										simplify_bits_input[most_influence[zj]] = BDD_mask_next[which_list_number].mask[most_influence[zj]];
									}
									bool simplify_bit_output = io_generator_single(simplify_bits_input,which_bit_output);
									if(simplify_bit_output != train_dataset_output_temp[zi]){
										it_can_simplify = 0;
									}
									delete[] simplify_bits_input;
								}
							}
						}
						if(it_can_simplify_neg){
							for(zi=0;zi<train_dataset_valid_line_amount;zi++){
								if((train_dataset_which_node[zi] == j) && (train_dataset_temp[zi][most_influence[i]] == 1)){
									bool* simplify_bits_input = new bool [bit_width];
									for(int zj=0;zj<bit_width;zj++){
										simplify_bits_input[zj] = train_dataset_temp[zi][zj];
									}
									for(int zj=0;zj<i+1;zj++){
										simplify_bits_input[most_influence[zj]] = BDD_mask_next[which_list_number_neg].mask[most_influence[zj]];
									}
									bool simplify_bit_output = io_generator_single(simplify_bits_input,which_bit_output);
									if(simplify_bit_output == train_dataset_output_temp[zi]){
										it_can_simplify_neg = 0;
									}
									delete[] simplify_bits_input;
								}
							}
						}
						if((it_can_simplify_neg == 0) && (it_can_simplify == 0)){
							it_cannot_simplify = 1;
						}
						if(it_cannot_simplify){
							BDD_width_each_layer[i+1] += 1;
							BDD[i+1][BDD_width_each_layer[i+1]-1].depth = i+1; 
							BDD[i+1][BDD_width_each_layer[i+1]-1].weight = BDD[i][j].weight/2; 
							for (zi=0;zi<i+1;zi++){
								BDD_mask_next[BDD_width_each_layer[i+1]-1].mask[most_influence[zi]]  = right_son_mask[most_influence[zi]]; 
							}
							BDD[i][j].right_node_index = 	BDD_width_each_layer[i+1]-1;
							BDD[i][j].right_node_neg    = 	0;
							for (zi=0;zi<parameter_sample_amount;zi++){
								simplify_list[BDD_width_each_layer[i+1]-1][zi] 	= right_mask_output_data[zi];
								//simplify_list_heap[BDD_width_each_layer[i+1]-1].bools_result[zi] = right_mask_output_data[zi];
							}
							simplify_list_sum[BDD_width_each_layer[i+1]-1]				= right_mask_output_data_sum;
							simplify_list_sum_half[BDD_width_each_layer[i+1]-1]			= right_mask_output_data_sum_half;
							simplify_list_first_zero[BDD_width_each_layer[i+1]-1]			= first_zero_right;
							simplify_list_first_one[BDD_width_each_layer[i+1]-1]			= first_one_right;
							//simplify_list_heap[BDD_width_each_layer[i+1]-1].BDD_node_index = BDD_width_each_layer[i+1] - 1;
						}else if(it_can_simplify){
							BDD[i][j].right_node_index = 	which_list_number;
							BDD[i][j].right_node_neg   = 0;
							BDD[i+1][which_list_number].weight += BDD[i][j].weight/2;
						}else if(it_can_simplify_neg){
							BDD[i][j].right_node_index = 	which_list_number_neg;
							BDD[i][j].right_node_neg   = 1;
							BDD[i+1][which_list_number_neg].weight += BDD[i][j].weight/2;
						}
					}else{
						BDD_width_each_layer[i+1] += 1;
						BDD[i+1][BDD_width_each_layer[i+1]-1].depth = i+1; 
						BDD[i+1][BDD_width_each_layer[i+1]-1].weight = BDD[i][j].weight/2; 
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
			//cout<<BDD[i][j].left_node_index<<" "<<BDD[i][j].right_node_index<<endl;
			//delete[]	left_son_mask;
			//delete[]	right_son_mask;
			//delete[]	left_mask_output_data;
			//delete[]	right_mask_output_data;
		}
		//delete[]	simplify_list_sum; 
		//for (zi=0;zi<BDD_width_each_layer[i]*2;zi++){
		//	delete[] simplify_list[zi];
		//}
		//delete[]	simplify_list;
		//delete[]	simplify_list_sort;
		//for (zi=0;zi<parameter_sample_amount;zi++){
		//	delete[]	random_input_data[zi] ;
		//	delete[]	mask_input_data[zi]   ;
		//}
		//delete[]	random_input_data;
		//delete[]	mask_input_data;
		//delete[]	mask_output_data;
		//cout<<endl;
		cout<<"Total Nodes:  "<<total_nodes_amount<<endl;
		//cout<<total_non_equal_nodes<<endl;
		cout<<"Total Splid nodes: "<<total_split_nodes;
		cout<<"		Finished Training weights =  "<<setprecision(12)<<(total_finish_weight)/pow(2.0,20)<<endl;
		gettimeofday(&finish_time,NULL);
		train_time = double(finish_time.tv_usec-start_time.tv_usec+1000000*(finish_time.tv_sec-start_time.tv_sec))/1000000;
		cout<<"Total train time = "<<train_time<<"s"<<endl;
		cout<<"######################################################################"<<endl;
	}
	//gettimeofday(&finish_time,NULL);
	//cout<<"到目前为止的训练时间: "<<(finish_time.tv_usec-start_time.tv_usec+1000000*(finish_time.tv_sec-start_time.tv_sec))<<endl;

	return 0;
};

double	circuit_accuracy;
int	error_amount = 0;
int	train_error  = 0;
int	incre_wrong_dataset_size = 0;
	
bool*	test_input_data = new bool[bit_width];
bool	test_output_data;
bool	infer_output_data;
bool*	infer_output_dataset = new bool[parameter_train_dataset_lines_all];

bool*	train_dataset_i = new bool [bit_width];

int	BDD_infer(int which_bit_output,BDD_node** BDD,int* most_influence){
	long i,j;
	
	error_amount = 0;
	for (i=0;i<parameter_test_amount;i++){
	////	for (j=0;j<bit_width;j++){
	////		//srand((int)time(0));
	////		test_input_data[i][j] = bool(rand()%2);
	////		//cout<<random_input_data[i][j];
	////	}
		//cout<<endl;
		if(which_bit_output<1024){	
			if(i%2==0){
				for (j=0;j<bit_width;j++){
					////if(j<32){
					////	test_input_data[j] = 0;
					////}
					////else 
					if((j<1024) || (j>=32*53)){
					      int zi = 0;
					      long randint;
					      zi = j%30;
					      if(zi == 0){
					      	randint = rand();
					      }
					      //test_input_data[j] = bool(rand()%2);
					      test_input_data[j] = bool((int(randint >> (zi)))%2);
					      //cout<<test_input_data[j]<<endl;
					}else{
						test_input_data[j] = 0;
					}
				}
				test_input_data[1792] = 0;
				test_input_data[1793] = 0;
				test_input_data[1794] = 0;
				test_input_data[1795] = 0;
			}else{
				for (j=0;j<bit_width;j++){
					int zi = 0;
					long randint;
					zi = j%30;
					if(zi == 0){
						randint = rand();
					}
					//test_input_data[j] = bool(rand()%2);
					test_input_data[j] = bool((int(randint >> (zi)))%2);
					//cout<<test_input_data[j]<<endl;
				}
			}
		}else{
			for (j=0;j<bit_width;j++){
				int zi = 0;
				long randint;
				zi = j%30;
				if(zi == 0){
					randint = rand();
				}
					test_input_data[j] = bool(((randint >> (zi)))%2);
				//test_input_data[j] = bool(rand()%2);
			}
		}
				bool	rs_a_code[5];
				bool	rs_b_code[5];
				for (j=0;j<5;j++){
					rs_a_code[j] = test_input_data[12+1696+j];
					rs_b_code[j] = test_input_data[ 7+1696+j];
				}
				if(which_bit_output<1024){
					for(j=0;j<5;j++)
						test_input_data[20+1696+j] = which_bit_output_general_reg_index[j];
				}
				int	reg_a_index = cvt_bit_to_number_unsigned(rs_a_code,5);
				int 	reg_b_index = cvt_bit_to_number_unsigned(rs_b_code,5);
				for(j=0;j<32;j++){
					test_input_data[1798+j] = test_input_data[32*reg_a_index+j];
					test_input_data[1830+j] = test_input_data[32*reg_b_index+j];
				}
			int	which_op_code = rand()%12;
			for(j=0;j<7;j++){
				test_input_data[j+1721] = which_op_code_data[which_op_code][j];
			}
			bool this_is_CSR 	= 	(test_input_data[1721] == 1)
						&&	(test_input_data[1722] == 1)
						&&	(test_input_data[1723] == 1)
						&&	(test_input_data[1724] == 0)
						&&	(test_input_data[1725] == 0)
						&&	(test_input_data[1726] == 1)
						&&	(test_input_data[1727] == 1)
						;
			if(this_is_CSR){
				int	which_csr 	= rand()%21;
				int	which_csr_a 	= rand()%2;
				for(j=0;j<12;j++){
					test_input_data[j+1696] = which_csr_data[which_csr][j];
				}
				if(which_csr_a==1){
					
				}else{
					if((which_bit_output>1023)&&(which_bit_output<53*32))
					for(j=0;j<12;j++){
						test_input_data[j+1696] = which_csr_data[int(which_bit_output-1024)/32][j];
					}
				}
				for(j=0;j<32;j++){
					test_input_data[j+1862] = test_input_data[which_csr*32+1024+j];
				}
			}else{
			}
						bool op_code_3_slli 	= 	(test_input_data[1721] == 0)
									&&	(test_input_data[1722] == 0)
									&&	(test_input_data[1723] == 1)
									&&	(test_input_data[1724] == 0)
									&&	(test_input_data[1725] == 0)
									&&	(test_input_data[1726] == 1)
									&&	(test_input_data[1727] == 1)
									&&	(test_input_data[1714] == 0)
									&&	(test_input_data[1715] == 1)
									;
						bool op_code_3_add 	= 	(test_input_data[1721] == 0)
									&&	(test_input_data[1722] == 1)
									&&	(test_input_data[1723] == 1)
									&&	(test_input_data[1724] == 0)
									&&	(test_input_data[1725] == 0)
									&&	(test_input_data[1726] == 1)
									&&	(test_input_data[1727] == 1)
									;
						if(op_code_3_slli || op_code_3_add){
							test_input_data[1696] = 0;
							test_input_data[1698] = 0;
							test_input_data[1699] = 0;
							test_input_data[1700] = 0;
							test_input_data[1701] = 0;
							test_input_data[1702] = 0;
						}
		//for(j=0;j<bit_width;j++){
		//		cout<<test_input_data[j];
		//	}
		//cout<<endl;
		test_output_data  	= io_generator_single(test_input_data,which_bit_output);
		infer_output_data 	= cal_infer_result(test_input_data,BDD,most_influence);
		if(test_output_data 	!= infer_output_data){
			error_amount = error_amount + 1;
			for(j=0;j<bit_width;j++){
				wrong_dataset[incre_wrong_dataset_size][j] = train_dataset_i[j];
			}
			wrong_dataset_output[incre_wrong_dataset_size] = train_dataset_output[i];
			incre_wrong_dataset_size += 1;
				
			////cout<<test_input_data[1120+20]<<test_input_data[1124+20]<<test_input_data[1120+28];
			////cout<<" ";
			////cout<<test_input_data[1152+20]<<test_input_data[1152+20]<<test_input_data[1152+28];
			////cout<<" ";
			for (j=0;j<5;j++){
				rs_a_code[j] = test_input_data[12+1696+j];
			}
			cout<<" ";
			for (j=0;j<5;j++){
				rs_b_code[j] = test_input_data[ 7+1696+j];
			}
			cout<<" ";

			for(j=0;j<4;j++)
				cout<<test_input_data[1792+j];
			cout<<"	op_code_2: ";
			for(j=0;j<3;j++)
				cout<<test_input_data[1696+17+j];
			cout<<"	op_code: ";
			for(j=0;j<7;j++)
				cout<<test_input_data[1721+j];
			cout<<"	CSR(top 12 bits): ";
			for(j=0;j<12;j++)
				cout<<test_input_data[1696+j];
			cout<<"	rs_1_code: ";
			for(j=0;j<5;j++)
				cout<<test_input_data[1696+12+j];
			cout<<"	rs_2_code: ";
			for(j=0;j<5;j++)
				cout<<test_input_data[1696+7+j];
			cout<<"	rd_code: ";
			for(j=0;j<5;j++)
				cout<<test_input_data[1696+20+j];
			cout<<"	reg_data_a: ";
			cout<<hex<<cvt_bit_to_number_unsigned(test_input_data+1798,32)<<" "<<dec;
			//for(j=0;j<32;j++)
			//	cout<<test_input_data[1798+j];
			cout<<"	reg_data_b: ";
			cout<<hex<<cvt_bit_to_number_unsigned(test_input_data+1830,32)<<" "<<dec;
			//for(j=0;j<32;j++)
			//	cout<<test_input_data[1830+j];
			cout<<"	pc: ";
			for(j=0;j<32;j++)
				cout<<test_input_data[1728+j];
			cout<<endl;

		}
		if((i%100000)==0){
			cout<<i<<" "<<error_amount<<endl;
		}
	}
	train_error  = 0;
	ifstream trace_file("trace_file_r",ios::binary);
	//////for (i=0;i<parameter_train_dataset_lines_all;i++){
	//////	trace_file.read((char*)(train_dataset_i),bit_width*sizeof(train_dataset_i[0]));//<<train_dataset_output[zi]<<endl;
	//////	infer_output_dataset[i] = cal_infer_result(train_dataset_i,BDD,most_influence);
	//////	if(train_dataset_output[i] != infer_output_dataset[i]){
	//////		for(j=0;j<bit_width;j++){
	//////			wrong_dataset[incre_wrong_dataset_size][j] = train_dataset_i[j];
	//////		}
	//////		wrong_dataset_output[incre_wrong_dataset_size] = train_dataset_output[i];
	//////		train_error += 1;
	//////		incre_wrong_dataset_size += 1;
	//////	}
	//////}
	for (i=0;i<100;i++){
		cout<<most_influence[i]<<" ";
	}
	cout<<endl;
	/////////for (i=0;i<parameter_train_dataset_lines_all;i++){
	/////////	if(train_dataset_output[i] != infer_output_dataset[i]){
	/////////		for(j=0;j<bit_width;j++){
	/////////			wrong_dataset[incre_wrong_dataset_size][j] = train_dataset[i][j];
	/////////		}
	/////////		wrong_dataset_output[incre_wrong_dataset_size] = train_dataset_output[i];
	/////////		train_error += 1;
	/////////		incre_wrong_dataset_size += 1;
	/////////	}
	/////////}
	circuit_accuracy = 1-((error_amount)/double(parameter_test_amount));
	cout<<error_amount<<"	Train accuracy = "<<1-(error_amount/double(parameter_test_amount))<<endl;
	cout<<train_error<<"	Test  accuracy = "<<1-(train_error /double(parameter_train_dataset_lines_all))<<endl;
	return 0;
};
//#include"new_print_circuit.h"
#include"print_circuit.h"
int	main(int argc,char* argv[]){

	int i,j;
	if(argc == 2){
		which_bit_output = atoi(argv[1]);
	}
	which_bit_output_general_reg_index = cvt_number_to_bit_unsigned(int(which_bit_output/32),5);
	DIR*	output_dir;
	output_dir	= opendir("../../../result/CPU/cluster_6/rtl");
	dirent	*ptr;
	vector<string>	files;	
	while((ptr=readdir(output_dir))!=NULL){
		files.push_back(ptr->d_name);
	}
	bool	this_bit_finished = 0;
	for (i=0;i<files.size();i++){
		if(files[i].substr(0,1) == "m"){
			int	this_output_bit = 0;
			for(j=0;j<10;j++){
				//cout<<files[i].substr(18+j,1);
				if(files[i].substr(18+j+1,1) == "."){
					break;
				}
			}
			if(atoi(files[i].substr(18,18+j).c_str()) == which_bit_output){
				this_bit_finished = 1;
				cout<<"this bit finished:	"<<which_bit_output<<endl;
				return 0;
				break;
			}
		}
	}
	closedir(output_dir);


	gettimeofday(&start_time,NULL);


	
	for (i=0;i<bit_width+1;i++){
		amount_turn_input_data[i]  = new bool* [parameter_sample_amount_influence_max];
		amount_turn_output_data[i] = new bool  [parameter_sample_amount_influence_max];
		for(j=0;j<parameter_sample_amount_influence_max;j++){
			amount_turn_input_data[i][j] = new bool [bit_width];
		}
		has_been_unfold[i] = 0;
	}
	for (i=0;i<parameter_train_dataset_lines_all;i++){
		train_dataset[i] 	= new bool [bit_width];
	}
	for (i=0;i<parameter_train_dataset_lines_temp;i++){
		train_dataset_temp[i] 	= new bool [bit_width];
	}

		gettimeofday(&finish_time,NULL);
		train_time = double(finish_time.tv_usec-start_time.tv_usec+1000000*(finish_time.tv_sec-start_time.tv_sec))/1000000;
////////	//ifstream trace_file("trace_file");
////////	//ifstream trace_file("trace_file_r",ios::binary);
////////	ifstream trace_file_output("trace_file_r_output",ios::binary);
////////	char trace_file_line[20000];
////////	trace_file_output.read((char*)train_dataset_output,parameter_train_dataset_lines_all*sizeof(train_dataset_output[0]));
////////		gettimeofday(&finish_time,NULL);
////////		train_time = double(finish_time.tv_usec-start_time.tv_usec+1000000*(finish_time.tv_sec-start_time.tv_sec))/1000000;
////////	////for(int zi=0;zi<parameter_train_dataset_lines_all;zi++){
////////	////	trace_file.read((char*)(train_dataset[zi]),bit_width*sizeof(train_dataset[0][0]));//<<train_dataset_output[zi]<<endl;
////////	////}
////////		gettimeofday(&finish_time,NULL);
////////		train_time = double(finish_time.tv_usec-start_time.tv_usec+1000000*(finish_time.tv_sec-start_time.tv_sec))/1000000;

////	ofstream trace_file("trace_file_r",ios::binary);
////	ofstream trace_file_output("trace_file_r_output",ios::binary);
	for (i=0;i<int(parameter_train_dataset_lines_all/pow(2.0,parameter_multi_output_index));i++){
		//trace_file.getline(trace_file_line,20000);
		for(j=0;j<bit_width-parameter_multi_output_index;j++){
			train_dataset[i][j] = rand()%2;//trace_file_line[j];
		}
		
	}
	for(int zi=0;zi<pow(2,parameter_multi_output_index);zi++){
		bool* zi_bit;
		zi_bit = cvt_number_to_bit_unsigned(zi,parameter_multi_output_index);
		for (i=0;i<parameter_train_dataset_lines_all/pow(2,parameter_multi_output_index);i++){
			for(j=0;j<bit_width-parameter_multi_output_index;j++){
				train_dataset[i+zi*int(parameter_train_dataset_lines_all/pow(2,parameter_multi_output_index))][j] = train_dataset[i][j];
			}
			for(j=bit_width-parameter_multi_output_index;j<bit_width;j++){
				train_dataset[i+zi*int(parameter_train_dataset_lines_all/pow(2,parameter_multi_output_index))][j] = zi_bit[j-bit_width+parameter_multi_output_index];
			}		
		}
		delete[] zi_bit;
	}
		gettimeofday(&finish_time,NULL);
		train_time = double(finish_time.tv_usec-start_time.tv_usec+1000000*(finish_time.tv_sec-start_time.tv_sec))/1000000;

	for(int zi=0;zi<parameter_train_dataset_lines_all;zi++){
		train_dataset_which_node[zi] 	= 0;
		train_dataset_output[zi]	= io_generator_single(train_dataset[zi],which_bit_output);
		/////train_dataset_output_temp[zi]	= train_dataset_output[zi];
		/////for(int zj=0;zj<bit_width;zj++){
		/////	train_dataset_temp[zi][zj] = train_dataset[zi][zj];
		/////}
	}
		gettimeofday(&finish_time,NULL);
		train_time = double(finish_time.tv_usec-start_time.tv_usec+1000000*(finish_time.tv_sec-start_time.tv_sec))/1000000;
////	//for(int zi=0;zi<parameter_train_dataset_lines_all;zi++){
////		trace_file_output.write((char*)(train_dataset_output),parameter_train_dataset_lines_all*sizeof(train_dataset_output[0]));//<<train_dataset_output[zi]<<endl;
////	//}
////		gettimeofday(&finish_time,NULL);
////		train_time = double(finish_time.tv_usec-start_time.tv_usec+1000000*(finish_time.tv_sec-start_time.tv_sec))/1000000;
////
////	for(int zi=0;zi<parameter_train_dataset_lines_all;zi++){
////		
////		trace_file.write((char*)(train_dataset[zi]),bit_width*sizeof(train_dataset[0][0]));//<<train_dataset_output[zi]<<endl;
////		//for(j=0;j<bit_width;j++){
////		//	trace_file<<train_dataset[zi][j];		
////		//}
////		//trace_file<<endl;
////	}
////		gettimeofday(&finish_time,NULL);
////		train_time = double(finish_time.tv_usec-start_time.tv_usec+1000000*(finish_time.tv_sec-start_time.tv_sec))/1000000;
	for(i=0;i<bit_width;i++){
		train_dataset_bit_influence[i] = 0;
		///////for (int zi_r=0;zi_r<100;zi_r++){
		///////	int zi = rand()%parameter_train_dataset_lines;
		///////	bool* bit_switch_input  = new bool[bit_width];
		///////	for(int zj=0;zj<bit_width;zj++){
		///////		bit_switch_input[zj] = train_dataset[zi][zj];
		///////		if(zj==i){
		///////			bit_switch_input[zj] = !train_dataset[zi][zj];
		///////		}
		///////	}
		///////	bool  bit_switch_output = io_generator_single(bit_switch_input,which_bit_output);
		///////	if(bit_switch_output != train_dataset_output[zi]){
		///////		train_dataset_bit_influence[i] += 1;
		///////	} 
		///////	delete[] bit_switch_input;
		///////}
		///////train_dataset_bit_influence[i] /= 1000;
		//cout<<i<<endl;
	}

	srand((unsigned)time(0));
//	truth_table = new bool[int(pow(2,24))];
//	for (i=0;i<int(pow(2,24));i++){
//		//truth_table[i] = rand()%2;
//		truth_table[i] = ((int(i/int(pow(2,12))) * (i%int(pow(2,12)))) >> which_bit_output) & (1);
//	}
//	cout<<"ready to train"<<endl;
	

	for(i=0;i<parameter_train_dataset_lines_temp;i++){
		wrong_dataset[i] = new bool [bit_width];
	}

	
	if(this_bit_finished == 0){
		parameter_train_dataset_lines = 0;
		parameter_train_dataset_lines_init = 0;
		for(int zi=0;zi<parameter_train_dataset_lines;zi++){
			train_dataset_which_node[zi] 	= 0;
			train_dataset_output_temp[zi]	= train_dataset_output[zi];
			for(int zj=0;zj<bit_width;zj++){
				train_dataset_temp[zi][zj] = train_dataset[zi][zj];
			}
		}
		for(int zi=0;zi<incre_wrong_dataset_size;zi++){
			train_dataset_which_node[zi+parameter_train_dataset_lines] 	= 0;
			train_dataset_output_temp[zi+parameter_train_dataset_lines]	= wrong_dataset_output[zi];
			for(int zj=0;zj<bit_width;zj++){
				train_dataset_temp[zi+parameter_train_dataset_lines][zj] = wrong_dataset[zi][zj];
			}
		}
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
		train_BDD(which_bit_output,BDD_0,most_influence_0);
		BDD_infer(which_bit_output,BDD_0,most_influence_0);
		if(train_error==0){
			if(which_bit_output<1024){
				if(circuit_accuracy > 0.999998){
					print_circuit(which_bit_output,BDD_0,most_influence_0);
				}
			}else{
				if(circuit_accuracy > 0.998){
					print_circuit(which_bit_output,BDD_0,most_influence_0);
				}
			}
		}
		for(int zz=0;zz<0;zz++){
			which_BDD += 1;
			delete[] most_influence_1;
			most_influence_1 = new int [bit_width];
			for(int zi=0;zi<total_BDD_depth;zi++){
				delete[] BDD_1[zi];
			}
			delete[] BDD_1;
			BDD_1 = new BDD_node*[1+bit_width];
			for(int zi=0;zi<parameter_train_dataset_lines_init;zi++){
				train_dataset_which_node[zi] 	= 0;
				train_dataset_output_temp[zi]	= train_dataset_output[zi];
				for(int zj=0;zj<bit_width;zj++){
					train_dataset_temp[zi][zj] = train_dataset[zi][zj];
				}
			}
			for(int zi=0;zi<incre_wrong_dataset_size;zi++){
				train_dataset_which_node[zi+parameter_train_dataset_lines_init] 	= 0;
				train_dataset_output_temp[zi+parameter_train_dataset_lines_init]	= wrong_dataset_output[zi];
				for(int zj=0;zj<bit_width;zj++){
					train_dataset_temp[zi+parameter_train_dataset_lines_init][zj] = wrong_dataset[zi][zj];
				}
			}
			parameter_train_dataset_lines = incre_wrong_dataset_size + parameter_train_dataset_lines_init;	
			train_BDD(which_bit_output,BDD_1,most_influence_1);
			BDD_infer(which_bit_output,BDD_1,most_influence_1);
			if(0){//train_error==0){
				if(which_bit_output<1024){
					if(circuit_accuracy > 0.999998){
						print_circuit(which_bit_output,BDD_1,most_influence_1);
					}
				}else{
					if(circuit_accuracy > 0.999998){
						print_circuit(which_bit_output,BDD_1,most_influence_1);
					}
				}
				
				break;
			}
		}
		
	}
	else{
		parameter_test_amount 		= 100;
		BDD_infer(which_bit_output,BDD_1,most_influence_1);
		print_circuit(which_bit_output,BDD_1,most_influence_1);
	}
};



