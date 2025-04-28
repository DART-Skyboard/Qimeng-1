We tested the [SPEC2000](https://www.spec.org/cpu2000/) CINT2000 benchmarks on our RISC-V chip. The modifications are as follows(in *benchspec/CINT2000* folder):

- *CINT2000/spec_run* folder contains the executable binary for each benchmark and related input data. After compiling, you can copy the folder to Linux system that running on our RISC-V chip, go to each benchmark subfolder and run them with the script *run.sh*.

- *CINT2000/runs* folder contains some useful scripts:
  
  - *make.py* is used to compile each benchmark with custimized Makefiles and move the executable binaries to the *CINT2000/spec_run*.
  
  - *generate_run.py* is used to generate the script *run.sh* for each benchmark which contains the execution commands.

- Some compiler flags are custimized to compile the benchmarks correctly. See the *makefile_rv.mk* in the src folder like *CINT2000/164.gzip/src* of each benchmark for the detailed compiler options. 
