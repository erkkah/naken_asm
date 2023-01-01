/**
 *  naken_asm assembler.
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: http://www.mikekohn.net/
 * License: GPLv3
 *
 * Copyright 2010-2021 by Michael Kohn
 *
 */

#ifndef NAKEN_ASM_TABLE_8048_H
#define NAKEN_ASM_TABLE_8048_H

#include "common/assembler.h"

enum
{
  OP_NONE,
  OP_A,
  OP_C,
  OP_I,
  OP_T,
  OP_F0,
  OP_F1,
  OP_BUS,
  OP_CLK,
  OP_CNT,
  OP_MB0,
  OP_MB1,
  OP_RB0,
  OP_RB1,
  OP_PSW,
  OP_TCNT,
  OP_TCNTI,
  OP_AT_A,
  OP_PP,
  OP_P0,
  OP_P03,
  OP_P12,
  OP_RR,
  OP_AT_R,
  OP_NUM,
  OP_ADDR,
  OP_PADDR,
  // Extra for 8041.
  OP_DMA,
  OP_FLAGS,
  OP_STS,
  OP_DBB,
  // Extra for NS405.
  OP_HACC,
  OP_INTR,
  OP_MASK,
  OP_II,
  OP_XI,
  OP_PORT,
  OP_MB2,
  OP_MB3,
  OP_SCR,
  OP_VCR,
  OP_HOME,
  OP_CURS,
  OP_AT_CURS,
  OP_BEGD,
  OP_ENDD,
  OP_SROW,
  OP_AL0,
  OP_AL1,
  OP_HPEN,
  OP_VPEN,
  OP_VINT,
  OP_PSR,
  OP_BAUD,
  OP_UCR,
  OP_UMX,
  OP_STAT,
  OP_RCVR,
  OP_XMTR,
  OP_TCP,
  OP_AT_TCP,
};

enum
{
  FLAG_NONE = 0,
  FLAG_8041 = 1,
  FLAG_8048 = 2,
  FLAG_NS405 = 4,
};

struct _table_8048
{
  char *name;
  uint8_t opcode;
  uint8_t mask;
  uint8_t operand_count;
  uint8_t operand_1;
  uint8_t operand_2;
  uint8_t cycles;
  uint8_t flags;
};

extern struct _table_8048 table_8048[];

#endif

