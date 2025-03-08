#ifndef CACHESIMULATOR_H
#define CACHESIMULATOR_H

#include <string>
#include "CacheConfig.h"

class CacheSimulator {
    public:
        CacheSimulator(int nsets, int bsize, int assoc, const std::string& subst, int flagOut, const std::string& inputFile);
        void run();
    
    private:
        int nsets, bsize, assoc, flagOut;
        std::string subst, inputFile;
        CacheConfig cacheConfig;
    
        void displayConfig() const;
    };

#endif