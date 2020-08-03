// Calculate first set of primes
// (c) August 2020, Maddie Burbage

#include <math.h>

#define TOTALPRIMES 100

int main(int argc, char *argv[]) {
  int i = 0;
  int j = 0;
  int p = 0;
  int isPrime;
  int primes[TOTALPRIMES];
  double root;

  while(p != TOTALPRIMES) {
    i++;
    root = sqrt(i);
    isPrime = 1;
    for(j = 1; j < root; j++) {
      if(i % j == 0) {
        isPrime = 0;
        break;
      }
    }
    if(isPrime) {
      primes[p++] = i;
    }
  }
}
