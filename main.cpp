#include "./include/CacheSimulator.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <cstdint>


int main(int argc, char* argv[]) {
    if (argc != 7) {
        std::cerr << "Numero de argumentos incorreto. Utilize:\n";
        std::cerr << "./cache_simulator <nsets> <bsize> <assoc> <substituicao> <flag_saida> <arquivo_de_entrada>\n";
        return EXIT_FAILURE;
    }

    CacheSimulator simulator(std::stoi(argv[1]), std::stoi(argv[2]), std::stoi(argv[3]), argv[4], std::stoi(argv[5]), argv[6]);
    simulator.run();
    return EXIT_SUCCESS;
}