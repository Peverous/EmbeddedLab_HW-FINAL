/*
 * crc.h
 *
 *  Created on: Feb 11, 2017
 *      Author: chris
 */

#ifndef CRC_H_
#define CRC_H_
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "xparameters.h"
#include "xtmrctr.h"
#include "xil_exception.h"
#include "xil_printf.h"
#include "Defs.h"
#define CRC_POLY  0xD5
#define WIDTH 8*(sizeof(CRC_POLY))
#define TOPBIT 1<<(WIDTH-1)



void crc_init();
void cacl_crc(u8 data);
void crc_check(u8 data);
void reset_stats(int rec_ack_var);

#endif /* CRC_H_ */
