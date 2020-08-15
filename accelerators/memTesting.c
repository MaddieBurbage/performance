// Tests for memory accesses
// (c) Maddie Burbage, 2020

#include "rocc.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TESTSIZE 20
#define DATA int
#define TYP 2

int main(void) {
  DATA memArray[TESTSIZE];
  int i;
  DATA response = -1;
  
  for(i = 0; i < TESTSIZE; i++) {
    ROCC_INSTRUCTION_DSS(0, response, &memArray[i], TYP, 1);
    printf("Received %d from the command %d \n", response, 1);
  }
  for(i = 0; i < TESTSIZE; i++) {
    ROCC_INSTRUCTION_DSS(0, response, &memArray[i], TYP, 0);
    printf("Received %d from the command %d \n", response, 0);
  }
  for(i = 0; i < TESTSIZE; i++) {
    printf("Saw %d at spot %d \n", memArray[i], i);
  }
}
