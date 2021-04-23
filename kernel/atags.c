#include "../includes/atags.h"

long long int getMemSize(/*atag_t * tag*/) {
//    while (tag->tag != NONE) {
//        if (tag->tag == MEM) {
//            return tag->mem.size;
//        }
//        tag = (atag_t *)(((int *)tag) + tag->tag_size);
//    }
//    return 0;
//     //VM:

    long long result;
    int memSize = 1024;
    result = (1024 * 1024) * memSize;
    return result;
}