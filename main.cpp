#include "IOHandler.h"
#include "Pager.h"
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include "Btree.h"
#include "iostream"


using dataType = std::variant<int, bool, char>;
using std::cout;
using std::endl;

// test serializeRow
// jank af gpt test code
int main() {
    // Prepare data and type order for serialization
    vector<dataType> data = {true, 42, 'a'};
    vector<Constants::Types> typeOrder = {Constants::Types::BOOLEAN, Constants::Types::INTEGER, Constants::Types::CHAR};

    // Serialize the row
    vector<byte>* serializedData = Btree::serializeRow(data, typeOrder);

    // Assume we start reading from the beginning of the serialized data
    int startIndex = 0;

    // Deserialize the row
    vector<dataType>* deserializedData = Btree::deserializeRow(*serializedData, startIndex, typeOrder);

    // Verify the deserialized data
    bool deserializedBool = std::get<bool>((*deserializedData)[0]);
    int deserializedInt = std::get<int>((*deserializedData)[1]);
    char deserializedChar = std::get<char>((*deserializedData)[2]);

    std::cout << "Deserialized Boolean: " << deserializedBool << std::endl;
    std::cout << "Deserialized Integer: " << deserializedInt << std::endl;
    std::cout << "Deserialized Char: " << deserializedChar << std::endl;

    // Clean up
    delete serializedData;
    delete deserializedData;

    return 0;
}

/*
// more gpt code to quickly test (i swear ill make real tests later)
int main() {
    // Create a mock header to serialize
    int pageNo = 123;
    int numAttributes = 2;
    vector<Constants::Types> rowTypeOrder = {Constants::Types::INTEGER, Constants::Types::CHAR};

    // Assuming pageHeader takes a vector directly now, based on your setup
    Btree::pageHeader originalHeader{pageNo, numAttributes, &rowTypeOrder};

    // Serialize the header
    vector<byte>* serializedHeader = Btree::serializeHeader(originalHeader);

    // Mock file contents as if it's read from a file, but actually using serialized data
    vector<byte> fileContents(serializedHeader->begin(), serializedHeader->end());

    // Deserialize the header
    auto deserializedHeader = Btree::deserializeHeader(fileContents);

    // Test to see if deserialization matches the original
    cout << "Original Page No: " << pageNo << ", Deserialized Page No: " << deserializedHeader->pageNo << endl;
    cout << "Original Num Attributes: " << numAttributes << ", Deserialized Num Attributes: " << deserializedHeader->numAttributes << endl;
    cout << "Original First Type: " << static_cast<int>(rowTypeOrder[0]) << ", Deserialized First Type: " << static_cast<Constants::Types>((*deserializedHeader->rowTypeOrder)[0]) << endl;
    cout << "Original Second Type: " << static_cast<int>(rowTypeOrder[1]) << ", Deserialized Second Type: " << static_cast<Constants::Types>((*deserializedHeader->rowTypeOrder)[1]) << endl;

    // Clean up dynamically allocated memory (assuming you're using raw pointers for this example)
    delete serializedHeader;
    delete deserializedHeader->rowTypeOrder; // If rowTypeOrder is dynamically allocated within deserializeHeader
    delete deserializedHeader;

    return 0;
}*/