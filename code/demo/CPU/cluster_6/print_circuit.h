
int print_circuit(int which_bit_output,BDD_node** BDD,int* most_influence){
 int i,j;
 int zi;
 
 char  output_file_name[100];
 sprintf(output_file_name,"../../../result/CPU/cluster_6/rtl/module_output_bit_%d.v",which_bit_output);
// string  output_file_name << output_file_name_begin << which_bit_output << output_file_name_end;

      cout<<output_file_name<<endl;

      ofstream output_module_file(output_file_name);
/////////////////////////////////// output_module_file << "//circuit accuracy = "<<circuit_accuracy<<endl; 
/////////////////////////////////// output_module_file << "//test amounts  = "<<parameter_test_amount<<endl; 
/////////////////////////////////// output_module_file << "//total BDD nodes = "<<total_nodes_amount<<endl; 
/////////////////////////////////// output_module_file << "//total split modes = "<<total_split_nodes<<endl; 
/////////////////////////////////// output_module_file << "//train time = "<<train_time<<endl; 
/////////////////////////////////// output_module_file << "module module_output_bit_"<<which_bit_output<<"(i,o);"<<endl<<endl; 
/////////////////////////////////// output_module_file << "input ["<<bit_width-1<<":0] i;"<<endl; 
/////////////////////////////////// output_module_file << "output  o;"<<endl<<endl;
///////////////////////////////////
/////////////////////////////////// output_module_file << "wire ["<<total_non_equal_nodes-1<<":0] nodes;"<<endl;
///////////////////////////////////
/////////////////////////////////// output_module_file <<endl<< "assign o = nodes[0];"<<endl<<endl;
/////////////////////////////////// for (i=0;i<total_BDD_depth+1;i++){
///////////////////////////////////  for (j=0;j<BDD_width_each_layer[i];j++){
///////////////////////////////////   if((BDD[i][j].left_node_index >= 0) && (BDD[i][j].right_node_index >= 0)){
///////////////////////////////////	if(BDD[i][j].left_node_index == BDD[i][j].right_node_index){
///////////////////////////////////		if((BDD[i][j].left_node_neg == 0) && (BDD[i][j].right_node_neg == 0)){
///////////////////////////////////			if(BDD[i][j].non_equal_number == BDD[i+1][BDD[i][j].left_node_index].non_equal_number){
///////////////////////////////////				;
///////////////////////////////////			}else{
///////////////////////////////////    				output_module_file << "assign nodes["<<BDD[i][j].non_equal_number<<"]    = (nodes["<<BDD[i+1][BDD[i][j].left_node_index].non_equal_number<<"]);"<<endl;
///////////////////////////////////			}
///////////////////////////////////		}else if((BDD[i][j].left_node_neg == 1) && (BDD[i][j].right_node_neg == 1)){
///////////////////////////////////    			output_module_file << "assign nodes["<<BDD[i][j].non_equal_number<<"]    = (!nodes["<<BDD[i+1][BDD[i][j].left_node_index].non_equal_number<<"]);"<<endl;
///////////////////////////////////		}else if((BDD[i][j].left_node_neg == 0) && (BDD[i][j].right_node_neg == 1)){
///////////////////////////////////    			output_module_file << "assign nodes["<<BDD[i][j].non_equal_number<<"]    = ( nodes["<<BDD[i+1][BDD[i][j].left_node_index].non_equal_number<<"] ^   i["<<most_influence[i]<<"]);"<<endl;	
///////////////////////////////////		}else if((BDD[i][j].left_node_neg == 1) && (BDD[i][j].right_node_neg == 0)){
///////////////////////////////////    			output_module_file << "assign nodes["<<BDD[i][j].non_equal_number<<"]    = ( nodes["<<BDD[i+1][BDD[i][j].left_node_index].non_equal_number<<"] ^  !i["<<most_influence[i]<<"]);"<<endl;	
///////////////////////////////////		}
///////////////////////////////////	}else{
///////////////////////////////////		if((BDD[i][j].left_node_neg == 0) && (BDD[i][j].right_node_neg == 0)){
///////////////////////////////////    			output_module_file << "assign nodes["<<BDD[i][j].non_equal_number<<"]    = ( nodes["<<BDD[i+1][BDD[i][j].left_node_index].non_equal_number<<"] & !i["<<most_influence[i]<<"]) | ( nodes["<<BDD[i+1][BDD[i][j].right_node_index].non_equal_number<<"] &  i["<<most_influence[i]<<"]);"<<endl;	
///////////////////////////////////		}else if((BDD[i][j].left_node_neg == 1) && (BDD[i][j].right_node_neg == 1)){
///////////////////////////////////    			output_module_file << "assign nodes["<<BDD[i][j].non_equal_number<<"]    = (!nodes["<<BDD[i+1][BDD[i][j].left_node_index].non_equal_number<<"] & !i["<<most_influence[i]<<"]) | (!nodes["<<BDD[i+1][BDD[i][j].right_node_index].non_equal_number<<"] &  i["<<most_influence[i]<<"]);"<<endl;	
///////////////////////////////////		}else if((BDD[i][j].left_node_neg == 0) && (BDD[i][j].right_node_neg == 1)){
///////////////////////////////////    			output_module_file << "assign nodes["<<BDD[i][j].non_equal_number<<"]    = ( nodes["<<BDD[i+1][BDD[i][j].left_node_index].non_equal_number<<"] & !i["<<most_influence[i]<<"]) | (!nodes["<<BDD[i+1][BDD[i][j].right_node_index].non_equal_number<<"] &  i["<<most_influence[i]<<"]);"<<endl;	
///////////////////////////////////		}else if((BDD[i][j].left_node_neg == 1) && (BDD[i][j].right_node_neg == 0)){
///////////////////////////////////    			output_module_file << "assign nodes["<<BDD[i][j].non_equal_number<<"]    = (!nodes["<<BDD[i+1][BDD[i][j].left_node_index].non_equal_number<<"] & !i["<<most_influence[i]<<"]) | ( nodes["<<BDD[i+1][BDD[i][j].right_node_index].non_equal_number<<"] &  i["<<most_influence[i]<<"]);"<<endl;	
///////////////////////////////////		}
///////////////////////////////////	}
///////////////////////////////////   }else if((BDD[i][j].left_node_index == -1) & (BDD[i][j].right_node_index >= 0)){
///////////////////////////////////	if(BDD[i][j].right_node_neg == 0){
///////////////////////////////////    		output_module_file << "assign nodes["<<BDD[i][j].non_equal_number<<"]    = (!i["<<most_influence[i]<<"]) | ( nodes["<<BDD[i+1][BDD[i][j].right_node_index].non_equal_number<<"] &  i["<<most_influence[i]<<"]);"<<endl;
///////////////////////////////////	}else{
///////////////////////////////////    		output_module_file << "assign nodes["<<BDD[i][j].non_equal_number<<"]    = (!i["<<most_influence[i]<<"]) | (!nodes["<<BDD[i+1][BDD[i][j].right_node_index].non_equal_number<<"] &  i["<<most_influence[i]<<"]);"<<endl;
///////////////////////////////////	}
///////////////////////////////////   }else if((BDD[i][j].left_node_index == -2) & (BDD[i][j].right_node_index >= 0)){
///////////////////////////////////	if(BDD[i][j].right_node_neg == 0){
///////////////////////////////////    		output_module_file << "assign nodes["<<BDD[i][j].non_equal_number<<"]    = ( nodes["<<BDD[i+1][BDD[i][j].right_node_index].non_equal_number<<"] &  i["<<most_influence[i]<<"]);"<<endl;
///////////////////////////////////	}else{
///////////////////////////////////    		output_module_file << "assign nodes["<<BDD[i][j].non_equal_number<<"]    = (!nodes["<<BDD[i+1][BDD[i][j].right_node_index].non_equal_number<<"] &  i["<<most_influence[i]<<"]);"<<endl;
///////////////////////////////////	}
///////////////////////////////////   }else if((BDD[i][j].left_node_index >= 0) & (BDD[i][j].right_node_index == -1 )){
///////////////////////////////////	if(BDD[i][j].left_node_neg == 0){
///////////////////////////////////    		output_module_file << "assign nodes["<<BDD[i][j].non_equal_number<<"]    = ( nodes["<<BDD[i+1][BDD[i][j].left_node_index].non_equal_number<<"] & !i["<<most_influence[i]<<"]) | (      i["<<most_influence[i]<<"]);"<<endl;
///////////////////////////////////	}else{
///////////////////////////////////    		output_module_file << "assign nodes["<<BDD[i][j].non_equal_number<<"]    = (!nodes["<<BDD[i+1][BDD[i][j].left_node_index].non_equal_number<<"] & !i["<<most_influence[i]<<"]) | (      i["<<most_influence[i]<<"]);"<<endl;
///////////////////////////////////	}
///////////////////////////////////   }else if((BDD[i][j].left_node_index >= 0) & (BDD[i][j].right_node_index == -2)){
///////////////////////////////////	if(BDD[i][j].right_node_neg == 0){
///////////////////////////////////    		output_module_file << "assign nodes["<<BDD[i][j].non_equal_number<<"]    = ( nodes["<<BDD[i+1][BDD[i][j].left_node_index].non_equal_number<<"] & !i["<<most_influence[i]<<"]);"<<endl;
///////////////////////////////////	}else{
///////////////////////////////////    		output_module_file << "assign nodes["<<BDD[i][j].non_equal_number<<"]    = (!nodes["<<BDD[i+1][BDD[i][j].left_node_index].non_equal_number<<"] & !i["<<most_influence[i]<<"]);"<<endl;
///////////////////////////////////	}
///////////////////////////////////   }else if((BDD[i][j].left_node_index == -1) & (BDD[i][j].right_node_index == -1)){
///////////////////////////////////    	output_module_file << "assign nodes["<<BDD[i][j].non_equal_number<<"]    = 1;"<<endl;
///////////////////////////////////   }else if((BDD[i][j].left_node_index == -2) & (BDD[i][j].right_node_index == -2)){
///////////////////////////////////    	output_module_file << "assign nodes["<<BDD[i][j].non_equal_number<<"]    = 0;"<<endl;
///////////////////////////////////   }else if((BDD[i][j].left_node_index == -1) & (BDD[i][j].right_node_index == -2)){
///////////////////////////////////    	output_module_file << "assign nodes["<<BDD[i][j].non_equal_number<<"]    = !i["<<most_influence[i]<<"];"<<endl;
///////////////////////////////////   }else if((BDD[i][j].left_node_index == -2) & (BDD[i][j].right_node_index == -1)){
///////////////////////////////////    	output_module_file << "assign nodes["<<BDD[i][j].non_equal_number<<"]    =  i["<<most_influence[i]<<"];"<<endl;
///////////////////////////////////   }
///////////////////////////////////  }
/////////////////////////////////// }
/////////////////////////////////// 
/////////////////////////////////// output_module_file << endl <<"endmodule"<<endl;









 output_module_file << "//circuit accuracy = "<<circuit_accuracy<<endl; 
 output_module_file << "//test amounts  = "<<parameter_test_amount<<endl; 
 output_module_file << "//total BDD nodes = "<<total_nodes_amount<<endl; 
 output_module_file << "//total split modes = "<<total_split_nodes<<endl; 
 output_module_file << "//train time = "<<train_time<<endl; 
 output_module_file << "module module_output_bit_"<<which_bit_output<<"(i,o);"<<endl<<endl; 
 output_module_file << "input ["<<bit_width-1<<":0] i;"<<endl; 
 output_module_file << "output  o;"<<endl<<endl;


 for (i=0;i<total_BDD_depth+1;i++){
  output_module_file << "wire ["<<BDD_width_each_layer[i]-1<<":0] l_"<<i<<";"<<endl;
 } 
 output_module_file <<endl<< "assign o = l_0[0];"<<endl<<endl;
 for (i=0;i<total_BDD_depth+1;i++){
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
	if(BDD[i][j].left_node_neg == 0){
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
 
 output_module_file << endl <<"endmodule"<<endl;











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
