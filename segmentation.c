#include <stdio.h>
#include "segmentation.h"

void segmentation() {
    int n, memory;
    printf("\n--- SEGMENTATION ---\n");

    printf("Enter number of segments: ");
    scanf("%d", &n);

    int size[n];
    int used = 0;

    for(int i=0;i<n;i++) {
        printf("Enter size of segment %d: ", i+1);
        scanf("%d", &size[i]);
    }

    printf("Enter total memory: ");
    scanf("%d", &memory);

    printf("\nAllocation:\n");

    for(int i=0;i<n;i++) {
        if(used + size[i] <= memory) {
            used += size[i];
            printf("Segment %d → Allocated\n", i+1);
        } else {
            printf("Segment %d → Not Allocated (Fragmentation)\n", i+1);
        }
    }

    printf("Memory Used: %d / %d\n", used, memory);
    printf("Fragmentation: %d\n", memory - used);
}