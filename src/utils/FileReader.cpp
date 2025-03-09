#include "../../include/FileReader.h"
#include <fstream>
#include <iostream>
#include <arpa/inet.h>

// Construtor: abre o arquivo no modo binário
FileReader::FileReader(const std::string& filename) : infile(filename, std::ios::binary) {
    if (!infile.is_open()) {
        throw std::runtime_error("Erro ao abrir o arquivo: " + filename);
    }
}

// Verifica se há mais endereços para ler
bool FileReader::hasNext() {
    return infile.peek() != EOF;
}

// Lê o próximo endereço de 32 bits do arquivo
uint32_t FileReader::nextAddress() {
    uint32_t address;
    infile.read(reinterpret_cast<char*>(&address), sizeof(address)); // Lê 4 bytes (32 bits)
    if (infile.gcount() != sizeof(address)) {
        throw std::runtime_error("Erro ao ler o endereço do arquivo.");
    }
    return convertToLittleEndian(address); // Converte para Little Endian
}

// Converte um endereço de Big Endian para Little Endian
uint32_t FileReader::convertToLittleEndian(uint32_t address) {
    return ntohl(address); // ntohl converte Big Endian para Little Endian
}

// Converte um endereço de Little Endian para Big Endian
uint32_t FileReader::convertToBigEndian(uint32_t address) {
    return htonl(address); // htonl converte Little Endian para Big Endian
}