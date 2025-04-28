RISCV_ARCH := rv32ia
RISCV_ABI := ilp32
RISCV_MCMODEL := medlow

TARGET := dhry

C_SRCS := \
	dhry_1.c \
	dhry_2.c

# CFLAGS := -O0 -fno-common -funroll-loops -finline-functions #--param max-inline-insns-auto=20 -falign-functions=4 -falign-jumps=4 -falign-loops=4
CFLAGS := -O2

CFLAGS += -DFLAGS_STR=\""$(CFLAGS)"\" -DREG=register
# CFLAGS += -DITERATIONS=10000 -DPERFORMANCE_RUN=1

COMMON_DIR = ..
TOOLCHAIN_DIR = ../../..
include ../common.mk
