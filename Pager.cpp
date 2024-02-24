//
// Created by Kylan Chen on 2/16/24.
//

#include "Pager.h"
#include "iostream"


/// initialize buffer pool manager and underlying IOHandler
Pager::Pager() {
    buffer = std::vector<Page*>();
    ioHandler = new IOHandler();
    numPages = 0;
}

/// looks for and returns pointer to page
/// if page doesn't exist, then find it in disk and add a copy of it to memory
Pager::Page* Pager::getPage(size_t pageNo) {
    /// if page does exist in buffer pool
    for (Page* page: buffer) {
        if (page->pageNo == pageNo) {
            return page;
        }
    }

    /// if the block doesn't exist in disk
    if (ioHandler->getNumBlocks() < pageNo) {
        std::cout << "REQUESTED PAGE DOES NOT EXIST" << std::endl;
        return nullptr;
    }

    /// if not then get page from IO
    auto contents = new std::vector<std::byte>(Constants::PAGE_SIZE);
    ioHandler->getBlock(contents, pageNo);
    Page* newPage = new Page(contents, pageNo);
    buffer.push_back(newPage);
    numPages++;
    return newPage;
}

/// creates a new empty page in disk and stores it in buffer pool
Pager::Page* Pager::makeNewPage() {
    auto data = new std::vector<std::byte>(Constants::PAGE_SIZE);
    size_t pageNo = ioHandler->addBlock(data);
    Page* newPage = new Page(data, pageNo);
    numPages++;
    return newPage;
}

/// writes changes page to disk
/// this is done very inefficiently right now, will redo later
void Pager::writePage(size_t pageNo) {
    for (auto page: buffer) {
        if (page->pageNo == pageNo) {
            ioHandler->writeBlock(page->contents, pageNo);
            return;
        }
    }
}

Pager::~Pager(){
    /// write all files at end of program
    for (auto page: buffer) {
        ioHandler->writeBlock(page->contents, page->pageNo);
        delete page;
    }

    delete ioHandler;
}
