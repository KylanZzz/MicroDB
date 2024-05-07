//
// Created by Kylan Chen on 2/22/24.
//

#include "FileHeap.h"
#include "iostream"
#include "exception"

FileHeap* FileHeap::INSTANCE = nullptr;

FileHeap::FileHeap() {
    auto pageDirectory = sPager->getPage(Constants::PageNo::PAGE_DIRECTORY);
    /// process the pageDirectory, storing each page along with its # free bytes in a list

    if (auto locked = pageDirectory.lock()) {
        size_t offset = 0;
        size_t numPages;
        std::memcpy(&numPages, locked->contents->data() + offset, sizeof(numPages));
        offset += sizeof(numPages);

        tuplePages.resize(numPages);
        for (auto& tuplePage : tuplePages) {
            /// initialize each page of pageDirectory
            size_t pageNo;
            memcpy(&pageNo, locked->contents->data() + offset, sizeof(pageNo));
            offset += sizeof(pageNo);

            size_t freeSpace;
            memcpy(&freeSpace, locked->contents->data() + offset, sizeof(freeSpace));
            offset += sizeof(freeSpace);

            tuplePage = std::make_pair(pageNo, freeSpace);
        }
    } else {
        throw std::logic_error("Could not get page directory from Pager");
    }

}

/// inserts a tuple into the file heap and returns a rowPointer (page & offset) to the tuple
/// TODO: right now changes are written every insertion, which may not be the most optimal
FileHeap::RowPointer FileHeap::insertTuple(std::unique_ptr<vector<byte>> tupleData) {
    for (auto& tuplePage : tuplePages) {
        size_t pageNo = tuplePage.first;
        size_t spaceLeft = tuplePage.second;
        if (spaceLeft >= tupleData->size()) {
            /// get page from pager
            auto page = sPager->getPage(pageNo);
            if (auto locked = page.lock()) {
                /// insert data
                size_t offset = Constants::PAGE_SIZE - spaceLeft;
                std::memcpy(locked->contents->data() + offset,tupleData->data(), tupleData->size());

                /// adjust spaceLeft offset
                tuplePage.second = tuplePage.second - tupleData->size();

                sPager->writePage(pageNo);
                return RowPointer{pageNo, offset, tupleData->size()};
            } else {
                throw std::logic_error("Attempting to lock empty weak pointer");
            }
        }
    }

    /// if there is not enough free space in ANY of the pages, make a new one
    auto newPage = sPager->makeNewPage();

    if (auto locked = newPage.lock()) {
        /// insert data
        std::memcpy(locked->contents->data(),tupleData->data(), tupleData->size());

        /// add data into tuplePages
        tuplePages.emplace_back(locked->pageNo, Constants::PAGE_SIZE - tupleData->size());

        sPager->writePage(locked->pageNo);
        return RowPointer {locked->pageNo, 0, tupleData->size()};
    } else {
        throw std::logic_error("Could not lock weak pointer from Pager");
    }
}

void FileHeap::deleteTuple(FileHeap::RowPointer row) {

}

FileHeap::~FileHeap() {
    std::cout << "DELETING FILE HEAP" << std::endl;

    auto pageDirectory = sPager->getPage(Constants::PageNo::PAGE_DIRECTORY);

    if (auto locked = pageDirectory.lock()) {
        /// update page directory
        size_t offset = 0;

        size_t numPages = tuplePages.size();
        std::memcpy(locked->contents->data() ,&numPages, sizeof (numPages));
        offset += sizeof(numPages);

        for (const auto& tuplePage: tuplePages) {
            size_t pageNo = tuplePage.first;
            std::memcpy(locked->contents->data() + offset, &pageNo, sizeof(pageNo));
            offset += sizeof(pageNo);

            size_t freeSpace = tuplePage.second;
            std::memcpy(locked->contents->data() + offset, &freeSpace, sizeof(freeSpace));
            offset += sizeof(freeSpace);
        }
    }
}
