// Find HPM values per second by averaging over a long period of time
// (c) July 2020, Maddie Burbage



//*** PARAMETERS ***
// The CSR to test
#if !defined(CYCLE) && !defined(TIME) && !defined(INSTRET)
#define INSTRET
#endif

// How many seconds should be tested over
#define DURATION 30

// How many tests to run
#define REPETITIONS 5

// Which CSR to test: CYCLE, TIME, INSTRET

// CSR-sampling code
#include "timeCycles.c"


/** Print the results of a CSR's trials
 * results: the longs retrieved from the CSR
 */
void printTable(long unsigned *results) {
  int i = 0;
  long unsigned elapsed;
  long unsigned perSecond;
  int minPerSecond = INT_MAX;
  int avgPerSecond = 0;
  int maxPerSecond = 0;
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
    if(perSecond > maxPerSecond) {
      maxPerSecond = perSecond;
    }
    avgPerSecond += perSecond;

    printf("   %-15d %-15lu %-15lu %-15lu   \n", i, results[i], elapsed, perSecond);
  }
  avgPerSecond /= REPETITIONS - 1; // Calculate average

  // Overall stats
  printf("Minimum %s per second: %d \n", header, minPerSecond);
  printf("Average %s per second: %d \n", header, avgPerSecond);
  printf("Maximum %s per second %d \n", header, maxPerSecond);

}

int main(int argc, char *argv[]) {
  long unsigned measured[REPETITIONS];

  trackCSR(measured);
  printTable(measured);

}
