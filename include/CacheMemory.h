#ifndef CACHEMEMORY_H
#define CACHEMEMORY_H

#include <vector>
#include <cstdint>

class CacheMemory {
public:
    CacheMemory(int nsets, int assoc);

    bool access(uint32_t tag, uint32_t index, int& hit, int& compulsorio, int& capacidade, int& conflito, int currentTime);

private:
    int assoc, nsets;
    std::vector<uint32_t> tags, timestamps;
    std::vector<uint8_t> valid;
};

#endif