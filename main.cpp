#include "IOHandler.h"
#include "Pager.h"
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include "Btree.h"
#include "iostream"


using dataType = std::variant<int, bool, char>;

int main() {

    // some jank chatGPT test code
    // Create some sample file contents representing serialized data
    std::vector<std::byte> serializedData;
    serializedData.push_back(std::byte{0x01}); // boolean: true (0x01)
    serializedData.push_back(std::byte{0x00}); // boolean: false (0x00)
    serializedData.push_back(std::byte{0x12}); // integer: 0x12 (18)
    serializedData.push_back(std::byte{0x00}); // padding
    serializedData.push_back(std::byte{0x00}); // padding
    serializedData.push_back(std::byte{0x00}); // padding
    serializedData.push_back(std::byte{0x34}); // integer: 0x34 (18)
    serializedData.push_back(std::byte{0x00}); // padding
    serializedData.push_back(std::byte{0x00}); // padding
    serializedData.push_back(std::byte{0x11}); // padding
    serializedData.push_back(std::byte{'A'});  // char: 'A'

    // Define the type order
    std::vector<Constants::Types> typeOrder = {
            Constants::Types::BOOLEAN,
            Constants::Types::BOOLEAN,
            Constants::Types::INTEGER,
            Constants::Types::INTEGER,
            Constants::Types::CHAR
    };

    // Deserialize the row
    std::vector<dataType>* deserializedRow = Btree::deserializeRow(serializedData, 0, typeOrder);

    // Print the deserialized data
    for (const auto& attribute : *deserializedRow) {
        if (std::holds_alternative<bool>(attribute)) {
            std::cout << "Boolean: " << std::get<bool>(attribute) << std::endl;
        } else if (std::holds_alternative<int>(attribute)) {
            std::cout << "Integer: " << std::get<int>(attribute) << std::endl;
        } else if (std::holds_alternative<char>(attribute)) {
            std::cout << "Char: " << std::get<char>(attribute) << std::endl;
        }
    }

    auto result = Btree::serializeRow(*deserializedRow, typeOrder);

    for (int i = 0; i < result->size(); ++i) {
        std::cout << std::to_integer<int>(serializedData[i]) << ", " << std::to_integer<int>((*result)[i]) << std::endl;
    }

    return 0;
}