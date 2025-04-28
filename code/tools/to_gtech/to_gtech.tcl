set target_library gtech.db
set_host_options -max_cores 16
set     structure       true
set  hlo_resource_allocation  area
for { set i 1 } { $i < 15 } { incr i } {
	read_verilog -rtl ../../result/demo_functions/rtl/function_${i}_BDD_number_0.v
	current_design function_${i}_BDD_number_0.v
	ungroup -flatten -all
	set_fix_multiple_port_nets  -all  -buffer_constants  [get_designs  *]
	set_app_var verilogout_no_tri true
	change_name -rule verilog -hier
	compile
	#write -f ddc -hierarchy -output work/gtech-compiled.ddc
	write -f verilog -hierarchy -output ../../result/demo_functions/gtech/gtech_function_${i}.v
}
