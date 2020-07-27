# performance
Tracking hardware performance of the lowRISC processor and our extensions of it. Right now these files focus on interfacing the lowRISC Rocket Chip's CSRs.

---

## Files
- `encoding.h` is source from [Spike](https://github.com/riscv/riscv-isa-sim/blob/master/riscv/encoding.h), providing handy macros for RISC-V instructions within C.
- `readCSR.c` will read from a single CSR address and print the result.
- `timeCycles.c` retrieves the values of instret (instructions retired), time, and cycle, to see their changes over each second.


## Uses
`readCSR` is for brief testing of a CSR, or validation to see if a custom CSR is initializing correctly.

`timeCycles` is how we'll obtain a 'cycles per second' estimate, since on the rocket chip the time and cycle CSRs are linked together. 
We can obtain an estimate of instructions per second as well.
