#include "./include/Cache.h"
#include <iostream>

int main(int argc, char* argv[]) {
    if (argc != 7) {
        std::cerr << "Usage: " << argv[0] << " <nsets> <bsize> <assoc> <substPolicy> <flag> <filename>" << std::endl;
        return 1;
    }

    int nsets = std::stoi(argv[1]);
    int bsize = std::stoi(argv[2]);
    int assoc = std::stoi(argv[3]);
    char substPolicy = argv[4][0];
    int flag = std::stoi(argv[5]);
    std::string filename = argv[6];

    try {
        Cache cache(nsets, bsize, assoc, substPolicy, flag);
        cache.simulate(filename);
        cache.printStatistics();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}