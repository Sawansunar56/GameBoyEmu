#include "emu.h"

// void printBits(u64 n)
// {
//  for (int i = 63; i >= 0; --i)
//  {
//   printf("%llu", (n >> i) & 1);
//   if (i % 8 == 0 && i != 0)
//    printf(" "); // optional: add space every byte
//  }
//  printf("\n");
// }

int main(int argc, char *argv[]) { return emu_run(argc, argv); }
