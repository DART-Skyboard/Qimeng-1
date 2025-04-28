module GRN_top(input_data,output_data);

input	[19:0]	input_data;
output	[19:0]	output_data;

module_output_bit_0 	m0 (input_data,output_data[0 ]);
module_output_bit_1 	m1 (input_data,output_data[1 ]);
module_output_bit_2 	m2 (input_data,output_data[2 ]);
module_output_bit_3 	m3 (input_data,output_data[3 ]);
module_output_bit_4 	m4 (input_data,output_data[4 ]);
module_output_bit_5 	m5 (input_data,output_data[5 ]);
module_output_bit_6 	m6 (input_data,output_data[6 ]);
module_output_bit_7 	m7 (input_data,output_data[7 ]);
module_output_bit_8 	m8 (input_data,output_data[8 ]);
module_output_bit_9 	m9 (input_data,output_data[9 ]);
module_output_bit_10 	m10(input_data,output_data[10]);
module_output_bit_11 	m11(input_data,output_data[11]);
module_output_bit_12 	m12(input_data,output_data[12]);
module_output_bit_13	m13(input_data,output_data[13]);
module_output_bit_14 	m14(input_data,output_data[14]);
module_output_bit_15	m15(input_data,output_data[15]);
module_output_bit_16	m16(input_data,output_data[16]);
module_output_bit_17	m17(input_data,output_data[17]);
module_output_bit_18	m18(input_data,output_data[18]);
module_output_bit_19	m19(input_data,output_data[19]);

endmodule
