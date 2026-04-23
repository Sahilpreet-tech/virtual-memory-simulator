/**
 * Virtual Memory Management Simulator
 * =====================================
 * Simulates paging (FIFO, LRU, Optimal) and segmentation with
 * fragmentation analysis. Includes step-by-step visualization of
 * memory state after each page reference.
 *
 * Build:  gcc main.c paging.c segmentation.c -o vm
 * Run:    ./vm
 */

#include <stdio.h>
#include <stdlib.h>
#include "paging.h"
#include "segmentation.h"

/* ANSI colour helpers */
#define CLR_RESET  "\033[0m"
#define CLR_BOLD   "\033[1m"
#define CLR_CYAN   "\033[1;36m"
#define CLR_GREEN  "\033[1;32m"
#define CLR_YELLOW "\033[1;33m"
#define CLR_RED    "\033[1;31m"
#define CLR_WHITE  "\033[1;37m"

static void print_banner(void) {
    printf(CLR_CYAN);
    printf("\n+----------------------------------------------+\n");
    printf("|    VIRTUAL MEMORY MANAGEMENT SIMULATOR      |\n");
    printf("|  Paging . Segmentation . Page Replacement   |\n");
    printf("+----------------------------------------------+\n");
    printf(CLR_RESET);
}

static void print_menu(void) {
    printf(CLR_WHITE "\n  +-----------------------------------+\n" CLR_RESET);
    printf(CLR_WHITE "  |  " CLR_YELLOW "Page Replacement Algorithms" CLR_WHITE "      |\n" CLR_RESET);
    printf(CLR_WHITE "  |  " CLR_GREEN "1." CLR_RESET " FIFO  (First-In First-Out)  " CLR_WHITE "|\n" CLR_RESET);
    printf(CLR_WHITE "  |  " CLR_GREEN "2." CLR_RESET " LRU   (Least Recently Used) " CLR_WHITE "|\n" CLR_RESET);
    printf(CLR_WHITE "  |  " CLR_GREEN "3." CLR_RESET " OPT   (Optimal / Belady's)  " CLR_WHITE "|\n" CLR_RESET);
    printf(CLR_WHITE "  |  " CLR_GREEN "4." CLR_RESET " Compare All Three           " CLR_WHITE "|\n" CLR_RESET);
    printf(CLR_WHITE "  +-----------------------------------+\n" CLR_RESET);
    printf(CLR_WHITE "  |  " CLR_YELLOW "Segmentation" CLR_WHITE "                     |\n" CLR_RESET);
    printf(CLR_WHITE "  |  " CLR_GREEN "5." CLR_RESET " Segmentation Simulator      " CLR_WHITE "|\n" CLR_RESET);
    printf(CLR_WHITE "  +-----------------------------------+\n" CLR_RESET);
    printf(CLR_WHITE "  |  " CLR_RED "6." CLR_RESET " Exit                        " CLR_WHITE "|\n" CLR_RESET);
    printf(CLR_WHITE "  +-----------------------------------+\n" CLR_RESET);
    printf(CLR_BOLD "  Choice: " CLR_RESET);
}

static int read_page_sequence(int pages[], int *n, int *frames) {
    printf(CLR_CYAN "\n  Enter number of page references (max 100): " CLR_RESET);
    if (scanf("%d", n) != 1 || *n < 1 || *n > 100) {
        printf(CLR_RED "  Invalid input.\n" CLR_RESET);
        return 0;
    }

    printf(CLR_CYAN "  Enter the page reference string (space-separated):\n  > " CLR_RESET);
    for (int i = 0; i < *n; i++) {
        if (scanf("%d", &pages[i]) != 1 || pages[i] < 0) {
            printf(CLR_RED "  Invalid page number.\n" CLR_RESET);
            return 0;
        }
    }

    printf(CLR_CYAN "  Enter number of frames (1-20): " CLR_RESET);
    if (scanf("%d", frames) != 1 || *frames < 1 || *frames > 20) {
        printf(CLR_RED "  Invalid frame count.\n" CLR_RESET);
        return 0;
    }
    return 1;
}

int main(void) {
    int choice;
    int pages[100], n, frames;

    print_banner();

    while (1) {
        print_menu();
        if (scanf("%d", &choice) != 1) { while (getchar() != '\n'); continue; }

        if (choice == 6) {
            printf(CLR_GREEN "\n  Goodbye!\n\n" CLR_RESET);
            break;
        }

        switch (choice) {
            case 1:
                if (read_page_sequence(pages, &n, &frames))
                    fifo(pages, n, frames);
                break;
            case 2:
                if (read_page_sequence(pages, &n, &frames))
                    lru(pages, n, frames);
                break;
            case 3:
                if (read_page_sequence(pages, &n, &frames))
                    optimal(pages, n, frames);
                break;
            case 4:
                if (read_page_sequence(pages, &n, &frames))
                    compare_all(pages, n, frames);
                break;
            case 5:
                segmentation_menu();
                break;
            default:
                printf(CLR_RED "  Invalid choice. Try again.\n" CLR_RESET);
        }
    }
    return 0;
}
