//
// Created by Kylan Chen on 3/18/24.
//

#pragma once

#include <variant>
#include <vector>
#include "Constants.h"
#include "Btree.h"

using std::vector;
using std::byte;
using dataType = std::variant<int, bool, char, size_t>;

class Table {
private:
    vector<Constants::Types> rowTypeOrder;
    std::string tableName;
    size_t rowSize;
    vector<Btree*> indexes;

public:
    Table(const vector<Constants::Types>& rowTypeOrder, std::string tableName);
    const vector<Constants::Types>& getRowTypeOrder() { return rowTypeOrder; }
    std::string getTableName() { return tableName; }
    void addBtree(Btree* btree) { indexes.push_back(btree); }
    size_t getRowSize() { return rowSize; }
};


