#include "../../include/Cache.h"
#include <fstream>
#include <random>
#include <algorithm>
#include <iomanip> // Para std::fixed e std::setprecision

// Construtor
Cache::Cache(int nsets, int bsize, int assoc, char substPolicy, int flag)
    : nsets(nsets), bsize(bsize), assoc(assoc), substPolicy(substPolicy), flag(flag),
      hits(0), totalAccesses(0), compulsoryMisses(0), capacityMisses(0), conflictMisses(0) {
    validateParameters();
    initializeCache();
}

// Validação dos parâmetros da cache
void Cache::validateParameters() {
    if (nsets <= 0 || bsize <= 0 || assoc <= 0) {
        throw std::invalid_argument("Invalid cache parameters");
    }
}

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

void Cache::handleMiss(int index, uint32_t tag) {
    if (cache[index].size() < assoc) {
        // Se o conjunto não está cheio, é um miss compulsório
        compulsoryMisses++;
        cache[index][tag] = true; // Adiciona o novo bloco à cache
        if (substPolicy == 'F') {
            fifoQueue[index].push(tag); // Adiciona à fila FIFO
        } else if (substPolicy == 'L') {
            lruList[index].push_back(tag); // Adiciona à lista LRU
        }
    } else {
        // Se o conjunto está cheio, aplica a política de substituição
        if (substPolicy == 'R') {
            replaceRandom(index, tag);
        } else if (substPolicy == 'F') {
            replaceFIFO(index, tag);
        } else if (substPolicy == 'L') {
            replaceLRU(index, tag);
        }

        // Classifica o miss como de capacidade ou conflito
        bool isCapacityMiss = true;
        for (const auto& set : cache) {
            if (set.size() < assoc) {
                isCapacityMiss = false;
                break;
            }
        }
        if (isCapacityMiss) {
            capacityMisses++;
        } else {
            conflictMisses++;
        }
    }
}


void Cache::replaceRandom(int index, uint32_t tag) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, assoc - 1);
    auto it = cache[index].begin();
    std::advance(it, dis(gen)); // Seleciona um bloco aleatório
    cache[index].erase(it); // Remove o bloco substituído
    cache[index][tag] = true; // Adiciona o novo bloco
}

void Cache::replaceFIFO(int index, uint32_t tag) {
    uint32_t victim = fifoQueue[index].front(); // Seleciona o bloco mais antigo
    fifoQueue[index].pop(); // Remove o bloco da fila
    cache[index].erase(victim); // Remove o bloco da cache
    fifoQueue[index].push(tag); // Adiciona o novo bloco à fila
    cache[index][tag] = true; // Adiciona o novo bloco à cache
}

void Cache::replaceLRU(int index, uint32_t tag) {
    // Seleciona o bloco menos recentemente usado (primeiro da lista)
    uint32_t victim = lruList[index].front();
    lruList[index].pop_front(); // Remove o bloco da lista
    cache[index].erase(victim); // Remove o bloco da cache

    // Adiciona o novo bloco à cache e à lista LRU
    cache[index][tag] = true;
    lruList[index].push_back(tag);
}

void Cache::simulate(const std::string& filename) {
    FileReader reader(filename);
    while (reader.hasNext()) {
        uint32_t address = reader.nextAddress(); 
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
        }
        totalAccesses++;
    }
}
void Cache::printStatistics() const {
    double hitRate = static_cast<double>(hits) / totalAccesses;
    double missRate = 1.0 - hitRate;
    int totalMisses = compulsoryMisses + capacityMisses + conflictMisses;
    double compulsoryMissRate = (totalMisses > 0) ? static_cast<double>(compulsoryMisses) / totalMisses : 0.0;
    double capacityMissRate = (totalMisses > 0) ? static_cast<double>(capacityMisses) / totalMisses : 0.0;
    double conflictMissRate = (totalMisses > 0) ? static_cast<double>(conflictMisses) / totalMisses : 0.0;

    if (flag == 0) {
        std::cout << "Total de acessos: " << totalAccesses << std::endl;
        std::cout << "Taxa de hits: " << std::fixed << std::setprecision(4) << hitRate * 100 << "%" << std::endl;
        std::cout << "Taxa de misses: " << std::fixed << std::setprecision(4) << missRate * 100 << "%" << std::endl;
        std::cout << "Taxa de miss compulsório: " << std::fixed << std::setprecision(2) << compulsoryMissRate * 100 << "%" << std::endl;
        std::cout << "Taxa de miss de capacidade: " << std::fixed << std::setprecision(2) << capacityMissRate * 100 << "%" << std::endl;
        std::cout << "Taxa de miss de conflito: " << std::fixed << std::setprecision(2) << conflictMissRate * 100 << "%" << std::endl;
    } else {
        std::cout << totalAccesses << " "
                  << std::fixed << std::setprecision(4) << hitRate << " "
                  << std::fixed << std::setprecision(4) << missRate << " "
                  << std::fixed << std::setprecision(2) << compulsoryMissRate << " "
                  << std::fixed << std::setprecision(2) << capacityMissRate << " "
                  << std::fixed << std::setprecision(2) << conflictMissRate << std::endl;
    }
}