#include <stdint.h>

#include "include/utils.h"


extern void trap_entry();


void _init()
{

    write_csr(mtvec, &trap_entry);

    write_csr(mstatus, 0x1888);

}
