def fifo_trace(pages, frames):
    memory = []
    queue = 0
    trace = []
    faults = 0

    for p in pages:
        fault = False

        if p not in memory:
            fault = True
            faults += 1

            if len(memory) < frames:
                memory.append(p)
            else:
                memory[queue] = p
                queue = (queue + 1) % frames

        trace.append((p, list(memory), fault))

    return trace, faults


def lru_trace(pages, frames):
    memory = []
    last = {}
    trace = []
    faults = 0

    for i, p in enumerate(pages):
        fault = False

        if p not in memory:
            fault = True
            faults += 1

            if len(memory) < frames:
                memory.append(p)
            else:
                lru = min(memory, key=lambda x: last[x])
                memory[memory.index(lru)] = p

        last[p] = i
        trace.append((p, list(memory), fault))

    return trace, faults


def optimal_trace(pages, frames):
    memory = []
    trace = []
    faults = 0

    for i, p in enumerate(pages):
        fault = False

        if p not in memory:
            fault = True
            faults += 1

            if len(memory) < frames:
                memory.append(p)
            else:
                future = pages[i+1:]
                replace = None
                far = -1

                for m in memory:
                    if m not in future:
                        replace = m
                        break
                    else:
                        idx = future.index(m)
                        if idx > far:
                            far = idx
                            replace = m

                memory[memory.index(replace)] = p

        trace.append((p, list(memory), fault))

    return trace, faults
