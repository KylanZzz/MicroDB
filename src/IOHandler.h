//
// Created by Kylan Chen on 2/16/24.
//

#pragma once

#include "string"
#include "vector"
#include "Constants.h"

class IOHandler {
public:
    IOHandler();
    ~IOHandler();

    void writeBlock(std::vector<std::byte>* dataBuffer, size_t blockIndex);
    size_t addBlock(std::vector<std::byte>* dataBuffer);
    void getBlock(std::vector<std::byte>* dataBuffer, size_t blockIndex);
    int openFile(const std::string& fileName);
    size_t getNumBlocks() { return numBlocks; }


private:
    int fileDescriptor;
    size_t numBlocks;
};

