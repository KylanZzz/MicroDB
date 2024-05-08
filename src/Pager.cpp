//
// Created by Kylan Chen on 2/16/24.
//

#include "Pager.h"
#include "iostream"
#include <stdexcept>

Pager* Pager::INSTANCE = nullptr;

/// initialize buffer pool manager and underlying IOHandler
Pager::Pager() {
    std::cout << "CONSTRUCTING PAGER" << std::endl;

    ioHandler = std::make_unique<IOHandler>();
    numPages = 0;
}

/// looks for and returns page
/// if page doesn't exist, then find it in disk and add a copy of it to memory
/// THROWS: invalid_argument
std::weak_ptr<Pager::Page> Pager::getPage(size_t pageNo) {
    /// check if pageNo is out of bounds
    if (ioHandler->getNumBlocks() <= pageNo) {
        throw std::invalid_argument("PageNo exceeds the total number of blocks");
    }

    /// check if page already exists in buffer pool
    /// TODO: implement LRU instead of keeping all pages
    for (const auto& shared : buffer) {
        if (shared->pageNo == pageNo) {
            return std::weak_ptr<Page>{shared};
        }
    }

    /// if not then get page from IO
    auto contents = ioHandler->getBlock(pageNo);
    buffer.emplace_back(std::make_shared<Page>(std::move(contents), pageNo));
    numPages++;
    return std::weak_ptr<Page>{buffer.back()};
}

/// creates a new empty page in disk and stores it in buffer pool
std::weak_ptr<Pager::Page> Pager::makeNewPage() {
    auto data = ioHandler->createBlock();
    buffer.emplace_back(std::make_shared<Page>(Page(std::move(data.first), data.second)));
    numPages++;
    return std::weak_ptr<Page>{buffer.back()};
}

/// this is done very inefficiently right now, will redo later
void Pager::writePage(size_t pageNo) {
    for (auto pageIt = buffer.begin(); pageIt != buffer.end(); ++pageIt) {
        if ((*pageIt)->pageNo == pageNo) {
            ioHandler->writeBlock(std::move((*pageIt)->contents), pageNo);
            buffer.erase(pageIt);
            return;
        }
    }
    throw std::invalid_argument("Page does not exist in the Pager");
}

Pager::~Pager(){
    /// write all files at end of program
    std::cout << "DELETING PAGER" << std::endl;

    for (auto& page: buffer) {
        ioHandler->writeBlock(std::move(page->contents), page->pageNo);
    }

    ioHandler.reset();
}
