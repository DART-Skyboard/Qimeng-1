This folder contains codes of the bootloader. The bootloader serves two purposes: moving itself from spiflash to the memory and loading *opensbi*, *linux kernel* and the device tree into memory from the block device (SD card).
We implement it based on two open source projects: [tinyriscv](https://github.com/liangkangnan/tinyriscv) and [Wishbone SD card controller](https://github.com/mczerski/SD-card-controller).

You can make it with the command

    ./make.sh

and the bootloader will be located in ```./movetomem/move/```