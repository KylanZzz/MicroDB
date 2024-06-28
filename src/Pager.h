//
// Created by Kylan Chen on 2/16/24.
//

#pragma once

#include <cstddef>
#include <vector>
#include "IOHandler.h"
#include "FileHeap.h"

#define sPager Pager::getInstance()

// Singleton architecture

class Pager {

private:

    Pager();
    ~Pager();

    static Pager* INSTANCE;

public:
    Pager(Pager&& p) = delete;
    Pager(const Pager& p) = delete;
    Pager& operator=(const Pager& p) = delete;
    Pager& operator=(Pager&& p) = delete;

    struct Page {
        std::unique_ptr<vector<byte>> contents;
        size_t pageNo;

        Page(std::unique_ptr<vector<byte>> _contents, size_t _pageNo):
                contents(std::move(_contents)), pageNo(_pageNo){};
    };

    static Pager* getInstance() {
        if (INSTANCE == nullptr) {
            INSTANCE = new Pager();
        }
        return INSTANCE;
    }

    static void deleteInstance() {
        delete INSTANCE;
        INSTANCE = nullptr;
    }

    std::weak_ptr<Page> getPage(size_t pageNo);
    std::weak_ptr<Page> makeNewPage();
    void writePage(size_t pageNo);

private:
    std::vector<std::shared_ptr<Page>> buffer;
    IOHandler ioHandler;
    size_t numPages;
};