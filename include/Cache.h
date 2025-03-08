#ifndef CACHE_H
#define CACHE_H

#include <vector>
#include <unordered_map>
#include <string>
#include <stdexcept>
#include <cmath>
#include <iostream>
#include <fstream>
#include <queue>
#include <list>
#include <random>

class Cache {
public:
    // Construtor
    Cache(int nsets, int bsize, int assoc, char substPolicy, int flag);

    // Método principal para simular a cache
    void simulate(const std::string& filename);

    // Método para imprimir estatísticas
    void printStatistics() const;

private:
    // Parâmetros da cache
    int nsets, bsize, assoc;
    char substPolicy;
    int flag;

    // Estrutura da cache
    std::vector<std::unordered_map<uint32_t, bool>> cache;
    std::vector<std::queue<uint32_t>> fifoQueue; // Para política FIFO
    std::vector<std::list<uint32_t>> lruList;    // Para política LRU

    // Estatísticas
    int totalAccesses = 0, hits = 0, compulsoryMisses = 0, capacityMisses = 0, conflictMisses = 0;

    // Métodos auxiliares
    void validateParameters();
    void initializeCache();
    uint32_t getTag(uint32_t address) const;
    uint32_t getIndex(uint32_t address) const;
    void handleMiss(int index, uint32_t tag);

    // Métodos para políticas de substituição
    void replaceRandom(int index, uint32_t tag);
    void replaceFIFO(int index, uint32_t tag);
    void replaceLRU(int index, uint32_t tag);

    // Método para ler o arquivo de entrada
    std::vector<uint32_t> readAddresses(const std::string& filename);
    uint32_t convertBigToLittleEndian(uint32_t value);
};

#endif // CACHE_H