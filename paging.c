/**
 * paging.c  -  Page Replacement Algorithm Implementations
 * =========================================================
 * Implements FIFO, LRU, and Optimal (Belady's) page-replacement
 * algorithms with step-by-step memory state visualisation and
 * detailed statistics (faults, hits, hit ratio).
 */

#include <stdio.h>
#include <string.h>
#include "paging.h"

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

/* Print current frame contents as a visual row */
static void print_frames(int memory[], int frames, int highlight) {
    printf("  Frames: ");
    for (int i = 0; i < frames; i++) {
        if (memory[i] == -1) {
            printf(CLR_DIM "[ _ ]" CLR_RESET " ");
        } else if (i == highlight) {
            printf(CLR_YELLOW "[%3d]" CLR_RESET " ", memory[i]);
        } else {
            printf(CLR_CYAN "[%3d]" CLR_RESET " ", memory[i]);
        }
    }
    printf("\n");
}

/* Print a labelled section header */
static void print_header(const char *algo_name) {
    printf(CLR_BOLD CLR_MAGENTA
           "\n+---------------------------------------------------+\n"
           "| %-49s|\n"
           "+---------------------------------------------------+\n"
           CLR_RESET, algo_name);
    printf(CLR_DIM
           "  Step | Page | Status | Frames\n"
           "  -----+------+--------+-------------------------\n"
           CLR_RESET);
}

/* Print final statistics */
static void print_stats(int n, int faults) {
    int hits = n - faults;
    double hit_ratio  = (n > 0) ? (100.0 * hits  / n) : 0.0;
    double miss_ratio = (n > 0) ? (100.0 * faults / n) : 0.0;

    printf(CLR_BOLD "\n  -- Summary ------------------------------------------\n" CLR_RESET);
    printf("  Total References : %d\n", n);
    printf(CLR_RED    "  Page Faults      : %d  (%.1f%%)\n" CLR_RESET, faults, miss_ratio);
    printf(CLR_GREEN  "  Page Hits        : %d  (%.1f%%)\n" CLR_RESET, hits,   hit_ratio);
    printf("  -----------------------------------------------------\n");
}

/* Check if a page is in memory; return index or -1 */
static int find_in_memory(int memory[], int frames, int page) {
    for (int i = 0; i < frames; i++)
        if (memory[i] == page) return i;
    return -1;
}

/* ---- FIFO ---- */

void fifo(int pages[], int n, int frames) {
    int memory[20];
    int faults = 0, head = 0;

    for (int i = 0; i < frames; i++) memory[i] = -1;

    print_header("  FIFO  -  First-In First-Out");

    for (int i = 0; i < n; i++) {
        int idx = find_in_memory(memory, frames, pages[i]);
        int replaced = -1;

        if (idx == -1) {
            replaced = head;
            memory[head] = pages[i];
            head = (head + 1) % frames;
            faults++;
            printf("  %4d | %4d | " CLR_RED "FAULT " CLR_RESET "| ", i + 1, pages[i]);
            print_frames(memory, frames, replaced);
        } else {
            printf("  %4d | %4d | " CLR_GREEN " HIT  " CLR_RESET "| ", i + 1, pages[i]);
            print_frames(memory, frames, idx);
        }
    }

    print_stats(n, faults);
}

/* ---- LRU ---- */

void lru(int pages[], int n, int frames) {
    int memory[20], last_used[20];
    int faults = 0;

    for (int i = 0; i < frames; i++) { memory[i] = -1; last_used[i] = -1; }

    print_header("  LRU  -  Least Recently Used");

    for (int i = 0; i < n; i++) {
        int idx = find_in_memory(memory, frames, pages[i]);
        int replaced = -1;

        if (idx == -1) {
            int victim = 0;
            for (int j = 1; j < frames; j++)
                if (last_used[j] < last_used[victim]) victim = j;

            replaced = victim;
            memory[victim] = pages[i];
            last_used[victim] = i;
            faults++;
            printf("  %4d | %4d | " CLR_RED "FAULT " CLR_RESET "| ", i + 1, pages[i]);
            print_frames(memory, frames, replaced);
        } else {
            last_used[idx] = i;
            printf("  %4d | %4d | " CLR_GREEN " HIT  " CLR_RESET "| ", i + 1, pages[i]);
            print_frames(memory, frames, idx);
        }
    }

    print_stats(n, faults);
}

/* ---- Optimal (Belady's) ---- */

void optimal(int pages[], int n, int frames) {
    int memory[20];
    int faults = 0;

    for (int i = 0; i < frames; i++) memory[i] = -1;

    print_header("  OPT  -  Optimal (Belady's Algorithm)");

    for (int i = 0; i < n; i++) {
        int idx = find_in_memory(memory, frames, pages[i]);
        int replaced = -1;

        if (idx == -1) {
            /* Find a free slot first */
            for (int j = 0; j < frames; j++) {
                if (memory[j] == -1) { replaced = j; break; }
            }

            /* No free slot: evict the page used farthest in the future */
            if (replaced == -1) {
                int farthest = -1;
                for (int j = 0; j < frames; j++) {
                    int k;
                    for (k = i + 1; k < n; k++)
                        if (memory[j] == pages[k]) break;
                    if (k > farthest) { farthest = k; replaced = j; }
                    if (farthest == n) break;
                }
            }

            memory[replaced] = pages[i];
            faults++;
            printf("  %4d | %4d | " CLR_RED "FAULT " CLR_RESET "| ", i + 1, pages[i]);
            print_frames(memory, frames, replaced);
        } else {
            printf("  %4d | %4d | " CLR_GREEN " HIT  " CLR_RESET "| ", i + 1, pages[i]);
            print_frames(memory, frames, idx);
        }
    }

    print_stats(n, faults);
}

/* ---- Comparison Table ---- */

void compare_all(int pages[], int n, int frames) {
    int mem[20], faults[3] = {0, 0, 0};

    /* FIFO count */
    for (int i = 0; i < frames; i++) mem[i] = -1;
    int head = 0;
    for (int i = 0; i < n; i++) {
        if (find_in_memory(mem, frames, pages[i]) == -1) {
            mem[head] = pages[i];
            head = (head + 1) % frames;
            faults[0]++;
        }
    }

    /* LRU count */
    int lu[20];
    for (int i = 0; i < frames; i++) { mem[i] = -1; lu[i] = -1; }
    for (int i = 0; i < n; i++) {
        int idx = find_in_memory(mem, frames, pages[i]);
        if (idx == -1) {
            int v = 0;
            for (int j = 1; j < frames; j++) if (lu[j] < lu[v]) v = j;
            mem[v] = pages[i]; lu[v] = i; faults[1]++;
        } else { lu[idx] = i; }
    }

    /* OPT count */
    for (int i = 0; i < frames; i++) mem[i] = -1;
    for (int i = 0; i < n; i++) {
        if (find_in_memory(mem, frames, pages[i]) == -1) {
            int rep = -1;
            for (int j = 0; j < frames; j++)
                if (mem[j] == -1) { rep = j; break; }
            if (rep == -1) {
                int far = -1;
                for (int j = 0; j < frames; j++) {
                    int k;
                    for (k = i + 1; k < n; k++) if (mem[j] == pages[k]) break;
                    if (k > far) { far = k; rep = j; }
                    if (far == n) break;
                }
            }
            mem[rep] = pages[i]; faults[2]++;
        }
    }

    /* Print detailed runs */
    fifo(pages, n, frames);
    lru(pages, n, frames);
    optimal(pages, n, frames);

    /* Side-by-side summary */
    int best_idx = 0;
    for (int i = 1; i < 3; i++)
        if (faults[i] < faults[best_idx]) best_idx = i;

    printf(CLR_BOLD CLR_WHITE
           "\n+===============================================+\n"
           "|        ALGORITHM COMPARISON SUMMARY          |\n"
           "+================+===========+==================+\n"
           "|  Algorithm     |  Faults   |  Hits            |\n"
           "+================+===========+==================+\n"
           CLR_RESET);

    const char *names[] = { "FIFO", "LRU", "Optimal" };
    for (int i = 0; i < 3; i++) {
        int hits = n - faults[i];
        if (i == best_idx)
            printf("|  %-14s|  %6d   |  %5d  " CLR_GREEN "<<BEST" CLR_RESET "        |\n",
                   names[i], faults[i], hits);
        else
            printf("|  %-14s|  %6d   |  %5d            |\n",
                   names[i], faults[i], hits);
    }

    printf(CLR_BOLD CLR_WHITE
           "+================+===========+==================+\n\n" CLR_RESET);

    printf(CLR_YELLOW
           "  Note: Optimal is the theoretical lower bound on page faults.\n"
           "        Real systems cannot implement it (requires future knowledge).\n\n"
           CLR_RESET);
}
