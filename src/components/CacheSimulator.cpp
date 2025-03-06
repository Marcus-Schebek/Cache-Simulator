#include "../../include/CacheSimulator.h"
#include <iostream>
#include <fstream>
#include <cmath>

CacheSimulator::CacheSimulator(int nsets, int bsize, int assoc, const std::string& subst, int flagOut, const std::string& inputFile)
    : nsets(nsets), bsize(bsize), assoc(assoc), subst(subst), flagOut(flagOut), inputFile(inputFile),
      cacheMemory(nsets, assoc) {}

uint32_t CacheSimulator::convert(char* buffer) {
    return (static_cast<uint32_t>(buffer[0]) << 24) |
           (static_cast<uint32_t>(buffer[1]) << 16) |
           (static_cast<uint32_t>(buffer[2]) << 8)  |
           (static_cast<uint32_t>(buffer[3]));
}

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

    std::ifstream infile(inputFile, std::ios::binary);
    char buffer[4];
    int hit = 0, compulsorio = 0, capacidade = 0, conflito = 0, totalAccesses = 0;

    while (infile.read(buffer, sizeof(buffer))) {
        totalAccesses++;
        uint32_t address = convert(buffer);
        uint32_t tag = address >> (offsetBits + indexBits);
        uint32_t index = address % nsets;

        cacheMemory.access(tag, index, hit, compulsorio, capacidade, conflito, totalAccesses);
    }

    int totalMiss = compulsorio + capacidade + conflito;
    std::cout << hit << " " << totalMiss << " " << compulsorio << " " << capacidade << " " << conflito << "\n";
    std::cout << totalAccesses << " " << (float)hit / totalAccesses << " " << (float)totalMiss / totalAccesses << "\n";
}