#include <stdio.h>
#include "paging.h"

void printMemory(int memory[], int frames) {
    for(int i=0;i<frames;i++) {
        if(memory[i] == -1) printf("[ ] ");
        else printf("[%d] ", memory[i]);
    }
    printf("\n");
}

void fifo(int pages[], int n, int frames) {
    int memory[frames];
    int faults = 0, index = 0;

    for(int i=0;i<frames;i++) memory[i] = -1;

    printf("\n--- FIFO ---\n");

    for(int i=0;i<n;i++) {
        int found = 0;

        for(int j=0;j<frames;j++) {
            if(memory[j] == pages[i]) {
                found = 1;
                break;
            }
        }

        if(!found) {
            memory[index] = pages[i];
            index = (index + 1) % frames;
            faults++;
            printf("Page %d → Fault\n", pages[i]);
        } else {
            printf("Page %d → Hit\n", pages[i]);
        }

        printMemory(memory, frames);
    }

    printf("Total Page Faults: %d\n", faults);
}

void lru(int pages[], int n, int frames) {
    int memory[frames], time[frames];
    int faults = 0;

    for(int i=0;i<frames;i++) {
        memory[i] = -1;
        time[i] = 0;
    }

    printf("\n--- LRU ---\n");

    for(int i=0;i<n;i++) {
        int found = 0;

        for(int j=0;j<frames;j++) {
            if(memory[j] == pages[i]) {
                found = 1;
                time[j] = i;
                break;
            }
        }

        if(!found) {
            int lru = 0;
            for(int j=1;j<frames;j++)
                if(time[j] < time[lru]) lru = j;

            memory[lru] = pages[i];
            time[lru] = i;
            faults++;
            printf("Page %d → Fault\n", pages[i]);
        } else {
            printf("Page %d → Hit\n", pages[i]);
        }

        printMemory(memory, frames);
    }

    printf("Total Page Faults: %d\n", faults);
}

void optimal(int pages[], int n, int frames) {
    int memory[frames];
    int faults = 0;

    for(int i=0;i<frames;i++) memory[i] = -1;

    printf("\n--- OPTIMAL ---\n");

    for(int i=0;i<n;i++) {
        int found = 0;

        for(int j=0;j<frames;j++) {
            if(memory[j] == pages[i]) {
                found = 1;
                break;
            }
        }

        if(!found) {
            int replace = -1, farthest = i;

            for(int j=0;j<frames;j++) {
                int k;
                for(k=i+1;k<n;k++) {
                    if(memory[j] == pages[k]) break;
                }

                if(k > farthest) {
                    farthest = k;
                    replace = j;
                }
            }

            if(replace == -1) replace = 0;

            memory[replace] = pages[i];
            faults++;

            printf("Page %d → Fault\n", pages[i]);
        } else {
            printf("Page %d → Hit\n", pages[i]);
        }

        printMemory(memory, frames);
    }

    printf("Total Page Faults: %d\n", faults);
}