#pragma once
#include<iostream>
#include<cstring>
using namespace std;

int	cvt_bit_to_number(bool input_data[], int k){

	int	output_number = 0;
	int	i;
	for (i=0;i<k-1;i++){
		output_number += uint32_t(input_data[k-1-i]) << i; 
		//cout<<input_data[i];
	}
	if(input_data[0]){
		output_number -= uint32_t(1)<<(k-1);
	}
	//cout<<' '<<dec<< output_number<<endl;
	return	output_number;
};


bool*	cvt_number_to_bit(int input_data, int k){
	int	i;
	//bool*	output_bits;
	bool* output_bits = new bool [k];
	for (i=0;i<k;i++){
		output_bits[i] = (input_data >> (k-1-i)) & int(1); 
	}
	return	output_bits;
};

long	cvt_bit_to_number_unsigned(bool input_data[], int k){

	uint32_t	output_number = 0;
	int	i;
	for (i=0;i<k;i++){
		output_number += uint32_t(input_data[i]) << (k-1-i);
		//cout<<input_data[i];
	}
	//cout<<' '<<dec<<output_number<<endl;
	return	output_number;
};

bool*	cvt_number_to_bit_unsigned(uint32_t input_data, int k){

	bool*	output_bits = new bool [k];
	int	i;
	for (i=0;i<k;i++){
		output_bits[i] = (input_data >> (k-1-i)) & uint32_t(1); 
		//cout<<output_number[i];
	}
	//cout<<' '<<hex<<input_data<<endl;
	return	output_bits;
};

void sign_extend(bool* bit_output, int output_length, bool* bit_input, int input_length) {
	for(int i = 0; i < output_length-input_length; i++)
		bit_output[i] = bit_input[0];
	for(int i = 0; i < input_length; i++)
		bit_output[output_length - input_length + i] = bit_input[i];
}

void zero_extend(bool* bit_output, int output_length, bool* bit_input, int input_length) {
	for(int i = 0; i < output_length-input_length; i++)
		bit_output[i] = 0;
	for(int i = 0; i < input_length; i++)
		bit_output[output_length - input_length + i] = bit_input[i];
}

void add_bit_list(bool* bit_output, bool* bit_input_a, bool* bit_input_b, int length) {
	bool c = 0;
	for(int i = length-1; i >= 0; i--) {
		int temp = int(bit_input_a[i]) + int(bit_input_b[i]) + int(c);
		bit_output[i] = temp%2==1 ? 1:0 ;
		c = temp > 1? 1:0;
	}
}

void copy_indice(bool* dst, uint32_t dst_idx, bool* src, uint32_t src_idx, uint32_t num) {
	memcpy(dst + dst_idx, src + src_idx, num*sizeof(bool));
}

template <typename T>
void cout_indice(T arr, uint32_t idx, uint32_t num) {
	for (int i = 0; i < num; i++)
		cout << arr[i + idx];
	cout << endl;
}

void init_indice(bool* arr, uint32_t idx, uint32_t num) {
	memset(arr+idx, 0, num*sizeof(bool));
}


void init_indice(uint32_t* arr, uint32_t idx, uint32_t num) {
	memset(arr+idx, 0, num*sizeof(uint32_t));
}

void cvt_number_to_bit_unsigned(bool* output_number, uint32_t input_data, int k) {
	int i;
	for (i = 0; i < k; i++) {
		output_number[i] = (input_data >> (k - 1 - i)) & uint32_t(1);
		//	cout<<output_number[i];
	}
	// cout<<' '<< dec<<input_data<<endl;
};

void cvt_number_to_bit(bool* output_number, int input_data, int k) {
	int i;
	for (i = 0; i < k; i++) {
		output_number[i] = (input_data >> (k - 1 - i)) & uint32_t(1);
		//	cout<<output_number[i];
	}
	// cout<<' '<< input_data<<endl;
};

