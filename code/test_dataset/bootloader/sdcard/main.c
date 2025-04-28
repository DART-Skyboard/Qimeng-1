/*
 * WISHBONE SD Card Controller IP Core
 *
 * sd_example.c
 *
 * This file is part of the WISHBONE SD Card
 * Controller IP Core project
 * http://opencores.org/project,sd_card_controller
 *
 * Description
 * Example application using WISHBONE SD Card Controller
 * IP Core. The app perform core initialisation,
 * mmc/sd card initialisation and then reads one block
 * of data from the card.
 * This app is using some of code from u-boot project
 * (mmc.c and mmc.h)
 *
 * Author(s):
 *     - Marek Czerski, ma.czerski@gmail.com
 */
/*
 *
 * Copyright (C) 2013 Authors
 *
 * This source file may be used and distributed without
 * restriction provided that this copyright statement is not
 * removed from the file and that any derivative work contains
 * the original copyright notice and the associated disclaimer.
 *
 * This source file is free software; you can redistribute it
 * and/or modify it under the terms of the GNU Lesser General
 * Public License as published by the Free Software Foundation;
 * either version 2.1 of the License, or (at your option) any
 * later version.
 *
 * This source is distributed in the hope that it will be
 * useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 * PURPOSE. See the GNU Lesser General Public License for more
 * details.
 *
 * You should have received a copy of the GNU Lesser General
 * Public License along with this source; if not, download it
 * from http://www.opencores.org/lgpl.shtml
 */

#include "mmc.h"
// #include <xprintf.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>


struct mmc * ocsdc_mmc_init(int base_addr, int clk_freq);

#define BLKSIZE 512
// #define BLKCNT 10
// #define OPENSBI_PTR 0x80000000
// #define OPENSBI_BLK 114
// #define LINUX_PTR 0x80400000
// #define LINUX_BLK 10722
// #define INITRD_PTR 0x82400000
// #define INITRD_BLK 2980
// #define FDT_PTR 0x83e00000
// #define FDT_BLK 8

// char buff[BLKSIZE*BLKCNT] = {'\0'};

void printHex(const void *lpvbits, const unsigned int n) {
    char* data = (char*) lpvbits;
    unsigned int i = 0;
    char line[17] = {};
    xprintf("%08LX | ", (unsigned int)data);
    while ( i < n ) {
        line[i%16] = *(data+i);
        if ((line[i%16] < 32) || (line[i%16] > 126)) {
            line[i%16] = '.';
        }
        xprintf("%02X", (unsigned char)*(data+i));
        i++;
        if (i%4 == 0) {
            if (i%16 == 0) {
                if (i < n-1)
                ;
                    xprintf(" | %s\n\r%08LX | ", line, (unsigned int)data+i);
            } else {
                xprintf(" ");
            }
        }
    }
    while (i%16 > 0) {
        (i%4 == 0)?xprintf("   "):xprintf("  ");
        line[i%16] = ' ';
        i++;
    }
    xprintf(" | %s\n\r", line);
}

static unsigned int crc_table[256];
static void init_crc_table(void) {
    unsigned int c;
    unsigned int i, j;
    for (i = 0;i < 256; i++) {
        c = (unsigned int) i;
        for (j = 0; j < 8; j++) {
            if (c & 1)
                c = 0xedb88320L ^ (c >> 1);
            else
                c = c >> 1;
        }
        crc_table[i] = c;
    }
}

uint32_t crc32(unsigned int crc, unsigned char *buffer, unsigned int size) {
    unsigned int i;
    for (i = 0;i < size;i ++)
    {
        crc = crc_table[(crc^buffer[i])&0xff] ^ (crc>>8);
    }
    return crc;
}


int main(void) {
    uart_init();
	xprintf("Hello World !!!\n\r");
    // int temp = 0;
    // for (int i = 0;i < 10;i ++) {
    //     for (int j = 0; j < 100000;j ++) {
    //         temp = i + j;
    //         if(temp % 1000 == 0)
    //             xprintf("%d\n\r", temp);
    //     }
    // }

    
	//init ocsdc driver
	struct mmc * drv = ocsdc_mmc_init(0x1fe10000, 50000000);
	if (!drv) {
		xprintf("ocsdc_mmc_init failed\n\r");
		return -1;
	}
	xprintf("ocsdc_mmc_init success\n\r");

	drv->has_init = 0;
	int err = mmc_init(drv);
	if (err != 0 || drv->has_init == 0) {
		xprintf("mmc_init failed\n\r");
		return -1;
	}
	xprintf("mmc_init success\n\r");

    
    uint32_t buff[512/4];

    uint32_t offset = 64;
	xprintf("attempting to load first sector\n\r");
	if (mmc_bread(drv, offset, 1, buff) == 0) {
		xprintf("load first sector failed\n\r");
		return -1;
	}
	xprintf("first sector loaded\n\r");

    for (int i = 0; i < 4 ; i++) {
        uint32_t load_addr = buff[3*i];
        uint32_t card_sector = buff[3*i+1];
        uint32_t len = buff[3*i+2];
        xprintf("attempting to load data\n\r");
        if (mmc_bread(drv, card_sector+offset, len, load_addr) == 0) {
            xprintf("load failed\n\r");
            return -1;
        }
        xprintf("data loaded\n\r");
    }
    xprintf("all data loaded\n\r");
    xprintf("calculating crc32\n\r");
    // init_crc_table();
    // for (int i = 0; i < 4 ; i++) {
        // if (i == 2) continue;
        // uint32_t load_addr = buff[3*i];
        // uint32_t card_sector = buff[3*i+1];
        // uint32_t len = buff[3*i+2];
        // unsigned int crc = 0xffffffff;
        // crc = crc32(crc, (char*)load_addr, len*512);
        // xprintf("%d:%x\n\r", i, crc);
    // }
    xprintf("crc32 calculated\n\r");

	// printHex(buff, BLKSIZE);

	return EXIT_SUCCESS;
}
