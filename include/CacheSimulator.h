#ifndef CACHESIMULATOR_H
#define CACHESIMULATOR_H

#include <string>
#include "CacheMemory.h"

class CacheSimulator {
public:
    CacheSimulator(int nsets, int bsize, int assoc, const std::string& subst, int flagOut, const std::string& inputFile);
    void run();

private:
    int nsets, bsize, assoc, flagOut;
    std::string subst, inputFile;
    CacheMemory cacheMemory;

    uint32_t convert(char* buffer);
    void displayConfig() const;
};

#endif