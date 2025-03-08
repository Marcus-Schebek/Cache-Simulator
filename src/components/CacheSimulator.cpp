#include "../../include/CacheSimulator.h"
#include "../../include/FileReader.h"
#include <iostream>
#include <cmath>

CacheSimulator::CacheSimulator(int nsets, int bsize, int assoc, const std::string& subst, int flagOut, const std::string& inputFile)
    : nsets(nsets), bsize(bsize), assoc(assoc), subst(subst), flagOut(flagOut), inputFile(inputFile),
      cacheConfig(nsets, assoc) {}

void CacheSimulator::displayConfig() const {
    std::cout << "nsets = " << nsets << "\n";
    std::cout << "bsize = " << bsize << "\n";
    std::cout << "assoc = " << assoc << "\n";
    std::cout << "subst = " << subst << "\n";
    std::cout << "flagOut = " << flagOut << "\n";
    std::cout << "arquivo = " << inputFile << "\n";
}

void CacheSimulator::run() {
    if (flagOut == 0) displayConfig();

    uint8_t offsetBits = static_cast<uint8_t>(log2(bsize));
    uint8_t indexBits = static_cast<uint8_t>(log2(nsets));

    FileReader fileReader(inputFile);
    int hit = 0, compulsorio = 0, capacidade = 0, conflito = 0, totalAccesses = 0;

    while (fileReader.hasNext()) {
        totalAccesses++;
        uint32_t address = fileReader.nextAddress();
        uint32_t tag = address >> (offsetBits + indexBits);
        uint32_t index = address % nsets;

        cacheConfig.access(tag, index, hit, compulsorio, capacidade, conflito, totalAccesses);
    }

    int totalMiss = compulsorio + capacidade + conflito;
    std::cout << hit << " " << totalMiss << " " << compulsorio << " " << capacidade << " " << conflito << "\n";
    std::cout << totalAccesses << " " << (float)hit / totalAccesses << " " << (float)totalMiss / totalAccesses << "\n";
}