#pragma once
#include"cvt.h"
enum enum_number_opcode {
	number_0_opcode_lui = 0b0110111, //lui
	number_1_opcode_auipc = 0b0010111, //auipc
	number_2_opcode_jal = 0b1101111,  //jal
	number_3_opcode_jalr = 0b1100111, //jalr
	number_4_opcode_beq = 0b1100011, //beq, bne, blt, bge, bltu, bgeu
	number_5_opcode_lb = 0b0000011, //lb, lh, lw, lbu, lhu
	number_6_opcode_sb = 0b0100011, //sb, sh, sw
	number_7_opcode_addi = 0b0010011, //addi, slti, sltiu, xori, ori, andi, slli, srli, srai
	number_8_opcode_add = 0b0110011, //add, sub, sll, slt, sltu, xor, srl, sra, or, and
	number_9_opcode_fence = 0b0001111, //fence, fence.i
	number_10_opcode_ecall = 0b1110011, //ecall, ebreak, csrrw, csrrs, csrrc, csrrwi, csrrsi, csrrci
	number_11_opcode_lrw = 0b0101111, //lr.w, sc.w, amoswap.w, amoadd.w, amoxor.w, amoand.w, amoor.w, amomin.w, amomax.w, amominu.w, amomaxu.w
};
	

void RISCV_32I(bool input_data[bit_width], bool* output_data) {
	
	bool general_regs	[1024];
	bool reg_csrs[32*21];
	bool instruction[32];
	bool bit_this_pc[32];
	bool bit_load_data[32];
	bool this_priviledge[2];

	//get input data
	copy_indice(general_regs, 0, input_data, 0, 1024);
	copy_indice(reg_csrs, 0, input_data, 1024, 32*21);
	copy_indice(instruction, 0, input_data, 1696, 32);
	copy_indice(bit_this_pc, 0, input_data, 1728, 32);
	copy_indice(bit_load_data, 0, input_data, 1760, 32);
	bool asy = input_data[1792];
	bool page_fault_inst = input_data[1793];
	bool page_fault_load = input_data[1794];
	bool page_fault_store = input_data[1795];
	copy_indice(this_priviledge, 0, input_data, 1796, 2);

	//initialize output data
	bool next_general_regs[1024];
	bool next_reg_csrs[32*21];
	bool bit_next_pc[32];
	bool bit_load_address[32];
	bool bit_store_data[32];
	bool bit_store_address[32];
	bool bit_result_tensor[32];
	bool bit_pc_4[32];
	bool next_priviledge[2];
	copy_indice(next_general_regs, 0, general_regs, 0, 1024);
	copy_indice(next_reg_csrs, 0, input_data, 1024, 32*21);
	init_indice(bit_next_pc, 0, 32);
	init_indice(bit_load_address, 0, 32);
	init_indice(bit_store_data, 0, 32);
	init_indice(bit_store_address, 0, 32);
	init_indice(bit_result_tensor, 0, 32);
	init_indice(bit_pc_4, 0, 32);
	copy_indice(next_priviledge, 0, this_priviledge, 0, 2);

	//pc + 4
	if((which_bit_output<1024)){
		uint32_t number_pc_unsigned = cvt_bit_to_number_unsigned(bit_this_pc, 32);
		uint32_t number_pc_4 = number_pc_unsigned + 4;
		cvt_number_to_bit_unsigned(bit_this_pc, number_pc_unsigned, 32);
		cvt_number_to_bit_unsigned(bit_pc_4, number_pc_4, 32);
		copy_indice(bit_next_pc, 0, bit_pc_4, 0, 32);
	}else{
		uint32_t number_pc_unsigned = cvt_bit_to_number_unsigned(bit_this_pc, 32);
		uint32_t number_pc_4 = number_pc_unsigned + 4;
		cvt_number_to_bit_unsigned(bit_pc_4, number_pc_4, 32);
		copy_indice(bit_next_pc, 0, bit_pc_4, 0, 32);
	}
	//split instruction
	bool bit_op_code[7]; //25-31
	bool rd_code[5]; //20-24
	bool rs_a_code[5]; //12-16
	bool rs_b_code[5]; //7-11
	bool bit_csr_code[12]; //0-11
	copy_indice(bit_op_code, 0, instruction, 25, 7);
	copy_indice(rd_code, 0, instruction, 20, 5);
	copy_indice(rs_a_code, 0, instruction, 12, 5);
	copy_indice(rs_b_code, 0, instruction, 7, 5);
	//准备opcode、funct3、funct7
	uint32_t number_op_code_unsigned = cvt_bit_to_number_unsigned(bit_op_code, 7);
	bool	bit_funct3[3];
	copy_indice(bit_funct3, 0, instruction, 17, 3);
	uint32_t number_funct3_unsigned = cvt_bit_to_number_unsigned(bit_funct3, 3);
	bool	bit_funct7[7];
	copy_indice(bit_funct7, 0, instruction, 0, 7);
	uint32_t number_funct7_unsigned = cvt_bit_to_number_unsigned(bit_funct7, 7);
	//准备寄存器
	uint32_t reg_d_index = cvt_bit_to_number_unsigned(rd_code, 5);
	uint32_t reg_a_index = cvt_bit_to_number_unsigned(rs_a_code, 5);
	uint32_t reg_b_index = cvt_bit_to_number_unsigned(rs_b_code, 5);
	bool	bit_reg_data_a[32];
	bool	bit_reg_data_b[32];
	copy_indice(bit_reg_data_a, 0, input_data, 1798, 32);
	copy_indice(bit_reg_data_b, 0, input_data, 1830, 32);



	//准备立即数
	bool bit_immi_u_type[32]; // U-type
	bool bit_immi_j_type[21]; // J-type
	bool bit_immi_i_type[12]; // I-type
	bool bit_immi_b_type[13]; // B-type
	bool bit_immi_s_type[12]; // S-type
	init_indice(bit_immi_u_type, 0, 32);
	copy_indice(bit_immi_u_type, 0, instruction, 0, 20);
	init_indice(bit_immi_j_type, 0, 21);
	bit_immi_j_type[0] = instruction[0];
	copy_indice(bit_immi_j_type, 1, instruction, 12, 8);
	bit_immi_j_type[9] = instruction[11];
	copy_indice(bit_immi_j_type, 10, instruction, 1, 10);
	copy_indice(bit_immi_i_type, 0, instruction, 0, 12);
	init_indice(bit_immi_b_type, 0, 13);
	bit_immi_b_type[0] = instruction[0];
	bit_immi_b_type[1] = instruction[24];
	copy_indice(bit_immi_b_type, 2, instruction, 1, 6);
	copy_indice(bit_immi_b_type, 8, instruction, 20, 4);
	copy_indice(bit_immi_s_type, 0, instruction, 0, 7);
	copy_indice(bit_immi_s_type, 7, instruction, 20, 5);

	switch(number_op_code_unsigned) {
	////////////////////	case number_0_opcode_lui: { //lui
	////////////////////		copy_indice(next_general_regs, reg_d_index*32, bit_immi_u_type, 0, 32);
	////////////////////		break;
	////////////////////	}
	////////////////////	case number_1_opcode_auipc: { //auipc
	////////////////////		bool bit_temp[32];
	////////////////////		add_bit_list(bit_temp, bit_this_pc, bit_immi_u_type, 32);
	////////////////////		copy_indice(next_general_regs, reg_d_index*32, bit_temp, 0, 32);
	////////////////////		break;
	////////////////////	}
	////////////////////	case number_2_opcode_jal: { //jal
	////////////////////		bool bit_temp[32];
	////////////////////		sign_extend(bit_temp, 32, bit_immi_j_type, 21);
	////////////////////		//add_bit_list(bit_next_pc, bit_this_pc, bit_temp, 32);
	////////////////////		copy_indice(next_general_regs, reg_d_index*32, bit_pc_4, 0, 32);
	////////////////////		break;
	////////////////////	}
	////////////////////	case number_3_opcode_jalr: { //jalr
	////////////////////		bool bit_temp[32];
	////////////////////		sign_extend(bit_temp, 32, bit_immi_i_type, 12);
	////////////////////		//add_bit_list(bit_next_pc, bit_reg_data_a, bit_temp, 32);
	////////////////////		//bit_next_pc[31] = 0;
	////////////////////		copy_indice(next_general_regs, reg_d_index*32, bit_pc_4, 0, 32);
	////////////////////		break;
	////////////////////	}
	////////////////////	case number_4_opcode_beq: { //beq, bne, blt, bge, bltu, bgeu
	////////////////////	/////	uint32_t number_reg_data_a_unsigned = cvt_bit_to_number_unsigned(bit_reg_data_a, 32);
	////////////////////	/////	uint32_t number_reg_data_b_unsigned = cvt_bit_to_number_unsigned(bit_reg_data_b, 32);
	////////////////////	/////	int number_reg_data_a = cvt_bit_to_number(bit_reg_data_a, 32);
	////////////////////	/////	int number_reg_data_b = cvt_bit_to_number(bit_reg_data_b, 32);
	////////////////////	/////	switch(number_funct3_unsigned) {
	////////////////////	/////		case 0: { //beq
	////////////////////	/////			if(number_reg_data_a == number_reg_data_b) {
	////////////////////	/////				bool bit_temp[32];
	////////////////////	/////				sign_extend(bit_temp, 32, bit_immi_b_type, 13);
	////////////////////	/////				add_bit_list(bit_next_pc, bit_this_pc, bit_temp, 32);
	////////////////////	/////			}
	////////////////////	/////			break;
	////////////////////	/////		}
	////////////////////	/////		case 1: { //bne
	////////////////////	/////			if(number_reg_data_a != number_reg_data_b) {
	////////////////////	/////				bool bit_temp[32];
	////////////////////	/////				sign_extend(bit_temp, 32, bit_immi_b_type, 13);
	////////////////////	/////				add_bit_list(bit_next_pc, bit_this_pc, bit_temp, 32);
	////////////////////	/////			}
	////////////////////	/////			break;
	////////////////////	/////		}
	////////////////////	/////		case 4: { //blt
	////////////////////	/////			if(number_reg_data_a < number_reg_data_b) {
	////////////////////	/////				bool bit_temp[32];
	////////////////////	/////				sign_extend(bit_temp, 32, bit_immi_b_type, 13);
	////////////////////	/////				add_bit_list(bit_next_pc, bit_this_pc, bit_temp, 32);
	////////////////////	/////			}
	////////////////////	/////			break;
	////////////////////	/////		}
	////////////////////	/////		case 5: { //bge
	////////////////////	/////			if(number_reg_data_a >= number_reg_data_b) {
	////////////////////	/////				bool bit_temp[32];
	////////////////////	/////				sign_extend(bit_temp, 32, bit_immi_b_type, 13);
	////////////////////	/////				add_bit_list(bit_next_pc, bit_this_pc, bit_temp, 32);
	////////////////////	/////			}
	////////////////////	/////			break;
	////////////////////	/////		}
	////////////////////	/////		case 6: { //bltu
	////////////////////	/////			if(number_reg_data_a_unsigned < number_reg_data_b_unsigned) {
	////////////////////	/////				bool bit_temp[32];
	////////////////////	/////				sign_extend(bit_temp, 32, bit_immi_b_type, 13);
	////////////////////	/////				add_bit_list(bit_next_pc, bit_this_pc, bit_temp, 32);
	////////////////////	/////			}
	////////////////////	/////			break;
	////////////////////	/////		}
	////////////////////	/////		case 7: { //bgeu
	////////////////////	/////			if(number_reg_data_a_unsigned >= number_reg_data_b_unsigned) {
	////////////////////	/////				bool bit_temp[32];
	////////////////////	/////				sign_extend(bit_temp, 32, bit_immi_b_type, 13);
	////////////////////	/////				add_bit_list(bit_next_pc, bit_this_pc, bit_temp, 32);
	////////////////////	/////			}
	////////////////////	/////			break;
	////////////////////	/////		}
	////////////////////	/////	}
	////////////////////		break;
	////////////////////	}                                                                                                                                                                                            
	////////////////////	case number_5_opcode_lb: { //lb, lh, lw, lbu, lhu
	////////////////////		switch(number_funct3_unsigned) {
	////////////////////			case 0: { //lb
	////////////////////				bool bit_temp[32];
	////////////////////				sign_extend(bit_temp, 32, bit_immi_i_type, 12);
	////////////////////				add_bit_list(bit_load_address, bit_reg_data_a, bit_temp, 32);
	////////////////////				uint32_t pos = uint32_t(1^bit_load_address[31]) + 2*uint32_t(1^bit_load_address[30]);
	////////////////////				pos *= 8;
	////////////////////				sign_extend(bit_temp, 32, bit_load_data+pos*sizeof(bool), 8);
	////////////////////				copy_indice(next_general_regs, reg_d_index*32, bit_temp, 0, 32);
	////////////////////				break;
	////////////////////			}
	////////////////////			case 1: { //lh
	////////////////////				bool bit_temp[32];
	////////////////////				sign_extend(bit_temp, 32, bit_immi_i_type, 12);
	////////////////////				add_bit_list(bit_load_address, bit_reg_data_a, bit_temp, 32);
	////////////////////				uint32_t pos = bit_load_address[30] ? 0:16;
	////////////////////				sign_extend(bit_temp, 32, bit_load_data+pos*sizeof(bool), 16);
	////////////////////				copy_indice(next_general_regs, reg_d_index*32, bit_temp, 0, 32);
	////////////////////				break;
	////////////////////			}
	////////////////////			case 2: { //lw
	////////////////////				bool bit_temp[32];
	////////////////////				sign_extend(bit_temp, 32, bit_immi_i_type, 12);
	////////////////////				add_bit_list(bit_load_address, bit_reg_data_a, bit_temp, 32);
	////////////////////				copy_indice(next_general_regs, reg_d_index*32, bit_load_data, 0, 32);
	////////////////////				break;
	////////////////////			}
	////////////////////			case 4: { //lbu
	////////////////////				bool bit_temp[32];
	////////////////////				sign_extend(bit_temp, 32, bit_immi_i_type, 12);
	////////////////////				add_bit_list(bit_load_address, bit_reg_data_a, bit_temp, 32);
	////////////////////				uint32_t pos = uint32_t(1^bit_load_address[31]) + 2*uint32_t(1^bit_load_address[30]);
	////////////////////				pos *= 8;
	////////////////////				zero_extend(bit_temp, 32, bit_load_data+pos*sizeof(bool), 8);
	////////////////////				copy_indice(next_general_regs, reg_d_index*32, bit_temp, 0, 32);
	////////////////////				break;
	////////////////////			}
	////////////////////			case 5: { //lhu
	////////////////////				bool bit_temp[32];
	////////////////////				sign_extend(bit_temp, 32, bit_immi_i_type, 12);
	////////////////////				add_bit_list(bit_load_address, bit_reg_data_a, bit_temp, 32);
	////////////////////				uint32_t pos = bit_load_address[30] ? 0:16;
	////////////////////				zero_extend(bit_temp, 32, bit_load_data+pos*sizeof(bool), 16);
	////////////////////				copy_indice(next_general_regs, reg_d_index*32, bit_temp, 0, 32);
	////////////////////				break;
	////////////////////			}
	////////////////////		}
	////////////////////		break;
	////////////////////	}
	////////////////////	case number_6_opcode_sb: { //sb, sh, sw
	////////////////////		bool bit_temp[32];
	////////////////////		sign_extend(bit_temp, 32, bit_immi_s_type, 12);
	////////////////////		add_bit_list(bit_store_address, bit_reg_data_a, bit_temp, 32);
	////////////////////		copy_indice(bit_store_data, 0, bit_reg_data_b, 0, 32);
	////////////////////		break;
	////////////////////	}
		case number_7_opcode_addi: { //addi, slti, sltiu, xori, ori, andi, slli, srli, srai
			switch(number_funct3_unsigned) {
			////	case 0: { //addi
			////		bool bit_temp[32];
			////		sign_extend(bit_temp, 32, bit_immi_i_type, 12);
			////		add_bit_list(bit_temp, bit_reg_data_a, bit_temp, 32);
			////		copy_indice(next_general_regs, reg_d_index*32, bit_temp, 0, 32);
			////		break;
			////	}
				case 2: { //slti
					int number_reg_data_a = cvt_bit_to_number(bit_reg_data_a, 32);
					bool bit_temp[32];
					sign_extend(bit_temp, 32, bit_immi_i_type, 12);
					int number_temp = cvt_bit_to_number(bit_temp, 32);
					init_indice(next_general_regs, reg_d_index*32, 32);
					next_general_regs[reg_d_index*32+31] = number_reg_data_a < number_temp ? 1:0;
					break;
				}
				case 3: { //sltiu
					uint32_t number_reg_data_a_unsigned = cvt_bit_to_number_unsigned(bit_reg_data_a, 32);
					bool bit_temp[32];
					sign_extend(bit_temp, 32, bit_immi_i_type, 12);
					uint32_t number_temp = cvt_bit_to_number_unsigned(bit_temp, 32);
					init_indice(next_general_regs, reg_d_index*32, 32);
					next_general_regs[reg_d_index*32+31] = number_reg_data_a_unsigned < number_temp ? 1:0;
					break;
				}
			//////////////////	case 4: { //xori
			//////////////////		bool bit_temp[32];
			//////////////////		sign_extend(bit_temp, 32, bit_immi_i_type, 12);
			//////////////////		for(int i = 0; i < 32; i++)
			//////////////////			next_general_regs[reg_d_index*32+i] = bit_reg_data_a[i] ^ bit_temp[i];
			//////////////////		break;
			//////////////////	}
			//////////////////	case 6: { //ori
			//////////////////		bool bit_temp[32];
			//////////////////		sign_extend(bit_temp, 32, bit_immi_i_type, 12);
			//////////////////		for(int i = 0; i < 32; i++)
			//////////////////			next_general_regs[reg_d_index*32+i] = bit_reg_data_a[i] | bit_temp[i];
			//////////////////		break;
			//////////////////	}
			//////////////////	case 7: { //andi
			//////////////////		bool bit_temp[32];
			//////////////////		sign_extend(bit_temp, 32, bit_immi_i_type, 12);
			//////////////////		for(int i = 0; i < 32; i++)
			//////////////////			next_general_regs[reg_d_index*32+i] = bit_reg_data_a[i] & bit_temp[i];
			//////////////////		break;
			//////////////////	}
			//////////////////	case 1: { //slli
			//////////////////		if(instruction[6] == 0) {
			//////////////////			uint32_t number_temp = cvt_bit_to_number_unsigned(rs_b_code, 5);
			//////////////////			init_indice(next_general_regs, reg_d_index*32, 32);
			//////////////////			copy_indice(next_general_regs, reg_d_index*32, bit_reg_data_a, number_temp, 32-number_temp);
			//////////////////		}
			//////////////////		break;
			//////////////////	}
			//////////////////	case 5: { //srli, srai
			//////////////////		//switch(number_funct7_unsigned) {
			//////////////////		//	case 0: { //srli
			//////////////////		//		if(instruction[6] == 0) {
			//////////////////		//			uint32_t number_temp = cvt_bit_to_number_unsigned(rs_b_code, 5);
			//////////////////		//			init_indice(next_general_regs, reg_d_index*32, 32);
			//////////////////		//			copy_indice(next_general_regs, reg_d_index*32+number_temp, bit_reg_data_a, 0, 32-number_temp);
			//////////////////		//		}
			//////////////////		//		break;
			//////////////////		//	}
			//////////////////		//	case 32: { //srai
			//////////////////		//		if(instruction[6] == 0) {
			//////////////////		//			uint32_t number_temp = cvt_bit_to_number_unsigned(rs_b_code, 5);
			//////////////////		//			for(int i = 0; i < number_temp; i++)
			//////////////////		//				next_general_regs[reg_d_index*32+i] = bit_reg_data_a[0];
			//////////////////		//			copy_indice(next_general_regs, reg_d_index*32+number_temp, bit_reg_data_a, 0, 32-number_temp);
			//////////////////		//		}
			//////////////////		//		break;
			//////////////////		//	}
			//////////////////		//}
			//////////////////		if(!instruction[1]){
			//////////////////			if(instruction[6] == 0) {
			//////////////////				uint32_t number_temp = cvt_bit_to_number_unsigned(rs_b_code, 5);
			//////////////////				init_indice(next_general_regs, reg_d_index*32, 32);
			//////////////////				copy_indice(next_general_regs, reg_d_index*32+number_temp, bit_reg_data_a, 0, 32-number_temp);
			//////////////////			}
			//////////////////		}else{
			//////////////////			if(instruction[6] == 0) {
			//////////////////				uint32_t number_temp = cvt_bit_to_number_unsigned(rs_b_code, 5);
			//////////////////				for(int i = 0; i < number_temp; i++)
			//////////////////					next_general_regs[reg_d_index*32+i] = bit_reg_data_a[0];
			//////////////////				copy_indice(next_general_regs, reg_d_index*32+number_temp, bit_reg_data_a, 0, 32-number_temp);
			//////////////////			}
			//////////////////		}
			//////////////////		break;
			//////////////////	}
			}
			break;
		}
	//////////	case number_8_opcode_add: { //add, sub, sll, slt, sltu, xor, srl, sra, or, and
	//////////		switch(number_funct3_unsigned) {
	//////////			case 0: { //add, sub
	//////////				////switch(number_funct7_unsigned) {
	//////////				////	case 0: { //add
	//////////				////		bool bit_temp[32];
	//////////				////		add_bit_list(bit_temp, bit_reg_data_a, bit_reg_data_b, 32);
	//////////				////		copy_indice(next_general_regs, reg_d_index*32, bit_temp, 0, 32);
	//////////				////		break;
	//////////				////	}
	//////////				////	case 32: { //sub
	//////////				////		int number_a = cvt_bit_to_number(bit_reg_data_a, 32);
	//////////				////		int number_b = cvt_bit_to_number(bit_reg_data_b, 32);
	//////////				////		int number_temp = number_a - number_b;
	//////////				////		bool bit_temp[32];
	//////////				////		cvt_number_to_bit(bit_temp, number_temp, 32);
	//////////				////		copy_indice(next_general_regs, reg_d_index*32, bit_temp, 0, 32);
	//////////				////		break;
	//////////				////	}
	//////////				////}
	//////////				if(!instruction[1]){
	//////////					bool bit_temp[32];
	//////////					add_bit_list(bit_temp, bit_reg_data_a, bit_reg_data_b, 32);
	//////////					copy_indice(next_general_regs, reg_d_index*32, bit_temp, 0, 32);
	//////////				}else{
	//////////					int number_a = cvt_bit_to_number(bit_reg_data_a, 32);
	//////////					int number_b = cvt_bit_to_number(bit_reg_data_b, 32);
	//////////					int number_temp = number_a - number_b;
	//////////					bool bit_temp[32];
	//////////					cvt_number_to_bit(bit_temp, number_temp, 32);
	//////////					copy_indice(next_general_regs, reg_d_index*32, bit_temp, 0, 32);
	//////////				}
	//////////				break;
	//////////			}
	//////////			case 1: { //sll
	//////////				uint32_t number_temp = cvt_bit_to_number_unsigned(bit_reg_data_b+27, 5);
	//////////				init_indice(next_general_regs, reg_d_index*32, 32);
	//////////				copy_indice(next_general_regs, reg_d_index*32, bit_reg_data_a, number_temp, 32-number_temp);
	//////////				break;
	//////////			}
	//////////			case 2: { //slt
	//////////				int number_reg_data_a = cvt_bit_to_number(bit_reg_data_a, 32);
	//////////				int number_reg_data_b = cvt_bit_to_number(bit_reg_data_b, 32);
	//////////				init_indice(next_general_regs, reg_d_index*32, 32);
	//////////				next_general_regs[reg_d_index*32+31] = number_reg_data_a < number_reg_data_b ? 1:0;
	//////////				break;
	//////////			}
	//////////			case 3: { //sltu
	//////////				uint32_t number_reg_data_a_unsigned = cvt_bit_to_number_unsigned(bit_reg_data_a, 32);
	//////////				uint32_t number_reg_data_b_unsigned = cvt_bit_to_number_unsigned(bit_reg_data_b, 32);
	//////////				init_indice(next_general_regs, reg_d_index*32, 32);
	//////////				next_general_regs[reg_d_index*32+31] = number_reg_data_a_unsigned < number_reg_data_b_unsigned ? 1:0;
	//////////				break;
	//////////			}
	//////////			case 4: { //xor
	//////////				for(int i = 0; i < 32; i++)
	//////////					next_general_regs[reg_d_index*32+i] = bit_reg_data_a[i] ^ bit_reg_data_b[i];
	//////////				break;
	//////////			}
	//////////			case 5: { //srl, sra
	//////////				if(!instruction[1]) {
	//////////					uint32_t number_temp = cvt_bit_to_number_unsigned(bit_reg_data_b+27, 5);
	//////////					init_indice(next_general_regs, reg_d_index*32, 32);
	//////////					copy_indice(next_general_regs, reg_d_index*32+number_temp, bit_reg_data_a, 0, 32-number_temp);
	//////////				}else{
	//////////						uint32_t number_temp = cvt_bit_to_number_unsigned(bit_reg_data_b+27, 5);
	//////////						for(int i = 0; i < number_temp; i++)
	//////////							next_general_regs[reg_d_index*32+i] = bit_reg_data_a[0];
	//////////						copy_indice(next_general_regs, reg_d_index*32+number_temp, bit_reg_data_a, 0, 32-number_temp);
	//////////				}
	//////////				break;
	//////////			}
	//////////			case 6: { //or
	//////////				for(int i = 0; i < 32; i++)
	//////////					next_general_regs[reg_d_index*32+i] = bit_reg_data_a[i] | bit_reg_data_b[i];
	//////////				break;
	//////////			}
	//////////			case 7: { //and
	//////////				for(int i = 0; i < 32; i++)
	//////////					next_general_regs[reg_d_index*32+i] = bit_reg_data_a[i] & bit_reg_data_b[i];
	//////////				break;
	//////////			}
	//////////		}
	//////////		break;
	//////////	}
		case number_9_opcode_fence: { //fence, fence.i
			break;
		}
		default: {
			// cerr << "error"<<endl;
			break;
		}
	}

	// output data
	init_indice(next_general_regs, 0, 32);
	copy_indice(output_data, 0, next_general_regs, 0, 1024);
	copy_indice(output_data, 1024, next_reg_csrs, 0, 32*21);
	copy_indice(output_data, 1696, bit_next_pc, 0, 32);
	copy_indice(output_data, 1728, bit_load_address, 0, 32);
	copy_indice(output_data, 1760, bit_store_data, 0, 32);
	copy_indice(output_data, 1792, bit_store_address, 0, 32);
	copy_indice(output_data, 1824, next_priviledge, 0, 2);
}
