//
// Created by Kylan Chen on 2/22/24.
//

#pragma once

#include <vector>
#include "Pager.h"

#define sFileHeap FileHeap::getInstance()

using std::vector;
using std::byte;

class FileHeap {

private:
    /// prevent a bunch of copy constructors/move assignments
    FileHeap(const FileHeap& obj) = delete;
    FileHeap& operator=(const FileHeap& obj) = delete;
    FileHeap(FileHeap&&) = delete;
    FileHeap& operator=(FileHeap&&) = delete;

    FileHeap();
    ~FileHeap();

    static FileHeap* INSTANCE;

public:
    struct RowPointer {
        size_t pageNo;
        size_t offset;
        size_t size;
    };

    static FileHeap* getInstance() {
        if (INSTANCE == nullptr) {
            INSTANCE = new FileHeap();
        }
        return INSTANCE;
    }

    static void deleteInstance() {
        delete INSTANCE;
        INSTANCE = nullptr;
    }

    RowPointer insertTuple(vector<byte>* data);
    void deleteTuple(RowPointer row);

private:
    vector<byte>* pageDirectory;

    /// page number || bytes of free space remaining
    vector<std::pair<size_t , size_t>>* tuplePages;
};
