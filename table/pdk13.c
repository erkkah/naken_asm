/**
 *  naken_asm assembler.
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: http://www.mikekohn.net/
 * License: GPLv3
 *
 * Copyright 2010-2022 by Michael Kohn
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include "table/pdk13.h"

struct _table_pdk13 table_pdk13[] =
{
  // Single name.
  { "nop",     0x0000, 0xffff, OP_NONE, 1, 1 },
  { "ldsptl",  0x0006, 0xffff, OP_NONE, 1, 1 },
  { "ldspth",  0x0007, 0xffff, OP_NONE, 1, 1 },
  // Misc.
  { "addc",    0x0010, 0xffff, OP_A,    1, 1 },
  { "subc",    0x0011, 0xffff, OP_A,    1, 1 },
  { "izsn",    0x0012, 0xffff, OP_A,    1, 1 },
  { "dzsn",    0x0013, 0xffff, OP_A,    1, 1 },
  { "pcadd",   0x0017, 0xffff, OP_A,    1, 1 },
  { "not",     0x0018, 0xffff, OP_A,    1, 1 },
  { "neg",     0x0019, 0xffff, OP_A,    1, 1 },
  { "sr",      0x001a, 0xffff, OP_A,    1, 1 },
  { "sl",      0x001b, 0xffff, OP_A,    1, 1 },
  { "src",     0x001c, 0xffff, OP_A,    1, 1 },
  { "slc",     0x001d, 0xffff, OP_A,    1, 1 },
  { "swap",    0x001e, 0xffff, OP_A,    1, 1 },
  { "wdreset", 0x0030, 0xffff, OP_NONE, 1, 1 },
  { "pushaf",  0x0032, 0xffff, OP_NONE, 1, 1 },
  { "popaf",   0x0033, 0xffff, OP_NONE, 1, 1 },
  { "reset",   0x0035, 0xffff, OP_NONE, 1, 1 },
  { "stopsys", 0x0036, 0xffff, OP_NONE, 1, 1 },
  { "stopexe", 0x0037, 0xffff, OP_NONE, 1, 1 },
  { "engint",  0x0038, 0xffff, OP_NONE, 1, 1 },
  { "disgint", 0x0039, 0xffff, OP_NONE, 1, 1 },
  { "ret",     0x003a, 0xffff, OP_NONE, 1, 1 },
  { "reti",    0x003b, 0xffff, OP_NONE, 1, 1 },
  { "mul",     0x003c, 0xffff, OP_NONE, 1, 1 },
  // Operations with A and IO.
  { "xor",     0x0060, 0xffe0, OP_IO_A, 1, 1 },
  { "mov",     0x0080, 0xffe0, OP_IO_A, 1, 1 },
  { "mov",     0x00a0, 0xffe0, OP_A_IO, 1, 1 },
  // 16 bit memory operations.
  { "stt16",   0x0300, 0xff81, OP_M4,   1, 1 },
  { "ldt16",   0x0301, 0xff81, OP_M4,   1, 1 },
  { "idxm",    0x0380, 0xff81, OP_M4_A, 2, 2 },
  { "idxm",    0x0381, 0xff81, OP_A_M4, 2, 2 },
  // Return with A.
  { "ret",     0x0100, 0xff00, OP_K,    1, 1 },
  // Operations with A and memory.
  { "add",     0x0400, 0xffc0, OP_M_A,  1, 1 },
  { "sub",     0x0440, 0xffc0, OP_M_A,  1, 1 },
  { "addc",    0x0480, 0xffc0, OP_M_A,  1, 1 },
  { "subc",    0x04c0, 0xffc0, OP_M_A,  1, 1 },
  { "and",     0x0500, 0xffc0, OP_M_A,  1, 1 },
  { "or",      0x0540, 0xffc0, OP_M_A,  1, 1 },
  { "xor",     0x0580, 0xffc0, OP_M_A,  1, 1 },
  { "mov",     0x05c0, 0xffc0, OP_M_A,  1, 1 },
  { "add",     0x0600, 0xffc0, OP_A_M,  1, 1 },
  { "sub",     0x0640, 0xffc0, OP_A_M,  1, 1 },
  { "addc",    0x0680, 0xffc0, OP_A_M,  1, 1 },
  { "subc",    0x06c0, 0xffc0, OP_A_M,  1, 1 },
  { "and",     0x0700, 0xffc0, OP_A_M,  1, 1 },
  { "or",      0x0740, 0xffc0, OP_A_M,  1, 1 },
  { "xor",     0x0780, 0xffc0, OP_A_M,  1, 1 },
  { "mov",     0x07c0, 0xffc0, OP_A_M,  1, 1 },
  // Operations with memory.
  { "addc",    0x8000, 0xffc0, OP_M,    1, 1 },
  { "subc",    0x8040, 0xffc0, OP_M,    1, 1 },
  { "izsn",    0x8080, 0xffc0, OP_M,    1, 1 },
  { "dzsn",    0x80c0, 0xffc0, OP_M,    1, 1 },
  { "inc",     0x9000, 0xffc0, OP_M,    1, 1 },
  { "dec",     0x9040, 0xffc0, OP_M,    1, 1 },
  { "clear",   0x9080, 0xffc0, OP_M,    1, 1 },
  { "xch",     0x90c0, 0xffc0, OP_M,    1, 1 },
  { "not",     0xa000, 0xffc0, OP_M,    1, 1 },
  { "neg",     0xa040, 0xffc0, OP_M,    1, 1 },
  { "sr",      0xa080, 0xffc0, OP_M,    1, 1 },
  { "sl",      0xa0c0, 0xffc0, OP_M,    1, 1 },
  { "src",     0xb000, 0xffc0, OP_M,    1, 1 },
  { "slc",     0xb040, 0xffc0, OP_M,    1, 1 },
  { "ceqsn",   0xb080, 0xffc0, OP_A_M,  1, 1 },
  //{ "cneqsn",  0xb0c0, 0xffc0, OP_A_M,  1, 1 },
  // Bit operations with IO.
  { "t0sn",    0xc000, 0xff80, OP_IO_N, 1, 1 },
  { "t1sn",    0xd000, 0xff80, OP_IO_N, 1, 1 },
  { "set0",    0xe000, 0xff80, OP_IO_N, 1, 1 },
  { "set1",    0xf000, 0xff80, OP_IO_N, 1, 1 },
  // Operations with A and 8 bit literal.
  { "add",     0x1000, 0xff00, OP_A_K,  1, 1 },
  { "sub",     0x1100, 0xff00, OP_A_K,  1, 1 },
  { "ceqsn",   0x1200, 0xff00, OP_A_K,  1, 1 },
  //{ "cneqsn",  0x1300, 0xff00, OP_A_K,  1, 1 },
  { "and",     0x1400, 0xff00, OP_A_K,  1, 1 },
  { "or",      0x1500, 0xff00, OP_A_K,  1, 1 },
  { "xor",     0x1600, 0xff00, OP_A_K,  1, 1 },
  { "mov",     0x1700, 0xff00, OP_A_K,  1, 1 },
  // Control transfers.
  { "goto",    0x1800, 0xfc00, OP_K10,  2, 2 },
  { "call",    0x1c00, 0xfc00, OP_K10,  2, 2 },
  { NULL,      0x0000, 0x0000,      0,  0, 0 }
};

