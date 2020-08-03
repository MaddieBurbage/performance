// Calculate first set of primes
// (c) August 2020, Maddie Burbage

#include <stdio.h>
#include <math.h>

#define TOTALPRIMES 100

int main(int argc, char *argv[]) {
  int i = 1;
  int j = 0;
  int p = 0;
  int isPrime;
  int primes[TOTALPRIMES];
  double root;

  while(p != TOTALPRIMES) {
    i++;
    root = sqrt(i);
    isPrime = 1;
    for(j = 2; j <= root; j++) {
      if(i % j == 0) {
        isPrime = 0;
        break;
      }
    }
    if(isPrime) {
      primes[p++] = i;
    }
  }
  printf("Primes found: 1st: %d 50th: %d 100th: %d \n", primes[0], primes[49], primes[99]);
}
