int print_circuit(int which_bit_output,BDD_node** BDD,int* most_influence){

 int i,j;
 int zi;
 
 char  output_file_name[100];
 sprintf(output_file_name,"rtl_2_stage/module_output_bit_%d.v",which_bit_output);
// string  output_file_name << output_file_name_begin << which_bit_output << output_file_name_end;

      cout<<output_file_name<<endl;

      ofstream output_module_file(output_file_name);
 output_module_file << "//circuit accuracy = "<<circuit_accuracy<<endl; 
 output_module_file << "//test amounts  = "<<parameter_test_amount<<endl; 
 output_module_file << "//total BDD nodes = "<<total_nodes_amount<<endl; 
 output_module_file << "//total split modes = "<<total_split_nodes<<endl; 
 output_module_file << "//train time = "<<train_time<<endl; 
 output_module_file << "module module_output_bit_"<<which_bit_output<<"(i,o);"<<endl<<endl; 
 output_module_file << "input ["<<bit_width-1<<":0] i;"<<endl; 
 output_module_file << "output["<<pow(2,parameter_multi_output_index)-1<<":0] o;"<<endl<<endl;

const int print_module_amount = 1;
 output_module_file <<endl<< "wire ["<<pow(2,parameter_multi_output_index)-1<<":0] l_"<<parameter_multi_output_index<<";"<<endl<<endl;
 for (i=1;i<print_module_amount;i++){
  	output_module_file << "wire ["<<BDD_width_each_layer[parameter_multi_output_index+int((total_BDD_depth-parameter_multi_output_index)*i/print_module_amount)]-1<<":0] l_"<<parameter_multi_output_index+int((total_BDD_depth-parameter_multi_output_index)*i/print_module_amount)<<";"<<endl;
}
 for (i=1;i<print_module_amount+1;i++){
	if(i==print_module_amount)
  		output_module_file << "module_output_bit_"<<which_bit_output<<"_"<<i<<"	m"<<i<<"(l_"<<parameter_multi_output_index+int((total_BDD_depth-parameter_multi_output_index)*(i-1)/print_module_amount)<<",i);"<<endl;
	else
  		output_module_file << "module_output_bit_"<<which_bit_output<<"_"<<i<<"	m"<<i<<"(l_"<<parameter_multi_output_index+int((total_BDD_depth-parameter_multi_output_index)*(i-1)/print_module_amount)<<",l_"<<parameter_multi_output_index+int((total_BDD_depth-parameter_multi_output_index)*i/print_module_amount)<<",i);"<<endl;
 } 

 output_module_file <<endl<< "assign o["<<pow(2,parameter_multi_output_index)-1<<":0] = l_"<<parameter_multi_output_index<<"["<<pow(2,parameter_multi_output_index)-1<<":0];"<<endl;

 output_module_file << endl <<"endmodule"<<endl<<endl;
for (zi=1;zi<print_module_amount+1;zi++){
	if(zi==print_module_amount)
 		output_module_file << "module module_output_bit_"<<which_bit_output<<"_"<<zi<<"(l_"<<parameter_multi_output_index+int((total_BDD_depth-parameter_multi_output_index)*(zi-1)/print_module_amount)<<",i);"<<endl<<endl;
	else
 		output_module_file << "module module_output_bit_"<<which_bit_output<<"_"<<zi<<"(l_"<<parameter_multi_output_index+int((total_BDD_depth-parameter_multi_output_index)*(zi-1)/print_module_amount)<<",l_"<<parameter_multi_output_index+int((total_BDD_depth-parameter_multi_output_index)*(zi)/print_module_amount)<<",i);"<<endl<<endl;
	if(zi==print_module_amount)
		;
	else
 		output_module_file << "input ["<<BDD_width_each_layer[parameter_multi_output_index+int((total_BDD_depth-parameter_multi_output_index)*zi/print_module_amount)]-1<<":0] l_"<<parameter_multi_output_index+int((total_BDD_depth-parameter_multi_output_index)*zi/print_module_amount)<<";"<<endl; 
 	output_module_file << "input ["<<bit_width-1<<":0] i;"<<endl; 
 	output_module_file << "output ["<<BDD_width_each_layer[parameter_multi_output_index+int((total_BDD_depth-parameter_multi_output_index)*(zi-1)/print_module_amount)]-1<<":0] l_"<<parameter_multi_output_index+int((total_BDD_depth-parameter_multi_output_index)*(zi-1)/print_module_amount)<<";"<<endl; 
	for (i=parameter_multi_output_index+int((total_BDD_depth-parameter_multi_output_index)*(zi-1)/print_module_amount)+1;i<=parameter_multi_output_index+int((total_BDD_depth-parameter_multi_output_index)*zi/print_module_amount)-1;i++){
  		output_module_file << "wire ["<<BDD_width_each_layer[i]-1<<":0] l_"<<i<<";"<<endl;
	}
	for (i=parameter_multi_output_index+int((total_BDD_depth-parameter_multi_output_index)*(zi-1)/print_module_amount);i<parameter_multi_output_index+int((total_BDD_depth-parameter_multi_output_index)*zi/print_module_amount);i++){
	for (j=0;j<BDD_width_each_layer[i];j++){
	   if((BDD[i][j].left_node_index >= 0) & (BDD[i][j].right_node_index >= 0)){
		if(BDD[i][j].left_node_index == BDD[i][j].right_node_index){
			if((BDD[i][j].left_node_neg == 0) && (BDD[i][j].right_node_neg == 0)){
	    			output_module_file << "assign l_"<<i<<"["<<j<<"]    = ( l_"<<i+1<<" ["<<BDD[i][j].left_node_index<<"]);"<<endl;
			}else if((BDD[i][j].left_node_neg == 1) && (BDD[i][j].right_node_neg == 1)){
	    			output_module_file << "assign l_"<<i<<"["<<j<<"]    = (!l_"<<i+1<<" ["<<BDD[i][j].left_node_index<<"]);"<<endl;
			}else if((BDD[i][j].left_node_neg == 0) && (BDD[i][j].right_node_neg == 1)){
	    			output_module_file << "assign l_"<<i<<"["<<j<<"]    = ( l_"<<i+1<<" ["<<BDD[i][j].left_node_index<<"] ^  i["<<most_influence[i]<<"]);"<<endl;	
			}else if((BDD[i][j].left_node_neg == 1) && (BDD[i][j].right_node_neg == 0)){
	    			output_module_file << "assign l_"<<i<<"["<<j<<"]    = ( l_"<<i+1<<" ["<<BDD[i][j].left_node_index<<"] ^ !i["<<most_influence[i]<<"]);"<<endl;	
			}
		}else{
			if((BDD[i][j].left_node_neg == 0) && (BDD[i][j].right_node_neg == 0)){
	    			output_module_file << "assign l_"<<i<<"["<<j<<"]    = ( l_"<<i+1<<" ["<<BDD[i][j].left_node_index<<"] & !i["<<most_influence[i]<<"]) | ( l_"<<i+1<<" ["<<BDD[i][j].right_node_index<<"] &  i["<<most_influence[i]<<"]);"<<endl;	
			}else if((BDD[i][j].left_node_neg == 1) && (BDD[i][j].right_node_neg == 1)){
	    			output_module_file << "assign l_"<<i<<"["<<j<<"]    = (!l_"<<i+1<<" ["<<BDD[i][j].left_node_index<<"] & !i["<<most_influence[i]<<"]) | (!l_"<<i+1<<" ["<<BDD[i][j].right_node_index<<"] &  i["<<most_influence[i]<<"]);"<<endl;	
			}else if((BDD[i][j].left_node_neg == 0) && (BDD[i][j].right_node_neg == 1)){
	    			output_module_file << "assign l_"<<i<<"["<<j<<"]    = ( l_"<<i+1<<" ["<<BDD[i][j].left_node_index<<"] & !i["<<most_influence[i]<<"]) | (!l_"<<i+1<<" ["<<BDD[i][j].right_node_index<<"] &  i["<<most_influence[i]<<"]);"<<endl;	
			}else if((BDD[i][j].left_node_neg == 1) && (BDD[i][j].right_node_neg == 0)){
	    			output_module_file << "assign l_"<<i<<"["<<j<<"]    = (!l_"<<i+1<<" ["<<BDD[i][j].left_node_index<<"] & !i["<<most_influence[i]<<"]) | ( l_"<<i+1<<" ["<<BDD[i][j].right_node_index<<"] &  i["<<most_influence[i]<<"]);"<<endl;	
			}
		}
	   }else if((BDD[i][j].left_node_index == -1) & (BDD[i][j].right_node_index >= 0)){
		if(BDD[i][j].right_node_neg == 0){
	    		output_module_file << "assign l_"<<i<<"["<<j<<"]    = (!i["<<most_influence[i]<<"]) | ( l_"<<i+1<<" ["<<BDD[i][j].right_node_index<<"] &  i["<<most_influence[i]<<"]);"<<endl;
		}else{
	    		output_module_file << "assign l_"<<i<<"["<<j<<"]    = (!i["<<most_influence[i]<<"]) | (!l_"<<i+1<<" ["<<BDD[i][j].right_node_index<<"] &  i["<<most_influence[i]<<"]);"<<endl;
		}
	   }else if((BDD[i][j].left_node_index == -2) & (BDD[i][j].right_node_index >= 0)){
		if(BDD[i][j].right_node_neg == 0){
	    		output_module_file << "assign l_"<<i<<"["<<j<<"]    = ( l_"<<i+1<<" ["<<BDD[i][j].right_node_index<<"] &  i["<<most_influence[i]<<"]);"<<endl;
		}else{
	    		output_module_file << "assign l_"<<i<<"["<<j<<"]    = (!l_"<<i+1<<" ["<<BDD[i][j].right_node_index<<"] &  i["<<most_influence[i]<<"]);"<<endl;
		}
	   }else if((BDD[i][j].left_node_index >= 0) & (BDD[i][j].right_node_index == -1 )){
		if(BDD[i][j].left_node_neg == 0){
	    		output_module_file << "assign l_"<<i<<"["<<j<<"]    = ( l_"<<i+1<<" ["<<BDD[i][j].left_node_index<<"] & !i["<<most_influence[i]<<"]) | (      i["<<most_influence[i]<<"]);"<<endl;
		}else{
	    		output_module_file << "assign l_"<<i<<"["<<j<<"]    = (!l_"<<i+1<<" ["<<BDD[i][j].left_node_index<<"] & !i["<<most_influence[i]<<"]) | (      i["<<most_influence[i]<<"]);"<<endl;
		}
	   }else if((BDD[i][j].left_node_index >= 0) & (BDD[i][j].right_node_index == -2)){
		if(BDD[i][j].right_node_neg == 0){
	    		output_module_file << "assign l_"<<i<<"["<<j<<"]    = ( l_"<<i+1<<" ["<<BDD[i][j].left_node_index<<"] & !i["<<most_influence[i]<<"]);"<<endl;
		}else{
	    		output_module_file << "assign l_"<<i<<"["<<j<<"]    = (!l_"<<i+1<<" ["<<BDD[i][j].left_node_index<<"] & !i["<<most_influence[i]<<"]);"<<endl;
		}
	   }else if((BDD[i][j].left_node_index == -1) & (BDD[i][j].right_node_index == -1)){
	    	output_module_file << "assign l_"<<i<<"["<<j<<"]    = 1;"<<endl;
	   }else if((BDD[i][j].left_node_index == -2) & (BDD[i][j].right_node_index == -2)){
	    	output_module_file << "assign l_"<<i<<"["<<j<<"]    = 0;"<<endl;
	   }else if((BDD[i][j].left_node_index == -1) & (BDD[i][j].right_node_index == -2)){
	    	output_module_file << "assign l_"<<i<<"["<<j<<"]    = !i["<<most_influence[i]<<"];"<<endl;
	   }else if((BDD[i][j].left_node_index == -2) & (BDD[i][j].right_node_index == -1)){
	    	output_module_file << "assign l_"<<i<<"["<<j<<"]    =  i["<<most_influence[i]<<"];"<<endl;
	   }
	  }
	 }
 
	output_module_file << endl <<"endmodule"<<endl<<endl;

 }

 











//	output_module_file << "//circuit accuracy	= "<<circuit_accuracy<<endl;	
//	output_module_file << "//test amounts		= "<<parameter_test_amount<<endl;	
//	output_module_file << "//total BDD nodes	= "<<total_nodes_amount<<endl;	
//	output_module_file << "module module_output_bit_"<<which_bit_output<<"(input_data,output_bit);"<<endl<<endl;	
//	output_module_file << "input	["<<bit_width-1<<":0]	input_data;"<<endl;	
//	output_module_file << "output		output_bit;"<<endl<<endl;
//
//
//	for (i=0;i<total_BDD_depth+1;i++){
//		output_module_file << "wire	["<<BDD_width_each_layer[i]-1<<":0]	layer_"<<i<<";"<<endl;
//	}	
//	output_module_file <<endl<< "assign	output_bit = layer_0[0];"<<endl<<endl;
//	for (i=0;i<total_BDD_depth+1;i++){
//		for (j=0;j<BDD_width_each_layer[i];j++){
//			if((BDD[i][j].left_node_index >= 0) & (BDD[i][j].right_node_index >= 0)){
//				output_module_file << "assign	layer_"<<i<<"["<<j<<"]   	= (layer_"<<i+1<<"	["<<BDD[i][j].left_node_index<<"]	& !input_data["<<most_influence[i]<<"])	| (layer_"<<i+1<<"	["<<BDD[i][j].right_node_index<<"]	&  input_data["<<most_influence[i]<<"]);"<<endl;
//			}else if((BDD[i][j].left_node_index == -1) & (BDD[i][j].right_node_index >= 0)){
//				output_module_file << "assign	layer_"<<i<<"["<<j<<"]   	= (			  !input_data["<<most_influence[i]<<"])	| (layer_"<<i+1<<"	["<<BDD[i][j].right_node_index<<"]	&  input_data["<<most_influence[i]<<"]);"<<endl;
//			}else if((BDD[i][j].left_node_index == -2) & (BDD[i][j].right_node_index >= 0)){
//				output_module_file << "assign	layer_"<<i<<"["<<j<<"]   	= (layer_"<<i+1<<"	["<<BDD[i][j].right_node_index<<"]	&  input_data["<<most_influence[i]<<"]);"<<endl;
//			}else if((BDD[i][j].left_node_index >= 0) & (BDD[i][j].right_node_index == -1 )){
//				output_module_file << "assign	layer_"<<i<<"["<<j<<"]   	= (layer_"<<i+1<<"	["<<BDD[i][j].left_node_index<<"]	& !input_data["<<most_influence[i]<<"])	| (			   input_data["<<most_influence[i]<<"]);"<<endl;
//			}else if((BDD[i][j].left_node_index >= 0) & (BDD[i][j].right_node_index == -2)){
//				output_module_file << "assign	layer_"<<i<<"["<<j<<"]   	= (layer_"<<i+1<<"	["<<BDD[i][j].left_node_index<<"]	& !input_data["<<most_influence[i]<<"]);"<<endl;
//			}else if((BDD[i][j].left_node_index == -1) & (BDD[i][j].right_node_index == -1)){
//				output_module_file << "assign	layer_"<<i<<"["<<j<<"]   	= 1;"<<endl;
//			}else if((BDD[i][j].left_node_index == -2) & (BDD[i][j].right_node_index == -2)){
//				output_module_file << "assign	layer_"<<i<<"["<<j<<"]   	= 0;"<<endl;
//			}else if((BDD[i][j].left_node_index == -1) & (BDD[i][j].right_node_index == -2)){
//				output_module_file << "assign	layer_"<<i<<"["<<j<<"]   	= !input_data["<<most_influence[i]<<"];"<<endl;
//			}else if((BDD[i][j].left_node_index == -2) & (BDD[i][j].right_node_index == -1)){
//				output_module_file << "assign	layer_"<<i<<"["<<j<<"]   	=  input_data["<<most_influence[i]<<"];"<<endl;
//			}
//		}
//	}
//	
//	output_module_file << endl <<"endmodule"<<endl;

	return 0 ;
};
