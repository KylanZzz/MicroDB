//
// Created by Kylan Chen on 2/16/24.
//

#include "Pager.h"
#include "iostream"

// initialize buffer pool manager and underlying IOHandler
Pager::Pager() {
    buffer = std::vector<Page*>();
    ioHandler = new IOHandler();
}

// looks for and returns pointer to page
// if page doesn't exist, then find it in disk and add a copy of it to memory
Pager::Page* Pager::getPage(int pageNo) {
    // if page does exist in buffer pool
    for (Page* page: buffer) {
        if (page->pageNo == pageNo) {
            return page;
        }
    }

    // if the block doesn't exist in disk
    if (ioHandler->getNumBlocks() < pageNo) {
        std::cout << "REQUESTED PAGE DOES NOT EXIST" << std::endl;
        return nullptr;
    }

    // if not then get page from IO
    auto contents = new std::vector<std::byte>();
    ioHandler->getBlock(contents, pageNo);
    Page* newPage = new Page(contents, pageNo);
    buffer.push_back(newPage);
    numPages++;
    return newPage;
}

// creates a new empty page in disk and stores it in buffer pool
Pager::Page* Pager::makeNewPage() {
    auto data = new std::vector<std::byte>(Constants::PAGE_SIZE);
    int pageNo = ioHandler->addBlock(data);
    Page* newPage = new Page(data, pageNo);
    numPages++;
    return newPage;
}

Pager::~Pager() {
    delete ioHandler;
    for (Page* page: buffer) {
        delete page;
    }
}
