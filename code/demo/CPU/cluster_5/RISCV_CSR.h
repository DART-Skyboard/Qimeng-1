#pragma once
#include"cvt.h"
#include<ctime>
// #include"CheckInterrupt.h"
enum enum_number_csr_code {
	number_mtvec = 0x305,
	number_mepc = 0x341,
	number_mcause = 0x342,
	number_mie = 0x304,
	number_mip = 0x344,
	number_mtval = 0x343,
	number_mscratch = 0x340,
	number_mstatus = 0x300,
	number_mideleg = 0x303,
	number_medeleg = 0x302,
	number_sepc = 0x141,
	number_stvec = 0x105,
	number_scause = 0x142,
	number_sscratch = 0x140,
	number_stval = 0x143,
	number_sstatus = 0x100,
	number_sie = 0x104,
	number_sip = 0x144,
	number_satp = 0x180,
	number_mhartid = 0xf14,
	number_misa = 0x301,
	number_time = 0xc01,
	number_timeh = 0xc81,
};

void RISCV_CSR(bool input_data[bit_width], bool* output_data) {
	//get input data
	bool general_regs[1024];
	bool csr[1+0xfff][32] = {0};
	bool instruction[32];
	bool bit_this_pc[32];
	bool bit_load_data[32];
	bool this_priviledge[2];
	copy_indice(general_regs, 0, input_data, 0, 1024);
	copy_indice(csr[number_mtvec], 0, input_data, 1024, 32);
	copy_indice(csr[number_mepc], 0, input_data, 1056, 32);
	copy_indice(csr[number_mcause], 0, input_data, 1088, 32);
	copy_indice(csr[number_mie], 0, input_data, 1120, 32);
	copy_indice(csr[number_mip], 0, input_data, 1152, 32);
	copy_indice(csr[number_mtval], 0, input_data, 1184, 32);
	copy_indice(csr[number_mscratch], 0, input_data, 1216, 32);
	copy_indice(csr[number_mstatus], 0, input_data, 1248, 32);
	copy_indice(csr[number_mideleg], 0, input_data, 1280, 32);
	copy_indice(csr[number_medeleg], 0, input_data, 1312, 32);
	copy_indice(csr[number_sepc], 0, input_data, 1344, 32);
	copy_indice(csr[number_stvec], 0, input_data, 1376, 32);
	copy_indice(csr[number_scause], 0, input_data, 1408, 32);
	copy_indice(csr[number_sscratch], 0, input_data, 1440, 32);
	copy_indice(csr[number_stval], 0, input_data, 1472, 32);
	copy_indice(csr[number_sstatus], 0, input_data, 1504, 32);
	copy_indice(csr[number_sie], 0, input_data, 1536, 32);
	copy_indice(csr[number_sip], 0, input_data, 1568, 32);
	copy_indice(csr[number_satp], 0, input_data, 1600, 32);
	copy_indice(csr[number_mhartid], 0, input_data, 1632, 32);
	copy_indice(csr[number_misa], 0, input_data, 1664, 32);
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
	bool next_csr[1+0xfff][32] = {0};
	bool bit_next_pc[32];
	bool bit_load_address[32];
	bool bit_store_data[32];
	bool bit_store_address[32];
	bool bit_result_tensor[32];
	bool bit_pc_4[32];
	bool next_priviledge[2];
	copy_indice(next_general_regs, 0, general_regs, 0, 1024);
	copy_indice(next_csr[number_mtvec], 0, csr[number_mtvec], 0, 32);
	copy_indice(next_csr[number_mepc], 0, csr[number_mepc], 0, 32);
	copy_indice(next_csr[number_mcause], 0, csr[number_mcause], 0, 32);
	copy_indice(next_csr[number_mie], 0, csr[number_mie], 0, 32);
	copy_indice(next_csr[number_mip], 0, csr[number_mip], 0, 32);
	copy_indice(next_csr[number_mtval], 0, csr[number_mtval], 0, 32);
	copy_indice(next_csr[number_mscratch], 0, csr[number_mscratch], 0, 32);
	copy_indice(next_csr[number_mstatus], 0, csr[number_mstatus], 0, 32);
	copy_indice(next_csr[number_mideleg], 0, csr[number_mideleg], 0, 32);
	copy_indice(next_csr[number_medeleg], 0, csr[number_medeleg], 0, 32);
	copy_indice(next_csr[number_sepc], 0, csr[number_sepc], 0, 32);
	copy_indice(next_csr[number_stvec], 0, csr[number_stvec], 0, 32);
	copy_indice(next_csr[number_scause], 0, csr[number_scause], 0, 32);
	copy_indice(next_csr[number_sscratch], 0, csr[number_sscratch], 0, 32);
	copy_indice(next_csr[number_stval], 0, csr[number_stval], 0, 32);
	copy_indice(next_csr[number_sstatus], 0, csr[number_sstatus], 0, 32);
	copy_indice(next_csr[number_sie], 0, csr[number_sie], 0, 32);
	copy_indice(next_csr[number_sip], 0, csr[number_sip], 0, 32);
	copy_indice(next_csr[number_satp], 0, csr[number_satp], 0, 32);
	copy_indice(next_csr[number_mhartid], 0, csr[number_mhartid], 0, 32);
	copy_indice(next_csr[number_misa], 0, csr[number_misa], 0, 32);
	init_indice(bit_next_pc, 0, 32);
	init_indice(bit_load_address, 0, 32);
	init_indice(bit_store_data, 0, 32);
	init_indice(bit_store_address, 0, 32);
	init_indice(bit_result_tensor, 0, 32);
	init_indice(bit_pc_4, 0, 32);
	copy_indice(next_priviledge, 0, this_priviledge, 0, 2);

	//pc + 4
	if((which_bit_output>1695) && (which_bit_output<1728)){
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
	copy_indice(bit_csr_code, 0, instruction, 0, 12);
	uint32_t number_csr_code_unsigned = cvt_bit_to_number_unsigned(bit_csr_code, 12);
	bool	bit_funct3[3];
	copy_indice(bit_funct3, 0, instruction, 17, 3);
	uint32_t number_funct3_unsigned = cvt_bit_to_number_unsigned(bit_funct3, 3);

	//准备寄存器
	uint32_t reg_d_index = cvt_bit_to_number_unsigned(rd_code, 5);
	uint32_t reg_a_index = cvt_bit_to_number_unsigned(rs_a_code, 5);
	uint32_t reg_b_index = cvt_bit_to_number_unsigned(rs_b_code, 5);
	bool	bit_reg_data_a[32];
	bool	bit_reg_data_b[32];
	copy_indice(bit_reg_data_a, 0, input_data, 1798, 32);
	copy_indice(bit_reg_data_b, 0, input_data, 1830, 32);
	///copy_indice(bit_reg_data_a, 0, general_regs, 32*reg_a_index, 32);
	///copy_indice(bit_reg_data_b, 0, general_regs, 32*reg_b_index, 32);

	bool bit_funct5[5];
	copy_indice(bit_funct5, 0, instruction, 0, 5);
	uint32_t number_funct5_unsigned = cvt_bit_to_number_unsigned(bit_funct5, 5);

	uint32_t number_this_priviledge = cvt_bit_to_number_unsigned(this_priviledge,2);
	uint32_t number_op_code = cvt_bit_to_number_unsigned(bit_op_code,7);
	bool M_software_interrupt  = csr[number_mip][31-3] && csr[number_mie][31-3] && (csr[number_mideleg][31-3]==0) && (number_this_priviledge<3 || csr[number_mstatus][31-3]==1); //M_software_interrupt
	bool M_timer_interrupt = csr[number_mip][31-7] && csr[number_mie][31-7] && (csr[number_mideleg][31-7]==0) && (number_this_priviledge<3 || csr[number_mstatus][31-3]==1); //M_timer_interrupt
	bool M_external_interrupt = csr[number_mip][31-11] && csr[number_mie][31-11] && (csr[number_mideleg][31-11]==0) && (number_this_priviledge<3 || csr[number_mstatus][31-3]==1);
	bool S_software_interrupt = (csr[number_mip][31-3] && csr[number_mie][31-3] && csr[number_mideleg][31-3]==1 && (number_this_priviledge<3 || csr[number_mstatus][31-3]==1)) || (csr[number_mip][31-1] && csr[number_mie][31-1] && number_this_priviledge!=3 && (number_this_priviledge<1 || csr[number_mstatus][31-1]==1));
	bool S_timer_interrupt = (csr[number_mip][31-7] && csr[number_mie][31-7] && csr[number_mideleg][31-7]==1 && (number_this_priviledge<3 || csr[number_mstatus][31-3]==1)) || (csr[number_mip][31-5] && csr[number_mie][31-5] && number_this_priviledge!=3 && (number_this_priviledge<1 || csr[number_mstatus][31-1]==1));
	bool S_external_interrupt = (csr[number_mip][31-11] && csr[number_mie][31-11] && csr[number_mideleg][31-11]==1 && (number_this_priviledge<3 || csr[number_mstatus][31-3]==1)) || (csr[number_mip][31-9] && csr[number_mie][31-9] && number_this_priviledge!=3 && (number_this_priviledge<1 || csr[number_mstatus][31-1]==1));
	bool ecall = (number_op_code==0x73 && number_funct3_unsigned==0 && cvt_bit_to_number_unsigned(bit_csr_code,12)==0);

	bool MRET = (number_op_code==0x73	&&	number_funct3_unsigned==0 && (cvt_bit_to_number_unsigned(bit_csr_code,12)== 0x302));
	bool SRET = (number_op_code==0x73	&&	number_funct3_unsigned==0 && (cvt_bit_to_number_unsigned(bit_csr_code,12)== 0x102));
	bool MTrap = (M_software_interrupt 	) 
			|| 	(M_timer_interrupt 	) 
			|| 	(M_external_interrupt 	)
			||	(ecall 	&& (number_this_priviledge==0) && !csr[number_medeleg][31- 8])
			||	(ecall 	&& (number_this_priviledge==1) && !csr[number_medeleg][31- 9])
			||	(ecall 	&& (number_this_priviledge==3) && !csr[number_medeleg][31-11])
			|| 	(page_fault_inst && !csr[number_medeleg][31-12])		
			|| 	(page_fault_load && !csr[number_medeleg][31-13])		
			|| 	(page_fault_store && !csr[number_medeleg][31-15]);
	bool STrap = S_software_interrupt
			||	S_timer_interrupt
			||	S_external_interrupt
			||	(ecall 	&& (number_this_priviledge==0) && csr[number_medeleg][31- 8])
			||	(ecall 	&& (number_this_priviledge==1) && csr[number_medeleg][31- 9])
			||	(ecall 	&& (number_this_priviledge==3) && csr[number_medeleg][31-11])
			|| 	(page_fault_inst && csr[number_medeleg][31-12])		
			|| 	(page_fault_load && csr[number_medeleg][31-13])		
			|| 	(page_fault_store && csr[number_medeleg][31-15]);

	if (MTrap){
	///////////	cout << "MTrap begin" << endl;
	///////////	cout << M_software_interrupt << endl;
	///////////	cout << M_timer_interrupt << endl;
	///////////	cout << M_external_interrupt << endl;
	///////////	cout << (ecall 	&& (number_this_priviledge==0) && !csr[number_medeleg][31- 8]) << endl;
	///////////	cout << (ecall 	&& (number_this_priviledge==1) && !csr[number_medeleg][31- 9]) << endl;
	///////////	cout << (ecall 	&& (number_this_priviledge==3) && !csr[number_medeleg][31-11]) << endl;
	///////////	cout << (page_fault_inst && !csr[number_medeleg][31-12]) << endl;
	///////////	cout << (page_fault_load && !csr[number_medeleg][31-13]) << endl;
	///////////	cout << (page_fault_store && !csr[number_medeleg][31-15]) << endl;
	///////////	print_csr_regs(input_data);
	///////////	cout << "end" << endl;
		copy_indice(bit_next_pc, 0, csr[number_mtvec], 0, 32);
		copy_indice(next_csr[number_mepc], 0, bit_this_pc, 0, 32);
		//next_mcause = interruptType;
		bool bit_interruptType[32] = {0};
		bit_interruptType[31-31] = M_software_interrupt || M_timer_interrupt || M_external_interrupt;
		uint32_t number_interruptType = cvt_bit_to_number_unsigned(bit_interruptType,32);
		number_interruptType += M_software_interrupt ? 3
								: M_timer_interrupt ? 7 
								: (ecall 	&& (number_this_priviledge==0) && !csr[number_medeleg][31- 8]) ? 8
								: (ecall 	&& (number_this_priviledge==1) && !csr[number_medeleg][31- 9]) ? 9
								: (M_external_interrupt ||(ecall 	&& (number_this_priviledge==3) && !csr[number_medeleg][31-11]))  ? 11 
								: (page_fault_inst && !csr[number_medeleg][31-12]) ? 12
								: (page_fault_load && !csr[number_medeleg][31-13]) ? 13
								: (page_fault_store && !csr[number_medeleg][31-15]) ? 15
								: 0 ;//给后31位赋值
		cvt_number_to_bit_unsigned(bit_interruptType, number_interruptType, 32);
		copy_indice(next_csr[number_mcause], 0, bit_interruptType, 0, 32);
		//next_mtval = 0;//要看具体实现
		copy_indice(next_csr[number_mstatus], 31-12, this_priviledge, 0, 2);//next_mstatus.MPP = this_priviledge
		next_csr[number_mstatus][31-7] = csr[number_mstatus][31-3];//next_mstatus.MPIE = mstatus.MIE;
		next_csr[number_mstatus][31-7] = 0;//next_mstatus.MIE = 0;
		if (ecall) {
			cvt_number_to_bit_unsigned(next_priviledge, 3, 2);
		}
	}
	else if(STrap){
	///////////////////////	cout << "STrap begin" << endl;
	///////////////////////	cout << S_software_interrupt << endl;
	///////////////////////	cout << S_timer_interrupt << endl;
	///////////////////////	cout << S_external_interrupt << endl;
	///////////////////////	cout << (ecall 	&& (number_this_priviledge==0) && csr[number_medeleg][31- 8]) << endl;
	///////////////////////	cout << (ecall 	&& (number_this_priviledge==1) && csr[number_medeleg][31- 9]) << endl;
	///////////////////////	cout << (ecall 	&& (number_this_priviledge==3) && csr[number_medeleg][31-11]) << endl;
	///////////////////////	cout << (page_fault_inst && csr[number_medeleg][31-12])	 << endl;
	///////////////////////	cout << (page_fault_load && csr[number_medeleg][31-13])	 << endl;
	///////////////////////	cout << (page_fault_store && csr[number_medeleg][31-15]) << endl;
	///////////////////////	print_csr_regs(input_data);
	///////////////////////	cout << "end" << endl;
		copy_indice(bit_next_pc, 0, csr[number_stvec], 0, 32);//next_pc = stvec;
		copy_indice(next_csr[number_sepc], 0, bit_this_pc, 0, 32);//next_sepc = this_pc;
		//next_scause = interruptType;
		bool bit_interruptType[32] = {0};
		bit_interruptType[31-31] = S_software_interrupt || S_timer_interrupt || S_external_interrupt;
		uint32_t number_interruptType = cvt_bit_to_number_unsigned(bit_interruptType,32);
		number_interruptType += S_software_interrupt ? 3
								: S_timer_interrupt ? 7 
								: (ecall 	&& (number_this_priviledge==0) && csr[number_medeleg][31- 8]) ? 8
								: (ecall 	&& (number_this_priviledge==1) && csr[number_medeleg][31- 9]) ? 9
								: (S_external_interrupt ||(ecall 	&& (number_this_priviledge==3) && csr[number_medeleg][31-11]))  ? 11 
								: (page_fault_inst && csr[number_medeleg][31-12]) ? 12
								: (page_fault_load && csr[number_medeleg][31-13]) ? 13
								: (page_fault_store && csr[number_medeleg][31-15]) ? 15
								: 0 ;//给后31位赋值
		cvt_number_to_bit_unsigned(bit_interruptType, number_interruptType, 32);
		copy_indice(next_csr[number_scause], 0, bit_interruptType, 0, 32);
		//next_stval = 0;//要看具体实现
		//sstatus是mstatus的子集，sstatus改变时mstatus也要变
		next_csr[number_mstatus][31-8] = (number_this_priviledge==0) ? 0 : 1; //next_mstatus.SPP = this_priviledge;
		next_csr[number_sstatus][31-8] = (number_this_priviledge==0) ? 0 : 1; //next_sstatus.SPP = this_priviledge;
		next_csr[number_mstatus][31-5] = csr[number_mstatus][31-1]; //next_mstatus.SPIE = mstatus.SIE;
		next_csr[number_sstatus][31-5] = csr[number_sstatus][31-1]; //next_sstatus.SPIE = sstatus.SIE;
		next_csr[number_mstatus][31-1] = 0; //next_mstatus.SIE = 0;
		next_csr[number_sstatus][31-1] = 0; //next_sstatus.SIE = 0;
		if (ecall) {
			cvt_number_to_bit_unsigned(next_priviledge, 1, 2);
		}
	}
	else if(MRET){
		next_csr[number_mstatus][31-3] = csr[number_mstatus][31-7]; //next_mstatus.MIE = mstatus.MPIE;
		copy_indice(next_priviledge, 0, csr[number_mstatus], 31-12, 2); //next_priviledge = mstatus.MPP;
		next_csr[number_mstatus][31-7] = 1; //next_mstatus.MPIE = 1;
		next_csr[number_mstatus][31-12] = 0; next_csr[number_mstatus][31-11] = 0;//next_mstatus.MPP = U;
		copy_indice(bit_next_pc, 0, csr[number_mepc], 0, 32); //next_pc = mepc;
	}
	else if(SRET){
		next_csr[number_mstatus][31-1] = csr[number_mstatus][31-5]; //next_mstatus.SIE = mstatus.SPIE;
		next_csr[number_sstatus][31-1] = csr[number_sstatus][31-5];//next_sstatus.SIE = sstatus.SPIE;
		next_priviledge[0]=0; next_priviledge[1] = csr[number_sstatus][31-8];//next_priviledge = sstatus.SPP;
		next_csr[number_mstatus][31-5] = 1; //next_mstatus.SPIE = 1;
		next_csr[number_sstatus][31-5] = 1; //next_sstatus.SPIE = 1;
		next_csr[number_mstatus][31-8] = 0;  //next_mstatus.SPP = U;
		next_csr[number_sstatus][31-8] = 0;  //next_sstatus.SPP = U;
		copy_indice(bit_next_pc, 0, csr[number_sepc], 0, 32); //next_pc = sepc;
	}
	/*
	#include"CheckInterrupt.h"
	if(CheckInterrupt(inputdata)){//进入中断
		//判断中断类型和要进入的运行态
		int interruptType;
		int targetState;
		for(int i=0;i<32;i++){
			if(mip[i]==1 && mie[i]==1){
                if(mideleg[i] == 1){//应该交给S态处理
                    if(SIE == 1){//if((cur_run_state == S && SIE == 1)||cur_run_state==U)
                    	interruptType = 31-i;
						targetState = S;
					}
                }
                else{//应该交给M态处理
                    if(MIE == 1){//if((cur_run_state == M && MIE == 1)||cur_run_state!=M)
						interruptType = 31-i;
						targetState = M;
					}
                }
			}
		}
		if(targetState == M){
			next_pc = mtvec;
			next_mepc = this_pc;
			next_mcause = interruptType;
			next_mtval = 0;//要看具体实现
			next_mstatus.MPP = cur_run_state;
			next_mstatus.MPIE = mstatus.MIE;
			next_mstatus.MIE = 0;
		}
		else if(targetState == S){
			next_pc = stvec;
			next_sepc = this_pc;
			next_scause = interruptType;
			next_stval = 0;//要看具体实现
			next_mstatus.SPP = cur_run_state;
			next_mstatus.SPIE = mstatus.SIE;
			next_mstatus.SIE = 0;
			next_sstatus.SPP = cur_run_state;
			next_sstatus.SPIE = sstatus.SIE;
			next_sstatus.SIE = 0;
		}
	}
	else if(Exception){//发生异常
		int exceptionType;
		int targetState;
		//exceptionType = 和异常信号相关
		if(medeleg[31-exceptionType] == 1){//应该交给S态处理
			if(SIE == 1){//if((cur_run_state == S && SIE == 1)||cur_run_state==U)
				targetState = S;
			}
		}
		else{//应该交给M态处理
			if(MIE == 1){//if((cur_run_state == M && MIE == 1)||cur_run_state!=M)
				targetState = M;
			}
		}
		if(targetState == M){
			next_pc = mtvec;
			next_mepc = this_pc; 
			next_mcause = exceptionType;
			next_mtval = 0;//要看具体实现
			next_mstatus.MPP = cur_run_state;
			next_mstatus.MPIE = mstatus.MIE;
			next_mstatus.MIE = 0;
		}
		else if(targetState == S){
			next_pc = stvec;
			next_sepc = this_pc;
			next_scause = exceptionType;
			next_stval = 0;//要看具体实现
			next_mstatus.SPP = cur_run_state;
			next_mstatus.SPIE = mstatus.SIE;
			next_mstatus.SIE = 0;
			next_sstatus.SPP = cur_run_state;
			next_sstatus.SPIE = sstatus.SIE;
			next_sstatus.SIE = 0;
		}
	}
	else if(MRET){//MRET，条件是imm[11:0]=0011000 00010
		next_mstatus.MIE = mstatus.MPIE;
		next_run_state = mstatus.MPP;
		next_mstatus.MPIE = 1;
		next_mstatus.MPP = U;
		next_pc = mepc;
	}
	else if(SRET){//SRET，条件是imm[11:0]=0001000 00010
		next_mstatus.SIE = mstatus.SPIE;
		next_run_state = mstatus.SPP;
		next_mstatus.SPIE = 1;
		next_mstatus.SPP = U;
		next_sstatus.SIE = sstatus.SPIE;
		next_run_state = sstatus.SPP;
		next_sstatus.SPIE = 1;
		next_sstatus.SPP = U;
		next_pc = sepc;
	}*/
    /*if(asy) {
		int	next_reg_mcause_number = 11;
		bool* next_reg_mcause_cvt = new bool [32];
		cvt_number_to_bit(next_reg_mcause_cvt, next_reg_mcause_number,32);
		copy_indice(next_csr[number_mcause], 0, next_reg_mcause_cvt, 0, 32);
		next_csr[number_mcause][0] =1;
		copy_indice(bit_next_pc, 0, csr[number_mtvec], 0, 32);
		copy_indice(next_csr[number_mepc], 0, bit_this_pc, 0, 32);
		delete[]	next_reg_mcause_cvt;
        ;
    }
    else if(page_fault_inst || page_fault_load || page_fault_store) {
        ;
    }*/
	else if (
			number_csr_code_unsigned != number_mtvec &&
			number_csr_code_unsigned != number_mepc &&
			number_csr_code_unsigned != number_mcause &&
			number_csr_code_unsigned != number_mie &&
			number_csr_code_unsigned != number_mip &&
			number_csr_code_unsigned != number_mtval &&
			number_csr_code_unsigned != number_mscratch &&
			number_csr_code_unsigned != number_mstatus &&
			number_csr_code_unsigned != number_mideleg &&
			number_csr_code_unsigned != number_medeleg &&
			number_csr_code_unsigned != number_sepc &&
			number_csr_code_unsigned != number_stvec &&
			number_csr_code_unsigned != number_scause &&
			number_csr_code_unsigned != number_sscratch &&
			number_csr_code_unsigned != number_stval &&
			number_csr_code_unsigned != number_sstatus &&
			number_csr_code_unsigned != number_sie &&
			number_csr_code_unsigned != number_sip &&
			number_csr_code_unsigned != number_satp &&
			number_csr_code_unsigned != number_mhartid &&
			number_csr_code_unsigned != number_misa 
		////	&&
		////	number_csr_code_unsigned != number_time &&
		////	number_csr_code_unsigned != number_timeh 
			) {
		;
	}
    else {
		bool bit_csr_data[32];
		copy_indice(bit_csr_data, 0, input_data, 1862, 32);
		//copy_indice(bit_csr_data, 0, csr[number_csr_code_unsigned], 0, 32);
		if (number_csr_code_unsigned == number_time) {
			//uint32_t now = time(0);
			//cvt_number_to_bit_unsigned(bit_csr_data, now, 32);
		}
		if (number_csr_code_unsigned == number_timeh) {
			//uint32_t now = time(0);
			//init_indice(bit_csr_data, 0, 32);
		}

		bool bit_csr_data_result[32];
		init_indice(bit_csr_data_result, 0, 32);
		switch(number_funct3_unsigned) {
			case 0: { //ecall, ebreak
				bool bit_immi_s_type[12]; // S-type
				copy_indice(bit_immi_s_type, 0, instruction, 0, 7);
				copy_indice(bit_immi_s_type, 7, instruction, 20, 5);
				uint32_t number_immi_s_type_unsigned = cvt_bit_to_number_unsigned(bit_immi_s_type, 12);
				if (number_immi_s_type_unsigned==0b100100000) {
					//if (log) cout << "sfence.vma" << endl;
					break;
				}
				if((!instruction[2]) && (!instruction[3])){
					int	next_reg_mcause_number = 11;
					cvt_number_to_bit(next_csr[number_mcause], next_reg_mcause_number,32);
					copy_indice(bit_next_pc, 0, csr[number_mtvec], 0, 32);
					copy_indice(next_csr[number_mepc], 0, bit_this_pc, 0, 32);
				}else{
					copy_indice(bit_next_pc, 0, csr[number_mepc], 0, 32);
					init_indice(next_csr[number_mcause], 0, 32);
				}
				break;
			}
			case 1: { //csrrw
				copy_indice(bit_csr_data_result, 0, bit_reg_data_a, 0, 32);
				copy_indice(next_general_regs, reg_d_index*32, bit_csr_data, 0, 32);
				break;
			}
			case 2: { //csrrs
				for(int i = 0; i < 32; i++)
					bit_csr_data_result[i] = bit_csr_data[i] | bit_reg_data_a[i];
				copy_indice(next_general_regs, reg_d_index*32, bit_csr_data, 0, 32);
				break;
			}
			case 3: { //csrrc
				for(int i = 0; i < 32; i++)
					bit_csr_data_result[i] = bit_csr_data[i] & ~bit_reg_data_a[i];
				copy_indice(next_general_regs, reg_d_index*32, bit_csr_data, 0, 32);
				break;
			}
			case 5: { //csrrwi
				copy_indice(bit_csr_data_result, 27, rs_a_code, 0, 5);
				copy_indice(next_general_regs, reg_d_index*32, bit_csr_data, 0, 32);
				break;
			}
			case 6: { //csrrsi
				copy_indice(bit_csr_data_result, 0, bit_csr_data, 0, 27);
				for(int i = 0; i < 5; i++)
					bit_csr_data_result[i+27] = bit_csr_data[i+27] | rs_a_code[i];
				copy_indice(next_general_regs, reg_d_index*32, bit_csr_data, 0, 32);
				break;
			}
			case 7: { //csrrci
				copy_indice(bit_csr_data_result, 0, bit_csr_data, 0, 27);
				for(int i = 0; i < 5; i++)
					bit_csr_data_result[i+27] = bit_csr_data[i+27] & ~rs_a_code[i];
				copy_indice(next_general_regs, reg_d_index*32, bit_csr_data, 0, 32);
				break;
			}
		}
		copy_indice(next_csr[number_csr_code_unsigned], 0, bit_csr_data_result, 0, 32);
	}

	init_indice(next_general_regs, 0, 32);
	copy_indice(output_data, 0, next_general_regs, 0, 1024);
	copy_indice(output_data, 1024, next_csr[number_mtvec], 0, 32);
	copy_indice(output_data, 1056, next_csr[number_mepc], 0, 32);
	copy_indice(output_data, 1088, next_csr[number_mcause], 0, 32);
	copy_indice(output_data, 1120, next_csr[number_mie], 0, 32);
	copy_indice(output_data, 1152, next_csr[number_mip], 0, 32);
	copy_indice(output_data, 1184, next_csr[number_mtval], 0, 32);
	copy_indice(output_data, 1216, next_csr[number_mscratch], 0, 32);
	copy_indice(output_data, 1248, next_csr[number_mstatus], 0, 32);
	copy_indice(output_data, 1280, next_csr[number_mideleg], 0, 32);
	copy_indice(output_data, 1312, next_csr[number_medeleg], 0, 32);
	copy_indice(output_data, 1344, next_csr[number_sepc], 0, 32);
	copy_indice(output_data, 1376, next_csr[number_stvec], 0, 32);
	copy_indice(output_data, 1408, next_csr[number_scause], 0, 32);
	copy_indice(output_data, 1440, next_csr[number_sscratch], 0, 32);
	copy_indice(output_data, 1472, next_csr[number_stval], 0, 32);
	copy_indice(output_data, 1504, next_csr[number_sstatus], 0, 32);
	copy_indice(output_data, 1536, next_csr[number_sie	], 0, 32);
	copy_indice(output_data, 1568, next_csr[number_sip	], 0, 32);
	copy_indice(output_data, 1600, next_csr[number_satp], 0, 32);
	copy_indice(output_data, 1632, next_csr[number_mhartid], 0, 32);
	copy_indice(output_data, 1664, next_csr[number_misa], 0, 32);
	copy_indice(output_data, 1696, bit_next_pc, 0, 32);
	copy_indice(output_data, 1728, bit_load_address, 0, 32);
	copy_indice(output_data, 1760, bit_store_data, 0, 32);
	copy_indice(output_data, 1792, bit_store_address, 0, 32);
	copy_indice(output_data, 1824, next_priviledge, 0, 2);
}
