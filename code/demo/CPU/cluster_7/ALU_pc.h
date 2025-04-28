bool	ALU_pc(bool input_data[bit_width]){

	
	bool	instruction[32];
	bool	this_pc[32];
	bool	reg_data_a[32];
	bool	reg_data_b[32];
	bool*	next_pc = new bool [32];
	bool data_immi_a[	32];
	bool data_immi_b[	21];
	bool data_immi_c[	12];
	bool data_immi_d[	13];
	bool data_immi_e[	12];
	for (i=0;i<32;i++){
		instruction[i]	= input_data[i];
		this_pc[i]	= input_data[i+32];
		reg_data_a[i]	= input_data[i+64];
		reg_data_b[i]	= input_data[i+96];
	}
	bool op_code[7];
	for (i=0;i<7;i++){
		op_code[i] = instruction[25+i];
	}
	
	for (i=0;i<32;i++){
		if(i<20){
			data_immi_a[i] = instruction[i];
		}else{
			data_immi_a[i] = 0;
		}
	}
	for (i=0;i<21;i++){
		if (i < 1){
			data_immi_b[i] = instruction[i];
		}else if (i < 9){
			data_immi_b[i] = instruction[11+i];
		}else if (i < 10){
			data_immi_b[i] = instruction[11];
		}else if (i < 20){
			data_immi_b[i] = instruction[i-9];
		}else{
			data_immi_b[i] = 0;
		}
	}
	for (i=0;i<12;i++){
		data_immi_c[i] = instruction[i];
	}
	for (i=0;i<13;i++){
		if (i < 1){
			data_immi_d[i] = instruction[i];
		}else if (i < 2){
			data_immi_d[i] = instruction[24];
		}else if (i < 8){
			data_immi_d[i] = instruction[i-1];
		}else if (i < 12){
			data_immi_d[i] = instruction[i+12];
		}else{
			data_immi_d[i] = 0;
		}
	}
	for (i=0;i<12;i++){
		if (i < 7){
			data_immi_e[i] = instruction[i];
		}else{
			data_immi_e[i] = instruction[i+13];
		}
	}
bool 	b_0_opcode_lui 	  [7]	= {0,1,1,0,1,1,1};
bool 	b_1_opcode_auipc  [7]	= {0,0,1,0,1,1,1};
bool 	b_2_opcode_jal 	  [7]	= {1,1,0,1,1,1,1};
bool 	b_3_opcode_jalr   [7]	= {1,1,0,0,1,1,1};
bool 	b_4_opcode_beq 	  [7]	= {1,1,0,0,0,1,1};
		if( compare_bool_list(op_code,b_0_opcode_lui,7) == 0){					//lui
		}else if( compare_bool_list(op_code,b_1_opcode_auipc,7) == 0){				//auipc
		}else if( compare_bool_list(op_code,b_2_opcode_jal,7) == 0){				//jal
			result_tensor_valid = 1;	
			number_a	=	cvt_bit_to_number(this_pc,32);
			number_b	=	cvt_bit_to_number(data_immi_b,21);
			number_result	=	number_a + number_b;
			delete[]	next_pc;
			next_pc		= 	cvt_number_to_bit(number_result,32);
			//cout<<number_a<<" "<<number_b<<" "<<number_result<<endl;
		}else if( compare_bool_list(op_code,b_3_opcode_jalr,7) == 0){				//jalr
			if(number_step_2_opcode == 0){						
				number_a	=	cvt_bit_to_number(reg_data_a,32);
				number_b	=	cvt_bit_to_number(data_immi_c,12);
				number_result	=	number_a + number_b;
				delete[]	next_pc;
				next_pc		= 	cvt_number_to_bit(number_result,32);
			}
		}else if( compare_bool_list(op_code,b_4_opcode_beq,7) == 0){				//分支指令系列
			//子选项是对step_2_opcode的索引
			if(number_step_2_opcode == 0){							//beq
				if(compare_bool_list(reg_data_a,reg_data_b,32) == 0){
					number_a	=	cvt_bit_to_number(this_pc,32);
					number_b	=	cvt_bit_to_number(data_immi_d,13);
					number_result	=	number_a + number_b;
					delete[]	next_pc;
					next_pc		= 	cvt_number_to_bit(number_result,32);
				}
			}else if(number_step_2_opcode == 1){						//bne
				if(!(compare_bool_list(reg_data_a,reg_data_b,32) == 0)){
					number_a	=	cvt_bit_to_number(this_pc,32);
					number_b	=	cvt_bit_to_number(data_immi_d,13);
					number_result	=	number_a + number_b;
					delete[]	next_pc;
					next_pc		= 	cvt_number_to_bit(number_result,32);
				}
			}else if(number_step_2_opcode == 4){						//blt
				number_reg_data_a = cvt_bit_to_number(reg_data_a,32);
				number_reg_data_b = cvt_bit_to_number(reg_data_b,32);
				if(number_reg_data_a < number_reg_data_b){
					number_a	=	cvt_bit_to_number(this_pc,32);
					number_b	=	cvt_bit_to_number(data_immi_d,13);
					number_result	=	number_a + number_b;
					delete[]	next_pc;
					next_pc		= 	cvt_number_to_bit(number_result,32);
				}
			}else if(number_step_2_opcode == 5){						//bge
				number_reg_data_a = cvt_bit_to_number(reg_data_a,32);
				number_reg_data_b = cvt_bit_to_number(reg_data_b,32);
				if(number_reg_data_a >= number_reg_data_b){
					number_a	=	cvt_bit_to_number(this_pc,32);
					number_b	=	cvt_bit_to_number(data_immi_d,13);
					number_result	=	number_a + number_b;
					delete[]	next_pc;
					next_pc		= 	cvt_number_to_bit(number_result,32);
				}
			}else if(number_step_2_opcode == 6){						//bltu
				number_reg_data_a = cvt_bit_to_number_unsigned(reg_data_a,32);
				number_reg_data_b = cvt_bit_to_number_unsigned(reg_data_b,32);
				//cout<<dec<<number_reg_data_a<<" "<<number_reg_data_b<<" "<<endl;
				if(number_reg_data_a < number_reg_data_b){
					number_a	=	cvt_bit_to_number(this_pc,32);
					number_b	=	cvt_bit_to_number(data_immi_d,13);
					number_result	=	number_a + number_b;
					delete[]	next_pc;
					next_pc		= 	cvt_number_to_bit(number_result,32);
				}
			}else if(number_step_2_opcode == 7){						//bgeu
				number_reg_data_a = cvt_bit_to_number_unsigned(reg_data_a,32);
				number_reg_data_b = cvt_bit_to_number_unsigned(reg_data_b,32);
				if(number_reg_data_a >= number_reg_data_b){
					number_a	=	cvt_bit_to_number(this_pc,32);
					number_b	=	cvt_bit_to_number(data_immi_d,13);
					number_result	=	number_a + number_b;
					delete[]	next_pc;
					next_pc		= 	cvt_number_to_bit(number_result,32);
				}
			}
		}
	return	next_pc[20];	
}
