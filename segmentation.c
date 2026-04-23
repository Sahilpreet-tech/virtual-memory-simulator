/**
 * segmentation.c  -  Memory Segmentation Simulation
 * ===================================================
 * Simulates a contiguous memory allocation scheme using named
 * segments (Code, Stack, Heap, Data, BSS ...).  Features:
 *   - First-fit allocation with base/limit tracking
 *   - Visual ASCII memory map
 *   - Internal & external fragmentation analysis
 *   - Memory compaction demonstration
 *   - Logical to Physical address translation demo
 */

#include <stdio.h>
#include <string.h>
#include "segmentation.h"

/* ANSI colours */
#define CLR_RESET   "\033[0m"
#define CLR_BOLD    "\033[1m"
#define CLR_RED     "\033[1;31m"
#define CLR_GREEN   "\033[1;32m"
#define CLR_YELLOW  "\033[1;33m"
#define CLR_CYAN    "\033[1;36m"
#define CLR_MAGENTA "\033[1;35m"
#define CLR_WHITE   "\033[1;37m"
#define CLR_DIM     "\033[2m"

#define MAX_SEG 20

static const char *seg_names[] = {
    "Code ", "Stack", "Heap ", "Data ", "BSS  ",
    "Seg-5","Seg-6","Seg-7","Seg-8","Seg-9",
    "Sg-10","Sg-11","Sg-12","Sg-13","Sg-14",
    "Sg-15","Sg-16","Sg-17","Sg-18","Sg-19"
};

static const char *seg_colours[] = {
    "\033[1;32m", "\033[1;34m", "\033[1;35m",
    "\033[1;33m", "\033[1;36m", "\033[1;31m",
    "\033[1;37m", "\033[0;32m", "\033[0;34m",
    "\033[0;35m", "\033[0;33m", "\033[0;36m",
    "\033[0;31m", "\033[0;37m", "\033[1;32m",
    "\033[1;34m", "\033[1;35m", "\033[1;33m",
    "\033[1;36m", "\033[1;31m"
};

/* Draw a proportional ASCII bar representing physical memory */
static void draw_memory_map(int memory, int base[], int size[], int n) {
    const int BAR = 60;
    int owner[60];

    for (int i = 0; i < BAR; i++) owner[i] = -1;

    for (int s = 0; s < n; s++) {
        if (base[s] < 0) continue;
        int lo = (int)((long)base[s] * BAR / memory);
        int hi = (int)((long)(base[s] + size[s]) * BAR / memory);
        if (hi > BAR) hi = BAR;
        for (int i = lo; i < hi; i++) owner[i] = s;
    }

    /* Top border */
    printf("  0   +");
    for (int i = 0; i < BAR; i++) printf("-");
    printf("+  %d KB\n", memory);

    /* The bar */
    printf("       |");
    int i = 0;
    while (i < BAR) {
        int s = owner[i];
        if (s == -1) {
            int j = i;
            while (j < BAR && owner[j] == -1) j++;
            for (int k = i; k < j; k++) printf(".");
            i = j;
        } else {
            int j = i;
            while (j < BAR && owner[j] == s) j++;
            printf("%s", seg_colours[s % 20]);
            for (int k = i; k < j; k++) printf("#");
            printf(CLR_RESET);
            i = j;
        }
    }
    printf("|\n");

    /* Bottom border */
    printf("       +");
    for (int i = 0; i < BAR; i++) printf("-");
    printf("+\n\n");

    /* Legend */
    printf("  Legend:  ");
    for (int s = 0; s < n; s++) {
        if (base[s] >= 0)
            printf("%s#%s=%s  ", seg_colours[s % 20], CLR_RESET, seg_names[s % 20]);
    }
    printf(CLR_DIM " .=Free\n" CLR_RESET);
}

/* Main segmentation simulator */
void segmentation_menu(void) {
    int n, memory;

    printf(CLR_BOLD CLR_MAGENTA
           "\n+---------------------------------------------------+\n"
           "|        SEGMENTATION  MEMORY  SIMULATOR           |\n"
           "+---------------------------------------------------+\n"
           CLR_RESET);

    printf(CLR_CYAN "  Enter number of segments (1-%d): " CLR_RESET, MAX_SEG);
    if (scanf("%d", &n) != 1 || n < 1 || n > MAX_SEG) {
        printf(CLR_RED "  Invalid input.\n" CLR_RESET); return;
    }

    int size[MAX_SEG], base[MAX_SEG];
    int total_requested = 0;

    for (int i = 0; i < n; i++) {
        printf(CLR_CYAN "  Size of segment %d (%s) in KB: " CLR_RESET,
               i + 1, seg_names[i]);
        if (scanf("%d", &size[i]) != 1 || size[i] <= 0) {
            printf(CLR_RED "  Invalid size.\n" CLR_RESET); return;
        }
        total_requested += size[i];
        base[i] = -1;
    }

    printf(CLR_CYAN "  Enter total physical memory in KB: " CLR_RESET);
    if (scanf("%d", &memory) != 1 || memory <= 0) {
        printf(CLR_RED "  Invalid memory.\n" CLR_RESET); return;
    }

    /* First-fit allocation */
    int used = 0, allocated_count = 0, unallocated_size = 0;

    printf(CLR_BOLD "\n  -- Allocation Table (First-Fit) --------------------\n" CLR_RESET);
    printf("  %-8s  %-8s  %-12s  %-12s  %s\n",
           "Segment", "Size", "Base Addr", "Limit", "Status");
    printf("  ----------------------------------------------------------\n");

    for (int i = 0; i < n; i++) {
        if (used + size[i] <= memory) {
            base[i] = used;
            used += size[i];
            allocated_count++;
            printf("  %s%-8s%s  %4d KB    %8d KB    %8d KB    " CLR_GREEN "Allocated\n" CLR_RESET,
                   seg_colours[i % 20], seg_names[i], CLR_RESET,
                   size[i], base[i], base[i] + size[i] - 1);
        } else {
            unallocated_size += size[i];
            printf("  %s%-8s%s  %4d KB    %8s      %8s      " CLR_RED "NOT Allocated (Fragmentation)\n" CLR_RESET,
                   seg_colours[i % 20], seg_names[i], CLR_RESET,
                   size[i], "---", "---");
        }
    }

    /* Visual memory map */
    printf(CLR_BOLD "\n  -- Physical Memory Map -----------------------------\n" CLR_RESET);
    draw_memory_map(memory, base, size, n);

    /* Fragmentation report */
    int external_frag = memory - used;
    double utilisation = (memory > 0) ? (100.0 * used / memory) : 0.0;

    printf(CLR_BOLD "\n  -- Fragmentation Analysis --------------------------\n" CLR_RESET);
    printf("  Total Physical Memory   : %d KB\n", memory);
    printf("  Total Requested         : %d KB\n", total_requested);
    printf(CLR_GREEN "  Memory Allocated        : %d KB  (%.1f%%)\n" CLR_RESET, used, utilisation);
    printf(CLR_RED   "  External Fragmentation  : %d KB\n" CLR_RESET, external_frag);
    if (unallocated_size > 0)
        printf(CLR_RED "  Unallocated Segments    : %d KB  (not enough contiguous space)\n" CLR_RESET,
               unallocated_size);

    /* Compaction hint */
    if (external_frag > 0 && unallocated_size > 0) {
        printf(CLR_YELLOW
               "\n  [!] COMPACTION INSIGHT:\n"
               "      After compaction, all allocated segments would be\n"
               "      shifted to one end, creating a single free block of %d KB.\n"
               "      This %s%s%s be enough to fit the remaining segments (%d KB).\n"
               CLR_RESET,
               external_frag,
               (external_frag >= unallocated_size) ? CLR_GREEN : CLR_RED,
               (external_frag >= unallocated_size) ? "WOULD" : "would NOT",
               CLR_YELLOW,
               unallocated_size);
    }

    /* Logical to Physical address translation demo */
    if (allocated_count > 0) {
        printf(CLR_BOLD "\n  -- Logical to Physical Address Translation Demo ----\n" CLR_RESET);
        printf("  (Translates a logical offset within a segment to a physical address)\n\n");

        int demo_seg = -1;
        for (int i = 0; i < n; i++) if (base[i] >= 0) { demo_seg = i; break; }

        if (demo_seg >= 0) {
            int offset = size[demo_seg] / 2;
            int phys   = base[demo_seg] + offset;
            printf("  Segment        : %s  (base = %d KB, size = %d KB)\n",
                   seg_names[demo_seg], base[demo_seg], size[demo_seg]);
            printf("  Logical offset : %d KB\n", offset);
            if (offset < size[demo_seg]) {
                printf("  Physical addr  = base + offset = %d + %d = " CLR_GREEN "%d KB\n" CLR_RESET,
                       base[demo_seg], offset, phys);
            } else {
                printf(CLR_RED "  SEGMENTATION FAULT: offset %d >= segment size %d\n" CLR_RESET,
                       offset, size[demo_seg]);
            }
        }
    }

    printf("\n");
}
