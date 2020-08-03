// View a RISC-V processor's hardware counters over a range of seconds
// (c) July 2020, Maddie Burbage

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <limits.h>
#include "encoding.h"

//*** CONSTANTS ***
// CSR readable-addresses (accessible to the User-level)
#define CYCLEADDR 0xc00
#define TIMEADDR 0xc01
#define INSTADDR 0xc02

// Custom HPM per-second readable addresses
#define IPSADDR 0x800 //Instructions retired
#define RPSADDR 0x801 //New RoCC instructions
#define SPSADDR 0x802 //Stalled-by-RoCC cycles

// Stringify macro address
#define readADDR(addr) read_csr(addr)

// Read address depending on the mode
#ifdef CYCLE
#define readCSR readADDR(CYCLEADDR)
#define PRETTYMODE "cycle"
#elif defined(TIME)
#define readCSR readADDR(TIMEADDR)
#define PRETTYMODE "time"
#elif defined(INSTRET)
#define readCSR readADDR(INSTADDR)
#define PRETTYMODE "instret"
#elif defined(IPS)
#define readCSR readADDR(IPSADDR)
#define PRETTYMODE "Instructions"
#elif defined(RPS)
#define readCSR readADDR(RPSADDR)
#define PRETTYMODE "RoCC instructions"
#elif defined(SPS)
#define readCSR readADDR(SPSADDR)
#define PRETTYMODE "RoCC stalls"
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
