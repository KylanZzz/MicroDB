//
// Created by Kylan Chen on 2/16/24.
//

#pragma once
#include "string"

namespace Constants{
    constexpr size_t PAGE_SIZE = 4096; // 4 kb page size
    constexpr size_t BUFFER_POOL_SIZE = PAGE_SIZE * 100; // 400kb buffer pool size
    constexpr size_t NUM_CHARS_IN_STRING = 80;
    const std::string DATABASE_FILE = "database.bin";
    enum Types {
        INTEGER = 0,
        CHAR = 1,
        BOOLEAN = 2,
//        STRING = 3
    };
}