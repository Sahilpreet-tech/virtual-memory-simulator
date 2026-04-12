from algorithms import fifo_trace, lru_trace, optimal_trace, segmentation_simulation

pages = [7, 0, 1, 2, 0, 3, 0, 4]
frames = 3

print("\n===== FIFO =====")
trace, faults = fifo_trace(pages, frames)
print("Page Faults:", faults)

print("\n===== LRU =====")
trace, faults = lru_trace(pages, frames)
print("Page Faults:", faults)

print("\n===== OPTIMAL =====")
trace, faults = optimal_trace(pages, frames)
print("Page Faults:", faults)

print("\n===== SEGMENTATION =====")
segments = [("code", 100), ("data", 200), ("stack", 150)]
alloc, used, frag = segmentation_simulation(segments, 400)

print("Allocated:", alloc)
print("Used:", used)
print("Fragmentation:", frag)