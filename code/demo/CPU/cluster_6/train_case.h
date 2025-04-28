
void train_case(int i,bool* train_case){

	int j;
	for (j=0;j<32;j++){
		//srand((int)time(0));
		train_case[j] = 0;
		//cout<<random_input_data[i][j];
	}
	bool	rs_a_code[5];
	bool	rs_b_code[5];
	for (j=0;j<5;j++){
		if(i%5==0){
			train_case[1696+12+j] = train_case[1696+7+j];
		}
		rs_a_code[j] = train_case[12+1696+j];
		rs_b_code[j] = train_case[ 7+1696+j];
	}
	int	reg_a_index = cvt_bit_to_number_unsigned(rs_a_code,5);
	int 	reg_b_index = cvt_bit_to_number_unsigned(rs_b_code,5);
	for(j=0;j<32;j++){
	////	  if(i%9==0){
	////	  	train_case[32*reg_a_index+j] = 0;
	////	  }
	////	if(i%9==1){
	////		if(j<16)
	////			train_case[32*reg_a_index+j] = 0;
	////	}if(i%9==2){
	////		if(j<16)
	////			train_case[32*reg_a_index+j] = 1;
	////	}
		train_case[1798+j] = train_case[32*reg_a_index+j];
		train_case[1830+j] = train_case[32*reg_b_index+j];

		////////////if(i%9==1){
		////////////	if(j<24)
		////////////		train_case[1798+j] = 0;
		////////////}if(i%9==2){
		////////////	if(j<16)
		////////////		train_case[1798+j] = 0;
		////////////}
	}	
		////////	bool this_is_CSR 	= 	(train_case[1721] == 1)
		////////				&&	(train_case[1722] == 1)
		////////				&&	(train_case[1723] == 1)
		////////				&&	(train_case[1724] == 0)
		////////				&&	(train_case[1725] == 0)
		////////				&&	(train_case[1726] == 1)
		////////				&&	(train_case[1727] == 1)
		////////				;
		////////	if(this_is_CSR){
		////////		int	which_csr = rand()%23;
		////////		for(j=0;j<12;j++){
		////////			train_case[j+1696] = which_csr_data[which_csr][j];
		////////		}
		////////		for(j=0;j<32;j++){
		////////			train_case[j+1862] = train_case[which_csr*32+1024+j];
		////////		}
		////////	}
	if((i%2)!=0){
		if(which_bit_output<1024){
			for(j=0;j<5;j++)
				train_case[20+1696+j] = which_bit_output_general_reg_index[j];
		}
	/////	if(i%10==1){
	/////		for(j=0;j<32;j++){
	/////			if(j<8)
	/////				train_case[1830+j] = train_case[1798+j];
	/////		}
	/////	}	
		if((which_bit_output<1024)||(which_bit_output>(32*53-1))){
			int	which_op_code = rand()%12;
			for(j=0;j<7;j++){
				train_case[j+1721] = which_op_code_data[4][j];
			}
			
			bool this_is_CSR 	= 	(train_case[1721] == 1)
						&&	(train_case[1722] == 1)
						&&	(train_case[1723] == 1)
						&&	(train_case[1724] == 0)
						&&	(train_case[1725] == 0)
						&&	(train_case[1726] == 1)
						&&	(train_case[1727] == 1)
						;
			if(this_is_CSR){
				int	which_csr = rand()%23;
				for(j=0;j<12;j++){
					train_case[j+1696] = which_csr_data[which_csr][j];
				}
				for(j=0;j<32;j++){
					train_case[j+1862] = train_case[which_csr*32+1024+j];
				}
			}
			///bool op_code_3_slli 	= 	(train_case[1721] == 0)
			///			&&	(train_case[1722] == 0)
			///			&&	(train_case[1723] == 1)
			///			&&	(train_case[1724] == 0)
			///			&&	(train_case[1725] == 0)
			///			&&	(train_case[1726] == 1)
			///			&&	(train_case[1727] == 1)
			///			&&	(train_case[1714] == 0)
			///			&&	(train_case[1715] == 1)
			///			;
			///bool op_code_3_add 	= 	(train_case[1721] == 0)
			///			&&	(train_case[1722] == 1)
			///			&&	(train_case[1723] == 1)
			///			&&	(train_case[1724] == 0)
			///			&&	(train_case[1725] == 0)
			///			&&	(train_case[1726] == 1)
			///			&&	(train_case[1727] == 1)
			///			;
			///if(op_code_3_slli || op_code_3_add){
			///	train_case[1696] = 0;
			///	train_case[1698] = 0;
			///	train_case[1699] = 0;
			///	train_case[1700] = 0;
			///	train_case[1701] = 0;
			///	train_case[1702] = 0;
			///}
		}
		else{
			int	which_op_code 	= rand()%12;
			int	which_op_code_a = rand()%2;
			if(which_op_code_a==1){
				for(j=0;j<7;j++){
					train_case[j+1721] = which_op_code_data[which_op_code][j];
				}
			}else{
				for(j=0;j<7;j++){
					train_case[j+1721] = which_op_code_data[10][j];
				}
			}
			bool this_is_CSR 	= 	(train_case[1721] == 1)
						&&	(train_case[1722] == 1)
						&&	(train_case[1723] == 1)
						&&	(train_case[1724] == 0)
						&&	(train_case[1725] == 0)
						&&	(train_case[1726] == 1)
						&&	(train_case[1727] == 1)
						;
			if(this_is_CSR){
				int	which_csr 	= rand()%23;
				int	which_csr_a 	= rand()%2;
				if(which_csr_a==1){
					for(j=0;j<12;j++){
						train_case[j+1696] = which_csr_data[which_csr][j];
					}
				}else{
					for(j=0;j<12;j++){
						train_case[j+1696] = which_csr_data[int(int(which_bit_output-1024)/32)][j];
					}
				}
				
			}
			///////bool op_code_3_slli 	= 	(train_case[1721] == 0)
			///////			&&	(train_case[1722] == 0)
			///////			&&	(train_case[1723] == 1)
			///////			&&	(train_case[1724] == 0)
			///////			&&	(train_case[1725] == 0)
			///////			&&	(train_case[1726] == 1)
			///////			&&	(train_case[1727] == 1)
			///////			&&	(train_case[1714] == 0)
			///////			&&	(train_case[1715] == 1)
			///////			;
			///////bool op_code_3_add 	= 	(train_case[1721] == 0)
			///////			&&	(train_case[1722] == 1)
			///////			&&	(train_case[1723] == 1)
			///////			&&	(train_case[1724] == 0)
			///////			&&	(train_case[1725] == 0)
			///////			&&	(train_case[1726] == 1)
			///////			&&	(train_case[1727] == 1)
			///////			;
			///////if(op_code_3_slli || op_code_3_add){
			///////	train_case[1696] = 0;
			///////	train_case[1698] = 0;
			///////	train_case[1699] = 0;
			///////	train_case[1700] = 0;
			///////	train_case[1701] = 0;
			///////	train_case[1702] = 0;
			///////}
			
		}
	}
}
