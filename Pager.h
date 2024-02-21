//
// Created by Kylan Chen on 2/16/24.
//

#pragma once

#include <cstddef>
#include "vector"
#include "IOHandler.h"


class Pager {

private:
    struct Page {
        std::vector<std::byte>* contents;
        int pageNo;
        Page(std::vector<std::byte>* _contents, int _pageNo):
            contents(_contents), pageNo(_pageNo){};
        ~Page() { delete contents; }
    };

public:
    Pager();
    ~Pager();
    Pager::Page* getPage(int pageNo);
    Pager::Page* makeNewPage();

private:
    std::vector<Page*> buffer;
    IOHandler* ioHandler;
    int numPages;
};