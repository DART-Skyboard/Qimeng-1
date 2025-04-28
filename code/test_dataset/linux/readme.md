This folder contains codes of the 5.15.12 [kernel](https://github.com/torvalds/linux).
We change the Makefile in ```./arch/riscv/``` and the config file.
Also, we add drivers for Wishbone SD card IP core.

You can make it with the command

    ./make.sh

and the Image we use will be located in ```./arch/riscv/boot/```.