def	print_top_module(input_bit_width,reg_width,output_bit_width):


	top_module_file	= open('RISCV_train.v','w');

	top_module_file.write('module RISCV_train(input_data_all, output_data_all);\n\n')
	
	top_module_file.write('input	['+str(input_bit_width - 1)+':0]	input_data_all;\n')
	
	top_module_file.write('wire	['+str(output_bit_width - 1)+':0]	output_data_all;\n\n')
	
	for i in range(0,output_bit_width):
		
		top_module_file.write('module_output_bit_'+str(i)+'	m'+str(i)+'	(input_data_all['+str(input_bit_width - 1)+':0],output_data_all['+str(i)+']);\n')

	top_module_file.write('\nendmodule')
	top_module_file.close()


	return

print_top_module(1797,0,1826)

############def	print_top_module(input_bit_width,reg_width,output_bit_width):
############
############
############	top_module_file	= open('rtl/top.v','w');
############
############	top_module_file.write('module top ( clock, reset, input_data, output_bit);\n\n')
############	
############	top_module_file.write('input		clock, reset;\n')
############	
############	top_module_file.write('input	['+str(input_bit_width - reg_width - 1)+':0]	input_data;\n')
############	
############	top_module_file.write('output	['+str(output_bit_width - reg_width - 1)+':0]	output_bit;\n\n')
############	if(reg_width>0):
############		top_module_file.write('reg	['+str(reg_width - 1)+':0]	reg_data;\n')
############	top_module_file.write('wire	['+str(input_bit_width - 1)+':0]	input_data_all;\n\n')
############	
############	top_module_file.write('assign	input_data_all ['+str(reg_width - 1)+':0]	=	reg_data	['+str(reg_width - 1)+':0];\n')
############	top_module_file.write('assign	input_data_all ['+str(input_bit_width - 1)+':'+str(reg_width)+']	=	input_data	['+str(input_bit_width - reg_width - 1)+':0];\n')
############	top_module_file.write('wire	['+str(output_bit_width - 1)+':0]	output_data_all;\n\n')
############	
############	for i in range(0,output_bit_width):
############		
############		top_module_file.write('module_output_bit_'+str(i)+'	m'+str(i)+'	(input_data_all['+str(input_bit_width - 1)+':0],output_data_all['+str(i)+']);\n')
############
############	top_module_file.write('assign	output_bit ['+str(output_bit_width - reg_width - 1)+':0]	=	output_data_all	['+str(output_bit_width - 1)+':'+str(reg_width)+'];\n')
############	if(reg_width > 0):
############		top_module_file.write('\nwire	['+str(reg_width - 1)+':0]	to_reg_data;\n')
############		top_module_file.write('assign	to_reg_data ['+str(reg_width - 1)+':0]	=	output_data_all	['+str(reg_width - 1)+':0];\n\n')
############		
############		top_module_file.write('always@(posedge clock)\n')
############		top_module_file.write('begin\n')
############		top_module_file.write('	if(!reset)\n')
############		top_module_file.write('		reg_data	['+str(reg_width - 1)+':0]	<=	'+str(reg_width)+'\'b0;\n')
############		top_module_file.write('	else\n')
############		top_module_file.write('		reg_data	['+str(reg_width - 1)+':0]	<=	to_reg_data ['+str(reg_width-1)+':0];\n')
############		top_module_file.write('end\n')
############	
############	top_module_file.write('\nendmodule')
############
############	return
