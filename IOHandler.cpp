//
// Created by Kylan Chen on 2/16/24.
//

#include "IOHandler.h"
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <iostream>

IOHandler::IOHandler() {
    fileDescriptor = openFile(Constants::DATABASE_FILE);

    // get the number of blocks in the file
    struct stat stats;
    fstat(fileDescriptor, &stats);
    numBlocks = stats.st_size / Constants::PAGE_SIZE;
}

// takes in an existing page and writes it to disk
void IOHandler::writeBlock(std::vector<std::byte>* dataBuffer, size_t blockIndex) {

    std::cout << "bytes wrote to disk: " << pwrite(fileDescriptor, dataBuffer->data(),
           Constants::PAGE_SIZE, blockIndex * Constants::PAGE_SIZE);
}

// copies disk block into a page contents
void IOHandler::getBlock(std::vector<std::byte>* dataBuffer, size_t blockIndex) {

    std::cout << "bytes read from disk: " << pread(fileDescriptor, dataBuffer->data(),
                       Constants::PAGE_SIZE, blockIndex * Constants::PAGE_SIZE);
}

// opens a file for both reading and writing, returning the file descriptor
int IOHandler::openFile(const std::string& fileName) {
    return open(fileName.c_str(), O_RDWR | O_CREAT,  S_IWUSR | S_IRUSR);
}

// adds a new block of data to file and returns the block index
size_t IOHandler::addBlock(std::vector<std::byte> *dataBuffer) {
    pwrite(fileDescriptor, dataBuffer->data(), Constants::PAGE_SIZE, numBlocks * Constants::PAGE_SIZE);
    return numBlocks++;
}

IOHandler::~IOHandler() {
    fsync(fileDescriptor);
    close(fileDescriptor);
}
