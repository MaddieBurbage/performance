// Tests for memory accesses
// (c) Maddie Burbage, 2020

#include "rocc.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TESTSIZE 20
#define DATA int
#define TYP 3

static inline void testAccelerator(DATA *array, int command) {
  int i = 0;
  int response = -1;
  for(; i < TESTSIZE; i++) {
    ROCC_INSTRUCTION_DSS(0, response, array[i], TYP, command);
    printf("Received %d from the command %d \n", response, command);
  }
}

int main(void) {
  DATA memArray[TESTSIZE];
  int i;

  testAccelerator(memArray, 1);
  testAccelerator(memArray, 0);
  for(i = 0; i < TESTSIZE; i++) {
    printf("Saw %d at spot %d \n", memArray[i], i);
  }
}
