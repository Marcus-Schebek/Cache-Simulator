#include "../../include/CacheConfig.h"
#include <climits>
#include <cstdlib>

CacheConfig::CacheConfig(int nsets, int assoc)
    : assoc(assoc), nsets(nsets), tags(nsets * assoc, 0), timestamps(nsets * assoc, 0), valid(nsets * assoc, 0) {}

bool CacheConfig::access(uint32_t tag, uint32_t index, int& hit, int& compulsorio, int& capacidade, int& conflito, int currentTime) {
    int start = index * assoc;
    int end = start + assoc;
    int emptySlot = -1;
    int oldestSlot = start;
    int oldestTime = INT_MAX;

    for (int i = start; i < end; ++i) {
        if (valid[i] && tags[i] == tag) {
            hit++;
            timestamps[i] = currentTime;
            return true;
        }
        if (!valid[i]) emptySlot = i;
        if (timestamps[i] < oldestTime) {
            oldestTime = timestamps[i];
            oldestSlot = i;
        }
    }

    if (emptySlot != -1) {
        compulsorio++;
        valid[emptySlot] = 1;
        tags[emptySlot] = tag;
        timestamps[emptySlot] = currentTime;
    } else {
        conflito++;
        valid[oldestSlot] = 1;
        tags[oldestSlot] = tag;
        timestamps[oldestSlot] = currentTime;
    }
    return false;
}