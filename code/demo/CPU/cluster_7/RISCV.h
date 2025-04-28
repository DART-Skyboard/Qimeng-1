#pragma once
bool 	b_0_opcode_lui 	  [7]	= {0,1,1,0,1,1,1};
bool 	b_1_opcode_auipc  [7]	= {0,0,1,0,1,1,1};
bool 	b_2_opcode_jal 	  [7]	= {1,1,0,1,1,1,1};
bool 	b_3_opcode_jalr   [7]	= {1,1,0,0,1,1,1};
bool 	b_4_opcode_beq 	  [7]	= {1,1,0,0,0,1,1};
bool 	b_5_opcode_lb 	  [7]	= {0,0,0,0,0,1,1};
bool 	b_6_opcode_sb 	  [7]	= {0,1,0,0,0,1,1};
bool 	b_7_opcode_addi   [7]	= {0,0,1,0,0,1,1};
bool 	b_8_opcode_add 	  [7]	= {0,1,1,0,0,1,1};
bool 	b_9_opcode_fence  [7]	= {0,0,0,1,1,1,1};
bool 	b_10_opcode_ecall [7]	= {1,1,1,0,0,1,1};
bool 	b_11_opcode_32a   [7]	= {0,1,0,1,1,1,1};

// #include"RISCV_public.h"
#include"RISCV_32I.h"
#include"RISCV_32A.h"
#include"RISCV_CSR.h"
#include"cvt.h"

bool	csr_mcause	[12]		= {0,0,1,1,0,1,0,0,0,0,1,0};//342
bool	csr_mepc	[12]		= {0,0,1,1,0,1,0,0,0,0,0,1};//341
bool	csr_mstatus	[12]		= {0,0,1,1,0,0,0,0,0,0,0,0};//300
bool	csr_mtvec	[12]		= {0,0,1,1,0,0,0,0,0,1,0,1};//305
/////////////////
/////////////////bool	csr_mhartid	[12]		= {1,1,1,1,0,0,0,1,0,0,0,0};//f10
/////////////////bool	csr_mstatush	[12]		= {0,0,0,0,0,0,0,0,0,0,0,0};
/////////////////bool	csr_mip		[12]		= {0,0,1,1,0,1,0,0,0,1,0,0};//344
/////////////////bool	csr_mideleg	[12]		= {0,0,1,1,0,0,0,0,0,0,1,1};//303
/////////////////bool	csr_medeleg	[12]		= {0,0,1,1,0,0,0,0,0,0,1,0};//302
/////////////////bool	csr_stvec	[12]		= {0,0,0,1,0,0,0,0,0,1,0,1};//105
/////////////////bool	csr_sepc	[12]		= {0,0,0,1,0,1,0,0,0,0,0,1};//141
/////////////////bool	csr_scause	[12]		= {0,0,0,1,0,1,0,0,0,0,1,0};//142
/////////////////bool	csr_mtval	[12]		= {0,0,1,1,0,1,0,0,0,0,1,1};//343
/////////////////bool	csr_stval	[12]		= {0,0,0,1,0,0,0,0,0,0,1,1};//143
/////////////////bool	csr_satp	[12]		= {0,0,0,1,1,0,0,0,0,0,0,0};//180

void RISCV(bool input_data[bit_width], bool* output_data) {

	// for (i=0;i<1024;i++){
	// 	general_regs[i] = input_data[i];
	// }
	//cout<<"指令为：";
	// for (i=0;i<32;i++){
	// 	reg_mtvec	[i] 	= input_data[1024+i];
	// 	reg_mepc	[i]	= input_data[1056+i];
	// 	reg_mcause	[i]	= input_data[1088+i];
	// 	reg_mie		[i]	= input_data[1120+i];
	// 	reg_mip		[i]	= input_data[1152+i];
	// 	reg_mtval	[i]	= input_data[1184+i];
	// 	reg_mscratch	[i]	= input_data[1216+i];
	// 	reg_mstatus	[i]	= input_data[1248+i];
	// 	reg_mideleg	[i]	= input_data[1280+i];
	// 	reg_medeleg	[i]	= input_data[1312+i];
	// 	reg_sepc	[i]	= input_data[1344+i];
	// 	reg_stvec	[i]	= input_data[1376+i];
	// 	reg_scause	[i]	= input_data[1408+i];
	// 	reg_sscratch	[i]	= input_data[1440+i];
	// 	reg_stval	[i]	= input_data[1472+i];
	// 	reg_sstatus	[i]	= input_data[1504+i];
	// 	reg_sie		[i]	= input_data[1536+i];
	// 	reg_sip		[i]	= input_data[1568+i];
	// 	reg_satp	[i]	= input_data[1600+i];
	// 	reg_mhartid	[i]	= input_data[1632+i];
	// 	reg_misa	[i]	= input_data[1664+i];
	// 	instruction	[i]	= input_data[1696+i];
	// 	this_pc		[i]	= input_data[1728+i];
	// 	load_data	[i]	= input_data[1760+i];
	// 	cout<<instruction[i];
	// }
	// cout<<endl;
	bool instruction[32];
	copy_indice(instruction, 0, input_data, 1696, 32);
	bool asy = input_data[1792];
	bool page_fault_inst = input_data[1793];
	bool page_fault_load = input_data[1794];
	bool page_fault_store = input_data[1795];
	bool bit_op_code[7];
	copy_indice(bit_op_code, 0, instruction, 25, 7);
	uint32_t number_op_code_unsigned = cvt_bit_to_number_unsigned(bit_op_code, 7);
	// if(number_op_code_unsigned == 0) {
	// 	exit(-1);
	// }
	if(asy || page_fault_inst || page_fault_load || page_fault_store || number_op_code_unsigned == number_10_opcode_ecall){	//进入中断控制程序，这里只完成了对1个中断的处理（只设置了1个mcause值）
		RISCV_CSR(input_data, output_data);
	}else if(number_op_code_unsigned == number_11_opcode_lrw){
		RISCV_32A(input_data, output_data);
	}else{
		RISCV_32I(input_data, output_data);
	}

}

