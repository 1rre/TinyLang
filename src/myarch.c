#include "myarch.h"
#include <stdlib.h>

UInt is_ws(char c) {
  return c == '\n' || c == ' ' || c == '\r' || c == '\t';
}

UInt is_digit(char c) {
  return c >= '0' && c <= '9';
}

UInt run_program(Statement* sts, UInt max_idx) {
  UInt pc = 0;
  UInt acc = 0;
  UInt reg[128];
  while (pc < max_idx) {
    UInt x = sts[pc].Id? reg[sts[pc].Val] : sts[pc].Val;
    switch (sts[pc].Op) {
      case op_load: acc = x; break;
      case op_end: return acc;
      case op_print: printf("%lu\n", acc); break;
      case op_mul: acc *= x; break;  
      case op_add: acc += x; break;  
      case op_sub: acc -= x; break;  
      case op_div: acc /= x; break;  
      case op_rem: acc %= x; break;  
      case op_ge: acc = acc >= x; break;
      case op_lt: acc = acc < x; break;
      case op_goto: pc = sts[pc].Id? x + 1 : acc + 1; break;
      case op_jumpbar: break;
      case op_getpc: acc = pc; break;
      case op_bnez:
        if (acc) {
          if (x) pc = x;
          else {
            x = pc;
            while (pc < max_idx && sts[pc].Op != op_jumpbar) pc++;
            sts[x].Val = pc;
          }
        }
      break;
      case op_put: reg[sts[pc].Val] = acc; break;
      case op_and: acc = acc && x; break;
      case op_or: acc = acc || x; break;
      case op_eq: acc = acc == x; break;
      default: printf("Op was %d\n", sts[pc].Op); exit(1);        
    }
    pc++;
  }
}

Statement* read_program(FILE* fp, UInt* size) {
  int c;
  Statement* rtn = 0;
  *size = 0;
  Statement cs = {0, 0, 0};
skip_ws_1:
  c = fgetc(fp);
init:
  if (is_ws(c)) goto skip_ws_1;
  else if (is_digit(c) || c == 'x') {
    cs.Op = op_load;
    goto parse_value;
  }
  
  switch (c) {
    case '?':
      cs.Op = op_bnez;
      cs.Val = 0;
      cs.Id = id_int;
      c = fgetc(fp);
      goto extend_rtn;
    case '!':
      cs.Op = op_jumpbar;
      c = fgetc(fp);
      goto extend_rtn;
    case '~':
      cs.Op = op_end; 
      c = fgetc(fp);
      goto extend_rtn;
    case '"':
      cs.Op = op_getpc;
      c = fgetc(fp);
      goto extend_rtn;
    case '\'':
      cs.Op = op_print;
      c = fgetc(fp);
      goto extend_rtn;
    case '#': cs.Op = op_goto; break;
    case ':': cs.Op = op_put; break;
    case '>': cs.Op = op_ge; break;
    case '<': cs.Op = op_lt; break;
    case '+': cs.Op = op_add; break;
    case '-': cs.Op = op_sub; break;
    case '*': cs.Op = op_mul; break;
    case '/': cs.Op = op_div; break;
    case '%': cs.Op = op_rem; break;
    case '=': cs.Op = op_eq; break;
    case -1: return rtn;
    default:
      printf("Issue @ %c (%d)\n", c, c);
    exit(1);
  }

skip_ws:
  c = fgetc(fp);
  if (is_ws(c)) goto skip_ws;

parse_value:
  {
    int x = 0;
    int init = c;
    if (init == 'x') {
      c = fgetc(fp);
      if (!is_digit(c)) {
        printf("Syntax Error @ %c\n", c);
        exit(1);
      }
    }
    do {
      x *= 10;
      x += (c - '0');
    } while (is_digit(c = fgetc(fp)));
    cs.Id = init == 'x'? id_reg : id_int;
    cs.Val = x;
  }

extend_rtn:
  (*size)++;
  rtn = realloc(rtn, sizeof(Statement) * *size);
  rtn[*size - 1] = cs;
  goto init;
}
