//
// Created by Kylan Chen on 2/28/24.
//

#define CATCH_CONFIG_MAIN
#include "../src/FileHeap.h"
#include "../src/Btree.h"
#include "iostream"
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <catch2/catch_test_macros.hpp>
#define makeByte(x) static_cast<std::byte>(x)

using dataType = std::variant<int, bool, char, size_t>;
using std::cout;
using std::endl;
using std::byte;
using std::vector;


TEST_CASE("Storage Manager inserts into Heap Files", "[FileHeap]") {

    /* deletes existing DB file */
    unlink(Constants::DATABASE_FILE.c_str());


    /* creates new db file, fills it with data   */
    /* data format: 0 -> page_directory     1 -> btree root  */
    /* page 0:  numPages (8 bytes)     heapFile data:   ( (pageNo (8) + spaceLeft(8)) * numPages bytes)  */
    /* page 1: empty (for now)  */
    vector<byte> data (Constants::PAGE_SIZE * 2, makeByte(0));

    size_t numHeapPages = 0;
    std::memcpy(data.data(), &numHeapPages, sizeof(numHeapPages));

    int fd = open(Constants::DATABASE_FILE.c_str(), O_RDWR | O_CREAT,  S_IWUSR | S_IRUSR);
    write(fd, data.data(), data.size());
    fsync(fd);
    close(fd);

    sPager;
    sFileHeap;

    /* test one tuple */
    auto* initialData = new vector<byte>(8);
    auto* insertionData = new vector<byte>(8);
    for (int i = 0; i < insertionData->size(); ++i) {
        (*insertionData)[i] = makeByte((i + 1) * 10);
        (*initialData)[i] = makeByte((i + 1) * 10);
    }
    auto rowPointer = sFileHeap->insertTuple(std::unique_ptr<vector<byte>>(insertionData));

    int fd2 = open(Constants::DATABASE_FILE.c_str(), O_RDONLY);
    auto* actualData = new vector<byte>(8);
    pread(fd2, actualData->data(), rowPointer.size, (rowPointer.pageNo * Constants::PAGE_SIZE) + rowPointer.offset);
    close(fd2);

    REQUIRE(rowPointer.pageNo == 2);
    REQUIRE(rowPointer.offset == 0);
    REQUIRE(actualData->size() == initialData->size());
    REQUIRE((*actualData) == (*initialData));
//
//
//    /* test another tuple */
//    auto insertionData2 = new vector<byte>(3000);
//    for (int i = 0; i < insertionData2->size(); ++i) {
//        (*insertionData2)[i] = makeByte(i);
//    }
//    auto rowPointer2 = sFileHeap->insertTuple(std::unique_ptr<vector<byte>>(insertionData2));
//
//    int fd3 = open(Constants::DATABASE_FILE.c_str(), O_RDONLY);
//    auto* actualData2 = new vector<byte>(3000);
//    pread(fd3, actualData2->data(), rowPointer2.size, (rowPointer2.pageNo * Constants::PAGE_SIZE) + rowPointer2.offset);
//    close(fd3);
//
//    sPager;
//    sFileHeap;
//
//    REQUIRE(rowPointer2.pageNo == 2);
//    REQUIRE(rowPointer2.offset == 8);
//    REQUIRE((*actualData2) == (*insertionData2));
//
//
//    /* test that another page is made if there isn't enough space */
//    auto insertionData3 = new vector<byte>(3000);
//    for (int i = 0; i < insertionData3->size(); ++i) {
//        (*insertionData3)[i] = makeByte(i);
//    }
//    auto rowPointer3 = sFileHeap->insertTuple(std::unique_ptr<vector<byte>>(insertionData3));
//
//    int fd4 = open(Constants::DATABASE_FILE.c_str(), O_RDONLY);
//    auto* actualData3 = new vector<byte>(3000);
//    pread(fd4, actualData3->data(), rowPointer3.size, (rowPointer3.pageNo * Constants::PAGE_SIZE) + rowPointer3.offset);
//    close(fd4);
//
//    REQUIRE(rowPointer3.pageNo == 3);
//    REQUIRE(rowPointer3.offset == 0);
//    REQUIRE((*actualData3) == (*insertionData3));

    FileHeap::deleteInstance();
    Pager::deleteInstance();
}

TEST_CASE("Pager createNewPage works", "[Pager]") {
    /* deletes existing DB file */
    unlink(Constants::DATABASE_FILE.c_str());


    /* creates new db file, fills it with data   */
    /* data format: 0 -> page_directory     1 -> btree root  */
    /* page 0:  numPages (8 bytes)     heapFile data:   ( (pageNo (8) + spaceLeft(8)) * numPages bytes)  */
    /* page 1: empty (for now)  */
    vector<byte> data (Constants::PAGE_SIZE * 2, makeByte(0));

    size_t numHeapPages = 0;
    std::memcpy(data.data(), &numHeapPages, sizeof(numHeapPages));

    int fd = open(Constants::DATABASE_FILE.c_str(), O_RDWR | O_CREAT,  S_IWUSR | S_IRUSR);
    write(fd, data.data(), data.size());
    fsync(fd);
    close(fd);

    sPager;
    sFileHeap;

    // there are 2 pages by default (root & page directory) so the next page to be made is the third
    // page with a pageNo of 2 (0-indexed counting)
    auto newPage = sPager->makeNewPage().lock();
    auto retrievedPage = sPager->getPage(2).lock();
    REQUIRE(retrievedPage == newPage);

    auto emptyContents = new vector<byte>(Constants::PAGE_SIZE, makeByte(0));
    REQUIRE(*emptyContents == *(retrievedPage->contents));
    REQUIRE(*emptyContents == *(newPage->contents));

    FileHeap::deleteInstance();
    Pager::deleteInstance();
}

TEST_CASE("Pager writes page to disk works", "[Pager]") {
    /* deletes existing DB file */
    unlink(Constants::DATABASE_FILE.c_str());


    /* creates new db file, fills it with data   */
    /* data format: 0 -> page_directory     1 -> btree root  */
    /* page 0:  numPages (8 bytes)     heapFile data:   ( (pageNo (8) + spaceLeft(8)) * numPages bytes)  */
    /* page 1: empty (for now)  */
    vector<byte> data (Constants::PAGE_SIZE * 2, makeByte(0));

    size_t numHeapPages = 0;
    std::memcpy(data.data(), &numHeapPages, sizeof(numHeapPages));

    int fd = open(Constants::DATABASE_FILE.c_str(), O_RDWR | O_CREAT,  S_IWUSR | S_IRUSR);
    write(fd, data.data(), data.size());
    fsync(fd);
    close(fd);

    sPager;
    sFileHeap;

    auto firstPage = sPager->getPage(0).lock();
    for (int i = 0; i < firstPage->contents->size(); ++i) {
        (*(firstPage->contents))[i] = makeByte(i % 100);
    }
    sPager->writePage(0);

    int fd2 = open(Constants::DATABASE_FILE.c_str(), O_RDONLY);
    auto outputData = new vector<byte>(Constants::PAGE_SIZE);
    pread(fd2, outputData->data(), Constants::PAGE_SIZE, 0);

    for (int i = 0; i < outputData->size(); ++i) {
        auto byte = (*(outputData))[i];
        REQUIRE(byte == makeByte(i % 100));
    }

    FileHeap::deleteInstance();
    Pager::deleteInstance();
}