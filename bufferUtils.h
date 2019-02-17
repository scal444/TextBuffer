// defines utility functions for testing and demonstrating TextBuffer capabilities

#ifndef TEXTBUFFER_BUFFERUTILS_H
#define TEXTBUFFER_BUFFERUTILS_H

#include <string>
#include "TextBuffer.h"

// Create a file for testing load functionality
void stringToFile(const std::string& fileName, const std::string& fileContent);

// wraps file removal functionality
void removeFile(const std::string& referenceName);

// Loads contents of a file to a string
std::string fileToString(const std::string& fileName);

// Compares string content of two files
bool compareFileContents(const std::string& file1, const std::string& file2);

// Convenience function to set up a TextBuffer with a specific text content
TextBuffer genTextBuffer(const std::string& content);


#endif //TEXTBUFFER_BUFFERUTILS_H
