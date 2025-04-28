src/

The latest version of the BSD Learner. Still working for the first released version on Github.

BSD.cpp			main code of our algorithm
cvt.h			head and some tool functions
print_circuit.h		turn the graph into verilog

1.	When we want more accurate/complex circuit:
	
	BSD.cpp	line 20:	This parameter "bit_width" means total input bit amounts.
		line 21:	If accuracy is not enough, get this parameter larger. In CPU tapeout task, it is set from 200k to 1M.
		line 24:	This paramter means how many root nodes of one BSD.
		line 29:	If BSD is not efficient enough in merging nodes together, get this parameter larger may help.
				Note that this parameter should not be larger than line 21	
		line 32:	How many samples are there in the test set. Should be larger if we want more accurate circuits.
		line 41:	The biggest BDD width. Could be smaller in the demo case, but should be large if we are facing more complex circuit.

2.	When we are doing ablation study/tradeoff:

	BSD.cpp line 22:	Set to a larger number than "bit_width", this program produces a decision tree.
		line 31:	A tradeoff between time/area, set to 1 (default) is the slowest but most area effient. Set to other N+ is quicker.
		line 34:	Until XX depth to stop expansion. It is a time/accuracy tradeoff. Default is the most accurate.
