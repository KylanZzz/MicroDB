//
// Created by Kylan Chen on 2/17/24.
//

#include <iostream>
#include "Btree.h"

using std::vector;
using std::byte;
using dataType = std::variant<int, bool, char, size_t>;

Btree::Btree(Table& _table): table(_table), rowTypeOrder(_table.getRowTypeOrder()) {
    table.addBtree(this);

    /// Initialize the root node
    this->root = sPager->getPage(Constants::BTREE_ROOT)->contents;

    /// right now, we will designate the first attribute in rowTypeOrder as the indexing key
    keyIndex = 0;
}

void Btree::insertRow(vector<dataType>& row) {
    /// convert row into bytes
    auto rowBytes = serializeRow(row);

    /// insert row into the fileheap. This is the 'value' of the btree
    auto rowPointer = sFileHeap->insertTuple(rowBytes);

    /// the key is the first element of row
    dataType key = row[keyIndex];

}

vector<dataType>* Btree::search(dataType& key) {

}

/// serializes the btree key
vector<byte>* Btree::serializeKey(dataType& key) {

    auto bytes = new vector<byte>(0);

    switch (rowTypeOrder[keyIndex]){
        case Constants::Types::BOOLEAN: {
            bool value = std::get<bool>(key);
            bytes->resize(sizeof(value));
            std::memcpy(bytes->data(), &value, sizeof(value));
            break;
        }
        case Constants::Types::INTEGER: {
            int value = std::get<int>(key);
            bytes->resize(sizeof(value));
            std::memcpy(bytes->data(), &value, sizeof(value));
            break;
        }
        case Constants::Types::CHAR: {
            bool value = std::get<bool>(key);
            bytes->resize(sizeof(value));
            std::memcpy(bytes->data(), &value, sizeof(value));
            break;
        }
        case Constants::Types::SIZE_T: {
            size_t value = std::get<size_t>(key);
            bytes->resize(sizeof(value));
            std::memcpy(bytes->data(), &value, sizeof(size_t));
            break;
        }
    }

    return bytes;
}

/// deserializes the btree key
dataType Btree::deserializeKey(vector<byte>& bytes) {

    switch (rowTypeOrder[keyIndex]){
        case Constants::Types::BOOLEAN: {
            bool value;
            std::memcpy(&value, bytes.data(), sizeof (bool));
            return value;
        }
        case Constants::Types::INTEGER: {
            int value;
            std::memcpy(&value, bytes.data(), sizeof (int));
            return value;
        }
        case Constants::Types::CHAR: {
            char value;
            std::memcpy(&value, bytes.data(), sizeof (char));
            return value;
        }
        case Constants::Types::SIZE_T: {
            size_t value;
            std::memcpy(&value, bytes.data(), sizeof (size_t));
            return value;
        }
    }
}

//// deserializes a string of bytes into their respective data types given a list of types in order
vector<dataType>* Btree::deserializeRow(vector<byte>& bytes) {
    int offset = 0;

    auto output = new vector<dataType>(rowTypeOrder.size());

    for (int i = 0; i < rowTypeOrder.size(); ++i) {
        switch (rowTypeOrder[i]){
            case Constants::Types::BOOLEAN: {
                bool value;
                std::memcpy(&value, bytes.data() + offset, sizeof (bool));
                (*output)[i] = value;
                offset += sizeof (bool);
                break;
            }
            case Constants::Types::INTEGER: {
                int value;
                std::memcpy(&value, bytes.data() + offset, sizeof (int ));
                (*output)[i] = value;
                offset += sizeof (int);
                break;
            }
            case Constants::Types::CHAR: {
                char value;
                std::memcpy(&value, bytes.data() + offset, sizeof (char));
                (*output)[i] = value;
                offset += sizeof (char);
                break;
            }
            case Constants::Types::SIZE_T: {
                size_t value;
                std::memcpy(&value, bytes.data() + offset, sizeof (size_t));
                (*output)[i] = value;
                offset += sizeof (size_t);
                break;
            }
                // doing this shit later bc fuck strings
//            case Constants::STRING:
//                std::string stringValue;
//                std::memcpy(&stringValue, &contents[startIndex], sizeof (char) * Constants::NUM_CHARS_IN_STRING);
//                (*output)[i] = stringValue;
//                startIndex += sizeof (char) * Constants::NUM_CHARS_IN_STRING;
//                break;
        }
    }

    return output;
}

//// returns byte representation (in list form) of a list of dataTypes
vector<byte>* Btree::serializeRow(vector<dataType> &data) {

    auto outputContent = new vector<byte>(table.getRowSize());
    int offset = 0;

    for (int i = 0; i < rowTypeOrder.size(); ++i) {
        switch (rowTypeOrder[i]) {
            case Constants::Types::BOOLEAN: {
                bool value = std::get<bool>(data[i]);
                std::memcpy(outputContent->data() + offset, &value, sizeof(value));
                offset += sizeof(value);
                break;
            }
            case Constants::Types::INTEGER: {
                int value = std::get<int>(data[i]);
                std::memcpy(outputContent->data() + offset, &value, sizeof(value));
                offset += sizeof(value);
                break;
            }
            case Constants::Types::CHAR: {
                char value = std::get<char>(data[i]);
                std::memcpy(outputContent->data() + offset, &value, sizeof(value));
                offset += sizeof(value);
                break;
            }
            case Constants::Types::SIZE_T: {
                size_t value = std::get<size_t>(data[i]);
                std::memcpy(outputContent->data() + offset, &value, sizeof(value));
                offset += sizeof(value);
                break;
            }
        }
    }

    return outputContent;
}

//// deserializes a header in order of the attributes in rowHeader
//Btree::NodeHeader* Btree::deserializeHeader(vector<byte> &fileContents) {
//    int currOffset = 0;
//
//    int pageNo;
//    std::memcpy(&pageNo, fileContents.data() + currOffset, sizeof (pageNo));
//    currOffset += sizeof (pageNo);
//
//    int numAttributes;
//    std::memcpy(&numAttributes, fileContents.data() + currOffset, sizeof (numAttributes));
//    currOffset += sizeof (numAttributes);
//
//    auto rowTypeOrder = new vector<Constants::Types>(numAttributes);
//    for (int i = 0; i < numAttributes; ++i) {
//        Constants::Types dataType;
//        std::memcpy(&(dataType), fileContents.data() + currOffset, sizeof (Constants::Types));
//
//        (*rowTypeOrder)[i] = dataType;
//        currOffset += sizeof (Constants::Types);
//    }
//
//    auto header = new NodeHeader{pageNo, numAttributes, rowTypeOrder};
//
//    return header;
//}
//
//vector<byte>* Btree::serializeHeader(Btree::NodeHeader& header) {
//    size_t size = sizeof(header.pageNo) +
//            sizeof(header.numAttributes) +
//            (header.numAttributes * sizeof(Constants::Types));
//
//    auto fileHeaderBytes = new vector<byte>(size);
//    int offset = 0;
//
//    std::memcpy(fileHeaderBytes->data() + offset, &(header.pageNo), sizeof(header.pageNo));
//    offset += sizeof(header.pageNo);
//
//    std::memcpy(fileHeaderBytes->data() + offset, &(header.numAttributes), sizeof(header.numAttributes));
//    offset += sizeof(header.numAttributes);
//
//    for (int i = 0; i < header.numAttributes; ++i) {
//        std::memcpy(fileHeaderBytes->data() + offset, &((*header.rowTypeOrder)[i]), sizeof(Constants::Types));
//        offset += sizeof (Constants::Types);
//    }
//
//    return fileHeaderBytes;
//}
