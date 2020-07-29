// View a RISC-V processor's hardware counters over a range of seconds
// (c) July 2020 Maddie Burbage

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <limits.h>
#include "encoding.h"


//*** PARAMETERS ***
// The CSR to test
#if !defined(CYCLE) || !defined(TIME) || !defined(INSTRET)
#define INSTRET
#endif

// How many seconds should be tested over
#define DURATION 30

// How many tests to run
#define REPETITIONS 5


//*** CONSTANTS ***
// CSR readable-addresses (accessible to the User-level)
#define CYCLEADDR 0xc00
#define TIMEADDR 0xc01
#define INSTADDR 0xc01

// Stringify macro address
#define readADDR(addr) read_csr(addr)

// Read address depending on the mode
#ifdef CYCLE
#define readCSR readADDR(CYCLEADDR)
#define PRETTYMODE "cycle"
#elif TIME
#define readCSR readADDR(TIMEADDR)
#define PRETTYMODE "time"
#else
#define readCSR readADDR(INSTADDR)
#define PRETTYMODE "instret"
#endif


/** Read a CSR every DURATION seconds, REPETITION times
 * csrValues: the array to store results to
 */
void trackCSR(unsigned long *csrValues) {
  int i = 0;
  unsigned int leftover;
  
  // Try to be as accurate to the second-count as possible within here
  csrValues[i++] = readCSR;
  for( ; i < REPETITIONS; i++) {
    leftover = sleep(DURATION);
    csrValues[i] = readCSR;
    
    if(leftover) { // Redo this trial if sleep was interrupted
      fprintf(stderr, "Error: sleep got interrupted \n");
      i--;
    }
  }
}

/** Print the results of a CSR's trials
 * results: the longs retrieved from the CSR
 */
void printTable(long unsigned *results) {
  int i = 0;
  long unsigned elapsed;
  long unsigned perSecond;
  int minPerSecond = INT_MAX;
  int avgPerSecond = 0;
  char *header = PRETTYMODE;

  // Headers
  printf("%s: \n", header);
  printf("** %-15s %-15s %-15s %-15s **\n", "Trial", "Total", "Elapsed", "Per Second");
  
  // Data
  printf("   %-15d %-15lu %-15s %-15s   \n", i++, results[0], "---", "---");
  for( ; i < REPETITIONS; i++) {
    elapsed = results[i] - results[i-1];
    perSecond = elapsed / DURATION;
    if(perSecond < minPerSecond) {
      minPerSecond = perSecond;
    }
    avgPerSecond += perSecond;
    
    printf("   %-15d %-15lu %-15lu %-15lu   \n", i, results[i], elapsed, perSecond);
  }
  avgPerSecond /= REPETITIONS - 1; // Calculate average

  // Overall stats
  printf("Minimum %s per second: %d  Average %s per second: %d \n", header, minPerSecond, header, avgPerSecond);
  
}
  
int main(int argc, char *argv[]) {
  long unsigned measured[REPETITIONS];

  trackCSR(measured);
  printTable(measured);
    
}
