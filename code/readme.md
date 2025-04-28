# BSD Learner (Binary Speculation Diagram Learner)

### Table of Contents
1. [Introduction](#introduction)
2. [Prerequisites](#prerequisites)
3. [Repo contents](#repo-contents)
4. [Experiments in the paper](#experiments-in-the-paper)
    1. [CPU design generation (Figure 3)](#cpu-design-generation-figure-3)
    2. [Manufactured chip validation (Figure 4 & Extended Data Figure 2)](#manufactured-chip-validation-figure-4-extended-data-figure-2)
    3. [Self-designing ability (Figure 5)](#demonstration-of-self-designing-ability-figure-5)
    4. [Comparision with SOTA (Extended Data Figure 4)](#comparison-with-sota-extended-data-figure-4)
    5. [GRN construction (Extended Data Figure 6)](#grn-construction-extended-data-figure-6)
5. [Source Code (latest version)](#source-code-latest-version)
    

# Introduction

BSD Learner (Binary Speculation Diagram Learner) is a program to design logic circuits automatically from Input/Output observations only.

# Prerequisites

+ Linux (our program has been tested with CentOS, and it should also work on other distributions)
+ GCC 9.3.0+
+ Python 3.6.8+
+ *OPTIONAL:* numpy 1.22.4+, matplotlib 3.5.2+ (visualize the results)
+ *OPTIONAL:* Synopsys Design Compiler (standardize the outputs into GTECH netlist)
+ *OPTIONAL:* slurm 20.02.6+

# Repo contents

+ `demo` holds two simple experiments (demo_function, GRN) and a complex experiment (CPU).
+ `result`	holds the results of the demos.
+ `scripts`	holds the helper scripts to run all tasks.
+ `src` holds the key source code.
+ `tools` holds source code used for visualizing and standardizing.
+ `test_dataset` holds projects used for testing our board.

# Experiments in the paper

Circuits generation: from black-box function to Verilog (Gate-level logic netlist).
The codebase has a few scripts in `scripts/` for running a particular task.
The main codes of the algorithm (BSD) are located in `src/`.

## CPU design generation (Figure 3)

The main experiment of this paper. Based on the generated CPU design, Figure 3a, Figure 3b, and Figure 3c are the layout, the manufactured chip, and the board with manufactured chip, respectively.

The BSD of the entire CPU design is partitioned into 10 clusters as presented in the manuscript. In each BSD cluster, we use one single-output BSD to design each of the 1826 output bits.

So the whole CPU design is built with 18260 single-output BSDs.

### Generating the entire CPU design sequentially

Run the following command to generate all 18260 single-output BSDs in a sequential order on a Linux computer. The result is saved in `result/CPU/cluster_$i/rtl`.
```
bash scripts/1_a_CPU.sh
```
Estimated execution time: ~3500 hours on a normal computer.

### Generating the entire CPU design in parallel

dependence: slurm

Run the following command to generate 18260 single-output BSDs in parallel on a high-performance Linux cluster, which is employed in conducting the experiment in our manuscript. 

The users should adjust the slurm setting in `/demo/CPU/cluster_$i/demo.sh` according to the configurations of their own Linux cluster.
```
bash scripts/1_b_CPU_slurm.sh
```
The results are Verilog files saved in `result/CPU/cluster_$i/rtl`.

Estimated execution time: <5 hours on our cluster with 68 servers, each of which is equiped with 2 Intel Xeon Gold 6230 CPUs, as presented in our manuscript.

### Generating a partial CPU design in an interactive mode

The users can generate arbitrary part of the CPU design interactively by specifying the number and order of generated single-output BSDs.

```
usage:
	bash scripts/1_c_CPU_arbitrary_cases.sh [--start START]  [--end END] [--cluster CLUSTER]
options:
	-s, --start   start single-output BSD id, an integer in the range 0..1826 (default: 32)
	-e, --end     end single-output BSD id, an integer in the range 0..1826 (default: 64)
	-c, --cluster BSD cluster id, an integer in the range 1..10 (default: 1)
```

Estimated execution time: ~10 minutes (ranging from several seconds to 30 minutes) to generate a single-output BSD on a Linux computer.

### Converting the CPU design to a standard GTECH netlist

dependence: Synopsys Design Compiler

Run the following command to convert the generated CPU designed stored in `result/CPU/cluster_$i/rtl`.

```sh
bash scripts/1_d_CPU_GTECH.sh
```

Estimated execution time: <1 minute on a Linux computer.


## Manufactured chip validation (Figure 4 & Extended Data Figure 2)

The manufactured chip is validated by running Linux operating system, SPEC CINT 2000, and Dhrystone.

The validation benchmarks and datasets are provided in `test_dataset/spec2000` and `test_dataset/dhrystone`. The execution script of SPEC CINT 2000 is in `test_dataset/spec2000/CINT2000/spec_run`. And the binary file for Dhrystone is in `test_dataset/dhrystone/dhry/` after compling with the command `./test_dataset/dhrystone/make.sh`.

The validated results are stored in `test_dataset/outputs`.

More details can be found in `test_dataset/readme.md`.


## Demonstration of self-designing ability (Figure 5)

The manufactured chip is able to design itself gradually by running the program (binary file) in `test_dataset/self_designing`.

The output circuit designs with 10, 20, and 30 BSD layers are stored in `test_dataset/outputs/self_designing`.

If users do not have our chip/board, we provide a demo running on a Linux comuputer just to show what the program is supposed to do. Run the following commend, and find the result in `result/CPU/cluster_$i/self_design`

```
usage:
    bash scripts/demo_on_servers_for_self_design.sh [--cluster CLUSTER] [--bit BIT]
options:
    -c, --cluster BSD cluster id, an integer in the range 1..10
    -b, --bit     single-output BSD id, an integer in the range 0..1826
```

## Comparison with SOTA (Extended Data Figure 4)

The experiments show that the SOTA RL methods generate circuits much smaller than ours. 

Extended data Figure 4a is the structure of one single task. Figure 4b for comparison can be found in Method Reference 3 as mentioned in the manusript.

Run the following command to visualize the generated CPU designed stored in `result/CPU/rtl` interactively.
```
usage:
    bash scripts/4_plot.sh [--cluster CLUSTER] [--bit BIT]
dependence:
    numpy, matplotlib
options:
    -c, --cluster BSD cluster id, an integer in the range 1..10
    -b, --bit     single-output BSD id, an integer in the range 0..1826
```

Using the default parameter and the output result is Extended Figure 5a in the manuscript. 

The results are saved in `result/CPU/cluster_$i/plot`, a python file, and its output image.

Estimated execution time: ~1 minute on a Linux computer.

## GRN construction (Extended Data Figure 6)

The experiment shows some potential for the usage of our method in other domains. Extended Figure 6 is visualized below.

dependence: Synopsys Design Compiler


```
bash scripts/5_GRN.sh
```

The results are saved in `result/GRN`. This is the graph data of Figure 6a.

We run the same dataset on the SCNS tool, which has been open-sourced on https://www.microsoft.com/en-us/research/project/scns/. We use this tool to get the biggest graph as we can, and get a graph data of Figure 6b.

Then using the visualization tool Cytoscape, we get Extended Figure 6a and Figure 6b, respectively.
 
## Source Code (latest version)

To analyze our code accomplishment, we strongly recommend our latest version in `src/`, which we are going to release on github with MIT license.
The latest version has a much clearer structure and is easier to understand. We are still working on it to prepare our first release version on github.

For more demo functions/results (not mentioned in the paper), run the following command and the result is saved in `result/demo_functions`.
```
bash scripts/latest_version_demo_functions.sh 
```
Then run the following command to get a standard gtech output. 

dependence: Synopsys Design Compiler

```
bash scripts/latest_version_to_gtech.sh
```
Execution time: ~10 minutes for all 19 circuits.
