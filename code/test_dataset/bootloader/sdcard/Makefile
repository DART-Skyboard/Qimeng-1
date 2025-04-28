RISCV_ARCH := rv32ia
RISCV_ABI := ilp32
RISCV_MCMODEL := medlow


TARGET = sdcard


#CFLAGS += -DSIMULATION
#CFLAGS += -Os
#ASM_SRCS +=
#LDFLAGS +=
#INCLUDES += -I.

C_SRCS := \
	main.c \
	mmc.c \
	ocsdc.c \


COMMON_DIR = ..
TOOLCHAIN_DIR = ../../..
include ../common.mk
