// Read the value of a custom-CSR
// (c) 2020 Maddie Burbage

#include <stdio.h>
#include "encoding.h"

int main(void) {
  long csrValue = 0;
  csrValue = read_csr(0x800);
  printf("Value at 0x800: %lu \n", csrValue);
}
