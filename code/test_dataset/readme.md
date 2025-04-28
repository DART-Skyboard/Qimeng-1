# Brief introduction to each folder
Bootloader, Busybox, OpenSBI and Linux are used for running the Linux operating system on our circuit board (**Figure 3** in our manuscript).
And Dhrystone and Spec2000 are two benchmarks used to evaluate the performace of CPU-AI (**Figure 4** and **Extended Data Figure 2** in our manuscript).

Each folder has a file named *readme.md* which shows the command to compile it and other details like modifications to the origin open source project.
As these codes are used for testing our board, you cannot execute binary files on your computer.
Due to the file size limit, we remove codes in *busybox*, *linux* and *spec2000*. You can contact us to get these codes if you request.

## Bootloader
Bootloader for our board. It will be burned into spi flash.

## Busybox
Many common UNIX utilities. We use it to generate our rootfs.

## OpenSBI
RISC-V Open Source Supervisor Binary Interface. We add support for rv32ia.

## Linux
Linux kernel. We present the outputs of booting up the Linux operating system in **Figure 4 a** of our manuscript.

## Dhrystone
Dhrystone benchmark. It is for a bare-metal system. The results is shown in **Figure 4 b** and **Extended Data Figure 2 b**.

## Spec2000
Spec2000 benchmark. We run it in the linux kernel. The output of 164.zip is shown in **Extended Data Figure 2 a**.

## Typical time for compiling these projects

| project | time |
| --- | --- |
| bootloader | <1min |
| busybox | 2min |
| dhrystone | <1min |
| linux | 5min |
| opensbi | 2min |
| spec2000 | <1min |



# Toolchains infomation
The version of the cross-compiler toolchain used is 11.1.0.

You should clone [riscv-gnu-toolchains](https://github.com/riscv-collab/riscv-gnu-toolchain) and compile it using the following commands

    ./configure --prefix=/opt/riscv-gnu-toolchain/riscv-elf32 --with-arch=rv32ia --with-abi=ilp32
    sudo make -j $(nproc)
    ./configure --prefix=/opt/riscv-gnu-toolchain/riscv-linux32 --with-arch=rv32ia --with-abi=ilp32
    sudo make linux -j $(nproc)


# Hardware and system infomation
System: Ubuntu 20.04.3 LTS

CPU: Intel® Core™ i7-11700

Memory:125.5 GiB