#include "IOHandler.h"
#include "Pager.h"
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include "Btree.h"
#include "iostream"
#include "StorageManager.h"


using dataType = std::variant<int, bool, char>;
using std::cout;
using std::endl;
using std::byte;
using std::vector;

// test serializeRow
// jank af gpt test code
int main() {
    /// flush file data
    /*
    vector<byte> filler (Constants::PAGE_SIZE);
    memset(filler.data(), 0, filler.size());
    size_t numPages = 0;
    std::memcpy(filler.data(), &numPages, sizeof(numPages));
    int fd = open(Constants::DATABASE_FILE.c_str(), O_RDWR | O_CREAT,  S_IWUSR | S_IRUSR);
    write(fd, filler.data(), filler.size());
    fsync(fd);
    close(fd);
     */


    /// this is really important, determines the destruction order of singletons
    /// in reverse order
    sPager;
    sStorageManager;

    auto rowTypeOrder = vector<Constants::Types>();
    rowTypeOrder.push_back(Constants::Types::BOOLEAN);
    rowTypeOrder.push_back(Constants::Types::INTEGER);

    std::string tableName = "test table";
    Btree btree(&rowTypeOrder, tableName);

    vector<dataType> row;
    row.push_back(true);
    row.push_back(1300510961);

    auto binaryData = btree.serializeRow(row);
    for (int i = 0; i < 3000; ++i) {
        auto rowPointer = sStorageManager.insertTuple(binaryData);

        cout << "INSERTED ROW: " << rowPointer.pageNo << ", " << rowPointer.offset << ", " << rowPointer.size << endl;
    }


    return 0;
}