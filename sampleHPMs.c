// Read per-second CSRs and compare the values over time and workload
// (c) July 2020, Maddie Burbage

#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <time.h>

//*** PARAMETERS ***
// The CSR to test
#if !defined(IPS) && !defined(RPS) && !defined(SPS)
#define SPS
#endif

// How many seconds between each sample
#define DURATION 1

// How many samples to run
#define REPETITIONS 30

// CSR-sampling code
#include "timeCycles.c"


/** Print the results of a CSR's trials
 * results: the longs retrieved from the CSR
 */
void printTable(long unsigned *results) {
  int i;
  long unsigned perSecond;
  int minPerSecond = INT_MAX;
  int avgPerSecond = 0;
  int maxPerSecond = 0;
  char *header = PRETTYMODE;

  // Headers
  printf("%s: \n", header);
  printf("** %-15s %-15s %-15s **\n", "Trial", "Total", "Per Second");

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

    printf("   %-15d %-15lu %-15lu   \n", i, results[i], perSecond);
  }
  avgPerSecond /= REPETITIONS - 1; // Calculate average

  // Overall stats
  printf("Minimum %s per second: %d \n", header, minPerSecond);
  printf("Average %s per second: %d \n", header, avgPerSecond);
  printf("Maximum %s per second %d \n", header, maxPerSecond);

}

int controlComputations (const char *path) {
  char *tokenizable = strdup(path);
  char *nextProgram = strtok(tokenizable, "/");
  char *program;
  pid_t cpid;
  time_t start, finish;

  start = time(NULL);
  finish = start + REPETITIONS * DURATION;
  while(nextProgram != NULL) { // Get program name from path
    program = nextProgram;
    nextProgram = strtok(NULL, "/");
  }
  free(tokenizable);

  do {
    cpid = fork();
    if(cpid == 0) { // Child runs a function
      printf("child running %s %s \n", path, program);
      execvp(path, &program);
    } else if(cpid < 0) { // Parent waits for child before starting another
      return -1;
    } else {
      waitpid(cpid, 0,0);
    }
  } while(cpid != 0 && time(NULL) < finish);
  return 0;
}

int main(int argc, char *argv[]) {
  long unsigned measured[REPETITIONS];

  if(argc < 2) {
    fprintf(stderr, "Usage: %s path\n", argv[0]);
    fprintf(stderr, "path: the relative path of a program to loop over\n");
    exit(-1);
  }

  pid_t pid = fork(); // Child process to control computation/RoCC
  if(pid == 0) { //Child
    controlComputations(argv[1]);
  } else {
    trackCSR(measured);
    waitpid(pid, 0, 0);
    printTable(measured);
  }
}
