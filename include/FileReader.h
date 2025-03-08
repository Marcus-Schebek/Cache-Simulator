#ifndef FILEREADER_H
#define FILEREADER_H

#include <string>
#include <fstream>
#include <cstdint>

class FileReader {
public:
    explicit FileReader(const std::string& filename);
    bool hasNext();
    uint32_t nextAddress();

private:
    std::ifstream infile;
    uint32_t convertToLittleEndian(uint32_t address);
    uint32_t convertToBigEndian(uint32_t address);
};

#endif
