//
// Created by Kylan Chen on 2/16/24.
//

#pragma once

#include <cstddef>
#include "vector"
#include "IOHandler.h"
#include "StorageManager.h"

#define sPager Pager::getInstance()

// Singleton architecture

class Pager {

private:
    static Pager* INSTANCE;

    struct Page {
        std::vector<std::byte>* contents;
        size_t pageNo;

        Page(std::vector<std::byte>* _contents, size_t _pageNo):
            contents(_contents), pageNo(_pageNo){};

        ~Page() { delete contents; }
    };

    Pager();
    ~Pager();

public:
    /// delete a bunch of copy constructors/move assignments
    Pager(const Pager& obj) = delete;
    Pager& operator=(const Pager& obj) = delete;
    Pager(Pager&&) = delete;
    Pager& operator=(Pager&&) = delete;

    static Pager* getInstance() {
        if (INSTANCE == nullptr) {
            INSTANCE = new Pager();
        }
        return INSTANCE;
    }

    Page* getPage(size_t pageNo);
    Page* makeNewPage();

private:
    std::vector<Page*> buffer;
    IOHandler* ioHandler;
    size_t numPages;
};