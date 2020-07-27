// Read the value of a custom-CSR
// (c) 2020 Maddie Burbage

#include <stdio.h>
#include <unistd.h>
#include "encoding.h"

int main(void) {
  long csrValue[30];
  int address[] = {0xc01, 0xc02, 0xc00};
  printf("Finds the values at time, instret, and cycle");
  for(int i = 0; i < 10; i++) {
    csrValue[i] = read_csr(0xc01);
    csrValue[2*i] = read_csr(0xc02);
    csrValue[3*i] = read_csr(0xc00);
    printf("%dth value is %lu %lu %lu \n", i, csrValue[i], csrValue[2*i], csrValue[3*i]);
    sleep(1);
  }


}
