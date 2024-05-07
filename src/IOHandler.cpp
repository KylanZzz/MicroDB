//
// Created by Kylan Chen on 2/16/24.
//

#include "IOHandler.h"
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <iostream>

using std::vector;
using std::byte;

IOHandler::IOHandler() {
    fileDescriptor = openFile(Constants::DATABASE_FILE);

    /// get the number of blocks in the file
    struct stat stats;
    fstat(fileDescriptor, &stats);
    numBlocks = stats.st_size / Constants::PAGE_SIZE;
}

/// takes in an existing page and writes it to disk
/// IMPORTANT right now I'm flushing changes after every write (for testing purposes) but may not be the most optimal
void IOHandler::writeBlock(std::unique_ptr<vector<byte>> dataBuffer, size_t blockIndex) {
    pwrite(fileDescriptor, dataBuffer->data(),Constants::PAGE_SIZE, blockIndex * Constants::PAGE_SIZE);
    fsync(fileDescriptor);
}

/// copies disk block into a page contents
std::unique_ptr<vector<byte>> IOHandler::getBlock(size_t blockIndex) {
    auto dataBuffer = std::make_unique<vector<byte>>();
    dataBuffer->reserve(Constants::PAGE_SIZE);
    pread(fileDescriptor, dataBuffer->data(),Constants::PAGE_SIZE, blockIndex * Constants::PAGE_SIZE);

    return dataBuffer;
}

/// opens a file for both reading and writing, returning the file descriptor
int IOHandler::openFile(const std::string& fileName) {
    return open(fileName.c_str(), O_RDWR | O_CREAT,  S_IWUSR | S_IRUSR);
}

/// adds a new block of data to file and returns the block index
std::pair<std::unique_ptr<vector<byte>>, size_t> IOHandler::createBlock() {
    auto dataBuffer = std::make_unique<vector<byte>>();
    dataBuffer->reserve(Constants::PAGE_SIZE);
    pwrite(fileDescriptor, dataBuffer->data(), Constants::PAGE_SIZE, numBlocks * Constants::PAGE_SIZE);
    return std::make_pair(std::move(dataBuffer), numBlocks++);
}

IOHandler::~IOHandler() {
    fsync(fileDescriptor);
    close(fileDescriptor);
}
