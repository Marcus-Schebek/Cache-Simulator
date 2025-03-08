#include "../../include/FileReader.h"
#include <fstream>
#include <iostream>
#include <arpa/inet.h>

FileReader::FileReader(const std::string& filename) : infile(filename, std::ios::binary) {}

bool FileReader::hasNext() {
    return infile.peek() != EOF;
}

uint32_t FileReader::nextAddress() {
    uint32_t address;
    infile.read(reinterpret_cast<char*>(&address), sizeof(address));
    return convertToLittleEndian(address);
}

uint32_t FileReader::convertToLittleEndian(uint32_t address) {
    return ntohl(address); //Convert Big Endian para Little Endian
}

uint32_t FileReader::convertToBigEndian(uint32_t address) {
    return htonl(address); //Convert Little Endian para Big Endian
}