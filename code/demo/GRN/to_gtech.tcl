set	target_library gtech.db
set_host_options -max_cores 16
set     structure       true
set  hlo_resource_allocation  area
for {set i 0} {$i < 20} {incr i} {
	read_verilog -rtl ../../result/GRN/rtl/module_output_bit_$i.v
	current_design module_output_bit_$i
	ungroup -flatten -all
	set_fix_multiple_port_nets  -all  -buffer_constants  [get_designs  *]
	set_app_var verilogout_no_tri true
	change_name -rule verilog -hier
	compile
	#write -f ddc -hierarchy -output work/gtech-compiled.ddc
	write -f verilog -hierarchy -output ../../result/GRN/gtech/module_output_bit_$i.v
}
####for {set i 0} {$i < 20} {incr i} {
####	read_verilog -netlist gtech/module_output_bit_$i.v
####}
	read_verilog GRN_top.v
	ungroup -flatten -all
	set_fix_multiple_port_nets  -all  -buffer_constants  [get_designs  *]
	set_app_var verilogout_no_tri true
	change_name -rule verilog -hier

	write -f verilog -hierarchy -output ../../result/GRN/GRN_all.v
