//
// Created by Kylan Chen on 3/18/24.
//

#include "Table.h"

Table::Table(const vector<Constants::Types>& rowTypeOrder, std::string tableName) {
    //// initialize row size for easy-of-use in the future
    this->rowTypeOrder = rowTypeOrder;
    this->tableName = tableName;

    rowSize = 0;
    for (auto type: rowTypeOrder){
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
            case Constants::Types::SIZE_T:
                rowSize += sizeof (size_t);
                break;
        }
    }
}