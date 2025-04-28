
    .section .init;
    .globl _start;
    .type _start,@function


_start:

	lui	t1,0x0
    lui	t3,0x83800
    la t2, _sdcard_start
    la t0, _sdcard_end
    sub	t0,t0,t2
loop:
    add	t4,t1,t2
    add	t5,t1,t3
    lb	t6,0(t4)
    sb	t6,0(t5)
    addi	t1,t1,1
    bne	t0,t1,loop





    lui	t0,0x0
    lui	t1,0x0
	lui	t2,0x0
    lui	t3,0x0
    lui	t4,0x0
    lui	t5,0x0
    lui	t6,0x0
    csrrs a0,mhartid,zero
    lui a1,0x83e00
    lui	t0,0x83800
    jalr	zero,0(t0)

_sdcard_start:
	.incbin "sdcard.bin"
_sdcard_end:

