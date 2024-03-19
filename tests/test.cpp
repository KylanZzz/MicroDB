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
    auto* insertionData = new vector<byte>(8);
    for (int i = 0; i < insertionData->size(); ++i) {
        (*insertionData)[i] = makeByte((i + 1) * 10);
    }
    auto rowPointer = sFileHeap->insertTuple(insertionData);

    int fd2 = open(Constants::DATABASE_FILE.c_str(), O_RDONLY);
    auto* actualData = new vector<byte>(8);
    pread(fd2, actualData->data(), rowPointer.size, (rowPointer.pageNo * Constants::PAGE_SIZE) + rowPointer.offset);
    close(fd2);

    REQUIRE(rowPointer.pageNo == 2);
    REQUIRE(rowPointer.offset == 0);
    REQUIRE((*actualData) == (*insertionData));


    /* test another tuple */
    auto insertionData2 = new vector<byte>(3000);
    for (int i = 0; i < insertionData2->size(); ++i) {
        (*insertionData2)[i] = makeByte(i);
    }
    auto rowPointer2 = sFileHeap->insertTuple(insertionData2);

    int fd3 = open(Constants::DATABASE_FILE.c_str(), O_RDONLY);
    auto* actualData2 = new vector<byte>(3000);
    pread(fd3, actualData2->data(), rowPointer2.size, (rowPointer2.pageNo * Constants::PAGE_SIZE) + rowPointer2.offset);
    close(fd3);

    sPager;
    sFileHeap;

    REQUIRE(rowPointer2.pageNo == 2);
    REQUIRE(rowPointer2.offset == 8);
    REQUIRE((*actualData2) == (*insertionData2));


    /* test that another page is made if there isn't enough space */
    auto insertionData3 = new vector<byte>(3000);
    for (int i = 0; i < insertionData3->size(); ++i) {
        (*insertionData3)[i] = makeByte(i);
    }
    auto rowPointer3 = sFileHeap->insertTuple(insertionData3);

    int fd4 = open(Constants::DATABASE_FILE.c_str(), O_RDONLY);
    auto* actualData3 = new vector<byte>(3000);
    pread(fd4, actualData3->data(), rowPointer3.size, (rowPointer3.pageNo * Constants::PAGE_SIZE) + rowPointer3.offset);
    close(fd4);

    REQUIRE(rowPointer3.pageNo == 3);
    REQUIRE(rowPointer3.offset == 0);
    REQUIRE((*actualData3) == (*insertionData3));

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
    auto newPage = sPager->makeNewPage();
    auto retrievedPage = sPager->getPage(2);
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

    auto firstPage = sPager->getPage(0);
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

TEST_CASE("Btree deserializing row works", "[Btree]") {
    vector<Constants::Types> rowTypeOrder = {
            Constants::Types::INTEGER,
            Constants::Types::BOOLEAN,
            Constants::Types::CHAR,
            Constants::Types::SIZE_T,
    };

    std::string databaseName = "row deserialization test table";

    Table table(rowTypeOrder, databaseName);
    Btree btree = Btree(table);

    vector<byte> inputBytes = {
            makeByte(0x11), // start of int
            makeByte(0x00),
            makeByte(0x00),
            makeByte(0x00), // end of int
            makeByte(0x01), // start & end of bool
            makeByte(0x5A), // start & end of char
            makeByte(0x22), // start of size_t
            makeByte(0x00),
            makeByte(0x00),
            makeByte(0x00),
            makeByte(0x00),
            makeByte(0x00),
            makeByte(0x00),
            makeByte(0x00), // end of size_t

    };

    auto deserializedRow = btree.deserializeRow(inputBytes);

    vector<dataType> expectedOutput = {
            17,
            true,
            'Z',
            static_cast<size_t>(34)
    };

    REQUIRE(deserializedRow->size() == expectedOutput.size());
    REQUIRE(*deserializedRow == expectedOutput);
}

TEST_CASE("Btree serializing row works", "[Btree]") {
    vector<Constants::Types> rowTypeOrder = {
            Constants::Types::INTEGER,
            Constants::Types::BOOLEAN,
            Constants::Types::CHAR,
            Constants::Types::SIZE_T,
    };

    std::string databaseName = "row deserialization test table";

    Table table(rowTypeOrder, databaseName);
    Btree btree = Btree(table);

    vector<dataType> inputData = {
            3,
            true,
            'D',
            static_cast<size_t>(38)
    };

    auto serializedRow = btree.serializeRow(inputData);

    vector<byte> expectedBytes = {
            makeByte(0x03), // start of int
            makeByte(0x00),
            makeByte(0x00),
            makeByte(0x00), // end of int
            makeByte(0x01), // start & end of bool
            makeByte(0x44), // start & end of char
            makeByte(0x26), // start of size_t
            makeByte(0x00),
            makeByte(0x00),
            makeByte(0x00),
            makeByte(0x00),
            makeByte(0x00),
            makeByte(0x00),
            makeByte(0x00), // end of size_t
    };

    serializedRow->data();
    REQUIRE(serializedRow->size() == expectedBytes.size());
    REQUIRE(*serializedRow == expectedBytes);
}

TEST_CASE("Btree deserializing key works", "[Btree]") {
    vector<Constants::Types> rowTypeOrder = {
            Constants::Types::INTEGER,
            Constants::Types::BOOLEAN,
            Constants::Types::CHAR,
            Constants::Types::SIZE_T,
    };

    std::string databaseName = "row deserialization test table";

    Table table(rowTypeOrder, databaseName);
    Btree btree = Btree(table);

    vector<byte> inputBytes = {
            makeByte(0x14), // 16 + 4 = 20
            makeByte(0x00),
            makeByte(0x00),
            makeByte(0x00)
    };

    dataType deserializedKey = btree.deserializeKey(inputBytes);
    int deserializedKeyAsInteger = *std::get_if<int>(&deserializedKey);
    int expectedResult = 20;

    REQUIRE(expectedResult == deserializedKeyAsInteger);
}

TEST_CASE("Btree serializing key works", "[Btree]") {
    vector<Constants::Types> rowTypeOrder = {
            Constants::Types::INTEGER,
            Constants::Types::BOOLEAN,
            Constants::Types::CHAR,
            Constants::Types::SIZE_T,
    };

    std::string databaseName = "row deserialization test table";

    Table table(rowTypeOrder, databaseName);
    Btree btree = Btree(table);

    dataType inputKey = 58;

    auto serializedKey = btree.serializeKey(inputKey);

    vector<byte> expectedOutput = {
            makeByte(0x3A), // (3)(16) + 10 = 58
            makeByte(0x00),
            makeByte(0x00),
            makeByte(0x00)
    };

    REQUIRE(serializedKey->size() == expectedOutput.size());
    REQUIRE(*serializedKey == expectedOutput);
}

