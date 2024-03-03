#include "IOHandler.h"
#include "Pager.h"
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include "Btree.h"
#include "iostream"
#include "FileHeap.h"

#define makeByte(x) static_cast<std::byte>(x)

int main() {
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

    // test code in here
    FileHeap::deleteInstance();
    Pager::deleteInstance();
    return 0;
}