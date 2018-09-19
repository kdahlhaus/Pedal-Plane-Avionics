#ifndef _free_mem_h
#define _free_mem_h

// Found on the teensy forums at forum.pjrc.com

uint32_t FreeMem() {
    // return the free ram available.

    uint32_t stackTop;
    uint32_t heapTop;

    // current position of the stack.
    stackTop = (uint32_t) &stackTop;

    // current position of heap.
    void* hTop = malloc(1);
    heapTop = (uint32_t) hTop;
    free(hTop);

    // The difference is (approximately) the free, available ram.
    return stackTop - heapTop;
}

#endif
