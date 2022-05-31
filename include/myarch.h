#ifndef myarch_header
#define myarch_header
#define _GNU_SOURCE
#include <stdio.h>
#include <stdint.h>

typedef uint64_t UInt;

typedef enum {
  id_int,
  id_reg
} ValueId;

typedef enum {
  op_load,
  op_end,
  op_print,
  op_mul,
  op_add,
  op_sub,
  op_div,
  op_rem,
  op_ge,
  op_eq,
  op_lt,
  op_goto,
  op_jumpbar,
  op_getpc,
  op_bnez,
  op_put,
  op_and,
  op_or
} Op;

typedef struct {
  Op Op;
  ValueId Id;
  UInt Val;
} Statement;

Statement* read_program(FILE*, UInt*);

UInt run_program(Statement*, UInt);

#endif /* myarch_header */