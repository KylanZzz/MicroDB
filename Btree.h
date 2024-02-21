//
// Created by Kylan Chen on 2/17/24.
//

#pragma once

#include <vector>
#include <variant>
#include "Constants.h"

using std::vector;
using std::byte;
using dataType = std::variant<int, bool, char>;

class Btree {

private:

    struct rowHeader {
        int pageNo;
        size_t numAttributes;
        vector<Constants::Types>* rowTypeOrder;
        ~rowHeader() { delete rowTypeOrder; }
    };

public:
    static vector<dataType>* deserializeRow(vector<byte>& fileContents,
                                     size_t startIndex,
                                     vector<Constants::Types>& typeOrder);

    static vector<byte>* serializeRow(vector<dataType>& data,
                                      vector<Constants::Types>& typeOrder);

    static rowHeader* deserializeHeader(vector<byte>& fileContents);

};
