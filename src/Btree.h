//
// Created by Kylan Chen on 2/17/24.
//

#pragma once

#include <vector>
#include <variant>
#include "Constants.h"
#include "Pager.h"
#include "Table.h"

using std::vector;
using std::byte;
using dataType = std::variant<int, bool, char, size_t>;

class Btree {

private:
    size_t keyIndex;
    vector<byte>* root;
    Table& table;
    const vector<Constants::Types>& rowTypeOrder;

    size_t NODE_HEADER_SIZE = sizeof (size_t) + sizeof(bool) + sizeof(size_t);
    size_t NODE_SIZE = 0;

    struct NodeHeader {
        size_t pageNo;
        bool isRoot;
        size_t parentNo; // only valid if NOT root
    };


public:

    Btree(Table& table);

    void insertRow(vector<dataType>& row);
    void removeRow(dataType& key);
    vector<dataType>* search(dataType& key);
    vector<byte>* serializeKey(dataType& key);
    dataType deserializeKey(vector<byte>& bytes);
    vector<byte>* serializeRow(vector<dataType>& data);
    vector<dataType>* deserializeRow(vector<byte>& pageContents);
//    NodeHeader* deserializeHeader(vector<byte>& page);
//    vector<byte>* serializeHeader(NodeHeader& NodeHeader);
};
