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
    FileHeap();
    ~FileHeap();

    static FileHeap* INSTANCE;

public:
    /// prevent copy constructors/move assignments
    FileHeap(const FileHeap& f) = delete;
    FileHeap(FileHeap&&) = delete;
    FileHeap& operator=(const FileHeap& f) = delete;
    FileHeap& operator=(FileHeap&&) = delete;

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

    RowPointer insertTuple(std::unique_ptr<vector<byte>> data);
    void deleteTuple(RowPointer row);

private:
    /// page number || bytes of free space remaining
    vector<std::pair<size_t , size_t>> tuplePages;
};
