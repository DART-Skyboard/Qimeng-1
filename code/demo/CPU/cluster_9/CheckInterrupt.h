#pragma once
#include "cvt.h"
bool CheckInterrupt(bool input_data[]){
    enum enum_number_csr_code {
        number_mtvec = 0x305,
        number_mepc = 0x341,
        number_mcause = 0x342,
        number_mie = 0x304,
        number_mip = 0x344,
        number_mtval = 0x343,
        number_mscratch = 0x340,
        number_mstatus = 0x300,
        number_mideleg = 0x303,
        number_medeleg = 0x302,
        number_sepc = 0x141,
        number_stvec = 0x105,
        number_scause = 0x142,
        number_sscratch = 0x140,
        number_stval = 0x143,
        number_sstatus = 0x100,
        number_sie = 0x104,
        number_sip = 0x144,
        number_satp = 0x180,
        number_mhartid = 0xf14,
        number_misa = 0x301,
    };
    /*enum run_state{
        U,S,M,
    }cur_run_state;*/
    bool mip[32];
    bool mie[32];
    bool mstatus[32];
    bool mideleg[32];
    bool MIE = mstatus[31-3];
    bool SIE = mstatus[31-1];
    copy_indice(mip, 0, input_data, 1152, 32);
    copy_indice(mie, 0, input_data, 1120, 32);
    copy_indice(mstatus, 0, input_data, 1248, 32);
    copy_indice(mideleg, 0, input_data, 1280, 32);
    for(int i=0; i<32; i++){
        if(mip[i] == 1){ //mip=1说明有中断请求
            if(mie[i] == 1){ //mie=1说明中断请求被允许
                //判断这个中断要到S还是M态处理
                if(mideleg[i] == 1){//应该交给S态处理，这里还存在一些问题，mideleg应该读取哪一位
                    if(SIE == 1)//if((cur_run_state == S && SIE == 1)||cur_run_state==U)
                    return true;
                }
                else{//应该交给M态处理
                    if(MIE == 1)//if((cur_run_state == M && MIE == 1)||cur_run_state!=M)
                    return true;
                }
            }
        }
    }
    return false;
}