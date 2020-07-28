// Read the value of a RISCV CSR
// (c) July 2020 Maddie Burbage

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "encoding.h"

/** Read and print a single CSR's value
 * address: the CSR address to view (defined in rocket/Instructions.scala)
 * returns: the CSR's value
 */
long readFromCSR(long address) {
  long csrValue;
  asm volatile ("csrr %[rd], %[csr]": [rd]"=r"(csrValue) : [csr]"r"(address));
  printf("Value at %lu: %lu \n", address, csrValue);
  return csrValue;
}

int main(int argc, char* argv[]) {
  long csrValue = 0;
  unsigned long address;
  char *endptr;
  int repetitions;
  
  if(argc < 2) {
    fprintf(stderr, "Usage: %s address [repetitions]\n", argv[0]);
    fprintf(stderr, "address: a hex (0x__) octal (00__) or decimal CSR address\n");
    fprintf(stderr, "repetitions: how many seconds in a row to check the values\n");
    exit(-1);
  }

  //Convert arg 2 to a long (base 16 8 or 10)
  address = strtol(argv[1], &endptr, 0); 
  if(endptr == argv[1]) {
    fprintf(stderr, "Error: no digits found\n");
    exit(-1);
  }

  if(argc == 3) {
    repetitions = atoi(argv[2]);
  }

  //Read from the CSR repetitions times
  for(int i = 0; i < repetitions; i++) {
    sleep(1);
    readFromCSR(address);
  }
  
}
