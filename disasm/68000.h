/**
 *  naken_asm assembler.
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: http://www.mikekohn.net/
 * License: GPL
 *
 * Copyright 2010-2017 by Michael Kohn
 *
 */

#ifndef _DISASM_68000_H
#define _DISASM_68000_H

#include "common/assembler.h"

int get_cycle_count_68000(unsigned short int opcode);
int disasm_68000(struct _memory *memory, uint32_t address, char *instruction, int *cycles_min, int *cycles_max);
void list_output_68000(struct _asm_context *asm_context, uint32_t start, uint32_t end);
void disasm_range_68000(struct _memory *memory, uint32_t flags, uint32_t start, uint32_t end);

#endif
