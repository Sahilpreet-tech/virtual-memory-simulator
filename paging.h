/**
 * paging.h  –  Page Replacement Algorithm Declarations
 * =====================================================
 * Declares FIFO, LRU, Optimal page-replacement simulations
 * and a comparison utility.
 */

#ifndef PAGING_H
#define PAGING_H

/**
 * fifo – First-In First-Out page replacement.
 * @pages  : reference string
 * @n      : length of reference string
 * @frames : number of physical frames
 */
void fifo(int pages[], int n, int frames);

/**
 * lru – Least Recently Used page replacement.
 * @pages  : reference string
 * @n      : length of reference string
 * @frames : number of physical frames
 */
void lru(int pages[], int n, int frames);

/**
 * optimal – Optimal (Belady's) page replacement.
 * @pages  : reference string
 * @n      : length of reference string
 * @frames : number of physical frames
 */
void optimal(int pages[], int n, int frames);

/**
 * compare_all – Run all three algorithms and display a
 *               side-by-side fault-count summary.
 * @pages  : reference string
 * @n      : length of reference string
 * @frames : number of physical frames
 */
void compare_all(int pages[], int n, int frames);

#endif /* PAGING_H */