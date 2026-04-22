#ifndef PAGING_H
#define PAGING_H

void fifo(int pages[], int n, int frames);
void lru(int pages[], int n, int frames);
void optimal(int pages[], int n, int frames);

#endif