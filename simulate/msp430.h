/**
 *  naken_asm assembler.
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: https://www.mikekohn.net/
 * License: GPLv3
 *
 * Copyright 2010-2023 by Michael Kohn
 *
 */

#ifndef NAKEN_ASM_SIMULATE_MSP430_H
#define NAKEN_ASM_SIMULATE_MSP430_H

#include <unistd.h>

#include "common/memory.h"
#include "simulate/common.h"

typedef struct _simulate_msp430
{
  uint16_t reg[16];
} SimulateMsp430;

Simulate *simulate_init_msp430();
void simulate_free_msp430(Simulate *simulate);
int simulate_dumpram_msp430(Simulate *simulate, int start, int end);
void simulate_push_msp430(Simulate *simulate, uint32_t value);
int simulate_set_reg_msp430(Simulate *simulate, char *reg_string, uint32_t value);
uint32_t simulate_get_reg_msp430(Simulate *simulate, char *reg_string);
void simulate_set_pc_msp430(Simulate *simulate, uint32_t value);
void simulate_reset_msp430(Simulate *simulate);
void simulate_dump_registers_msp430(Simulate *simulate);
int simulate_run_msp430(Simulate *simulate, int max_cycles, int step);

#endif

