#include "myarch.h"

int main(int argc, char* argv[argc]) {
  if (argc <= 1) return 1;
  FILE* f = fopen(argv[1], "r");
  UInt size = 0;
  Statement* ops = read_program(f, &size);
  printf("Starting run\n");
  UInt rtn = run_program(ops, size);
  printf("%lu\n", rtn);
}