//
// Created by Kylan Chen on 2/17/24.
//

#include <iostream>
#include "Btree.h"

using std::vector;
using std::byte;
using dataType = std::variant<int, bool, char>;

Btree::Btree(vector<Constants::Types>* rowTypeOrder, std::string& name) {
    this->rowTypeOrder = rowTypeOrder;
    this->tableName = name;

    //// initialize row size for easy-of-use in the future
    rowSize = 0;
    for (auto type: (*rowTypeOrder)){
        switch (type) {
            case Constants::Types::BOOLEAN:
               rowSize += sizeof (bool);
               break;
            case Constants::Types::CHAR:
                rowSize += sizeof (char);
                break;
            case Constants::Types::INTEGER:
                rowSize += sizeof (int);
                break;
        }
    }

    /// Initialize the root node
    this->root = sPager->getPage(Constants::BTREE_ROOT)->contents;
}

//// deserializes a string of bytes into their respective data types given a list of types in order
vector<dataType>* Btree::deserializeRow(vector<byte>& fileContents, int startIndex) {

    auto output = new vector<dataType>(rowTypeOrder->size());

    for (int i = 0; i < rowTypeOrder->size(); ++i) {
        switch ((*rowTypeOrder)[i]){
            case Constants::Types::BOOLEAN: {
                bool value;
                std::memcpy(&value, &fileContents[startIndex], sizeof (bool));
                (*output)[i] = value;
                startIndex += sizeof (bool);
                break;
            }
            case Constants::Types::INTEGER: {
                int value;
                std::memcpy(&value, &fileContents[startIndex], sizeof (int ));
                (*output)[i] = value;
                startIndex += sizeof (int);
                break;
            }
            case Constants::Types::CHAR: {
                char value;
                std::memcpy(&value, &fileContents[startIndex], sizeof (char));
                (*output)[i] = value;
                startIndex += sizeof (char);
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

    size_t numBytes = 0;
    for (auto type: *rowTypeOrder) {
        switch (type) {
            case Constants::Types::BOOLEAN:
                numBytes += sizeof (bool);
                break;
            case Constants::Types::INTEGER:
                numBytes += sizeof (int);
                break;
            case Constants::Types::CHAR:
                numBytes += sizeof (char);
                break;
        }
    }

    auto outputContent = new vector<byte>(numBytes);
    int offset = 0;

    for (int i = 0; i < rowTypeOrder->size(); ++i) {
        switch ((*rowTypeOrder)[i]) {
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
