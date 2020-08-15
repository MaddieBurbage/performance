# performance
Tracking hardware performance of the lowRISC processor and our extensions of it. Right now these files focus on interfacing the lowRISC Rocket Chip's CSRs.

---

## Contents
- `accelerators` contains the source and tests for accelerators on chipyard and lowRISC. Right now the only accelerator is MemTest, which usees RoCC instructions to test out the RoCC mem interface.
- `encoding.h` is source from [riscv-opcodes](https://github.com/riscv/riscv-opcodes) as part of riscv-tools, providing handy macros for RISC-V instructions within C.
- `rocc.h` is source from Chipyard's example [tests](https://github.com/ucb-bar/chipyard/tree/master/tests).
- `readCSR.c` will read from a single CSR address and print the result.
- `averageHPMs.c` retrieves the values of instret (instructions retired), time, and cycle, and calculates their changes over each second, while another process runs.
- `sampleHPMs.c` retrieves the values of custom per-second HPMs: IPS (instructions), RPS (RoCC instructions), and SPS (RoCC-stalled cycles). 
- `timeCycles.c` has methods for reading from CSR registers.
- `primes.c` loops through finding the first set of prime numbers (useful with averageHPMs).


## Uses
`readCSR` is for brief testing of a CSR, or validation to see if a custom CSR is initializing correctly.

`averageHPMs` is how we'll obtain a 'cycles per second' estimate, since on the rocket chip the time and cycle CSRs are linked together. 
We can obtain an estimate of instructions per second as well.

`sampleHPMs` is for monitoring the pipeline over time while the processor handles different tasks.
