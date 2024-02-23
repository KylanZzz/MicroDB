//
// Created by Kylan Chen on 2/22/24.
//

#pragma once

#include <vector>
#include "Pager.h"

#define sStorageManager StorageManager::getInstance()

using std::vector;
using std::byte;

class StorageManager {

private:
    static StorageManager* INSTANCE;

    struct RowPointer {
        size_t pageNo;
        size_t offset;
        size_t size;
    };

    StorageManager();

public:
    /// this is temporary, i know this is bad practice; will change later
    ~StorageManager();

    /// delete a bunch of copy constructors/move assignments
    StorageManager(const StorageManager& obj) = delete;
    StorageManager& operator=(const StorageManager& obj) = delete;
    StorageManager(StorageManager&&) = delete;
    StorageManager& operator=(StorageManager&&) = delete;

    static StorageManager* getInstance() {
        if (INSTANCE == nullptr) {
            INSTANCE = new StorageManager();
        }
        return INSTANCE;
    }

    RowPointer insertTuple(vector<byte>* data);
    void deleteTuple(RowPointer cursor);

private:
    vector<byte>* pageDirectory;

    /// page number || bytes of free space remaining
    vector<std::pair<size_t , size_t>>* tuplePages;
};
