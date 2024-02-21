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

    // 4 bytes pageNo ---- 8 bytes numAttributes --
    // -- 4 bytes (size of enum) * (# attributes)
//    struct pageHeader {
//        int pageNo;
//        int numAttributes;
//        vector<Constants::Types>* rowTypeOrder;
//        ~pageHeader() { delete rowTypeOrder; }
//    };

public:
    struct pageHeader {
        int pageNo;
        int numAttributes;
        vector<Constants::Types>* rowTypeOrder;
        ~pageHeader() { delete rowTypeOrder; }
    };

    static vector<dataType>* deserializeRow(vector<byte>& fileContents,
                                            int startIndex,
                                     vector<Constants::Types>& typeOrder);

    static vector<byte>* serializeRow(vector<dataType>& data,
                                      vector<Constants::Types>& typeOrder);

    static pageHeader* deserializeHeader(vector<byte>& fileContents);

    static vector<byte>* serializeHeader(pageHeader& pageHeader);
};
