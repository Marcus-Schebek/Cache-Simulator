#include "../../include/Cache.h"
#include <fstream>
#include <random>
#include <algorithm>

// Construtor
Cache::Cache(int nsets, int bsize, int assoc, char substPolicy, int flag)
    : nsets(nsets), bsize(bsize), assoc(assoc), substPolicy(substPolicy), flag(flag) {
    validateParameters();
    initializeCache();
}

// Validação dos parâmetros da cache
void Cache::validateParameters() {
    if (nsets <= 0 || bsize <= 0 || assoc <= 0) {
        throw std::invalid_argument("Invalid cache parameters");
    }
}

// Inicialização da cache
void Cache::initializeCache() {
    cache.resize(nsets);
    if (substPolicy == 'F') {
        fifoQueue.resize(nsets);
    } else if (substPolicy == 'L') {
        lruList.resize(nsets);
    }
}

// Extrai a tag do endereço
uint32_t Cache::getTag(uint32_t address) const {
    return address >> (static_cast<int>(log2(bsize)) + static_cast<int>(log2(nsets)));
}

// Extrai o índice do endereço
uint32_t Cache::getIndex(uint32_t address) const {
    return (address >> static_cast<int>(log2(bsize))) & (nsets - 1);
}

// Processa um miss na cache
void Cache::handleMiss(int index, uint32_t tag) {
    if (cache[index].size() < assoc) {
        compulsoryMisses++;
    } else {
        if (substPolicy == 'R') {
            replaceRandom(index, tag);
        } else if (substPolicy == 'F') {
            replaceFIFO(index, tag);
        } else if (substPolicy == 'L') {
            replaceLRU(index, tag);
        }
        conflictMisses++;
    }
    cache[index][tag] = true;
}

// Política de substituição Random
void Cache::replaceRandom(int index, uint32_t tag) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, assoc - 1);
    int victim = dis(gen);
    auto it = cache[index].begin();
    std::advance(it, victim);
    cache[index].erase(it);
}

// Política de substituição FIFO
void Cache::replaceFIFO(int index, uint32_t tag) {
    uint32_t victim = fifoQueue[index].front();
    fifoQueue[index].pop();
    cache[index].erase(victim);
    fifoQueue[index].push(tag);
}

// Política de substituição LRU
void Cache::replaceLRU(int index, uint32_t tag) {
    uint32_t victim = lruList[index].front();
    lruList[index].pop_front();
    cache[index].erase(victim);
    lruList[index].push_back(tag);
}

// Método para simular a cache
void Cache::simulate(const std::string& filename) {
    std::vector<uint32_t> addresses = readAddresses(filename);
    for (uint32_t address : addresses) {
        uint32_t tag = getTag(address);
        uint32_t index = getIndex(address);

        if (cache[index].find(tag) != cache[index].end()) {
            hits++;
            if (substPolicy == 'L') {
                lruList[index].remove(tag);
                lruList[index].push_back(tag);
            }
        } else {
            handleMiss(index, tag);
            if (substPolicy == 'F') {
                fifoQueue[index].push(tag);
            } else if (substPolicy == 'L') {
                lruList[index].push_back(tag);
            }
        }
        totalAccesses++;
    }
}

// Método para ler o arquivo de entrada
std::vector<uint32_t> Cache::readAddresses(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        throw std::runtime_error("Failed to open file: " + filename);
    }

    std::vector<uint32_t> addresses;
    uint32_t address;
    while (file.read(reinterpret_cast<char*>(&address), sizeof(address))) {
        addresses.push_back(convertBigToLittleEndian(address));
    }
    return addresses;
}

// Conversão de Big Endian para Little Endian
uint32_t Cache::convertBigToLittleEndian(uint32_t value) {
    return ((value >> 24) & 0xff) | ((value >> 8) & 0xff00) |
           ((value << 8) & 0xff0000) | ((value << 24) & 0xff000000);
}

// Método para imprimir estatísticas
void Cache::printStatistics() const {
    double hitRate = static_cast<double>(hits) / totalAccesses;
    double missRate = 1.0 - hitRate;
    double compulsoryMissRate = static_cast<double>(compulsoryMisses) / totalAccesses;
    double capacityMissRate = static_cast<double>(capacityMisses) / totalAccesses;
    double conflictMissRate = static_cast<double>(conflictMisses) / totalAccesses;

    if (flag == 0) {
        std::cout << "Total de acessos: " << totalAccesses << std::endl;
        std::cout << "Taxa de hits: " << hitRate * 100 << "%" << std::endl;
        std::cout << "Taxa de misses: " << missRate * 100 << "%" << std::endl;
        std::cout << "Taxa de miss compulsório: " << compulsoryMissRate * 100 << "%" << std::endl;
        std::cout << "Taxa de miss de capacidade: " << capacityMissRate * 100 << "%" << std::endl;
        std::cout << "Taxa de miss de conflito: " << conflictMissRate * 100 << "%" << std::endl;
    } else {
        std::cout << totalAccesses << " " << hitRate << " " << missRate << " "
                  << compulsoryMissRate << " " << capacityMissRate << " " << conflictMissRate << std::endl;
    }
}