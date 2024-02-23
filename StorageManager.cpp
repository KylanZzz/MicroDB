//
// Created by Kylan Chen on 2/22/24.
//

#include "StorageManager.h"

StorageManager::StorageManager() {
    pageDirectory = sPager->getPage(Constants::PageNo::PAGE_DIRECTORY)->contents;

    //// process the pageDirectory, storing each page along with its # free bytes in a list

    int offset = 0;
    int numPages;
    std::memcpy(&numPages, pageDirectory->data() + offset, sizeof(numPages));
    offset += sizeof(numPages);

    tuplePages = new vector<std::pair<size_t, size_t>>(numPages);
    for (auto & tuplePage : *tuplePages) {
        //// initialize each page of pageDirectory
        int pageNo;
        memcpy(&pageNo, pageDirectory->data() + offset, sizeof(pageNo));
        offset += sizeof(pageNo);

        int freeSpace;
        memcpy(&freeSpace, pageDirectory->data() + offset, sizeof(freeSpace));
        offset += sizeof(freeSpace);

        tuplePage = std::make_pair(pageNo, freeSpace);
    }

}

StorageManager::RowPointer StorageManager::insertTuple(vector<byte>* data) {
    for (auto & tuplePage : *tuplePages) {
        size_t pageNo = tuplePage.first;
        size_t spaceLeft = tuplePage.second;
        if (spaceLeft >= data->size()) {
            /// IMPORTANT: assuming that heap files are stored top down,
            /// with no header and only tuples information

            /// get page from pager
            auto page = sPager->getPage(pageNo);

            /// insert data
            size_t offset = Constants::PAGE_SIZE - spaceLeft;
            std::memcpy(page->contents->data() + offset,data->data(), data->size());

            /// adjust spaceLeft offset
            tuplePage.second = tuplePage.second - data->size();

            return RowPointer{pageNo, offset, data->size()};
        }
    }

    /// if there is not enough free space in ANY of the pages, make a new one
    auto newPage = sPager->makeNewPage();

    /// insert data
    std::memcpy(newPage->contents->data(),data->data(), data->size());

    /// add data into tuplePages
    tuplePages->emplace_back(newPage->pageNo, Constants::PAGE_SIZE - data->size());

    return RowPointer {newPage->pageNo, 0, data->size()};

}

void StorageManager::deleteTuple(StorageManager::RowPointer row) {

}




StorageManager::~StorageManager() {
    /// update page directory
    size_t offset = 0;

    size_t numPages = tuplePages->size();
    std::memcpy(pageDirectory->data() ,&numPages, sizeof (numPages));
    offset += sizeof(numPages);

    for (auto& tuplePage: *tuplePages) {
        size_t pageNo = tuplePage.first;
        std::memcpy(pageDirectory->data() + offset, &pageNo, sizeof(pageNo));
        offset += sizeof(pageNo);

        size_t freeSpace = tuplePage.second;
        std::memcpy(pageDirectory->data() + offset, &freeSpace, sizeof(freeSpace));
        offset += sizeof(freeSpace);
    }

    /// free memory
    delete pageDirectory;
    delete tuplePages;
}
