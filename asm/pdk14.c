/**
 *  naken_asm assembler.
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: https://www.mikekohn.net/
 * License: GPLv3
 *
 * Copyright 2010-2022 by Michael Kohn
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "asm/pdk14.h"
#include "asm/common.h"
#include "common/assembler.h"
#include "common/tokens.h"
#include "common/eval_expression.h"
#include "table/pdk14.h"

#define MAX_OPERANDS 2

enum
{
  OPERAND_NUMBER,
  OPERAND_A,
  OPERAND_BIT_OFFSET,
};

struct _operand
{
  int value;
  int type;
  int bit;
};

int parse_instruction_pdk14(struct _asm_context *asm_context, char *instr)
{
  char instr_case_mem[TOKENLEN];
  char *instr_case = instr_case_mem;
  char token[TOKENLEN];
  struct _operand operands[MAX_OPERANDS];
  int operand_count = 0;
  int token_type;
  int num, n;
  uint16_t opcode;
  int matched = 0;

  if (asm_context->pass == 1)
  {
    ignore_line(asm_context);

    add_bin16(asm_context, 0, IS_OPCODE);
    return 2;
  }

  lower_copy(instr_case, instr);
  memset(&operands, 0, sizeof(operands));

  while (1)
  {
    token_type = tokens_get(asm_context, token, TOKENLEN);

    if (token_type == TOKEN_EOL || token_type == TOKEN_EOF)
    {
      if (operand_count != 0)
      {
        print_error_unexp(token, asm_context);
        return -1;
      }
      break;
    }

    if (IS_TOKEN(token, 'a') || IS_TOKEN(token, 'A'))
    {
      operands[operand_count].type = OPERAND_A;
    }
      else
    {
      tokens_push(asm_context, token, token_type);

      if (eval_expression(asm_context, &num) != 0)
      {
        if (asm_context->pass == 1)
        {
          ignore_operand(asm_context);
          num = 0;
        }
          else
        {
          print_error_unexp(token, asm_context);
          return -1;
        }
      }

      operands[operand_count].value = num;
      operands[operand_count].type = OPERAND_NUMBER;

      token_type = tokens_get(asm_context, token, TOKENLEN);

      if (IS_TOKEN(token, '.'))
      {
        if (eval_expression(asm_context, &num) != 0)
        {
          if (asm_context->pass == 1)
          {
            ignore_operand(asm_context);
            num = 0;
          }
            else
          {
            print_error_unexp(token, asm_context);
            return -1;
          }
        }

        operands[operand_count].bit = num;
        operands[operand_count].type = OPERAND_BIT_OFFSET;
      }
        else
      {
        tokens_push(asm_context, token, token_type);
      }
    }

    operand_count++;
    token_type = tokens_get(asm_context, token, TOKENLEN);

    if (token_type == TOKEN_EOL) { break; }
    if (IS_NOT_TOKEN(token, ',') || operand_count == 2)
    {
      print_error_unexp(token, asm_context);
      return -1;
    }
  }

  for (n = 0; table_pdk14[n].instr != NULL; n++)
  {
    if (strcmp(table_pdk14[n].instr, instr_case) == 0)
    {
      matched = 1;

      switch (table_pdk14[n].type)
      {
        case OP_NONE:
        {
          if (operand_count != 0) { continue; }

          add_bin16(asm_context, table_pdk14[n].opcode, IS_OPCODE);

          return 2;
        }
        case OP_A:
        {
          if (operand_count != 1 || operands[0].type != OPERAND_A)
          {
            continue;
          }

          add_bin16(asm_context, table_pdk14[n].opcode, IS_OPCODE);

          return 2;
        }
        case OP_IO_A:
        {
          if (operand_count != 2 ||
              operands[0].type != OPERAND_NUMBER ||
              operands[1].type != OPERAND_A)
          {
            continue;
          }

          if (operands[0].value < 0 || operands[0].value > 0x3f)
          {
            print_error_range("IO", 0, 0x3f, asm_context);
            return -1;
          }

          opcode = table_pdk14[n].opcode | (operands[0].value & 0x3f);
          add_bin16(asm_context, opcode, IS_OPCODE);

          return 2;
        }
        case OP_A_IO:
        {
          if (operand_count != 2 ||
              operands[0].type != OPERAND_A ||
              operands[1].type != OPERAND_NUMBER)
          {
            continue;
          }

          if (operands[1].value < 0 || operands[1].value > 0x3f)
          {
            print_error_range("IO", 0, 0x3f, asm_context);
            return -1;
          }

          opcode = table_pdk14[n].opcode | (operands[1].value & 0x3f);
          add_bin16(asm_context, opcode, IS_OPCODE);

          return 2;
        }
        case OP_M6:
        {
          if (operand_count != 1 || operands[0].type != OPERAND_NUMBER)
          {
            continue;
          }

          if (operands[0].value < 0 || operands[0].value > 0x7f)
          {
            print_error_range("address", 0, 0x7f, asm_context);
            return -1;
          }

          if ((operands[0].value & 1) != 1)
          {
            print_error_align(asm_context, 2);
            return -1;
          }

          opcode = table_pdk14[n].opcode | (operands[0].value & 0x7f);
          add_bin16(asm_context, opcode, IS_OPCODE);

          return 2;
        }
        case OP_M:
        {
          if (operand_count != 1 || operands[0].type != OPERAND_NUMBER)
          {
            continue;
          }

          if (operands[0].value < 0 || operands[0].value > 0x7f)
          {
            print_error_range("address", 0, 0x7f, asm_context);
            return -1;
          }

          opcode = table_pdk14[n].opcode | (operands[0].value & 0x7f);
          add_bin16(asm_context, opcode, IS_OPCODE);

          return 2;
        }
        case OP_A_M6:
        {
          if (operand_count != 2 ||
              operands[0].type != OPERAND_A ||
              operands[1].type != OPERAND_NUMBER)
          {
            continue;
          }

          if (operands[1].value < 0 || operands[1].value > 0x7f)
          {
            print_error_range("address", 0, 0x7f, asm_context);
            return -1;
          }

          if ((operands[1].value & 1) != 1)
          {
            print_error_align(asm_context, 2);
            return -1;
          }

          opcode = table_pdk14[n].opcode | (operands[1].value & 0x7f);
          add_bin16(asm_context, opcode, IS_OPCODE);

          return 2;
        }
        case OP_M6_A:
        {
          if (operand_count != 2 ||
              operands[0].type != OPERAND_NUMBER ||
              operands[1].type != OPERAND_A)
          {
            continue;
          }

          if (operands[0].value < 0 || operands[0].value > 0x7f)
          {
            print_error_range("address", 0, 0x7f, asm_context);
            return -1;
          }

          if ((operands[0].value & 1) != 1)
          {
            print_error_align(asm_context, 2);
            return -1;
          }

          opcode = table_pdk14[n].opcode | (operands[0].value & 0x7f);
          add_bin16(asm_context, opcode, IS_OPCODE);

          return 2;
        }
        case OP_A_M:
        {
          if (operand_count != 2 ||
              operands[0].type != OPERAND_A ||
              operands[1].type != OPERAND_NUMBER)
          {
            continue;
          }

          if (operands[1].value < 0 || operands[1].value > 0x7f)
          {
            print_error_range("address", 0, 0x7f, asm_context);
            return -1;
          }

          opcode = table_pdk14[n].opcode | (operands[1].value & 0x7f);
          add_bin16(asm_context, opcode, IS_OPCODE);

          return 2;
        }
        case OP_M_A:
        {
          if (operand_count != 2 ||
              operands[1].type != OPERAND_NUMBER ||
              operands[0].type != OPERAND_A)
          {
            continue;
          }

          if (operands[0].value < 0 || operands[0].value > 0x7f)
          {
            print_error_range("address", 0, 0x7f, asm_context);
            return -1;
          }

          opcode = table_pdk14[n].opcode | (operands[0].value & 0x7f);
          add_bin16(asm_context, opcode, IS_OPCODE);

          return 2;
        }
        case OP_A_K:
        {
          if (operand_count != 2 ||
              operands[0].type != OPERAND_A ||
              operands[1].type != OPERAND_NUMBER)
          {
            continue;
          }

          if (operands[1].value < -128 || operands[1].value > 0xff)
          {
            print_error_range("address", -128, 0xff, asm_context);
            return -1;
          }

          opcode = table_pdk14[n].opcode | (operands[1].value & 0xff);
          add_bin16(asm_context, opcode, IS_OPCODE);

          return 2;
        }
        case OP_IO_N:
        case OP_M_N:
        {
          if (operand_count != 1 || operands[0].type != OPERAND_BIT_OFFSET)
          {
            continue;
          }

          if (operands[0].value < 0 || operands[0].value > 0x3f)
          {
            print_error_range(
              table_pdk14[n].type == OP_IO_N ? "io" : "address",
              0, 0x3f, asm_context);
            return -1;
          }

          if (operands[0].bit < 0 || operands[0].bit > 8)
          {
            print_error_range("bit_offset", 0, 8, asm_context);
            return -1;
          }

          opcode =
            table_pdk14[n].opcode |
           (operands[0].bit << 6) |
            operands[0].value;
          add_bin16(asm_context, opcode, IS_OPCODE);

          return 2;
        }
        case OP_K8:
        {
          if (operand_count != 1 || operands[0].type != OPERAND_NUMBER)
          {
            continue;
          }

          if (operands[0].value < -128 || operands[0].value > 255)
          {
            print_error_range("Literal", -128, 255, asm_context);
            return -1;
          }

          opcode = table_pdk14[n].opcode | (operands[0].value & 0xff);
          add_bin16(asm_context, opcode, IS_OPCODE);

          return 2;
        }
        case OP_K11:
        {
          if (operand_count != 1 || operands[0].type != OPERAND_NUMBER)
          {
            continue;
          }

          if (operands[0].value < -1024 || operands[0].value > 2047)
          {
            print_error_range("Literal", -1024, 2047, asm_context);
            return -1;
          }

          opcode = table_pdk14[n].opcode | (operands[0].value & 0x7ff);
          add_bin16(asm_context, opcode, IS_OPCODE);

          return 2;
        }
        default:
          break;
      }
    }
  }

  if (matched == 1)
  {
    print_error_unknown_operand_combo(instr, asm_context);
  }
    else
  {
    print_error_unknown_instr(instr, asm_context);
  }

  return -1;
}

