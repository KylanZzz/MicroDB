//
// Created by Kylan Chen on 2/16/24.
//

#pragma once
#include "string"

namespace Constants{
    constexpr size_t PAGE_SIZE = 4096; // 4 kb page size
    constexpr size_t BUFFER_POOL_SIZE = 100; // 100page / 400kb buffer pool size
    constexpr size_t MAX_CHARS_IN_STRING = 80;
    const std::string DATABASE_FILE = "database.bin";

    enum Types {
        INTEGER = 0,
        CHAR = 1,
        BOOLEAN = 2,
        SIZE_T = 3
//        STRING = 3
    };

    enum PageNo {
        PAGE_DIRECTORY = 0,
        BTREE_ROOT = 1,
    };
}