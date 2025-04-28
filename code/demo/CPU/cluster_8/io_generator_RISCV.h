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
#include"RISCV_V2C.h"
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

bool io_generator_RISCV(bool input_data[bit_width],int which_bit_output) {
////////////////////////////////////////////////////////	bool output_data[1826];
////////////////////////////////////////////////////////	//RISCV_input_flip(input_data, 1798+64);
////////////////////////////////////////////////////////	// for (i=0;i<1024;i++){
////////////////////////////////////////////////////////	// 	general_regs[i] = input_data[i];
////////////////////////////////////////////////////////	// }
////////////////////////////////////////////////////////	//cout<<"指令为：";
////////////////////////////////////////////////////////	// for (i=0;i<32;i++){
////////////////////////////////////////////////////////	// 	reg_mtvec	[i] 	= input_data[1024+i];
////////////////////////////////////////////////////////	// 	reg_mepc	[i]	= input_data[1056+i];
////////////////////////////////////////////////////////	// 	reg_mcause	[i]	= input_data[1088+i];
////////////////////////////////////////////////////////	// 	reg_mie		[i]	= input_data[1120+i];
////////////////////////////////////////////////////////	// 	reg_mip		[i]	= input_data[1152+i];
////////////////////////////////////////////////////////	// 	reg_mtval	[i]	= input_data[1184+i];
////////////////////////////////////////////////////////	// 	reg_mscratch	[i]	= input_data[1216+i];
////////////////////////////////////////////////////////	// 	reg_mstatus	[i]	= input_data[1248+i];
////////////////////////////////////////////////////////	// 	reg_mideleg	[i]	= input_data[1280+i];
////////////////////////////////////////////////////////	// 	reg_medeleg	[i]	= input_data[1312+i];
////////////////////////////////////////////////////////	// 	reg_sepc	[i]	= input_data[1344+i];
////////////////////////////////////////////////////////	// 	reg_stvec	[i]	= input_data[1376+i];
////////////////////////////////////////////////////////	// 	reg_scause	[i]	= input_data[1408+i];
////////////////////////////////////////////////////////	// 	reg_sscratch	[i]	= input_data[1440+i];
////////////////////////////////////////////////////////	// 	reg_stval	[i]	= input_data[1472+i];
////////////////////////////////////////////////////////	// 	reg_sstatus	[i]	= input_data[1504+i];
////////////////////////////////////////////////////////	// 	reg_sie		[i]	= input_data[1536+i];
////////////////////////////////////////////////////////	// 	reg_sip		[i]	= input_data[1568+i];
////////////////////////////////////////////////////////	// 	reg_satp	[i]	= input_data[1600+i];
////////////////////////////////////////////////////////	// 	reg_mhartid	[i]	= input_data[1632+i];
////////////////////////////////////////////////////////	// 	reg_misa	[i]	= input_data[1664+i];
////////////////////////////////////////////////////////	// 	instruction	[i]	= input_data[1696+i];
////////////////////////////////////////////////////////	// 	this_pc		[i]	= input_data[1728+i];
////////////////////////////////////////////////////////	// 	load_data	[i]	= input_data[1760+i];
////////////////////////////////////////////////////////	// 	cout<<instruction[i];
////////////////////////////////////////////////////////	// }
////////////////////////////////////////////////////////	// cout<<endl;
	bool instruction[32];
	copy_indice(instruction, 0, input_data, 1696, 32);
	bool asy = input_data[1792];
	bool page_fault_inst = input_data[1793];
	bool page_fault_load = input_data[1794];
	bool page_fault_store = input_data[1795];
	bool bit_op_code[7];
	copy_indice(bit_op_code, 0, instruction, 25, 7);
	uint32_t number_op_code_unsigned = cvt_bit_to_number_unsigned(bit_op_code, 7);
////////////////////////////////////////////////////////	//if(number_op_code_unsigned == 0) {
////////////////////////////////////////////////////////	//	exit(-1);
////////////////////////////////////////////////////////	//}
////////////////////////////////////////////////////////
////////////////////////////////////////////////////////	if((page_fault_inst || page_fault_load || page_fault_store || number_op_code_unsigned == number_10_opcode_ecall)){	//进入中断控制程序，这里只完成了对1个中断的处理（只设置了1个mcause值）
////////////////////////////////////////////////////////		RISCV_CSR(input_data, output_data);
////////////////////////////////////////////////////////		//RISCV_32I(input_data, output_data);
////////////////////////////////////////////////////////	}else if(number_op_code_unsigned == number_11_opcode_lrw){
////////////////////////////////////////////////////////		RISCV_32A(input_data, output_data);
////////////////////////////////////////////////////////	}else{
////////////////////////////////////////////////////////		RISCV_32I(input_data, output_data);
////////////////////////////////////////////////////////	}
	/////RISCV_32I(input_data, output_data);
	bool output_data[1826];
////////////////	bool instruction[32];
////////////////	bool this_priviledge[2];
////////////////	bool csr_mie[32];
////////////////	bool csr_mip[32];
////////////////	bool csr_mstatus[32];
////////////////	bool csr_mideleg[32];
////////////////	bool csr_medeleg[32];
////////////////	bool csr_sie[32];
////////////////	bool csr_sip[32];
////////////////	bool csr_sstatus[32];
////////////////	copy_indice(csr_mie, 0, input_data, 1120, 32);
////////////////	copy_indice(csr_mip, 0, input_data, 1152, 32);
////////////////	copy_indice(csr_mstatus, 0, input_data, 1248, 32);
////////////////	copy_indice(csr_mideleg, 0, input_data, 1280, 32);
////////////////	copy_indice(csr_medeleg, 0, input_data, 1312, 32);
////////////////	copy_indice(csr_sstatus, 0, input_data, 1504, 32);
////////////////	copy_indice(csr_sie, 0, input_data, 1536, 32);
////////////////	copy_indice(csr_sip, 0, input_data, 1568, 32);
////////////////	copy_indice(instruction, 0, input_data, 1696, 32);
///////////////	bool asy = input_data[1792];
///////////////	bool page_fault_inst = input_data[1793];
///////////////	bool page_fault_load = input_data[1794];
///////////////	bool page_fault_store = input_data[1795];
////////////////	copy_indice(this_priviledge, 0, input_data, 1796, 2);
////////////////
////////////////	//split instruction
////////////////	bool bit_op_code[7]; //25-31
////////////////	bool bit_csr_code[12]; //0-11
////////////////	copy_indice(bit_op_code, 0, instruction, 25, 7);
////////////////	uint32_t number_op_code_unsigned = cvt_bit_to_number_unsigned(bit_op_code, 7);
////////////////	copy_indice(bit_csr_code, 0, instruction, 0, 12);
////////////////	uint32_t number_csr_code_unsigned = cvt_bit_to_number_unsigned(bit_csr_code, 12);
////////////////	bool	bit_funct3[3];
////////////////	copy_indice(bit_funct3, 0, instruction, 17, 3);
////////////////	uint32_t number_funct3_unsigned = cvt_bit_to_number_unsigned(bit_funct3, 3);
////////////////
////////////////	uint32_t number_this_priviledge = cvt_bit_to_number_unsigned(this_priviledge,2);
////////////////	uint32_t number_op_code = cvt_bit_to_number_unsigned(bit_op_code,7);
////////////////	bool M_software_interrupt  = csr_mip[31-3] && csr_mie[31-3] && (csr_mideleg[31-3]==0) && (number_this_priviledge<3 || csr_mstatus[31-3]==1); //M_software_interrupt
////////////////	bool M_timer_interrupt = csr_mip[31-7] && csr_mie[31-7] && (csr_mideleg[31-7]==0) && (number_this_priviledge<3 || csr_mstatus[31-3]==1); //M_timer_interrupt
////////////////	bool M_external_interrupt = csr_mip[31-11] && csr_mie[31-11] && (csr_mideleg[31-11]==0) && (number_this_priviledge<3 || csr_mstatus[31-3]==1);
////////////////	bool S_software_interrupt = (csr_mip[31-3] && csr_mie[31-3] && csr_mideleg[31-3]==1 && (number_this_priviledge<3 || csr_mstatus[31-3]==1)) || (csr_mip[31-1] && csr_mie[31-1] && number_this_priviledge!=3 && (number_this_priviledge<1 || csr_mstatus[31-1]==1));
////////////////	bool S_timer_interrupt = (csr_mip[31-7] && csr_mie[31-7] && csr_mideleg[31-7]==1 && (number_this_priviledge<3 || csr_mstatus[31-3]==1)) || (csr_mip[31-5] && csr_mie[31-5] && number_this_priviledge!=3 && (number_this_priviledge<1 || csr_mstatus[31-1]==1));
////////////////	bool S_external_interrupt = (csr_mip[31-11] && csr_mie[31-11] && csr_mideleg[31-11]==1 && (number_this_priviledge<3 || csr_mstatus[31-3]==1)) || (csr_mip[31-9] && csr_mie[31-9] && number_this_priviledge!=3 && (number_this_priviledge<1 || csr_mstatus[31-1]==1)) || (csr_sstatus[31-1]==true && csr_sie[31-9] == true && csr_sip[31-9] == true);
////////////////	bool ecall = (number_op_code==0x73 && number_funct3_unsigned==0 && cvt_bit_to_number_unsigned(bit_csr_code,12)==0);
////////////////
////////////////	bool MRET = (number_op_code==0x73	&&	number_funct3_unsigned==0 && (cvt_bit_to_number_unsigned(bit_csr_code,12)== 0x302));
////////////////	bool SRET = (number_op_code==0x73	&&	number_funct3_unsigned==0 && (cvt_bit_to_number_unsigned(bit_csr_code,12)== 0x102));
////////////////	bool MTrap = (M_software_interrupt 	) 
////////////////			|| 	(M_timer_interrupt 	) 
////////////////			|| 	(M_external_interrupt 	)
////////////////			||	(ecall 	&& (number_this_priviledge==0) && !csr_medeleg[31- 8])
////////////////			||	(ecall 	&& (number_this_priviledge==1) && !csr_medeleg[31- 9])
////////////////			||	(ecall 	&& (number_this_priviledge==3) && !csr_medeleg[31-11])
////////////////			|| 	(page_fault_inst && !csr_medeleg[31-12])		
////////////////			|| 	(page_fault_load && !csr_medeleg[31-13])		
////////////////			|| 	(page_fault_store && !csr_medeleg[31-15]);
////////////////	bool STrap = S_software_interrupt
////////////////			||	S_timer_interrupt
////////////////			||	S_external_interrupt
////////////////			||	(ecall 	&& (number_this_priviledge==0) && csr_medeleg[31- 8])
////////////////			||	(ecall 	&& (number_this_priviledge==1) && csr_medeleg[31- 9])
////////////////			||	(ecall 	&& (number_this_priviledge==3) && csr_medeleg[31-11])
////////////////			|| 	(page_fault_inst && csr_medeleg[31-12])		
////////////////			|| 	(page_fault_load && csr_medeleg[31-13])		
////////////////			|| 	(page_fault_store && csr_medeleg[31-15]);
////////////////
////////////////	asy = MTrap || STrap || MRET || SRET;
////////////////	uint32_t number_instruction = cvt_bit_to_number_unsigned(instruction, 32);
////////////////	if (number_instruction == 0x10500073 && !asy && !page_fault_inst && !page_fault_load && !page_fault_store) {
////////////////		cerr << "wfi" << endl;
////////////////		//exit(-1);
////////////////	}
////	if(asy || page_fault_inst || page_fault_load || page_fault_store || number_op_code_unsigned == number_10_opcode_ecall){	//进入中断控制程序，这里只完成了对1个中断的处理（只设置了1个mcause值）
////		RISCV_CSR(input_data, output_data);
////	}else if(number_op_code_unsigned == number_11_opcode_lrw){
////		RISCV_32A(input_data, output_data);
////	}else{
////		RISCV_32I(input_data, output_data);
////	}
//	if(number_op_code_unsigned == number_11_opcode_lrw){
//		RISCV_32A(input_data, output_data);
//	}else{
		RISCV_32I(input_data, output_data);
//	}
	bool	return_bool;
		return_bool = output_data[which_bit_output];
	return	return_bool;
	//RISCV_output_flip(output_data, 1826);
}

