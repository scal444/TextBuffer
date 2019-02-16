/* unit tests for TextBuffer class */

#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "TextBuffer.h"
#include <stdio.h>
#include <cstdio>

// -----------------------------------------------------------------------------------
// utility functions for tests
// -----------------------------------------------------------------------------------

// Create a file for testing load functionality
// TODO this should be a general testing utility (e.g. testutils.h)
static void stringToFile(const std::string& fileName, const std::string& fileContent)
{
    std::ofstream outStream;
    outStream.open(fileName);
    outStream << fileContent;
    outStream.close();
}

// wraps file removal functionality
// TODO this should be a general testing utility (e.g. testutils.h)
static void removeFile(const std::string& referenceName)
{
    std::remove(referenceName.c_str());
}

// Loads contents of a file to a string
// TODO this is duplicated in the TextBuffer class and should be in e.g. a stringutils header
static std::string fileToString(const std::string& fileName)
{
    std::ifstream fileStream(fileName);
    std::stringstream stringStream;
    stringStream << fileStream.rdbuf();
    return stringStream.str();
}

// Compares string content of two files
// TODO this should be a general testing utility (e.g. testutils.h)
static bool compareFileContents(const std::string& file1, const std::string& file2)
{
    auto string1 = fileToString(file1);
    auto string2 = fileToString(file2);
    return (string1 == string2);
}

// Convenience function to set up a TextBuffer with a specific text content
static TextBuffer genTextBuffer(const std::string& content)
{
    stringToFile("temp.txt", content);
    TextBuffer buf("temp.txt");
    removeFile("temp.txt");
    return buf;
}
// -----------------------------------------------------------------------------------
// Tests for TextBuffer
// -----------------------------------------------------------------------------------
TEST_CASE("TextBuffer loads")
{
    const std::string referenceString = "Hello world\nSecond line";
    const std::string loadFilePath = "sample_input.txt";
    stringToFile(loadFilePath, referenceString);
    SECTION("Load with constructor")
    {
        TextBuffer buffer(loadFilePath);
        REQUIRE(buffer.getString() == referenceString);
    }

    SECTION("Load with member")
    {
        TextBuffer buffer;
        buffer.loadFromText(loadFilePath);
        REQUIRE(buffer.getString() == referenceString);
    }
    removeFile(loadFilePath);
}

TEST_CASE("Textbuffer writes")
{
    const std::string referenceString = "Test write reference";
    const std::string referencePath = "reference.txt";
    const std::string writePath = "write_output.txt";
    stringToFile(referencePath, referenceString);

    TextBuffer buffer(referencePath);
    buffer.saveToText(writePath);

    REQUIRE(compareFileContents(referencePath, writePath));

    removeFile(referencePath);
    removeFile(writePath);
}

TEST_CASE("Textbuffer appending")
{
    std::string startString = "text to append";
    auto buffer = genTextBuffer(startString);
    buffer.appendSubstring(" - appended");
    REQUIRE(buffer.getString() == "text to append - appended");
}

TEST_CASE("TextBuffer insertion")
{
    auto buffer = genTextBuffer("word1 word2");
    buffer.insertSubstring(" word3", 5);
    REQUIRE(buffer.getString() == "word1 word3 word2");
}

TEST_CASE("TextBuffer deleting")
{
    auto buffer = genTextBuffer("word1 word2");

    SECTION("Delete from end")
    {
        buffer.eraseTrailingCharacters(5);
        REQUIRE(buffer.getString() == "word1");
    }

    SECTION("Delete from index")
    {
        buffer.eraseCharacters(5, 3);
        REQUIRE(buffer.getString() == "word1rd2");
    }

    SECTION("Delete past 0")
    {

    }
}

TEST_CASE("TextBuffer basic operations")
{

}

TEST_CASE("TextBuffer buffering operations")
{

}