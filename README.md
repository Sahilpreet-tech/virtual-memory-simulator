# Virtual Memory Management Simulator

> A terminal-based C simulator for **paging** and **segmentation** with rich
> step-by-step visualisation, page-replacement algorithm comparison, and
> fragmentation analysis.

---

## Features

| Module | Capabilities |
|---|---|
| **FIFO** | Step-by-step frame state, fault/hit per reference, summary stats |
| **LRU** | Timestamp-based victim selection, hit ratio |
| **Optimal** | Belady's algorithm, theoretical lower bound on faults |
| **Compare All** | Side-by-side fault/hit table, highlights best algorithm |
| **Segmentation** | First-fit allocation, ASCII memory map, external fragmentation, compaction insight, logical→physical address translation demo |

---

## Project Structure

```
virtual_memory/
├── main.c          – Entry point, menu, input handling
├── paging.c        – FIFO, LRU, Optimal implementations
├── paging.h        – Paging function declarations
├── segmentation.c  – Segmentation simulator
├── segmentation.h  – Segmentation function declarations
└── README.md       – This file
```

---

## How to Build & Run

### Linux / macOS
```bash
gcc main.c paging.c segmentation.c -o vm
./vm
```

### Windows (MinGW)
```bash
gcc main.c paging.c segmentation.c -o vm.exe
vm.exe
```

> **Note:** ANSI colour codes require a terminal that supports them
> (Linux/macOS terminal, Windows Terminal, Git Bash, or VS Code terminal).

---

## Sample Interaction

```
╔══════════════════════════════════════════════╗
║      VIRTUAL MEMORY MANAGEMENT SIMULATOR    ║
║   Paging · Segmentation · Page Replacement  ║
╚══════════════════════════════════════════════╝

  ┌─────────────────────────────────┐
  │  Page Replacement Algorithms    │
  │  1. FIFO  (First-In First-Out)  │
  │  2. LRU   (Least Recently Used) │
  │  3. OPT   (Optimal / Belady's)  │
  │  4. Compare All Three           │
  ├─────────────────────────────────┤
  │  Segmentation                   │
  │  5. Segmentation Simulator      │
  ├─────────────────────────────────┤
  │  6. Exit                        │
  └─────────────────────────────────┘
  Choice: 4

  Enter number of page references (max 100): 12
  Enter the page reference string:
  > 7 0 1 2 0 3 0 4 2 3 0 3
  Enter number of frames (1–20): 3
```

---

## Algorithms Explained

### FIFO (First-In First-Out)
Replaces the page that has been in memory the longest.  Simple to implement
but suffers from **Belady's Anomaly** (more frames can cause more faults).

### LRU (Least Recently Used)
Replaces the page that was least recently accessed.  Approximates the
optimal policy and does **not** suffer from Belady's Anomaly.

### Optimal / Belady's Algorithm
Replaces the page that will not be used for the longest time in the future.
Requires future knowledge, so it cannot be used in practice – but serves as
a benchmark lower bound.

### Segmentation
Divides a process's logical address space into variable-length **segments**
(Code, Stack, Heap, Data, BSS …).  Each segment has a **base** (physical
start address) and a **limit** (size).  A logical address `(segment, offset)`
maps to physical address `base + offset`, provided `offset < limit`;
otherwise a **segmentation fault** is raised.

---

## Concepts Demonstrated

- **Demand Paging** – pages are loaded only on demand (on first reference).
- **Page Fault** – a referenced page is not in memory; must be loaded.
- **Page Hit** – a referenced page is already in memory; no I/O needed.
- **Hit Ratio** – fraction of references satisfied without a fault.
- **External Fragmentation** – free memory that is scattered and cannot be
  used for large contiguous allocations.
- **Compaction** – shifting allocated segments to merge free holes.
- **Logical → Physical Address Translation** – the MMU maps segment + offset
  to a physical address using the segment table.

---

## Technology Used

| Item | Details |
|---|---|
| Language | C (C99 standard) |
| Compiler | GCC |
| Libraries | `<stdio.h>`, `<string.h>` (standard C only) |
| Version Control | Git / GitHub |

---

## Future Scope

- Implement **Second-Chance (Clock)** page-replacement algorithm
- Add **paged segmentation** (combination of both techniques)
- Export simulation trace to CSV for external analysis
- Port to a GUI using ncurses or a web front-end

---

## References

1. Silberschatz, A., Galvin, P. B., & Gagne, G. (2018). *Operating System Concepts* (10th ed.). Wiley.
2. Tanenbaum, A. S. (2014). *Modern Operating Systems* (4th ed.). Pearson.
3. Belady, L. A. (1966). A study of replacement algorithms for a virtual-storage computer. *IBM Systems Journal*, 5(2), 78–101.
