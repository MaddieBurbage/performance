// View a RISC-V processor's hardware counters over a range of seconds
// (c) July 2020 Maddie Burbage

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <limits.h>
#include "encoding.h"


//*** PARAMETERS ***
// How many seconds should be tested over
#define DURATION 30

// How many tests to run
#define REPETITIONS 5


//*** CONSTANTS ***
// CSR readable-addresses (accessible to the User-level)
#define CYCLEADDR 0xc00
#define TIMEADDR 0xc01
#define INSTADDR 0xc01

#define MODES {"cycle", "time", "instret"}
#define MODENUM 3



/** Read a CSR every DURATION seconds, REPETITION times
 * address: the CSR's address to read from
 * csrValues: the array to store results to
 */
void trackCSR(int address, unsigned long *csrValues) {
  int i = 0;
  unsigned int leftover;
  
  // Try to be as accurate to the second-count as possible within here
  csrValues[i++] = read_csr(address);
  for( ; i < REPETITIONS; i++) {
    leftover = sleep(DURATION);
    csrValues[i] = read_csr(address);
    
    if(leftover) { // Redo this trial if sleep was interrupted
      fprintf(stderr, "Error: sleep got interrupted \n");
      i--;
    }
  }
}

/** Print the results of a CSR's trials
 * results: the longs retrieved from the CSR
 * header: the CSR's name
 */
void printTable(long unsigned *results, char *header) {
  int i = 0;
  long unsigned elapsed;
  long unsigned perSecond;
  int minPerSecond = INT_MAX;
  int avgPerSecond = 0;

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
  int i = 0;
  long unsigned measured[REPETITIONS];
  char *modes[] = MODES;
  char first;
  char modeFirst;
  int all;

  if(argc < 2) {
    fprintf(stderr, "Usage: %s csr\n", argv[0]);
    fprintf(stderr, "csr: the csr to track (cycle, time, instret, all)\n");
    exit(-1);
  }
  // Find the mode from arg 1's first character
  first = argv[1][0];
  all = (first == 'a');

  // Run tests for each mode selected
  for( ; i < MODENUM; i++) {
    modeFirst = modes[i][0];
    if(first == modeFirst || all) {
      trackCSR(CYCLEADDR, measured);
      printTable(measured, modes[i]);
    }
  }
    
}
