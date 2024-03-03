//
// Created by Kylan Chen on 2/17/24.
//

#pragma once

#include <vector>
#include <variant>
#include "Constants.h"
#include "Pager.h"

using std::vector;
using std::byte;
using dataType = std::variant<int, bool, char>;

class Btree {

private:
    vector<Constants::Types>* rowTypeOrder; // only valid if IS isLeaf
    std::string tableName;
    size_t rowSize;
    vector<byte>* root;

    size_t NODE_HEADER_SIZE = sizeof (size_t) + sizeof(bool) + sizeof(bool) + sizeof(size_t) + sizeof(size_t);
    size_t NODE_SIZE = 0;

    struct NodeHeader {
        size_t pageNo;
        bool isRoot;
        size_t parentNo; // only valid if NOT root
    };

    struct NodeCell {
    };

public:


    Btree(vector<Constants::Types>* rowTypeOrder, std::string& name);

    vector<dataType>* deserializeRow(vector<byte>& pageContents, int offset);

    vector<byte>* serializeRow(vector<dataType>& data);

    NodeHeader* deserializeHeader(vector<byte>& page);

    vector<byte>* serializeHeader(NodeHeader& NodeHeader);
};
