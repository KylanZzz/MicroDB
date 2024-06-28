//
// Created by Kylan Chen on 2/16/24.
//

#pragma once

#include "string"
#include "vector"
#include "Constants.h"

using std::vector;
using std::byte;

class IOHandler {
public:
    IOHandler();
    ~IOHandler();

    IOHandler(IOHandler&& i) = delete;
    IOHandler(const IOHandler& i) = delete;
    IOHandler& operator=(IOHandler&& i) = delete;
    IOHandler& operator=(const IOHandler& i) = delete;

    void writeBlock(std::unique_ptr<vector<byte>> dataBuffer, size_t blockIndex);
    std::pair<std::unique_ptr<vector<byte>>, size_t> createBlock();
    std::unique_ptr<vector<byte>> getBlock(size_t blockIndex);
    size_t getNumBlocks() const { return numBlocks; }


private:
    int fileDescriptor;
    size_t numBlocks;
};

