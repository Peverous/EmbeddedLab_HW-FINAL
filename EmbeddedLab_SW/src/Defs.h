/*
 * Defs.h
 *
 *  Created on: 9 ��� 2016
 *      Author: Administrator
 */

#ifndef DEFS_H_
#define DEFS_H_

#define BuffSize 128
#define BuffSize_1 16
#define VecSize 64
#define XON 20
#define XOFF 21
#define DEL 16

u8 crcTable[256];
u8 TB[BuffSize];
u8 RB[BuffSize];
int Wr_TB;
int Wr_RB;
int Rd_TB;
int Rd_RB;
int i,j;
int rec_ack;
int snd_back;
int num_of_data; //gia ton timer katw gia na steilei pisw to (n)ack # dedomenwn pros matlab
int num_of_incoming; //number of incoming data from chanel
int num_of_writes ;//number of input data, or size of packet with preamble header data crc
int num_of_reads;
u8 crc_final;
u8 crc_rec;

u8 vector;
int flag_rc; //flag gia na steilei sto kanali. 1->steile / 0->mhn steileis
int flag_xon_xoff;
int flag_enable_TC;//flag gia energopoihsh toy TC Cyclic Buffer __ENERGOPOIOYNTAI (1) STOUS TIMERS__!!!!!!
int flag_enable_RC;//flag gia energopoihsh toy RC Cyclic Buffer


#endif /* DEFS_H_ */
