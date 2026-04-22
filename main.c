#include <stdio.h>
#include "paging.h"
#include "segmentation.h"

int main() {
    int choice;
    int pages[50], n, frames;

    while(1) {
        printf("\n===== Virtual Memory Simulator =====\n");
        printf("1. FIFO\n2. LRU\n3. Optimal\n4. Segmentation\n5. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        if(choice == 5) break;

        if(choice >=1 && choice <=3) {
            printf("Enter number of pages: ");
            scanf("%d", &n);

            printf("Enter pages:\n");
            for(int i=0;i<n;i++) scanf("%d", &pages[i]);

            printf("Enter frames: ");
            scanf("%d", &frames);
        }

        switch(choice) {
            case 1: fifo(pages, n, frames); break;
            case 2: lru(pages, n, frames); break;
            case 3: optimal(pages, n, frames); break;
            case 4: segmentation(); break;
        }
    }

    return 0;
}