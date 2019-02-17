// -----------------------------------------------------------------------------------
// utility functions for testing and demonstrating TextBuffer capabilities
// -----------------------------------------------------------------------------------

#include "bufferUtils.h"

#include <fstream>
#include <sstream>

// Create a file for testing load functionality
void stringToFile(const std::string& fileName, const std::string& fileContent)
{
    std::ofstream outStream;
    outStream.open(fileName);
    outStream << fileContent;
    outStream.close();
}

// wraps file removal functionality
void removeFile(const std::string& referenceName)
{
    std::remove(referenceName.c_str());
}

// Loads contents of a file to a string
std::string fileToString(const std::string& fileName)
{
    std::ifstream fileStream(fileName);
    std::stringstream stringStream;
    stringStream << fileStream.rdbuf();
    return stringStream.str();
}

// Compares string content of two files
bool compareFileContents(const std::string& file1, const std::string& file2)
{
    auto string1 = fileToString(file1);
    auto string2 = fileToString(file2);
    return (string1 == string2);
}

// Convenience function to set up a TextBuffer with a specific text content
TextBuffer genTextBuffer(const std::string& content)
{
    stringToFile("temp.txt", content);
    TextBuffer buf("temp.txt");
    removeFile("temp.txt");
    return buf;
}
