//
// Created by Kylan Chen on 2/17/24.
//

#include <iostream>
#include "Btree.h"

using std::vector;
using std::byte;
using dataType = std::variant<int, bool, char>;

// deserializes a string of bytes into their respective data types given a list of types in order
vector<dataType>* Btree::deserializeRow(
        vector<byte>& fileContents, size_t startIndex, vector<Constants::Types>& typeOrder) {

    auto output = new vector<dataType>(typeOrder.size());

    for (int i = 0; i < typeOrder.size(); ++i) {
        switch (typeOrder[i]){
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

// returns byte representation (in list form) of a list of dataTypes
vector<byte>* Btree::serializeRow(
        vector<dataType> &data, vector<Constants::Types>& typeOrder) {

    size_t byteSize = 0;
    for (auto type: typeOrder) {
        switch (type) {
            case Constants::Types::BOOLEAN:
                byteSize += sizeof (bool);
                break;
            case Constants::INTEGER:
                byteSize += sizeof (int);
                break;
            case Constants::CHAR:
                byteSize += sizeof (char);
                break;
        }
    }

    auto outputContent = new vector<byte>(byteSize);
    size_t offset = 0;

    for (int i = 0; i < typeOrder.size(); ++i) {
        switch (typeOrder[i]) {
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

// deserializes a header in order of the attributes in rowHeader
Btree::rowHeader* Btree::deserializeHeader(vector<byte> &fileContents) {
    int currOffset = 0;

    int pageNo;
    std::memcpy(&pageNo, fileContents.data() + currOffset, sizeof (pageNo));
    currOffset += sizeof (pageNo);

    size_t numAttributes;
    std::memcpy(&numAttributes, fileContents.data() + currOffset, sizeof (numAttributes));
    currOffset += sizeof (numAttributes);

    vector<Constants::Types> rowTypeOrder (numAttributes);
    for (int i = 0; i < numAttributes; ++i) {
        Constants::Types dataType;
        std::memcpy(&(dataType), fileContents.data() + currOffset, sizeof (Constants::Types));

        rowTypeOrder[i] = dataType;
        currOffset += sizeof (Constants::Types);
    }

    auto header = new rowHeader{pageNo, numAttributes, &rowTypeOrder};

    return header;
}
