// Read per-second CSRs and compare the values over time and workload
// (c) July 2020, Maddie Burbage

#include <sys/types.h>
#include <sys/wait.h>

//*** PARAMETERS ***
// The CSR to test
#if !defined(CYCLE) && !defined(TIME) && !defined(INSTRET)
#define INSTRET
#endif

// How many seconds between each sample
#define DURATION 1

// How many samples to run
#define REPETITIONS 30

// Which CSR to test: IPS, RPS, SPS

// CSR-sampling code
#include "timeCycles.c"


/** Print the results of a CSR's trials
 * results: the longs retrieved from the CSR
 */
void printTable(long unsigned *results) {
  int i;
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
  for(i = 0; i < REPETITIONS; i++) {
    perSecond = results[i];
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

int controlComputations (char *program) {
  char *path = "./" + program
  do {
    pid_t cpid = fork();
    if(cpid == 0) { // Grandchild runs a function
      execv(&path, &program);
    } else if(cpid < 0) {
      return -1;
    } else {
      waitpid(cpid, 0,0);
    }
  } while(cpid != 0);
  return 0;
}

int main(int argc, char *argv[]) {
  long unsigned measured[REPETITIONS];

  if(argc < 2) {
    fprintf(stderr, "Usage: %s program", argv[0]);
    fprintf(stderr, "program: the name of the same-directory file to run");
  }

  pid_t pid = fork(); // Child process to control computation/RoCC
  if(pid == 0) { //Child
    controlComputations(argv[1]);
  } else {
    trackCSR(measured);
    waitpid(pid, 0, 0);
    printTable(measured);


}
