/**
 * segmentation.h  –  Memory Segmentation Declarations
 * =====================================================
 * Declares the segmentation simulation which models a process's
 * logical address space as named segments mapped into physical
 * memory, with internal and external fragmentation analysis.
 */

#ifndef SEGMENTATION_H
#define SEGMENTATION_H

/**
 * segmentation_menu – Interactive segmentation simulator.
 * Accepts segment count, individual segment sizes, and total
 * physical memory; then attempts first-fit allocation and
 * reports fragmentation details.
 */
void segmentation_menu(void);

#endif /* SEGMENTATION_H */
