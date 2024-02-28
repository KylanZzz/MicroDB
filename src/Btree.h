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

    struct NodeHeader {
        int pageNo;
        bool isLeaf;
        bool isRoot;

        int parentNo; // only valid if NOT root
        int numRows; // only valid if IS leaf
    };

public:


    Btree(vector<Constants::Types>* rowTypeOrder, std::string& name);

    vector<dataType>* deserializeRow(vector<byte>& pageContents, int offset);

    vector<byte>* serializeRow(vector<dataType>& data);

    NodeHeader* deserializeHeader(vector<byte>& page);

    vector<byte>* serializeHeader(NodeHeader& NodeHeader);
};
