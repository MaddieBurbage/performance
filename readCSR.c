// Read the value of a RISCV CSR
// (c) July 2020 Maddie Burbage

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "encoding.h"

// *** PARAMS ***
#ifndef ADDR
#define ADDR 0xc00 // address of the CSR to read
#endif

// Macros for allowing ADDR to be stringified
#define readCSR(addr) read_csr(addr)
#define readAddress readCSR(ADDR)

/** Read and print a single CSR's value
 * returns: the CSR's value
 */
long readFromCSR() {
  long csrValue = readAddress;
  printf("Value at %d: %lu \n", ADDR, csrValue);
  return csrValue;
}

int main(int argc, char* argv[]) {
  int repetitions;
  
  if(argc < 2) {
    fprintf(stderr, "Usage: %s repetitions\n", argv[0]);
    fprintf(stderr, "repetitions: how many seconds in a row to check the values\n");
    exit(-1);
  }
  repetitions = atoi(argv[1]);

  //Read from the CSR repetitions times
  for(int i = 0; i < repetitions; i++) {
    sleep(1);
    readFromCSR();
  }
  
}
